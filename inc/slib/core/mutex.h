#ifndef CHECKHEADER_SLIB_CORE_MUTEX
#define CHECKHEADER_SLIB_CORE_MUTEX

#include "definition.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Mutex
{
public:
	Mutex();
	
	Mutex(const Mutex& other);
	
	~Mutex();

public:
	sl_bool tryLock() const;
	
	void lock() const;
	
	void unlock() const;
	
public:
	Mutex& operator=(const Mutex& other);

private:
	mutable void* m_pObject;
	
private:
	void _init();
	
	void _free();
	
};

#define SLIB_MAX_LOCK_MUTEX 16

class SLIB_EXPORT MutexLocker
{
public:
	MutexLocker();

	~MutexLocker();

	MutexLocker(const Mutex* mutex);

	MutexLocker(const Mutex* mutex1, const Mutex* mutex2);
	
	MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3);

	MutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4);

	MutexLocker(Mutex const* const* mutex_array, sl_size count);

public:
	void lock(const Mutex* mutex);
	
	void lock(const Mutex* mutex1, const Mutex* mutex2);
	
	void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3);
	
	void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4);
	
	void lock(Mutex const* const* mutex_array, sl_size count);

	
	void unlock();
	
private:
	sl_size m_count;
	const Mutex* m_mutex[SLIB_MAX_LOCK_MUTEX];
	
};

SLIB_NAMESPACE_END

#endif
