#include "../../../inc/slib/core/atomic.h"
#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

sl_reg AtomicInt::increase()
{
	return Base::interlockedIncrement(&value);
}

sl_reg AtomicInt::decrease()
{
	return Base::interlockedDecrement(&value);
}

sl_reg AtomicInt::add(sl_reg other)
{
	return Base::interlockedAdd(&value, other);
}

sl_bool AtomicInt::waitZero(sl_int32 timeout)
{
	TimeCounter t;
	sl_uint32 count = 0;
	while (timeout < 0 || t.getEllapsedMilliseconds() < timeout) {
		if (Base::interlockedCompareExchange(&value, 0, 0)) {
			return sl_true;
		}
		System::yield(count);
		count++;
	}
	return sl_false;
}

SLIB_NAMESPACE_END
