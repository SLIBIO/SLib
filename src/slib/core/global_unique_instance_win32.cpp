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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/core/global_unique_instance.h"

#include "slib/core/file.h"

#include <windows.h>

namespace slib
{

	namespace priv
	{
		namespace global_unique_instance
		{
			class GlobalUniqueInstanceImpl : public GlobalUniqueInstance
			{
			public:
				HANDLE m_hMutex;

			public:
				GlobalUniqueInstanceImpl()
				{
				}

				~GlobalUniqueInstanceImpl()
				{
					CloseHandle(m_hMutex);
				}
			};
		}
	}

	using namespace priv::global_unique_instance;

	Ref<GlobalUniqueInstance> GlobalUniqueInstance::create(const String& _name)
	{
		if (_name.isEmpty()) {
			return sl_null;
		}
		String16 name = "Global\\" + File::makeSafeFileName(_name);
		HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)(name.getData()));
		if (hMutex != NULL) {
			CloseHandle(hMutex);
			return sl_null;
		}
		Ref<GlobalUniqueInstanceImpl> instance = new GlobalUniqueInstanceImpl;
		if (instance.isNotNull()) {
			hMutex = CreateMutexW(NULL, FALSE, (LPCWSTR)(name.getData()));
			if (hMutex) {
				instance->m_hMutex = hMutex;
				return instance;
			}
		}
		return sl_null;
	}

	sl_bool GlobalUniqueInstance::exists(const String& _name)
	{
		if (_name.isEmpty()) {
			return sl_false;
		}
		String16 name = "Global\\" + File::makeSafeFileName(_name);
		HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)(name.getData()));
		if (hMutex != NULL) {
			CloseHandle(hMutex);
			return sl_true;
		}
		return sl_false;
	}

}

#endif
