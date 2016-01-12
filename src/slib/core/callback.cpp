#include "../../../inc/slib/core/callback.h"

SLIB_NAMESPACE_BEGIN

Ref<Runnable> _Runnable_createCallback(void(*callback)())
{
	return new _CallbackRunnable(callback);
}

SLIB_NAMESPACE_END
