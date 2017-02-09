#include "../../../inc/slib/network/dns.h"

#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/log.h"

#define _MAX_NAME SLIB_NETWORK_DNS_NAME_MAX_LENGTH

SLIB_NETWORK_NAMESPACE_BEGIN

sl_uint16 DnsHeader::getId() const
{
	return MIO::readUint16BE(_id);
}

void DnsHeader::setId(sl_uint16 id)
{
	MIO::writeUint16BE(_id, id);
}

sl_bool DnsHeader::isQuestion() const
{
	return (_flags[0] & 0x80) == 0;
}

void DnsHeader::setQuestion(sl_bool flag)
{
	_flags[0] = (sl_uint8)((_flags[0] & 0x7F) | (flag ? 0 : 0x80));
}

DnsOpcode DnsHeader::getOpcode() const
{
	return (DnsOpcode)((_flags[0] >> 3) & 0x0F);
}

void DnsHeader::setOpcode(DnsOpcode opcode)
{
	_flags[0] = (sl_uint8)((_flags[0] & 0x87) | (((((sl_uint32)opcode) & 0x0F) << 1)));
}

sl_bool DnsHeader::isAA() const
{
	return (_flags[0] & 0x04) != 0;
}

void DnsHeader::setAA(sl_bool flag)
{
	_flags[0] = (sl_uint8)((_flags[0] & 0xFB) | (flag ? 0x04 : 0));
}

sl_bool DnsHeader::isTC() const
{
	return (_flags[0] & 0x02) != 0;
}

void DnsHeader::setTC(sl_bool flag)
{
	_flags[0] = (sl_uint8)((_flags[0] & 0xFD) | (flag ? 0x02 : 0));
}

sl_bool DnsHeader::isRD() const
{
	return (_flags[0] & 0x01) != 0;
}

void DnsHeader::setRD(sl_bool flag)
{
	_flags[0] = (sl_uint8)((_flags[0] & 0xFE) | (flag ? 0x01 : 0));
}

sl_bool DnsHeader::isRA() const
{
	return (_flags[1] & 0x80) != 0;
}

void DnsHeader::setRA(sl_bool flag)
{
	_flags[1] = (sl_uint8)((_flags[1] & 0x7F) | (flag ? 0x80 : 0));
}

sl_bool DnsHeader::isAD() const
{
	return (_flags[1] & 0x20) != 0;
}

void DnsHeader::setAD(sl_bool flag)
{
	_flags[1] = (sl_uint8)((_flags[1] & 0xDF) | (flag ? 0x20 : 0));
}

sl_bool DnsHeader::isCD() const
{
	return (_flags[1] & 0x10) != 0;
}

void DnsHeader::setCD(sl_bool flag)
{
	_flags[1] = (sl_uint8)((_flags[1] & 0xEF) | (flag ? 0x10 : 0));
}

DnsResponseCode DnsHeader::getResponseCode() const
{
	return (DnsResponseCode)(_flags[1] & 0x0F);
}

void DnsHeader::setResponseCode(DnsResponseCode code)
{
	_flags[1] = (sl_uint8)((_flags[1] & 0xF0) | (((sl_uint32)code) & 0x0F));
}

sl_uint16 DnsHeader::getQuestionsCount() const
{
	return MIO::readUint16BE(_totalQuestions);
}

void DnsHeader::setQuestionsCount(sl_uint16 count)
{
	MIO::writeUint16BE(_totalQuestions, count);
}

sl_uint16 DnsHeader::getAnswersCount() const
{
	return MIO::readUint16BE(_totalAnswers);
}

void DnsHeader::setAnswersCount(sl_uint16 count)
{
	MIO::writeUint16BE(_totalAnswers, count);
}

sl_uint16 DnsHeader::getAuthoritiesCount() const
{
	return MIO::readUint16BE(_totalAuthorities);
}

void DnsHeader::setAuthoritiesCount(sl_uint16 count)
{
	MIO::writeUint16BE(_totalAuthorities, count);
}

sl_uint16 DnsHeader::getAdditionalsCount() const
{
	return MIO::readUint16BE(_totalAdditionals);
}

void DnsHeader::setAdditionalsCount(sl_uint16 count)
{
	MIO::writeUint16BE(_totalAdditionals, count);
}


DnsRecord::DnsRecord()
{
	_type = DnsRecordType::None;
	_class = DnsClass::IN;
}

DnsRecord::~DnsRecord()
{
}

const String& DnsRecord::getName() const
{
	return _name;
}

void DnsRecord::setName(const String& name)
{
	_name = name;
}

DnsRecordType DnsRecord::getType() const
{
	return (DnsRecordType)_type;
}

void DnsRecord::setType(DnsRecordType type)
{
	_type = type;
}

DnsClass DnsRecord::getClass() const
{
	return (DnsClass)(_class);
}

void DnsRecord::setClass(DnsClass cls)
{
	_class = cls;
}

sl_uint32 DnsRecord::_parseName(String& nameOut, const void* _buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8* buf = (const sl_uint8*)(_buf);

	char name[_MAX_NAME];
	sl_uint32 lenName = 0;

	sl_uint32 ret = 0;

	sl_uint32 now = offset;
	while (now < size) {
		sl_uint8 ch = buf[now];
		if ((ch & 0xC0) == 0) {
			sl_uint32 lenLabel = ch & 0x3F;
			if (lenLabel == 0) {
				nameOut = String::fromUtf8(name, lenName);
				if (ret == 0) {
					ret = now + 1;
				}
				return ret;
			} else {
				if (lenName >= _MAX_NAME) {
					return 0;
				}
				now++;
				if (now + lenLabel > size) {
					return 0;
				}				
				if (lenName) {
					name[lenName] = '.';
					lenName++;
				}
				if (lenLabel > _MAX_NAME - lenName) {
					return 0;
				}
				Base::copyMemory(name + lenName, buf + now, lenLabel);
				lenName += lenLabel;
				now += lenLabel;
			}

		} else if ((ch & 0xC0) == 0xC0) {
			// Message Compression, jump to pointer
			now++;
			if (now >= size) {
				return 0;
			}
			sl_uint32 ptr = (ch & 0x3F);
			ptr = (ptr << 8) | (buf[now]);
			if (ptr >= size) {
				return 0;
			}
			if (ret == 0) {
				ret = now + 1;
			}
			now = ptr;
		} else {
			return 0;
		}
	}
	return 0;
}

sl_uint32 DnsRecord::_buildName(const String& name, void* _buf, sl_uint32 offset, sl_uint32 size)
{
	sl_char8* bufIn = name.getData();
	sl_uint32 lenIn = (sl_uint32)(name.getLength());
	sl_uint8* bufOut = (sl_uint8*)_buf;
	if (lenIn + 2 + offset > size) {
		return 0;
	}
	sl_uint32 nowIn = 0;
	sl_uint32 nowOut = offset + 1;
	sl_uint32 posLabel = offset;
	while (nowIn <= lenIn) {
		sl_char8 ch = bufIn[nowIn];
		if (ch == '.' || ch == 0) {
			sl_uint32 lenLabel = nowOut - posLabel - 1;
			if (lenLabel > 63) {
				return 0;
			}
			if (lenLabel == 0) {
				return 0;
			}
			bufOut[posLabel] = lenLabel;
			posLabel = nowOut;
			if (ch == 0) {
				bufOut[nowOut] = 0;
				return nowOut + 1;
			}
		} else {
			bufOut[nowOut] = bufIn[nowIn];
		}
		nowOut++;
		nowIn++;
	}
	return 0;
}

sl_uint32 DnsRecord::_parseHeader(const void* _buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8* buf = (const sl_uint8*)_buf;
	String name;
	sl_uint32 pos = _parseName(name, buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 4 > size) {
		return 0;
	}
	_name = name;
	_type = (DnsRecordType)(MIO::readUint16BE(buf + pos));
	_class = (DnsClass)(MIO::readUint16BE(buf + pos + 2));
	return pos + 4;
}

sl_uint32 DnsRecord::_buildHeader(void* _buf, sl_uint32 offset, sl_uint32 size)
{
	sl_uint8* buf = (sl_uint8*)_buf;
	sl_uint32 pos = _buildName(_name, buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 4 > size) {
		return 0;
	}
	MIO::writeUint16BE(buf + pos, (sl_uint16)_type);
	MIO::writeUint16BE(buf + pos + 2, (sl_uint16)_class);
	return pos + 4;
}


DnsQuestionRecord::DnsQuestionRecord()
{
}

DnsQuestionRecord::~DnsQuestionRecord()
{
}

sl_uint32 DnsQuestionRecord::parseRecord(const void* buf, sl_uint32 offset, sl_uint32 size)
{
	return _parseHeader(buf, offset, size);
}

sl_uint32 DnsQuestionRecord::buildRecord(void* buf, sl_uint32 offset, sl_uint32 size)
{
	return _buildHeader(buf, offset, size);
}


DnsResponseRecord::DnsResponseRecord()
{
	_message = 0;
	_messageLength = 0;
	_dataOffset = 0;
	_dataLength = 0;
}

DnsResponseRecord::~DnsResponseRecord()
{
}

sl_uint32 DnsResponseRecord::getTTL() const
{
	return _TTL;
}

void DnsResponseRecord::setTTL(sl_uint32 TTL)
{
	_TTL = TTL;
}

sl_uint16 DnsResponseRecord::getDataLength() const
{
	return _dataLength;
}

sl_uint16 DnsResponseRecord::getDataOffset() const
{
	return _dataOffset;
}

sl_uint32 DnsResponseRecord::parseRecord(const void* _buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8* buf = (const sl_uint8*)_buf;
	_message = buf;
	_messageLength = size;

	sl_uint32 pos = _parseHeader(buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 6 > size) {
		return 0;
	}
	_TTL = MIO::readUint32BE(buf + pos);
	_dataLength = MIO::readUint16BE(buf + pos + 4);
	pos += 6;
	_dataOffset = pos;
	pos += _dataLength;
	if (pos > size) {
		return 0;
	}
	return pos;
}

sl_uint32 DnsResponseRecord::buildRecord(void* _buf, sl_uint32 offset, sl_uint32 size, const void* data, sl_uint16 sizeData)
{
	sl_uint8* buf = (sl_uint8*)_buf;
	sl_uint32 pos = _buildHeader(buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 6 + sizeData > size) {
		return 0;
	}
	MIO::writeUint32BE(buf + pos, _TTL);
	MIO::writeUint16BE(buf + pos + 4, sizeData);
	Base::copyMemory(buf + pos + 6, data, sizeData);
	return pos + 6 + sizeData;
}

IPv4Address DnsResponseRecord::parseData_A() const
{
	if (getType() == DnsRecordType::A && _dataLength == 4) {
		return IPv4Address(_message + _dataOffset);
	}
	return IPv4Address::zero();
}

sl_uint32 DnsResponseRecord::buildRecord_A(void* buf, sl_uint32 offset, sl_uint32 size, const IPv4Address& addr)
{
	setType(DnsRecordType::A);
	return buildRecord(buf, offset, size, &addr, 4);
}

String DnsResponseRecord::parseData_CNAME() const
{
	if (getType() == DnsRecordType::CNAME) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return sl_null;
}

sl_uint32 DnsResponseRecord::buildRecord_CNAME(void* buf, sl_uint32 offset, sl_uint32 size, const String& cname)
{
	setType(DnsRecordType::CNAME);
	return _buildName(cname, buf, offset, size);
}

String DnsResponseRecord::parseData_NS() const
{
	if (getType() == DnsRecordType::NS) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return sl_null;
}

sl_uint32 DnsResponseRecord::buildRecord_NS(void* buf, sl_uint32 offset, sl_uint32 size, const String& ns)
{
	setType(DnsRecordType::NS);
	return _buildName(ns, buf, offset, size);
}

IPv6Address DnsResponseRecord::parseData_AAAA() const
{
	if (getType() == DnsRecordType::AAAA && _dataLength == 16) {
		return IPv6Address(_message + _dataOffset);
	}
	return IPv6Address::zero();
}

sl_uint32 DnsResponseRecord::buildRecord_AAAA(void* buf, sl_uint32 offset, sl_uint32 size, const IPv6Address& addr)
{
	setType(DnsRecordType::AAAA);
	return buildRecord(buf, offset, size, addr.m, 16);
}

String DnsResponseRecord::parseData_PTR() const
{
	if (getType() == DnsRecordType::PTR) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return sl_null;
}

sl_uint32 DnsResponseRecord::buildRecord_PTR(void* buf, sl_uint32 offset, sl_uint32 size, const String& dname)
{
	setType(DnsRecordType::PTR);
	return _buildName(dname, buf, offset, size);
}

String DnsResponseRecord::toString() const
{
	String ret = getName() + " ";
	DnsRecordType type = getType();
	if (type == DnsRecordType::A) {
		ret += "A " + parseData_A().toString();
	} else if (type == DnsRecordType::CNAME) {
		ret += "CNAME " + parseData_CNAME();
	} else if (type == DnsRecordType::NS) {
		ret += "NS " + parseData_NS();
	} else if (type == DnsRecordType::AAAA) {
		ret += "AAAA " + parseData_AAAA().toString();
	} else if (type == DnsRecordType::PTR) {
		ret += "PTR " + parseData_PTR();
	} else {
		ret += String("TYPE=") + (sl_uint32)type;
	}
	return ret;
}

/*************************************************************
					DnsPacket
*************************************************************/

DnsPacket::DnsPacket()
{
}

DnsPacket::~DnsPacket()
{
}

sl_bool DnsPacket::parsePacket(const void* packet, sl_uint32 size)
{
	sl_uint8* buf = (sl_uint8*)packet;
	
	do {
		if (size < sizeof(DnsHeader)) {
			break;
		}
		DnsHeader* header = (DnsHeader*)buf;
		if (header->isQuestion()) { // is question?
			flagQuestion = sl_true;
		} else {
			flagQuestion = sl_false;
		}
		id = header->getId();
		
		sl_uint32 i, n;
		sl_uint32 offset = sizeof(DnsHeader);
		
		n = header->getQuestionsCount();
		for (i = 0; i < n; i++) {
			DnsQuestionRecord record;
			offset = record.parseRecord(buf, offset, size);
			if (offset == 0) {
				break;
			}
			DnsPacket::Question question;
			question.name = record.getName();
			question.type = record.getType();
			questions.add(question);
		}
		if (i != n) {
			break;
		}
		n = header->getAnswersCount() + header->getAuthoritiesCount() + header->getAdditionalsCount();
		for (i = 0; i < n; i++) {
			DnsResponseRecord record;
			offset = record.parseRecord(buf, offset, size);
			if (offset == 0) {
				break;
			}
			DnsRecordType type = record.getType();
			if (type == DnsRecordType::A) {
				DnsPacket::Address item;
				item.name = record.getName();
				IPv4Address addr = record.parseData_A();
				if (addr.isNotZero()) {
					item.address = addr;
					addresses.add(item);
				}
			} else if (type == DnsRecordType::AAAA) {
				DnsPacket::Address item;
				item.name = record.getName();
				IPv6Address addr = record.parseData_AAAA();
				if (addr.isNotZero()) {
					item.address = addr;
					addresses.add(item);
				}
			} else if (type == DnsRecordType::CNAME) {
				DnsPacket::Alias item;
				item.name = record.getName();
				item.alias = record.parseData_CNAME();
				if (item.alias.isNotEmpty()) {
					aliases.add(item);
				}
			} else if (type == DnsRecordType::NS) {
				DnsPacket::NameServer item;
				item.name = record.getName();
				item.server = record.parseData_NS();
				if (item.server.isNotEmpty()) {
					nameServers.add(item);
				}
			} else if (type == DnsRecordType::PTR) {
				DnsPacket::NamePointer item;
				item.name = record.getName();
				item.pointer = record.parseData_PTR();
				if (item.pointer.isNotEmpty()) {
					pointers.add(item);
				}
			}
		}
		
		return sl_true;
		
	} while (0);
	
	return sl_false;
}

Memory DnsPacket::buildQuestionPacket(sl_uint16 id, const String& host)
{
	char buf[1024];
	DnsHeader* header = (DnsHeader*)buf;
	Base::zeroMemory(buf, sizeof(DnsHeader));
	header->setQuestion(sl_true); // Question
	header->setId(id);
	header->setRD(sl_true); // Recursive Desired
	header->setOpcode(DnsOpcode::Query);
	header->setQuestionsCount(1);
	DnsQuestionRecord record;
	record.setName(host);
	record.setType(DnsRecordType::A);
	sl_uint32 size = record.buildRecord(buf, sizeof(DnsHeader), 1024);
	if (size > 0) {
		return Memory::create(buf, size);
	}
	return sl_null;
}

Memory DnsPacket::buildHostAddressAnswerPacket(sl_uint16 id, const String& hostName, const IPv4Address& hostAddress)
{
	char buf[4096];
	Base::zeroMemory(buf, sizeof(buf));
	
	if (hostAddress.isNotZero()) {
		
		DnsHeader* header = (DnsHeader*)(buf);
		header->setId(id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(DnsOpcode::Query);
		header->setResponseCode(DnsResponseCode::NoError);
		header->setQuestionsCount(1);
		header->setAnswersCount(1);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);
		
		sl_uint32 offset = sizeof(DnsHeader);
		DnsQuestionRecord recordQuestion;
		recordQuestion.setName(hostName);
		recordQuestion.setType(DnsRecordType::A);
		offset = recordQuestion.buildRecord(buf, offset, 1024);
		if (offset > 0) {
			DnsResponseRecord recordResponse;
			recordResponse.setName(hostName);
			offset = recordResponse.buildRecord_A(buf, offset, 1024, hostAddress);
			if (offset > 0) {
				return Memory::create(buf, offset);
			}
		}
		
	} else {
		
		DnsHeader* header = (DnsHeader*)(buf);
		header->setId(id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(DnsOpcode::Query);
		header->setResponseCode(DnsResponseCode::NameError);
		header->setQuestionsCount(1);
		header->setAnswersCount(0);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);
		
		sl_uint32 offset = sizeof(DnsHeader);
		DnsQuestionRecord recordQuestion;
		recordQuestion.setName(hostName);
		recordQuestion.setType(DnsRecordType::A);
		offset = recordQuestion.buildRecord(buf, offset, 1024);
		if (offset == 0) {
			return Memory::create(buf, offset);
		}
	}
	
	return sl_null;
	
}

/*************************************************************
				DnsClient
*************************************************************/

DnsClientParam::DnsClientParam()
{
}

DnsClientParam::~DnsClientParam()
{
}

SLIB_DEFINE_OBJECT(DnsClient, Object)

DnsClient::DnsClient()
{
	m_idLast = 0;
}

DnsClient::~DnsClient()
{
}

Ref<DnsClient> DnsClient::create(const DnsClientParam& param)
{
	Ref<DnsClient> ret = new DnsClient;
	if (ret.isNotNull()) {
		ret->m_listener = param.listener;
		AsyncUdpSocketParam up;
		up.listener.setWeak(ret);
		up.packetSize = 4096;
		up.ioLoop = param.ioLoop;
		Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(up);
		if (socket.isNotNull()) {
			ret->m_udp = socket;
		}
	}
	return ret;
}

void DnsClient::sendQuestion(const SocketAddress& serverAddress, const String& hostName)
{
	sl_uint16 id = m_idLast++;
	Memory mem = DnsPacket::buildQuestionPacket(id, hostName);
	if (mem.isNotEmpty()) {
		m_udp->sendTo(serverAddress, mem);
	}
}

void DnsClient::sendQuestion(const IPv4Address& serverIp, const String& hostName)
{
	sendQuestion(SocketAddress(serverIp, SLIB_NETWORK_DNS_PORT), hostName);
}

void DnsClient::onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& address, void* data, sl_uint32 sizeReceive)
{
	DnsPacket packet;
	if (packet.parsePacket(data, sizeReceive)) {
		_onDnsAnswer(address, packet);
	}
}

void DnsClient::_onDnsAnswer(const SocketAddress& serverAddress, const DnsPacket& packet)
{
	PtrLocker<IDnsClientListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onDnsAnswer(this, serverAddress, packet);
	}
}

/*************************************************************
					DnsServer
*************************************************************/
DnsResolveHostParam::DnsResolveHostParam()
{
	clientAddress.setNone();
	hostAddress.setZero();
	flagIgnoreRequest = sl_false;
	forwardAddress.setNone();
	flagEncryptForward = sl_false;
}

DnsResolveHostParam::~DnsResolveHostParam()
{
}

DnsServerParam::DnsServerParam()
{
	portDns = SLIB_NETWORK_DNS_PORT;

	portEncryption = 0;

	flagProxy = sl_false;

	defaultForwardAddress.setNone();
	flagEncryptDefaultForward = sl_false;

	flagAutoStart = sl_true;
}

DnsServerParam::~DnsServerParam()
{
}

void DnsServerParam::parse(const Variant& conf)
{
	portDns = (sl_uint16)conf.getField("dns_port").getUint32(SLIB_NETWORK_DNS_PORT);
	portEncryption = (sl_uint16)conf.getField("secure_port").getUint32(0);
	encryptionKey = conf.getField("secure_key").getString();

	flagProxy = conf.getField("is_proxy").getBoolean(sl_false);

	IPv4Address defaultForwardAddressIp = IPv4Address(8, 8, 4, 4);
	defaultForwardAddressIp.parse(conf.getField("forward_dns").getString());
	defaultForwardAddress = SocketAddress(defaultForwardAddressIp, SLIB_NETWORK_DNS_PORT);
}


SLIB_DEFINE_OBJECT(DnsServer, Object)

DnsServer::DnsServer()
{
	m_flagInit = sl_false;
	m_flagRunning = sl_false;

	m_lastForwardId = 0;
}

DnsServer::~DnsServer()
{
	release();
}

#define TAG_SERVER "DnsServer"

Ref<DnsServer> DnsServer::create(const DnsServerParam& param)
{
	Ref<DnsServer> ret = new DnsServer;
	
	if (ret.isNotNull()) {

		AsyncUdpSocketParam up;
		up.listener.setWeak(ret);
		up.packetSize = 4096;
		up.ioLoop = param.ioLoop;
		up.flagAutoStart = sl_false;
		
		up.bindAddress.port = param.portDns;
		Ref<AsyncUdpSocket> socketDns = AsyncUdpSocket::create(up);
		if (socketDns.isNull()) {
			LogError(TAG_SERVER, "Failed to bind to port %d", param.portDns);
			return sl_null;
		}
		
		up.bindAddress.port = param.portEncryption;
		Ref<AsyncUdpSocket> socketEncrypt = AsyncUdpSocket::create(up);
		if (socketEncrypt.isNull()) {
			LogError(TAG_SERVER, "Failed to bind to port %d", param.portEncryption);
			return sl_null;
		}

		if (socketDns.isNotNull() || socketEncrypt.isNotNull()) {

			ret->m_udpDns = socketDns;
			ret->m_udpEncrypt = socketEncrypt;

			ret->m_encrypt.setKey_SHA256(param.encryptionKey);

			ret->m_flagProxy = param.flagProxy;

			ret->m_defaultForwardAddress = param.defaultForwardAddress;
			ret->m_flagEncryptDefaultForward = param.flagEncryptDefaultForward;

			ret->m_listener = param.listener;

			ret->m_flagInit = sl_true;
			if (param.flagAutoStart) {
				ret->start();
			}
			return ret;

		}
		
	}
	return sl_null;
}

void DnsServer::release()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	m_flagInit = sl_false;

	m_flagRunning = sl_false;
	if (m_udpDns.isNotNull()) {
		m_udpDns->close();
	}
	if (m_udpEncrypt.isNotNull()) {
		m_udpEncrypt->close();
	}
}

void DnsServer::start()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	if (m_flagRunning) {
		return;
	}
	if (m_udpDns.isNotNull()) {
		m_udpDns->start();
	}
	if (m_udpEncrypt.isNotNull()) {
		m_udpEncrypt->start();
	}
	m_flagRunning = sl_true;
}

sl_bool DnsServer::isRunning()
{
	return m_flagRunning;
}

void DnsServer::_processReceivedDnsQuestion(const SocketAddress& clientAddress, sl_uint16 id, const String& hostName, sl_bool flagEncryptedRequest)
{
	if (hostName.indexOf('.') < 0) {
		return;
	}
	DnsResolveHostParam rp;
	rp.clientAddress = clientAddress;
	rp.hostName = hostName;
	rp.forwardAddress = m_defaultForwardAddress;
	rp.flagEncryptForward = m_flagEncryptDefaultForward;
	_resolveDnsHost(rp);
	if (rp.flagIgnoreRequest) {
		return;
	}
	if (rp.forwardAddress.isInvalid()) {
		_sendPacket(flagEncryptedRequest, clientAddress, _buildHostAddressAnswerPacket(id, hostName, rp.hostAddress, flagEncryptedRequest));
		return;
	}
	if (rp.hostAddress.isNotZero()) {
		_sendPacket(flagEncryptedRequest, clientAddress, _buildHostAddressAnswerPacket(id, hostName, rp.hostAddress, flagEncryptedRequest));
	}
	
	// forward DNS request
	{
		sl_uint16 idForward = m_lastForwardId++;
		ForwardElement fe;
		fe.requestedId = id;
		fe.requestedHostName = hostName;
		fe.flagEncrypted = flagEncryptedRequest;
		if (rp.hostAddress.isNotZero()) {
			fe.clientAddress.ip.setNone();
			fe.clientAddress.port = 0;
		} else {
			fe.clientAddress = clientAddress;
		}
		m_mapForward.put(idForward, fe);
		_sendPacket(rp.flagEncryptForward, rp.forwardAddress, _buildQuestionPacket(idForward, hostName, rp.flagEncryptForward));
	}

}

void DnsServer::_processReceivedDnsAnswer(const DnsPacket& packet)
{

	sl_uint16 idForward = packet.id;

	ForwardElement fe;
	if (m_mapForward.get(idForward, &fe)) {

		m_mapForward.remove(idForward);

		String reqNameLower = fe.requestedHostName.toLower();

		IPv4Address resolvedAddress;
		resolvedAddress.setZero();

		HashMap<String, IPv4Address> aliasAddresses4;
		HashMap<String, IPv6Address> aliasAddresses6;
		// address
		{
			ListElements<DnsPacket::Address> addresses(packet.addresses);
			sl_size n = addresses.count;
			for (sl_size i = 0; i < n; i++) {
				DnsPacket::Address& address = addresses[n - 1 - i];
				if (address.address.isNotNone()) {
					if (address.address.isIPv4() && address.address.getIPv4().isHost()) {
						_cacheDnsHost(address.name, address.address);
						aliasAddresses4.put_NoLock(address.name.toLower(), address.address.getIPv4());
					} else if (address.address.isIPv6()) {
						_cacheDnsHost(address.name, address.address);
						aliasAddresses6.put_NoLock(address.name.toLower(), address.address.getIPv6());
					}
					if (reqNameLower == address.name.toLower()) {
						if (resolvedAddress.isZero()) {
							resolvedAddress = address.address.getIPv4();
						}
					}
				}
			}
		}
		// alias
		{
			List<DnsPacket::Alias> aliasesProcess = packet.aliases.duplicate_NoLock();
			sl_bool flagProcess = sl_true;
			while (flagProcess) {
				flagProcess = sl_false;
				List<DnsPacket::Alias> aliasesNoProcess;
				ListElements<DnsPacket::Alias> aliases(aliasesProcess);
				sl_size n = aliases.count;
				for (sl_size i = 0; i < n; i++) {
					DnsPacket::Alias& alias = aliases[n - 1 - i];
					IPv4Address addr4;
					IPv6Address addr6;
					sl_bool flagAddr = sl_false;
					if (aliasAddresses4.get_NoLock(alias.alias.toLower(), &addr4)) {
						aliasAddresses4.put_NoLock(alias.name.toLower(), addr4);
						_cacheDnsHost(alias.name, addr4);
						if (reqNameLower == alias.name.toLower()) {
							if (resolvedAddress.isZero()) {
								resolvedAddress = addr4;
							}
						}
						flagProcess = sl_true;
						flagAddr = sl_true;
					}
					if (aliasAddresses6.get_NoLock(alias.alias.toLower(), &addr6)) {
						aliasAddresses6.put_NoLock(alias.name.toLower(), addr6);
						_cacheDnsHost(alias.name, addr6);
						flagProcess = sl_true;
						flagAddr = sl_true;
					}
					if (!flagAddr) {
						aliasesNoProcess.add_NoLock(alias);
					}
				}
				aliasesProcess = aliasesNoProcess;
			}
		}
		if (fe.clientAddress.isValid()) {
			_sendPacket(fe.flagEncrypted, fe.clientAddress, _buildHostAddressAnswerPacket(fe.requestedId, fe.requestedHostName, resolvedAddress, fe.flagEncrypted));
		}
	}
}

void DnsServer::_processReceivedProxyQuestion(const SocketAddress& clientAddress, void* data, sl_uint32 size, sl_bool flagEncryptedRequest)
{
	DnsHeader* header = (DnsHeader*)data;

	sl_uint16 idForward = m_lastForwardId++;

	ForwardElement fe;
	fe.requestedId = header->getId();
	fe.flagEncrypted = flagEncryptedRequest;
	fe.clientAddress = clientAddress;

	header->setId(idForward);
	Memory packet = Memory::create(data, size);
	if (m_flagEncryptDefaultForward) {
		packet = m_encrypt.encrypt_CBC_PKCS7Padding(packet);
	}
	if (packet.isEmpty()) {
		return;
	}

	m_mapForward.put(idForward, fe);

	_sendPacket(m_flagEncryptDefaultForward, m_defaultForwardAddress, packet);

}

void DnsServer::_processReceivedProxyAnswer(void* data, sl_uint32 size)
{
	DnsHeader* header = (DnsHeader*)data;
	sl_uint16 idForward = header->getId();
	ForwardElement fe;
	if (m_mapForward.get(idForward, &fe)) {

		m_mapForward.remove(idForward);

		header->setId(fe.requestedId);
		Memory packet = Memory::create(data, size);
		if (fe.flagEncrypted) {
			packet = m_encrypt.encrypt_CBC_PKCS7Padding(packet);
		}
		if (packet.isEmpty()) {
			return;
		}

		_sendPacket(fe.flagEncrypted, fe.clientAddress, packet);
	}
}

void DnsServer::_sendPacket(sl_bool flagEncrypted, const SocketAddress& targetAddress, const Memory& packet)
{
	if (packet.isNotEmpty()) {
		Ref<AsyncUdpSocket> socket;
		if (flagEncrypted) {
			socket = m_udpEncrypt;
		} else {
			socket = m_udpDns;
		}
		if (socket.isNotNull()) {
			socket->sendTo(targetAddress, packet);
		}
	}
}

Memory DnsServer::_buildQuestionPacket(sl_uint16 id, const String& host, sl_bool flagEncrypt)
{
	Memory mem = DnsPacket::buildQuestionPacket(id, host);
	if (flagEncrypt) {
		return m_encrypt.encrypt_CBC_PKCS7Padding(mem);
	}
	return mem;
}

Memory DnsServer::_buildHostAddressAnswerPacket(sl_uint16 id, const String& hostName, const IPv4Address& hostAddress, sl_bool flagEncrypt)
{
	Memory mem = DnsPacket::buildHostAddressAnswerPacket(id, hostName, hostAddress);
	if (flagEncrypt) {
		return m_encrypt.encrypt_CBC_PKCS7Padding(mem);
	}
	return mem;
}

void DnsServer::onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& addressFrom, void* data, sl_uint32 size)
{
	sl_bool flagEncrypted = sl_false;
	Memory memDecrypt;
	if (socket == m_udpEncrypt) {
		flagEncrypted = sl_true;
		memDecrypt = m_encrypt.decrypt_CBC_PKCS7Padding(data, size);
		if (memDecrypt.isEmpty()) {
			return;
		}
		data = memDecrypt.getData();
		size = (sl_uint32)(memDecrypt.getSize());
	}
	if (m_flagProxy) {
		if (size < sizeof(DnsHeader)) {
			return;
		}
		DnsHeader* header = (DnsHeader*)data;
		if (header->isQuestion()) {
			_processReceivedProxyQuestion(addressFrom, data, size, flagEncrypted);
		} else {
			_processReceivedProxyAnswer(data, size);
		}
	} else {
		char* buf = (char*)data;
		DnsPacket packet;
		if (packet.parsePacket(buf, size)) {
			if (packet.flagQuestion) {
				if (packet.questions.getCount() == 1) {
					DnsPacket::Question& question = (packet.questions.getData())[0];
					if (question.type == DnsRecordType::A) {
						_processReceivedDnsQuestion(addressFrom, packet.id, question.name, flagEncrypted);
					}
				}
			} else {
				_processReceivedDnsAnswer(packet);
			}
		}
	}
}

void DnsServer::_resolveDnsHost(DnsResolveHostParam& param)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->resolveDnsHost(this, param);
	}
}

void DnsServer::_cacheDnsHost(const String& hostName, const IPAddress& hostAddress)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->cacheDnsHost(this, hostName, hostAddress);
	}
}

SLIB_NETWORK_NAMESPACE_END
