#ifndef CHECKHEADER_SLIB_NETWORK_ICMP
#define CHECKHEADER_SLIB_NETWORK_ICMP

/********************************************************************
				
				INTERNET CONTROL MESSAGE PROTOCOL

				https://tools.ietf.org/html/rfc792

	
	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Type      |     Code      |          Checksum             |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|						Rest of Header						    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|     Data ...
	+-+-+-+-+-

 - Types

 * Echo/Echo Reply Type

		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|           Identifier          |        Sequence Number        |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Data ...
		+-+-+-+-+-

		Type
			0 = Echo Reply
			8 = Echo

		Code = 0

 * Destination Unreachable Message Type

		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                             unused                            |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      Internet Header + 64 bits of Original Data Datagram      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		Type = 3

		Code
			0 = net unreachable;
			1 = host unreachable;
			2 = protocol unreachable;
			3 = port unreachable;
			4 = fragmentation needed and DF set;
			5 = source route failed.

 * Redirect Message
		
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                 Gateway Internet Address                      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      Internet Header + 64 bits of Original Data Datagram      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		Type =	5

		Code
			0 = Redirect datagrams for the Network.
			1 = Redirect datagrams for the Host.
			2 = Redirect datagrams for the Type of Service and Network.
			3 = Redirect datagrams for the Type of Service and Host.

 * Time Exceeded Message
		
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|                             unused                            |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      Internet Header + 64 bits of Original Data Datagram      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		Type = 11

		Code
			0 = time to live exceeded in transit;
			1 = fragment reassembly time exceeded.


 * Parameter Problem Message

		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |     Code      |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|    Pointer    |                   unused                      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|      Internet Header + 64 bits of Original Data Datagram      |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		Type = 12

		Code = 0


 * Timestamp or Timestamp Reply Message
		
		0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Type      |      Code     |          Checksum             |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|           Identifier          |        Sequence Number        |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Originate Timestamp                                       |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Receive Timestamp                                         |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		|     Transmit Timestamp                                        |
		+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

		Type
			13 = timestamp message;
			14 = timestamp reply message.

		Code = 0

********************************************************************/

#include "definition.h"

#include "tcpip.h"

SLIB_NETWORK_NAMESPACE_BEGIN

enum IcmpType
{
	icmpType_EchoReply = 0,
	icmpType_DestinationUnreachable = 3,
	icmpType_Redirect = 5,
	icmpType_Echo = 8,
	icmpType_TimeExceeded = 11,
	icmpType_ParameterProblem = 12,
	icmpType_Timestamp = 13,
	icmpType_TimestampReply = 14
};

class SLIB_EXPORT IcmpHeaderFormat
{
public:
	SLIB_INLINE IcmpType getType() const
	{
		return (IcmpType)_type;
	}
    
	SLIB_INLINE void setType(IcmpType type)
	{
		_type = (sl_uint8)type;
	}

    
	SLIB_INLINE sl_uint8 getCode() const
	{
		return _code;
	}
    
	SLIB_INLINE void setCode(sl_uint8 code)
	{
		_code = code;
	}

    
	SLIB_INLINE sl_uint16 getChecksum() const
	{
		return MIO::readUint16BE(_checksum);
	}
    
	SLIB_INLINE void setChecksum(sl_uint16 checksum)
	{
		MIO::writeUint16BE(_checksum, checksum);
	}

    
	void updateChecksum(sl_uint32 sizeContent);
    
	sl_bool checkChecksum(sl_uint32 sizeContent) const;

    
	SLIB_INLINE sl_uint16 getEchoIdentifier() const
	{
		return MIO::readUint16BE(_rest);
	}

	SLIB_INLINE void setEchoIdentifier(sl_uint16 id)
	{
		MIO::writeUint16BE(_rest, id);
	}

    
	SLIB_INLINE sl_uint16 getEchoSequenceNumber() const
	{
		return MIO::readUint16BE(_rest + 2);
	}

	SLIB_INLINE void setEchoSequenceNumber(sl_uint16 sn)
	{
		MIO::writeUint16BE(_rest + 2, sn);
	}

    
	SLIB_INLINE IPv4Address getRedirectGatewayAddress() const
	{
		return IPv4Address(_rest);
	}

	SLIB_INLINE void setRedirectGatewayAddress(const IPv4Address& address)
	{
		address.get(_rest);
	}

    
	SLIB_INLINE sl_uint8 getParameterProblemPointer() const
	{
		return _rest[0];
	}

	SLIB_INLINE void setParameterProblemPointer(sl_uint8 pointer)
	{
		_rest[0] = pointer;
	}

    
	SLIB_INLINE sl_uint16 getTimestampIdentifier() const
	{
		return MIO::readUint16BE(_rest);
	}

	SLIB_INLINE void setTimestampIdentifier(sl_uint16 id)
	{
		MIO::writeUint16BE(_rest, id);
	}

    
	SLIB_INLINE sl_uint16 getTimestampSequenceNumber() const
	{
		return MIO::readUint16BE(_rest + 2);
	}

	SLIB_INLINE void setTimestampSequenceNumber(sl_uint16 sn)
	{
		MIO::writeUint16BE(_rest + 2, sn);
	}

    
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return (const sl_uint8*)(this) + sizeof(sizeof(IcmpHeaderFormat));
	}
    
	SLIB_INLINE sl_uint8* getContent()
	{
		return (sl_uint8*)(this) + sizeof(sizeof(IcmpHeaderFormat));
	}

    
	sl_bool check(sl_uint32 sizeContent) const;

private:
	sl_uint8 _type;
	sl_uint8 _code;
	sl_uint8 _checksum[2];
	sl_uint8 _rest[4];
};

class SLIB_EXPORT IcmpEchoAddress
{
public:
	IPv4Address ip;
	sl_uint16 identifier;
	sl_uint16 sequenceNumber;

public:
	sl_bool operator==(const IcmpEchoAddress& other) const;
    
	SLIB_INLINE sl_bool operator!=(const IcmpEchoAddress& other) const
	{
		return !(*this == other);
	}

	int compare(const IcmpEchoAddress& other) const;

	sl_uint32 hashCode() const;
};

template <>
SLIB_INLINE int Compare<IcmpEchoAddress>::compare(const IcmpEchoAddress& a, const IcmpEchoAddress& b)
{
	return a.compare(b);
}

template <>
SLIB_INLINE sl_bool Compare<IcmpEchoAddress>::equals(const IcmpEchoAddress& a, const IcmpEchoAddress& b)
{
	return a == b;
}

template <>
SLIB_INLINE sl_uint32 Hash<IcmpEchoAddress>::hash(const IcmpEchoAddress& a)
{
	return a.hashCode();
}

SLIB_NETWORK_NAMESPACE_END

#endif
