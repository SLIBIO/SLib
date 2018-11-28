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

#include "slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_UNIX

#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <errno.h>

#include "slib/core/pipe.h"

namespace slib
{

	sl_bool Pipe::_open(sl_pipe& hRead, sl_pipe& hWrite)
	{
		int fd[2];
		if (0 == pipe(fd)) {
			hRead = (int)(fd[0]);
			hWrite = (int)(fd[1]);
			return sl_true;
		}
		return sl_false;
	}

	void Pipe::_close(sl_pipe handle)
	{
		::close((int)handle);
	}

	sl_int32 Pipe::read32(void* buf, sl_uint32 size)
	{
		if (isOpened()) {
			if (size == 0) {
				return 0;
			}
			int fd = (int)m_hRead;
			ssize_t n = ::read(fd, buf, size);
			if (n >= 0) {
				return (sl_int32)n;
			} else {
				int err = errno;
				if (err == EAGAIN || err == EWOULDBLOCK) {
					return 0;
				}
				return -1;
			}
		}
		return -1;
	}

	sl_int32 Pipe::write32(const void* buf, sl_uint32 size)
	{
		if (isOpened()) {
			if (size == 0) {
				return 0;
			}
			int fd = (int)m_hWrite;
			ssize_t n = ::write(fd, buf, size);
			if (n >= 0) {
				return (sl_int32)n;
			} else {
				int err = errno;
				if (err == EAGAIN || err == EWOULDBLOCK) {
					return 0;
				}
				return -1;
			}
		}
		return -1;
	}

}

#endif
