/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/network/io.h"

namespace slib
{

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

}
