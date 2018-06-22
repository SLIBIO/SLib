/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/rw_lock.h"

#include "slib/core/base.h"

#if defined(SLIB_PLATFORM_IS_UNIX)
#include <pthread.h>
#endif

namespace slib
{

	ReadWriteLock::ReadWriteLock() noexcept
	{
		_init();
	}

	ReadWriteLock::ReadWriteLock(const ReadWriteLock& other) noexcept
	{
		_init();
	}
	
	ReadWriteLock::ReadWriteLock(ReadWriteLock&& other) noexcept
	{
		_init();
	}

	ReadWriteLock::~ReadWriteLock() noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		_free();
#endif
	}

	void ReadWriteLock::_init() noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		m_pObject = Base::createMemory(sizeof(pthread_rwlock_t));
		::pthread_rwlock_init((pthread_rwlock_t*)(m_pObject), NULL);
#else
		m_nReading = 0;
#endif
	}

	void ReadWriteLock::_free() noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		::pthread_rwlock_destroy((pthread_rwlock_t*)(m_pObject));
		Base::freeMemory(m_pObject);
		m_pObject = sl_null;
#endif
	}

	sl_bool ReadWriteLock::tryLockRead() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return sl_false;
		}
		return ::pthread_rwlock_tryrdlock((pthread_rwlock_t*)(m_pObject)) == 0;
#else
		if (m_lockReading.tryLock()) {
			m_nReading++;
			if (m_nReading == 1) {
				if (m_lockWriting.tryLock()) {
					m_lockReading.unlock();
					return sl_true;
				} else {
					m_nReading = 0;
					m_lockReading.unlock();
					return sl_false;
				}
			} else {
				m_lockReading.unlock();
				return sl_true;
			}
		} else {
			return sl_false;
		}
#endif
	}
	
	void ReadWriteLock::lockRead() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return;
		}
		::pthread_rwlock_rdlock((pthread_rwlock_t*)(m_pObject));
#else
		MutexLocker lock(&m_lockReading);
		m_nReading++;
		if (m_nReading == 1) {
			m_lockWriting.lock();
		}
#endif
	}

	void ReadWriteLock::unlockRead() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return;
		}
		pthread_rwlock_unlock((pthread_rwlock_t*)(m_pObject));
#else
		MutexLocker lock(&m_lockReading);
		m_nReading--;
		if (m_nReading == 0) {
			m_lockWriting.unlock();
		}
#endif
	}

	sl_bool ReadWriteLock::tryLockWrite() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return sl_false;
		}
		return ::pthread_rwlock_trywrlock((pthread_rwlock_t*)(m_pObject)) == 0;
#else
		return m_lockWriting.tryLock();
#endif
	}
	void ReadWriteLock::lockWrite() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return;
		}
		::pthread_rwlock_wrlock((pthread_rwlock_t*)(m_pObject));
#else
		m_lockWriting.lock();
#endif
	}

	void ReadWriteLock::unlockWrite() const noexcept
	{
#if defined(SLIB_PLATFORM_IS_UNIX)
		if (!m_pObject) {
			return;
		}
		pthread_rwlock_unlock((pthread_rwlock_t*)(m_pObject));
#else
		m_lockWriting.unlock();
#endif
	}
	
	ReadWriteLock& ReadWriteLock::operator=(const ReadWriteLock& other) noexcept
	{
		return *this;
	}
	
	ReadWriteLock& ReadWriteLock::operator=(ReadWriteLock&& other) noexcept
	{
		return *this;
	}


	ReadLocker::ReadLocker() noexcept
	{
		m_lock = sl_null;
	}

	ReadLocker::ReadLocker(const ReadWriteLock* lock) noexcept
	{
		m_lock = lock;
		if (lock) {
			lock->lockRead();
		}
	}

	ReadLocker::~ReadLocker() noexcept
	{
		unlock();
	}

	void ReadLocker::lock(const ReadWriteLock* lock) noexcept
	{
		if (m_lock) {
			return;
		}
		if (lock) {
			m_lock = lock;
			lock->lockRead();
		}
	}

	void ReadLocker::unlock() noexcept
	{
		const ReadWriteLock* lock = m_lock;
		if (lock) {
			m_lock->unlockRead();
			m_lock = sl_null;
		}
	}

	
	WriteLocker::WriteLocker() noexcept
	{
		m_lock = sl_null;
	}
	
	WriteLocker::WriteLocker(const ReadWriteLock* lock) noexcept
	{
		m_lock = lock;
		if (lock) {
			lock->lockWrite();
		}
	}
	
	WriteLocker::~WriteLocker() noexcept
	{
		unlock();
	}
	
	void WriteLocker::lock(const ReadWriteLock* lock) noexcept
	{
		if (m_lock) {
			return;
		}
		if (lock) {
			m_lock = lock;
			lock->lockWrite();
		}
	}
	
	void WriteLocker::unlock() noexcept
	{
		const ReadWriteLock* lock = m_lock;
		if (lock) {
			m_lock->unlockWrite();
			m_lock = sl_null;
		}
	}

}
