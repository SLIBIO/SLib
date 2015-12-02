#ifndef CHECKHEADER_SLIB_CORE_MUTEX
#define CHECKHEADER_SLIB_CORE_MUTEX

#include "definition.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Mutex
{
	SLIB_DECLARE_CLASS_NOCOPY(Mutex)
public:
	Mutex();
	~Mutex();

	sl_bool tryLock() const;
	void lock() const;
	void unlock() const;

private:
	void* m_pObject;
};

#define SLIB_MAX_LOCK_MUTEX 16
class SLIB_EXPORT MutexLocker
{
	SLIB_DECLARE_CLASS_NOCOPY(MutexLocker)
private:
	sl_size m_count;
	const Mutex* m_mutex[SLIB_MAX_LOCK_MUTEX];
public:
	SLIB_INLINE MutexLocker()
	{
		init();
	}

	SLIB_INLINE ~MutexLocker()
	{
		unlock();
	}

	SLIB_INLINE MutexLocker(const Mutex* mutex)
	{
		init();
		lock(mutex);
	}
	SLIB_INLINE MutexLocker(const Mutex* mutex1, const Mutex* mutex2)
	{
		init();
		lock(mutex1, mutex2);
	}
	SLIB_INLINE MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3)
	{
		init();
		lock(mutex1, mutex2, mutex3);
	}
	SLIB_INLINE MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4)
	{
		init();
		lock(mutex1, mutex2, mutex3, mutex4);
	}
	SLIB_INLINE MutexLocker(Mutex const* const* mutex_array, sl_size count)
	{
		init();
		lock(mutex_array, count);
	}

	void lock(const Mutex* mutex);
	void lock(const Mutex* mutex1, const Mutex* mutex2);
	void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3);
	void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4);
	void lock(Mutex const* const* mutex_array, sl_size count);

	void unlock();

private:
	SLIB_INLINE void init()
	{
		m_count = 0;
	}
};

SLIB_NAMESPACE_END

#endif
