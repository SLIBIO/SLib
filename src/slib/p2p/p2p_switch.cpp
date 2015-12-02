#include "../../../inc/slib/p2p/switch.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/pointer.h"
#include "../../../inc/slib/core/log.h"

#include "../../../inc/slib/crypto/aes.h"
#include "../../../inc/slib/crypto/rsa.h"
#include "../../../inc/slib/crypto/sha2.h"

#define PACKET_SIZE (SLIB_P2P_PACKET_MAX_SIZE + 1000)

SLIB_P2P_NAMESPACE_BEGIN

class _P2PSwitch_Impl : public P2PSwitch
{
public:
	class _DirectUdpSession : public P2PSwitchDirectUdpSession
	{
	public:
		_DirectUdpSession()
		{
			m_remoteSessionId = 0;
			m_flagConnected = 0;

			m_timeLastUdpKeepAliveRequest.setZero();
		}

		SocketAddress m_remoteAddress;
		sl_uint64 m_localSessionId;
		sl_uint64 m_remoteSessionId;
		sl_bool m_flagConnected;

		Time m_timeLastUdpKeepAliveRequest;

		friend class _P2PSwitch_Impl;
	};

	class _TurningUdpSession : public P2PSwitchTurningUdpSession
	{
	public:
		_TurningUdpSession()
		{
			m_flagConnected = sl_false;
		}

		String m_serviceId;
		sl_bool m_flagConnected;

		friend class _P2PSwitch_Impl;
	};

	class _Host : public P2PSwitchHost
	{
	public:
		_Host()
		{
			m_flagConnected = sl_false;
			m_flagConnectedDirectUdp = sl_false;
			m_flagConnectedTurningUdp = sl_false;
			
			m_timeLastUdpQueryHostRequest.setZero();
			m_timeLastUdpDirectHolePunching.setZero();
		}

		sl_bool m_flagConnected;
		sl_bool m_flagConnectedDirectUdp;
		sl_bool m_flagConnectedTurningUdp;

		Ref<_DirectUdpSession> m_sessionBestDirectUdp;
		List< Ref<_DirectUdpSession> > m_sessionsDirectUdp;
		Ref<_DirectUdpSession> m_sessionBestTurningUdp;
		List< Ref<_TurningUdpSession> > m_sessionsTurningUdp;

		List< SocketAddress > m_addressesUdpHolePunching;
		List< SocketAddress > m_addressesUdpHolePunchingPredict;
		List< SocketAddress > m_addressesUdpDirect;

		List< Ref<P2PSwitchDirectUdpSession> > getDirectUdpSessions() const
		{
			return List< Ref<P2PSwitchDirectUdpSession> >::fromReference(m_sessionsDirectUdp.duplicate().getReference());
		}

		List< Ref<P2PSwitchTurningUdpSession> > getTurningUdpSessions() const
		{
			return List< Ref<P2PSwitchTurningUdpSession> >::fromReference(m_sessionsTurningUdp.duplicate().getReference());
		}
		
		
		Time m_timeLastUdpQueryHostRequest;
		Time m_timeLastUdpDirectHolePunching;

		friend class _P2PSwitch_Impl;
	};

private:
	sl_bool m_flagRunning;

	Ref<Socket> m_socketUdp;
	sl_uint32 m_portUdp;

	Ref<Thread> m_threadInterval;
	Ref<Thread> m_threadUdpMain;
	
	String m_hostId;
	String m_hostCertificate;
	String m_hostCertificateForServices;

	Map< String, Ref<HnsClient> > m_services;
	Map< String, Ref<_Host> > m_hosts;
	Map< sl_uint64, Ref<_DirectUdpSession> > m_sessionsDirectUdp;

	char m_iv[16];

	sl_uint32 m_connectionTimeoutMilliseconds;
	sl_uint32 getConnectionTimeoutMilliseconds()
	{
		return m_connectionTimeoutMilliseconds;
	}
	void setConnectionTimeoutMilliseconds(sl_uint32 millis)
	{
		MutexLocker lock(getLocker());
		m_connectionTimeoutMilliseconds = millis;
		ListLocker< Ref<HnsClient> > services(m_services.values());
		for (sl_size i = 0; i < services.count(); i++) {
			Ref<HnsClient> service = services[i];
			if (service.isNotNull()) {
				service->setConnectionTimeoutMilliseconds(millis);
			}
		}
	}
	sl_uint32 m_keepAliveIntervalMilliseconds;
	sl_uint32 getKeepAliveIntervalMilliseconds()
	{
		return m_keepAliveIntervalMilliseconds;
	}
	void setKeepAliveIntervalMilliseconds(sl_uint32 millis)
	{
		MutexLocker lock(getLocker());
		m_keepAliveIntervalMilliseconds = millis;
		ListLocker< Ref<HnsClient> > services(m_services.values());
		for (sl_size i = 0; i < services.count(); i++) {
			Ref<HnsClient> service = services[i];
			if (service.isNotNull()) {
				service->setKeepAliveIntervalMilliseconds(millis);
			}
		}
	}

	class _ServiceListener : public Referable, public IHnsClientListener
	{
	public:
		_P2PSwitch_Impl* m_parent;

		_ServiceListener(_P2PSwitch_Impl* parent)
		{
			m_parent = parent;
		}

		void onConnected(HnsClient* client)
		{
			m_parent->onServiceConnectedUdp(client);
		}

		void onQueryHost(HnsClient* client, const HnsHostInfo& info)
		{
			m_parent->onServiceQueryHostUdp(client, info);
		}

		void onMessage(HnsClient* client, String senderId, sl_uint64 messageId, const void* message, sl_uint32 size)
		{
			m_parent->onServiceMessageUdp(client, senderId, messageId, message, size);
		}

		void onBroadcastMessage(HnsClient* client, String senderId, sl_uint64 messageId, const void* message, sl_uint32 size)
		{
			m_parent->onServiceBroadcastMessageUdp(client, senderId, messageId, message, size);
		}

		void onKeepAlive(HnsClient* client, Time serviceTimeSent, sl_uint64 milliseconsdsEllapsed)
		{
			m_parent->onServiceKeepAliveUdp(client, serviceTimeSent, milliseconsdsEllapsed);
		}
	};
	Ref<_ServiceListener> m_listenerServiceUdp;


	SLIB_INLINE _P2PSwitch_Impl()
	{
		m_flagRunning = sl_true;

		m_services.initHash();
		m_hosts.initTree();

		Math::randomMemory(m_iv, 16);

		m_listenerServiceUdp = new _ServiceListener(this);

	}

	static void logError(String err)
	{
		SLIB_LOG("P2PSwitch", err);
	}

public:
	~_P2PSwitch_Impl()
	{
		release();
	}

	static Ref<_P2PSwitch_Impl> start(const P2PSwitchParam& param)
	{
		Ref<_P2PSwitch_Impl> ret;

		Ref<Socket> socketUdp = Socket::openUdp_IPv4();
		if (socketUdp.isNotNull()) {
			
			socketUdp->setNonBlockingMode(sl_true);
			socketUdp->setOption_Broadcast(sl_true);
			socketUdp->setOption_IpTTL(128);
			socketUdp->setOption_SendBufferSize(1024 << 8);
			socketUdp->setOption_ReceiveBufferSize(1024 << 8);

			if (socketUdp->bind(SocketAddress(param.hostPortUdp))) {
				
				ret = new _P2PSwitch_Impl();

				if (ret.isNotNull()) {
					ret->m_hostId = param.hostId;
					ret->m_hostCertificate = param.hostCertificate;
					ret->m_hostCertificateForServices = param.hostCertificateForServices;

					ret->m_connectionTimeoutMilliseconds = param.connectionTimeoutMilliseconds;
					ret->m_keepAliveIntervalMilliseconds = param.keepAliveIntervalMilliseconds;

					ret->setListener(param.listener);

					ret->m_socketUdp = socketUdp;
					ret->m_portUdp = param.hostPortUdp;

					ret->m_threadInterval = Thread::start(SLIB_CALLBACK_REF(_P2PSwitch_Impl, runInterval, ret));
					ret->m_threadUdpMain = Thread::start(SLIB_CALLBACK_REF(_P2PSwitch_Impl, runUdp, ret));
					return ret;
				}

			} else {
				logError(String("Can not bind to udp port - ") + param.hostPortUdp);
			}
			socketUdp->close();
		} else {
			logError("Can not open udp socket");
		}
		return ret;
	}

	void runInterval()
	{
		sl_uint32 interval = 0;
		HashMap<String, sl_uint32> punchLog;

		while (Thread::isNotStoppingCurrent()) {

			// ip addresses
			ListLocker<IPv4AddressInfo> localIps(NetworkAddress::findAllIPv4AddressInfos());

			Time now = Time::now();

			// run service intervals
			{
				ListLocker< Ref<HnsClient> > services(m_services.values());
				for (sl_size i = 0; i < services.count(); i++) {
					Ref<HnsClient> service = services[i];
					if (service.isNotNull()) {
						service->runStep();
						Thread::sleep(1);
					}
				}
			}
			// run host intervals
			{
				ListLocker< Ref<_Host> > hosts(m_hosts.values());
				sl_uint32 nHosts = (sl_uint32)(hosts.count());
				sl_uint32 step = (sl_uint32)(500.0 / nHosts);
				for (sl_size i = 0; i < nHosts; i++) {
					Ref<_Host> host = hosts[i];
					
					if (host.isNotNull()) {

						String hostId = host->getHostId();

						if ((now - host->m_timeLastCommunicated).getMillisecondsCount() > m_connectionTimeoutMilliseconds) {
							host->m_flagConnected = sl_false;
						} else {
							host->m_flagConnected = sl_true;
						}
						if ((now - host->m_timeLastCommunicatedDirectUdp).getMillisecondsCount() > m_connectionTimeoutMilliseconds) {
							host->m_flagConnectedDirectUdp = sl_false;
						} else {
							host->m_flagConnectedDirectUdp = sl_true;
						}
						if ((now - host->m_timeLastCommunicatedTurningUdp).getMillisecondsCount() > m_connectionTimeoutMilliseconds) {
							host->m_flagConnectedTurningUdp = sl_false;
						} else {
							host->m_flagConnectedTurningUdp = sl_true;
						}

						// UdpDirectSession
						{
							ListLocker< Ref<_DirectUdpSession> > sessions(host->m_sessionsDirectUdp.duplicate());
							sl_uint32 n = (sl_uint32)(sessions.count());
							if (n > 0) {
								for (sl_size i = 0; i < n; i++) {
									Ref<_DirectUdpSession> session = sessions[i];
									if ((now - session->m_timeLastCommunicated).getMillisecondsCount() > m_connectionTimeoutMilliseconds) {
										session->m_flagConnected = sl_false;
										if ((now - session->m_timeLastUdpKeepAliveRequest).getMillisecondsCount() > m_keepAliveIntervalMilliseconds) {
											_keepAliveDirectUdp(session.get());
											Thread::sleep(1);
											session->m_timeLastUdpKeepAliveRequest = now;
										}
									} else {
										session->m_flagConnected = sl_true;
									}
								}
							}
						}

						// TurningUdpSession
						{
							ListLocker< Ref<_TurningUdpSession> > sessions(host->m_sessionsTurningUdp.duplicate());
							sl_uint32 n = (sl_uint32)(sessions.count());
							if (n > 0) {
								for (sl_size i = 0; i < n; i++) {
									Ref<_TurningUdpSession> session = sessions[i];
									if ((now - session->m_timeLastCommunicated).getMillisecondsCount() > m_connectionTimeoutMilliseconds) {
										session->m_flagConnected = sl_false;
									}
									else {
										session->m_flagConnected = sl_true;
									}
								}
							}
						}

						// query host
						{
							if ((now - host->m_timeLastUdpQueryHostRequest).getMillisecondsCount() > m_keepAliveIntervalMilliseconds) {
								ListLocker< Ref<HnsClient> > services(m_services.values());
								for (sl_size i = 0; i < services.count(); i++) {
									Ref<HnsClient> service = services[i];
									if (service.isNotNull() && service->isConnected()) {
										service->queryHost(hostId);
										Thread::sleep(1);
									}
								}
								_send_method_message_query_addresses_request(hostId);
								Thread::sleep(1);

								host->m_timeLastUdpQueryHostRequest = now;
							}
						}

						// udp direct hole punching
						{
							if ((now - host->m_timeLastUdpDirectHolePunching).getMillisecondsCount() > m_keepAliveIntervalMilliseconds) {
								ListLocker< SocketAddress > addresses(host->m_addressesUdpHolePunching.duplicate());
								for (sl_size i = 0; i < addresses.count(); i++) {
									const SocketAddress& address = addresses[i];
									_send_ping_knock_udp(address);
									Thread::sleep(1);
								}
								host->m_timeLastUdpDirectHolePunching = now;
							}
						}

						// udp port hole punching
						if (1) {
							sl_bool flagExistDirectChannel = sl_false;
							{
								ListLocker< Ref<_DirectUdpSession> > sessions(host->m_sessionsDirectUdp.duplicate());
								sl_uint32 n = (sl_uint32)(sessions.count());
								for (sl_size i = 0; i < n; i++) {
									Ref<_DirectUdpSession> session = sessions[i];
									if (session->m_flagConnected) {
										flagExistDirectChannel = sl_true;
										break;
									}
								}
							}
							if (!flagExistDirectChannel) {
								ListLocker< SocketAddress > addresses(host->m_addressesUdpHolePunchingPredict.duplicate());
								for (sl_size i = 0; i < addresses.count(); i++) {
									const SocketAddress& addressOrig = addresses[i];
									SocketAddress address;
									address.ip = addressOrig.ip;
									sl_uint16 portOrig = (sl_uint16)(addressOrig.port);
									sl_uint32 log = 0;
									
									int SEG = 512;
									punchLog.get(addressOrig.toString(), &log);
									if (log < 0x10000) {
										{
											for (sl_uint16 k = 0; k < SEG; k++) {
												address.port = (sl_uint16)(portOrig + log + k);
												_send_ping_knock_udp(address);
											}
										}
										{
											for (sl_uint16 k = 0; k < SEG; k++) {
												address.port = (sl_uint16)(portOrig - log - SEG + k);
												_send_ping_knock_udp(address);
											}
										}
										log += SEG;
										punchLog.put(addressOrig.toString(), log);
									}
								}
							}
						}

						Thread::sleep(step);
					}
				}
				
				// broadcast ping
				if (interval % 3 == 0) {
					for (sl_size i = 0; i < localIps.count(); i++) {
						IPv4AddressInfo& info = localIps[i];
						IPv4Address addr = info.getNetworkMask();
						IPv4Address broadcastAddr((info.address.toInt() & addr.toInt()) | (~(addr.toInt())));
						_send_ping_request_udp(SocketAddress(broadcastAddr, m_portUdp), String::null(), sl_true);
						Thread::sleep(1);
					}
				}
			}
			Thread::sleep(1000);
			interval++;
		}
	}

	void runUdp()
	{
		Ref<SocketEvent> ev = SocketEvent::createRead(m_socketUdp);
		if (ev.isNull()) {
			return;
		}
		while (Thread::isNotStoppingCurrent()) {
			SocketAddress address;
			char bufPacket[PACKET_SIZE];
			sl_int32 n = m_socketUdp->receiveFrom(bufPacket, PACKET_SIZE, address);
			if (!m_flagRunning) {
				return;
			}
			if (n == 1 && bufPacket[0] == 1) {
				// ping knock
				_send_ping_request_udp(address, String::null());
			}else if (n >= 8) {
				sl_uint32 h = SHA256::make32bitChecksum(bufPacket + 4, n - 4);
				if (h == MIO::readUint32(bufPacket)) {
					sl_uint32 method = bufPacket[4] & 15;
					if (method >= 8) {
						for (sl_int32 i = 8; i < n; i++) {
							bufPacket[i] ^= bufPacket[4 + (i & 3)];
						}
						if (method == 8) {
							_receive_ping_request_udp(address, bufPacket + 8, n - 8);
						} else if (method == 9) {
							_receive_ping_response_udp(address, bufPacket + 8, n - 8);
						} else if (method == 10) {
							_receive_direct_message_udp(address, bufPacket + 8, n - 8);
						}
					} else {
						ListLocker< Ref<HnsClient> > services(m_services.values());
						for (sl_size i = 0; i < services.count(); i++) {
							Ref<HnsClient> service = services[i];
							if (service.isNotNull()) {
								if (service->processReceivedPacket(address, method, bufPacket + 8, n - 8)) {
									break;
								}
							}
						}
					}
				}
			} else {
				ev->wait();
			}
		}
	}

	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return;
		}

		m_flagRunning = sl_false;
		if (m_threadInterval.isNotNull()) {
			m_threadInterval->finishAndWait();
			m_threadInterval.setNull();
		}
		if (m_threadUdpMain.isNotNull()) {
			m_threadUdpMain->finishAndWait();
			m_threadUdpMain.setNull();
		}
		if (m_socketUdp.isNotNull()) {
			m_socketUdp->close();
		}
	}

	sl_bool isRunning()
	{
		return m_flagRunning;
	}

	sl_bool addService(const P2PSwitchServiceInfo& info)
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return sl_false;
		}

		removeService(info.serviceId);
		sl_uint32 servicePubKeyLen = info.servicePublicKey.getLength();
		if (servicePubKeyLen < 128 || servicePubKeyLen > 512) {
			logError("Connect To Service - Service RSA Key length can be between 1024~4096 bits");
			return sl_false;
		}
		Ref<HnsClient> service;
		if (info.flagUdp) {
			HnsClientParam param;
			param.name = info.serviceId;
			param.clientId = m_hostId;
			param.clientCertificate = m_hostCertificateForServices;
			param.serviceAddress = info.serviceAddress;
			param.servicePublicKey = info.servicePublicKey;
			param.serviceSecret = info.serviceSecret;
			param.keepAliveIntervalMilliseconds = m_keepAliveIntervalMilliseconds;
			param.connectionTimeoutMilliseconds = m_connectionTimeoutMilliseconds;
			param.listener = m_listenerServiceUdp;
			param.socket = m_socketUdp;
			param.sessionSecret = param.serviceSecret;
			service = HnsClient::createUdp(param);
		}
		if (service.isNull()) {
			return sl_false;
		}

		m_services.put(info.serviceId, service);

		return sl_true;
	}

	sl_bool removeService(String serviceId)
	{
		MutexLocker lock(getLocker());

		Ref<HnsClient> service;
		m_services.get(serviceId, &service);
		if (service.isNotNull()) {
			service->release();
			m_services.remove(serviceId);
			return sl_true;
		}
		return sl_false;
	}

	Ref<HnsClient> getService(String serviceId)
	{
		Ref<HnsClient> service;
		m_services.get(serviceId, &service);
		if (service.isNotNull()) {
			return service;
		} else {
			return Ref<HnsClient>::null();
		}
	}

	Ref<P2PSwitchHost> addHost(String hostId)
	{
		return Ref<P2PSwitchHost>::from(_registerHost(hostId));
	}

	sl_bool removeHost(String hostId)
	{
		MutexLocker lock(getLocker());
		Ref<_Host> host;
		m_hosts.get(hostId, &host);
		if (host.isNull()) {
			return sl_false;
		}
		// remove sessions
		{
			ListLocker< Ref<_DirectUdpSession> > sessions(host->m_sessionsDirectUdp);
			for (sl_size i = 0; i < sessions.count(); i++) {
				const Ref<_DirectUdpSession>& session = sessions[i];
				if (session.isNotNull()) {
					m_sessionsDirectUdp.remove(session->m_localSessionId);
				}
			}
		}
		m_hosts.remove(hostId);
		return sl_true;
	}

	Ref<P2PSwitchHost> getHost(String hostId)
	{
		return Ref<P2PSwitchHost>::from(_getHost(hostId));
	}

	void sendBroadcastMessage(sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		if (!m_flagRunning) {
			return;
		}
		if (n > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		ListLocker< Ref<HnsClient> > services(m_services.values());
		for (sl_size i = 0; i < services.count(); i++) {
			Ref<HnsClient> service = services[i];
			if (service.isNotNull()) {
				service->broadcastMessage(messageId, data, n);
			}
		}
	}

	void sendMessageThroughAllServices(const String& targetId, sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		if (!m_flagRunning) {
			return;
		}
		if (n > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		ListLocker< Ref<HnsClient> > services(m_services.values());
		for (sl_size i = 0; i < services.count(); i++) {
			Ref<HnsClient> service = services[i];
			if (service.isNotNull()) {
				service->sendMessage(targetId, messageId, data, n);
			}
		}
	}

	sl_bool sendMessage(String hostId, sl_uint64 messageId, const void* data, sl_uint32 n, MessageMode mode)
	{
		if (messageId == 0) {
			return sl_false;
		}
		return _sendMessage(hostId, messageId, data, n, mode);
	}

	void sendMessage(Ref<P2PSwitchSession> session, sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		if (messageId == 0) {
			return;
		}
		_sendMessage(session, messageId, data, n);
	}

	void onServiceConnectedUdp(HnsClient* client)
	{
		PtrLocker<IP2PSwitchListener> listener(getListener());
		if (listener.isNotNull()) {
			P2PSwitchServiceInfo info;
			info.flagUdp = sl_true;
			info.serviceId = client->getServiceName();
			info.serviceAddress = client->getServiceAddress();
			info.servicePublicKey = client->getServicePublicKey();
			listener->onServiceConnected(this, info);
		}
	}

	void onServiceQueryHostUdp(HnsClient* client, const HnsHostInfo& info)
	{
		Ref<_Host> host = _getHost(info.clientId);
		if (host.isNull()) {
			return;
		}
		if (info.clientAddress.isValid()) {
			host->m_addressesUdpHolePunching.addIfNotExist(info.clientAddress);
			host->m_addressesUdpHolePunchingPredict.addIfNotExist(info.clientAddress);
			
			Time t = Time::now() - info.elapsedMillisecondsFromLastCommunicated;
			if (t > host->m_timeLastCommunicatedTurningUdp) {
				host->m_timeLastCommunicatedTurningUdp = t;
			}
			Ref<_TurningUdpSession> session = _registerTurningUdpSession(host.get(), client);
			if (session.isNull()) {
				return;
			}
			if (t > session->m_timeLastCommunicated) {
				session->m_timeLastCommunicated = t;
			}
		}
	}

	void onServiceMessageUdp(HnsClient* client, String senderId, sl_uint64 messageId, const void* data, sl_uint32 size)
	{
		Ref<_Host> host = _getHost(senderId);
		if (host.isNull()) {
			host = Ref<_Host>::from(addHost(senderId));
			if (host.isNull()) {
				return;
			}
		}
		Time now = Time::now();
		host->m_timeLastCommunicated = now;
		host->m_timeLastCommunicatedTurningUdp = now;
		Ref<_TurningUdpSession> session = _registerTurningUdpSession(host.get(), client);
		if (session.isNull()) {
			return;
		}
		session->m_timeLastCommunicated = now;
		_receive_message(session.get(), messageId, data, size);
	}

	void onServiceBroadcastMessageUdp(HnsClient* client, String senderId, sl_uint64 messageId, const void* data, sl_uint32 size)
	{
		PtrLocker<IP2PSwitchListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onReceiveBroadcast(this, senderId, messageId, data, size);
		}
	}

	void onServiceKeepAliveUdp(HnsClient* client, Time serviceTimeSent, sl_uint64 milliseconsdsEllapsed)
	{
	}

	void _keepAliveDirectUdp(_DirectUdpSession* session)
	{
		if (session) {
			Ref<P2PSwitchHost> host = session->getHost();
			if (host.isNotNull()) {
				_send_ping_request_udp(session->m_remoteAddress, host->getHostId());
			}
		}
	}

	Ref<_Host> _registerHost(String hostId)
	{
		Ref<_Host> ret;
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return ret;
		}
		if (hostId == m_hostId) {
			return ret;
		}

		m_hosts.get(hostId, &ret);
		if (ret.isNull()) {
			ret = new _Host();
			if (ret.isNull()) {
				return ret;
			}
			ret->m_hostId = hostId;
			m_hosts.put(hostId, ret);
		}
		return ret;
	}

	Ref<_Host> _getHost(String hostId)
	{
		Ref<_Host> host;
		m_hosts.get(hostId, &host);
		return host;
	}

	Mutex m_mutexSessionId;
	sl_uint64 _generateNewSessionId()
	{
		static sl_uint64 sessionId = 0;
		MutexLocker lockSessionId(&m_mutexSessionId);
		if (sessionId == 0) {
			sessionId = Time::now().getMillisecondsCount();
		}
		sessionId++;
		return sessionId;
	}

	Ref<_DirectUdpSession> _registerDirectUdpSession(_Host* host, const SocketAddress& addr)
	{
		Ref<_DirectUdpSession> ret;
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return ret;
		}
		if (!host) {
			return ret;
		}
		{
			ListLocker< Ref<_DirectUdpSession> > sessions(host->m_sessionsDirectUdp);
			for (sl_size i = 0; i < sessions.count(); i++) {
				const Ref<_DirectUdpSession>& session = sessions[i];
				if (session.isNotNull() && session->m_remoteAddress == addr) {
					return session;
				}
			}
		}
		ret = new _DirectUdpSession;
		if (ret.isNotNull()) {
			ret->m_host = host;
			ret->m_remoteAddress = addr;
			ret->m_localSessionId = _generateNewSessionId();

			host->m_addressesUdpDirect.addIfNotExist(addr);
			host->m_sessionsDirectUdp.add(ret);
			m_sessionsDirectUdp.put(ret->m_localSessionId, ret);
		}
		return ret;
	}

	Ref<_TurningUdpSession> _getTurningUdpSession(_Host* host, String serviceId)
	{
		Ref<_TurningUdpSession> ret;
		{
			ListLocker< Ref<_TurningUdpSession> > sessions(host->m_sessionsTurningUdp);
			for (sl_size i = 0; i < sessions.count(); i++) {
				const Ref<_TurningUdpSession>& session = sessions[i];
				if (session.isNotNull() && session->m_serviceId == serviceId) {
					ret = session;
					break;
				}
			}
		}
		return ret;
	}

	Ref<_TurningUdpSession> _registerTurningUdpSession(_Host* host, HnsClient* service)
	{
		Ref<_TurningUdpSession> ret;
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return ret;
		}
		if (!host) {
			return ret;
		}
		String serviceId = service->getServiceName();
		{
			ListLocker< Ref<_TurningUdpSession> > sessions(host->m_sessionsTurningUdp);
			for (sl_size i = 0; i < sessions.count(); i++) {
				const Ref<_TurningUdpSession>& session = sessions[i];
				if (session.isNotNull() && session->m_serviceId == serviceId) {
					return session;
				}
			}
		}
		ret = new _TurningUdpSession;
		if (ret.isNotNull()) {
			ret->m_host = host;
			ret->m_serviceId = serviceId;
			host->m_sessionsTurningUdp.add(ret);
		}
		return ret;
	}

	void _send_turning_message_udp(_TurningUdpSession* session, sl_uint64 messageId, const void* data, sl_uint32 size)
	{
		if (size > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		if (!session) {
			return;
		}
		Ref<HnsClient> service = getService(session->m_serviceId);
		if (service.isNull()) {
			return;
		}
		Ref<P2PSwitchHost> host = session->getHost();
		if (host.isNull()) {
			return;
		}
		service->sendMessage(host->getHostId(), messageId, data, size);
	}

	void _send_direct_udp(const SocketAddress& addr, sl_uint32 method, char* bufPacket, sl_uint32 size)
	{
		sl_uint32 nonce = (sl_uint32)(Time::now().toInt());
		MIO::writeUint32(bufPacket + 4, (nonce << 4) | method);
		for (sl_uint32 i = 8; i < size; i++) {
			bufPacket[i] ^= bufPacket[4 + (i & 3)];
		}
		MIO::writeUint32(bufPacket, SHA256::make32bitChecksum(bufPacket + 4, size - 4));
		m_socketUdp->sendTo(bufPacket, size, addr);
	}
	
	void _send_ping_knock_udp(const SocketAddress& addr)
	{
		char c = 1;
		m_socketUdp->sendTo(&c, 1, addr);
	}

	void _send_ping_request_udp(const SocketAddress& addr, String targetHostId, sl_bool flagBroadcast = sl_false)
	{
		if (targetHostId == m_hostId) {
			return;
		}
		
		char bufPacket[256];
		MemoryWriter writer(bufPacket, 256);
		
		// Hash, MethodID & Nonce
		writer.seek(8);

		// SourceHostId
		writer.writeString(m_hostId, SLIB_P2P_HOSTID_MAX_SIZE);
		// TargetHostId
		writer.writeString(targetHostId, SLIB_P2P_HOSTID_MAX_SIZE);
		// time
		writer.writeTime(Time::now());
		
		if (!flagBroadcast) {
			SLIB_LOG("Ping", addr.toString());
		}
		_send_direct_udp(addr, 8, bufPacket, (sl_uint32)(writer.getOffset()));
		Thread::sleep(1);
	}

	void _receive_ping_request_udp(const SocketAddress& addr, char* data, sl_uint32 size)
	{
		MemoryReader reader(data, size);
		String sourceHostId;
		if (!reader.readString(&sourceHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		if (sourceHostId == m_hostId) {
			return;
		}
		String targetHostId;
		if (!reader.readString(&targetHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		if (targetHostId.isNotEmpty() && targetHostId != m_hostId) {
			return;
		}
		SLIB_LOG("RecvPingRequest", sourceHostId + ":" + addr.toString());
		
		Time timeSource;
		if (!reader.readTime(&timeSource)) {
			return;
		}

		// make host
		Ref<_Host> host = _registerHost(sourceHostId);
		if (host.isNull()) {
			return;
		}

		// make direct udp session
		Ref<_DirectUdpSession> session = _registerDirectUdpSession(host.get(), addr);
		if (session.isNull()) {
			return;
		}

		// send response
		_send_ping_response_udp(session.get(), addr, timeSource);
	}

	void _send_ping_response_udp(_DirectUdpSession* session, const SocketAddress& address, Time timeSource)
	{
		Ref<P2PSwitchHost> host = session->getHost();
		if (host.isNull()) {
			return;
		}
		char bufPacket[256];
		MemoryWriter writer(bufPacket, 256);

		// Hash,  MethodID & Nonce
		writer.seek(8);
		// SourceHostId
		writer.writeString(host->getHostId(), SLIB_P2P_HOSTID_MAX_SIZE);
		// TargetHostId
		writer.writeString(m_hostId, SLIB_P2P_HOSTID_MAX_SIZE);
		// SourceTime
		writer.writeTime(timeSource);
		// TargetTime
		writer.writeTime(Time::now());
		// SessionId
		writer.writeUint64(session->m_localSessionId);
		
		_send_direct_udp(address, 9, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_ping_response_udp(const SocketAddress& addr, char* data, sl_uint32 size)
	{
		MemoryReader reader(data, size);
		String sourceHostId;
		if (!reader.readString(&sourceHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		if (sourceHostId != m_hostId) {
			return;
		}
		String targetHostId;
		if (!reader.readString(&targetHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		SLIB_LOG("RecvPingResponse", targetHostId + ":" + addr.toString());
		Time timeSource;
		if (!reader.readTime(&timeSource)) {
			return;
		}
		Time timeTarget;
		if (!reader.readTime(&timeTarget)) {
			return;
		}
		sl_uint64 remoteSessionId;
		if (!reader.readUint64(&remoteSessionId)) {
			return;
		}

		// get host
		Ref<_Host> host = _getHost(targetHostId);
		if (host.isNull()) {
			return;
		}

		// make direct udp session
		Ref<_DirectUdpSession> session = _registerDirectUdpSession(host.get(), addr);
		if (session.isNull()) {
			return;
		}
		session->m_remoteSessionId = remoteSessionId;
		Time now = Time::now();
		session->m_timeLastCommunicated = now;
		host->m_timeLastCommunicatedDirectUdp = now;
		host->m_timeLastCommunicated = now;
	}

	sl_bool _sendMessage(String hostId, sl_uint64 messageId, const void* data, sl_uint32 n, MessageMode mode)
	{
		if (!m_flagRunning) {
			return sl_false;
		}
		if (n > SLIB_P2P_PACKET_MAX_SIZE) {
			return sl_false;
		}
		Ref<_Host> host = _getHost(hostId);
		if (host.isNull()) {
			return sl_false;
		}
		if (mode == messageModeDirectOnly || mode == messageModeDirectAndTurn || mode == messageModeAlwaysSend) {
			ListLocker< Ref<_DirectUdpSession> > sessions(host->m_sessionsDirectUdp);
			for (sl_uint32 i = 0; i < sessions.count(); i++) {
				Ref<_DirectUdpSession> session = sessions[i];
				if (session.isNotNull() && session->m_flagConnected) {
					sessions.unlock();
					_sendMessage(Ref<P2PSwitchSession>::from(session), messageId, data, n);
					return sl_true;
				}
			}
		}
		if (mode == messageModeTurnOnly || mode == messageModeDirectAndTurn) {
			sl_bool flag = sl_false;;
			ListLocker< Ref<_TurningUdpSession> > sessions(host->m_sessionsTurningUdp);
			for (sl_uint32 i = 0; i < sessions.count(); i++) {
				Ref<_TurningUdpSession> session = sessions[i];
				if (session.isNotNull() && session->m_flagConnected) {
					sessions.unlock();
					_sendMessage(Ref<P2PSwitchSession>::from(session), messageId, data, n);
					flag = sl_true;
				}
			}
			return flag;
		}
		if (mode == messageModeAlwaysSend) {
			sendMessageThroughAllServices(hostId, messageId, data, n);
			return sl_true;
		} 
		return sl_false;
	}

	void _sendMessage(Ref<P2PSwitchSession> session, sl_uint64 messageId, const void* data, sl_uint32 n)
	{
		P2PSwitchSession* s = session.getObject();
		if (!s) {
			return;
		}
		P2PSwitchSession::SessionType type = s->getType();
		if (type == P2PSwitchSession::typeDirectUdp) {
			_send_direct_message_udp((_DirectUdpSession*)s, messageId, data, n);
		} else if (type == P2PSwitchSession::typeTurningUdp) {
			_send_turning_message_udp((_TurningUdpSession*)s, messageId, data, n);
		}
	}

	void _send_direct_message_udp(_DirectUdpSession* session, sl_uint64 messageId, const void* data, sl_uint32 size)
	{
		if (size > SLIB_P2P_PACKET_MAX_SIZE) {
			return;
		}
		if (!session) {
			return;
		}
		if (session->m_remoteSessionId == 0) {
			return;
		}
		Ref<P2PSwitchHost> host = session->getHost();
		if (host.isNull()) {
			return;
		}
		char bufPacket[PACKET_SIZE];
		MemoryWriter writer(bufPacket, PACKET_SIZE);

		// Hash,  MethodID & Nonce
		writer.seek(8);

		// SessionId
		writer.writeUint64(session->m_remoteSessionId);
		// SourceHostId
		writer.writeString(m_hostId, SLIB_P2P_HOSTID_MAX_SIZE);
		// TargetHostId
		writer.writeString(host->getHostId(), SLIB_P2P_HOSTID_MAX_SIZE);
		// MessageId
		writer.writeUint64(messageId);
		// Content
		writer.write(data, size);

		_send_direct_udp(session->m_remoteAddress, 10, bufPacket, (sl_uint32)(writer.getOffset()));
	}

	void _receive_direct_message_udp(const SocketAddress& addr, char* data, sl_uint32 size)
	{
		MemoryReader reader(data, size);

		sl_uint64 sessionId;
		if (!reader.readUint64(&sessionId)) {
			return;
		}
		Ref<_DirectUdpSession> session;
		m_sessionsDirectUdp.get(sessionId, &session);
		if (session.isNull()) {
			return;
		}

		String sourceHostId;
		if (!reader.readString(&sourceHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		String targetHostId;
		if (!reader.readString(&targetHostId, SLIB_P2P_HOSTID_MAX_SIZE)) {
			return;
		}
		if (targetHostId != m_hostId) {
			return;
		}

		if (session->getHostId() != sourceHostId) {
			return;
		}

		sl_uint64 messageId;
		if (!reader.readUint64(&messageId)) {
			return;
		}
		char* p = data + reader.getOffset();
		sl_uint32 n = size - (sl_uint32)(reader.getOffset());

		Time now = Time::now();
		session->m_timeLastCommunicated = now;
		_Host* host = (_Host*)(session->getHost().getObject());
		host->m_timeLastCommunicatedDirectUdp = now;
		host->m_timeLastCommunicated = now;
		_receive_message(session.get(), messageId, p, n);
	}

	void _receive_message(P2PSwitchSession* session, sl_uint64 messageId, const void* data, sl_uint32 size)
	{
		if (messageId == 0) {
			if (size > 0) {
				sl_uint8* buf = (sl_uint8*)data;
				sl_uint8 methodId = buf[0];
				sl_uint8* content = buf + 1;
				size--;
				if (methodId == 1) {
					_receive_method_message_query_addresses_request(session);
				} else if (methodId == 2) {
					_receive_method_message_query_addresses_response(session, content, size);
				}
			}
		} else {
			PtrLocker<IP2PSwitchListener> listener(getListener());
			if (listener.isNotNull()) {
				listener->onReceiveMessage(this, session, messageId, data, size);
			}
		}
	}

	void _send_method_message_query_addresses_request(String hostId)
	{
		char buf[1] = { 1 }; // method id
		_sendMessage(hostId, 0, buf, 1, messageModeTurnOnly);
	}

	void _receive_method_message_query_addresses_request(P2PSwitchSession* session) {
		_send_method_message_query_addresses_response(session);
	}

	void _send_method_message_query_addresses_response(P2PSwitchSession* session)
	{
		char buf[SLIB_P2P_PACKET_MAX_SIZE];
		MemoryWriter writer(buf, SLIB_P2P_ADDRESS_MAX_SIZE);

		// method id
		if (!writer.writeUint8(2)) {
			return;
		}

		ListLocker<IPv4Address> addresses(NetworkAddress::findAllIPv4Addresses());
		sl_uint32 n = (sl_uint32)(addresses.count());
		if (n > 16) {
			n = 16;
		}
		// address count
		if (!writer.writeUint32CVLI(n)) {
			return;
		}
		SocketAddress address;
		address.port = m_portUdp;
		for (sl_uint32 i = 0; i < n; i++) {
			address.ip = addresses[i];
			if (!writer.writeString(address.toString())) {
				return;
			}
		}
		_sendMessage(session, 0, buf, (sl_uint32)(writer.getOffset()));
	}

	void _receive_method_message_query_addresses_response(P2PSwitchSession* session, const void* content, sl_uint32 size) {

		Ref<_Host> host = Ref<_Host>::from(session->getHost());

		MemoryReader reader(content, size);
		sl_uint32 n = 0;
		if (!reader.readUint32CVLI(&n)) {
			return;
		}
		for (sl_uint32 i = 0; i < n; i++) {
			String strAddress;
			if (!reader.readString(&strAddress)) {
				return;
			}
			SocketAddress addr;
			if (addr.parse(strAddress)) {
				host->m_addressesUdpHolePunching.addIfNotExist(addr);
			}
		}
	}

};

Ref<P2PSwitch> P2PSwitch::start(const P2PSwitchParam& param)
{
	return Ref<P2PSwitch>::from(_P2PSwitch_Impl::start(param));
}

String P2PSwitchSession::getHostId()
{
	Ref<P2PSwitchHost> host = getHost();
	if (host.isNotNull()) {
		return host->getHostId();
	}
	return String::null();
}

SLIB_P2P_NAMESPACE_END

