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

#include "slib/core/mutex.h"

#include "slib/core/base.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#include <windows.h>
#elif defined(SLIB_PLATFORM_IS_UNIX)
#include <pthread.h>
#endif

namespace slib
{

	Mutex::Mutex() noexcept
	 : m_object(sl_null)
	{
	}

	Mutex::Mutex(const Mutex& other) noexcept
	 : m_object(sl_null)
	{
	}
	
	Mutex::Mutex(Mutex&& other) noexcept
	 : m_object(sl_null)
	{
	}

	Mutex::~Mutex() noexcept
	{
		void* object = m_object;
		if (!object) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::DeleteCriticalSection((PCRITICAL_SECTION)object);
		Base::freeMemory(object);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_destroy((pthread_mutex_t*)(object));
		Base::freeMemory(object);
#endif
		m_object = (void*)((sl_size)1);
	}

	void* Mutex::_initObject() const noexcept
	{
		void* object = m_object;
		if (object) {
			return object;
		}
		m_lock.lock();
		object = m_object;
		if (!object) {
#if defined(SLIB_PLATFORM_IS_WINDOWS)
			object = Base::createMemory(sizeof(CRITICAL_SECTION));
#	if defined(SLIB_PLATFORM_IS_WIN32)
			::InitializeCriticalSection((PCRITICAL_SECTION)object);
#	else
			::InitializeCriticalSectionEx((PCRITICAL_SECTION)object, NULL, NULL);
#	endif
#elif defined(SLIB_PLATFORM_IS_UNIX)
			object = Base::createMemory(sizeof(pthread_mutex_t));
			pthread_mutexattr_t attr;
			::pthread_mutexattr_init(&attr);
			::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			::pthread_mutex_init((pthread_mutex_t*)(object), &attr);
			::pthread_mutexattr_destroy(&attr);
#endif
			m_object = object;
		}
		m_lock.unlock();
		return object;
	}

	SLIB_INLINE void* Mutex::_getObject() const noexcept
	{
		void* object = m_object;
		if ((sl_size)object > 1) {
			return object;
		}
		if (object) {
			return sl_null;
		}
		return _initObject();
	}
	
	void Mutex::lock() const noexcept
	{
		void* object = _getObject();
		if (!object) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::EnterCriticalSection((PCRITICAL_SECTION)object);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_lock((pthread_mutex_t*)(object));
#endif
	}

	sl_bool Mutex::tryLock() const noexcept
	{
		void* object = _getObject();
		if (!object) {
			return sl_false;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		return ::TryEnterCriticalSection((PCRITICAL_SECTION)object) != 0;
#elif defined(SLIB_PLATFORM_IS_UNIX)
		return ::pthread_mutex_trylock((pthread_mutex_t*)(object)) == 0;
#endif
	}

	void Mutex::unlock() const noexcept
	{
		void* object = _getObject();
		if (!object) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::LeaveCriticalSection((PCRITICAL_SECTION)object);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_unlock((pthread_mutex_t*)(object));
#endif
	}
	
	SpinLock* Mutex::getSpinLock() const noexcept
	{
		return const_cast<SpinLock*>(&m_lock);
	}

	Mutex& Mutex::operator=(const Mutex& other) noexcept
	{
		return *this;
	}
	
	Mutex& Mutex::operator=(Mutex&& other) noexcept
	{
		return *this;
	}

	
	MutexLocker::MutexLocker() noexcept
	{
		m_mutex = sl_null;
	}
	
	MutexLocker::MutexLocker(const Mutex* mutex) noexcept
	{
		m_mutex = mutex;
		if (mutex) {
			mutex->lock();
		}
	}
	
	MutexLocker::~MutexLocker() noexcept
	{
		unlock();
	}
	
	void MutexLocker::lock(const Mutex* mutex) noexcept
	{
		if (m_mutex) {
			return;
		}
		if (mutex) {
			m_mutex = mutex;
			mutex->lock();
		}
	}
	
	void MutexLocker::unlock() noexcept
	{
		const Mutex* mutex = m_mutex;
		if (mutex) {
			mutex->unlock();
			m_mutex = sl_null;
		}
	}


	MultipleMutexLocker::MultipleMutexLocker() noexcept
	{
		m_count = 0;
	}

	MultipleMutexLocker::MultipleMutexLocker(const Mutex* mutex) noexcept
	{
		m_count = 0;
		lock(mutex);
	}

	MultipleMutexLocker::MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2) noexcept
	{
		m_count = 0;
		lock(mutex1, mutex2);
	}

	MultipleMutexLocker::MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept
	{
		m_count = 0;
		lock(mutex1, mutex2, mutex3);
	}

	MultipleMutexLocker::MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept
	{
		m_count = 0;
		lock(mutex1, mutex2, mutex3, mutex4);
	}

	MultipleMutexLocker::MultipleMutexLocker(Mutex const* const* mutex_array, sl_size count) noexcept
	{
		m_count = 0;
		lock(mutex_array, count);
	}

	MultipleMutexLocker::~MultipleMutexLocker() noexcept
	{
		unlock();
	}

	void MultipleMutexLocker::lock(const Mutex* mutex) noexcept
	{
		if (m_count > 0) {
			return;
		}
		if (mutex) {
			m_mutex[0] = mutex;
			m_count = 1;
			mutex->lock();
		}
	}

	void MultipleMutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2) noexcept
	{
		if (m_count > 0) {
			return;
		}
		if (mutex1 == mutex2) {
			lock(mutex1);
			return;
		}
		if (mutex1) {
			if (mutex2) {
				if (mutex1 < mutex2) {
					m_mutex[0] = mutex1;
					m_mutex[1] = mutex2;
					m_count = 2;
					mutex1->lock();
					mutex2->lock();
				} else {
					m_mutex[0] = mutex2;
					m_mutex[1] = mutex1;
					m_count = 2;
					mutex2->lock();
					mutex1->lock();
				}
			} else {
				m_mutex[0] = mutex1;
				m_count = 1;
				mutex1->lock();
			}
		} else {
			if (mutex2) {
				m_mutex[0] = mutex2;
				m_count = 1;
				mutex2->lock();
			}
		}
	}

	void MultipleMutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept
	{
		Mutex const* arr[3];
		arr[0] = mutex1;
		arr[1] = mutex2;
		arr[2] = mutex3;
		lock(arr, 3);
	}

	void MultipleMutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept
	{
		Mutex const* arr[4];
		arr[0] = mutex1;
		arr[1] = mutex2;
		arr[2] = mutex3;
		arr[3] = mutex4;
		lock(arr, 4);
	}

	void MultipleMutexLocker::lock(Mutex const* const* mutex_array, sl_size count) noexcept
	{
		if (count > SLIB_MAX_LOCK_MUTEX) {
			return;
		}
		m_count = 0;
		for (sl_size i = 0; i < count; i++) {
			const Mutex* m = mutex_array[i];
			if (m) {
				sl_bool flagEqual = sl_false;
				sl_size j = 0;
				for (; j < m_count; j++) {
					if (m == m_mutex[j]) {
						flagEqual = sl_true;
						break;
					}
					if (m < m_mutex[j]) {
						break;
					}
				}
				if (!flagEqual) {
					for (sl_size k = m_count; k > j; k--) {
						m_mutex[k] = m_mutex[k - 1];
					}
					m_mutex[j] = m;
					m_count++;
				}
			}
		}
		if (m_count > 0) {
			for (sl_size i = 0; i < m_count; i++) {
				m_mutex[i]->lock();
			}
		}
	}

	void MultipleMutexLocker::unlock() noexcept
	{
		if (m_count > 0) {
			for (sl_size i = m_count; i > 0;) {
				m_mutex[--i]->unlock();
			}
			m_count = 0;
		}
	}

}
