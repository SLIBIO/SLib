#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/window.h"

#include "../../../inc/slib/core/safe_static.h"

namespace slib
{
	typedef HashMap<const void*, WeakRef<ViewInstance> > _UiViewInstanceMap;
	SLIB_SAFE_STATIC_GETTER(_UiViewInstanceMap, _UI_getViewInstances)
	
	void UIPlatform::_registerViewInstance(const void* handle, ViewInstance* instance)
	{
		_UiViewInstanceMap* map = _UI_getViewInstances();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<ViewInstance> UIPlatform::_getViewInstance(const void* handle)
	{
		_UiViewInstanceMap* map = _UI_getViewInstances();
		if (map) {
			return map->getValue(handle, WeakRef<ViewInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeViewInstance(const void* handle)
	{
		_UiViewInstanceMap* map = _UI_getViewInstances();
		if (map) {
			map->remove(handle);
		}
	}
	
	typedef HashMap<const void*, WeakRef<WindowInstance> > _UiWindowInstanceMap;
	SLIB_SAFE_STATIC_GETTER(_UiWindowInstanceMap, _UI_getWindowInstances)
	
	void UIPlatform::_registerWindowInstance(const void* handle, WindowInstance* instance)
	{
		_UiWindowInstanceMap* map = _UI_getWindowInstances();
		if (map) {
			map->put(handle, instance);
		}
	}
	
	Ref<WindowInstance> UIPlatform::_getWindowInstance(const void* handle)
	{
		_UiWindowInstanceMap* map = _UI_getWindowInstances();
		if (map) {
			return map->getValue(handle, WeakRef<WindowInstance>::null());
		}
		return sl_null;
	}
	
	void UIPlatform::_removeWindowInstance(const void* handle)
	{
		_UiWindowInstanceMap* map = _UI_getWindowInstances();
		if (map) {
			map->remove(handle);
		}
	}
}
