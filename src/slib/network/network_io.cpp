#include "../../../inc/slib/network/io.h"

SLIB_NETWORK_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TcpStream, Object)

TcpStream::TcpStream()
{
}

TcpStream::TcpStream(const Ref<Socket>& socket) : m_socket(socket)
{
}

TcpStream::~TcpStream()
{
}

Ref<Socket> TcpStream::getSocket()
{
	return m_socket;
}

void TcpStream::setSocket(const Ref<Socket>& socket)
{
	m_socket = socket;
}

void TcpStream::close()
{
	Ref<Socket> socket = m_socket;
	if (socket.isNotNull()) {
		socket->close();
		m_socket.setNull();
	}
}

sl_int32 TcpStream::read32(void* buf, sl_uint32 size)
{
	Ref<Socket> socket = m_socket;
	if (socket.isNotNull()) {
		sl_int32 n = socket->receive(buf, size);
		return n;
	}
	return -1;
}

sl_int32 TcpStream::write32(const void* buf, sl_uint32 size)
{
	Ref<Socket> socket = m_socket;
	if (socket.isNotNull()) {
		sl_int32 n = socket->send(buf, size);
		return n;
	}
	return -1;
}

SLIB_NETWORK_NAMESPACE_END
