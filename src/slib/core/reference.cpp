#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/reference.h"
#include "../../../inc/slib/core/object.h"
#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/core/system.h"

sl_bool sl_compare_class_type(sl_class_type a, sl_class_type b)
{
	return a == b;
}

/*
void sl_generate_class_type(sl_class_type& a)
{
	static slib::SpinLock lock;
	static sl_class_type lastId = 0;
	slib::SpinLocker locker(&lock);
	if (a == 0) {
		lastId++;
		a = lastId;
	}
}
*/

SLIB_NAMESPACE_BEGIN
sl_bool RefCount::waitNoRef(sl_int32 timeout)
{
	TimeCounter t;
	sl_uint32 count = 0;
	while (timeout < 0 || t.getEllapsedMilliseconds() < timeout) {
		if (Base::interlockedCompareExchange(&m_nRef, 0, 0)) {
			return sl_true;
		}
		System::yield(count);
		count++;
	}
	return sl_false;
}

#define _SIGNATURE 0x15181289

Referable::Referable()
{
#ifdef SLIB_DEBUG_REFERENCE
	m_signature = _SIGNATURE;
#endif
	m_flagNoRef = sl_false;
	m_nRefCount = 0;
	m_weak = sl_null;
	
}

Referable::~Referable()
{
	_clearWeak();
}

WeakRefObject* Referable::_getWeak()
{
	SpinLocker lock(SpinLockPoolForWeakReference::get(this));
	if (! m_weak) {
		m_weak = new WeakRefObject;
		m_weak->object = this;
		m_weak->increaseReference();
	}
	return m_weak;
}

void Referable::_clearWeak()
{
	if (m_weak) {
		SpinLocker lock(&(m_weak->lock));
		m_weak->object = sl_null;
		lock.unlock();
		m_weak->decreaseReference();
		m_weak = sl_null;
	}
}

void Referable::_free()
{
	if (m_flagNoRef) {
		return;
	}
	_clearWeak();
	delete this;
}

#ifdef SLIB_DEBUG_REFERENCE
void Referable::_checkValid()
{
	if (m_signature != _SIGNATURE) {
		SLIB_ABORT("Not-Referable object is referenced");
	}
}
#endif

sl_class_type Referable::getClassType() const
{
	return 0;
}

const char* Referable::getClassTypeName() const
{
	return "unknown";
}

sl_bool Referable::checkClassType(sl_class_type type) const
{
	return sl_false;
}

void* const _Ref_null = sl_null;

WeakRefObject::WeakRefObject()
{
}

Object::Object()
{
}

SLIB_NAMESPACE_END
