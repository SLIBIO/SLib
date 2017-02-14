#ifndef CHECKHEADER_SLIB_CORE_DETAIL_SPIN_LOCK
#define CHECKHEADER_SLIB_CORE_DETAIL_SPIN_LOCK

#include "../spin_lock.h"

namespace slib
{

	extern template class SpinLockPool<-10>;
	typedef SpinLockPool<-10> SpinLockPoolForBase;
	
	extern template class SpinLockPool<-20>;
	typedef SpinLockPool<-20> SpinLockPoolForList;
	
	extern template class SpinLockPool<-21>;
	typedef SpinLockPool<-21> SpinLockPoolForMap;
	
	template <int CATEGORY>
	sl_int32 SpinLockPool<CATEGORY>::m_locks[SLIB_SPINLOCK_POOL_SIZE] = { 0 };
	
	template <int CATEGORY>
	SLIB_INLINE SpinLock* SpinLockPool<CATEGORY>::get(const void* ptr)
	{
		sl_size index = ((sl_size)(ptr)) % SLIB_SPINLOCK_POOL_SIZE;
		return reinterpret_cast<SpinLock*>(m_locks + index);
	}

}

#endif
