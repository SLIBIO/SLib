#ifndef CHECKHEADER_SLIB_CORE_OBJECT
#define CHECKHEADER_SLIB_CORE_OBJECT

#include "definition.h"
#include "reference.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Object : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(Object)
private:
	Mutex m_locker;

public:
	Object();

	SLIB_INLINE const Mutex* getLocker() const
	{
		return &m_locker;
	}

	SLIB_INLINE Mutex* getLocker()
	{
		return &m_locker;
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
SLIB_NAMESPACE_END

#endif// CHECK_HEADER_SLIB_CORE_OBJECT
