/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	{
		_init();
	}

	Mutex::Mutex(const Mutex& other) noexcept
	{
		_init();
	}
	
	Mutex::Mutex(Mutex&& other) noexcept
	{
		_init();
	}

	Mutex::~Mutex() noexcept
	{
		_free();
	}

	void Mutex::_init() noexcept
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		m_pObject = Base::createMemory(sizeof(CRITICAL_SECTION));
#	if defined(SLIB_PLATFORM_IS_WIN32)
		::InitializeCriticalSection((PCRITICAL_SECTION)m_pObject);
#	else
		::InitializeCriticalSectionEx((PCRITICAL_SECTION)m_pObject, NULL, NULL);
#	endif
#elif defined(SLIB_PLATFORM_IS_UNIX)
		m_pObject = Base::createMemory(sizeof(pthread_mutex_t));
		pthread_mutexattr_t attr;
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		::pthread_mutex_init((pthread_mutex_t*)(m_pObject), &attr);
		::pthread_mutexattr_destroy(&attr);
#endif
	}

	void Mutex::_free() noexcept
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::DeleteCriticalSection((PCRITICAL_SECTION)m_pObject);
		Base::freeMemory(m_pObject);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_destroy((pthread_mutex_t*)(m_pObject));
		Base::freeMemory(m_pObject);
#endif
		m_pObject = sl_null;
	}

	void Mutex::lock() const noexcept
	{
		if (!m_pObject) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::EnterCriticalSection((PCRITICAL_SECTION)m_pObject);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_lock((pthread_mutex_t*)(m_pObject));
#endif
	}

	sl_bool Mutex::tryLock() const noexcept
	{
		if (!m_pObject) {
			return sl_false;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		return ::TryEnterCriticalSection((PCRITICAL_SECTION)m_pObject) != 0;
#elif defined(SLIB_PLATFORM_IS_UNIX)
		return ::pthread_mutex_trylock((pthread_mutex_t*)(m_pObject)) == 0;
#endif
	}

	void Mutex::unlock() const noexcept
	{
		if (!m_pObject) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::LeaveCriticalSection((PCRITICAL_SECTION)m_pObject);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_mutex_unlock((pthread_mutex_t*)(m_pObject));
#endif
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
