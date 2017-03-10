#ifndef CHECKHEADER_SLIB_CORE_DETAIL_PTR
#define CHECKHEADER_SLIB_CORE_DETAIL_PTR

#include "../ptr.h"

#include <new>

namespace slib
{
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr() : _ptr(sl_null)
	{
	}

	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(Ptr<T>&& other)
	{
		_move_init(&other);
	}

	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(const Ptr<T>& other) : _ptr(other._ptr), ref(other.ref)
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(Ptr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}

	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(const Ptr<O>& other) : _ptr(other._ptr), ref(other.ref)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
	}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(AtomicPtr<T>&& other)
	{
		_move_init(&other);
	}
	
	template <class T>
	Ptr<T>::Ptr(const AtomicPtr<T>& other)
	{
		_ptr = other._retain(ref);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(AtomicPtr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicPtr<O>& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(ref);
	}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(sl_null_t) : _ptr(sl_null)
	{
	}
	
	template <class T>
	SLIB_INLINE Ptr<T>::Ptr(T* pointer) : _ptr(pointer)
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(const Ref<O>& reference) : _ptr(reference._ptr), ref(reference)
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T>::Ptr(T* pointer, const Ref<O>& reference) : ref(reference)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}

	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicRef<O>& reference) : ref(reference)
	{
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(T* pointer, const AtomicRef<O>& reference) : ref(reference)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const WeakRef<O>& weak)
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
	Ptr<T>::Ptr(T* pointer, const WeakRef<O>& weak) : ref(weak._weak)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Ptr<T>::Ptr(const AtomicWeakRef<O>& _weak)
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
	Ptr<T>::Ptr(T* pointer, const AtomicWeakRef<O>& weak) : ref(weak._weak)
	{
		if (ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	SLIB_INLINE Ptr<T> Ptr<T>::fromPointer(T* pointer)
	{
		return Ptr<T>(pointer);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(O* reference)
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const Ref<O>& reference)
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const Ref<O>& reference)
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicRef<O>& reference)
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const AtomicRef<O>& reference)
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const WeakRef<O>& reference)
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const WeakRef<O>& reference)
	{
		return Ptr<T>(pointer, Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicWeakRef<O>& reference)
	{
		return Ptr<T>(Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromRef(T* pointer, const AtomicWeakRef<O>& reference)
	{
		return Ptr<T>(pointer, Ref<O>(reference));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(O* reference)
	{
		return Ptr<T>(reference, Ref<Referable>(reference ? reference->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const Ref<O>& reference)
	{
		O* o = reference._ptr;
		return Ptr<T>(o, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const Ref<O>& reference)
	{
		O* o = reference._ptr;
		return Ptr<T>(o ? pointer : sl_null, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		return Ptr<T>(o, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const AtomicRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		O* o = reference._ptr;
		return Ptr<T>(o ? pointer : sl_null, Ref<Referable>(o ? o->_getWeakObject() : sl_null));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const WeakRef<O>& reference)
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const WeakRef<O>& reference)
	{
		return Ptr<T>(pointer, reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicWeakRef<O>& reference)
	{
		return Ptr<T>(reference);
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(T* pointer, const AtomicWeakRef<O>& reference)
	{
		return Ptr<T>(pointer, reference);
	}
	
	
	template <class T>
	SLIB_INLINE const Ptr<T>& Ptr<T>::null()
	{
		return *(reinterpret_cast<Ptr<T> const*>(&_Ptr_Null));
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::isNull() const
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::isNotNull() const
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	void Ptr<T>::setNull()
	{
		_ptr = sl_null;
		ref.setNull();
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE const Ptr<T>& Ptr<T>::from(const Ptr<O>& other)
	{
		return *(reinterpret_cast<Ptr<T> const*>(&other));
	}
	
	template <class T>
	sl_bool Ptr<T>::isWeak() const
	{
		Referable* obj = ref._ptr;
		return obj && obj->_isWeakRef();
	}
	
	template <class T>
	Ptr<T> Ptr<T>::lock() const
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
	void Ptr<T>::setPointer(T* pointer)
	{
		_ptr = pointer;
		ref.setNull();
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(O* reference)
	{
		_ptr = reference;
		ref = reference;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(const Ref<O>& reference)
	{
		_ptr = reference._ptr;
		ref = reference;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const Ref<O>& reference)
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
	void Ptr<T>::setRef(const AtomicRef<O>& reference)
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const AtomicRef<O>& reference)
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
	void Ptr<T>::setRef(const WeakRef<O>& reference)
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const WeakRef<O>& reference)
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
	void Ptr<T>::setRef(const AtomicWeakRef<O>& reference)
	{
		ref = reference;
		_ptr = ref._ptr;
	}
	
	template <class T>
	template <class O>
	void Ptr<T>::setRef(T* pointer, const AtomicWeakRef<O>& reference)
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
	void Ptr<T>::setWeak(O* reference)
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
	void Ptr<T>::setWeak(const Ref<O>& reference)
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
	void Ptr<T>::setWeak(T* pointer, const Ref<O>& reference)
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
	void Ptr<T>::setWeak(const AtomicRef<O>& _reference)
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
	void Ptr<T>::setWeak(T* pointer, const AtomicRef<O>& _reference)
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
	void Ptr<T>::setWeak(const WeakRef<O>& weak)
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
	void Ptr<T>::setWeak(T* pointer, const WeakRef<O>& weak)
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
	void Ptr<T>::setWeak(const AtomicWeakRef<O>& _weak)
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
	void Ptr<T>::setWeak(T* pointer, const AtomicWeakRef<O>& _weak)
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
	Ptr<T>& Ptr<T>::operator=(Ptr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(const Ptr<T>& other)
	{
		_ptr = other._ptr;
		ref = other.ref;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}

	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const Ptr<O>& other)
	{
		_ptr = other._ptr;
		ref = other.ref;
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(AtomicPtr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(const AtomicPtr<T>& other)
	{
		_ptr = other._retain(ref);
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(AtomicPtr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const AtomicPtr<O>& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(ref);
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(sl_null_t)
	{
		_ptr = sl_null;
		ref.setNull();
		return *this;
	}
	
	template <class T>
	Ptr<T>& Ptr<T>::operator=(T* pointer)
	{
		_ptr = pointer;
		ref.setNull();
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const Ref<O>& reference)
	{
		_ptr = reference._ptr;
		ref = reference;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const AtomicRef<O>& reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		ref = reference;
		_ptr = ref._ptr;
		return *this;
	}
	
	template <class T>
	template <class O>
	Ptr<T>& Ptr<T>::operator=(const WeakRef<O>& weak)
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
	Ptr<T>& Ptr<T>::operator=(const AtomicWeakRef<O>& _weak)
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
	SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<T>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<O>& other) const
	{
		return _ptr == other._ptr;
	}
	
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const AtomicPtr<T>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator==(const AtomicPtr<O>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<T>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<O>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const AtomicPtr<T>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Ptr<T>::operator!=(const AtomicPtr<O>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE T& Ptr<T>::operator*() const
	{
		return *_ptr;
	}
	
	template <class T>
	SLIB_INLINE T* Ptr<T>::operator->() const
	{
		return _ptr;
	}
	
	template <class T>
	SLIB_INLINE void Ptr<T>::_move_init(void* _other)
	{
		Ptr<T>& other = *(reinterpret_cast<Ptr<T>*>(_other));
		_ptr = other._ptr;
		ref._move_init(&(other.ref));
	}
	
	template <class T>
	SLIB_INLINE void Ptr<T>::_move_assign(void* _other)
	{
		if ((void*)this != _other) {
			Ptr<T>& other = *(reinterpret_cast<Ptr<T>*>(_other));
			_ptr = other._ptr;
			ref._move_assign(&(other.ref));
		}
	}
	
	
	template <class T>
	Atomic< Ptr<T> >::Atomic() : _ptr(sl_null)
	{
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(AtomicPtr<T>&& other)
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(const AtomicPtr<T>& other)
	{
		_ptr = other._retain(_ref);
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(AtomicPtr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicPtr<O>& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_ptr = other._retain(_ref);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(Ptr<T>&& other)
	{
		_move_init(&other);
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(const Ptr<T>& other) : _ptr(other._ptr), _ref(other.ref)
	{
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(Ptr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_init(&other);
	}

	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const Ptr<O>& other) : _ptr(other._ptr), _ref(other.ref)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(sl_null_t) : _ptr(sl_null)
	{
	}
	
	template <class T>
	Atomic< Ptr<T> >::Atomic(T* pointer) : _ptr(pointer)
	{
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const Ref<O>& reference) : _ptr(reference._ptr), _ref(reference)
	{
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const Ref<O>& reference) : _ref(reference)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicRef<O>& reference) : _ref(reference)
	{
		_ptr = _ref._ptr;
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(T* pointer, const AtomicRef<O>& reference) : _ref(reference)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const WeakRef<O>& weak)
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
	Atomic< Ptr<T> >::Atomic(T* pointer, const WeakRef<O>& weak) : _ref(weak._weak)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	template <class O>
	Atomic< Ptr<T> >::Atomic(const AtomicWeakRef<O>& _weak)
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
	Atomic< Ptr<T> >::Atomic(T* pointer, const AtomicWeakRef<O>& weak) : _ref(weak._weak)
	{
		if (_ref.isNull()) {
			_ptr = sl_null;
		} else {
			_ptr = pointer;
		}
	}
	
	template <class T>
	SLIB_INLINE const AtomicPtr<T>& Atomic< Ptr<T> >::null()
	{
		return *(reinterpret_cast<AtomicPtr<T> const*>(&_Ptr_Null));
	}
	
	template <class T>
	sl_bool Atomic< Ptr<T> >::isNull() const
	{
		return _ptr == sl_null;
	}
	
	template <class T>
	sl_bool Atomic< Ptr<T> >::isNotNull() const
	{
		return _ptr != sl_null;
	}
	
	template <class T>
	void Atomic< Ptr<T> >::setNull()
	{
		_replace(sl_null, Ref<Referable>::null());
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE const AtomicPtr<T>& Atomic< Ptr<T> >::from(const AtomicPtr<O>& other)
	{
		return *(reinterpret_cast<AtomicPtr<T> const*>(&other));
	}
	
	template <class T>
	Ptr<T> Atomic< Ptr<T> >::lock() const
	{
		Ptr<T> p(*this);
		return p.lock();
	}
	
	template <class T>
	void Atomic< Ptr<T> >::setPointer(T* pointer)
	{
		_replace(pointer, Ref<Referable>::null());
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(O* reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference, reference);
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const Ref<O>& reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const Ref<O>& reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const AtomicRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const AtomicRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const WeakRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const WeakRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(const AtomicWeakRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setRef(T* pointer, const AtomicWeakRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference.isNotNull() ? pointer : sl_null, Ref<Referable>::from(reference));
	}
	
	template <class T>
	template <class O>
	void Atomic< Ptr<T> >::setWeak(O* o)
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
	void Atomic< Ptr<T> >::setWeak(const Ref<O>& reference)
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
	void Atomic< Ptr<T> >::setWeak(T* pointer, const Ref<O>& reference)
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
	void Atomic< Ptr<T> >::setWeak(const AtomicRef<O>& _reference)
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
	void Atomic< Ptr<T> >::setWeak(T* pointer, const AtomicRef<O>& _reference)
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
	void Atomic< Ptr<T> >::setWeak(const WeakRef<O>& reference)
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
	void Atomic< Ptr<T> >::setWeak(T* pointer, const WeakRef<O>& reference)
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
	void Atomic< Ptr<T> >::setWeak(const AtomicWeakRef<O>& _reference)
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
	void Atomic< Ptr<T> >::setWeak(T* pointer, const AtomicWeakRef<O>& _reference)
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
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(AtomicPtr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicPtr<T>& other)
	{
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(AtomicPtr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicPtr<O>& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(Ptr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ptr<T>& other)
	{
		_replace(other._ptr, other.ref);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(Ptr<O>&& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_move_assign(&other);
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ptr<O>& other)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(other._ptr, other.ref);
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(sl_null_t)
	{
		_replace(sl_null, Ref<Referable>::null());
		return *this;
	}
	
	template <class T>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(T* pointer)
	{
		_replace(pointer, Ref<Referable>::null());
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ref<O>& reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		_replace(reference._ptr, Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicRef<O>& _reference)
	{
		SLIB_TRY_CONVERT_TYPE(O*, T*)
		Ref<O> reference(_reference);
		_replace(reference._ptr, Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class T>
	template <class O>
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const WeakRef<O>& reference)
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
	AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicWeakRef<O>& _reference)
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
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const AtomicPtr<T>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const AtomicPtr<O>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const Ptr<T>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator==(const Ptr<O>& other) const
	{
		return _ptr == other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const AtomicPtr<T>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const AtomicPtr<O>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const Ptr<T>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE sl_bool Atomic< Ptr<T> >::operator!=(const Ptr<O>& other) const
	{
		return _ptr != other._ptr;
	}
	
	template <class T>
	SLIB_INLINE T* Atomic< Ptr<T> >::_retain(Ref<Referable>& reference) const
	{
		if ((void*)this == (void*)(&_Ptr_Null)) {
			return sl_null;
		} else {
			SpinLocker lock(&m_lock);
			reference = _ref;
			return _ptr;
		}
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_replace(T* pointer, const Ref<Referable>& reference)
	{
		Referable* refOld;
		{
			SpinLocker lock(&m_lock);
			_ptr = pointer;
			refOld = _ref._ptr;
			new (&_ref) Ref<Referable>(reference);
		}
		if (refOld) {
			refOld->decreaseReference();
		}
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_move_init(void* _other)
	{
		AtomicPtr<T>& other = *(reinterpret_cast<AtomicPtr<T>*>(_other));
		_ptr = other._ptr;
		_ref._move_init(&(other._ref));
	}

	template <class T>
	SLIB_INLINE void Atomic< Ptr<T> >::_move_assign(void* _other)
	{
		if ((void*)this != _other) {
			AtomicPtr<T>& other = *(reinterpret_cast<AtomicPtr<T>*>(_other));
			Referable* refOld;
			{
				SpinLocker lock(&m_lock);
				_ptr = other._ptr;
				refOld = _ref._ptr;
				_ref._move_init(&(other._ref));
			}
			if (refOld) {
				refOld->decreaseReference();
			}
		}
	}


	template <class T>
	SLIB_INLINE PtrLocker<T>::PtrLocker(const Ptr<T>& ptr) : m_ptr(ptr.lock())
	{
	}

	template <class T>
	SLIB_INLINE PtrLocker<T>::PtrLocker(const AtomicPtr<T>& ptr) : m_ptr(ptr.lock())
	{
	}

	template <class T>
	SLIB_INLINE void PtrLocker<T>::unlock()
	{
		m_ptr.setNull();
	}

	template <class T>
	SLIB_INLINE T* PtrLocker<T>::get()
	{
		return m_ptr._ptr;
	}

	template <class T>
	SLIB_INLINE sl_bool PtrLocker<T>::isNull()
	{
		return m_ptr.isNull();
	}

	template <class T>
	SLIB_INLINE sl_bool PtrLocker<T>::isNotNull()
	{
		return m_ptr.isNotNull();
	}

	template <class T>
	SLIB_INLINE T& PtrLocker<T>::operator*() const
	{
		return *(m_ptr._ptr);
	}

	template <class T>
	SLIB_INLINE T* PtrLocker<T>::operator->() const
	{
		return m_ptr._ptr;
	}

}

#endif
