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

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/win32_com.h"

namespace slib
{

	Memory Win32_COM::readAllBytesFromStream(IStream* stream)
	{
		Memory ret;
		HRESULT hr;

		STATSTG stat;
		Base::zeroMemory(&stat, sizeof(stat));
		hr = stream->Stat(&stat, STATFLAG_NONAME);
		if (hr == S_OK) {
			LARGE_INTEGER n;
			n.QuadPart = 0;
			stream->Seek(n, STREAM_SEEK_SET, NULL);
			ULONG size = (ULONG)(stat.cbSize.QuadPart);
			if (size > 0) {
				ret = Memory::create(size);
				if (ret.isNotNull()) {
					ULONG nRead = 0;
					hr = stream->Read(ret.getData(), size, &nRead);
					if (hr == S_OK && nRead == size) {
						return ret;
					}
				}
				ret.setNull();
			}
		}
		return ret;
	}

}

#endif
