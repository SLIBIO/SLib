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
		
		void close() override;
		
		sl_int32 read32(void* buf, sl_uint32 size) override;
		
		sl_int32 write32(const void* buf, sl_uint32 size) override;
		
	protected:
		AtomicRef<Socket> m_socket;
		
	};

}

#endif
