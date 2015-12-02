#include "../../../inc/slib/p2p/hns_client.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/pointer.h"
#include "../../../inc/slib/core/log.h"

#include "../../../inc/slib/crypto/aes.h"
#include "../../../inc/slib/crypto/rsa.h"
#include "../../../inc/slib/crypto/sha2.h"

#define PACKET_SIZE (SLIB_P2P_PACKET_MAX_SIZE + 1000)

SLIB_P2P_NAMESPACE_BEGIN
class _HnsClientUdp_Impl : public HnsClient
{
private:
	sl_bool m_flagRunning;
	
	char m_iv[16];
	char m_serviceSecret[32];
	char m_sessionSecret[32];

	sl_uint64 m_sessionId;

	Time m_timeLastKeepAliveSent;
	sl_int32 m_lastKeepAliveSentNumber;

	Time m_timeLastServiceTime;
	Time m_timeClientOnLastServiceTime;

	SLIB_INLINE _HnsClientUdp_Impl()
	{
		m_flagRunning = sl_true;

		Math::randomMemory(m_iv, 16);

		m_timeLastCommunicated.setZero();

		m_sessionId = 0;
		m_timeLastKeepAliveSent.setZero();
		m_lastKeepAliveSentNumber = 0;

		m_timeLastServiceTime.setZero();
		m_timeClientOnLastServiceTime.setZero();
	}

	static void logError(String err)
	{
		SLIB_LOG("HnsClientUdp", err);
	}

public:
	~_HnsClientUdp_Impl()
	{
		release();
	}

	static Ref<_HnsClientUdp_Impl> create(const HnsClientParam& param)
	{
		Ref<_HnsClientUdp_Impl> ret;

		sl_uint32 servicePubKeyLen = param.servicePublicKey.getLength();
		if (servicePubKeyLen < SLIB_P2P_RSA_KEY_MIN_SIZE || servicePubKeyLen > SLIB_P2P_RSA_KEY_MAX_SIZE) {
			logError("Create - Service RSA Key length can be between 1024~4096 bits");
			return ret;
		}
		
		ret = new _HnsClientUdp_Impl();

		if (ret.isNotNull()) {
			ret->m_socket = param.socket;
			ret->m_listener = param.listener;
			
			ret->m_name = param.name;
			ret->m_serviceAddress = param.serviceAddress;
			ret->m_servicePublicKey = param.servicePublicKey;
			SHA256::hash(param.serviceSecret, ret->m_serviceSecret);

			ret->m_clientId = param.clientId;
			ret->m_clientCertificate = param.clientCertificate;

			ret->m_connectionTimeoutMilliseconds = param.connectionTimeoutMilliseconds;
			ret->m_keepAliveIntervalMilliseconds = param.keepAliveIntervalMilliseconds;

			ret->m_sessionId = param.sessionId;
			SHA256::hash(param.sessionSecret, ret->m_sessionSecret);
		}
		return ret;
	}

	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		m_sessionId = 0;
	}

	sl_bool isRunning()
	{
		return m_flagRunning;
	}

	sl_bool processReceivedPacket(const SocketAddress& address, sl_uint32 methodId, const void* data, sl_uint32 size)
	{
		if (!m_flagRunning) {
			return sl_false;
		}
		if (address != getServiceAddress()) {
			return sl_false;
		}
		
		if (methodId == 2) {
			_receive_L1_initializeSessionResponse(address, (char*)data, size);
		} else if (methodId == 4) {
			_receive_L1_sendDataFromServiceToClient(address, (char*)data, size);
		} else if (methodId == 5) {
			// error - reconnect
			reconnect();
		}
		return sl_true;
	}

	void runStep()
	{
		Time now = Time::now();
		sl_int64 t = (now - m_timeLastKeepAliveSent).getMillisecondsCount();
		if (t < 0 || (sl_uint64)t > (sl_uint64)(m_keepAliveIntervalMilliseconds)) {
			connect();
			m_timeLastKeepAliveSent = now;
		}
	}

	sl_bool isConnected()
	{
		if (!m_flagRunning) {
			return sl_false;
		}
		if (m_sessionId == 0) {
			return sl_false;
		}
		sl_int64 millis = (Time::now() - m_timeLastCommunicated).getMillisecondsCount();
		if (millis >= 0 && (sl_uint64)millis < (sl_uint64)(m_connectionTimeoutMilliseconds)) {
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void connect()
	{
		if (!m_flagRunning) {
			return;
		}
		if (isConnected()) {
			_send_L2_keepAlive();
		} else {
			_send_L1_initializeSession();
		}
	}

	void reconnect()
	{
		if (!m_flagRunning) {
			return;
		}
		_send_L1_initializeSession();
	}

	void queryHost(String targetId)
	{
		if (!isConnected()) {
			return;
		}
		_send_L2_queryHost(targetId);
	}

	void broadcastMessage(sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		if (n > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		if (!isConnected()) {
			return;
		}
		_send_L2_broadcastMessage(messageId, data, n);
	}

	void sendMessage(String targetId, sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		if (n > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		if (! isConnected()) {
			return;
		}
		_send_L2_sendMessage(targetId, messageId, data, n);
	}

	Time getServiceTime()
	{
		if (m_timeClientOnLastServiceTime.isZero()) {
			return Time::zero();
		} else {
			return m_timeLastServiceTime + (Time::now() - m_timeClientOnLastServiceTime);
		}
	}

	void _write_L1_method(MemoryWriter& writer, sl_uint32 method)
	{
		sl_uint32 nonce = (sl_uint32)(Time::now().toInt());
		writer.writeUint32((nonce << 4) | method);
	}

	void _send_L1_initializeSession()
	{
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);
		// Hash
		writer.seek(4);
		// MethodID & Nonce
		_write_L1_method(writer, 1);

		// Body
		{
			char bufBody[PACKET_SIZE];
			MemoryWriter writerBody(bufBody, PACKET_SIZE);
			writerBody.writeString(getClientId(), SLIB_P2P_HOSTID_MAX_SIZE);
			writerBody.writeString(getClientCertificate(), SLIB_P2P_CERTIFICATE_MAX_SIZE);
			AES aes;
			aes.setKey(m_serviceSecret, 32);
			sl_size size = aes.encrypt_CBC_PKCS7Padding(bufBody, writerBody.getOffset(), bufPacket + writer.getOffset());
			writer.seek(size);
		}

		sl_int32 n = (sl_int32)(writer.getOffset());
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, n - 4));

		Ref<Socket> socket = getSocket();
		if (socket.isNotNull()) {
			sl_int32 m = socket->sendTo(bufPacket, n, getServiceAddress());
			if (m < 0) {
				logError("Initialize Session - Can not send the packet");
			}
		}
	}

	void _receive_L1_initializeSessionResponse(SocketAddress address, char* data, sl_size size)
	{
		if (!m_flagRunning) {
			return;
		}		
		MemoryReader reader(data, size);
		// Body
		{
			sl_size size = reader.getLength() - reader.getOffset();
			AES aes;
			aes.setKey(m_sessionSecret, 32);
			char bufBody[PACKET_SIZE];
			size = aes.decrypt_CBC_PKCS7Padding(reader.getBuffer() + reader.getOffset(), size, bufBody);
			if (size == 0) {
				return;
			}

			MemoryReader readerBody(bufBody, size);
			if (!readerBody.readUint64(&m_sessionId)) {
				return;
			}

			String strSocketAddress;
			if (!readerBody.readString(&strSocketAddress, SLIB_P2P_ADDRESS_MAX_SIZE)) {
				return;
			}
			m_publicAddress.parse(strSocketAddress);
		}

		m_timeLastCommunicated = Time::now();

		PtrLocker<IHnsClientListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onConnected(this);		
		}
	}

	sl_bool _send_L1_sendDataFromClientToService(const void* data, sl_uint32 sizeData)
	{
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);
		// Hash
		writer.seek(4);
		// MethodID & Nonce
		_write_L1_method(writer, 3);

		writer.writeUint64(m_sessionId);

		// Body-Section
		{
			char bufBody[PACKET_SIZE];
			MemoryWriter writerBody(bufBody, PACKET_SIZE);
			writerBody.writeUint32(SHA256::make32bitChecksum(data, sizeData));
			writerBody.write(data, sizeData);
			AES aes;
			aes.setKey(m_sessionSecret, 32);
			writer.write(m_iv, 16);
			sl_size size = aes.encrypt_CBC_PKCS7Padding(m_iv, bufBody, writerBody.getOffset(), bufPacket + writer.getOffset());
			Base::copyMemory(m_iv, bufPacket + writer.getOffset(), 16);
			writer.seek(size);
		}
		
		sl_int32 n = (sl_int32)(writer.getOffset());
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, n - 4));

		Ref<Socket> socket = getSocket();
		if (socket.isNotNull()) {
			sl_int32 m = getSocket()->sendTo(bufPacket, n, getServiceAddress());
			if (m < 0) {
				return sl_false;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void _receive_L1_sendDataFromServiceToClient(SocketAddress address, char* data, sl_size sizeData)
	{
		if (!m_flagRunning) {
			return;
		};
		if (!isConnected()) {
			return;
		};

		// Body
		{
			char bufBody[PACKET_SIZE];
			AES aes;
			aes.setKey(m_sessionSecret, 32);
			sl_uint32 size = (sl_uint32)(aes.decrypt_CBC_PKCS7Padding(data, sizeData, bufBody));
			if (size <= 4) {
				return;
			}
			char* bufL2 = bufBody + 4;
			sl_uint32 sizeL2 = size - 4;
			if (MIO::readUint32(bufBody) != SHA256::make32bitChecksum(bufL2, sizeL2)) {
				return;
			}
			m_timeLastCommunicated = Time::now();
			_process_L1_sendDataFromServiceToClient(bufL2, sizeL2);
		}
	}

	void _process_L1_sendDataFromServiceToClient(const void* data, sl_uint32 sizeData)
	{
		if (sizeData <= 1) {
			return;
		}
		char* cdata = (char*)data;
		sl_uint32 method = cdata[0];
		cdata++;
		sizeData--;
		switch (method) {
		case 2:
			_receive_L2_keepAliveResponse(cdata, sizeData);
			break;
		case 4:
			_receive_L2_queryHostResponse(cdata, sizeData);
			break;
		case 12:
			_receive_L2_sendMessageToClient(cdata, sizeData);
			break;
		case 14:
			_receive_L2_broadcastMessageToClient(cdata, sizeData);
			break;
		}
	}

	sl_bool _send_L2_keepAlive()
	{
		char bufPacket[256];
		MemoryWriter writer(bufPacket, 256);

		// method
		writer.writeInt8(1);
		// request Number
		sl_int32 reqNum = Base::interlockedIncrement32(&m_lastKeepAliveSentNumber);
		writer.writeInt32(reqNum);
		// time
		writer.writeTime(Time::now());

		return _send_L1_sendDataFromClientToService(bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_L2_keepAliveResponse(const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		sl_int32 reqNum;
		if (!reader.readInt32(&reqNum)) {
			return;
		}
		// client time
		Time reqTime;
		if (!reader.readTime(&reqTime)) {
			return;
		}
		// service time
		Time serviceTime;
		if (!reader.readTime(&serviceTime)) {
			return;
		}
		// public address
		String strPublicAddress;
		if (!reader.readString(&strPublicAddress, SLIB_P2P_ADDRESS_MAX_SIZE)) {
			return;
		}
		m_publicAddress.parse(strPublicAddress);
		m_timeLastServiceTime = serviceTime;
		m_timeClientOnLastServiceTime = Time::now();

		PtrLocker<IHnsClientListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onKeepAlive(this, serviceTime, (m_timeClientOnLastServiceTime - reqTime).getMillisecondsCount());
		}
	}

	void _send_L2_queryHost(String hostId)
	{
		char bufPacket[256];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(3);
		// hostId
		writer.writeString(hostId, SLIB_P2P_HOSTID_MAX_SIZE);

		_send_L1_sendDataFromClientToService(bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_L2_queryHostResponse(const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);

		HnsHostInfo info;
		if (!reader.readString(&(info.clientId), SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		String addr;
		if (!reader.readString(&addr, SLIB_P2P_ADDRESS_MAX_SIZE)) {
			return;
		}
		if (addr.isNotEmpty()) {
			info.clientAddress.parse(addr);
			if (!reader.readUint64(&(info.elapsedMillisecondsFromLastCommunicated))) {
				return;
			}
		}
		PtrLocker<IHnsClientListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onQueryHost(this, info);
		}
	}

	sl_bool _send_L2_sendMessage(String targetId, sl_uint64 messageId, const void* data, sl_uint32 sizeData)
	{
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(11);
		// targetHostId
		writer.writeString(targetId, SLIB_P2P_HOSTID_MAX_SIZE);
		// messageId
		writer.writeUint64(messageId);
		// content
		writer.write(data, sizeData);

		return _send_L1_sendDataFromClientToService(bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_L2_sendMessageToClient(const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		String senderId;
		if (!reader.readString(&senderId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		sl_uint64 messageId;
		if (!reader.readUint64(&messageId)) {
			return;
		}
		char* dataMessage = (char*)data + reader.getOffset();
		sl_uint32 lenMessage = sizeData - (sl_uint32)(reader.getOffset());
		PtrLocker<IHnsClientListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onMessage(this, senderId, messageId, dataMessage, lenMessage);
		}
	}

	void _send_L2_broadcastMessage(sl_uint64 messageId, const void* data, sl_uint32 sizeData)
	{
		if (!m_flagRunning) {
			return;
		}
		if (sizeData > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(13);
		// messageId
		writer.writeUint64(messageId);
		// content
		writer.write(data, sizeData);

		_send_L1_sendDataFromClientToService(bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_L2_broadcastMessageToClient(const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		String senderId;
		if (!reader.readString(&senderId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		sl_uint64 messageId;
		if (!reader.readUint64(&messageId)) {
			return;
		}
		char* dataMessage = (char*)data + reader.getOffset();
		sl_uint32 lenMessage = sizeData - (sl_uint32)(reader.getOffset());
		PtrLocker<IHnsClientListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onBroadcastMessage(this, senderId, messageId, dataMessage, lenMessage);
		}
	}

};

Ref<HnsClient> HnsClient::createUdp(const HnsClientParam& param)
{
	return Ref<HnsClient>::from(_HnsClientUdp_Impl::create(param));
}

SLIB_P2P_NAMESPACE_END
