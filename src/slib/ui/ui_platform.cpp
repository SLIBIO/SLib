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

#include "slib/ui/core.h"

#include "slib/ui/platform.h"
#include "slib/ui/view.h"
#include "slib/ui/window.h"

#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace ui_platform
		{

			typedef CHashMap<const void*, WeakRef<ViewInstance> > ViewInstanceMap;
			SLIB_SAFE_STATIC_GETTER(ViewInstanceMap, GetViewInstanceMap)
	
			typedef CHashMap<const void*, WeakRef<WindowInstance> > WindowInstanceMap;
			SLIB_SAFE_STATIC_GETTER(WindowInstanceMap, GetWindowInstanceMap)
	
		}
	}

	using namespace priv::ui_platform;

	void UIPlatform::_registerViewInstance(const void* handle, ViewInstance* instance)
	{
		ViewInstanceMap* map = GetViewInstanceMap();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<ViewInstance> UIPlatform::_getViewInstance(const void* handle)
	{
		ViewInstanceMap* map = GetViewInstanceMap();
		if (map) {
			return map->getValue(handle, WeakRef<ViewInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeViewInstance(const void* handle)
	{
		ViewInstanceMap* map = GetViewInstanceMap();
		if (map) {
			map->remove(handle);
		}
	}
	
	void UIPlatform::_registerWindowInstance(const void* handle, WindowInstance* instance)
	{
		WindowInstanceMap* map = GetWindowInstanceMap();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<WindowInstance> UIPlatform::_getWindowInstance(const void* handle)
	{
		WindowInstanceMap* map = GetWindowInstanceMap();
		if (map) {
			return map->getValue(handle, WeakRef<WindowInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeWindowInstance(const void* handle)
	{
		WindowInstanceMap* map = GetWindowInstanceMap();
		if (map) {
			map->remove(handle);
		}
	}

}
