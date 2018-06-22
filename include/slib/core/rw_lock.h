/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_RW_LOCK
#define CHECKHEADER_SLIB_CORE_RW_LOCK

#include "definition.h"

#include "mutex.h"

namespace slib
{
	
	class SLIB_EXPORT ReadWriteLock
	{
	public:
		ReadWriteLock() noexcept;

		ReadWriteLock(const ReadWriteLock& other) noexcept;
		
		ReadWriteLock(ReadWriteLock&& other) noexcept;

		~ReadWriteLock() noexcept;
	
	public:
		sl_bool tryLockRead() const noexcept;

		void lockRead() const noexcept;

		void unlockRead() const noexcept;

		sl_bool tryLockWrite() const noexcept;
		
		void lockWrite() const noexcept;
		
		void unlockWrite() const noexcept;
		
	public:
		ReadWriteLock& operator=(const ReadWriteLock& other) noexcept;
	
		ReadWriteLock& operator=(ReadWriteLock&& other) noexcept;
		
	private:
#if defined(SLIB_PLATFORM_IS_UNIX)
		mutable void* m_pObject;
#else
		Mutex m_lockReading;
		Mutex m_lockWriting;
		mutable sl_reg m_nReading;
#endif

	private:
		void _init() noexcept;

		void _free() noexcept;

	};
	
	class SLIB_EXPORT ReadLocker
	{
	public:
		ReadLocker() noexcept;
	
		ReadLocker(const ReadWriteLock* lock) noexcept;
		
		ReadLocker(const ReadLocker& other) = delete;
		
		ReadLocker(ReadLocker&& other) = delete;

		~ReadLocker() noexcept;

	public:
		void lock(const ReadWriteLock* rwLock) noexcept;

		void unlock() noexcept;
		
	public:
		ReadLocker& operator=(const ReadLocker& other) = delete;
		
		ReadLocker& operator=(ReadLocker&& other) = delete;

	private:
		const ReadWriteLock* m_lock;

	};
	
	class SLIB_EXPORT WriteLocker
	{
	public:
		WriteLocker() noexcept;
		
		WriteLocker(const ReadWriteLock* lock) noexcept;
		
		WriteLocker(const WriteLocker& other) = delete;
		
		WriteLocker(WriteLocker&& other) = delete;
		
		~WriteLocker() noexcept;
		
	public:
		void lock(const ReadWriteLock* rwLock) noexcept;
		
		void unlock() noexcept;
		
	public:
		WriteLocker& operator=(const WriteLocker& other) = delete;
		
		WriteLocker& operator=(WriteLocker&& other) = delete;
		
	private:
		const ReadWriteLock* m_lock;
		
	};

}

#endif
