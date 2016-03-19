#include "../../../inc/slib/core/object.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_ROOT_OBJECT(Object)

Mutex* Object::getLocker() const
{
	return (Mutex*)(&m_locker);
}

void Object::lock() const
{
	m_locker.lock();
}

void Object::unlock() const
{
	m_locker.unlock();
}

sl_bool Object::tryLock() const
{
	return m_locker.tryLock();
}


ObjectLocker::ObjectLocker()
{
}

ObjectLocker::ObjectLocker(const Object* object) : MutexLocker(object ? object->getLocker(): sl_null)
{
}

ObjectLocker::ObjectLocker(const Object* object1, const Object* object2) : MutexLocker(object1 ? object1->getLocker() : sl_null, object2 ? object2->getLocker() : sl_null)
{
}

void ObjectLocker::lock(const Object* object)
{
	if (object) {
		MutexLocker::lock(object->getLocker());
	}
}

void ObjectLocker::lock(const Object* object1, const Object* object2)
{
	if (object1) {
		if (object2) {
			MutexLocker::lock(object1->getLocker(), object2->getLocker());
		} else {
			MutexLocker::lock(object1->getLocker());
		}
	} else {
		if (object2) {
			MutexLocker::lock(object2->getLocker());
		}
	}
}

SLIB_NAMESPACE_END
