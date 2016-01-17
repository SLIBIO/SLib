#ifndef CHECKHEADER_SLIB_CRYPTO_SECURE_STREAM
#define CHECKHEADER_SLIB_CRYPTO_SECURE_STREAM

#include "definition.h"

/**************************************************************
	SecureStream - for securly transfer data between server and
		clients via AsyncStream or IStream
		(as TLS - Transport Layer Security)

Used Algorithms
	- AES-256
	- RSA PKCS#1 v2.1 (OAEP/SHA-256, Label='SecureStream', KeySize >= 2048)
	- GCM (Galois/Counter Mode)
	- CTM (Counter Mode)

[Client->Server]

Authentication Part

	Encrypted by AES-256/GCM with [Server Master Key](SHA-256 Hash)

	Plain-Section (16 bytes), Encrypted by AES-256/ECB with [Server Master Key] || 'SecureStream' (SHA-256 Hash)
		- Header IV : 14 bytes filled by random bytes
		- Length of Encryption Section: 2 bytes, Little Endian

	Encrypted-Section
		- Version: 2 bytes, Little Endian ( = 1)
		- RSA Block (encrypted by RSAES_OAEP_SHA256)
			* Session Key: 32 bytes (Random Bytes)
			* Message IV: 8 bytes filled by random bytes
	Tag (16 bytes)

Content Part (Encrypted by AES-256/CTR with [Session Key])

[Server->Client]
	
Authentication Part

	Encrypted by AES-256/GCM with [Session Key]
	
	Plain-Section (16 bytes)
		- Header IV : 16 bytes filled by random bytes

	Encrypted-Section (32 bytes)
		- Version: 2 bytes, Little Endian ( = 1)
		- Message IV: 8 bytes filled by random bytes
		- Random Bytes Padding
	
	Tag (16 bytes)

Content Part (Encrypted by AES-256/CTR with [Session Key])

***************************************************************/

#include "../core/async.h"

#include "rsa.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT SecureStream : public Object, public IStream, public IClosable
{
public:
	/*
		Return Value
			>0 : Connected
			=0 : Not Connected Yet
			<0 : Error
	*/
	virtual sl_int32 connect() = 0;
	
	virtual sl_bool isConnected() = 0;

public:
	static const RSAPrivateKey& getDefaultPrivateKey();
	
	static const RSAPublicKey& getDefaultPublicKey();
};

class AsyncSecureStream;
class SLIB_EXPORT IAsyncSecureStreamListener
{
public:
	virtual void onConnectedSecureStream(AsyncSecureStream* securedStream, sl_bool flagError) = 0;
};

class SLIB_EXPORT AsyncSecureStream : public AsyncStream
{
public:
	virtual sl_bool connect() = 0;
	
	virtual sl_bool isConnected() = 0;
	
public:
	const Ptr<IAsyncSecureStreamListener>& getListener() const
	{
		return m_listener;
	}

protected:
	void onConnected(sl_bool flagError);
	
protected:
	Ptr<IAsyncSecureStreamListener> m_listener;
	
};

class SLIB_EXPORT SecureStreamServerParam
{
public:
	String masterKey;
	RSAPrivateKey privateKey;

public:
	SecureStreamServerParam();

public:
	sl_bool checkParam() const;
};

class SLIB_EXPORT AsyncSecureStreamServerParam : public SecureStreamServerParam
{
public:
	Ptr<IAsyncSecureStreamListener> listener;
};

class SLIB_EXPORT SecureStreamServer
{
public:
	static Ref<SecureStream> createStream(const Ptr<IStream>& baseStream, const SecureStreamServerParam& param);
	static Ref<AsyncSecureStream> createAsyncStream(const Ref<AsyncStream>& baseStream, const AsyncSecureStreamServerParam& param, sl_bool flagConnect = sl_true);
};

class SLIB_EXPORT SecureStreamClientParam
{
public:
	String masterKey;
	RSAPublicKey publicKey;

public:
	SecureStreamClientParam();

public:
	sl_bool checkParam() const;
};

class SLIB_EXPORT AsyncSecureStreamClientParam : public SecureStreamClientParam
{
public:
	Ptr<IAsyncSecureStreamListener> listener;
};

class SLIB_EXPORT SecureStreamClient
{
public:
	static Ref<SecureStream> createStream(const Ptr<IStream>& baseStream, const SecureStreamClientParam& param);
	static Ref<AsyncSecureStream> createAsyncStream(const Ref<AsyncStream>& baseStream, const AsyncSecureStreamClientParam& param, sl_bool flagConnect = sl_true);
};

SLIB_CRYPTO_NAMESPACE_END

#endif
