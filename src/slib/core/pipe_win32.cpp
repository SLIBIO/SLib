/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
