#include "../../../inc/slib/network/dns.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/log.h"

#include <stdlib.h>
#define _MAX_NAME SLIB_NETWORK_DNS_NAME_MAX_LENGTH

SLIB_NETWORK_NAMESPACE_BEGIN

DnsRecord::DnsRecord()
{
	_type = 0;
	_class = dnsClass_IN;
}

sl_uint32 DnsRecord::_parseName(String &nameOut, const void *_buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8 *buf = (const sl_uint8*)(_buf);

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

sl_uint32 DnsRecord::_buildName(const String &name, void *_buf, sl_uint32 offset, sl_uint32 size)
{
	sl_char8* bufIn = name.getBuf();
	sl_uint32 lenIn = name.getLength();
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

sl_uint32 DnsRecord::_parseHeader(const void *_buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8 *buf = (const sl_uint8*)_buf;
	String name;
	sl_uint32 pos = _parseName(name, buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 4 > size) {
		return 0;
	}
	_name = name;
	_type = MIO::readUint16BE(buf + pos);
	_class = MIO::readUint16BE(buf + pos + 2);
	return pos + 4;
}

sl_uint32 DnsRecord::_buildHeader(void *_buf, sl_uint32 offset, sl_uint32 size)
{
	sl_uint8* buf = (sl_uint8*)_buf;
	sl_uint32 pos = _buildName(_name, buf, offset, size);
	if (pos == 0) {
		return 0;
	}
	if (pos + 4 > size) {
		return 0;
	}
	MIO::writeUint16BE(buf + pos, _type);
	MIO::writeUint16BE(buf + pos + 2, _class);
	return pos + 4;
}

sl_uint32 DnsQuestionRecord::parseRecord(const void *buf, sl_uint32 offset, sl_uint32 size)
{
	return _parseHeader(buf, offset, size);
}

sl_uint32 DnsQuestionRecord::buildRecord(void *buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DnsResponseRecord::parseRecord(const void *_buf, sl_uint32 offset, sl_uint32 size)
{
	const sl_uint8 *buf = (const sl_uint8*)_buf;
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

sl_uint32 DnsResponseRecord::buildRecord(void *_buf, sl_uint32 offset, sl_uint32 size, const void *data, sl_uint16 sizeData)
{
	sl_uint8 *buf = (sl_uint8*)_buf;
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
	if (getType() == dnsRecordType_A && _dataLength == 4) {
		return IPv4Address(_message + _dataOffset);
	}
	return IPv4Address::any();
}

sl_uint32 DnsResponseRecord::buildRecord_A(void *buf, sl_uint32 offset, sl_uint32 size, const IPv4Address &addr)
{
	setType(dnsRecordType_A);
	sl_uint8 a[4];
	addr.get(a);
	
	return buildRecord(buf, offset, size, a, 4);
}

String DnsResponseRecord::parseData_CNAME() const
{
	if (getType() == dnsRecordType_CNAME) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return String::null();
}

sl_uint32 DnsResponseRecord::buildRecord_CNAME(void* buf, sl_uint32 offset, sl_uint32 size, const String& cname)
{
	setType(dnsRecordType_CNAME);
	return buildRecord(buf, offset, size, cname.getBuf(), cname.getLength());
}

String DnsResponseRecord::parseData_NS() const
{
	if (getType() == dnsRecordType_NS) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return String::null();
}

sl_uint32 DnsResponseRecord::buildRecord_NS(void* buf, sl_uint32 offset, sl_uint32 size, const String& ns)
{
	setType(dnsRecordType_NS);
	return buildRecord(buf, offset, size, ns.getBuf(), ns.getLength());
}

String DnsResponseRecord::toString() const
{
	String ret = getName() + " ";
	sl_uint32 type = getType();
	if (type == dnsRecordType_A) {
		ret += "A " + parseData_A().toString();
	} else if (type == dnsRecordType_CNAME) {
		ret += "CNAME " + parseData_CNAME();
	} else if (type == dnsRecordType_NS) {
		ret += "NS " + parseData_NS();
	} else {
		ret += String("TYPE=") + type;
	}
	return ret;
}

/*************************************************************
					DnsPacket
*************************************************************/
sl_bool DnsPacket::parsePacket(const void *packet, sl_uint32 size)
{
	sl_uint8 *buf = (sl_uint8*)packet;
	
	do {
		if (size < sizeof(DnsHeaderFormat)) {
			break;
		}
		DnsHeaderFormat *header = (DnsHeaderFormat*)buf;
		if (header->isQuestion()) { // is question?
			flagQuestion = sl_true;
		} else {
			flagQuestion = sl_false;
		}
		id = header->getId();
		
		sl_uint32 i, n;
		sl_uint32 offset = sizeof(DnsHeaderFormat);
		
		n = header->getQuestionsCount();
		for (i = 0; i < n; i++) {
			DnsQuestionRecord record;
			offset = record.parseRecord(buf, offset, size);
			if (offset == 0) {
				break;
			}
			DnsPacket::Question question;
			question.name = record.getName();
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
			sl_uint16 type = record.getType();
			if (type == dnsRecordType_A) {
				DnsPacket::Address item;
				item.name = record.getName();
				item.address = record.parseData_A();
				if (!(item.address.isAny())) {
					addresses.add(item);
				}
			} else if (type == dnsRecordType_CNAME) {
				DnsPacket::Alias item;
				item.name = record.getName();
				item.alias = record.parseData_CNAME();
				if (item.alias.isNotEmpty()) {
					aliases.add(item);
				}
			} else if (type == dnsRecordType_NS) {
				DnsPacket::NameServer item;
				item.name = record.getName();
				item.server = record.parseData_NS();
				if (item.server.isNotEmpty()) {
					nameServers.add(item);
				}
			}
		}
		
		return sl_true;
		
	} while (0);
	
	return sl_false;
}

Memory DnsPacket::buildQuestionPacket(sl_uint16 id, const String &host)
{
	char buf[1024];
	DnsHeaderFormat *header = (DnsHeaderFormat*)buf;
	Base::zeroMemory(buf, sizeof(DnsHeaderFormat));
	header->setQuestion(sl_true); // Question
	header->setId(id);
	header->setRD(sl_true); // Recursive Desired
	header->setOpcode(dnsOpcode_Query);
	header->setQuestionsCount(1);
	DnsQuestionRecord record;
	record.setName(host);
	record.setType(dnsRecordType_A);
	sl_uint32 size = record.buildRecord(buf, sizeof(DnsHeaderFormat), 1024);
	if (size > 0) {
		return Memory::create(buf, size);
	}
	return Memory::null();
}

Memory DnsPacket::buildHostAddressAnswerPacket(sl_uint16 id, const String &hostName, const IPv4Address &hostAddress)
{
	char buf[1024];
	Base::zeroMemory(buf, sizeof(buf));
	
	if (hostAddress.isNotZero()) {
		
		DnsHeaderFormat *header = (DnsHeaderFormat*)(buf);
		header->setId(id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(dnsOpcode_Query);
		header->setResponseCode(dnsResponseCode_NoError);
		header->setQuestionsCount(1);
		header->setAnswersCount(1);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);
		
		sl_uint32 offset = sizeof(DnsHeaderFormat);
		DnsQuestionRecord recordQuestion;
		recordQuestion.setName(hostName);
		recordQuestion.setType(dnsRecordType_A);
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
		
		DnsHeaderFormat *header = (DnsHeaderFormat*)(buf);
		header->setId(id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(dnsOpcode_Query);
		header->setResponseCode(dnsResponseCode_NameError);
		header->setQuestionsCount(1);
		header->setAnswersCount(0);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);
		
		sl_uint32 offset = sizeof(DnsHeaderFormat);
		DnsQuestionRecord recordQuestion;
		recordQuestion.setName(hostName);
		recordQuestion.setType(dnsRecordType_A);
		offset = recordQuestion.buildRecord(buf, offset, 1024);
		if (offset == 0) {
			return Memory::create(buf, offset);
		}
	}
	
	return Memory::null();
	
}

/*************************************************************
				DnsClient
*************************************************************/
DnsClient::DnsClient()
{
	m_idLast = 0;
}

Ref<DnsClient> DnsClient::create(const DnsClientParam &param, const Ref<AsyncIoLoop> &loop)
{
	Ref<DnsClient> ret = new DnsClient;
	if (ret.isNotNull()) {
		ret->m_listener = param.listener;
		Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create((WeakRef<DnsClient>)(ret), 4096, loop);
		if (socket.isNotNull()) {
			ret->m_udp = socket;
		}
	}
	return ret;
}

Ref<DnsClient> DnsClient::create(const DnsClientParam &param)
{
	return DnsClient::create(param, AsyncIoLoop::getDefault());
}

void DnsClient::sendQuestion(const SocketAddress &serverAddress, const String &hostName)
{
	sl_uint16 id = m_idLast++;
	Memory mem = DnsPacket::buildQuestionPacket(id, hostName);
	if (mem.isNotEmpty()) {
		m_udp->sendTo(serverAddress, mem);
	}
}

void DnsClient::sendQuestion(const IPv4Address &serverIp, const String &hostName)
{
	sendQuestion(SocketAddress(serverIp, SLIB_NETWORK_DNS_PORT), hostName);
}

void DnsClient::onReceiveFrom(AsyncUdpSocket *socket, const SocketAddress &address, void *data, sl_uint32 sizeReceive)
{
	DnsPacket packet;
	if (packet.parsePacket(data, sizeReceive)) {
		_onDnsAnswer(address, packet);
	}
}

void DnsClient::_onDnsAnswer(const SocketAddress &serverAddress, const DnsPacket &packet)
{
	PtrLocker<IDnsClientListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onDnsAnswer(this, serverAddress, packet);
	}
}

/*************************************************************
					DnsServer
*************************************************************/
sl_bool IDnsServerListener::resolveDnsHost(DnsServer *server, const String &hostName, IPv4Address &outAddr)
{
	return sl_false;
}

void IDnsServerListener::cacheDnsHost(DnsServer *server, const String &hostName, const IPv4Address &addr)
{
}

DnsServerParam::DnsServerParam()
{
	forwardTarget.setZero();
	flagUseCache = sl_false;
}

DnsServer::DnsServer()
{
	m_lastForwardId = 0;
	m_forwardTarget.setZero();
	
	m_flagUseCache = sl_false;
}

Ref<DnsServer> DnsServer::create(const DnsServerParam &param, const Ref<AsyncIoLoop> &loop)
{
	Ref<DnsServer> ret = new DnsServer;
	if (ret.isNotNull()) {
		ret->m_forwardTarget = param.forwardTarget;
		ret->m_flagUseCache = param.flagUseCache;
		ret->m_listener = param.listener;
		Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(SLIB_NETWORK_DNS_PORT, (WeakRef<DnsServer>)(ret), 4096, loop);
		if (socket.isNotNull()) {
			ret->m_udp = socket;
			return ret;
		} else {
			SLIB_LOG_ERROR("DNS Server", "Can not bind to DNS port 53");
		}
	}
	return ret;
}

void DnsServer::_processReceivedQuestion(const SocketAddress &clientAddress, sl_uint16 id, const String &hostName)
{
	IPv4Address hostAddress;
	sl_bool flagResolved = _resolveDnsHost(hostName, hostAddress);
	if (m_flagUseCache) {
		if (flagResolved) {
			m_cacheAddresses.put(hostName, hostAddress);
		} else {
			if (m_cacheAddresses.get(hostName, &hostAddress)) {
				flagResolved = sl_true;
			}
		}
	}
	if (flagResolved) {
		_sendPacket(clientAddress, DnsPacket::buildHostAddressAnswerPacket(id, hostName, hostAddress));
	} else {
		if (m_forwardTarget.isNotZero()) {
			sl_uint16 idForward = m_lastForwardId++;
			ForwardElement e;
			e.requestedId = id;
			e.requestedHostName = hostName;
			e.clientAddress = clientAddress;
			e.alias = hostName;
			e.countRequests = 1;
			m_mapForward.put(idForward, e);
			_forwardPacket(DnsPacket::buildQuestionPacket(idForward, hostName));
		} else {
			_sendPacket(clientAddress, DnsPacket::buildHostAddressAnswerPacket(id, hostName, IPv4Address::zero()));
		}
	}
}

#define MAX_RECURSIVE_COUNT 16

void DnsServer::_processReceivedAnswer(const DnsPacket &packet)
{
	sl_uint16 idForward = packet.id;
	ForwardElement e;
	if (m_mapForward.get(idForward, &e)) {
		// address
		{
			ListItems<DnsPacket::Address> addresses(packet.addresses);
			for (sl_size i = 0; i < addresses.count(); i++) {
				if (e.alias == addresses[i].name) {
					if (m_flagUseCache) {
						m_cacheAddresses.put(e.requestedHostName, addresses[i].address);
					}
					_cacheDnsHost(e.requestedHostName, addresses[i].address);
					_sendPacket(e.clientAddress, DnsPacket::buildHostAddressAnswerPacket(e.requestedId, e.requestedHostName, addresses[i].address));
					return;
				}
			}
		}
		// alias
		if (e.countRequests < MAX_RECURSIVE_COUNT) {
			ListItems<DnsPacket::Alias> aliases(packet.aliases);
			for (sl_size i = 0; i < aliases.count(); i++) {
				if (e.alias == aliases[i].name) {
					e.alias = aliases[i].alias;
					e.countRequests++;
					_forwardPacket(DnsPacket::buildQuestionPacket(idForward, e.alias));
					return;
				}
			}
		}
	}
}

void DnsServer::_sendPacket(const SocketAddress &clientAddress, const Memory &packet)
{
	m_udp->sendTo(clientAddress, packet);
}

void DnsServer::_forwardPacket(const Memory &packet)
{
	if (m_forwardTarget.isNotZero()) {
		m_udp->sendTo(SocketAddress(m_forwardTarget, SLIB_NETWORK_DNS_PORT), packet);
	}
}

void DnsServer::onReceiveFrom(AsyncUdpSocket *socket, const SocketAddress &address, void *data, sl_uint32 size)
{
	char *buf = (char*)data;
	DnsPacket packet;
	if (packet.parsePacket(buf, size)) {
		if (packet.flagQuestion) {
			if (packet.questions.count() == 1) {
				String hostName = packet.questions[0].name;
				_processReceivedQuestion(address, packet.id, hostName);
			}
		} else {
			if (address.ip == m_forwardTarget && address.port == SLIB_NETWORK_DNS_PORT) {
				_processReceivedAnswer(packet);
			}
		}
	}
}

sl_bool DnsServer::_resolveDnsHost(const String &hostName, IPv4Address &outAddr)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		return listener->resolveDnsHost(this, hostName, outAddr);
	}
	return sl_false;
}

void DnsServer::_cacheDnsHost(const String &hostName, const IPv4Address &address)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->cacheDnsHost(this, hostName, address);
	}
}

SLIB_NETWORK_NAMESPACE_END
