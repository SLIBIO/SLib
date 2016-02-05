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
	_type = MIO::readUint16BE(buf + pos);
	_class = MIO::readUint16BE(buf + pos + 2);
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
	MIO::writeUint16BE(buf + pos, _type);
	MIO::writeUint16BE(buf + pos + 2, _class);
	return pos + 4;
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
	if (getType() == dnsRecordType_A && _dataLength == 4) {
		return IPv4Address(_message + _dataOffset);
	}
	return IPv4Address::any();
}

sl_uint32 DnsResponseRecord::buildRecord_A(void* buf, sl_uint32 offset, sl_uint32 size, const IPv4Address& addr)
{
	setType(dnsRecordType_A);
	return buildRecord(buf, offset, size, addr.bytes(), 4);
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
	return _buildName(cname, buf, offset, size);
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
	return _buildName(ns, buf, offset, size);
}

IPv6Address DnsResponseRecord::parseData_AAAA() const
{
	if (getType() == dnsRecordType_AAAA && _dataLength == 16) {
		return IPv6Address(_message + _dataOffset);
	}
	return IPv6Address::any();
}

sl_uint32 DnsResponseRecord::buildRecord_AAAA(void* buf, sl_uint32 offset, sl_uint32 size, const IPv6Address& addr)
{
	setType(dnsRecordType_AAAA);
	return buildRecord(buf, offset, size, addr.bytes(), 16);
}

String DnsResponseRecord::parseData_PTR() const
{
	if (getType() == dnsRecordType_PTR) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return String::null();
}

sl_uint32 DnsResponseRecord::buildRecord_PTR(void* buf, sl_uint32 offset, sl_uint32 size, const String& dname)
{
	setType(dnsRecordType_PTR);
	return _buildName(dname, buf, offset, size);
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
	} else if (type == dnsRecordType_AAAA) {
		ret += "AAAA " + parseData_AAAA().toString();
	} else if (type == dnsRecordType_PTR) {
		ret += "PTR " + parseData_PTR();
	} else {
		ret += String("TYPE=") + type;
	}
	return ret;
}

/*************************************************************
					DnsPacket
*************************************************************/
sl_bool DnsPacket::parsePacket(const void* packet, sl_uint32 size)
{
	sl_uint8* buf = (sl_uint8*)packet;
	
	do {
		if (size < sizeof(DnsHeaderFormat)) {
			break;
		}
		DnsHeaderFormat* header = (DnsHeaderFormat*)buf;
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
			sl_uint16 type = record.getType();
			if (type == dnsRecordType_A) {
				DnsPacket::Address item;
				item.name = record.getName();
				IPv4Address addr = record.parseData_A();
				if (addr.isNotZero()) {
					item.address = addr;
					addresses.add(item);
				}
			} else if (type == dnsRecordType_AAAA) {
				DnsPacket::Address item;
				item.name = record.getName();
				IPv6Address addr = record.parseData_AAAA();
				if (addr.isNotZero()) {
					item.address = addr;
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
			} else if (type == dnsRecordType_PTR) {
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
	DnsHeaderFormat* header = (DnsHeaderFormat*)buf;
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

Memory DnsPacket::buildHostAddressAnswerPacket(sl_uint16 id, const String& hostName, const List<IPAddress>& _hostAddresses)
{
	char buf[4096];
	Base::zeroMemory(buf, sizeof(buf));
	
	IPv4Address hostAddress;
	hostAddress.setZero();
	{
		ListLocker<IPAddress> addrs(_hostAddresses);
		for (sl_size i = 0; i < addrs.count(); i++) {
			if (addrs[i].isIPv4()) {
				hostAddress = addrs[i].getIPv4();
				if (hostAddress.isNotZero()) {
					break;
				}
			}
		}
	}
	
	if (hostAddress.isNotZero()) {
		
		DnsHeaderFormat* header = (DnsHeaderFormat*)(buf);
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
		
		DnsHeaderFormat* header = (DnsHeaderFormat*)(buf);
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

Ref<DnsClient> DnsClient::create(const DnsClientParam& param, const Ref<AsyncIoLoop>& loop)
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

Ref<DnsClient> DnsClient::create(const DnsClientParam& param)
{
	return DnsClient::create(param, AsyncIoLoop::getDefault());
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
DnsServerParam::DnsServerParam()
{
	forwardTarget.setZero();
}

DnsServer::DnsServer()
{
	m_lastForwardId = 0;
	m_forwardTarget.setZero();
}

Ref<DnsServer> DnsServer::create(const DnsServerParam& param, const Ref<AsyncIoLoop>& loop)
{
	Ref<DnsServer> ret = new DnsServer;
	if (ret.isNotNull()) {
		ret->m_forwardTarget = param.forwardTarget;
		ret->m_listener = param.listener;
		Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(SLIB_NETWORK_DNS_PORT, (WeakRef<DnsServer>)(ret), 4096, loop);
		if (socket.isNotNull()) {
			ret->m_udp = socket;
			return ret;
		} else {
			SLIB_LOG_ERROR("DNS Server", "Failed to bind DNS port 53");
		}
	}
	return Ref<DnsServer>::null();
}

Ref<DnsServer> DnsServer::create(const DnsServerParam& param)
{
	return create(param, AsyncIoLoop::getDefault());
}

void DnsServer::_processReceivedQuestion(const SocketAddress& clientAddress, sl_uint16 id, const String& hostName)
{
	Time now = Time::now();
	CacheElement ce;
	sl_bool flagExistInCache = m_cacheAddresses.get(hostName, &ce);
	if (flagExistInCache) {
		if ((now - ce.lastCachedTime).getMillisecondsCount() < 5000) {
			_sendPacket(clientAddress, DnsPacket::buildHostAddressAnswerPacket(id, hostName, ce.addresses));
			return;
		}
	}
	sl_bool flagStatic = sl_false;
	List<IPAddress> addresses = _resolveDnsHost(hostName, flagStatic);
	if (addresses.isNotNull()) {
		ce.addresses = addresses;
		ce.lastCachedTime = now;
		ce.flagStatic = flagStatic;
		m_cacheAddresses.put(hostName, ce);
	} else {
		if (flagExistInCache) {
			addresses = ce.addresses;
		}
	}
	if (addresses.isNotNull()) {
		_sendPacket(clientAddress, DnsPacket::buildHostAddressAnswerPacket(id, hostName, addresses));
	} else {
		if (m_forwardTarget.isZero()) {
			_sendPacket(clientAddress, DnsPacket::buildHostAddressAnswerPacket(id, hostName, List<IPAddress>::null()));
		}
	}
	
	if (!flagStatic) {
		sl_uint16 idForward = m_lastForwardId++;
		ForwardElement fe;
		fe.requestedId = id;
		fe.requestedHostName = hostName;
		fe.clientAddress = clientAddress;
		m_mapForward.put(idForward, fe);
		_forwardPacket(DnsPacket::buildQuestionPacket(idForward, hostName));
	}

}

void DnsServer::_processReceivedAnswer(const DnsPacket& packet)
{
	Time now = Time::now();
	sl_uint16 idForward = packet.id;
	ForwardElement fe;
	if (m_mapForward.get(idForward, &fe)) {

		String reqNameLower = fe.requestedHostName.toLower();

		List<IPAddress> resolvedAddresses;
		HashMap<String, IPv4Address> aliasAddresses4;
		HashMap<String, IPv6Address> aliasAddresses6;
		// address
		{
			ListItems<DnsPacket::Address> addresses(packet.addresses);
			for (sl_size i = 0; i < addresses.count(); i++) {
				DnsPacket::Address& address = addresses[i];
				if (address.address.isNotNone()) {
					_cacheDnsHost(address.name, address.address);
					if (address.address.isIPv4()) {
						aliasAddresses4.put_NoLock(address.name.toLower(), address.address.getIPv4());
					} else if (address.address.isIPv6()) {
						aliasAddresses6.put_NoLock(address.name.toLower(), address.address.getIPv6());
					}
					if (reqNameLower == address.name.toLower()) {
						resolvedAddresses.addIfNotExist_NoLock(address.address);
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
				ListItems<DnsPacket::Alias> aliases(aliasesProcess);
				for (sl_size i = 0; i < aliases.count(); i++) {
					DnsPacket::Alias& alias = aliases[i];
					IPv4Address addr4;
					IPv6Address addr6;
					sl_bool flagAddr = sl_false;
					if (aliasAddresses4.get_NoLock(alias.alias.toLower(), &addr4)) {
						aliasAddresses4.put_NoLock(alias.name.toLower(), addr4);
						_cacheDnsHost(alias.name, addr4);
						if (reqNameLower == alias.name.toLower()) {
							resolvedAddresses.addIfNotExist_NoLock(addr4);
						}
						flagProcess = sl_true;
						flagAddr = sl_true;
					}
					if (aliasAddresses6.get_NoLock(alias.alias.toLower(), &addr6)) {
						aliasAddresses6.put_NoLock(alias.name.toLower(), addr6);
						_cacheDnsHost(alias.name, addr6);
						if (reqNameLower == alias.name.toLower()) {
							resolvedAddresses.addIfNotExist_NoLock(addr6);
						}
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
		if (resolvedAddresses.isNotEmpty()) {
			CacheElement ce;
			ce.addresses = resolvedAddresses;
			ce.lastCachedTime = now;
			ce.flagStatic = sl_false;
			m_cacheAddresses.put(fe.requestedHostName, ce);
		}
		_sendPacket(fe.clientAddress, DnsPacket::buildHostAddressAnswerPacket(fe.requestedId, fe.requestedHostName, resolvedAddresses));

	}
}

void DnsServer::_sendPacket(const SocketAddress& clientAddress, const Memory& packet)
{
	m_udp->sendTo(clientAddress, packet);
}

void DnsServer::_forwardPacket(const Memory& packet)
{
	if (m_forwardTarget.isNotZero()) {
		m_udp->sendTo(SocketAddress(m_forwardTarget, SLIB_NETWORK_DNS_PORT), packet);
	}
}

void DnsServer::onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& address, void* data, sl_uint32 size)
{
	char* buf = (char*)data;
	DnsPacket packet;
	if (packet.parsePacket(buf, size)) {
		if (packet.flagQuestion) {
			if (packet.questions.count() == 1) {
				DnsPacket::Question& question = packet.questions[0];
				if (question.type == dnsRecordType_A) {
					_processReceivedQuestion(address, packet.id, question.name);
				}
			}
		} else {
			if (address.ip == m_forwardTarget && address.port == SLIB_NETWORK_DNS_PORT) {
				_processReceivedAnswer(packet);
			}
		}
	}
}

List<IPAddress> DnsServer::_resolveDnsHost(const String& hostName, sl_bool& flagStatic)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		return listener->resolveDnsHost(this, hostName, flagStatic);
	}
	return List<IPAddress>::null();
}

void DnsServer::_cacheDnsHost(const String& hostName, const IPAddress& hostAddress)
{
	PtrLocker<IDnsServerListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->cacheDnsHost(this, hostName, hostAddress);
	}
}

SLIB_NETWORK_NAMESPACE_END
