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

#ifdef SLIB_PLATFORM_IS_WIN32

#include <windows.h>

#include "slib/core/pipe.h"

namespace slib
{

	sl_bool Pipe::_open(sl_pipe& hRead, sl_pipe& hWrite)
	{
		HANDLE _hRead, _hWrite;
		BOOL ret = ::CreatePipe(&_hRead, &_hWrite, NULL, 4096);
		if (ret) {
			hRead = (sl_pipe)_hRead;
			hWrite = (sl_pipe)_hWrite;
			return sl_true;
		}
		return sl_false;
	}

	void Pipe::_close(sl_pipe handle)
	{
		::CloseHandle((HANDLE)handle);
	}

	sl_int32 Pipe::read32(void* buf, sl_uint32 size)
	{
		if (isOpened()) {
			if (size == 0) {
				return 0;
			}
			DWORD ret = 0;
			HANDLE handle = (HANDLE)m_hRead;
			if (::ReadFile(handle, buf, size, &ret, NULL)) {
				return ret;
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
			DWORD ret = 0;
			HANDLE handle = (HANDLE)m_hWrite;
			if (::WriteFile(handle, (LPVOID)buf, size, &ret, NULL)) {
				return ret;
			}
		}
		return -1;
	}

}

#endif
