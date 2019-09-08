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

#if defined(SLIB_PLATFORM_IS_UNIX) && !defined(SLIB_PLATFORM_IS_MOBILE)

#include "slib/core/global_unique_instance.h"

#include "slib/core/file.h"
#include "slib/core/system.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

namespace slib
{
	
	namespace priv
	{
		namespace global_unique_instance
		{
			
			class GlobalUniqueInstanceImpl : public GlobalUniqueInstance
			{
			public:
				int handle;
				String path;
				
			public:
				GlobalUniqueInstanceImpl()
				{
				}
				
				~GlobalUniqueInstanceImpl()
				{
					struct flock fl;
					Base::resetMemory(&fl, 0, sizeof(fl));
					fl.l_start = 0;
					fl.l_len = 0;
					fl.l_type = F_UNLCK;
					fl.l_whence = SEEK_SET;
					fcntl(handle, F_SETLK, &fl);
					close(handle);
					//remove(path.getData());
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
		String name = File::makeSafeFileName(_name);
		String pathRoot = System::getHomeDirectory() + "/.SLib.io/global_lock";
		if (!(File::exists(pathRoot))) {
			File::createDirectory(pathRoot);
		}
		String path = pathRoot + "/" + name;
		
		int handle = open(path.getData(), O_RDWR | O_CREAT | O_EXCL, 0644);
		if (handle == -1) {
			handle = open(path.getData(), O_RDWR);
			if (handle == -1) {
				return sl_null;
			}
		}
		if (handle != -1) {
			struct flock fl;
			Base::resetMemory(&fl, 0, sizeof(fl));
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			int ret = fcntl(handle, F_SETLK, &fl);
			if (ret >= 0) {
				Ref<GlobalUniqueInstanceImpl> instance = new GlobalUniqueInstanceImpl;
				if (instance.isNotNull()) {
					instance->handle = handle;
					instance->path = path;
					return instance;
				}
			}
			close(handle);
			//remove(path.getData());
		}
		return sl_null;
	}
	
	sl_bool GlobalUniqueInstance::exists(const String& name)
	{
		return GlobalUniqueInstance::create(name).isNull();
	}

}

#endif
