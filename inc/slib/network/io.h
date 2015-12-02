#ifndef CHECKHEADER_SLIB_NETWORK_IO
#define CHECKHEADER_SLIB_NETWORK_IO

#include "definition.h"
#include "socket.h"
#include "../core/io.h"

SLIB_NETWORK_NAMESPACE_BEGIN
class SLIB_EXPORT TcpStream : public Object, public IStream, public IClosable
{
public:
	SLIB_INLINE TcpStream()
	{
	}

	SLIB_INLINE TcpStream(const Ref<Socket>& socket)
	{
		m_socket = socket;
	}

	SLIB_INLINE void setSocket(const Ref<Socket>& socket)
	{
		m_socket = socket;
	}
	SLIB_INLINE Ref<Socket> getSocket()
	{
		return m_socket;
	}

	void close();

	sl_int32 read32(void* buf, sl_uint32 size);
	sl_int32 write32(const void* buf, sl_uint32 size);

protected:
	Ref<Socket> m_socket;

};

SLIB_NETWORK_NAMESPACE_END

#endif
