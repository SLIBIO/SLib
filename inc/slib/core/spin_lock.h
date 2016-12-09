#ifndef CHECKHEADER_SLIB_CORE_SPIN_LOCK
#define CHECKHEADER_SLIB_CORE_SPIN_LOCK

#include "definition.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT SpinLock
{
public:
	SpinLock();
	
	SpinLock(const SpinLock& other);
	
public:
	void lock() const;
	
	sl_bool tryLock() const;
	
	void unlock() const;

public:
	SpinLock& operator=(const SpinLock& other);

private:
	volatile mutable sl_int32 m_flagLock;
	
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
private:
	static sl_int32 m_locks[SLIB_SPINLOCK_POOL_SIZE];

public:
	static SpinLock* get(const void* ptr);
	
};

extern template class SpinLockPool<-20>;
typedef SpinLockPool<-20> SpinLockPoolForList;

extern template class SpinLockPool<-21>;
typedef SpinLockPool<-21> SpinLockPoolForMap;


template <int CATEGORY>
sl_int32 SpinLockPool<CATEGORY>::m_locks[SLIB_SPINLOCK_POOL_SIZE] = { 0 };

template <int CATEGORY>
SLIB_INLINE SpinLock* SpinLockPool<CATEGORY>::get(const void* ptr)
{
	sl_size index = ((sl_size)(ptr)) % SLIB_SPINLOCK_POOL_SIZE;
	return (SpinLock*)((void*)(m_locks + index));
}

SLIB_NAMESPACE_END

#define SLIB_STATIC_SPINLOCK(NAME) \
	static sl_int32 _static_spinlock_##NAME = 0; \
	slib::SpinLock& NAME = *((slib::SpinLock*)((void*)(&_static_spinlock_##NAME)));

#define SLIB_STATIC_SPINLOCKER(NAME) \
	SLIB_STATIC_SPINLOCK(NAME) \
	slib::SpinLocker _static_spinlocker_##NAME(&NAME);

#endif
