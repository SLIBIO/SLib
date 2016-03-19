#ifndef CHECKHEADER_SLIB_NETWORK_IO
#define CHECKHEADER_SLIB_NETWORK_IO

#include "definition.h"

#include "socket.h"
#include "../core/io.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT TcpStream : public Object, public IStream, public IClosable
{
public:
	TcpStream();
	
	TcpStream(const Ref<Socket>& socket);

public:
	void setSocket(const Ref<Socket>& socket);
	
	Ref<Socket> getSocket();

	// override
	void close();

	// override
	sl_int32 read32(void* buf, sl_uint32 size);
	
	// override
	sl_int32 write32(const void* buf, sl_uint32 size);

protected:
	SafeRef<Socket> m_socket;

};

SLIB_NETWORK_NAMESPACE_END

#endif
