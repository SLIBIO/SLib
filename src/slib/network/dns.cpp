#include "../../../inc/slib/network/dns.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/log.h"

#define _MAX_NAME SLIB_NETWORK_DNS_NAME_MAX_LENGTH

SLIB_NETWORK_NAMESPACE_BEGIN
DNS_Record::DNS_Record()
{
	_type = 0;
	_class = class_IN;
}

sl_uint32 DNS_Record::_parseName(String& nameOut, const void* _buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DNS_Record::_buildName(const String& name, void* _buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DNS_Record::_parseHeader(const void* _buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DNS_Record::_buildHeader(void* _buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DNS_QuestionRecord::parseRecord(const void* buf, sl_uint32 offset, sl_uint32 size)
{
	return _parseHeader(buf, offset, size);
}

sl_uint32 DNS_QuestionRecord::buildRecord(void* buf, sl_uint32 offset, sl_uint32 size)
{
	return _buildHeader(buf, offset, size);
}

DNS_ResponseRecord::DNS_ResponseRecord()
{
	_message = 0;
	_messageLength = 0;
	_dataOffset = 0;
	_dataLength = 0;
}

sl_uint32 DNS_ResponseRecord::parseRecord(const void* _buf, sl_uint32 offset, sl_uint32 size)
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

sl_uint32 DNS_ResponseRecord::buildRecord(void* _buf, sl_uint32 offset, sl_uint32 size, const void* data, sl_uint16 sizeData)
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

IPv4Address DNS_ResponseRecord::parseData_A() const
{
	if (getType() == type_A && _dataLength == 4) {
		return IPv4Address(_message + _dataOffset);
	}
	return IPv4Address::any();
}

sl_uint32 DNS_ResponseRecord::buildRecord_A(void* buf, sl_uint32 offset, sl_uint32 size, const IPv4Address& addr)
{
	setType(type_A);
	sl_uint8 a[4];
	addr.get(a);
	return buildRecord(buf, offset, size, a, 4);
}

String DNS_ResponseRecord::parseData_CNAME() const
{
	if (getType() == type_CNAME) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return String::null();
}

sl_uint32 DNS_ResponseRecord::buildRecord_CNAME(void* buf, sl_uint32 offset, sl_uint32 size, const String& cname)
{
	setType(type_CNAME);
	return buildRecord(buf, offset, size, cname.getBuf(), cname.getLength());
}

String DNS_ResponseRecord::parseData_NS() const
{
	if (getType() == type_NS) {
		if (_message) {
			String str;
			_parseName(str, _message, _dataOffset, _messageLength);
			return str;
		}
	}
	return String::null();
}

sl_uint32 DNS_ResponseRecord::buildRecord_NS(void* buf, sl_uint32 offset, sl_uint32 size, const String& ns)
{
	setType(type_NS);
	return buildRecord(buf, offset, size, ns.getBuf(), ns.getLength());
}

String DNS_ResponseRecord::toString() const
{
	String ret = getName() + " ";
	sl_uint32 type = getType();
	if (type == type_A) {
		ret += "A " + parseData_A().toString();
	} else if (type == type_CNAME) {
		ret += "CNAME " + parseData_CNAME();
	} else if (type == type_NS) {
		ret += "NS " + parseData_NS();
	} else {
		ret += String("TYPE=") + type;
	}
	return ret;
}

/*************************************************************
				DnsClient
*************************************************************/
DnsClient::DnsClient()
{
	m_idLast = 0;
}

DnsClient::~DnsClient()
{
}

void DnsClient::sendQuestion(const SocketAddress& serverAddress, const DnsQuestion& question)
{
	char buf[1024];
	DNS_HeaderFormat* header = (DNS_HeaderFormat*)buf;
	Base::zeroMemory(buf, sizeof(DNS_HeaderFormat));
	header->setQuestion(sl_true); // Question
	header->setId(question.id);
	header->setRD(sl_true); // Recursive Desired
	header->setOpcode(DNS_HeaderFormat::opcodeQuery);
	header->setQuestionsCount(1);
	DNS_QuestionRecord record;
	record.setName(question.name);
	record.setType(DNS_QuestionRecord::type_A);
	sl_uint32 size = record.buildRecord(buf, sizeof(DNS_HeaderFormat), 1024);
	if (size != 0) {
		Memory mem = Memory::create(buf, size);
		if (mem.isNotEmpty()) {
			m_udp->sendTo(serverAddress, mem);
		}
	}
}

void DnsClient::sendQuestion(const IPv4Address& serverIp, const String& host)
{
	DnsQuestion question;
	question.id = m_idLast++;
	question.name = host;
	sendQuestion(SocketAddress(serverIp, SLIB_NETWORK_DNS_PORT), question);
}

void DnsClient::onReceiveFrom(AsyncUdpSocket* socket, void* data, sl_uint32 sizeReceive, const SocketAddress& address, sl_bool flagError)
{
	sl_uint8* buf = (sl_uint8*)data;
	sl_uint32 size = sizeReceive;
	do {
		if (flagError) {
			break;
		}
		if (size < sizeof(DNS_HeaderFormat)) {
			break;
		}
		DNS_HeaderFormat* header = (DNS_HeaderFormat*)buf;
		if (header->isQuestion()) { // is question?
			break;
		}
		DnsAnswer response;
		response.id = header->getId();
		
		sl_uint32 i, n;
		sl_uint32 offset = sizeof(DNS_HeaderFormat);

		n = header->getQuestionsCount();
		for (i = 0; i < n; i++) {
			DNS_QuestionRecord record;
			offset = record.parseRecord(buf, offset, size);
			if (offset == 0) {
				break;
			}
			DnsAnswer::Question question;
			question.name = record.getName();
			response.questions.add(question);
		}
		if (i != n) {
			break;
		}
		n = header->getAnswersCount() + header->getAuthoritiesCount() + header->getAdditionalsCount();
		for (i = 0; i < n; i++) {
			DNS_ResponseRecord record;
			offset = record.parseRecord(buf, offset, size);
			if (offset == 0) {
				break;
			}
			sl_uint16 type = record.getType();
			if (type == DNS_ResponseRecord::type_A) {
				DnsAnswer::Address item;
				item.name = record.getName();
				item.address = record.parseData_A();
				if (!(item.address.isAny())) {
					response.addresses.add(item);
				}
			} else if (type == DNS_ResponseRecord::type_CNAME) {
				DnsAnswer::Alias item;
				item.name = record.getName();
				item.alias = record.parseData_CNAME();
				if (item.alias.isNotEmpty()) {
					response.aliases.add(item);
				}
			} else if (type == DNS_ResponseRecord::type_NS) {
				DnsAnswer::NameServer item;
				item.name = record.getName();
				item.server = record.parseData_NS();
				if (item.server.isNotEmpty()) {
					response.nameServers.add(item);
				}
			}
		}

		onDnsAnswer(address, response);
	} while (0);
	m_udp->receiveFrom(m_memReceive, (WeakRef<DnsClient>)(this));
}

void DnsClient::onDnsAnswer(const SocketAddress& serverAddress, const DnsAnswer& response)
{
	PtrLocker<IDnsClientListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onDnsAnswer(this, serverAddress, response);
	}
}

Ref<DnsClient> DnsClient::create(const Ref<AsyncLoop>& loop)
{
	Ref<DnsClient> ret;
	Memory memReceive = Memory::create(4096);
	if (memReceive.isEmpty()) {
		return ret;
	}
	Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(loop);
	if (socket.isNotNull()) {
		ret = new DnsClient;
		if (ret.isNotNull()) {
			ret->m_udp = socket;
			ret->m_memReceive = memReceive;
			socket->receiveFrom(memReceive, (WeakRef<DnsClient>)(ret));
		}
	}
	return ret;
}

Ref<DnsClient> DnsClient::create()
{
	return DnsClient::create(AsyncLoop::getDefault());
}


/*************************************************************
						DnsServer
*************************************************************/
IPv4Address IDnsServerListener::onResolveDnsHost(DnsServer* server, const SocketAddress& clientAddress, const DnsQuestion& question)
{
	return IPv4Address::zero();
}

void IDnsServerListener::onCacheDnsHost(DnsServer* server, const String& name, const IPv4Address& addr)
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

DnsServer::~DnsServer()
{
	release();
}

void DnsServer::release()
{
	ObjectLocker lock(this);
	Ref<Thread> thread = m_thread;
	if (thread.isNotNull()) {
		thread->finishAndWait();
	}
	m_thread.setNull();
	Ref<Socket> udp = m_udp;
	if (udp.isNull()) {
		udp->close();
	}
	m_udp.setNull();
}

void DnsServer::sendHostAddressAnswer(const SocketAddress& clientAddress, const DnsQuestion& question, const IPv4Address& answer)
{
	Ref<Socket> udp = m_udp;
	if (udp.isNull()) {
		return;
	}

	char buf[1024];
	Base::zeroMemory(buf, sizeof(buf));

	if (answer.isNotZero()) {

		DNS_HeaderFormat* header = (DNS_HeaderFormat*)(buf);
		header->setId(question.id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(DNS_HeaderFormat::opcodeQuery);
		header->setResponseCode(DNS_HeaderFormat::responseNoError);
		header->setQuestionsCount(1);
		header->setAnswersCount(1);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);

		sl_uint32 offset = sizeof(DNS_HeaderFormat);
		DNS_QuestionRecord recordQuestion;
		recordQuestion.setName(question.name);
		recordQuestion.setType(DNS_QuestionRecord::type_A);
		offset = recordQuestion.buildRecord(buf, offset, 1024);
		if (offset == 0) {
			return;
		}

		DNS_ResponseRecord recordResponse;
		recordResponse.setName(question.name);
		offset = recordResponse.buildRecord_A(buf, offset, 1024, answer);
		if (offset == 0) {
			return;
		}

		udp->sendTo(buf, offset, clientAddress);

	} else {

		DNS_HeaderFormat* header = (DNS_HeaderFormat*)(buf);
		header->setId(question.id);
		header->setQuestion(sl_false); // Response
		header->setRD(sl_false);
		header->setOpcode(DNS_HeaderFormat::opcodeQuery);
		header->setResponseCode(DNS_HeaderFormat::responseNameError);
		header->setQuestionsCount(1);
		header->setAnswersCount(0);
		header->setAuthoritiesCount(0);
		header->setAdditionalsCount(0);

		sl_uint32 offset = sizeof(DNS_HeaderFormat);
		DNS_QuestionRecord recordQuestion;
		recordQuestion.setName(question.name);
		recordQuestion.setType(DNS_QuestionRecord::type_A);
		offset = recordQuestion.buildRecord(buf, offset, 1024);
		if (offset == 0) {
			return;
		}

		udp->sendTo(buf, offset, clientAddress);
	}

}

void DnsServer::_run()
{
	Ref<Socket> udp = m_udp;
	if (udp.isNull()) {
		return;
	}
	Ref<SocketEvent> ev = SocketEvent::createRead(udp);
	if (ev.isNull()) {
		return;
	}
	sl_uint8* buf = (sl_uint8*)(m_memReceive.getBuf());
	sl_uint32 sizeBuf = (sl_uint32)(m_memReceive.getSize());
	while (Thread::isNotStoppingCurrent()) {
		SocketAddress address;
		sl_int32 size = udp->receiveFrom(buf, sizeBuf, address);
		if (size >= sizeof(DNS_HeaderFormat)) {
			DNS_HeaderFormat* header = (DNS_HeaderFormat*)buf;
			if (header->isQuestion()) { // is response?
				if (header->getQuestionsCount() > 0) {
					sl_uint32 offset = sizeof(DNS_HeaderFormat);
					while (1) {
						DNS_QuestionRecord record;
						offset = record.parseRecord(buf, offset, size);
						if (offset == 0) {
							break;
						}
						if (record.getType() == DNS_Record::type_A) {
							String host = record.getName();
							if (host.isNotEmpty()) {
								DnsQuestion question;
								question.id = header->getId();
								question.name = host;
								IPv4Address ip = resolveDnsHost(address, question);
								sl_bool flagSent = sl_false;
								if (ip.isNotZero()) {
									sendHostAddressAnswer(address, question, ip);
									flagSent = sl_true;
								} else {
									if (m_flagUseCache) {
										if (m_mapDNSCache.get(question.name, &ip)) {
											sendHostAddressAnswer(address, question, ip);
											flagSent = sl_true;
										}
									}
								}
								if (!flagSent) {
									if (m_forwardTarget.isNotZero()) {
										sl_uint16 idTarget = m_lastForwardId++;
										sl_uint16 idSource = header->getId();
										m_mapForwardId.put(idTarget, idSource);
										m_mapForwardAddress.put(idTarget, address);
										header->setId(idTarget);
										udp->sendTo(buf, size, SocketAddress(m_forwardTarget, SLIB_NETWORK_DNS_PORT));
									} else {
										sendHostAddressAnswer(address, question, IPv4Address::zero());
									}
								}
							}
							break;
						}
					}
				}
			} else {
				sl_uint16 idSource = header->getId();
				sl_uint16 idTarget = 0;
				if (m_mapForwardId.get(idSource, &idTarget)) {
					SocketAddress addrTarget;
					if (m_mapForwardAddress.get(idSource, &addrTarget)) {
						header->setId(idTarget);
						udp->sendTo(buf, size, addrTarget);
					}
				}

				sl_uint32 i, n;
				sl_uint32 offset = sizeof(DNS_HeaderFormat);

				n = header->getQuestionsCount();
				for (i = 0; i < n; i++) {
					DNS_QuestionRecord record;
					offset = record.parseRecord(buf, offset, size);
					if (offset == 0) {
						break;
					}
				}
				if (i != n) {
					break;
				}
				n = header->getAnswersCount() + header->getAuthoritiesCount() + header->getAdditionalsCount();
				for (i = 0; i < n; i++) {
					DNS_ResponseRecord record;
					offset = record.parseRecord(buf, offset, size);
					if (offset == 0) {
						break;
					}
					sl_uint16 type = record.getType();
					if (type == DNS_ResponseRecord::type_A) {
						String name = record.getName();
						IPv4Address address = record.parseData_A();
						if (address.isNotZero()) {
							cacheDnsHost(name, address);
						}
					}
				}
			}
		}
		ev->wait();
	}
}

IPv4Address DnsServer::resolveDnsHost(const SocketAddress& clientAddress, const DnsQuestion& question)
{
	PtrLocker<IDnsServerListener> listener(getListener());
	if (listener.isNotNull()) {
		return listener->onResolveDnsHost(this, clientAddress, question);
	}
	return IPv4Address::zero();
}

void DnsServer::cacheDnsHost(const String& name, const IPv4Address& address)
{
	if (m_flagUseCache) {
		m_mapDNSCache.put(name, address);
	}
	PtrLocker<IDnsServerListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onCacheDnsHost(this, name, address);
	}
}

Ref<DnsServer> DnsServer::create(const DnsServerParam& param)
{
	Ref<DnsServer> ret;
	Memory memReceive = Memory::create(4096);
	if (memReceive.isEmpty()) {
		return ret;
	}
	Ref<Socket> socket = Socket::openUdp_IPv4();
	if (socket.isNotNull()) {
		if (socket->bind(SocketAddress(IPAddress::none(), SLIB_NETWORK_DNS_PORT))) {
			socket->setNonBlockingMode(sl_true);
			ret = new DnsServer;
			if (ret.isNotNull()) {
				ret->m_udp = socket;
				ret->m_memReceive = memReceive;
				ret->m_forwardTarget = param.forwardTarget;
				ret->setListener(param.listener);
				ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(DnsServer, _run, ret.get()));
			}
		} else {
			SLIB_LOG_ERROR("DNS Server", "Can not bind to DNS port 53");
		}
	}
	return ret;
}
SLIB_NETWORK_NAMESPACE_END
