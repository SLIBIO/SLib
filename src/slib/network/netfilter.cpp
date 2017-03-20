/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/network/netfilter.h"

#define TAG "NetFilter"
#define SIZE_PACKET 0x10000

namespace slib
{

	NetFilterPacket::NetFilterPacket(): data(sl_null), length(0), deviceIn(0), deviceOut(0), physicalDeviceIn(0), physicalDeviceOut(0), queueNumber(0)
	{
	}

	NetFilterPacket::~NetFilterPacket()
	{
	}


	INetFilterListener::INetFilterListener()
	{
	}

	INetFilterListener::~INetFilterListener()
	{
	}


	NetFilterParam::NetFilterParam()
	{
		flagAutoStart = sl_true;
	}

	NetFilterParam::~NetFilterParam()
	{
	}

	SLIB_DEFINE_OBJECT(NetFilter, Object)

	NetFilter::NetFilter()
	{
	}

	NetFilter::~NetFilter()
	{
	}

	void NetFilter::_onFilterPacket(NetFilterPacket* packet)
	{
		PtrLocker<INetFilterListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onFilterPacket(this, packet);
		}
		m_onFilterPacket(this, packet);
	}
}

#if defined(SLIB_PLATFORM_IS_LINUX)

#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/pipe.h"
#include "../../../inc/slib/core/log.h"

#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <poll.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../../../inc/thirdparty/netfilter/libnetfilter_queue.h"

namespace slib
{
	
	class _Linux_NetFilterPacket : public NetFilterPacket
	{
	public:
		nfq_q_handle* queue;
		nfq_data* nfad;
		sl_uint32 id;
		sl_bool flagVerdict;
		
		_Linux_NetFilterPacket(nfq_q_handle* _queue, nfq_data* _data, sl_uint32 _id): queue(_queue), nfad(_data), id(_id)
		{
			flagVerdict = sl_false;
		}

		sl_bool getSourceMacAddress(MacAddress& address)
		{
			nfqnl_msg_packet_hw* hw = nfq_get_packet_hw(nfad);
			if (hw) {
				int len = ntohs(hw->hw_addrlen);
				if (len == 6) {
					Base::copyMemory(address.m, hw->hw_addr, 6);
					return sl_true;
				}
			}
			return sl_false;
		}
		
		void doAccept(void* _data, sl_uint32 _len)
		{
			nfq_set_verdict(queue, id, NF_ACCEPT, _len, (sl_uint8*)_data);
			flagVerdict = sl_true;
		}
		
		void doDrop()
		{
			nfq_set_verdict(queue, id, NF_DROP, length, data);
			flagVerdict = sl_true;
		}

		void doRepeat(void* _data, sl_uint32 _len)
		{
			nfq_set_verdict(queue, id, NF_REPEAT, _len, (sl_uint8*)_data);
			flagVerdict = sl_true;
		}

		void doQueue(sl_uint16 queueNumber, void* _data, sl_uint32 _len)
		{
			nfq_set_verdict(queue, id, NF_QUEUE | (((sl_uint32)queueNumber) << 16), _len, (sl_uint8*)_data);
			flagVerdict = sl_true;
		}

		void doStop()
		{
			nfq_set_verdict(queue, id, NF_STOP, length, data);
			flagVerdict = sl_true;
		}
	};

	class _Linux_NetFilter;
	class _Linux_NetFilterQueue : public Referable
	{
	public:
		_Linux_NetFilter* m_filter;
		nfq_q_handle* m_queue;
		sl_uint16 m_queueNumber;

		_Linux_NetFilterQueue()
		{
			m_queueNumber = 0;
			m_filter = sl_null;
			m_queue = sl_null;
		}

		~_Linux_NetFilterQueue()
		{
			if (m_queue) {
				nfq_destroy_queue(m_queue);
			}
		}
	};

	class _Linux_NetFilter : public NetFilter
	{
	public:
		nfq_handle* m_handle;
		
		Memory m_bufPacket;	
		Ref<Thread> m_thread;

		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		Ref<PipeEvent> m_wake;

		HashMap< sl_uint16, Ref<_Linux_NetFilterQueue> > m_queues;
		LinkedQueue< Ref<_Linux_NetFilterQueue> > m_listRemovingQueues;
		
	public:
		_Linux_NetFilter()
		{	
			m_handle = sl_null;

			m_flagInit = sl_false;
			m_flagRunning = sl_false;
		}
		
		~_Linux_NetFilter()
		{
			release();
		}
		
	public:
		static int _callbackFilter(nfq_q_handle* handle, nfgenmsg* msg, nfq_data* nfad, void* data)
		{
			_Linux_NetFilterQueue* queue = (_Linux_NetFilterQueue*)data;
			return queue->m_filter->_filter(queue, nfad);
		}
		
		static Ref<_Linux_NetFilter> create(const NetFilterParam& param)
		{
			Memory bufPacket = Memory::create(SIZE_PACKET);
			if (bufPacket.isEmpty()) {
				return sl_null;
			}
			Ref<PipeEvent> wake = PipeEvent::create();
			if (wake.isNull()) {
				return sl_null;
			}
			
			nfq_handle* handle = nfq_open();
			if (handle) {
				nfq_unbind_pf(handle, AF_INET);
				int iRet = nfq_bind_pf(handle, AF_INET);
				if (iRet >= 0) {
					Ref<_Linux_NetFilter> ret = new _Linux_NetFilter;
					if (ret.isNotNull()) {
						ret->m_handle = handle;
						ret->m_bufPacket = bufPacket;
						ret->m_wake = wake;
						ret->m_listener = param.listener;
						ret->m_onFilterPacket = param.onFilterPacket;
						ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(_Linux_NetFilter, _run, ret.get()));
						if (ret->m_thread.isNotNull()) {
							ret->m_flagInit = sl_true;
							if (param.flagAutoStart) {
								ret->start();
							}
							return ret;
						} else {
							LogError(TAG, "Failed to create thread");
						}
					}
				} else {
					LogError(TAG, "Failed to bind netfilter handle");
				}
				nfq_close(handle);
			}
			return sl_null;
		}
		
		void release()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			m_flagInit = sl_false;
			
			m_flagRunning = sl_false;
			if (m_wake.isNotNull()) {
				m_wake->set();
			}
			if (m_thread.isNotNull()) {
				m_thread->finishAndWait();
				m_thread.setNull();
			}

			m_queues.removeAll();
			
			nfq_unbind_pf(m_handle, AF_INET);
			nfq_close(m_handle);
		}

		void start()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			if (m_flagRunning) {
				return;
			}
			if (m_thread.isNotNull()) {
				if (m_thread->start()) {
					m_flagRunning = sl_true;
				}
			}
		}
		
		sl_bool isRunning()
		{
			return m_flagRunning;
		}

		sl_bool createQueue(sl_uint16 queueNumber)
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return sl_false;
			}
			if (m_queues.contains(queueNumber)) {
				return sl_true;
			}
			Ref<_Linux_NetFilterQueue> queueObj = new _Linux_NetFilterQueue;
			if (queueObj.isNotNull()) {
				nfq_q_handle* queue = nfq_create_queue(m_handle, queueNumber, &_Linux_NetFilter::_callbackFilter, queueObj.get());
				if (queue) {
					nfq_set_queue_maxlen(queue, 1024*10);
					queueObj->m_filter = this;
					queueObj->m_queue = queue;
					queueObj->m_queueNumber = queueNumber;
					int iRet = nfq_set_mode(queue, NFQNL_COPY_PACKET, SIZE_PACKET);
					if (iRet >= 0) {
						m_queues.put(queueNumber, queueObj);
						return sl_true;
					} else {
						LogError(TAG, "Failed to set queue mode");
					}
					nfq_destroy_queue(queue);
				} else {
					LogError(TAG, "Failed to create queue - number=%d", queueNumber);
				}
			}
			return sl_false;
		}

		void removeQueue(sl_uint16 queueNumber)
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			Ref<_Linux_NetFilterQueue> queueObj;
			if (m_queues.get(queueNumber, &queueObj)) {
				if (queueObj.isNotNull()) {
					m_listRemovingQueues.push(queueObj);
					m_wake->set();
				}
				m_queues.remove(queueNumber);
			}
		}
		
		int _filter(_Linux_NetFilterQueue* queue, nfq_data* data)
		{
			nfqnl_msg_packet_hdr* hdr = nfq_get_msg_packet_hdr(data);
			int id = ntohl(hdr->packet_id);
			sl_uint8* payload;
			int lenPayload = nfq_get_payload(data, &payload);
			if (lenPayload > 0) {
				_Linux_NetFilterPacket packet(queue->m_queue, data, id);
				packet.data = payload;
				packet.length = lenPayload;
				packet.queueNumber = queue->m_queueNumber;
				packet.deviceIn = nfq_get_indev(data);
				packet.deviceOut = nfq_get_outdev(data);
				packet.physicalDeviceIn = nfq_get_physindev(data);
				packet.physicalDeviceOut = nfq_get_physoutdev(data);
				_onFilterPacket(&packet);
				if (!(packet.flagVerdict)) {
					nfq_set_verdict(queue->m_queue, id, NF_ACCEPT, lenPayload, payload);
				}
			}
			return 0;
		}
		
		void _run()
		{		
			int fd = nfq_fd(m_handle);
			
			if (m_wake.isNull()) {
				return;
			}
			
			void* buf = m_bufPacket.getData();
			sl_uint32 sizeBuf = (sl_uint32)(m_bufPacket.getSize());
			
			nfnl_rcvbufsiz(nfq_nfnlh(m_handle), 1024*1024*100);
#if defined SOL_NETLINK && defined NETLINK_NO_ENOBUFS
			int v = 1;
			setsockopt(fd, SOL_NETLINK, NETLINK_NO_ENOBUFS, &v, sizeof(int));
#endif
		
			pollfd fds[2];
			Base::zeroMemory(fds, sizeof(pollfd)*2);
			fds[0].fd = fd;
			fds[0].events = POLLIN | POLLPRI | POLLERR | POLLHUP;
			fds[1].fd = (int)(m_wake->getReadPipeHandle());
			fds[1].events = POLLIN | POLLPRI | POLLERR | POLLHUP;
			
			while (Thread::isNotStoppingCurrent() && m_flagRunning) {
				ssize_t n = recv(fd, buf, sizeBuf, MSG_DONTWAIT);
				if (n > 0) {
					nfq_handle_packet(m_handle, (char*)buf, (int)n);
				} else if (n < 0) {
					int err = errno;
					if (err == EAGAIN || err == EWOULDBLOCK) {
						poll(fds, 2, -1);
					} else if (err == ENOBUFS) {
						LogError(TAG, "Not enough receive buffer");
					} else {
						break;
					}
				}
				m_listRemovingQueues.removeAll();
			}
		}
	};

	Ref<NetFilter> NetFilter::create(const NetFilterParam& param)
	{
		return _Linux_NetFilter::create(param);
	}

}

#else

namespace slib
{

	Ref<NetFilter> NetFilter::create(const NetFilterParam& param)
	{
		return sl_null;
	}

}

#endif
