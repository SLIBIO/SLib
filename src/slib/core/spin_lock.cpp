#include "../../../inc/slib/core/spin_lock.h"

#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/system.h"

namespace slib
{

	void SpinLock::lock() const
	{
		sl_uint32 count = 0;
		while (!tryLock()) {
			System::yield(count);
			count++;
		}
	}

	sl_bool SpinLock::tryLock() const
	{
		sl_int32* p = (sl_int32*)(&m_flagLock);
		return Base::interlockedCompareExchange32(p, 1, 0);
	}

	void SpinLock::unlock() const
	{
		sl_int32* p = (sl_int32*)(&m_flagLock);
		Base::interlockedCompareExchange32(p, 0, 1);
	}

	SpinLock& SpinLock::operator=(const SpinLock& other)
	{
		return *this;
	}


	SpinLocker::SpinLocker() : m_lock(sl_null)
	{
	}

	SpinLocker::SpinLocker(const SpinLock* lock)
	{
		m_lock = lock;
		if (lock) {
			lock->lock();
		}
	}

	SpinLocker::~SpinLocker()
	{
		unlock();
	}

	void SpinLocker::lock(const SpinLock* lock)
	{
		if (! m_lock) {
			m_lock = lock;
			if (lock) {
				lock->lock();
			}
		}
	}

	void SpinLocker::unlock()
	{
		if (m_lock) {
			m_lock->unlock();
			m_lock = sl_null;
		}
	}

	template class SpinLockPool<-10>;

	template class SpinLockPool<-20>;

	template class SpinLockPool<-21>;

}
