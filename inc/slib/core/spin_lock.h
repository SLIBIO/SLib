#ifndef CHECKHEADER_SLIB_CORE_SPIN_LOCK
#define CHECKHEADER_SLIB_CORE_SPIN_LOCK

#include "definition.h"

namespace slib
{
	
	class SLIB_EXPORT SpinLock
	{
	public:
		constexpr SpinLock() : m_flagLock(0) {}

		constexpr SpinLock(const SpinLock& other) : m_flagLock(0) {}

	public:
		void lock() const;

		sl_bool tryLock() const;

		void unlock() const;
	
	public:
		SpinLock& operator=(const SpinLock& other);
	
	private:
		sl_int32 m_flagLock;

	};
	
	class SLIB_EXPORT SpinLocker
	{
	public:
		SpinLocker();

		SpinLocker(const SpinLock* lock);

		~SpinLocker();

	public:
		void lock(const SpinLock* lock);

		void unlock();

	private:
		const SpinLock* m_lock;

	};

#define SLIB_SPINLOCK_POOL_SIZE 971
	
	template <int CATEGORY>
	class SLIB_EXPORT SpinLockPool
	{
	public:
		static SpinLock* get(const void* ptr);

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


#include "detail/spin_lock.h"

#endif
