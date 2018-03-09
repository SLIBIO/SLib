/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
