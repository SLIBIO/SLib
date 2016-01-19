#ifndef CHECKHEADER_SLIB_NETWORK_NETFILTER
#define CHECKHEADER_SLIB_NETWORK_NETFILTER

#include "definition.h"

#include "mac_address.h"

#include "../core/object.h"
#include "../core/pointer.h"

/*******************************************************
	NetFilter based on Linux netfilter_queue

- Requirements
You must install and link "libnfnetlink" and "libnetfilter_queue"

- Run
iptables -t raw -A PREROUTING -j NFQUEUE --queue-num 0
iptables -t raw -A OUTPUT -j NFQUEUE --queue-num 0

sudo setcap 'cap_net_admin=+iep' path/to/executable

*******************************************************/

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT NetFilterPacket
{
public:
	sl_uint8* data;
	sl_uint32 length;

	sl_uint32 deviceIn;
	sl_uint32 deviceOut;
	sl_uint32 physicalDeviceIn;
	sl_uint32 physicalDeviceOut;

	sl_uint16 queueNumber;

public:
	virtual sl_bool getSourceMacAddress(MacAddress& address) = 0;

	virtual void doAccept(void* data, sl_uint32 len) = 0;
	
	virtual void doDrop() = 0;
	
	virtual void doRepeat(void* data, sl_uint32 len) = 0;
	
	virtual void doQueue(sl_uint16 queueNumber, void* data, sl_uint32 len) = 0;
	
	virtual void doStop() = 0;
	
};

class NetFilter;
class SLIB_EXPORT INetFilterListener
{
public:
	virtual void onFilterPacket(NetFilter* filter, NetFilterPacket* packet) = 0;
	
};

class SLIB_EXPORT NetFilterParam
{
public:
	Ptr<INetFilterListener> listener;
};

class SLIB_EXPORT NetFilter : public Object
{
	SLIB_DECLARE_OBJECT(NetFilter, Object)
    
public:
	static Ref<NetFilter> create(const NetFilterParam& param);
	
public:
	virtual void release() = 0;
	
	virtual sl_bool isRunning() = 0;

	
	virtual sl_bool createQueue(sl_uint16 queueNumber) = 0;
	
	virtual void removeQueue(sl_uint16 queueNumber) = 0;
	
protected:
	void _onFilterPacket(NetFilterPacket* packet);

protected:
	Ptr<INetFilterListener> m_listener;
};

SLIB_NETWORK_NAMESPACE_END

#endif
