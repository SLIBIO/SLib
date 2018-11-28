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
