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

		if (ret.isEmpty()) {
			hr = stream->Stat(&stat, STATFLAG_NONAME);
			if (hr == S_OK) {
				LARGE_INTEGER n;
				n.QuadPart = 0;
				stream->Seek(n, STREAM_SEEK_SET, NULL);
				ULONG size = (ULONG)(stat.cbSize.QuadPart);
				if (size > 0) {
					ret = Memory::create(size);
					if (ret.isNotEmpty()) {
						ULONG nRead = 0;
						hr = stream->Read(ret.getData(), size, &nRead);
						if (hr == S_OK && nRead == size) {
							return ret;
						}
					}
					ret.setNull();
				}
			}
		}
		return ret;
	}

}

#endif
