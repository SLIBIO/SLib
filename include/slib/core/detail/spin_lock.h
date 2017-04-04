/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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
