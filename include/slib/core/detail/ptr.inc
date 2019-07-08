/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include <new>

namespace slib
{
	
	namespace priv
	{
		namespace ptr
		{
			struct ConstStruct;
			extern const ConstStruct g_null;
		}
	}

	template <class T>
	SLIB_INLINE Ptr<T>::Ptr() noexcept
	 : _ptr(sl_null)
	 {}

	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(Ptr<T>&& other) noexcept
	{
		_move_init(&other);
	}

	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(const Ptr<T>& other) noexcept
	 : _ptr(other._ptr), ref(other.ref)
	 {}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(Ptr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}

	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(const Ptr<O>& other) noexcept
	 : _ptr(other._ptr), ref(other.ref)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
	}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(AtomicPtr<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Ptr<T>::Ptr(const AtomicPtr<T>& other) noexcept
	{
		_ptr = other._retain(ref);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(AtomicPtr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicPtr<O>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(ref);
	}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(sl_null_t) noexcept
	 : _ptr(sl_null)
	 {}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(T* pointer) noexcept
	 : _ptr(pointer)
	 {}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(const Ref<O>& reference) noexcept
	 : _ptr(reference._ptr), ref(reference)
	 {}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(T* pointer, const Ref<O>& reference) noexcept
	 : ref(reference)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}

	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicRef<O>& reference) noexcept
	 : ref(reference)
	{
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(T* pointer, const AtomicRef<O>& reference) noexcept
	 : ref(reference)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const WeakRef<O>& weak) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(T* pointer, const WeakRef<O>& weak) noexcept
	 : ref(weak._weak)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicWeakRef<O>& _weak) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		WeakRef<O> weak(_weak);
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(T* pointer, const AtomicWeakRef<O>& weak) noexcept
	 : ref(weak._weak)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	SLIB_INLINE Ptr<T> Ptr<T>::fromPointer(T* pointer) noexcept
	{
		return Ptr<T>(pointer);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(O* reference) noexcept
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, O* reference) noexcept
	{
		return Ptr<T>(pointer, Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const Ref<O>& reference) noexcept
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const Ref<O>& reference) noexcept
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicRef<O>& reference) noexcept
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const AtomicRef<O>& reference) noexcept
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const WeakRef<O>& reference) noexcept
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const WeakRef<O>& reference) noexcept
	{
		return Ptr<T>(pointer, Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicWeakRef<O>& reference) noexcept
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const AtomicWeakRef<O>& reference) noexcept
	{
		return Ptr<T>(pointer, Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(O* reference) noexcept
	{
		return Ptr<T>(reference, Ref<Referable>(reference ? reference->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, O* o) noexcept
	{
		return Ptr<T>(o ? pointer : sl_null, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const Ref<O>& reference) noexcept
	{
		O* o = reference._ptr;
		return Ptr<T>(o, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const Ref<O>& reference) noexcept
	{
		O* o = reference._ptr;
		return Ptr<T>(o ? pointer : sl_null, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicRef<O>& _reference) noexcept
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		return Ptr<T>(o, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const AtomicRef<O>& _reference) noexcept
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		return Ptr<T>(o ? pointer : sl_null, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const WeakRef<O>& reference) noexcept
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const WeakRef<O>& reference) noexcept
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicWeakRef<O>& reference) noexcept
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const AtomicWeakRef<O>& reference) noexcept
	{
		return Ptr<T>(pointer, reference);
	}
	
	
	template <class T>
	SLIB_INLINE const Ptr<T>& Ptr<T>::null() noexcept
	{
		return *(reinterpret_cast<Ptr<T> const*>(&(priv::ptr::g_null)));
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::isNull() const noexcept
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::isNotNull() const noexcept
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	void Ptr<T>::setNull() noexcept
	{
		_ptr = sl_null;
		ref.setNull();
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE const Ptr<T>& Ptr<T>::from(const Ptr<O>& other) noexcept
	{
		return *(reinterpret_cast<Ptr<T> const*>(&other));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>& Ptr<T>::from(Ptr<O>& other) noexcept
	{
		return *(reinterpret_cast<Ptr<T>*>(&other));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>&& Ptr<T>::from(Ptr<O>&& other) noexcept
	{
		return static_cast<Ptr<T>&&>(*(reinterpret_cast<Ptr<T>*>(&other)));
	}

	template <class T>
	sl_bool Ptr<T>::isWeak() const noexcept
	{
		Referable* obj = ref._ptr;
		return obj && obj->_isWeakRef();
	}
	
	template <class T>
	Ptr<T> Ptr<T>::lock() const noexcept
	{
		if (_ptr) {
			Referable* obj = ref._ptr;
			if (obj && obj->_isWeakRef()) {
				CWeakRef* weak = static_cast<CWeakRef*>(obj);
				Ref<Referable> r(weak->lock());
				if (r.isNotNull()) {
					return Ptr<T>(_ptr, r);
				}
			} else {
				return *this;
			}
		}
		return sl_null;
	}

	template <class T>
	Ptr<T> Ptr<T>::toWeak() const noexcept
	{
		if (_ptr) {
			Referable* obj = ref._ptr;
			if (obj && !(obj->_isWeakRef())) {
				return Ptr<T>(_ptr, WeakRef<Referable>(obj));
			} else {
				return *this;
			}
		}
		return sl_null;
	}

	template <class T>
	SLIB_INLINE T* Ptr<T>::get() const noexcept
	{
		return _ptr;
	}

	template <class T>
	void Ptr<T>::setPointer(T* pointer) noexcept
	{
		_ptr = pointer;
		ref.setNull();
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(O* reference) noexcept
	{
		_ptr = reference;
		ref = reference;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(const Ref<O>& reference) noexcept
	{
		_ptr = reference._ptr;
		ref = reference;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const Ref<O>& reference) noexcept
	{
		if (reference.isNotNull()) {
			_ptr = pointer;
		} else {
			_ptr = sl_null;
		}
		ref = reference;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(const AtomicRef<O>& reference) noexcept
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const AtomicRef<O>& reference) noexcept
	{
		ref = reference;
		if (ref.isNotNull()) {
			_ptr = pointer;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(const WeakRef<O>& reference) noexcept
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const WeakRef<O>& reference) noexcept
	{
		ref = reference;
		if (ref.isNotNull()) {
			_ptr = pointer;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(const AtomicWeakRef<O>& reference) noexcept
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const AtomicWeakRef<O>& reference) noexcept
	{
		ref = reference;
		if (ref.isNotNull()) {
			_ptr = pointer;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(O* reference) noexcept
	{
		_ptr = reference;
		if (reference) {
			ref = reference->_getWeakObject();
		} else {
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(const Ref<O>& reference) noexcept
	{
		O* o = reference._ptr;
		_ptr = o;
		if (o) {
			ref = o->_getWeakObject();
		} else {
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(T* pointer, const Ref<O>& reference) noexcept
	{
		O* o = reference._ptr;
		if (o) {
			ref = o->_getWeakObject();
			_ptr = pointer;
		} else {
			ref.setNull();
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(const AtomicRef<O>& _reference) noexcept
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		_ptr = o;
		if (o) {
			ref = o->_getWeakObject();
		} else {
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(T* pointer, const AtomicRef<O>& _reference) noexcept
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		if (o) {
			ref = o->_getWeakObject();
			_ptr = pointer;
		} else {
			ref.setNull();
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(const WeakRef<O>& weak) noexcept
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(T* pointer, const WeakRef<O>& weak) noexcept
	{
		if (weak.isNotNull()) {
			_ptr = pointer;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(const AtomicWeakRef<O>& _weak) noexcept
	{
		WeakRef<O> weak(_weak);
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setWeak(T* pointer, const AtomicWeakRef<O>& _weak) noexcept
	{
		WeakRef<O> weak(_weak);
		if (weak.isNotNull()) {
			_ptr = pointer;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(Ptr<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(const Ptr<T>& other) noexcept
	{
		_ptr = other._ptr;
		ref = other.ref;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}

	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const Ptr<O>& other) noexcept
	{
		_ptr = other._ptr;
		ref = other.ref;
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(AtomicPtr<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(const AtomicPtr<T>& other) noexcept
	{
		_ptr = other._retain(ref);
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(AtomicPtr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const AtomicPtr<O>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(ref);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(sl_null_t) noexcept
	{
		_ptr = sl_null;
		ref.setNull();
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(T* pointer) noexcept
	{
		_ptr = pointer;
		ref.setNull();
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const Ref<O>& reference) noexcept
	{
		_ptr = reference._ptr;
		ref = reference;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const AtomicRef<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		ref = reference;
		_ptr = ref._ptr;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const WeakRef<O>& weak) noexcept
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const AtomicWeakRef<O>& _weak) noexcept
	{
		WeakRef<O> weak(_weak);
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			ref = weak._weak;
		} else {
			_ptr = sl_null;
			ref.setNull();
		}
		return *this;
	}

	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<O>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const AtomicPtr<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const AtomicPtr<O>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<O>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const AtomicPtr<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const AtomicPtr<O>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE T& Ptr<T>::operator*() const noexcept
	{
		return *_ptr;
	}
	
	template <class T>
	SLIB_INLINE T* Ptr<T>::operator->() const noexcept
	{
		return _ptr;
	}
	
	template <class T>
	SLIB_INLINE void Ptr<T>::_move_init(void* _other) noexcept
	{
		Ptr<T>& other = *(reinterpret_cast<Ptr<T>*>(_other));
		_ptr = other._ptr;
		ref._move_init(&(other.ref));
	}
	
	template <class T>
	SLIB_INLINE void Ptr<T>::_move_assign(void* _other) noexcept
	{
		if ((void*)this != _other) {
			Ptr<T>& other = *(reinterpret_cast<Ptr<T>*>(_other));
			_ptr = other._ptr;
			ref._move_assign(&(other.ref));
		}
	}
	
	
	template <class T>
	Atomic< Ptr<T> >::Atomic() noexcept
	 : _ptr(sl_null)
	 {}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(AtomicPtr<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(const AtomicPtr<T>& other) noexcept
	{
		_ptr = other._retain(_ref);
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(AtomicPtr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicPtr<O>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(_ref);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(Ptr<T>&& other) noexcept
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(const Ptr<T>& other) noexcept
	 : _ptr(other._ptr), _ref(other.ref)
	 {}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(Ptr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}

	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const Ptr<O>& other) noexcept
	 : _ptr(other._ptr), _ref(other.ref)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(sl_null_t) noexcept
	 : _ptr(sl_null)
	 {}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(T* pointer) noexcept
	 : _ptr(pointer)
	 {}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const Ref<O>& reference) noexcept
	 : _ptr(reference._ptr), _ref(reference)
	 {}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const Ref<O>& reference) noexcept
	 : _ref(reference)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicRef<O>& reference) noexcept
	 : _ref(reference)
	{
		_ptr = _ref._ptr;
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const AtomicRef<O>& reference) noexcept
	 : _ref(reference)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const WeakRef<O>& weak) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			_ref = weak._weak;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const WeakRef<O>& weak) noexcept
	 : _ref(weak._weak)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicWeakRef<O>& _weak) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		WeakRef<O> weak(_weak);
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_ptr = o._ptr;
			_ref = weak._weak;
		} else {
			_ptr = sl_null;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const AtomicWeakRef<O>& weak) noexcept
	 : _ref(weak._weak)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	SLIB_INLINE const AtomicPtr<T>& Atomic< Ptr<T> >::null() noexcept
	{
		return *(reinterpret_cast<AtomicPtr<T> const*>(&(priv::ptr::g_null)));
	}
	
	template <class T>
	sl_bool Atomic< Ptr<T> >::isNull() const noexcept
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	sl_bool Atomic< Ptr<T> >::isNotNull() const noexcept
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	void Atomic< Ptr<T> >::setNull() noexcept
	{
		_replace(sl_null, Ref<Referable>::null());
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE const AtomicPtr<T>& Atomic< Ptr<T> >::from(const AtomicPtr<O>& other) noexcept
	{
		return *(reinterpret_cast<AtomicPtr<T> const*>(&other));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE AtomicPtr<T>& Atomic< Ptr<T> >::from(AtomicPtr<O>& other) noexcept
	{
		return *(reinterpret_cast<AtomicPtr<T>*>(&other));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE AtomicPtr<T>&& Atomic< Ptr<T> >::from(AtomicPtr<O>&& other) noexcept
	{
		return static_cast<AtomicPtr<T>&&>(*(reinterpret_cast<AtomicPtr<T>*>(&other)));
	}
	
	template <class T>
	Ptr<T> Atomic< Ptr<T> >::lock() const noexcept
	{
		Ptr<T> p(*this);
		return p.lock();
	}
	
	template <class T>
	void Atomic< Ptr<T> >::setPointer(T* pointer) noexcept
	{
		_replace(pointer, Ref<Referable>::null());
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(O* reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference, reference);
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const Ref<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const Ref<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const AtomicRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const AtomicRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const WeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const WeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const AtomicWeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const AtomicWeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(O* o) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		if (o) {
			_replace(o, o->_getWeakObject());
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(const Ref<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		O* o = reference._ptr;
		if (o) {
			_replace(o, o->_getWeakObject());
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(T* pointer, const Ref<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		O* o = reference._ptr;
		if (o) {
			_replace(pointer, o->_getWeakObject());
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(const AtomicRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		if (o) {
			_replace(o, o->_getWeakObject());
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(T* pointer, const AtomicRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		if (o) {
			_replace(pointer, o->_getWeakObject());
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(const WeakRef<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> o(reference);
		if (o.isNotNull()) {
			_replace(o._ptr, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(T* pointer, const WeakRef<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		if (reference.isNotNull()) {
			_replace(pointer, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(const AtomicWeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		WeakRef<O> reference(_reference);
		Ref<O> o(reference);
		if (o.isNotNull()) {
			_replace(o._ptr, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(T* pointer, const AtomicWeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		WeakRef<O> reference(_reference);
		if (reference.isNotNull()) {
			_replace(pointer, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
	}

	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(AtomicPtr<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}

	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicPtr<T>& other) noexcept
	{
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(AtomicPtr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicPtr<O>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(Ptr<T>&& other) noexcept
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ptr<T>& other) noexcept
	{
		_replace(other._ptr, other.ref);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(Ptr<O>&& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ptr<O>& other) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(other._ptr, other.ref);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(sl_null_t) noexcept
	{
		_replace(sl_null, Ref<Referable>::null());
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(T* pointer) noexcept
	{
		_replace(pointer, Ref<Referable>::null());
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ref<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const WeakRef<O>& reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> o(reference);
		if (o.isNotNull()) {
			_replace(o._ptr, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicWeakRef<O>& _reference) noexcept
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		WeakRef<O> reference(_reference);
		Ref<O> o(reference);
		if (o.isNotNull()) {
			_replace(o._ptr, Ref<Referable>::from(reference._weak));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const AtomicPtr<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const AtomicPtr<O>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const Ptr<T>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const Ptr<O>& other) const noexcept
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const AtomicPtr<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const AtomicPtr<O>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const Ptr<T>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const Ptr<O>& other) const noexcept
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE T* Atomic< Ptr<T> >::_retain(Ref<Referable>& reference) const noexcept
	{
		if ((void*)this == (void*)(&(priv::ptr::g_null))) {
			return sl_null;
		} else {			
			m_lock.lock();
			reference = _ref;
			T* ret = _ptr;
			m_lock.unlock();
			return ret;
		}
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_replace(T* pointer, const Ref<Referable>& reference) noexcept
	{
		m_lock.lock();
		_ptr = pointer;
		Referable* refOld = _ref._ptr;
		new (&_ref) Ref<Referable>(reference);
		m_lock.unlock();
		if (refOld) {
			refOld->decreaseReference();
		}
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_move_init(void* _other) noexcept
	{
		AtomicPtr<T>& other = *(reinterpret_cast<AtomicPtr<T>*>(_other));
		_ptr = other._ptr;
		_ref._move_init(&(other._ref));
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_move_assign(void* _other) noexcept
	{
		if ((void*)this != _other) {
			AtomicPtr<T>& other = *(reinterpret_cast<AtomicPtr<T>*>(_other));
			m_lock.lock();
			_ptr = other._ptr;
			Referable* refOld = _ref._ptr;
			_ref._move_init(&(other._ref));
			m_lock.unlock();
			if (refOld) {
				refOld->decreaseReference();
			}
		}
	}


	template <class T>
	SLIB_INLINE PtrLocker<T>::PtrLocker(const Ptr<T>& ptr) noexcept
	 : m_ptr(ptr.lock())
	 {}

	template <class T>
	SLIB_INLINE PtrLocker<T>::PtrLocker(const AtomicPtr<T>& ptr) noexcept
	 : m_ptr(ptr.lock())
	 {}

	template <class T>
	SLIB_INLINE void PtrLocker<T>::unlock() noexcept
	{
		m_ptr.setNull();
	}

	template <class T>
	SLIB_INLINE T* PtrLocker<T>::get() noexcept
	{
		return m_ptr._ptr;
	}

	template <class T>
	SLIB_INLINE sl_bool PtrLocker<T>::isNull() noexcept
	{
		return m_ptr.isNull();
	}

	template <class T>
	SLIB_INLINE sl_bool PtrLocker<T>::isNotNull() noexcept
	{
		return m_ptr.isNotNull();
	}

	template <class T>
	SLIB_INLINE T& PtrLocker<T>::operator*() const noexcept
	{
		return *(m_ptr._ptr);
	}

	template <class T>
	SLIB_INLINE T* PtrLocker<T>::operator->() const noexcept
	{
		return m_ptr._ptr;
	}

	namespace priv
	{
		namespace ptr
		{

			template <class T>
			class SharedPtrContainer : public Referable
			{
			public:
				T* ptr;
				
			public:
				SLIB_INLINE SharedPtrContainer(T* _ptr) : ptr(_ptr)
				{					
				}
				
				SLIB_INLINE ~SharedPtrContainer()
				{
					delete ptr;
				}				
			};

			template <class T, class DELETER>
			class SharedPtrContainerWithDeleter : public Referable
			{
			public:
				T* ptr;
				DELETER deleter;
				
			public:
				SLIB_INLINE SharedPtrContainerWithDeleter(T* _ptr, const DELETER& _deleter): ptr(_ptr), deleter(_deleter)
				{
				}

				SLIB_INLINE ~SharedPtrContainerWithDeleter()
				{
					deleter(ptr);
				}
				
			};
			
			template <class T>
			class SharedPtrObjectContainer : public Referable
			{
			public:
				T object;
				
			public:
				template <class... ARGS>
				SLIB_INLINE SharedPtrObjectContainer(ARGS&&... args): object(Forward<ARGS>(args)...)
				{
				}
				
			};
			
		}
	}

	template <class T>
	SLIB_INLINE Ptr<T> SharedPtr(T* ptr)
	{
		if (ptr) {
			return Ptr<T>::fromRef(ptr, new priv::ptr::SharedPtrContainer<T>(ptr));
		} else {
			return sl_null;
		}
	}

	template <class T, class DELETER>
	SLIB_INLINE Ptr<T> SharedPtr(T* ptr, const DELETER& deleter)
	{
		if (ptr) {
			return Ptr<T>::fromRef(ptr, new priv::ptr::SharedPtrContainerWithDeleter<T, DELETER>(ptr, deleter));
		} else {
			return sl_null;
		}
	}
		
	template<class T, class... ARGS>
	SLIB_INLINE Ptr<T> MakeShared(ARGS&&... args)
	{
		priv::ptr::SharedPtrObjectContainer<T>* ptr = new priv::ptr::SharedPtrObjectContainer<T>(Forward<ARGS>(args)...);
		if (ptr) {
			return Ptr<T>::fromRef(&(ptr->object), ptr);
		} else {
			return sl_null;
		}
	}

}
