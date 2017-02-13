#ifndef CHECKHEADER_SLIB_NETWORK_IO
#define CHECKHEADER_SLIB_NETWORK_IO

#include "definition.h"

#include "socket.h"
#include "../core/io.h"

namespace slib
{
	class SLIB_EXPORT TcpStream : public Object, public IStream, public IClosable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TcpStream();
		
		TcpStream(const Ref<Socket>& socket);
		
		~TcpStream();
		
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
		AtomicRef<Socket> m_socket;
		
	};	
}

#endif
