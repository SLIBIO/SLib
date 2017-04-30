/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/ref.h"

#define _SIGNATURE 0x15181289

namespace slib
{

	struct _priv_Ref_Const
	{
		void* ptr;
		sl_int32 lock;
	};

	const _priv_Ref_Const _priv_Ref_Null = {0, 0};

	Referable::Referable() noexcept
	{
#ifdef SLIB_DEBUG_REFERENCE
		m_signature = _SIGNATURE;
#else
		SLIB_UNUSED(m_signature);
#endif
		m_nRefCount = 0;
		m_flagWeakRef = sl_false;
		m_weak = sl_null;
	}

	Referable::Referable(const Referable& other) noexcept
	{
#ifdef SLIB_DEBUG_REFERENCE
		m_signature = _SIGNATURE;
#endif
		m_nRefCount = 0;
		m_flagWeakRef = sl_false;
		m_weak = sl_null;
	}

	Referable::~Referable() noexcept
	{
		_clearWeak();
	}

	sl_reg Referable::increaseReference() noexcept
	{
		if (m_nRefCount >= 0) {
#ifdef SLIB_DEBUG_REFERENCE
			_checkValid();
#endif
			return Base::interlockedIncrement(&m_nRefCount);
		}
		return 1;
	}

	sl_reg Referable::decreaseReference() noexcept
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
	
	sl_reg Referable::getReferenceCount() noexcept
	{
		return m_nRefCount;
	}

	sl_reg Referable::decreaseReferenceNoFree() noexcept
	{
		if (m_nRefCount > 0) {
#ifdef SLIB_DEBUG_REFERENCE
			_checkValid();
#endif
			return Base::interlockedDecrement(&m_nRefCount);
		}
		return 1;
	}

	void Referable::makeNeverFree() noexcept
	{
		m_nRefCount = -1;
	}
	
	sl_object_type Referable::ObjectType() noexcept
	{
		return 0;
	}
	
	sl_bool Referable::checkObjectType(sl_object_type type) noexcept
	{
		return sl_false;
	}

	sl_object_type Referable::getObjectType() const noexcept
	{
		return 0;
	}

	sl_bool Referable::isInstanceOf(sl_object_type type) const noexcept
	{
		return sl_false;
	}

	CWeakRef* Referable::_getWeakObject() noexcept
	{
		SpinLocker lock(&m_lockWeak);
		if (! m_weak) {
			m_weak = CWeakRef::create(this);
		}
		return m_weak;
	}

	void Referable::_clearWeak() noexcept
	{
		if (m_weak) {
			m_weak->release();
			m_weak = sl_null;
		}
	}

	void Referable::_free() noexcept
	{
		_clearWeak();
		delete this;
	}

#ifdef SLIB_DEBUG_REFERENCE
	void Referable::_checkValid() noexcept
	{
		if (m_signature != _SIGNATURE) {
			SLIB_ABORT("Not-Referable object is referenced");
		}
	}
#endif


	SLIB_DEFINE_ROOT_OBJECT(CWeakRef)

	CWeakRef::CWeakRef() noexcept
	{
		m_object = sl_null;
		m_flagWeakRef = sl_true;
	}

	CWeakRef::~CWeakRef() noexcept
	{
	}

	CWeakRef* CWeakRef::create(Referable* object) noexcept
	{
		CWeakRef* ret = new CWeakRef;
		if (ret) {
			ret->m_object = (Referable*)object;
			ret->increaseReference();
		}
		return ret;
	}

	Ref<Referable> CWeakRef::lock() noexcept
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

	void CWeakRef::release() noexcept
	{
		{
			SpinLocker lock(&m_lock);
			m_object = sl_null;
		}
		decreaseReference();
	}


	ReferableKeeper::ReferableKeeper(Referable* object) noexcept
	{
		m_object = object;
		object->increaseReference();
	}

	ReferableKeeper::~ReferableKeeper() noexcept
	{
		m_object->decreaseReferenceNoFree();
	}

}
