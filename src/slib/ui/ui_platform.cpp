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

	typedef CHashMap<const void*, WeakRef<ViewInstance> > _priv_UiViewInstanceMap;
	SLIB_SAFE_STATIC_GETTER(_priv_UiViewInstanceMap, _priv_UI_getViewInstances)
	
	void UIPlatform::_registerViewInstance(const void* handle, ViewInstance* instance)
	{
		_priv_UiViewInstanceMap* map = _priv_UI_getViewInstances();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<ViewInstance> UIPlatform::_getViewInstance(const void* handle)
	{
		_priv_UiViewInstanceMap* map = _priv_UI_getViewInstances();
		if (map) {
			return map->getValue(handle, WeakRef<ViewInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeViewInstance(const void* handle)
	{
		_priv_UiViewInstanceMap* map = _priv_UI_getViewInstances();
		if (map) {
			map->remove(handle);
		}
	}
	
	typedef CHashMap<const void*, WeakRef<WindowInstance> > _priv_UiWindowInstanceMap;
	SLIB_SAFE_STATIC_GETTER(_priv_UiWindowInstanceMap, _priv_UI_getWindowInstances)
	
	void UIPlatform::_registerWindowInstance(const void* handle, WindowInstance* instance)
	{
		_priv_UiWindowInstanceMap* map = _priv_UI_getWindowInstances();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<WindowInstance> UIPlatform::_getWindowInstance(const void* handle)
	{
		_priv_UiWindowInstanceMap* map = _priv_UI_getWindowInstances();
		if (map) {
			return map->getValue(handle, WeakRef<WindowInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeWindowInstance(const void* handle)
	{
		_priv_UiWindowInstanceMap* map = _priv_UI_getWindowInstances();
		if (map) {
			map->remove(handle);
		}
	}

}
