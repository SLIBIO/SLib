/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/ref.h"

#define _SIGNATURE 0x15181289

namespace slib
{

	struct _Ref_Const
	{
		void* ptr;
		sl_int32 lock;
	};

	const _Ref_Const _Ref_Null = {0, 0};

	Referable::Referable()
	{
#ifdef SLIB_DEBUG_REFERENCE
		m_signature = _SIGNATURE;
#endif
		m_nRefCount = 0;
		m_flagWeakRef = sl_false;
		m_weak = sl_null;
	}

	Referable::Referable(const Referable& other)
	{
#ifdef SLIB_DEBUG_REFERENCE
		m_signature = _SIGNATURE;
#endif
		m_nRefCount = 0;
		m_flagWeakRef = sl_false;
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
	
	sl_reg Referable::getReferenceCount()
	{
		return m_nRefCount;
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

	void Referable::makeNeverFree()
	{
		m_nRefCount = -1;
	}
	
	sl_object_type Referable::ObjectType()
	{
		return 0;
	}
	
	sl_bool Referable::checkObjectType(sl_object_type type)
	{
		return sl_false;
	}

	sl_object_type Referable::getObjectType() const
	{
		return 0;
	}

	sl_bool Referable::isInstanceOf(sl_object_type type) const
	{
		return sl_false;
	}

	CWeakRef* Referable::_getWeakObject()
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


	SLIB_DEFINE_ROOT_OBJECT(CWeakRef)

	CWeakRef::CWeakRef()
	{
		m_object = sl_null;
		m_flagWeakRef = sl_true;
	}

	CWeakRef::~CWeakRef()
	{
	}

	CWeakRef* CWeakRef::create(Referable* object)
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


	ReferableKeeper::ReferableKeeper(Referable* object)
	{
		m_object = object;
		object->increaseReference();
	}

	ReferableKeeper::~ReferableKeeper()
	{
		m_object->decreaseReferenceNoFree();
	}

}
