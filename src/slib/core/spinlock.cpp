#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/spinlock.h"

SLIB_NAMESPACE_BEGIN
void SpinLock::lock() const
{
	sl_uint32 count = 0;
	while (!tryLock()) {
		System::yield(count);
		count++;
	}
}
SLIB_NAMESPACE_END
