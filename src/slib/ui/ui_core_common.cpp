/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui_core_common.h"

#include "slib/core/safe_static.h"

namespace slib
{
	SLIB_SAFE_STATIC_GETTER(LinkedQueue< Function<void()> >, _priv_Ui_getDispatchQueue);
	
	sl_bool _priv_UIDispatcher::addCallback(const Function<void()>& callback)
	{
		if (callback.isNotNull()) {
			LinkedQueue< Function<void()> >* queue = _priv_Ui_getDispatchQueue();
			if (queue) {
				return queue->push(callback);
			}
		}
		return sl_false;
	}
	
	void _priv_UIDispatcher::processCallbacks()
	{
		LinkedQueue< Function<void()> >* queue = _priv_Ui_getDispatchQueue();
		if (queue) {
			sl_size n = queue->getCount();
			Function<void()> callback;
			while (n > 0 && queue->pop(&callback)) {
				callback();
				n--;
			}
		}
	}
	
	typedef CHashMap< sl_reg, Function<void()> > _priv_Ui_DispatchDelayedMap;
	
	SLIB_SAFE_STATIC_GETTER(_priv_Ui_DispatchDelayedMap, _priv_Ui_getDispatchDelayedMap);
	sl_reg _g_ui_dispatch_delayed_last_ptr = 0;
	
	sl_bool _priv_UIDispatcher::addDelayedCallback(const Function<void()>& callback, sl_reg& ptr)
	{
		if (callback.isNotNull()) {
			_priv_Ui_DispatchDelayedMap* map = _priv_Ui_getDispatchDelayedMap();
			if (map) {
				sl_reg p = Base::interlockedIncrement(&_g_ui_dispatch_delayed_last_ptr);
				if (map->put(p, callback)) {
					ptr = p;
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	void _priv_UIDispatcher::processDelayedCallback(sl_reg ptr)
	{
		_priv_Ui_DispatchDelayedMap* map = _priv_Ui_getDispatchDelayedMap();
		if (map) {
			Function<void()> callback;
			if (map->get(ptr, &callback)) {
				callback();
			}
			map->remove(ptr);
		}
	}
	
	void _priv_UIDispatcher::removeAllCallbacks()
	{
		LinkedQueue< Function<void()> >* queue = _priv_Ui_getDispatchQueue();
		if (queue) {
			queue->removeAll();
		}
		_priv_Ui_DispatchDelayedMap* map = _priv_Ui_getDispatchDelayedMap();
		if (map) {
			map->removeAll();
		}
	}
}
