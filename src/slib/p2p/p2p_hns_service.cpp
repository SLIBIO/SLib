#include "../../../inc/slib/p2p/hns_service.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/pointer.h"
#include "../../../inc/slib/core/log.h"

#include "../../../inc/slib/crypto/aes.h"
#include "../../../inc/slib/crypto/rsa.h"
#include "../../../inc/slib/crypto/sha2.h"

#define PACKET_SIZE (SLIB_P2P_PACKET_MAX_SIZE + 1000)

SLIB_P2P_NAMESPACE_BEGIN
class _HnsService_Impl : public HnsService
{
public:
	class _Session : public HnsServiceSession
	{
	public:
		_Session()
		{
		}
	};

private:
	sl_bool m_flagRunning;
	Ref<Socket> m_socketUdp;
	Ref<Thread> m_threadUdp;

	char m_secretService[32];

	char m_iv[16];

	RSAPrivateKey m_privateKey;

	Map< sl_uint64, Ref<_Session> > m_sessions;
	Map< String, Ref<_Session> > m_mapClientSessions;

	SLIB_INLINE _HnsService_Impl()
	{
		m_flagRunning = sl_true;
		m_sessions.initTree();
		m_mapClientSessions.initTree();
		Math::randomMemory(m_iv, 16);
	}

	static void logError(String err)
	{
		SLIB_LOG("HnsService", err);
	}

public:
	~_HnsService_Impl()
	{
		release();
	}

	static Ref<_HnsService_Impl> start(const HnsServiceParam& param)
	{
		Ref<_HnsService_Impl> ret;

		sl_uint32 servicePubKeyLen = param.privateKey.getLength();
		if (servicePubKeyLen < SLIB_P2P_RSA_KEY_MIN_SIZE || servicePubKeyLen > SLIB_P2P_RSA_KEY_MAX_SIZE) {
			logError("Start - Service RSA Key length can be between 1024~4096 bits");
			return ret;
		}

		Ref<Socket> socketUdp = Socket::openUdp_IPv4();
		if (socketUdp.isNotNull()) {
			socketUdp->setNonBlockingMode(sl_true);
			if (socketUdp->bind(SocketAddress(param.portUdp))) {				
				ret = new _HnsService_Impl();
				if (ret.isNotNull()) {					
					ret->m_privateKey = param.privateKey;
					SHA256::hash(param.secret, ret->m_secretService);
					ret->setListener(param.listener);
					ret->m_socketUdp = socketUdp;
					ret->m_threadUdp = Thread::start(SLIB_CALLBACK_REF(_HnsService_Impl, runUdpReceiver, ret));
					return ret;
				}
			} else {
				logError(String("Can not bind to udp port - ") + param.portUdp);
			}
			socketUdp->close();
		} else {
			logError("Can not open udp socket");
		}
		return ret;
	}

	void runUdpReceiver()
	{
		Ref<SocketEvent> ev = SocketEvent::createRead(m_socketUdp);
		if (ev.isNull()) {
			return;
		}
		while (Thread::isNotStoppingCurrent()) {
			SocketAddress address;
			char bufPacket[PACKET_SIZE];
			sl_int32 n = m_socketUdp->receiveFrom(bufPacket, PACKET_SIZE, address);
			if (!m_flagRunning) {
				return;
			}
			if (n > 0) {
				if (n >= 8) {
					sl_uint32 h = SHA256::make32bitChecksum(bufPacket + 4, n - 4);
					if (h == MIO::readUint32(bufPacket)) {
						sl_uint32 method = bufPacket[4] & 15;
						if (method == 1) {
							Memory mem = Memory::create(bufPacket + 8, n - 8);
							Thread::start(SLIB_CALLBACK_CLASS(_HnsService_Impl, _udp_receive_L1_initializeSession, this, address, mem));
						} else if (method == 3) {
							_udp_receive_L1_sendDataFromClientToService(address, bufPacket + 8, n - 8);
						}
					}
				}
			} else {
				ev->wait();
			}
		}
	}

	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		if (m_threadUdp.isNotNull()) {
			m_threadUdp->finishAndWait();
			m_threadUdp.setNull();
		}
		if (m_socketUdp.isNotNull()) {
			m_socketUdp->close();
		}
	}

	sl_bool isRunning()
	{
		return m_flagRunning;
	}

	Ref<HnsServiceSession> getClientSession(String clientId)
	{
		Ref<_Session> clientSession;
		m_mapClientSessions.get(clientId, &clientSession);
		if (clientSession.isNotNull()) {
			return Ref<HnsServiceSession>::from(clientSession);
		} else {
			return Ref<HnsServiceSession>::null();
		}
	}

	Mutex m_mutexSessionId;
	sl_uint64 _generateNewSessionId()
	{
		static sl_uint64 sessionId = 0;
		MutexLocker lockSessionId(&m_mutexSessionId);
		if (sessionId == 0) {
			sessionId = Time::now().getMillisecondsCount();
		}
		sessionId++;
		return sessionId;
	}

	void _udp_write_L1_method(MemoryWriter& writer, sl_uint32 method)
	{
		sl_uint32 nonce = (sl_uint32)(Time::now().toInt());
		writer.writeUint32((nonce << 4) | method);
	}

	void _udp_receive_L1_initializeSession(SocketAddress address, Memory dataPacket)
	{
		if (!m_flagRunning) {
			return;
		}
		MemoryReader reader(dataPacket);
		HnsServiceUdpSessionInfo info;
		// Body
		{
			sl_size size = reader.getLength() - reader.getOffset();
			AES aes;
			aes.setKey(m_secretService, 32);
			char bufBody[PACKET_SIZE];
			size = aes.decrypt_CBC_PKCS7Padding(reader.getBuffer() + reader.getOffset(), size, bufBody);
			if (size == 0) {
				return;
			}
			MemoryReader readerBody(bufBody, size);
			if (!readerBody.readString(&(info.clientId), SLIB_P2P_HOSTID_MAX_SIZE)) {
				return;
			}
			if (!readerBody.readString(&(info.clientCertificate), SLIB_P2P_CERTIFICATE_MAX_SIZE)) {
				return;
			}
			info.clientAddress = address;
		}

		PtrLocker<IHnsServiceListener> listener(getListener());
		if (listener.isNotNull()) {
			if (!listener->onAuthenticateUdp(this, info)) {
				return;
			}
		}

		if (!m_flagRunning) {
			return;
		}
		Ref<_Session> clientSession;
		m_mapClientSessions.get(info.clientId, &clientSession);

		if (clientSession.isNull()) {
			clientSession = new _Session;
			if (clientSession.isNull()) {
				return;
			}
			clientSession->clientId = info.clientId;
			clientSession->sessionId = _generateNewSessionId();
			clientSession->udpSessionAddress = info.clientAddress;
			Base::copyMemory(clientSession->secret, m_secretService, 32);

			m_sessions.put(clientSession->sessionId, clientSession);
			m_mapClientSessions.put(info.clientId, clientSession);
		} else {
			clientSession->udpSessionAddress = info.clientAddress;
		}
		
		clientSession->timeLastCommunicatedUdp = Time::now();

		_udp_send_L1_initializeSessionResponse(clientSession);
	}

	void _udp_send_L1_initializeSessionResponse(Ref<_Session> clientSession)
	{
		if (!m_flagRunning) {
			return;
		}
		if (clientSession.isNull()) {
			return;
		}
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// Hash
		writer.seek(4);
		// MethodID & Nonce
		_udp_write_L1_method(writer, 2);

		// Body
		{
			char bufBody[PACKET_SIZE];
			MemoryWriter writerBody(bufBody, PACKET_SIZE);
			writerBody.writeUint64(clientSession->sessionId);
			writerBody.writeString(clientSession->udpSessionAddress.toString());
			AES aes;
			aes.setKey(clientSession->secret, 32);
			sl_size size = aes.encrypt_CBC_PKCS7Padding(bufBody, writerBody.getOffset(), bufPacket + writer.getOffset());
			writer.seek(size);
		}

		sl_int32 n = (sl_int32)(writer.getOffset());
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, n - 4));

		sl_int32 m = m_socketUdp->sendTo(bufPacket, n, clientSession->udpSessionAddress);
		if (m < 0) {
			logError("Initialize Session Response - Can not send the packet");
		}
	}

	void _udp_receive_L1_sendDataFromClientToService(SocketAddress address, char* data, sl_size sizeData)
	{
		if (!m_flagRunning) {
			return;
		};

		MemoryReader reader(data, sizeData);

		sl_uint64 sessionId;
		Ref<_Session> session;

		if (!reader.readUint64(&sessionId)) {
			return;
		}
		m_sessions.get(sessionId, &session);
		if (session.isNull()) {
			return;
		}
		
		// Body
		{
			char bufBody[PACKET_SIZE];
			AES aes;
			aes.setKey(session->secret, 32);
			sl_uint32 size = (sl_uint32)(aes.decrypt_CBC_PKCS7Padding(data + reader.getOffset(), reader.getLength() - reader.getOffset(), bufBody));
			if (size <= 4) {
				return;
			}
			char* bufL2 = bufBody + 4;
			sl_uint32 sizeL2 = size - 4;
			if (MIO::readUint32(bufBody) != SHA256::make32bitChecksum(bufL2, sizeL2)) {
				_udp_send_L1_error(session);
				return;
			}
			
			session->udpSessionAddress = address;
			session->timeLastCommunicatedUdp = Time::now();

			_udp_process_L1_sendDataFromClientToService(session, bufL2, sizeL2);
		}
	}

	void _udp_send_L1_sendDataFromServiceToClient(Ref<_Session> session, const void* data, sl_uint32 sizeData)
	{
		if (!m_flagRunning) {
			return;
		}

		if (session.isNull()) {
			return;
		}

		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);
		// Hash
		writer.seek(4);
		// MethodID & Nonce
		_udp_write_L1_method(writer, 4);

		// Body-Section
		{
			char bufBody[PACKET_SIZE];
			MemoryWriter writerBody(bufBody, PACKET_SIZE);
			writerBody.writeUint32(SHA256::make32bitChecksum(data, sizeData));
			writerBody.write(data, sizeData);
			AES aes;
			aes.setKey(session->secret, 32);
			writer.write(m_iv, 16);
			sl_size size = aes.encrypt_CBC_PKCS7Padding(m_iv, bufBody, writerBody.getOffset(), bufPacket + writer.getOffset());
			Base::copyMemory(m_iv, bufPacket + writer.getOffset(), 16);
			writer.seek(size);
		}
		
		sl_int32 n = (sl_int32)(writer.getOffset());
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, n - 4));

		m_socketUdp->sendTo(bufPacket, n, session->udpSessionAddress);
	}

	void _udp_send_L1_error(Ref<_Session> session)
	{
		if (!m_flagRunning) {
			return;
		}
		if (session.isNull()) {
			return;
		}

		char bufPacket[8];
		MemoryWriter writer(bufPacket, 8);
		// Hash
		writer.seek(4);
		// MethodID & Nonce
		_udp_write_L1_method(writer, 5);

		sl_int32 n = (sl_int32)(writer.getOffset());
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, n - 4));

		m_socketUdp->sendTo(bufPacket, n, session->udpSessionAddress);
	}

	void _udp_process_L1_sendDataFromClientToService(Ref<_Session> session, const void* data, sl_uint32 sizeData)
	{
		if (sizeData <= 1) {
			return;
		}
		char* cdata = (char*)data;
		sl_uint32 method = cdata[0];
		cdata++;
		sizeData--;
		switch (method) {
		case 1:
			_udp_receive_L2_keepAlive(session, cdata, sizeData);
			break;
		case 3:
			_udp_receive_L2_queryHost(session, cdata, sizeData);
			break;
		case 11:
			_udp_receive_L2_sendMessage(session, cdata, sizeData);
			break;
		case 13:
			_udp_receive_L2_broadcastMessage(session, cdata, sizeData);
			break;
		}
	}

	void _udp_receive_L2_keepAlive(Ref<_Session> session, const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		sl_int32 reqNum;
		if (!reader.readInt32(&reqNum)) {
			return;
		}
		Time reqTime;
		if (!reader.readTime(&reqTime)) {
			return;
		}
		PtrLocker<IHnsServiceListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onKeepAliveUdp(this, session.getObject(), reqTime);
		}
		_udp_send_L2_keepAliveResponse(session, reqNum, reqTime);
	}

	void _udp_send_L2_keepAliveResponse(Ref<_Session> session, sl_int32 reqNum, const Time& reqTime)
	{
		if (!m_flagRunning) {
			return;
		}
		if (session.isNull()) {
			return;
		}
		char bufPacket[256];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(2);
		// request Number
		writer.writeInt32(reqNum);
		// client time
		writer.writeTime(reqTime);
		// service time
		writer.writeTime(Time::now());
		// client address
		writer.writeString(session->udpSessionAddress.toString());

		_udp_send_L1_sendDataFromServiceToClient(session, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _udp_receive_L2_queryHost(Ref<_Session> session, const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		
		String hostId;
		if (!reader.readString(&hostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		Ref<_Session> target;
		m_mapClientSessions.get(hostId, &target);
		if (target.isNull()) {
			_udp_send_L2_queryHostResponseFailed(session, hostId);
		} else {
			PtrLocker<IHnsServiceListener> listener(getListener());
			if (listener.isNotNull()) {
				if (!listener->onQueryHostUdp(this, session.getObject(), target.getObject())) {
					return;
				}
			}
			_udp_send_L2_queryHostResponse(session, target);
		}
	}

	void _udp_send_L2_queryHostResponse(Ref<_Session> session, Ref<_Session> target)
	{
		if (!m_flagRunning) {
			return;
		}
		if (session.isNull()) {
			return;
		}

		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(4);
		// hostId
		writer.writeString(target->clientId, SLIB_P2P_HOSTID_MAX_SIZE);
		// address
		writer.writeString(target->udpSessionAddress.toString());
		// elapsedMillisecondsFromLastCommunicated
		writer.writeUint64((Time::now() - target->timeLastCommunicatedUdp).getMillisecondsCount());

		_udp_send_L1_sendDataFromServiceToClient(session, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _udp_send_L2_queryHostResponseFailed(Ref<_Session> session, String targetId)
	{
		if (!m_flagRunning) {
			return;
		}
		if (session.isNull()) {
			return;
		}

		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(4);
		// hostId
		writer.writeString(targetId, SLIB_P2P_HOSTID_MAX_SIZE);
		// address
		writer.writeString(String::null());

		_udp_send_L1_sendDataFromServiceToClient(session, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _udp_receive_L2_sendMessage(Ref<_Session> senderClientSession, const void* data, sl_uint32 sizeData)
	{
		MemoryReader reader(data, sizeData);
		String targetId;
		if (!reader.readString(&targetId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		Ref<_Session> targetClientSession;
		m_mapClientSessions.get(targetId, &targetClientSession);
		if (targetClientSession.isNotNull()) {
			sl_uint64 messageId;
			if (!reader.readUint64(&messageId)) {
				return;
			}
			char* dataMessage = (char*)data + reader.getOffset();
			sl_uint32 lenMessage = sizeData - (sl_uint32)(reader.getOffset());

			PtrLocker<IHnsServiceListener> listener(getListener());
			if (listener.isNotNull()) {
				if (!listener->onMessageUdp(this
					, senderClientSession.getObject()
					, targetClientSession.getObject()
					, messageId, dataMessage, lenMessage)) {
					return;
				}
			}
			_udp_send_L2_sendMessageToClient(targetClientSession, senderClientSession->clientId, messageId, dataMessage, lenMessage);
		}
	}

	void _udp_send_L2_sendMessageToClient(Ref<_Session> targetClientSession, String senderId, sl_uint64 messageId, const void* data, sl_uint32 sizeData)
	{
		if (!m_flagRunning) {
			return;
		}
		if (targetClientSession.isNull()) {
			return;
		}

		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// method
		writer.writeInt8(12);
		// senderHostId
		writer.writeString(senderId, SLIB_P2P_HOSTID_MAX_SIZE);
		// messageId
		writer.writeUint64(messageId);
		// content
		writer.write(data, sizeData);

		_udp_send_L1_sendDataFromServiceToClient(targetClientSession, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _udp_receive_L2_broadcastMessage(Ref<_Session> session, const void* data, sl_uint32 sizeData)
	{
		if (sizeData < 8) {
			return;
		}
		sl_uint64 messageId = MIO::readUint64(data);
		PtrLocker<IHnsServiceListener> listener(getListener());
		if (listener.isNotNull()) {
			if (!(listener->onBroadcastMessageUdp(this, session.getObject(), messageId, ((char*)data) + 8, sizeData - 8))) {
				return;
			}
		}
		String senderId = session->clientId;
		Memory mem = Memory::create(data, sizeData);
		Thread::start(SLIB_CALLBACK_REF(_HnsService_Impl, _udp_send_L2_relayBroadcastMessage, this, senderId, mem));
	}

	void _udp_send_L2_relayBroadcastMessage(String senderId, Memory packetContent)
	{
		if (!m_flagRunning) {
			return;
		}
		
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);
		// method
		writer.writeUint8(14);
		// senderId
		writer.writeString(senderId);
		// messageId, content
		writer.write(packetContent.getBuf(), packetContent.getSize());

		sl_uint32 size = (sl_uint32)(writer.getOffset());

		ListLocker< Ref<_Session> > sessions(m_mapClientSessions.values());
		for (sl_size i = 0; i < sessions.count(); i++) {
			if (sessions[i].isNotNull() && sessions[i]->clientId != senderId) {
				_udp_send_L1_sendDataFromServiceToClient(sessions[i], bufPacket, size);
			}
		}
	}

};

Ref<HnsService> HnsService::start(const HnsServiceParam& param)
{
	return Ref<HnsService>::from(_HnsService_Impl::start(param));
}

SLIB_P2P_NAMESPACE_END
