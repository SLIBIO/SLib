#ifndef CHECKHEADER_SLIB_NETWORK_EVENT
#define CHECKHEADER_SLIB_NETWORK_EVENT

#include "definition.h"

#include "socket.h"
#include "../core/event.h"

SLIB_NETWORK_NAMESPACE_BEGIN

enum SocketEventType
{
    socketEventType_Read = 1,		// receive, receiveFrom, accept
    socketEventType_Write = 2,		// send, sendTo, connect
    socketEventType_Close = 4		// close, error
};

class SLIB_EXPORT SocketEvent : public Event
{
	SLIB_DECLARE_OBJECT(SocketEvent, Event)
	
public:
	static Ref<SocketEvent> create(const Ref<Socket>& socket);
	
	static Ref<SocketEvent> create(const Ref<Socket>& socket, sl_uint32 events);
	
	static Ref<SocketEvent> createRead(const Ref<Socket>& socket);
	
	static Ref<SocketEvent> createWrite(const Ref<Socket>& socket);
	
	static Ref<SocketEvent> createReadWrite(const Ref<Socket>& socket);
	
public:	
	sl_bool setup(sl_uint32 events);
	
	SLIB_INLINE sl_bool setupRead()
	{
		return setup(socketEventType_Read | socketEventType_Close);
	}
	
	SLIB_INLINE sl_bool setupWrite()
	{
		return setup(socketEventType_Write | socketEventType_Close);
	}
	
	SLIB_INLINE sl_bool setupReadWrite()
	{
		return setup(socketEventType_Read | socketEventType_Write | socketEventType_Close);
	}

	sl_uint32 waitEvents(sl_int32 timeout = -1);

	// count must be less than 64
	static sl_bool waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout = -1);

	SLIB_INLINE const Ref<Socket>& getSocket()
	{
		return m_socket;
	}

protected:
	sl_bool __wait(sl_int32 timeout);

	virtual sl_bool __setup(sl_uint32 events) = 0;
	
	static sl_bool __waitMultipleEvents(const Ref<SocketEvent>* events, sl_uint32* status, sl_uint32 count, sl_int32 timeout);

protected:
	Ref<Socket> m_socket;

};

SLIB_NETWORK_NAMESPACE_END

#endif
