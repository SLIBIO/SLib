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

#include "slib/crypto/sha2.h"
#include "slib/core/safe_static.h"

#include "openssl/ssl.h"
#include "openssl/aes.h"
#include "openssl/rand.h"

extern "C"
{
	void ChaCha20_ctr32(unsigned char *out, const unsigned char *inp, size_t len, const unsigned int key[8], const unsigned int counter[4]);
	
	struct poly1305_context;
	typedef struct poly1305_context POLY1305;
	
	size_t Poly1305_ctx_size(void);
	void Poly1305_Init(POLY1305 *ctx, const unsigned char key[32]);
	void Poly1305_Update(POLY1305 *ctx, const unsigned char *inp, size_t len);
	void Poly1305_Final(POLY1305 *ctx, unsigned char mac[16]);
}

namespace slib
{
	
	namespace priv
	{
		namespace openssl
		{

#if defined(SLIB_PLATFORM_IS_WIN32)
			
			class ThreadHandler : public Referable
			{
			public:
				ThreadHandler()
				{
				}

				~ThreadHandler()
				{
					OPENSSL_thread_stop();
				}
			};

			static void initThread()
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
						ref = new ThreadHandler;
						if (ref.isNotNull()) {
							thread->attachObject(name, ref.get());
						}
					}
				}
			}
#else
			static void initThread()
			{
			}
#endif
			
			class KeyStore : public Referable
			{
			public:
				X509* certificate;
				Queue<X509*> certificateChain;
				EVP_PKEY* privateKey;
				
			public:
				KeyStore() : certificate(sl_null), privateKey(sl_null)
				{
				}
				
				~KeyStore()
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
			
			class ContextImpl : public OpenSSL_Context
			{
			public:
				SSL_CTX* m_context;
				HashMap< String, Ref<KeyStore> > m_keyStores;
				String m_serverName;
				
			public:
				ContextImpl()
				{
				}
				
				~ContextImpl()
				{
					SSL_CTX_free(m_context);
				}
				
			public:
				static Ref<ContextImpl> create(const TlsContextParam& param)
				{
					initThread();
					SSL_CTX* ctx = SSL_CTX_new(TLS_method());
					if (ctx) {
						KeyStore::applyCertificate(ctx, param.certificate);
						KeyStore::applyPrivateKey(ctx, param.privateKey);
						HashMap< String, Ref<KeyStore> > keyStores;
						if (param.certificates.isNotEmpty() && param.privateKeys.isNotEmpty()) {
							for (auto& item : param.certificates) {
								if (item.key.isNotEmpty() && item.value.isNotNull()) {
									Memory privateKey;
									if (param.privateKeys.get(item.key, &privateKey)) {
										if (privateKey.isNotNull()) {
											Ref<KeyStore> keyStore = new KeyStore;
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
						Ref<ContextImpl> ret = new ContextImpl;
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
					ContextImpl* context = (ContextImpl*)arg;
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
										Ref<KeyStore>* pKeyStore = context->m_keyStores.getItemPointer(String(serverName, len));
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
			
			class SLIB_EXPORT StreamImpl : public OpenSSL_AsyncStream
			{
			public:
				Ref<AsyncStream> m_baseStream;
				Ref<ContextImpl> m_context;
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
				StreamImpl(const Ref<AsyncStream>& baseStream)
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
					
					m_callbackRead = SLIB_FUNCTION_WEAKREF(StreamImpl, onRead, this);
					m_callbackWrite = SLIB_FUNCTION_WEAKREF(StreamImpl, onWrite, this);
					m_doStartWritingBase = SLIB_FUNCTION_WEAKREF(StreamImpl, startWritingBase, this);
					m_doStartWriting = SLIB_FUNCTION_WEAKREF(StreamImpl, startWriting, this);
				}
				
				~StreamImpl()
				{
					close();
				}
				
			public:
				static Ref<StreamImpl> connectStream(const Ref<AsyncStream>& stream, const TlsConnectStreamParam& param)
				{
					Ref<StreamImpl> ret = create(stream, param);
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
				
				static Ref<StreamImpl> acceptStream(const Ref<AsyncStream>& stream, const TlsAcceptStreamParam& param)
				{
					Ref<StreamImpl> ret = create(stream, param);
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
				static Ref<StreamImpl> create(const Ref<AsyncStream>& baseStream, const TlsStreamParam& param)
				{
					initThread();
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
					Ref<ContextImpl> context = Ref<ContextImpl>::from(param.context);
					if (!(IsInstanceOf<ContextImpl>(context))) {
						context = ContextImpl::create(param);
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
								Ref<StreamImpl> ret = new StreamImpl(baseStream);
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
					StreamImpl* stream = (StreamImpl*)(BIO_get_callback_arg(b));
					if ((oper == (BIO_CB_READ | BIO_CB_RETURN) && (!ret || (ret && len > *processed))) || oper == (BIO_CB_GETS | BIO_CB_RETURN)) {
						stream->onRequestRead();
					}
					return ret;
				}
				
				static long write_callback(BIO *b, int oper, const char *argp, size_t len, int argi, long argl, int ret, size_t *processed)
				{
					StreamImpl* stream = (StreamImpl*)(BIO_get_callback_arg(b));
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
					initThread();
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
			
			static BigInt get_BigInt_from_BIGNUM(const BIGNUM* bn)
			{
				if (bn) {
					sl_size n = (sl_size)(BN_num_bytes(bn));
					if (n) {
						Memory mem = Memory::create(n);
						if (mem.isNotNull()) {
							BN_bn2bin(bn, (unsigned char*)(mem.getData()));
							return BigInt::fromBytesBE(mem);
						}
					}
				}
				return sl_null;
			}
			
			static BIGNUM* get_BIGNUM_from_BigInt(const BigInt& n)
			{
				if (n.isNotNull()) {
					Memory mem = n.getBytesBE();
					if (mem.isNotNull()) {
						return BN_bin2bn((unsigned char*)(mem.getData()), (int)(mem.getSize()), sl_null);
					}
				}
				return sl_null;
			}
			
			static ECPoint get_ECPoint_from_EC_POINT(EC_GROUP* group, const EC_POINT* pt)
			{
				ECPoint ret;
				BIGNUM* x = BN_new();
				if (x) {
					BIGNUM* y = BN_new();
					if (y) {
						if (1 == EC_POINT_get_affine_coordinates(group, pt, x, y, sl_null)) {
							ret.x = get_BigInt_from_BIGNUM(x);
							if (ret.x.isNotZero()) {
								ret.y = get_BigInt_from_BIGNUM(y);
							}
						}
						BN_free(y);
					}
					BN_free(x);
				}
				return ret;
			}
			
			static EC_POINT* get_EC_POINT_from_ECPoint(EC_GROUP* group, const ECPoint& pt)
			{
				EC_POINT* ret = EC_POINT_new(group);
				if (ret) {
					sl_bool flagSuccess = sl_false;
					BIGNUM* x = get_BIGNUM_from_BigInt(pt.x);
					if (x) {
						BIGNUM* y = get_BIGNUM_from_BigInt(pt.y);
						if (y) {
							if (1 == EC_POINT_set_affine_coordinates(group, ret, x, y, sl_null)) {
								flagSuccess = sl_true;
							}
							BN_free(y);
						}
						BN_free(x);
					}
					if (flagSuccess) {
						return ret;
					}
					EC_POINT_free(ret);
				}
				return sl_null;
			}
			
			static EC_KEY* get_EC_KEY_from_ECPublicKey(EC_GROUP* group, const ECPublicKey& key)
			{
				if (key.Q.isO()) {
					return sl_null;
				}
				EC_KEY* ek = EC_KEY_new();
				if (ek) {
					sl_bool flagSuccess = sl_false;
					EC_KEY_set_group(ek, group);
					EC_POINT* pt = get_EC_POINT_from_ECPoint(group, key.Q);
					if (pt) {
						EC_KEY_set_public_key(ek, pt);
						flagSuccess = sl_true;
						EC_POINT_free(pt);
					}
					if (flagSuccess) {
						return ek;
					}
					EC_KEY_free(ek);
				}
				return sl_null;
			}
			
			static EC_KEY* get_EC_KEY_from_ECPrivateKey(EC_GROUP* group, const ECPrivateKey& key)
			{
				EC_KEY* ek = get_EC_KEY_from_ECPublicKey(group, key);
				if (ek) {
					sl_bool flagSuccess = sl_false;
					BIGNUM* bn = get_BIGNUM_from_BigInt(key.d);
					if (bn) {
						EC_KEY_set_private_key(ek, bn);
						flagSuccess = sl_true;
						BN_free(bn);
					}
					if (flagSuccess) {
						return ek;
					}
					EC_KEY_free(ek);
				}
				return sl_null;
			}
			
			static ECDSA_SIG* get_ECDSA_SIG_from_ECDSA_Signature(const ECDSA_Signature& _sig)
			{
				ECDSA_SIG* sig = ECDSA_SIG_new();
				if (sig) {
					BIGNUM* r = get_BIGNUM_from_BigInt(_sig.r);
					if (r) {
						BIGNUM* s = get_BIGNUM_from_BigInt(_sig.s);
						if (s) {
							ECDSA_SIG_set0(sig, r, s);
							return sig;
						}
						BN_free(r);
					}
					ECDSA_SIG_free(sig);
				}
				return sl_null;
			}
			
			static ECDSA_Signature do_sign_ECDSA_secp256k1(const ECPrivateKey& key, const void* hash, sl_size size)
			{
				initThread();
				ECDSA_Signature ret;
				EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
				if (group) {
					EC_KEY* ek = get_EC_KEY_from_ECPrivateKey(group, key);
					if (ek) {
						ECDSA_SIG* sig = ECDSA_do_sign((unsigned char*)hash, (int)size, ek);
						if (sig) {
							const BIGNUM* r = ECDSA_SIG_get0_r(sig);
							if (r) {
								ret.r = get_BigInt_from_BIGNUM(r);
							}
							const BIGNUM* s = ECDSA_SIG_get0_s(sig);
							if (s) {
								ret.s = get_BigInt_from_BIGNUM(s);
							}
							ECDSA_SIG_free(sig);
						}
						EC_KEY_free(ek);
					}
					EC_GROUP_free(group);
				}
				return ret;
			}
			
			static sl_bool do_verify_ECDSA_secp256k1(const ECPublicKey& key, const void* hash, sl_size size, const ECDSA_Signature& _sig)
			{
				initThread();
				sl_bool flagVerified = sl_false;
				EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
				if (group) {
					EC_KEY* ek = get_EC_KEY_from_ECPublicKey(group, key);
					if (ek) {
						ECDSA_SIG* sig = get_ECDSA_SIG_from_ECDSA_Signature(_sig);
						if (sig) {
							if (1 == ECDSA_do_verify((unsigned char*)hash, (int)size, sig, ek)) {
								flagVerified = sl_true;
							}
							ECDSA_SIG_free(sig);
						}
						EC_KEY_free(ek);
					}
					EC_GROUP_free(group);
				}
				return flagVerified;
			}

		}
	}
	
	using namespace priv::openssl;
	
	
	OpenSSL_AES::OpenSSL_AES()
	{
		m_keyEnc = sl_null;
		m_keyDec = sl_null;
	}
	
	OpenSSL_AES::~OpenSSL_AES()
	{
		if (m_keyEnc) {
			Base::freeMemory(m_keyEnc);
		}
		if (m_keyDec) {
			Base::freeMemory(m_keyDec);
		}
	}
	
	sl_bool OpenSSL_AES::setKey(const void* key, sl_uint32 lenKey)
	{
		if (!m_keyEnc) {
			m_keyEnc = Base::createMemory(sizeof(AES_KEY));
		}
		if (AES_set_encrypt_key((unsigned char*)key, lenKey << 3, (AES_KEY*)m_keyEnc) != 0) {
			return sl_false;
		}
		if (!m_keyDec) {
			m_keyDec = Base::createMemory(sizeof(AES_KEY));
		}
		if (AES_set_decrypt_key((unsigned char*)key, lenKey << 3, (AES_KEY*)m_keyDec) != 0) {
			return sl_false;
		}
		return sl_true;
	}
	
	void OpenSSL_AES::setKey_SHA256(const String& key)
	{
		char sig[32];
		SHA256::hash(key, sig);
		setKey(sig, 32);
	}
	
	void OpenSSL_AES::encryptBlock(const void* src, void* dst) const
	{
		AES_encrypt((unsigned char*)src, (unsigned char*)dst, (AES_KEY*)m_keyEnc);
	}
	
	void OpenSSL_AES::decryptBlock(const void* src, void* dst) const
	{
		AES_decrypt((unsigned char*)src, (unsigned char*)dst, (AES_KEY*)m_keyDec);
	}
	
	
	OpenSSL_ChaCha20::OpenSSL_ChaCha20(): m_pos(0)
	{
	}
	
	OpenSSL_ChaCha20::~OpenSSL_ChaCha20()
	{
	}
	
	void OpenSSL_ChaCha20::setKey(const void* _key)
	{
		const sl_uint8* key = (const sl_uint8*)_key;
		sl_uint32* input = m_key;
		for (sl_uint32 i = 0; i < 8; i++) {
			sl_uint32 j = i << 2;
			input[i] = SLIB_MAKE_DWORD(key[j+3], key[j+2], key[j+1], key[j]);
		}
	}
	
	void OpenSSL_ChaCha20::generateBlock(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3, void* output)
	{
		sl_uint32 nonces[] = {nonce0, nonce1, nonce2, nonce3};
		Base::zeroMemory(output, 64);
		ChaCha20_ctr32((unsigned char*)output, (const unsigned char*)output, 64, (const unsigned int*)m_key, (const unsigned int*)nonces);
	}
	
	void OpenSSL_ChaCha20::start(sl_uint32 nonce0, sl_uint32 nonce1, sl_uint32 nonce2, sl_uint32 nonce3)
	{
		m_nonce[0] = nonce0;
		m_nonce[1] = nonce1;
		m_nonce[2] = nonce2;
		m_nonce[3] = nonce3;
		m_pos = 0;
	}
	
	void OpenSSL_ChaCha20::start(const void* _iv, sl_uint32 counter)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		m_nonce[0] = counter;
		m_nonce[1] = SLIB_MAKE_DWORD(iv[3], iv[2], iv[1], iv[0]);
		m_nonce[2] = SLIB_MAKE_DWORD(iv[7], iv[6], iv[5], iv[4]);
		m_nonce[3] = SLIB_MAKE_DWORD(iv[11], iv[10], iv[9], iv[8]);
		m_pos = 0;
	}
	
	void OpenSSL_ChaCha20::encrypt(const void* _src, void* _dst, sl_size len)
	{
		sl_uint8* lastBlock = m_lastBlock;
		const sl_uint8* src = (const sl_uint8*)_src;
		sl_uint8* dst = (sl_uint8*)_dst;
		sl_uint32 pos = m_pos;
		if (pos) {
			sl_uint32 remain = 64 - pos;
			if (len < remain) {
				sl_uint32 _len = (sl_uint32)len;
				for (sl_uint32 i = 0; i < _len; i++) {
					dst[i] = src[i] ^ lastBlock[pos + i];
				}
				m_pos += _len;
				return;
			} else {
				for (sl_uint32 i = 0; i < remain; i++) {
					dst[i] = src[i] ^ lastBlock[pos + i];
				}
				len -= remain;
				src += remain;
				dst += remain;
				m_pos = 0;
			}
		}
		sl_size nBlocks = len >> 6;
		sl_size nBytes = nBlocks << 6;
		if (nBlocks) {
			ChaCha20_ctr32((unsigned char*)dst, (const unsigned char*)src, (size_t)(nBytes), (const unsigned int*)m_key, (const unsigned int*)m_nonce);
			m_nonce[0] += (sl_uint32)nBlocks;
		}
		pos = (sl_uint32)(len & 0x3F);
		if (pos) {
			src += nBytes;
			dst += nBytes;
			Base::zeroMemory(lastBlock, 64);
			ChaCha20_ctr32((unsigned char*)lastBlock, (const unsigned char*)lastBlock, 64, (const unsigned int*)m_key, (const unsigned int*)m_nonce);
			m_nonce[0]++;
			for (sl_uint32 i = 0; i < pos; i++) {
				dst[i] = lastBlock[i] ^ src[i];
			}
			m_pos = pos;
		}
	}
	
	void OpenSSL_ChaCha20::decrypt(const void* src, void* dst, sl_size len)
	{
		encrypt(src, dst, len);
	}
	
	
	OpenSSL_Poly1305::OpenSSL_Poly1305(): m_context(sl_null)
	{
	}
	
	OpenSSL_Poly1305::~OpenSSL_Poly1305()
	{
		if (m_context) {
			Base::freeMemory(m_context);
		}
	}
	
	void OpenSSL_Poly1305::start(const void* key)
	{
		if (!m_context) {
			m_context = Base::createMemory((sl_size)(Poly1305_ctx_size()));
		}
		Poly1305_Init((POLY1305*)m_context, (const unsigned char*)key);
	}
	
	void OpenSSL_Poly1305::update(const void* input, sl_size n)
	{
		Poly1305_Update((POLY1305*)m_context, (const unsigned char*)input, (size_t)n);
	}
	
	void OpenSSL_Poly1305::finish(void* output)
	{
		Poly1305_Final((POLY1305*)m_context, (unsigned char*)output);
	}
	
	void OpenSSL_Poly1305::execute(const void* key, const void* message, sl_size lenMessage, void* output)
	{
		OpenSSL_Poly1305 p;
		p.start(key);
		p.update(message, lenMessage);
		p.finish(output);
	}
	
	
	OpenSSL_ChaCha20_Poly1305::OpenSSL_ChaCha20_Poly1305()
	{
	}
	
	OpenSSL_ChaCha20_Poly1305::~OpenSSL_ChaCha20_Poly1305()
	{
	}
	
	void OpenSSL_ChaCha20_Poly1305::setKey(const void* key)
	{
		m_cipher.setKey(key);
	}
	
	void OpenSSL_ChaCha20_Poly1305::start(sl_uint32 senderId, const void* _iv)
	{
		const sl_uint8* iv = (const sl_uint8*)_iv;
		sl_uint32 n0 = SLIB_MAKE_DWORD(iv[3], iv[2], iv[1], iv[0]);
		sl_uint32 n1 = SLIB_MAKE_DWORD(iv[7], iv[6], iv[5], iv[4]);
		m_cipher.start(1, senderId, n0, n1);
		sl_uint8 block0[64];
		m_cipher.generateBlock(0, senderId, n0, n1, block0);
		m_auth.start(block0); // use first 32 bytes
		m_lenAAD = 0;
		m_lenInput = 0;
	}
	
	void OpenSSL_ChaCha20_Poly1305::putAAD(const void* data, sl_size len)
	{
		m_auth.update(data, len);
		m_lenAAD += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::finishAAD()
	{
		sl_uint32 n = (sl_uint32)(m_lenAAD & 15);
		if (n) {
			static const sl_uint8 zeros[16] = {0};
			m_auth.update(zeros, 16 - n);
		}
	}
	
	void OpenSSL_ChaCha20_Poly1305::encrypt(const void* src, void* dst, sl_size len)
	{
		m_cipher.encrypt(src, dst, len);
		m_auth.update(dst, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::decrypt(const void* src, void* dst, sl_size len)
	{
		m_auth.update(src, len);
		m_cipher.encrypt(src, dst, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::check(const void* src, sl_size len)
	{
		m_auth.update(src, len);
		m_lenInput += len;
	}
	
	void OpenSSL_ChaCha20_Poly1305::finish(void* outputTag)
	{
		sl_uint32 n = (sl_uint32)(m_lenInput & 15);
		if (n) {
			static const sl_uint8 zeros[16] = {0};
			m_auth.update(zeros, 16 - n);
		}
		sl_uint8 len[16];
		MIO::writeUint64LE(len, m_lenAAD);
		MIO::writeUint64LE(len + 8, m_lenInput);
		m_auth.update(len, 16);
		m_auth.finish(outputTag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::finishAndCheckTag(const void* _tag)
	{
		const sl_uint8* tag = (const sl_uint8*)_tag;
		sl_uint8 outputTag[16];
		finish(outputTag);
		sl_uint32 n = 0;
		for (sl_uint32 i = 0; i < 16; i++) {
			n |= (outputTag[i] ^ tag[i]);
		}
		return n == 0;
	}
	
	void OpenSSL_ChaCha20_Poly1305::encrypt(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, void* outputTag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			encrypt(src, dst, len);
		}
		finish(outputTag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::decrypt(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, void* dst, sl_size len, const void* tag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			decrypt(src, dst, len);
		}
		return finishAndCheckTag(tag);
	}
	
	sl_bool OpenSSL_ChaCha20_Poly1305::check(sl_uint32 senderId, const void* iv, const void* AAD, sl_size lenAAD, const void* src, sl_size len, const void* tag)
	{
		start(senderId, iv);
		if (lenAAD) {
			putAAD(AAD, lenAAD);
			finishAAD();
		}
		if (len) {
			check(src, len);
		}
		return finishAndCheckTag(tag);
	}
	
	
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
	
	
	Ref<OpenSSL_Context> OpenSSL::createContext(const TlsContextParam& param)
	{
		return Ref<OpenSSL_Context>::from(ContextImpl::create(param));
	}

	Ref<OpenSSL_AsyncStream> OpenSSL::connectStream(const Ref<AsyncStream>& baseStream, const TlsConnectStreamParam& param)
	{
		return Ref<OpenSSL_AsyncStream>::from(StreamImpl::connectStream(baseStream, param));
	}
	
	Ref<OpenSSL_AsyncStream> OpenSSL::acceptStream(const Ref<AsyncStream>& baseStream, const TlsAcceptStreamParam& param)
	{
		return Ref<OpenSSL_AsyncStream>::from(StreamImpl::acceptStream(baseStream, param));
	}

	
	sl_bool OpenSSL::isProbablePrime(const void* num_BigEndian, sl_uint32 nBytes, sl_bool* pFlagError)
	{
		initThread();
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
		initThread();
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
		initThread();
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

	void OpenSSL::generateRSA(RSAPrivateKey& key, sl_uint32 nBits)
	{
		initThread();
		sl_uint32 h = nBits >> 1;
		nBits = h << 1;
		for (;;) {
			key.P = BigInt::fromBytesBE(generatePrime(h));
			key.Q = BigInt::fromBytesBE(generatePrime(h));
			if (key.generateFromPrimes(nBits)) {
				return;
			}
		}
	}
	
	sl_bool OpenSSL::generate_ECKey_secp256k1(ECPrivateKey& _output)
	{
		initThread();
		sl_bool flagSuccess = sl_false;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			EC_KEY* key = EC_KEY_new();
			if (key) {
				flagSuccess = sl_true;
				EC_KEY_set_group(key, group);
				for (;;) {
					if (1 == EC_KEY_generate_key(key)) {
						const BIGNUM* p = EC_KEY_get0_private_key(key);
						_output.d = get_BigInt_from_BIGNUM(p);
						if (_output.d.isNull()) {
							flagSuccess = sl_false;
							break;
						}
						const EC_POINT* Q = EC_KEY_get0_public_key(key);
						_output.Q = get_ECPoint_from_EC_POINT(group, Q);
						if (_output.Q.isO()) {
							flagSuccess = sl_false;
							break;
						}
						break;
					}
				}
				EC_KEY_free(key);
			}
			EC_GROUP_free(group);
		}
		return flagSuccess;
	}
	
	sl_bool OpenSSL::check_ECKey_secp256k1(const ECPublicKey& key)
	{
		initThread();
		sl_bool flagSuccess = sl_false;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			EC_KEY* ek = get_EC_KEY_from_ECPublicKey(group, key);
			if (ek) {
				flagSuccess = EC_KEY_check_key(ek) == 1;
				EC_KEY_free(ek);
			}
			EC_GROUP_free(group);
		}
		return flagSuccess;
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_secp256k1(const ECPrivateKey& key, const BigInt& z)
	{
		Memory mem = z.getBytesBE();
		if (mem.isNull()) {
			return ECDSA_Signature();
		}
		return do_sign_ECDSA_secp256k1(key, mem.getData(), mem.getSize());
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_secp256k1(const ECPrivateKey& key, const void* hash, sl_size size)
	{
		return do_sign_ECDSA_secp256k1(key, hash, size);
	}
	
	ECDSA_Signature OpenSSL::sign_ECDSA_SHA256_secp256k1(const ECPrivateKey& key, const void* data, sl_size size)
	{
		sl_uint8 hash[SHA256::HashSize];
		SHA256::hash(data, size, hash);
		return do_sign_ECDSA_secp256k1(key, hash, SHA256::HashSize);
	}
	
	sl_bool OpenSSL::verify_ECDSA_secp256k1(const ECPublicKey& key, const BigInt& z, const ECDSA_Signature& signature)
	{
		Memory mem = z.getBytesBE();
		if (mem.isNull()) {
			return sl_false;
		}
		return do_verify_ECDSA_secp256k1(key, mem.getData(), mem.getSize(), signature);
	}
	
	sl_bool OpenSSL::verify_ECDSA_secp256k1(const ECPublicKey& key, const void* hash, sl_size size, const ECDSA_Signature& signature)
	{
		return do_verify_ECDSA_secp256k1(key, hash, size, signature);
	}
	
	sl_bool OpenSSL::verify_ECDSA_SHA256_secp256k1(const ECPublicKey& key, const void* data, sl_size size, const ECDSA_Signature& signature)
	{
		sl_uint8 hash[SHA256::HashSize];
		SHA256::hash(data, size, hash);
		return do_verify_ECDSA_secp256k1(key, hash, SHA256::HashSize, signature);
	}
	
	BigInt OpenSSL::getSharedKey_ECDH_secp256k1(const ECPrivateKey& keyLocal, const ECPublicKey& keyRemote)
	{
		initThread();
		BigInt ret;
		EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_secp256k1);
		if (group) {
			BIGNUM* priv = get_BIGNUM_from_BigInt(keyLocal.d);
			if (priv) {
				EC_POINT* pub = get_EC_POINT_from_ECPoint(group, keyRemote.Q);
				if (pub) {
					EC_POINT* pt = EC_POINT_new(group);
					if (pt) {
						if (EC_POINT_mul(group, pt, sl_null, pub, priv, sl_null)) {
							BIGNUM* x = BN_new();
							if (x) {
								if (EC_POINT_get_affine_coordinates(group, pt, x, sl_null, sl_null)) {
									ret = get_BigInt_from_BIGNUM(x);
								}
								BN_free(x);
							}
						}
						EC_POINT_free(pt);
					}
					EC_POINT_free(pub);
				}
				BN_free(priv);
			}
			EC_GROUP_free(group);
		}
		return ret;
	}
	
}
