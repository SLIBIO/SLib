#ifndef CHECKHEADER_SLIB_P2P_HNS_SERVICE
#define CHECKHEADER_SLIB_P2P_HNS_SERVICE

#include "definition.h"

/*
	HNS - Host Name System

 __________________
|  UDP Section     |
--------------------
Packet Format:

All packets are leaded by SHA-256 Hash

	Hash (hash of Content-Region) = 4 bytes
	Nonce, MethodId = 4 bytes (28 bits, 4 bits)
	Content-Region

* Using Little Endian

(1) Level 1

- Initialize Session
Request: (MethodId = 1)
	Body [
		ClientId = String-64
		ClientCertificate = String-4096
	] AES-256-CBC-PKCS7Padding by ServiceSecret

Response: (MethodId = 2)
	Body [
		SessionId = 8 bytes
		ClientAddress = String-64
	] AES-256-CBC-PKCS7Padding by SessionSecret

- Send Data (Client->Service) (MethodId = 3)
	SessionId = 8 bytes
	Body [
		Content-Hash = 4 bytes
		Content (Level-2)
	] AES-256-CBC-PKCS7Padding by SessionSecret

- Send Data (Service->Client) (MethodId = 4)
	SessionId = 8 bytes
	Body [
		Content-Hash = 4 bytes
		Content (Level-2)
	] AES-256-CBC-PKCS7Padding by SessionSecret

- Send Error (MethodId = 5)
	None
	* only sevice->client

(2) Level 2
Based on initialized session

MethodId = (1 byte)
Content

- KeepAlive
Request: (L2-MethodId = 1)
	RequestNumber = 4 bytes
	Client's SentTime = 8 bytes

Response: (L2-MethodId = 2)
	RequestNumber = 4 bytes
	Client's SentTime = 8 bytes
	Service's SentTime = 8 bytes
	ClientAddress = String-64

- Query Host
Request: (L2-MethodId = 3)
	ClientId = String-64

Response: (L2-MethodId = 4)
	ClientId = String-64
	ClientAddress = String-64  (Empty String - Not Found)
	elapsedMillisecondsFromLastCommunicated = 8 bytes

- Send Message
Sender->Server: (L2-MethodId = 11)
	TargetId = String-64
	MessageId = 8 bytes
	Content

Server->Target: (L2-MethodId = 12)
	SenderId = String-64
	MessageId = 8 bytes
	Content

- Broadcast Message
Sender->Server: (L2-MethodId = 13)
	MessageId = 8 bytes
	Content

Server->Sender: (L2-MethodId = 14)
	SenderId = String-64
	MessageId = 8 bytes
	Content

*/

#include "../core/object.h"
#include "../core/string.h"
#include "../core/time.h"
#include "../core/pointer.h"
#include "../crypto/rsa.h"

#include "../network/address.h"
#include "../network/socket.h"

SLIB_P2P_NAMESPACE_BEGIN

class HnsService;

class SLIB_EXPORT HnsServiceSession : public Referable
{
public:
	String clientId;
	sl_uint64 sessionId;
	char secret[32]; // AES-256

	SocketAddress udpSessionAddress;
	Time timeLastCommunicatedUdp;

	HnsServiceSession()
	{
		sessionId = 0;
		timeLastCommunicatedUdp = Time::zero();
	}
};

class SLIB_EXPORT HnsServiceUdpSessionInfo
{
public:
	String clientId;
	String clientCertificate;
	SocketAddress clientAddress;
};

class SLIB_EXPORT IHnsServiceListener
{
public:
	// returns sl_true when the client is acceptable
	virtual sl_bool onAuthenticateUdp(HnsService* service, const HnsServiceUdpSessionInfo& info)
	{
		return sl_true;
	}

	// returns sl_true when the query is acceptable
	virtual sl_bool onQueryHostUdp(
		HnsService* service
		, HnsServiceSession* sender, HnsServiceSession* target)
	{
		return sl_true;
	}

	// returns sl_true when the sending message is acceptable
	virtual sl_bool onMessageUdp(
		HnsService* service
		, HnsServiceSession* sender, HnsServiceSession* target
		, sl_uint64 messageId, const void* message, sl_uint32 size)
	{
		return sl_true;
	}

	// returns sl_true when the broadcast is acceptable
	virtual sl_bool onBroadcastMessageUdp(
		HnsService* service
		, HnsServiceSession* sender
		, sl_uint64 messageId, const void* message, sl_uint32 size)
	{
		return sl_false;
	}

	virtual void onKeepAliveUdp(HnsService* service, HnsServiceSession* client, Time clientTimeSent) {}
};

class SLIB_EXPORT HnsServiceParam
{
public:
	sl_uint32 portUdp;
	String secret;

	Ptr<IHnsServiceListener> listener;

	RSAPrivateKey privateKey;

	SLIB_INLINE HnsServiceParam()
	{
		portUdp = 0;
		secret = "HNS_SERVICE";
	}
};

class SLIB_EXPORT HnsService : public Object
{
protected:
	SLIB_INLINE HnsService()
	{
	}

public:
	virtual void release() = 0;

	virtual sl_bool isRunning() = 0;

	virtual Ref<HnsServiceSession> getClientSession(String clientId) = 0;

public:
	SLIB_PROPERTY_INLINE(Ptr<IHnsServiceListener>, Listener)

public:
	static Ref<HnsService> start(const HnsServiceParam& param);
};

SLIB_P2P_NAMESPACE_END

#endif

