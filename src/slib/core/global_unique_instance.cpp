/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/global_unique_instance.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(GlobalUniqueInstance, Object)
	
	GlobalUniqueInstance::GlobalUniqueInstance()
	{
	}
	
	GlobalUniqueInstance::~GlobalUniqueInstance()
	{
	}
	
}

#if defined(SLIB_PLATFORM_IS_MOBILE)

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
				
			public:
				GlobalUniqueInstanceImpl()
				{
				}
				
				~GlobalUniqueInstanceImpl()
				{
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
		String name = _name;
		if (name.isEmpty()) {
			return sl_null;
		}
		name = File::makeSafeFileName(name);
		GlobalUniqueInstanceList* list = GetGlobalUniqueInstanceList();
		if (!list) {
			return sl_null;
		}
		if (list->indexOf(name) >= 0) {
			return sl_null;
		}
		Ref<GlobalUniqueInstanceImpl> instance = new GlobalUniqueInstanceImpl();
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
		GlobalUniqueInstanceList* list = GetGlobalUniqueInstanceList();
		if (!list) {
			return sl_false;
		}
		if (list->indexOf(name) >= 0) {
			return sl_true;
		}
		return sl_true;
	}

}

#endif
