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

sl_reg Referable::increaseReference()
{
	if (m_nRefCount >= 0) {
#ifdef SLIB_DEBUG_REFERENCE
		_checkValid();
#endif
		return Base::interlockedIncrement(&m_nRefCount);
	}
	return 1;
}

sl_reg Referable::decreaseReference()
{
	if (m_nRefCount > 0) {
#ifdef SLIB_DEBUG_REFERENCE
		_checkValid();
#endif
		sl_reg nRef = Base::interlockedDecrement(&m_nRefCount);
		if (nRef == 0) {
			_free();
		}
		return nRef;
	}
	return 1;
}

sl_reg Referable::decreaseReferenceNoFree()
{
	if (m_nRefCount > 0) {
#ifdef SLIB_DEBUG_REFERENCE
		_checkValid();
#endif
		return Base::interlockedDecrement(&m_nRefCount);
	}
	return 1;
}

CWeakRef* Referable::getWeakObject()
{
	SpinLocker lock(&m_lockWeak);
	if (! m_weak) {
		m_weak = CWeakRef::create(this);
	}
	return m_weak;
}

void Referable::makeNeverFree()
{
	m_nRefCount = -1;
}

sl_class_type Referable::getClassType() const
{
	return 0;
}

sl_bool Referable::checkClassType(sl_class_type type) const
{
	return sl_false;
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

const _Ref_Const _Ref_Null = {0, 0};


SLIB_DEFINE_ROOT_OBJECT(CWeakRef)

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
		sl_reg n = obj->increaseReference();
		if (n > 1) {
			ret = obj;
		}
		obj->decreaseReferenceNoFree();
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
