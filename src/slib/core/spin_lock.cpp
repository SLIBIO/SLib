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
#if defined(USE_CPP_ATOMIC)
		std::atomic_flag* p = (std::atomic_flag*)(&m_flagLock);
		return p->test_and_set(std::memory_order_acquire);
#else
		bool* p = (bool*)(&m_flagLock);
		// __atomic_test_and_set equals __atomic_exchange(p, true, mem_order)
		return !(__atomic_test_and_set(p, __ATOMIC_ACQUIRE));
#endif
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

	template class SpinLockPool<-10>;

	template class SpinLockPool<-20>;

	template class SpinLockPool<-21>;

}
