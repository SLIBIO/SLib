/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/mutex.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#include <windows.h>
#elif defined(SLIB_PLATFORM_IS_UNIX)
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#endif

namespace slib
{

	Mutex::Mutex()
	{
		_init();
	}

	Mutex::Mutex(const Mutex& other)
	{
		_init();
	}

	Mutex::~Mutex()
	{
		_free();
	}

	void Mutex::_init()
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		m_pObject = Base::createMemory(sizeof(CRITICAL_SECTION));
#	if defined(SLIB_PLATFORM_IS_DESKTOP)
		InitializeCriticalSection((PCRITICAL_SECTION)m_pObject);
#	elif defined(SLIB_PLATFORM_IS_MOBILE)
		InitializeCriticalSectionEx((PCRITICAL_SECTION)m_pObject, NULL, NULL);
#	endif
#elif defined(SLIB_PLATFORM_IS_UNIX)
		m_pObject = Base::createMemory(sizeof(pthread_mutex_t));
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init((pthread_mutex_t*)(m_pObject), &attr);
		pthread_mutexattr_destroy(&attr);
#endif
	}

	void Mutex::_free()
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		DeleteCriticalSection((PCRITICAL_SECTION)m_pObject);
		Base::freeMemory(m_pObject);
		m_pObject = sl_null;
#elif defined(SLIB_PLATFORM_IS_UNIX)
		pthread_mutex_destroy((pthread_mutex_t*)(m_pObject));
		Base::freeMemory(m_pObject);
#endif
	}

	void Mutex::lock() const
	{
		if (!m_pObject) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		EnterCriticalSection((PCRITICAL_SECTION)m_pObject);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		pthread_mutex_lock((pthread_mutex_t*)(m_pObject));
#endif
	}

	sl_bool Mutex::tryLock() const
	{
		if (!m_pObject) {
			return sl_false;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		return TryEnterCriticalSection((PCRITICAL_SECTION)m_pObject) != 0;
#elif defined(SLIB_PLATFORM_IS_UNIX)
		return pthread_mutex_trylock((pthread_mutex_t*)(m_pObject)) == 0;
#endif
	}

	void Mutex::unlock() const
	{
		if (!m_pObject) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		LeaveCriticalSection((PCRITICAL_SECTION)m_pObject);
#elif defined(SLIB_PLATFORM_IS_UNIX)
		pthread_mutex_unlock((pthread_mutex_t*)(m_pObject));
#endif
	}

	Mutex& Mutex::operator=(const Mutex& other)
	{
		return *this;
	}



	MutexLocker::MutexLocker()
	{
		m_count = 0;
	}

	MutexLocker::MutexLocker(const Mutex* mutex)
	{
		m_count = 0;
		lock(mutex);
	}

	MutexLocker::MutexLocker(const Mutex* mutex1, const Mutex* mutex2)
	{
		m_count = 0;
		lock(mutex1, mutex2);
	}

	MutexLocker::MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3)
	{
		m_count = 0;
		lock(mutex1, mutex2, mutex3);
	}

	MutexLocker::MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4)
	{
		m_count = 0;
		lock(mutex1, mutex2, mutex3, mutex4);
	}

	MutexLocker::MutexLocker(Mutex const* const* mutex_array, sl_size count)
	{
		m_count = 0;
		lock(mutex_array, count);
	}

	MutexLocker::~MutexLocker()
	{
		unlock();
	}

	void MutexLocker::lock(const Mutex* mutex)
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

	void MutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2)
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

	void MutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3)
	{
		Mutex const* arr[3];
		arr[0] = mutex1;
		arr[1] = mutex2;
		arr[2] = mutex3;
		lock(arr, 3);
	}

	void MutexLocker::lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4)
	{
		Mutex const* arr[4];
		arr[0] = mutex1;
		arr[1] = mutex2;
		arr[2] = mutex3;
		arr[3] = mutex4;
		lock(arr, 4);
	}

	void MutexLocker::lock(Mutex const* const* mutex_array, sl_size count)
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

	void MutexLocker::unlock()
	{
		if (m_count > 0) {
			for (sl_size i = m_count; i > 0;) {
				m_mutex[--i]->unlock();
			}
			m_count = 0;
		}
	}

}
