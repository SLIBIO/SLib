#ifndef CHECKHEADER_SLIB_CORE_SPINLOCK
#define CHECKHEADER_SLIB_CORE_SPINLOCK

#include "definition.h"
#include "base.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT SpinLock
{
	SLIB_DECLARE_CLASS_NOCOPY(SpinLock)
public:
	SLIB_INLINE SpinLock()
	{
		m_flagLock = 0;
	}

	SLIB_INLINE sl_bool tryLock() const
	{
		return Base::interlockedCompareExchange32((sl_int32*)(&m_flagLock), 1, 0);
	}
	
	void lock() const;

	SLIB_INLINE void unlock() const
	{
		Base::interlockedCompareExchange32((sl_int32*)(&m_flagLock), 0, 1);
	}

	void yield(sl_uint32 ellapsed);

private:
	sl_int32 m_flagLock;
};

class SLIB_EXPORT SpinLocker
{
	SLIB_DECLARE_CLASS_NOCOPY(SpinLocker)
private:
	const SpinLock* m_lock;
	
public:
	SLIB_INLINE SpinLocker()
	{
		m_lock = sl_null;
	}
	
	SLIB_INLINE SpinLocker(const SpinLock* lock)
	{
		m_lock = lock;
		if (lock) {
			lock->lock();
		}
	}
	
	SLIB_INLINE void lock(const SpinLock* lock)
	{
		if (! m_lock) {			
			m_lock = lock;
			if (lock) {
				lock->lock();
			}
		}
	}
	
	SLIB_INLINE void unlock()
	{
		if (m_lock) {
			m_lock->unlock();
			m_lock = sl_null;
		}
	}
	
	SLIB_INLINE ~SpinLocker()
	{
		unlock();
	}
};

#define SLIB_SPINLOCK_POOL_SIZE 971
template <int CATEGORY>
class SLIB_EXPORT SpinLockPool
{
private:
	static sl_int32 m_locks[SLIB_SPINLOCK_POOL_SIZE];

public:
	static SpinLock* get(const void* ptr)
	{
		sl_size index = ((sl_size)(ptr)) % SLIB_SPINLOCK_POOL_SIZE;
		return (SpinLock*)((void*)(m_locks + index));
	}
};

template <int CATEGORY>
sl_int32 SpinLockPool<CATEGORY>::m_locks[SLIB_SPINLOCK_POOL_SIZE] = { 0 };

typedef SpinLockPool<-20> SpinLockPoolForList;
typedef SpinLockPool<-21> SpinLockPoolForMap;

SLIB_NAMESPACE_END

#define SLIB_STATIC_SPINLOCK(NAME) \
	static sl_int32 _static_spinlock_##NAME = 0; \
	slib::SpinLock& NAME = *((slib::SpinLock*)((void*)(&_static_spinlock_##NAME)));

#define SLIB_STATIC_SPINLOCKER(NAME) \
	SLIB_STATIC_SPINLOCK(NAME) \
	slib::SpinLocker _static_spinlocker_##NAME(&NAME);

#endif