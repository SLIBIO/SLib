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

CWeakRef* Referable::_getWeak()
{
	SpinLocker lock(&m_lockWeak);
	if (! m_weak) {
		m_weak = CWeakRef::create(this);
	}
	return m_weak;
}

void Referable::_clearWeak()
{
	if (m_weak) {
		m_weak->release();
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

const _Ref_Const _Ref_Null = {0, 0};

CWeakRef::CWeakRef()
{
}

CWeakRef* CWeakRef::create(const Referable* object)
{
	CWeakRef* ret = new CWeakRef;
	if (ret) {
		ret->m_object = (Referable*)object;
		ret->increaseReference();
	}
	return ret;
}

Ref<Referable> CWeakRef::lock()
{
	Ref<Referable> ret;
	SpinLocker lock(&m_lock);
	Referable* obj = m_object;
	if (obj) {
		sl_reg n = obj->_increaseReference();
		if (n > 1) {
			ret = obj;
		}
		obj->_decreaseReference();
	}
	return ret;
}

void CWeakRef::release()
{
	{
		SpinLocker lock(&m_lock);
		m_object = sl_null;
	}
	decreaseReference();
}

SLIB_NAMESPACE_END
