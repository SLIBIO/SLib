#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"
#include "reference.h"
#include "mutex.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Object : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(Object)
private:
	Mutex m_locker;

public:
	SLIB_INLINE Object() {}

public:
	SLIB_INLINE Mutex* getLocker() const
	{
		return (Mutex*)(&m_locker);
	}

	SLIB_INLINE void lock() const
	{
		m_locker.lock();
	}

	SLIB_INLINE void unlock() const
	{
		m_locker.unlock();
	}

	SLIB_INLINE sl_bool tryLock() const
	{
		return m_locker.tryLock();
	}

};

class SLIB_EXPORT ObjectLocker
{
public:
	MutexLocker m_locker;

public:
	SLIB_INLINE ObjectLocker(const Object* object) : m_locker(object ? object->getLocker(): sl_null)
	{
	}

	SLIB_INLINE ObjectLocker(const Object* object1, const Object* object2) : m_locker(object1 ? object1->getLocker() : sl_null, object2 ? object2->getLocker() : sl_null)
	{
	}
};

SLIB_NAMESPACE_END

#endif
