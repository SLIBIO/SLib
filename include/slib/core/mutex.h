/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_MUTEX
#define CHECKHEADER_SLIB_CORE_MUTEX

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT Mutex
	{
	public:
		Mutex() noexcept;

		Mutex(const Mutex& other) noexcept;
		
		Mutex(Mutex&& other) noexcept;

		~Mutex() noexcept;
	
	public:
		sl_bool tryLock() const noexcept;

		void lock() const noexcept;

		void unlock() const noexcept;

	public:
		Mutex& operator=(const Mutex& other) noexcept;
	
		Mutex& operator=(Mutex&& other) noexcept;
		
	private:
		mutable void* m_pObject;

	private:
		void _init() noexcept;

		void _free() noexcept;

	};
	
#define SLIB_MAX_LOCK_MUTEX 16
	
	class SLIB_EXPORT MutexLocker
	{
	public:
		MutexLocker() noexcept;
	
		MutexLocker(const Mutex* mutex) noexcept;
	
		MutexLocker(const Mutex* mutex1, const Mutex* mutex2) noexcept;

		MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept;
	
		MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept;
	
		MutexLocker(Mutex const* const* mutex_array, sl_size count) noexcept;
		
		MutexLocker(const MutexLocker& other) = delete;
		
		MutexLocker(MutexLocker&& other) = delete;

		~MutexLocker() noexcept;

	public:
		void lock(const Mutex* mutex) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept;

		void lock(Mutex const* const* mutex_array, sl_size count) noexcept;
	
		void unlock() noexcept;
		
	public:
		MutexLocker& operator=(const MutexLocker& other) = delete;
		
		MutexLocker& operator=(MutexLocker&& other) = delete;

	private:
		sl_size m_count;
		const Mutex* m_mutex[SLIB_MAX_LOCK_MUTEX];

	};

}

#endif
