#include "../../../inc/slib/network/io.h"

SLIB_NETWORK_NAMESPACE_BEGIN

void TcpStream::close()
{
	if (m_socket.isNotNull()) {
		m_socket->close();
		m_socket.setNull();
	}
}

sl_int32 TcpStream::read32(void* buf, sl_uint32 size)
{
	sl_int32 n = m_socket->receive(buf, size);
	return n;
}

sl_int32 TcpStream::write32(const void* buf, sl_uint32 size)
{
	sl_int32 n = m_socket->send(buf, size);
	return n;
}

SLIB_NETWORK_NAMESPACE_END
