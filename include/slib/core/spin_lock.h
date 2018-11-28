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

#ifndef CHECKHEADER_SLIB_CORE_SPIN_LOCK
#define CHECKHEADER_SLIB_CORE_SPIN_LOCK

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT SpinLock
	{
	public:
		constexpr SpinLock() noexcept: m_flagLock(0) {}

		constexpr SpinLock(const SpinLock& other) noexcept: m_flagLock(0) {}

	public:
		void lock() const noexcept;

		sl_bool tryLock() const noexcept;

		void unlock() const noexcept;
	
	public:
		SpinLock& operator=(const SpinLock& other) noexcept;
	
	private:
		sl_int32 m_flagLock;

	};
	
	class SLIB_EXPORT SpinLocker
	{
	public:
		SpinLocker() noexcept;

		SpinLocker(const SpinLock* lock) noexcept;

		~SpinLocker() noexcept;

	public:
		void lock(const SpinLock* lock) noexcept;

		void unlock() noexcept;

	private:
		const SpinLock* m_lock;

	};
	
	class SLIB_EXPORT DualSpinLocker
	{
	public:
		DualSpinLocker(const SpinLock* lock1, const SpinLock* lock2) noexcept;
		
		~DualSpinLocker() noexcept;
		
	public:
		void unlock() noexcept;
		
	private:
		const SpinLock* m_lock1;
		const SpinLock* m_lock2;
		
	};
	
#define SLIB_SPINLOCK_POOL_SIZE 971
	
	template <int CATEGORY>
	class SLIB_EXPORT SpinLockPool
	{
	public:
		static SpinLock* get(const void* ptr) noexcept;

	private:
		static sl_int32 m_locks[SLIB_SPINLOCK_POOL_SIZE];

	};

}

#define SLIB_STATIC_SPINLOCK(NAME) \
	static sl_int32 _static_spinlock_##NAME = 0; \
	slib::SpinLock& NAME = *(reinterpret_cast<slib::SpinLock*>(&_static_spinlock_##NAME));

#define SLIB_STATIC_SPINLOCKER(NAME) \
	SLIB_STATIC_SPINLOCK(NAME) \
	slib::SpinLocker _static_spinlocker_##NAME(&NAME);


#include "detail/spin_lock.inc"

#endif
