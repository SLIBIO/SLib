#include "ui_core_common.h"

#include "../../../inc/slib/core/safe_static.h"

namespace slib
{
	SLIB_SAFE_STATIC_GETTER(LinkedQueue< Function<void()> >, _Ui_getDispatchQueue);
	
	sl_bool _UIDispatcher::addCallback(const Function<void()>& callback)
	{
		if (callback.isNotNull()) {
			LinkedQueue< Function<void()> >* queue = _Ui_getDispatchQueue();
			if (queue) {
				return queue->push(callback);
			}
		}
		return sl_false;
	}
	
	void _UIDispatcher::processCallbacks()
	{
		LinkedQueue< Function<void()> >* queue = _Ui_getDispatchQueue();
		if (queue) {
			sl_size n = queue->getCount();
			Function<void()> callback;
			while (n > 0 && queue->pop(&callback)) {
				callback();
				n--;
			}
		}
	}
	
	typedef HashMap< sl_reg, Function<void()> > _Ui_DispatchDelayedMap;
	
	SLIB_SAFE_STATIC_GETTER(_Ui_DispatchDelayedMap, _Ui_getDispatchDelayedMap);
	sl_reg _g_ui_dispatch_delayed_last_ptr = 0;
	
	sl_bool _UIDispatcher::addDelayedCallback(const Function<void()>& callback, sl_reg& ptr)
	{
		if (callback.isNotNull()) {
			_Ui_DispatchDelayedMap* map = _Ui_getDispatchDelayedMap();
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
	
	void _UIDispatcher::processDelayedCallback(sl_reg ptr)
	{
		_Ui_DispatchDelayedMap* map = _Ui_getDispatchDelayedMap();
		if (map) {
			Function<void()> callback;
			if (map->get(ptr, &callback)) {
				callback();
			}
			map->remove(ptr);
		}
	}
	
	void _UIDispatcher::removeAllCallbacks()
	{
		LinkedQueue< Function<void()> >* queue = _Ui_getDispatchQueue();
		if (queue) {
			queue->removeAll();
		}
		_Ui_DispatchDelayedMap* map = _Ui_getDispatchDelayedMap();
		if (map) {
			map->removeAll();
		}
	}
}
