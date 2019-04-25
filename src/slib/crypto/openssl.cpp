/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/crypto/openssl.h"

#include "openssl/ssl.h"
#include "openssl/rand.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(OpenSSL_Context, TlsContext)
	
	OpenSSL_Context::OpenSSL_Context()
	{
	}
	
	OpenSSL_Context::~OpenSSL_Context()
	{
	}
	
	SLIB_DEFINE_OBJECT(OpenSSL_AsyncStream, TlsAsyncStream)
	
	OpenSSL_AsyncStream::OpenSSL_AsyncStream()
	{
	}
	
	OpenSSL_AsyncStream::~OpenSSL_AsyncStream()
	{
	}
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	class _priv_OpenSSL_ThreadHandler : public Referable
	{
	public:
		_priv_OpenSSL_ThreadHandler()
		{
		}
		
		~_priv_OpenSSL_ThreadHandler()
		{
			OPENSSL_thread_stop();
		}
	};
	
	static void _priv_OpenSSL_initThread()
	{
		static SLIB_THREAD int check = 0;
		if (check) {
			return;
		}
		check = 1;
		SSL_library_init();
		Ref<Thread> thread = Thread::getCurrent();
		if (thread.isNotNull()) {
			SLIB_STATIC_STRING(name, "_SLIB_OPENSSL")
			Ref<Referable> ref = thread->getAttachedObject(name);
			if (ref.isNull()) {
				ref = new _priv_OpenSSL_ThreadHandler;
				if (ref.isNotNull()) {
					thread->attachObject(name, ref.get());
				}
			}
		}
	}
#else
	static void _priv_OpenSSL_initThread()
	{
	}
#endif
	
	class _priv_OpenSSL_KeyStore : public Referable
	{
	public:
		X509* certificate;
		Queue<X509*> certificateChain;
		EVP_PKEY* privateKey;
		
	public:
		_priv_OpenSSL_KeyStore() : certificate(sl_null), privateKey(sl_null)
		{
		}
		
		~_priv_OpenSSL_KeyStore()
		{
			close();
		}
		
	public:
		void close()
		{
			resetCertificate();
			resetPrivateKey();
		}
		
		void resetCertificate()
		{
			if (certificate) {
				X509_free(certificate);
				certificate = sl_null;
			}
			X509* x;
			while (certificateChain.pop_NoLock(&x)) {
				X509_free(x);
			}
		}
		
		void setCertificate(const Memory& certificate)
		{
			if (certificate.isNull()) {
				return;
			}
			resetCertificate();
			BIO* bio = BIO_new(BIO_s_mem());
			if (bio) {
				BIO_write(bio, certificate.getData(), (int)(certificate.getSize()));
				X509* x = PEM_read_bio_X509_AUX(bio, sl_null, sl_null, sl_null);
				if (x) {
					this->certificate = x;
					for (;;) {
						x = PEM_read_bio_X509(bio, sl_null, sl_null, sl_null);
						if (!x) {
							break;
						}
						this->certificateChain.push_NoLock(x);
					}
				}
				BIO_free(bio);
			}
		}
		
		static void applyCertificate(SSL_CTX* ctx, const Memory& certificate)
		{
			if (certificate.isNull()) {
				return;
			}
			BIO* bio = BIO_new(BIO_s_mem());
			if (bio) {
				BIO_write(bio, certificate.getData(), (int)(certificate.getSize()));
				X509* x = PEM_read_bio_X509_AUX(bio, sl_null, sl_null, sl_null);
				if (x) {
					SSL_CTX_use_certificate(ctx, x);
					X509_free(x);
					SSL_CTX_clear_chain_certs(ctx);
					for (;;) {
						x = PEM_read_bio_X509(bio, sl_null, sl_null, sl_null);
						if (!x) {
							break;
						}
						SSL_CTX_add0_chain_cert(ctx, x);
					}
				}
				BIO_free(bio);
			}
		}
		
		void applyCertificate(SSL* ssl)
		{
			if (certificate) {
				SSL_use_certificate(ssl, certificate);
			}
			SSL_clear_chain_certs(ssl);
			Link<X509*>* link = certificateChain.getFront();
			while (link) {
				X509_up_ref(link->value);
				SSL_add0_chain_cert(ssl, link->value);
				link = link->next;
			}
		}

		void resetPrivateKey()
		{
			if (privateKey) {
				EVP_PKEY_free(privateKey);
				privateKey = sl_null;
			}
		}
		
		void setPrivateKey(const Memory& privateKey)
		{
			if (privateKey.isNull()) {
				return;
			}
			resetPrivateKey();
			BIO* bio = BIO_new(BIO_s_mem());
			if (bio) {
				BIO_write(bio, privateKey.getData(), (int)(privateKey.getSize()));
				EVP_PKEY* key = PEM_read_bio_PrivateKey(bio, sl_null, sl_null, sl_null);
				if (key) {
					this->privateKey = key;
				}
				BIO_free(bio);
			}
		}
		
		static void applyPrivateKey(SSL_CTX* ctx, const Memory& privateKey)
		{
			if (privateKey.isNull()) {
				return;
			}
			BIO* bio = BIO_new(BIO_s_mem());
			if (bio) {
				BIO_write(bio, privateKey.getData(), (int)(privateKey.getSize()));
				EVP_PKEY* key = PEM_read_bio_PrivateKey(bio, sl_null, sl_null, sl_null);
				if (key) {
					SSL_CTX_use_PrivateKey(ctx, key);
					EVP_PKEY_free(key);
				}
				BIO_free(bio);
			}
		}
		
		void applyPrivateKey(SSL* ssl)
		{
			if (privateKey) {
				SSL_use_PrivateKey(ssl, privateKey);
			}
		}
		
		void apply(SSL* ssl) {
			applyCertificate(ssl);
			applyPrivateKey(ssl);
		}

	};

	class _priv_OpenSSL_Context : public OpenSSL_Context
	{
	public:
		SSL_CTX* m_context;
		HashMap< String, Ref<_priv_OpenSSL_KeyStore> > m_keyStores;
		String m_serverName;

	public:
		_priv_OpenSSL_Context()
		{
		}
		
		~_priv_OpenSSL_Context()
		{
			SSL_CTX_free(m_context);
		}
		
	public:
		static Ref<_priv_OpenSSL_Context> create(const TlsContextParam& param)
		{
			_priv_OpenSSL_initThread();
			SSL_CTX* ctx = SSL_CTX_new(TLS_method());
			if (ctx) {
				_priv_OpenSSL_KeyStore::applyCertificate(ctx, param.certificate);
				_priv_OpenSSL_KeyStore::applyPrivateKey(ctx, param.privateKey);
				HashMap< String, Ref<_priv_OpenSSL_KeyStore> > keyStores;
				if (param.certificates.isNotEmpty() && param.privateKeys.isNotEmpty()) {
					for (auto& item : param.certificates) {
						if (item.key.isNotEmpty() && item.value.isNotNull()) {
							Memory privateKey;
							if (param.privateKeys.get(item.key, &privateKey)) {
								if (privateKey.isNotNull()) {
									Ref<_priv_OpenSSL_KeyStore> keyStore = new _priv_OpenSSL_KeyStore;
									if (keyStore.isNotNull()) {
										keyStore->setCertificate(item.value);
										keyStore->setPrivateKey(privateKey);
										if (keyStore->certificate && keyStore->privateKey) {
											keyStores.put_NoLock(item.key, keyStore);
										}
									}
								}
							}
						}
					}
				}
				Ref<_priv_OpenSSL_Context> ret = new _priv_OpenSSL_Context;
				if (ret.isNotNull()) {
					ret->m_context = ctx;
					ret->m_keyStores = keyStores;
					ret->m_serverName = param.serverName;
					if (param.flagVerify) {
						SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);
					}
					if (keyStores.isNotEmpty() || param.serverName.isNotEmpty()) {
						SSL_CTX_set_client_hello_cb(ctx, client_hello_callback, ret.get());
					}
					return ret;
				}
				SSL_CTX_free(ctx);
			}
			return sl_null;
		}

		static int verify_callback(int preverify, X509_STORE_CTX* x509_ctx)
		{
			return preverify;
		}
		
		static int client_hello_callback(SSL* ssl, int* al, void* arg)
		{
			_priv_OpenSSL_Context* context = (_priv_OpenSSL_Context*)arg;
			if (context) {
				if (SSL_is_server(ssl)) {
					const unsigned char* extServerName = sl_null;
					size_t size = 0;
					int ret = SSL_client_hello_get0_ext(ssl, TLSEXT_TYPE_server_name, &extServerName, &size);
					if (ret && extServerName && size >= 5) {
						sl_uint32 len = SLIB_MAKE_WORD(extServerName[0], extServerName[1]);
						if (len + 2 == size && extServerName[2] == TLSEXT_NAMETYPE_host_name) {
							len = SLIB_MAKE_WORD(extServerName[3], extServerName[4]);
							if (len + 5 <= size) {
								const char* serverName = (const char *)(extServerName + 5);
								Ref<_priv_OpenSSL_KeyStore>* pKeyStore = context->m_keyStores.getItemPointer(String(serverName, len));
								if (pKeyStore) {
									(*pKeyStore)->apply(ssl);
								}
							}
						}
					}
				} else {
					if (context->m_serverName.isNotEmpty()) {
						SSL_set_tlsext_host_name(ssl, context->m_serverName.getData());
					}
				}
			}
			return SSL_CLIENT_HELLO_SUCCESS;
		}
		
		SSL_CTX* getContext() override
		{
			return m_context;
		}
		
	};
	
	class SLIB_EXPORT _priv_OpenSSL_Stream : public OpenSSL_AsyncStream
	{
	public:
		Ref<AsyncStream> m_baseStream;
		Ref<_priv_OpenSSL_Context> m_context;
		SSL* m_ssl;
		BIO* m_rbio;
		BIO* m_wbio;
		
		sl_bool m_flagReadingBase;
		sl_bool m_flagWritingBase;
		sl_bool m_flagReadingError;
		sl_bool m_flagWritingError;
		Memory m_bufReadingBase;
		Memory m_bufWritingBase;
		sl_reg m_sizeWritingBase;
		Function<void(AsyncStreamResult&)> m_callbackRead;
		Function<void(AsyncStreamResult&)> m_callbackWrite;
		Function<void()> m_doStartWritingBase;

		Ref<AsyncStreamRequest> m_requestRead;
		Queue< Ref<AsyncStreamRequest> > m_queueRead;

		Ref<AsyncStreamRequest> m_requestWrite;
		sl_uint32 m_sizeWritten;
		Queue< Ref<AsyncStreamRequest> > m_queueWrite;
		Function<void()> m_doStartWriting;
		
		sl_bool m_flagHandshaking;
		sl_bool m_flagInitHandshake;
		Function<void(TlsStreamResult&)> m_onHandshake;

	protected:
		_priv_OpenSSL_Stream(const Ref<AsyncStream>& baseStream)
		 : m_baseStream(baseStream)
		{
			m_flagReadingBase = sl_false;
			m_flagWritingBase = sl_false;
			m_flagReadingError = sl_false;
			m_flagWritingError = sl_false;
			m_sizeWritingBase = 0;

			m_sizeWritten = 0;
			
			m_flagHandshaking = sl_true;
			m_flagInitHandshake = sl_false;
		}

		void init() override
		{
			OpenSSL_AsyncStream::init();
			
			m_callbackRead = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_Stream, onRead, this);
			m_callbackWrite = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_Stream, onWrite, this);
			m_doStartWritingBase = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_Stream, startWritingBase, this);
			m_doStartWriting = SLIB_FUNCTION_WEAKREF(_priv_OpenSSL_Stream, startWriting, this);
		}
		
		~_priv_OpenSSL_Stream()
		{
			close();
		}
		
	public:
		static Ref<_priv_OpenSSL_Stream> connectStream(const Ref<AsyncStream>& stream, const TlsConnectStreamParam& param)
		{
			Ref<_priv_OpenSSL_Stream> ret = create(stream, param);
			if (ret.isNotNull()) {
				ret->m_onHandshake = param.onHandshake;
				SSL_set_connect_state(ret->m_ssl);
				if (param.flagAutoStartHandshake) {
					ret->handshake();
				}
				return ret;
			}
			return sl_null;
		}
		
		static Ref<_priv_OpenSSL_Stream> acceptStream(const Ref<AsyncStream>& stream, const TlsAcceptStreamParam& param)
		{
			Ref<_priv_OpenSSL_Stream> ret = create(stream, param);
			if (ret.isNotNull()) {
				ret->m_onHandshake = param.onHandshake;
				SSL_set_accept_state(ret->m_ssl);
				if (param.flagAutoStartHandshake) {
					ret->handshake();
				}
				return ret;
			}
			return sl_null;
		}
		
	private:
		static Ref<_priv_OpenSSL_Stream> create(const Ref<AsyncStream>& baseStream, const TlsStreamParam& param)
		{
			_priv_OpenSSL_initThread();
			if (baseStream.isNull()) {
				return sl_null;
			}
			Memory bufReading = Memory::create(param.readingBufferSize);
			if (bufReading.isNull()) {
				return sl_null;
			}
			Memory bufWriting = Memory::create(param.readingBufferSize);
			if (bufWriting.isNull()) {
				return sl_null;
			}
			Ref<_priv_OpenSSL_Context> context = Ref<_priv_OpenSSL_Context>::from(param.context);
			if (!(IsInstanceOf<_priv_OpenSSL_Context>(context))) {
				context = _priv_OpenSSL_Context::create(param);
				if (context.isNull()) {
					return sl_null;
				}
			}
			SSL* ssl = SSL_new(context->getContext());
			if (ssl) {
				BIO* rbio = BIO_new(BIO_s_mem());
				if (rbio) {
					BIO* wbio = BIO_new(BIO_s_mem());
					if (wbio) {
						SSL_set_bio(ssl, rbio, wbio); // `rbio` and `wbio` is freed when `SSL_free` is called
						Ref<_priv_OpenSSL_Stream> ret = new _priv_OpenSSL_Stream(baseStream);
						if (ret.isNotNull()) {
							ret->m_ssl = ssl;
							ret->m_context = context;
							ret->m_rbio = rbio;
							ret->m_wbio = wbio;
							BIO_set_callback_arg(rbio, (char*)(ret.get()));
							BIO_set_callback_arg(wbio, (char*)(ret.get()));
							BIO_set_callback_ex(rbio, read_callback);
							BIO_set_callback_ex(wbio, write_callback);
							ret->m_bufReadingBase = bufReading;
							ret->m_bufWritingBase = bufWriting;
							return ret;
						}
					}
					BIO_free(rbio);
				}
				SSL_free(ssl);
			}
			return sl_null;
		}
		
		static long read_callback(BIO *b, int oper, const char *argp, size_t len, int argi, long argl, int ret, size_t *processed)
		{
			_priv_OpenSSL_Stream* stream = (_priv_OpenSSL_Stream*)(BIO_get_callback_arg(b));
			if ((oper == (BIO_CB_READ | BIO_CB_RETURN) && (!ret || (ret && len > *processed))) || oper == (BIO_CB_GETS | BIO_CB_RETURN)) {
				stream->onRequestRead();
			}
			return ret;
		}
		
		static long write_callback(BIO *b, int oper, const char *argp, size_t len, int argi, long argl, int ret, size_t *processed)
		{
			_priv_OpenSSL_Stream* stream = (_priv_OpenSSL_Stream*)(BIO_get_callback_arg(b));
			if ((oper == (BIO_CB_WRITE | BIO_CB_RETURN) && ret) || oper == (BIO_CB_PUTS | BIO_CB_RETURN)) {
				stream->onRequestWrite(*processed);
			}
			return ret;
		}
		
	private:
		void onRequestRead()
		{
			startReadingBase();
		}
		
		void onRequestWrite(sl_size len)
		{
			Base::interlockedAdd(&m_sizeWritingBase, len);
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			if (m_flagWritingBase) {
				return;
			}
			addTask(m_doStartWritingBase);
		}
		
		void startReadingBase()
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			if (m_flagReadingBase) {
				return;
			}
			if (m_flagReadingError) {
				return;
			}
			m_flagReadingBase = sl_true;
			m_baseStream->readToMemory(m_bufReadingBase, m_callbackRead);
		}
		
		void startWritingBase()
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			if (m_flagWritingBase) {
				return;
			}
			if (m_flagWritingError) {
				return;
			}
			void* data = m_bufWritingBase.getData();
			int len = BIO_read(m_wbio, data, (int)(m_bufWritingBase.getSize()));
			if (len > 0) {
				Base::interlockedAdd(&m_sizeWritingBase, -len);
				m_flagWritingBase = sl_true;
				m_baseStream->write(data, len, m_callbackWrite);
			}
		}
		
		void startReading()
		{
			for (;;) {
				ObjectLocker lock(this);
				if (m_baseStream.isNull()) {
					return;
				}
				if (m_requestRead.isNull()) {
					return;
				}
				int ret = SSL_read(m_ssl, (char*)(m_requestRead->data), m_requestRead->size);
				if (ret > 0) {
					Ref<AsyncStreamRequest> request = m_requestRead;
					if (!(m_queueRead.pop_NoLock(&m_requestRead))) {
						m_requestRead.setNull();
					}
					lock.unlock();
					request->runCallback(this, (sl_uint32)ret, sl_false);
				} else {
					if (!m_flagReadingError) {
						int err = SSL_get_error(m_ssl, ret);
						if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
							return;
						}
					}
					Ref<AsyncStreamRequest> request = m_requestRead;
					if (!(m_queueRead.pop_NoLock(&m_requestRead))) {
						m_requestRead.setNull();
					}
					lock.unlock();
					request->runCallback(this, 0, sl_true);
					break;
				}
			}
		}
		
		void startWriting()
		{
			for (;;) {
				ObjectLocker lock(this);
				if (m_baseStream.isNull()) {
					return;
				}
				if (m_requestWrite.isNull()) {
					return;
				}
				for (;;) {
					int ret = -1;
					if (!m_flagWritingError) {
						if ((sl_size)m_sizeWritingBase > (m_bufWritingBase.getSize() << 1)) {
							return;
						}
						ret = SSL_write(m_ssl, (char*)(m_requestWrite->data) + m_sizeWritten, m_requestWrite->size - m_sizeWritten);
					}
					if (ret > 0) {
						m_sizeWritten += ret;
						sl_uint32 size = m_sizeWritten;
						if (size >= m_requestWrite->size) {
							Ref<AsyncStreamRequest> request = m_requestWrite;
							m_sizeWritten = 0;
							if (!(m_queueWrite.pop_NoLock(&m_requestWrite))) {
								m_requestWrite.setNull();
							}
							lock.unlock();
							request->runCallback(this, size, sl_false);
							break;
						}
					} else {
						if (!m_flagWritingError) {
							int err = SSL_get_error(m_ssl, ret);
							if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
								return;
							}
						}
						Ref<AsyncStreamRequest> request = m_requestWrite;
						sl_uint32 size = m_sizeWritten;
						m_sizeWritten = 0;
						if (!(m_queueWrite.pop_NoLock(&m_requestWrite))) {
							m_requestWrite.setNull();
						}
						lock.unlock();
						request->runCallback(this, size, sl_true);
						break;
					}
				}
			}
		}
		
		void onRead(AsyncStreamResult& result)
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			m_flagReadingBase = sl_false;
			if (result.flagError) {
				m_flagReadingError = sl_true;
			}
			if (result.size > 0) {
				BIO_write(m_rbio, result.data, result.size);
			}
			doIO(lock);
		}
		
		void onWrite(AsyncStreamResult& result)
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			m_flagWritingBase = sl_false;
			doIO(lock);
			startWritingBase();
		}
		
		void doIO(ObjectLocker& lock)
		{
			_priv_OpenSSL_initThread();
			if (m_flagHandshaking) {
				doHandshake(lock);
			} else {
				if (m_requestRead.isNotNull()) {
					startReading();
				}
				if (m_requestWrite.isNotNull()) {
					startWriting();
				}
			}
		}
		
		void handshake() override
		{
			ObjectLocker lock(this);
			if (m_flagInitHandshake) {
				return;
			}
			m_flagInitHandshake = sl_true;
			doHandshake(lock);
		}
		
		void doHandshake(ObjectLocker& lock)
		{
			int err = 0;
			int ret = SSL_connect(m_ssl);
			if (ret == 1) {
				m_flagHandshaking = sl_false;
				lock.unlock();
				if (m_onHandshake.isNotNull()) {
					TlsStreamResult result(this);
					result.flagError = sl_false;
					m_onHandshake(result);
				}
				return;
			} else {
				err = SSL_get_error(m_ssl, ret);
				if (err == SSL_ERROR_WANT_READ) {
					startReadingBase();
					return;
				} else if (err == SSL_ERROR_WANT_WRITE) {
					return;
				}
			}
			if (m_onHandshake.isNotNull()) {
				TlsStreamResult result(this);
				m_onHandshake(result);
			}
		}
		
	public:
		SSL* getSSL() override
		{
			return m_ssl;
		}
		
		void close() override
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return;
			}
			SSL_free(m_ssl);
			m_context.setNull();
			m_baseStream.setNull();
		}
		
		sl_bool isOpened() override
		{
			return m_baseStream.isNotNull();
		}
		
		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject) override
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return sl_false;
			}
			if (m_flagReadingError) {
				return sl_false;
			}
			Ref<AsyncStreamRequest> request = AsyncStreamRequest::createRead(data, size, userObject, callback);
			if (request.isNull()) {
				return sl_false;
			}
			if (m_requestRead.isNull()) {
				m_requestRead = request;
				startReadingBase();
			} else {
				m_queueRead.push_NoLock(request);
			}
			return sl_true;
		}
		
		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject) override
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return sl_false;
			}
			if (m_flagWritingError) {
				return sl_false;
			}
			Ref<AsyncStreamRequest> request = AsyncStreamRequest::createWrite(data, size, userObject, callback);
			if (request.isNull()) {
				return sl_false;
			}
			if (m_requestWrite.isNull()) {
				m_requestWrite = request;
				m_sizeWritten = 0;
				addTask(m_doStartWriting);
			} else {
				m_queueWrite.push_NoLock(request);
			}
			return sl_true;
		}
		
		sl_bool addTask(const Function<void()>& callback) override
		{
			ObjectLocker lock(this);
			if (m_baseStream.isNull()) {
				return sl_false;
			}
			return m_baseStream->addTask(callback);
		}
		
	};
	
	Ref<OpenSSL_Context> OpenSSL::createContext(const TlsContextParam& param)
	{
		return Ref<OpenSSL_Context>::from(_priv_OpenSSL_Context::create(param));
	}

	Ref<OpenSSL_AsyncStream> OpenSSL::connectStream(const Ref<AsyncStream>& baseStream, const TlsConnectStreamParam& param)
	{
		return Ref<OpenSSL_AsyncStream>::from(_priv_OpenSSL_Stream::connectStream(baseStream, param));
	}
	
	Ref<OpenSSL_AsyncStream> OpenSSL::acceptStream(const Ref<AsyncStream>& baseStream, const TlsAcceptStreamParam& param)
	{
		return Ref<OpenSSL_AsyncStream>::from(_priv_OpenSSL_Stream::acceptStream(baseStream, param));
	}

	
	sl_bool OpenSSL::isProbablePrime(const void* num_BigEndian, sl_uint32 nBytes, sl_bool* pFlagError)
	{
		if (!nBytes) {
			if (pFlagError) {
				*pFlagError = sl_false;
			}
			return sl_false;
		}
		BIGNUM* num = BN_bin2bn((unsigned char*)num_BigEndian, nBytes, sl_null);
		if (num) {
			int ret = BN_is_prime_fasttest_ex(num, 0, sl_null, sl_false, sl_null);
			BN_free(num);
			if (pFlagError) {
				if (ret < 0) {
					*pFlagError = sl_true;
				} else {
					*pFlagError = sl_false;
				}
			}
			return ret == 1;
		} else {
			if (pFlagError) {
				*pFlagError = sl_true;
			}
		}
		return sl_false;
	}
	
	Memory OpenSSL::generatePrime(sl_uint32 nBits)
	{
		BIGNUM* prime = BN_new();
		if (BN_generate_prime_ex(prime, (int)nBits, sl_false, sl_null, sl_null, sl_null)) {
			sl_size n = (sl_size)(BN_num_bytes(prime));
			Memory ret = Memory::create(n);
			if (ret.isNotNull()) {
				BN_bn2bin(prime, (unsigned char*)(ret.getData()));
			}
			BN_free(prime);
			return ret;
		}
		return sl_null;
	}
	
	sl_bool OpenSSL::randomBytes(void* bytes, sl_uint32 nBytes, sl_bool flagPrivate)
	{
		if (flagPrivate) {
			if (RAND_priv_bytes((unsigned char*)bytes, (int)nBytes) == 1) {
				return sl_true;
			}
		} else {
			if (RAND_bytes((unsigned char*)bytes, (int)nBytes) == 1) {
				return sl_true;
			}
		}
		return sl_false;
	}

}
