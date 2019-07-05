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

#include "slib/core/system.h"

#include "slib/core/file.h"
#include "slib/core/console.h"
#include "slib/core/log.h"
#include "slib/core/list.h"
#include "slib/core/safe_static.h"

namespace slib
{

	String System::getApplicationDirectory()
	{
		String path = getApplicationPath();
#if !defined(SLIB_PLATFORM_IS_ANDROID)
		path = File::getParentDirectoryPath(path);
#endif
		return path;
	}

#if !defined(SLIB_PLATFORM_IS_APPLE)
	String System::getApplicationHomeDirectory()
	{
		return getApplicationDirectory();
	}

	String System::getCachesDirectory()
	{
		String dir = getApplicationDirectory() + "/caches";
		File::createDirectory(dir);
		return dir;
	}
#endif

#if defined(SLIB_PLATFORM_IS_MOBILE)
	sl_bool System::createProcess(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
	{
		LogError("System::createProcess", "Not supported");
		return sl_false;
	}

	void System::exec(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
	{
		LogError("System::exec", "Not supported");
	}

	void System::exit(int code)
	{
		LogError("System::exit", "Not supported");
	}

	void System::setCrashHandler(SIGNAL_HANDLER handler)
	{
		LogError("System::setCrashHandler", "Not supported");
	}
#endif
	
#if !defined(SLIB_PLATFORM_IS_WIN32)
	void System::setDebugFlags()
	{
	}
#endif

	void System::yield(sl_uint32 elapsed)
	{
		if (elapsed < 16) {
			return;
		}
		if (elapsed < 32) {
			System::yield();
		} else {
			System::sleep(1);
		}
	}


	SLIB_DEFINE_OBJECT(GlobalUniqueInstance, Object)
	
	GlobalUniqueInstance::GlobalUniqueInstance()
	{	
	}

	GlobalUniqueInstance::~GlobalUniqueInstance()
	{
	}

#if defined(SLIB_PLATFORM_IS_MOBILE)
	
	namespace priv
	{
		namespace system
		{
			
			typedef CList<String> GlobalUniqueInstanceList;
			
			SLIB_SAFE_STATIC_GETTER(GlobalUniqueInstanceList, getGlobalUniqueInstanceList)
			
			class GlobalUniqueInstanceImpl : public GlobalUniqueInstance
			{
			public:
				String m_name;
				
			public:
				GlobalUniqueInstanceImpl()
				{
				}
				
				~GlobalUniqueInstanceImpl()
				{
					GlobalUniqueInstanceList* list = getGlobalUniqueInstanceList();
					if (list) {
						list->remove(m_name);
					}
				}
				
			};

		}
	}


	Ref<GlobalUniqueInstance> GlobalUniqueInstance::create(const String& _name)
	{
		String name = _name;
		if (name.isEmpty()) {
			return sl_null;
		}
		name = File::makeSafeFileName(name);
		priv::system::GlobalUniqueInstanceList* list = priv::system::getGlobalUniqueInstanceList();
		if (!list) {
			return sl_null;
		}
		if (list->indexOf(name) >= 0) {
			return sl_null;
		}
		Ref<priv::system::GlobalUniqueInstanceImpl> instance = new priv::system::GlobalUniqueInstanceImpl();
		if (instance.isNotNull()) {
			instance->m_name = name;
			list->add(name);
			return instance;
		}
		return sl_null;
	}

	sl_bool GlobalUniqueInstance::exists(const String& _name)
	{
		String name = _name;
		if (name.isEmpty()) {
			return sl_false;
		}
		name = File::makeSafeFileName(name);
		priv::system::GlobalUniqueInstanceList* list = priv::system::getGlobalUniqueInstanceList();
		if (!list) {
			return sl_false;
		}
		if (list->indexOf(name) >= 0) {
			return sl_true;
		}
		return sl_true;
	}

#endif

	void Console::println(const String& s)
	{
		SLIB_STATIC_STRING(l, "\n");
		print(s + l);
	}

#if defined(SLIB_PLATFORM_IS_MOBILE)
	String Console::readLine()
	{
		return sl_null;
	}
	
	sl_char16 Console::readChar(sl_bool flagEcho)
	{
		return 0;
	}
#endif
	
}
