#include "../../../inc/slib/core/base.h"
#include "../../../inc/slib/core/reference.h"
#include "../../../inc/slib/core/object.h"

sl_bool sl_compare_class_type(sl_class_type a, sl_class_type b)
{
	return a == b;
}

SLIB_NAMESPACE_BEGIN

#define _SIGNATURE 0x15181289

Referable::Referable()
{
#ifdef SLIB_DEBUG_REFERENCE
	m_signature = _SIGNATURE;
#endif
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

SLIB_NAMESPACE_END
