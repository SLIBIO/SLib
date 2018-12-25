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

#ifndef CHECKHEADER_SLIB_NETWORK_STUN
#define CHECKHEADER_SLIB_NETWORK_STUN

#include "definition.h"

#include "socket_address.h"
#include "async.h"

#include "../core/memory.h"

/********************************************************************
           Session Traversal Utilities for NAT (STUN)
          RFC-5389: https://tools.ietf.org/html/rfc5389

 - STUN Message Header
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |0 0|     STUN Message Type     |         Message Length        |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                         Magic Cookie                          |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                                                               |
 |                     Transaction ID (96 bits)                  |
 |                                                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
 
 - Format of STUN Message Type Field
 0                 1
 2  3  4 5 6 7 8 9 0 1 2 3 4 5
 
 +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
 |M |M |M|M|M|C|M|M|M|C|M|M|M|M|
 |11|10|9|8|7|1|6|5|4|0|3|2|1|0|
 +--+--+-+-+-+-+-+-+-+-+-+-+-+-+

 - STUN Attributes
 After the STUN header are zero or more attributes.
 
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |         Type                  |            Length             |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                         Value (variable)                ....
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 
   MAPPED-ADDRESS
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |0 0 0 0 0 0 0 0|    Family     |           Port                |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                                                               |
 |                 Address (32 bits or 128 bits)                 |
 |                                                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 
   XOR-MAPPED-ADDRESS
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |x x x x x x x x|    Family     |         X-Port                |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                X-Address (Variable)
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 
   ERROR-CODE
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |           Reserved, should be 0         |Class|     Number    |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |      Reason Phrase (variable)                                ..
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   UNKNOWN-ATTRIBUTES
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |      Attribute 1 Type           |     Attribute 2 Type        |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |      Attribute 3 Type           |     Attribute 4 Type    ...
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 
********************************************************************/

#define SLIB_NETWORK_STUN_PORT 3478
#define SLIB_NETWORK_STUN_MAGIC_COOKIE 0x2112A442

namespace slib
{
	
	enum class StunMessageClass
	{
		Request = 0,
		Indication = 1,
		Response = 2,
		ErrorResponse = 3
	};

	enum class StunMethod
	{
		Binding = 1,
	};
	
	enum class StunAttributeType
	{
		MappedAddress = 0x0001,
		UserName = 0x0006,
		MessageIntegrity = 0x0008,
		ErrorCode = 0x0009,
		UnknownAttributes = 0x000A,
		Realm = 0x0014,
		Nonce = 0x0015,
		XorMappedAddress = 0x0020,
		
		Software = 0x8022,
		AlternateServer = 0x8023,
		Fingerprint = 0x8028
	};
	
	enum class StunErrorCode
	{
		Success = 0,
		TryAlternate = 300, // The client should contact an alternate server for this request.  This error response MUST only be sent if the request included a USERNAME attribute and a valid MESSAGE-INTEGRITY attribute; otherwise, it MUST NOT be sent and error code 400 (Bad Request) is suggested.  This error response MUST be protected with the MESSAGE-INTEGRITY attribute, and receivers MUST validate the MESSAGE-INTEGRITY of this response before redirecting themselves to an alternate server.
		BadRequest = 400, // The request was malformed.  The client SHOULD NOT retry the request without modification from the previous attempt.  The server may not be able to generate a valid MESSAGE-INTEGRITY for this error, so the client MUST NOT expect a valid MESSAGE-INTEGRITY attribute on this response.
		Unauthorized = 401, // The request did not contain the correct credentials to proceed.  The client should retry the request with proper credentials.
		UnknownAttribute = 420, // The server received a STUN packet containing a comprehension-required attribute that it did not understand. The server MUST put this unknown attribute in the UNKNOWN-ATTRIBUTE attribute of its error response.
		StaleNonce = 438, // The NONCE used by the client was no longer valid. The client should retry, using the NONCE provided in the response.
		ServerError = 500 // The server has suffered a temporary error.  The client should try again.
	};
	
	class SLIB_EXPORT StunAttributes
	{
	public:
		SocketAddress mappedAddress;

		SocketAddress xorMappedAddress;
		
		String userName; // less than 513 bytes
		String password;
		
		String realm; // less than 763 characters, Presence of the REALM attribute in a request indicates that long-term credentials are being used for authentication.
		
		String nonce; // less than 763 characters
		
		StunErrorCode errorCode;
		String errorReasonPhase; // less than 763 characters
		
		List<sl_uint16> unknownAttributes;
		List<sl_uint16> unknownAttributesDuringParsing;

		String software; // less than 763 characters
		
		List<SocketAddress> alternateServers;
		
		sl_bool flagHaveMessageIntegrity;
		sl_size offsetMessageIntegrityAttribute;
		
		sl_bool flagHaveFingerprint;
		
	public:
		StunAttributes();
		
	public:
		void initialize();
		
	};
	
	class SLIB_EXPORT StunPacket
	{
	public:
		enum
		{
			HeaderSize = 20
		};

	public:
		StunMessageClass getMessageClass() const;
		
		void setMessageClass(StunMessageClass _class);
		
		StunMethod getMethod() const;
		
		void setMethod(StunMethod method);
		
		sl_uint16 getMessageLength() const;
		
		void setMessageLength(sl_uint16 length);
		
		sl_uint32 getMagicCookie() const;
		
		void setMagicCookie(sl_uint32 cookie = SLIB_NETWORK_STUN_MAGIC_COOKIE);
		
		const sl_uint8* getTransactionID() const;
		
		sl_uint8* getTransactionID();

		const void* getAttributes() const;

		void* getAttributes();

		
		static sl_bool checkHeader(const void* packet, sl_size size);
		
		sl_bool parseAttributes(sl_size packetSize, StunAttributes& attributes) const;
		
		sl_bool checkMessageIntegrity(const StunAttributes& attributes) const;
		
		static Memory buildPacket(StunMessageClass _class, StunMethod method, const void* transactionID /* 12 bytes */, const StunAttributes& attributes);
		
		
		static void writeAttributeHeader(StunAttributeType type, sl_size len, void* data);
		
		static sl_size writeStringAttribute(StunAttributeType type, const String& str, void* data);
		
		static sl_bool readMappedAddressAttributeValue(const void* data, sl_size size, SocketAddress& mappedAddress);
		
		static sl_size writeMappedAddressAttribute(StunAttributeType type, const SocketAddress& mappedAddress, void* data);

		sl_bool readXorMappedAddressAttributeValue(const void* data, sl_size size, SocketAddress& mappedAddress) const;
		
		sl_size writeXorMappedAddressAttribute(const SocketAddress& mappedAddress, void* data) const;
		
		static sl_bool readErrorCodeAttributeValue(const void* data, sl_size size, StunErrorCode& errorCode, String& errorReasonPhase);
		
		static sl_size writeErrorCodeAttribute(StunErrorCode errorCode, const String& errorReasonPhase, void* data);
		
		static sl_bool readUnknownAttributesAttributeValue(const void* data, sl_size size, List<sl_uint16>& unknownAttributes);
		
		static sl_size writeUnknownAttributesAttribute(const List<sl_uint16>& unknownAttributes, void* data);
		
		static void calculateMessageIntegrity(void* output /* 20 bytes */, const void* packet, sl_size size, const String& userName, const String& realm, const String& password);
		
		static void calculateFingerprint(void* output /* 4 bytes */, const void* packet, sl_size size);

	public:
		sl_uint8 _messageType[2];
		sl_uint8 _messageLength[2];
		sl_uint8 _magicCookie[4];
		sl_uint8 _transactionID[12];
		
	};
	
	
	class SLIB_EXPORT StunServerParam
	{
	public:
		sl_uint16 port;
		sl_bool flagAutoStart;
		sl_bool flagLogging;
		
		Ref<AsyncIoLoop> ioLoop;
		
	public:
		StunServerParam();
		
		~StunServerParam();
		
	};
	
	class SLIB_EXPORT StunServer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		StunServer();
		
		~StunServer();
		
	public:
		static Ref<StunServer> create(const StunServerParam& param);
		
	public:
		void release();
		
		void start();
		
		sl_bool isRunning();
		
	protected:
		void _onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& address, void* data, sl_uint32 sizeReceive);
		
	private:
		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		sl_bool m_flagLogging;
		
		Ref<AsyncUdpSocket> m_udp;
		
	};
	
}

#endif
