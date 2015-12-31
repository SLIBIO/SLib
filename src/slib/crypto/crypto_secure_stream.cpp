#include "../../../inc/slib/crypto/secure_stream.h"
#include "../../../inc/slib/crypto/aes.h"
#include "../../../inc/slib/core/mio.h"

#define RSA_MIN_LEN 256
#define RSA_MAX_LEN 1024
#define RDATA_SIZE 2048
#define SDATA_SIZE 64
#define SEGMENT_SIZE 4096
#define OAEP_LABEL "SecureStream"
#define OAEP_LABEL_LEN 12

SLIB_CRYPTO_NAMESPACE_BEGIN
void AsyncSecureStream::onConnected(sl_bool flagError)
{
	PtrLocker<IAsyncSecureStreamListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onConnectedSecureStream(this, flagError);
	}
}

SecureStreamServerParam::SecureStreamServerParam()
{
	privateKey = SecureStream::getDefaultPrivateKey();
}

sl_bool SecureStreamServerParam::checkParam() const
{
	if (privateKey.getLength() < RSA_MIN_LEN || privateKey.getLength() > RSA_MAX_LEN) {
		return sl_false;
	}
	return sl_true;
}

class _SecureStreamServer_Context
{
private:
	SecureStreamServerParam m_param;
	sl_bool m_flagConnected;
	
	AES m_cipher;

	sl_uint8 m_readIV[16];
	sl_uint64 m_sizeRead;
	sl_uint8 m_writeIV[16];
	sl_uint64 m_sizeWrite;

public:
	_SecureStreamServer_Context()
	{
		m_flagConnected = sl_false;
	}

	sl_bool init(const SecureStreamServerParam& param)
	{
		if (!(param.checkParam())) {
			return sl_false;
		}
		m_param = param;
		m_flagConnected = sl_false;
		m_sizeRead = 0;
		m_sizeWrite = 0; 
		return sl_true;
	}

	sl_int32 processConnect(void* _rdata, sl_uint32 lenRdata, void* _sdata)
	{
		sl_uint8* rdata = (sl_uint8*)_rdata;
		sl_uint8* sdata = (sl_uint8*)_sdata;
		if (lenRdata < 16) {
			return 0;
		}
		AES aesPlain;
		sl_uint8 plain[16];
		aesPlain.setKey_SHA256(m_param.masterKey + OAEP_LABEL);
		aesPlain.decryptBlock(rdata, plain);
		sl_uint32 lenEnc = MIO::readUint16LE(plain + 14);
		if (lenEnc != 2 + m_param.privateKey.getLength()) {
			return -1;
		}
		if (lenRdata < 32 + lenEnc) {
			return 0;
		}
		AES_GCM aes;
		aes.setKey_SHA256(m_param.masterKey);
		if (! (aes.decrypt(plain, 14, plain, 16, rdata + 16, rdata + 16, lenEnc, rdata + 16 + lenEnc, 16))) {
			return -1;
		}
		sl_uint32 version = MIO::readUint16LE(rdata + 16);
		if (version != 1) {
			return -1;
		}
		sl_uint32 lenRSA = RSA::decryptPrivate_oaep_v21(m_param.privateKey, CryptoHash::sha256(), rdata + 18, rdata + 18, m_param.privateKey.getLength(), OAEP_LABEL, OAEP_LABEL_LEN);
		if (lenRSA < 40) {
			return -1;
		}
		sl_uint8* sessionKey = rdata + 18;;
		Base::copyMemory(m_readIV, rdata + 50, 8);

		// build send packet
		Math::randomMemory(sdata, 32);
		MIO::writeUint16LE(sdata + 16, version);
		Base::copyMemory(m_writeIV, sdata + 18, 8);

		aes.setKey(sessionKey, 32);
		if (!aes.encrypt(sdata, 16, sdata, 16, sdata + 16, sdata + 16, 32, sdata + 48, 16)) {
			return -1;
		}

		m_cipher.setKey(sessionKey, 32);
		
		m_flagConnected = sl_true;

		return 32 + lenEnc;
	}

	void processReceive(const void* input, sl_uint32 size, void* output)
	{
		m_cipher.encrypt_CTR(m_readIV, m_sizeRead, input, size, output);
		m_sizeRead += size;
	}

	void processSend(const void* input, sl_uint32 size, void* output)
	{
		m_cipher.encrypt_CTR(m_writeIV, m_sizeWrite, input, size, output);
		m_sizeWrite += size;
	}

	SLIB_INLINE sl_bool isConnected()
	{
		return m_flagConnected;
	}
};

class _SecureStreamServer_SecureStream : public SecureStream
{
public:
	Ptr<IStream> m_streamBase;
	_SecureStreamServer_Context m_context;

	sl_bool m_flagClosed;

	Memory m_rdata;
	sl_uint32 m_posRdata;
	sl_uint32 m_sizeRdata;
	Memory m_sdata;
	sl_uint32 m_posSdata;
	sl_uint32 m_sizeSdata;

	Mutex m_lockConnecting;
	Mutex m_lockReading;
	Mutex m_lockWriting;

public:
	_SecureStreamServer_SecureStream()
	{
		m_posRdata = 0;
		m_sizeRdata = 0;
		m_posSdata = 0;
		m_sizeSdata = 0;

		m_flagClosed = sl_false;
	}

	void _close()
	{
		m_streamBase.setNull();
	}

	sl_int32 _connect(IStream* stream)
	{
		if (m_context.isConnected()) {
			return 1;
		}
		MutexLocker lock(&m_lockConnecting);
		if (m_flagClosed) {
			return -1;
		}
		if (m_sizeRdata > RDATA_SIZE) {
			_close();
			return -1;
		}

		sl_uint8* bufRdata = (sl_uint8*)(m_rdata.getBuf());
		sl_uint8* bufSdata = (sl_uint8*)(m_sdata.getBuf());
		while (!(m_context.isConnected())) {
			sl_int32 n = stream->read32(bufRdata + m_sizeRdata, RDATA_SIZE - m_sizeRdata);
			if (n > 0) {
				m_sizeRdata += n;
				n = m_context.processConnect(bufRdata, m_sizeRdata, bufSdata);
				if (n < 0) {
					_close();
					return n;
				}
				if (n > 0) {
					m_posRdata = n;
					m_sizeRdata -= n;
					if (m_sizeRdata > 0) {
						m_context.processReceive(bufRdata + m_posRdata, m_sizeRdata, bufRdata + m_posRdata);
					}
					// send response
					{
						sl_int32 m = stream->write32(bufSdata, SDATA_SIZE);
						if (m < 0) {
							_close();
							return m;
						}
						m_posSdata = m;
						m_sizeSdata = SDATA_SIZE - m;
					}
					return n;
				}
			} else {
				if (n < 0) {
					_close();
				}
				return n;
			}
		}
		return 1;
	}
	
	sl_int32 connect()
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		return _connect(stream.getValue());
	}

	sl_bool isConnected()
	{
		if (m_flagClosed) {
			return sl_false;
		}
		return m_context.isConnected();
	}

	sl_int32 read32(void* _buf, sl_uint32 size)
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		sl_int32 n = _connect(stream.getValue());
		if (n <= 0) {
			return n;
		}
		MutexLocker lock(&m_lockReading);
		if (m_flagClosed) {
			return -1;
		}
		if (size == 0) {
			return 0;
		}
		sl_uint8* buf = (sl_uint8*)_buf;
		if (m_sizeRdata > 0) {
			sl_uint8* bufRdata = (sl_uint8*)(m_rdata.getBuf());
			if (size < m_sizeRdata) {
				n = size;
			} else {
				n = m_sizeRdata;
			}
			Base::copyMemory(buf, bufRdata + m_posRdata, n);
			m_sizeRdata -= n;
			m_posRdata += n;
			if (m_sizeRdata == 0) {
				m_rdata.setNull();
			}
			return n;
		}
		n = stream->read32(buf, size);
		if (n > 0) {
			m_context.processReceive(buf, n, buf);
		}
		return n;
	}

	sl_int32 write32(const void* _buf, sl_uint32 size)
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		sl_int32 n = _connect(stream.getValue());
		if (n <= 0) {
			return n;
		}
		MutexLocker lock(&m_lockWriting);
		if (m_flagClosed) {
			return -1;
		}
		if (size == 0) {
			return 0;
		}
		const sl_uint8* buf = (const sl_uint8*)_buf;
		sl_uint8* bufSdata = (sl_uint8*)(m_sdata.getBuf());
		while (0 < m_sizeSdata) {
			n = stream->write32(bufSdata + m_posSdata, m_sizeSdata);
			if (n > 0) {
				m_posSdata += n;
				m_sizeSdata -= n;
			} else {
				if (n < 0) {
					_close();
				}
				return n;
			}
		}
		m_sdata.setNull();

		sl_uint8 seg[SEGMENT_SIZE];
		sl_uint32 total = 0;
		while (size > 0) {
			sl_uint32 m = SEGMENT_SIZE;
			if (m > size) {
				m = size;
			}
			m_context.processSend(buf, m, seg);
			sl_int32 n = stream->write32(seg, m);
			if (n < 0) {
				return n;
			}
			if (n == 0) {
				break;
			}
			buf += n;
			size -= n;
			total += n;
		}
		return total;
	}

	void close()
	{
		MutexLocker lock1(&m_lockConnecting);
		MutexLocker lock2(&m_lockReading);
		MutexLocker lock3(&m_lockWriting);
		_close();
	}

public:
	static Ref<_SecureStreamServer_SecureStream> create(const Ptr<IStream>& _baseStream, const SecureStreamServerParam& param)
	{
		Ref<_SecureStreamServer_SecureStream> ret;
		Memory rdata = Memory::create(RDATA_SIZE);
		Memory sdata = Memory::create(SDATA_SIZE);
		if (rdata.isEmpty() || sdata.isEmpty()) {
			return ret;
		}
		Ptr<IStream> baseStream = _baseStream;
		if (baseStream.isNotNull()) {
			ret = new _SecureStreamServer_SecureStream;
			ret->m_streamBase = baseStream;
			ret->m_rdata = rdata;
			ret->m_sdata = sdata;
			if (ret->m_context.init(param)) {
				return ret;
			}
			ret.setNull();
		}
		return ret;
	}
};

Ref<SecureStream> SecureStreamServer::createStream(const Ptr<IStream>& baseStream, const SecureStreamServerParam& param)
{
	Ref<_SecureStreamServer_SecureStream> ret = _SecureStreamServer_SecureStream::create(baseStream, param);
	return Ref<SecureStream>::from(ret);
}

class _SecureStreamServer_AsyncStream : public AsyncSecureStream, public IAsyncStreamListener
{
public:
	Ref<AsyncStream> m_streamBase;
	_SecureStreamServer_Context m_context;

	Memory m_rdata;
	sl_uint32 m_posRdata;
	sl_uint32 m_sizeRdata;

	sl_bool m_flagInited;

public:
	_SecureStreamServer_AsyncStream()
	{
		m_posRdata = 0;
		m_sizeRdata = 0;
		m_flagInited = sl_false;
	}

	void _close()
	{
		m_streamBase.setNull();
	}

	sl_bool connect()
	{
		MutexLocker lock(getLocker());
		if (m_flagInited) {
			return sl_false;
		}
		m_flagInited = sl_true;
		WeakRef<_SecureStreamServer_AsyncStream> _this(this);
		if (m_streamBase->readToMemory(m_rdata, _this)) {
			return sl_true;
		}
		return sl_false;
	}

	sl_bool isConnected()
	{
		if (!m_flagInited) {
			return sl_false;
		}
		return m_context.isConnected();
	}

	void onRead(AsyncStream* _stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError)
	{
		MutexLocker lock(getLocker());
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNull()) {
			flagError = sl_true;
		}
		if (m_context.isConnected()) {
			if (!(AsyncStreamRequest::checkInstance(ref))) {
				return;
			}
			AsyncStreamRequest* req = (AsyncStreamRequest*)ref;
			if (sizeRead > 0) {
				m_context.processReceive(data, sizeRead, data);
			}
			lock.unlock();
			PtrLocker<IAsyncStreamListener> listener(req->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, data, sizeRead, req->refData.get(), flagError);
			}
		} else {
			if (flagError) {
				_close();
				lock.unlock();
				onConnected(sl_true);
			} else {
				if (sizeRead > 0) {
					char bufSend[SDATA_SIZE];
					m_sizeRdata += sizeRead;
					sl_int32 n = m_context.processConnect(m_rdata.getBuf(), m_sizeRdata, bufSend);
					if (n > 0) {
						m_posRdata = n;
						m_sizeRdata -= n;
						if (m_sizeRdata > 0) {
							sl_uint8* p = (sl_uint8*)(m_rdata.getBuf()) + m_posRdata;
							m_context.processReceive(p, m_sizeRdata, p);
						}
						
						// write first response
						{
							WeakRef<_SecureStreamServer_AsyncStream> _this(this);
							stream->writeFromMemory(Memory::create(bufSend, SDATA_SIZE), _this);
						}

						lock.unlock();
						onConnected(sl_false);
					} else {
						if (n < 0 || m_sizeRdata >= RDATA_SIZE) {
							_close();
							lock.unlock();
							onConnected(sl_true);
						} else {
							WeakRef<_SecureStreamServer_AsyncStream> _this(this);
							stream->read((sl_uint8*)(m_rdata.getBuf()) + m_sizeRdata, RDATA_SIZE - m_sizeRdata, _this, m_rdata.getReference());
						}
					}
				}
			}
		}
	}
	
	void onWrite(AsyncStream* _stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError)
	{
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNull()) {
			flagError = sl_true;
		}
		if (!(AsyncStreamRequest::checkInstance(ref))) {
			return;
		}
		AsyncStreamRequest* req = (AsyncStreamRequest*)ref;
		PtrLocker<IAsyncStreamListener> listener(req->listener);
		if (listener.isNotNull()) {
			listener->onWrite(this, data, sizeWritten, req->refData.get(), flagError);
		}
	}

	void processReadDone(Ref<AsyncStreamRequest> req)
	{
		if (req.isNotNull()) {
			PtrLocker<IAsyncStreamListener> listener(req->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, req->data, req->size, req->refData.get(), sl_false);
			}
		}
	}

	sl_bool _read(AsyncStreamRequest* req)
	{
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNotNull()) {
			if (m_context.isConnected()) {
				if (m_sizeRdata) {
					sl_uint32 n = m_sizeRdata;
					if (n > req->size) {
						n = req->size;
					}
					req->size = n;
					Base::copyMemory(req->data, (sl_uint8*)(m_rdata.getBuf()) + m_posRdata, n);
					m_sizeRdata -= n;
					m_posRdata += n;
					Ref<AsyncLoop> loop = stream->getLoop();
					if (loop.isNotNull()) {
						loop->addTask(SLIB_CALLBACK_WEAKREF(_SecureStreamServer_AsyncStream, processReadDone, this, Ref<AsyncStreamRequest>(req)), sl_false);
					}
					if (m_sizeRdata == 0) {
						m_rdata.setNull();
					}
				} else {
					return stream->read(req->data, req->size, WeakRef<_SecureStreamServer_AsyncStream>(this), req);
				}
			}
		}
		return sl_false;
	}

	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref)
	{
		MutexLocker lock(getLocker());
		if (!m_flagInited) {
			return sl_false;
		}
		Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, ref, listener);
		if (req.isNotNull()) {
			return _read(req.get());
		}
		return sl_false;
	}

	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref)
	{
		MutexLocker lock(getLocker());
		if (!m_flagInited) {
			return sl_false;
		}
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNotNull()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, ref, listener);
			if (req.isNotNull()) {
				if (m_context.isConnected()) {
					m_context.processSend(data, size, data);
					return stream->write(data, size, WeakRef<_SecureStreamServer_AsyncStream>(this), req.get());
				}
			}
		}
		return sl_false;
	}

	void close()
	{
		MutexLocker lock(getLocker());
		_close();
	}

	sl_bool isOpened()
	{
		return m_streamBase.isNotNull();
	}

public:
	static Ref<_SecureStreamServer_AsyncStream> create(const Ref<AsyncStream>& _baseStream, const AsyncSecureStreamServerParam& param, sl_bool flagConnect)
	{
		Ref<_SecureStreamServer_AsyncStream> ret;
		Memory rdata = Memory::create(RDATA_SIZE);
		if (rdata.isEmpty()) {
			return ret;
		}
		Ref<AsyncStream> baseStream = _baseStream;
		if (baseStream.isNotNull()) {
			Ref<AsyncLoop> loop = baseStream->getLoop();
			if (loop.isNotNull()) {
				ret = new _SecureStreamServer_AsyncStream;
				if (ret.isNotNull()) {
					ret->setLoop(baseStream->getLoop());
					ret->m_rdata = rdata;
					ret->m_streamBase = baseStream;
					ret->setListener(param.listener);
					if (ret->m_context.init(param)) {
						if (flagConnect) {
							if (ret->connect()) {
								return ret;
							}
						} else {
							return ret;
						}
					}
					ret.setNull();
				}
			}
		}
		return ret;
	}
};

Ref<AsyncSecureStream> SecureStreamServer::createAsyncStream(const Ref<AsyncStream>& baseStream, const AsyncSecureStreamServerParam& param, sl_bool flagConnect)
{
	Ref<_SecureStreamServer_AsyncStream> ret = _SecureStreamServer_AsyncStream::create(baseStream, param, flagConnect);
	return Ref<AsyncSecureStream>::from(ret);
}

SecureStreamClientParam::SecureStreamClientParam()
{
	publicKey = SecureStream::getDefaultPublicKey();
}

sl_bool SecureStreamClientParam::checkParam() const
{
	if (publicKey.getLength() < RSA_MIN_LEN || publicKey.getLength() > RSA_MAX_LEN) {
		return sl_false;
	}
	return sl_true;
}

class _SecureStreamClient_Context
{
private:
	SecureStreamClientParam m_param;
	sl_bool m_flagReceivedHeader;

	AES m_cipher;

	sl_uint8 m_readIV[16];
	sl_uint64 m_sizeRead;
	sl_uint8 m_writeIV[16];
	sl_uint64 m_sizeWrite;

public:
	_SecureStreamClient_Context()
	{
		m_flagReceivedHeader = sl_false;
	}

	sl_int32 init(const SecureStreamClientParam& param, void* _rdata)
	{
		if (!(param.checkParam())) {
			return -1;
		}
		m_param = param;
		m_flagReceivedHeader = sl_false;

		sl_uint8* rdata = (sl_uint8*)(_rdata);
		
		sl_uint32 lenEnc = 2 + m_param.publicKey.getLength();
		Math::randomMemory(rdata, 14);
		MIO::writeUint16LE(rdata + 14, lenEnc);

		MIO::writeUint16LE(rdata + 16, 1);
		
		Math::randomMemory(rdata + 18, 40);
		m_cipher.setKey(rdata + 18, 32);
		Base::copyMemory(m_writeIV, rdata + 50, 8);
		if (!(RSA::encryptPublic_oaep_v21(m_param.publicKey, CryptoHash::sha256(), rdata + 18, 40, rdata + 18, OAEP_LABEL, OAEP_LABEL_LEN))) {
			return -1;
		}

		AES_GCM aes;
		aes.setKey_SHA256(m_param.masterKey);
		if (!(aes.encrypt(rdata, 14, rdata, 16, rdata + 16, rdata + 16, lenEnc, rdata + 16 + lenEnc, 16))) {
			return -1;
		}

		AES aesPlain;
		aesPlain.setKey_SHA256(m_param.masterKey + OAEP_LABEL);
		aesPlain.encryptBlock(rdata, rdata);

		m_sizeRead = 0;
		m_sizeWrite = 0;

		return 32 + lenEnc;
	}

	sl_int32 receiveHeader(void* _sdata, sl_uint32 lenSdata)
	{
		sl_uint8* sdata = (sl_uint8*)_sdata;
		if (lenSdata < SDATA_SIZE) {
			return 0;
		}
		
		AES_GCM aes;
		aes.setCipher(&m_cipher);
		if (!(aes.decrypt(sdata, 16, sdata, 16, sdata + 16, sdata + 16, 32, sdata + 48, 16))) {
			return -1;
		}
		sl_uint32 version = MIO::readUint16LE(sdata + 16);
		if (version != 1) {
			return -1;
		}
		Base::copyMemory(m_readIV, sdata + 18, 8);

		m_flagReceivedHeader = sl_true;

		return SDATA_SIZE;
	}

	void processReceive(const void* input, sl_uint32 size, void* output)
	{
		m_cipher.encrypt_CTR(m_readIV, m_sizeRead, input, size, output);
		m_sizeRead += size;
	}

	void processSend(const void* input, sl_uint32 size, void* output)
	{
		m_cipher.encrypt_CTR(m_writeIV, m_sizeWrite, input, size, output);
		m_sizeWrite += size;
	}

	SLIB_INLINE sl_bool isReceivedHeader()
	{
		return m_flagReceivedHeader;
	}
};

class _SecureStreamClient_SecureStream : public SecureStream
{
public:
	Ptr<IStream> m_streamBase;
	_SecureStreamClient_Context m_context;

	sl_bool m_flagClosed;

	Memory m_rdata;
	sl_uint32 m_posRdata;
	sl_uint32 m_sizeRdata;
	Memory m_sdata;
	sl_uint32 m_posSdata;
	sl_uint32 m_sizeSdata;

	Mutex m_lockReading;
	Mutex m_lockWriting;

public:
	_SecureStreamClient_SecureStream()
	{
		m_posRdata = 0;
		m_sizeRdata = 0;
		m_posSdata = 0;
		m_sizeSdata = 0;

		m_flagClosed = sl_false;
	}

	void _close()
	{
		m_streamBase.setNull();
	}


	sl_bool init(const SecureStreamClientParam& param)
	{
		sl_int32 n = m_context.init(param, m_rdata.getBuf());
		if (n <= 0) {
			return sl_false;
		}
		m_sizeRdata = n;
		return sl_true;
	}

	sl_int32 _init(IStream* stream)
	{
		sl_uint8* bufRdata = (sl_uint8*)(m_rdata.getBuf());
		while (0 < m_sizeRdata) {
			sl_int32 n = stream->write32(bufRdata + m_posRdata, m_sizeRdata);
			if (n > 0) {
				m_posRdata += n;
				m_sizeRdata -= n;
			} else {
				if (n < 0) {
					_close();
				}
				return n;
			}
		}
		m_rdata.setNull();
		return 1;
	}

	sl_int32 _connect(IStream* stream)
	{
		if (m_context.isReceivedHeader()) {
			return 1;
		}
		if (m_sizeSdata > SDATA_SIZE) {
			_close();
			return -1;
		}
		{
			MutexLocker lock(&m_lockWriting);
			sl_int32 n = _init(stream);
			if (n <= 0) {
				return n;
			}
		}
		sl_uint8* bufSdata = (sl_uint8*)(m_sdata.getBuf());
		while (!(m_context.isReceivedHeader())) {
			sl_int32 n = stream->read32(bufSdata + m_sizeSdata, SDATA_SIZE - m_sizeSdata);
			if (n > 0) {
				m_sizeSdata += n;
				n = m_context.receiveHeader(bufSdata, m_sizeSdata);
				if (n < 0) {
					_close();
					return n;
				}
				if (n > 0) {
					m_posSdata = n;
					m_sizeSdata -= n;
					if (m_sizeSdata > 0) {
						m_context.processReceive(bufSdata + m_posSdata, m_sizeSdata, bufSdata + m_posSdata);
					}
					return n;
				}
			} else {
				if (n < 0) {
					_close();
				}
				return n;
			}
		}
		return 1;
	}

	sl_int32 connect()
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		MutexLocker lock(&m_lockReading);
		if (m_flagClosed) {
			return -1;
		}
		return _connect(stream.getValue());
	}

	sl_bool isConnected()
	{
		if (m_flagClosed) {
			return sl_false;
		}
		return m_context.isReceivedHeader();
	}

	sl_int32 read32(void* _buf, sl_uint32 size)
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		MutexLocker lock(&m_lockReading);
		if (m_flagClosed) {
			return -1;
		}
		sl_int32 n = _connect(stream.getValue());
		if (n <= 0) {
			return n;
		}
		if (size == 0) {
			return 0;
		}
		sl_uint8* buf = (sl_uint8*)_buf;
		if (m_sizeSdata > 0) {
			sl_uint8* bufSdata = (sl_uint8*)(m_sdata.getBuf());
			if (size < m_sizeSdata) {
				n = size;
			} else {
				n = m_sizeSdata;
			}
			Base::copyMemory(buf, bufSdata + m_posSdata, n);
			m_sizeSdata -= n;
			m_posSdata += n;
			if (m_sizeSdata == 0) {
				m_sdata.setNull();
			}
			return n;
		}
		n = stream->read32(buf, size);
		if (n > 0) {
			m_context.processReceive(buf, n, buf);
		}
		return n;
	}

	sl_int32 write32(const void* _buf, sl_uint32 size)
	{
		PtrLocker<IStream> stream(m_streamBase);
		if (stream.isNull()) {
			return -1;
		}
		MutexLocker lock(&m_lockWriting);
		if (m_flagClosed) {
			return -1;
		}
		sl_int32 n = _init(stream.getValue());
		if (n <= 0) {
			return n;
		}
		if (size == 0) {
			return 0;
		}
		const sl_uint8* buf = (const sl_uint8*)_buf;
		sl_uint8 seg[SEGMENT_SIZE];
		sl_uint32 total = 0;
		while (size > 0) {
			sl_uint32 m = SEGMENT_SIZE;
			if (m > size) {
				m = size;
			}
			m_context.processSend(buf, m, seg);
			n = stream->write32(seg, m);
			if (n < 0) {
				return n;
			}
			if (n == 0) {
				break;
			}
			buf += n;
			size -= n;
			total += n;
		}
		return total;
	}

	void close()
	{
		MutexLocker lock1(&m_lockReading);
		MutexLocker lock2(&m_lockWriting);
		_close();
	}

public:
	static Ref<_SecureStreamClient_SecureStream> create(const Ptr<IStream>& _baseStream, const SecureStreamClientParam& param)
	{
		Ref<_SecureStreamClient_SecureStream> ret;
		Memory rdata = Memory::create(RDATA_SIZE);
		Memory sdata = Memory::create(SDATA_SIZE);
		if (rdata.isEmpty() || sdata.isEmpty()) {
			return ret;
		}
		Ptr<IStream> baseStream = _baseStream;
		if (baseStream.isNotNull()) {
			ret = new _SecureStreamClient_SecureStream;
			ret->m_streamBase = baseStream;
			ret->m_rdata = rdata;
			ret->m_sdata = sdata;
			if (ret->init(param)) {
				return ret;
			}
			ret.setNull();
		}
		return ret;
	}
};

Ref<SecureStream> SecureStreamClient::createStream(const Ptr<IStream>& baseStream, const SecureStreamClientParam& param)
{
	Ref<_SecureStreamClient_SecureStream> ret = _SecureStreamClient_SecureStream::create(baseStream, param);
	return Ref<SecureStream>::from(ret);
}

class _SecureStreamClient_AsyncStream : public AsyncSecureStream, public IAsyncStreamListener
{
public:
	Ref<AsyncStream> m_streamBase;
	_SecureStreamClient_Context m_context;

	Memory m_sdata;
	sl_uint32 m_posSdata;
	sl_uint32 m_sizeSdata;
	Memory m_rdata;
	sl_uint32 m_sizeRdata;

	sl_bool m_flagInited;

public:
	_SecureStreamClient_AsyncStream()
	{
		m_posSdata = 0;
		m_sizeSdata = 0;
		m_sizeRdata = 0;
		m_flagInited = sl_false;
	}

	void _close()
	{
		m_streamBase.setNull();
	}

	sl_bool connect()
	{
		MutexLocker lock(getLocker());
		if (m_flagInited) {
			return sl_false;
		}
		m_flagInited = sl_true;
		WeakRef<_SecureStreamClient_AsyncStream> _this(this);
		Memory rdata = m_rdata.sub(0, m_sizeRdata);
		m_rdata.setNull();
		if (m_streamBase->writeFromMemory(rdata, _this)) {
			if (m_streamBase->readToMemory(m_sdata, _this)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool isConnected()
	{
		if (!m_flagInited) {
			return sl_false;
		}
		return m_context.isReceivedHeader();
	}

	void onRead(AsyncStream* _stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError)
	{
		MutexLocker lock(getLocker());
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNull()) {
			flagError = sl_true;
		}
		if (m_context.isReceivedHeader()) {
			if (!(AsyncStreamRequest::checkInstance(ref))) {
				return;
			}
			AsyncStreamRequest* req = (AsyncStreamRequest*)ref;
			if (sizeRead > 0) {
				m_context.processReceive(data, sizeRead, data);
			}
			lock.unlock();
			PtrLocker<IAsyncStreamListener> listener(req->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, data, sizeRead, req->refData.get(), flagError);
			}
		} else {
			if (flagError) {
				_close();
				lock.unlock();
				onConnected(sl_true);
			} else {
				if (sizeRead > 0) {
					m_sizeSdata += sizeRead;
					sl_int32 n = m_context.receiveHeader(m_sdata.getBuf(), m_sizeSdata);
					if (n > 0) {
						m_sizeSdata -= n;
						m_posSdata = n;
						if (m_sizeSdata > 0) {
							sl_uint8* p = (sl_uint8*)(m_sdata.getBuf()) + m_posSdata;
							m_context.processReceive(p, m_sizeSdata, p);
						}
						lock.unlock();
						onConnected(sl_false);
					} else {
						if (n < 0 || m_sizeSdata >= SDATA_SIZE) {
							_close();
							lock.unlock();
							onConnected(sl_true);
						} else {
							WeakRef<_SecureStreamClient_AsyncStream> _this(this);
							stream->read((sl_uint8*)(m_sdata.getBuf()) + m_sizeSdata, SDATA_SIZE - m_sizeSdata, _this, m_sdata.getReference());
						}
					}
				}
			}
		}
	}

	void onWrite(AsyncStream* _stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError)
	{
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNull()) {
			flagError = sl_true;
		}
		if (!(AsyncStreamRequest::checkInstance(ref))) {
			return;
		}
		AsyncStreamRequest* req = (AsyncStreamRequest*)ref;
		PtrLocker<IAsyncStreamListener> listener(req->listener);
		if (listener.isNotNull()) {
			listener->onWrite(this, data, sizeWritten, req->refData.get(), flagError);
		}
	}

	void processReadDone(Ref<AsyncStreamRequest> req)
	{
		if (req.isNotNull()) {
			PtrLocker<IAsyncStreamListener> listener(req->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, req->data, req->size, req->refData.get(), sl_false);
			}
		}
	}

	sl_bool _read(AsyncStreamRequest* req)
	{
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNotNull()) {
			if (m_context.isReceivedHeader()) {
				if (m_sizeSdata) {
					sl_uint32 n = m_sizeSdata;
					if (n > req->size) {
						n = req->size;
					}
					req->size = n;
					Base::copyMemory(req->data, (sl_uint8*)(m_sdata.getBuf()) + m_posSdata, n);
					m_sizeSdata -= n;
					m_posSdata += n;
					Ref<AsyncLoop> loop = stream->getLoop();
					if (loop.isNotNull()) {
						loop->addTask(SLIB_CALLBACK_WEAKREF(_SecureStreamClient_AsyncStream, processReadDone, this, Ref<AsyncStreamRequest>(req)), sl_false);
					}
					if (m_sizeSdata == 0) {
						m_sdata.setNull();
					}
				} else {
					return stream->read(req->data, req->size, WeakRef<_SecureStreamClient_AsyncStream>(this), req);
				}
			}
		}
		return sl_false;
	}

	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref)
	{
		MutexLocker lock(getLocker());
		if (!m_flagInited) {
			return sl_false;
		}
		Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, ref, listener);
		if (req.isNotNull()) {
			return _read(req.get());
		}
		return sl_false;
	}

	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref)
	{
		MutexLocker lock(getLocker());
		if (!m_flagInited) {
			return sl_false;
		}
		Ref<AsyncStream> stream = m_streamBase;
		if (stream.isNotNull()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, ref, listener);
			if (req.isNotNull()) {
				m_context.processSend(data, size, data);
				return stream->write(data, size, WeakRef<_SecureStreamClient_AsyncStream>(this), req.get());
			}
		}
		return sl_false;
	}

	void close()
	{
		MutexLocker lock(getLocker());
		_close();
	}

	sl_bool isOpened()
	{
		return m_streamBase.isNotNull();
	}

public:
	static Ref<_SecureStreamClient_AsyncStream> create(const Ref<AsyncStream>& _baseStream, const AsyncSecureStreamClientParam& param, sl_bool flagConnect)
	{
		Ref<_SecureStreamClient_AsyncStream> ret;
		Memory sdata = Memory::create(SDATA_SIZE);
		Memory rdata = Memory::create(RDATA_SIZE);
		if (sdata.isEmpty() || rdata.isEmpty()) {
			return ret;
		}
		Ref<AsyncStream> baseStream = _baseStream;
		if (baseStream.isNotNull()) {
			Ref<AsyncLoop> loop = baseStream->getLoop();
			if (loop.isNotNull()) {
				ret = new _SecureStreamClient_AsyncStream;
				if (ret.isNotNull()) {
					ret->setLoop(loop);
					ret->m_sdata = sdata;
					ret->m_rdata = rdata;
					ret->m_streamBase = baseStream;
					ret->setListener(param.listener);
					sl_int32 n = ret->m_context.init(param, rdata.getBuf());
					if (n > 0) {
						ret->m_sizeRdata = n;
						if (flagConnect) {
							if (ret->connect()) {
								return ret;
							}
						} else {
							return ret;
						}
					}
					ret.setNull();
				}
			}
		}
		return ret;
	}
};


Ref<AsyncSecureStream> SecureStreamClient::createAsyncStream(const Ref<AsyncStream>& baseStream, const AsyncSecureStreamClientParam& param, sl_bool flagConnect)
{
	Ref<_SecureStreamClient_AsyncStream> ret = _SecureStreamClient_AsyncStream::create(baseStream, param, flagConnect);
	return Ref<AsyncSecureStream>::from(ret);
}

struct _SecureStream_DefaultKey
{
	RSAPrivateKey privateKey;
	RSAPublicKey publicKey;

	_SecureStream_DefaultKey()
	{
		publicKey.N = BigInt::fromHexString(
			"00a1793cc1aa5cc6ebc55ad20220be"
			"24f805a4c4deea33a39bec054d8866"
			"1c383bf69d3622a51073bf1cd7f2ee"
			"71e60c16eb61b5475b2cf47cfdaf24"
			"b8174ba943aff431617895c9bc508b"
			"4d81f7df34506275dbada5740b3891"
			"79d421bfc73d16c599ff1f328c7b61"
			"74fd4067beb834b460fc79f0a7195a"
			"7bcee7d43f10e94a455a5a73e2fe5b"
			"dd22e44aaae8c177538fb3041116e5"
			"c2835b75f32c08110b4a50cfb667e9"
			"a57fc00a4824bd2e33cd724a6f26e2"
			"1652ab7860a960e397e3143c1be1aa"
			"7c29b056350b96f102ad717b2938ec"
			"19dc29e702053ccf2fd615720c5781"
			"fd27dad5e0ef3bf0e90bcce85ea526"
			"877bc729a5b9a5d3b5274f8bedea50"
			"aaa9"
			);
		publicKey.E = BigInt::fromInt32(65537);

		privateKey.N = publicKey.N;
		privateKey.E = publicKey.E;
		privateKey.D = BigInt::fromHexString(
			"7fca4d150fd8a0ed12a14fb6b36708"
			"26e9807ef5d3efce39a5ce848fc00f"
			"f149336133e8b32d940cd9ace5eb63"
			"64a688b4fbfcd55e08f3ecbf700ee5"
			"4a3a6cd8ceb5b926e4a1d6a81c2e42"
			"c6eafe508e957da4bf86439265d06b"
			"d44069602aae8cefd50241a500381e"
			"b8167284738ff279171702279c5f09"
			"c0b3a43bcd0c73e3b14b8ada604f85"
			"cd2ff4b7c0403ac4e4541279159a67"
			"782f858f98c5da79cfd29729e22e0d"
			"181cc39adceda50112c7404ed63929"
			"897452421b315ba99205e33470c1c9"
			"51c36077e97cf670dd58ee575ad7a6"
			"b413e78f7792a150be25e8db1eb82b"
			"975b89b8ca663565356faa8d9e67cb"
			"c94553d46e39691bff7830746b56ca"
			"69"
			);
		privateKey.P = BigInt::fromHexString(
			"00cd6bb0c73a1f517fb1309f75aee7"
			"ad8221da551d8cc62a6906ad578a29"
			"318d7cfd785d76210ba65c4cf36d32"
			"c7d2e7ba0e1711d241edceac1507e7"
			"fc55ddf949d7452b5d3370931bc226"
			"a135a05867f83c30d20cb461d9856b"
			"ec87a6ff85be0941c47159aa8ec1a7"
			"9e2ab17085ede82f1c05769a2ff6a3"
			"4533c11fc2bcd6c1ab"
			);
		privateKey.Q = BigInt::fromHexString(
			"00c93b6c981f8505aaa763ba4cdc13"
			"2480af4e91c0ba072efce3d2365a04"
			"29f66a9349f2ff2510427d7be2cd18"
			"52804cbb9b3e08474a551abba676f0"
			"5bfae86349594ee31181b18620f7b9"
			"f999a5dc38909535bbb5fb6fcb5bf8"
			"f0684ab28731a2c608408fe794cacc"
			"ce026b6b10fd2fc89e6063c1f4682a"
			"98a3a5168ca18a58fb"
			);
		privateKey.DP = BigInt::fromHexString(
			"45770f56afcefb6e64eafe617b7e18"
			"ec92aa147263b27791009309b14cf0"
			"d6d9d7952675d602ec84f5f8b38aa2"
			"8c2d947b434de17e2dfedfae54f81c"
			"1aa56fb18abc774793279477a4960f"
			"99e4d9407f6f4b4b1cc4bafa25e633"
			"1c445ad04d4d3012500a7464c613fa"
			"a6914e6ddbdfd25e75727ffd8c37f6"
			"24e43feb48948ef5"
			);
		privateKey.DQ = BigInt::fromHexString(
			"14e9cb1868362fe7936be6e1b4017d"
			"4aae6d70f261a2fb7c26c52306e9e5"
			"de8926f3898080be96da69b4a17204"
			"a8f6d08c8abedb81db24631a22c44a"
			"1ae269e72693ef3f53863a9554898c"
			"6a2e759b9f53127986a931ee2589d2"
			"9a542107b1dde3ccd9c0dae578ca1e"
			"22dc3d0d4ff2d65bb752c6c67ac8ea"
			"821f571f986e435f"
			);
		privateKey.IQ = BigInt::fromHexString(
			"00c98222352b39c1e8c0e1bfae7cd5"
			"38742d67ed8fe00f9306c9b36b79b7"
			"6bde897784edf1e99249c8cf63b3d9"
			"5efdd85dd81ff5331e8ceaa1913123"
			"0556bba2f6ccb4bf4983fd1e8b103e"
			"6955e8bb37b2b97494a7f48f1a05a1"
			"f58f75a4496f2f7dab2b9a1fd7fd8b"
			"2e96fe2c1e53521e57157bca3359d9"
			"eda87e5e39526a13c9"
			);
	}
};

static const _SecureStream_DefaultKey& _SecureStream_getDefaultKey()
{
	SLIB_SAFE_STATIC(_SecureStream_DefaultKey, ret);
	return ret;
}

const RSAPrivateKey& SecureStream::getDefaultPrivateKey()
{
	return _SecureStream_getDefaultKey().privateKey;
}

const RSAPublicKey& SecureStream::getDefaultPublicKey()
{
	return _SecureStream_getDefaultKey().publicKey;
}
SLIB_CRYPTO_NAMESPACE_END
