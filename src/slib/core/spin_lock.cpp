#include "../../../inc/slib/core/spin_lock.h"

#include "../../../inc/slib/core/system.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#define USE_CPP_ATOMIC
#endif

#if defined(USE_CPP_ATOMIC)
#include <atomic>
#endif

namespace slib
{
	
	SLIB_INLINE static sl_bool _SpinLock_tryLock(const sl_int32* lock)
	{
#if defined(USE_CPP_ATOMIC)
		std::atomic_flag* p = (std::atomic_flag*)(lock);
		return p->test_and_set(std::memory_order_acquire);
#else
		bool* p = (bool*)(lock);
		// __atomic_test_and_set equals __atomic_exchange(p, true, mem_order)
		return !(__atomic_test_and_set(p, __ATOMIC_ACQUIRE));
#endif
	}

	void SpinLock::lock() const
	{
		sl_uint32 count = 0;
		while (!(_SpinLock_tryLock(&m_flagLock))) {
			System::yield(count);
			count++;
		}
	}

	sl_bool SpinLock::tryLock() const
	{
		return _SpinLock_tryLock(&m_flagLock);
	}

	void SpinLock::unlock() const
	{
#if defined(USE_CPP_ATOMIC)
		std::atomic_flag* p = (std::atomic_flag*)(&m_flagLock);
		p->clear(std::memory_order_release);
#else
		bool* p = (bool*)(&m_flagLock);
		__atomic_clear(p, __ATOMIC_RELEASE);
#endif
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

	DualSpinLocker::DualSpinLocker(const SpinLock* lock1, const SpinLock* lock2)
	{
		if (lock1 < lock2) {
			Swap(lock1, lock2);
		}
		m_lock1 = lock1;
		m_lock2 = lock2;
		if (lock1) {
			lock1->lock();
		}
		if (lock2) {
			lock2->lock();
		}
	}
	
	DualSpinLocker::~DualSpinLocker()
	{
		unlock();
	}
	
	void DualSpinLocker::unlock()
	{
		if (m_lock1) {
			m_lock1->unlock();
			m_lock1 = sl_null;
		}
		if (m_lock2) {
			m_lock2->unlock();
			m_lock2 = sl_null;
		}
	}

	template class SpinLockPool<-10>;

	template class SpinLockPool<-20>;

	template class SpinLockPool<-21>;

}
