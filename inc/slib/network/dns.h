#ifndef CHECKHEADER_SLIB_NETWORK_DNS
#define CHECKHEADER_SLIB_NETWORK_DNS

#include "definition.h"

#include "socket_address.h"
#include "async.h"

#include "../core/mio.h"
#include "../core/string.h"

/********************************************************************
	DNS Specification from RFC 1035, RFC 1034, RFC 2535

- Messages sent using UDP/TCP user server port 53 (decimal)

- Message Format
    +---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+

- Header Format
	
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA| Z|AD|CD|   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

- Quetion Record
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

- RR (Resource Record) Format
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

- Name Format
	Label.Label.Label. ... .Label

	Domain names in messages are expressed in terms of a sequence of labels.
	Each label is represented as a one octet length field followed by that
	number of octets.  Since every domain name ends with the null label of
	the root, a domain name is terminated by a length byte of zero.  The
	high order two bits of every length octet must be zero, and the
	remaining six bits of the length field limit the label to 63 octets or
	less.

- Message compression
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    | 1  1|                OFFSET                   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	In this scheme, an entire domain name or a list of labels at
	the end of a domain name is replaced with a pointer to a prior occurance
	of the same name.

********************************************************************/

#define SLIB_NETWORK_DNS_PORT 53
#define SLIB_NETWORK_DNS_LABEL_MAX_LENGTH 63
#define SLIB_NETWORK_DNS_NAME_MAX_LENGTH 255
#define SLIB_NETWORK_DNS_RECORD_HEADER_MAX_LENGTH 263

SLIB_NETWORK_NAMESPACE_BEGIN
    
enum DnsOpcode
{
    dnsOpcode_Query = 0,
    dnsOpcode_InverseQuery = 1,
    dnsOpcode_ServerStatusRequest = 2
};

enum DnsResponseCode
{
    dnsResponseCode_NoError = 0,
    dnsResponseCode_FormatError = 1, // The name server was unable to interpret the query
    dnsResponseCode_ServerFailure = 2, // The name server was unable to process this query due to a problem with the name server
    dnsResponseCode_NameError = 3, // Meaningful only for responses from an authoritative name server, this code signifies that the domain name referenced in the query does not exist
    dnsResponseCode_NotImplemented = 4, // The name server does not support the requested kind of query
    dnsResponseCode_Refused = 5 // The name server refuses to perform the specified operation for policy reasons
};

enum DnsRecordType
{
    dnsRecordType_A = 1, // a host address
    dnsRecordType_NS = 2, // an authoritative name server
    dnsRecordType_MD = 3, // a mail destination (Obsolete - use MX)
    dnsRecordType_MF = 4, // a mail forwarder (Obsolete - use MX)
    dnsRecordType_CNAME = 5, // the canonical name for an alias
    dnsRecordType_SOA = 6, // marks the start of a zone of authority
    dnsRecordType_MB = 7, // a mailbox domain name (EXPERIMENTAL)
    dnsRecordType_MG = 8, // a mail group member (EXPERIMENTAL)
    dnsRecordType_MR = 9, // a mail rename domain name (EXPERIMENTAL)
    dnsRecordType_NULL = 10, // a null RR (EXPERIMENTAL)
    dnsRecordType_WKS = 11, // a well known service description
    dnsRecordType_PTR = 12, // a domain name pointer
    dnsRecordType_HINFO = 13, // host information
    dnsRecordType_MINFO = 14, // mailbox or mail list information
    dnsRecordType_MX = 15, // mail exchange
    dnsRecordType_TXT = 16, // text strings
    dnsRecordType_Question_AXFR = 252, // A request for a transfer of an entire zone
    dnsRecordType_Question_MAILB = 253, // A request for mailbox-related records (MB, MG or MR)
    dnsRecordType_Question_MAILA = 254, // A request for mail agent RRs (Obsolete - see MX)
    dnsRecordType_Question_ALL = 255 // A request for all records
};

enum DnsClass
{
    dnsClass_IN = 1, // the Internet
    dnsClass_CS = 2, // the CSNET class (Obsolete - used only for examples in some obsolete RFCs)
    dnsClass_CH = 3, // the CHAOS class
    dnsClass_HS = 4, // Hesiod [Dyer 87]
    dnsClass_Question_ANY = 255 // any class
};

class SLIB_EXPORT DnsHeaderFormat
{
public:
	SLIB_INLINE sl_uint16 getId() const
	{
		return MIO::readUint16BE(_id);
	}
	
	SLIB_INLINE void setId(sl_uint16 id)
	{
		MIO::writeUint16BE(_id, id);
	}

	
	SLIB_INLINE sl_bool isQuestion() const
	{
		return (_flags[0] & 0x80) == 0;
	}

	SLIB_INLINE void setQuestion(sl_bool flag)
	{
		_flags[0] = (sl_uint8)((_flags[0] & 0x7F) | (flag ? 0 : 0x80));
	}

	
	// 4 bits
	SLIB_INLINE DnsOpcode getOpcode() const
	{
		return (DnsOpcode)((_flags[0] >> 3) & 0x0F);
	}
	
	// 4 bits
	SLIB_INLINE void setOpcode(DnsOpcode opcode)
	{
		_flags[0] = (sl_uint8)((_flags[0] & 0x87) | (((opcode & 0x0F) << 1)));
	}

	
	// Authoritative Answer (only valid in Response)
	SLIB_INLINE sl_bool isAA() const
	{
		return (_flags[0] & 0x04) != 0;
	}
	
	// Authoritative Answer (only valid in Response)
	SLIB_INLINE void setAA(sl_bool flag)
	{
		_flags[0] = (sl_uint8)((_flags[0] & 0xFB) | (flag ? 0x04 : 0));
	}

	
	// TrunCation: this message was truncated due to length greater than that permitted on the transmission channel
	SLIB_INLINE sl_bool isTC() const
	{
		return (_flags[0] & 0x02) != 0;
	}
	
	// TrunCation: this message was truncated due to length greater than that permitted on the transmission channel
	SLIB_INLINE void setTC(sl_bool flag)
	{
		_flags[0] = (sl_uint8)((_flags[0] & 0xFD) | (flag ? 0x02 : 0));
	}

	
	// Recursion Desired: it directs the name server to pursue the query recursively
	SLIB_INLINE sl_bool isRD() const
	{
		return (_flags[0] & 0x01) != 0;
	}
	
	// Recursion Desired: it directs the name server to pursue the query recursively
	SLIB_INLINE void setRD(sl_bool flag)
	{
		_flags[0] = (sl_uint8)((_flags[0] & 0xFE) | (flag ? 0x01 : 0));
	}

	
	// Recursion Available: denotes whether recursive query support is available in the name server (valid in response)
	SLIB_INLINE sl_bool isRA() const
	{
		return (_flags[1] & 0x80) != 0;
	}
	
	// Recursion Available: denotes whether recursive query support is available in the name server (valid in response)
	SLIB_INLINE void setRA(sl_bool flag)
	{
		_flags[1] = (sl_uint8)((_flags[1] & 0x7F) | (flag ? 0x80 : 0));
	}

	
	// Authentic Data: all the data included in the answer and authority portion of the response has been authenticated by the server (valid in response)
	SLIB_INLINE sl_bool isAD() const
	{
		return (_flags[1] & 0x20) != 0;
	}
	
	// Authentic Data: all the data included in the answer and authority portion of the response has been authenticated by the server (valid in response)
	SLIB_INLINE void setAD(sl_bool flag)
	{
		_flags[1] = (sl_uint8)((_flags[1] & 0xDF) | (flag ? 0x20 : 0));
	}

	
	// Checking Disabled: Pending (non-authenticated) data is acceptable to the resolver sending the query
	SLIB_INLINE sl_bool isCD() const
	{
		return (_flags[1] & 0x10) != 0;
	}
	
	// Checking Disabled: Pending (non-authenticated) data is acceptable to the resolver sending the query
	SLIB_INLINE void setCD(sl_bool flag)
	{
		_flags[1] = (sl_uint8)((_flags[1] & 0xEF) | (flag ? 0x10 : 0));
	}

	
	// 4 bits
	SLIB_INLINE DnsResponseCode getResponseCode() const
	{
		return (DnsResponseCode)(_flags[1] & 0x0F);
	}
	
	// 4 bits
	SLIB_INLINE void setResponseCode(DnsResponseCode code)
	{
		_flags[1] = (sl_uint8)((_flags[1] & 0xF0) | (code & 0x0F));
	}

	
	// QDCOUNT
	SLIB_INLINE sl_uint16 getQuestionsCount() const
	{
		return MIO::readUint16BE(_totalQuestions);
	}
	
	// QDCOUNT
	SLIB_INLINE void setQuestionsCount(sl_uint16 count)
	{
		MIO::writeUint16BE(_totalQuestions, count);
	}

	
	// ANCOUNT
	SLIB_INLINE sl_uint16 getAnswersCount() const
	{
		return MIO::readUint16BE(_totalAnswers);
	}
	
	// ANCOUNT
	SLIB_INLINE void setAnswersCount(sl_uint16 count)
	{
		MIO::writeUint16BE(_totalAnswers, count);
	}

	
	// NSCOUNT
	SLIB_INLINE sl_uint16 getAuthoritiesCount() const
	{
		return MIO::readUint16BE(_totalAuthorities);
	}
	
	// NSCOUNT
	SLIB_INLINE void setAuthoritiesCount(sl_uint16 count)
	{
		MIO::writeUint16BE(_totalAuthorities, count);
	}

	
	// ARCOUNT
	SLIB_INLINE sl_uint16 getAdditionalsCount() const
	{
		return MIO::readUint16BE(_totalAdditionals);
	}
	
	// ARCOUNT
	SLIB_INLINE void setAdditionalsCount(sl_uint16 count)
	{
		MIO::writeUint16BE(_totalAdditionals, count);
	}
	
private:
	sl_uint8 _id[2];
	sl_uint8 _flags[2];
	sl_uint8 _totalQuestions[2]; // QDCOUNT
	sl_uint8 _totalAnswers[2]; // ANCOUNT
	sl_uint8 _totalAuthorities[2]; // NSCOUNT
	sl_uint8 _totalAdditionals[2];  // ARCOUNT
};

class SLIB_EXPORT DnsRecord
{
public:
	DnsRecord();

public:
	SLIB_INLINE const String& getName() const
	{
		return _name;
	}
	
	SLIB_INLINE void setName(const String& name)
	{
		_name = name;
	}

	
	SLIB_INLINE DnsRecordType getType() const
	{
		return (DnsRecordType)_type;
	}
	
	SLIB_INLINE void setType(DnsRecordType type)
	{
		_type = (sl_uint16)type;
	}

	
	SLIB_INLINE DnsClass getClass() const
	{
		return (DnsClass)(_class);
	}
	
	SLIB_INLINE void setClass(DnsClass cls)
	{
		_class = cls;
	}

protected:
	static sl_uint32 _parseName(String& nameOut, const void* buf, sl_uint32 offset, sl_uint32 sizeName);
	static sl_uint32 _buildName(const String& nameIn, void* buf, sl_uint32 offset, sl_uint32 sizeName);
	sl_uint32 _parseHeader(const void* buf, sl_uint32 offset, sl_uint32 size);
	sl_uint32 _buildHeader(void* buf, sl_uint32 offset, sl_uint32 size);

private:
	String _name;
	sl_uint16 _type;
	sl_uint16 _class;
};

class SLIB_EXPORT DnsQuestionRecord : public DnsRecord
{
public:
	sl_uint32 parseRecord(const void* buf, sl_uint32 offset, sl_uint32 size);
	
	sl_uint32 buildRecord(void* buf, sl_uint32 offset, sl_uint32 size);
	
};

class SLIB_EXPORT DnsResponseRecord : public DnsRecord
{
public:
	DnsResponseRecord();
	
public:
	SLIB_INLINE sl_uint32 getTTL() const
	{
		return _TTL;
	}
	
	SLIB_INLINE void setTTL(sl_uint32 TTL)
	{
		_TTL = TTL;
	}
	
	
	SLIB_INLINE sl_uint16 getDataLength() const
	{
		return _dataLength;
	}

	SLIB_INLINE sl_uint16 getDataOffset() const
	{
		return _dataOffset;
	}

public:
	sl_uint32 parseRecord(const void* buf, sl_uint32 offset, sl_uint32 size);
	
	sl_uint32 buildRecord(void* buf, sl_uint32 offset, sl_uint32 size, const void* data, sl_uint16 sizeData);
	

	// A 32 bit Internet address
	IPv4Address parseData_A() const;
	
	//A 32 bit Internet address
	sl_uint32 buildRecord_A(void* buf, sl_uint32 offset, sl_uint32 size, const IPv4Address& addr);

	
	// A <domain-name> which specifies the canonical or primary	name for the owner. The owner name is an alias
	String parseData_CNAME() const;
	
	// A <domain-name> which specifies the canonical or primary	name for the owner. The owner name is an alias
	sl_uint32 buildRecord_CNAME(void* buf, sl_uint32 offset, sl_uint32 size, const String& cname);

	
	// A <domain-name> which specifies a host which should be authoritative for the specified class and domain
	String parseData_NS() const;
	
	// A <domain-name> which specifies a host which should be authoritative for the specified class and domain
	sl_uint32 buildRecord_NS(void* buf, sl_uint32 offset, sl_uint32 size, const String& nameServer);
	
	
	String toString() const;

private:
	sl_uint32 _TTL;
	sl_uint16 _dataLength;
	sl_uint16 _dataOffset;
	const sl_uint8* _message;
	sl_uint32 _messageLength;
};

struct SLIB_EXPORT DnsQuestion
{
	sl_uint16 id;
	String name;
};

struct SLIB_EXPORT DnsAnswer
{
	sl_uint16 id;

	struct Question
	{
		String name;
	};
	List<Question> questions;
	
	struct Address
	{
		String name;
		IPv4Address address;
	};
	List<Address> addresses;

	struct Alias
	{
		String name;
		String alias;
	};
	List<Alias> aliases;

	struct NameServer
	{
		String name;
		String server;
	};
	List<NameServer> nameServers;
};

class DnsClient;

class SLIB_EXPORT IDnsClientListener
{
public:
	virtual void onDnsAnswer(DnsClient* client, const SocketAddress& serverAddress, const DnsAnswer& answer) = 0;
};

class SLIB_EXPORT DnsClientParam
{
public:
	Ptr<IDnsClientListener> listener;
};

class SLIB_EXPORT DnsClient : public Object, public IAsyncUdpSocketListener
{
protected:
	DnsClient();

public:
	static Ref<DnsClient> create(const DnsClientParam& param, const Ref<AsyncLoop>& loop);
	
	static Ref<DnsClient> create(const DnsClientParam& param);

public:
	void sendQuestion(const SocketAddress& serverAddress, const DnsQuestion& question);
	
	void sendQuestion(const IPv4Address& serverIp, const String& host);


protected:
	Ref<AsyncUdpSocket> m_udp;
	sl_uint16 m_idLast;
	Memory m_memReceive;
	
	Ptr<IDnsClientListener> m_listener;

protected:
	void _onDnsAnswer(const SocketAddress& serverAddress, const DnsAnswer& answer);
	
	// override
	virtual void onReceiveFrom(AsyncUdpSocket* socket, void* data, sl_uint32 sizeReceive, const SocketAddress& address, sl_bool flagError);

};

class DnsServer;

class SLIB_EXPORT IDnsServerListener
{
public:
	virtual IPv4Address onResolveDnsHost(DnsServer* server, const SocketAddress& clientAddress, const DnsQuestion& question);
	
	virtual void onCacheDnsHost(DnsServer* server, const String& name, const IPv4Address& addr);
};

class SLIB_EXPORT DnsServerParam
{
public:
	IPv4Address forwardTarget;
	sl_bool flagUseCache;

	Ptr<IDnsServerListener> listener;
	
public:
	DnsServerParam();
};

class SLIB_EXPORT DnsServer : public Object
{
protected:
	DnsServer();
	~DnsServer();
    
public:
	static Ref<DnsServer> create(const DnsServerParam& param);

public:
	void release();

	void sendHostAddressAnswer(const SocketAddress& clientAddress, const DnsQuestion& question, const IPv4Address& answer);
	
private:
	SafeRef<Socket> m_udp;
	Memory m_memReceive;
	SafeRef<Thread> m_thread;
	
	HashMap<String, IPv4Address> m_mapDNSCache;
	sl_bool m_flagUseCache;

	HashMap<sl_uint16, sl_uint16> m_mapForwardId;
	HashMap<sl_uint16, SocketAddress> m_mapForwardAddress;
	IPv4Address m_forwardTarget;
	sl_uint16 m_lastForwardId;

	Ptr<IDnsServerListener> m_listener;
	
protected:
	void _run();
	
protected:
	IPv4Address _resolveDnsHost(const SocketAddress& clientAddress, const DnsQuestion& question);
	
	void _cacheDnsHost(const String& name, const IPv4Address& addr);
	
};

SLIB_NETWORK_NAMESPACE_END

#endif
