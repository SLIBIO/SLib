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
