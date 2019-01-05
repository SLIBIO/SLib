/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/spin_lock.h"

#include "slib/core/system.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#define USE_CPP_ATOMIC
#endif

#if defined(USE_CPP_ATOMIC)
#include <atomic>
#endif

namespace slib
{
	
	SLIB_INLINE static sl_bool _priv_SpinLock_tryLock(const sl_int32* lock)
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

	void SpinLock::lock() const noexcept
	{
		sl_uint32 count = 0;
		while (!(_priv_SpinLock_tryLock(&m_flagLock))) {
			System::yield(count);
			count++;
		}
	}

	sl_bool SpinLock::tryLock() const noexcept
	{
		return _priv_SpinLock_tryLock(&m_flagLock);
	}

	void SpinLock::unlock() const noexcept
	{
#if defined(USE_CPP_ATOMIC)
		std::atomic_flag* p = (std::atomic_flag*)(&m_flagLock);
		p->clear(std::memory_order_release);
#else
		bool* p = (bool*)(&m_flagLock);
		__atomic_clear(p, __ATOMIC_RELEASE);
#endif
	}

	SpinLock& SpinLock::operator=(const SpinLock& other) noexcept
	{
		return *this;
	}


	SpinLocker::SpinLocker() noexcept: m_lock(sl_null)
	{
	}

	SpinLocker::SpinLocker(const SpinLock* lock) noexcept
	{
		m_lock = lock;
		if (lock) {
			lock->lock();
		}
	}
	
	SpinLocker::~SpinLocker() noexcept
	{
		unlock();
	}

	void SpinLocker::lock(const SpinLock* lock) noexcept
	{
		if (! m_lock) {
			m_lock = lock;
			if (lock) {
				lock->lock();
			}
		}
	}

	void SpinLocker::unlock() noexcept
	{
		if (m_lock) {
			m_lock->unlock();
			m_lock = sl_null;
		}
	}

	DualSpinLocker::DualSpinLocker(const SpinLock* lock1, const SpinLock* lock2) noexcept
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
	
	DualSpinLocker::~DualSpinLocker() noexcept
	{
		unlock();
	}
	
	void DualSpinLocker::unlock() noexcept
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

	template class SpinLockPool<-11>;

	template class SpinLockPool<-20>;

	template class SpinLockPool<-21>;

	template class SpinLockPool<-30>;

}
