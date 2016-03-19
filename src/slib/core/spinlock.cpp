#include "../../../inc/slib/core/spinlock.h"
#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

SpinLock::SpinLock() : m_flagLock(0)
{
}

SpinLock::SpinLock(const SpinLock& other) : m_flagLock(0)
{
}

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
	return Base::interlockedCompareExchange32((sl_int32*)(&m_flagLock), 1, 0);
}

void SpinLock::unlock() const
{
	Base::interlockedCompareExchange32((sl_int32*)(&m_flagLock), 0, 1);
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

template class SpinLockPool<-20>;

template class SpinLockPool<-21>;


SLIB_NAMESPACE_END
