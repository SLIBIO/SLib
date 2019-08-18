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
#include "slib/core/list.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace global_unique_instance
		{
			
			typedef CList<String> GlobalUniqueInstanceList;
			
			SLIB_SAFE_STATIC_GETTER(GlobalUniqueInstanceList, GetGlobalUniqueInstanceList)
			
			class GlobalUniqueInstanceImpl : public GlobalUniqueInstance
			{
			public:
				String m_name;
				Ref<File> m_file;
				String m_filePath;
				
			public:
				GlobalUniqueInstanceImpl()
				{
				}
				
				~GlobalUniqueInstanceImpl()
				{
					m_file->unlock();
					m_file->close();
					File::deleteFile(m_filePath);
					GlobalUniqueInstanceList* list = GetGlobalUniqueInstanceList();
					if (list) {
						list->remove(m_name);
					}
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
		GlobalUniqueInstanceList* list = GetGlobalUniqueInstanceList();
		if (!list) {
			return sl_null;
		}
		if (list->indexOf(name) >= 0) {
			return sl_null;
		}
		
		Ref<GlobalUniqueInstanceImpl> instance = new GlobalUniqueInstanceImpl;
		if (instance.isNotNull()) {
			String filePath = "/tmp/.slib_global_lock_" + name;
			Ref<File> file = File::openForWrite(filePath);
			if (file.isNotNull()) {
				if (file->lock()) {
					instance->m_name = name;
					instance->m_file = file;
					instance->m_filePath = filePath;
					list->add(name);
					return instance;
				}
				file->close();
				File::deleteFile(filePath);
			}
		}
		return sl_null;
	}
	
	sl_bool GlobalUniqueInstance::exists(const String& _name)
	{
		if (_name.isEmpty()) {
			return sl_false;
		}
		String name = File::makeSafeFileName(_name);
		GlobalUniqueInstanceList* list = GetGlobalUniqueInstanceList();
		if (!list) {
			return sl_false;
		}
		if (list->indexOf(name) >= 0) {
			return sl_true;
		}
		String fileName = "/tmp/.slib_global_lock_" + name;
		if (File::exists(fileName)) {
			Ref<File> file = File::openForWrite(fileName);
			if (file.isNull()) {
				return sl_true;
			} else {
				if (file->lock()) {
					file->unlock();
				} else {
					return sl_true;
				}
				file->close();
				File::deleteFile(fileName);
			}
		}
		return sl_false;
	}

}

#endif
