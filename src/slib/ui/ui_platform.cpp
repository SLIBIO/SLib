#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

typedef HashMap<const void*, WeakRef<ViewInstance> > _UiViewInstanceMap;
SLIB_SAFE_STATIC_GETTER(_UiViewInstanceMap, _UI_getViewInstances);

#if defined(SLIB_PLATFORM_IS_WIN32)
#define CHECK_FREE_STATIC(v) if (!(&v)) return;
#define CHECK_FREE_STATIC_RET(v, r) if (!(&v)) return r;
#else
#define CHECK_FREE_STATIC(v)
#define CHECK_FREE_STATIC_RET(v, r)
#endif

void UIPlatform::_registerViewInstance(const void* handle, ViewInstance* instance)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC(map);
	map.put(handle, instance);
}

Ref<ViewInstance> UIPlatform::_getViewInstance(const void* handle)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC_RET(map, Ref<ViewInstance>::null());
	return map.getValue(handle, WeakRef<ViewInstance>::null());
}

void UIPlatform::_removeViewInstance(const void* handle)
{
	_UiViewInstanceMap& map = _UI_getViewInstances();
	CHECK_FREE_STATIC(map);
	map.remove(handle);
}

typedef HashMap<const void*, WeakRef<WindowInstance> > _UiWindowInstanceMap;
SLIB_SAFE_STATIC_GETTER(_UiWindowInstanceMap, _UI_getWindowInstances);

void UIPlatform::_registerWindowInstance(const void* handle, WindowInstance* instance)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC(map);
	map.put(handle, instance);
}

Ref<WindowInstance> UIPlatform::_getWindowInstance(const void* handle)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC_RET(map, Ref<WindowInstance>::null());
	return map.getValue(handle, WeakRef<WindowInstance>::null());
}

void UIPlatform::_removeWindowInstance(const void* handle)
{
	_UiWindowInstanceMap& map = _UI_getWindowInstances();
	CHECK_FREE_STATIC(map);
	map.remove(handle);
}

SLIB_UI_NAMESPACE_END

