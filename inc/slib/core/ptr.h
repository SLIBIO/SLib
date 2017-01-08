#ifndef CHECKHEADER_SLIB_CORE_PTR
#define CHECKHEADER_SLIB_CORE_PTR

#include "definition.h"

#include "ref.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class Ptr;

template <class T>
using AtomicPtr = Atomic< Ptr<T> >;

struct _Ptr_Const
{
	void* _ptr;
	void* ref;
	sl_int32 lock;
};

extern const _Ptr_Const _Ptr_Null;

template <class T>
class SLIB_EXPORT Ptr
{
public:
	T* _ptr;
	Ref<Referable> ref;

public:
	Ptr();
	
	Ptr(Ptr<T>&& other);

	Ptr(const Ptr<T>& other);
	
	template <class O>
	Ptr(Ptr<O>&& other);

	template <class O>
	Ptr(const Ptr<O>& other);
	
	Ptr(AtomicPtr<T>&& other);
	
	Ptr(const AtomicPtr<T>& other);
	
	template <class O>
	Ptr(AtomicPtr<O>&& other);
	
	template <class O>
	Ptr(const AtomicPtr<O>& other);
	
	Ptr(sl_null_t);
	
	Ptr(const T* pointer);
	
	Ptr(const Ref<T>& reference);

	template <class O>
	Ptr(const Ref<O>& reference);
	
	template <class O>
	Ptr(const T* pointer, const Ref<O>& reference);
	
	Ptr(const AtomicRef<T>& reference);

	template <class O>
	Ptr(const AtomicRef<O>& reference);
	
	template <class O>
	Ptr(const T* pointer, const AtomicRef<O>& reference);
	
	Ptr(const WeakRef<T>& weak);
	
	template <class O>
	Ptr(const WeakRef<O>& weak);
	
	template <class O>
	Ptr(const T* pointer, const WeakRef<O>& weak);
	
	Ptr(const AtomicWeakRef<T>& weak);
	
	template <class O>
	Ptr(const AtomicWeakRef<O>& weak);
	
	template <class O>
	Ptr(const T* pointer, const AtomicWeakRef<O>& weak);
	
public:
	static Ptr<T> fromPointer(const T* pointer);
	
	static Ptr<T> fromRef(const Ref<T>& reference);
	
	template <class O>
	static Ptr<T> fromRef(const Ref<O>& reference);
	
	static Ptr<T> fromRef(const AtomicRef<T>& reference);
	
	template <class O>
	static Ptr<T> fromRef(const AtomicRef<O>& reference);
	
	static Ptr<T> fromWeak(const WeakRef<T>& weak);
	
	template <class O>
	static Ptr<T> fromWeak(const WeakRef<O>& weak);
	
	static Ptr<T> fromWeak(const AtomicWeakRef<T>& weak);
	
	template <class O>
	static Ptr<T> fromWeak(const AtomicWeakRef<O>& weak);
	
public:
	static const Ptr<T>& null();

	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();
	
	template <class O>
	static const Ptr<T>& from(const Ptr<O>& other);

	sl_bool isWeak() const;
	
	Ptr<T> lock() const;
	
public:
	void setPointer(const T* pointer);
	
	void setRef(const Ref<T>& reference);
	
	template <class O>
	void setRef(const Ref<O>& reference);
	
	void setRef(const AtomicRef<T>& reference);
	
	template <class O>
	void setRef(const AtomicRef<O>& reference);
	
	void setWeak(const WeakRef<T>& weak);
	
	template <class O>
	void setWeak(const WeakRef<O>& weak);
	
	void setWeak(const AtomicWeakRef<T>& weak);
	
	template <class O>
	void setWeak(const AtomicWeakRef<O>& weak);
	
public:
	Ptr<T>& operator=(Ptr<T>&& other);
	
	Ptr<T>& operator=(const Ptr<T>& other);
	
	template <class O>
	Ptr<T>& operator=(Ptr<O>&& other);

	template <class O>
	Ptr<T>& operator=(const Ptr<O>& other);
	
	Ptr<T>& operator=(AtomicPtr<T>&& other);
	
	Ptr<T>& operator=(const AtomicPtr<T>& other);
	
	template <class O>
	Ptr<T>& operator=(AtomicPtr<O>&& other);
	
	template <class O>
	Ptr<T>& operator=(const AtomicPtr<O>& other);
	
	Ptr<T>& operator=(sl_null_t);
	
	Ptr<T>& operator=(const T* pointer);

	Ptr<T>& operator=(const Ref<T>& reference);
	
	template <class O>
	Ptr<T>& operator=(const Ref<O>& reference);
	
	Ptr<T>& operator=(const AtomicRef<T>& reference);
	
	template <class O>
	Ptr<T>& operator=(const AtomicRef<O>& reference);
	
	Ptr<T>& operator=(const WeakRef<T>& weak);
	
	template <class O>
	Ptr<T>& operator=(const WeakRef<O>& weak);
	
	Ptr<T>& operator=(const AtomicWeakRef<T>& weak);
	
	template <class O>
	Ptr<T>& operator=(const AtomicWeakRef<O>& weak);
	
	
	sl_bool operator==(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const Ptr<O>& other) const;

	sl_bool operator==(const AtomicPtr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const AtomicPtr<O>& other) const;
	
	sl_bool operator!=(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const Ptr<O>& other) const;
	
	sl_bool operator!=(const AtomicPtr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const AtomicPtr<O>& other) const;
	
	
	T& operator*() const;
	
	T* operator->() const;

	
public:
	void _move_init(void* _other);
	
	void _move_assign(void* _other);
	
};


template <class T>
class SLIB_EXPORT Atomic< Ptr<T> >
{
public:
	T* _ptr;
	Ref<Referable> _ref;

public:
	Atomic();
	
	Atomic(AtomicPtr<T>&& other);
	
	Atomic(const AtomicPtr<T>& other);
	
	template <class O>
	Atomic(AtomicPtr<O>&& other);
	
	template <class O>
	Atomic(const AtomicPtr<O>& other);
	
	Atomic(Ptr<T>&& other);
	
	Atomic(const Ptr<T>& other);
	
	template <class O>
	Atomic(Ptr<O>&& other);

	template <class O>
	Atomic(const Ptr<O>& other);
	
	Atomic(sl_null_t);
	
	Atomic(const T* pointer);
	
	Atomic(const Ref<T>& reference);
	
	template <class O>
	Atomic(const Ref<O>& reference);
	
	template <class O>
	Atomic(const T* pointer, const Ref<O>& reference);
	
	Atomic(const AtomicRef<T>& reference);
	
	template <class O>
	Atomic(const AtomicRef<O>& reference);
	
	template <class O>
	Atomic(const T* pointer, const AtomicRef<O>& reference);
	
	Atomic(const WeakRef<T>& weak);
	
	template <class O>
	Atomic(const WeakRef<O>& weak);
	
	template <class O>
	Atomic(const T* pointer, const WeakRef<O>& weak);
	
	Atomic(const AtomicWeakRef<T>& weak);
	
	template <class O>
	Atomic(const AtomicWeakRef<O>& weak);
	
	template <class O>
	Atomic(const T* pointer, const AtomicWeakRef<O>& weak);
	
public:
	static const AtomicPtr<T>& null();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();
	
	template <class O>
	static const AtomicPtr<T>& from(const AtomicPtr<O>& other);
	
	Ptr<T> lock() const;
	
public:
	void setPointer(const T* pointer);
	
	void setRef(const Ref<T>& reference);
	
	template <class O>
	void setRef(const Ref<O>& reference);
	
	void setRef(const AtomicRef<T>& reference);
	
	template <class O>
	void setRef(const AtomicRef<O>& reference);
	
	void setWeak(const WeakRef<T>& weak);
	
	template <class O>
	void setWeak(const WeakRef<O>& weak);
	
	void setWeak(const AtomicWeakRef<T>& weak);
	
	template <class O>
	void setWeak(const AtomicWeakRef<O>& weak);
	
public:
	AtomicPtr<T>& operator=(AtomicPtr<T>&& other);
	
	AtomicPtr<T>& operator=(const AtomicPtr<T>& other);
	
	template <class O>
	AtomicPtr<T>& operator=(AtomicPtr<O>&& other);

	template <class O>
	AtomicPtr<T>& operator=(const AtomicPtr<O>& other);
	
	AtomicPtr<T>& operator=(Ptr<T>&& other);

	AtomicPtr<T>& operator=(const Ptr<T>& other);
	
	template <class O>
	AtomicPtr<T>& operator=(Ptr<O>&& other);

	template <class O>
	AtomicPtr<T>& operator=(const Ptr<O>& other);
	
	AtomicPtr<T>& operator=(sl_null_t);
	
	AtomicPtr<T>& operator=(const T* pointer);
	
	AtomicPtr<T>& operator=(const Ref<T>& reference);
	
	template <class O>
	AtomicPtr<T>& operator=(const Ref<O>& reference);
	
	AtomicPtr<T>& operator=(const AtomicRef<T>& reference);
	
	template <class O>
	AtomicPtr<T>& operator=(const AtomicRef<O>& reference);
	
	AtomicPtr<T>& operator=(const WeakRef<T>& weak);
	
	template <class O>
	AtomicPtr<T>& operator=(const WeakRef<O>& weak);
	
	AtomicPtr<T>& operator=(const AtomicWeakRef<T>& weak);
	
	template <class O>
	AtomicPtr<T>& operator=(const AtomicWeakRef<O>& weak);
	
	
	sl_bool operator==(const AtomicPtr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const AtomicPtr<O>& other) const;
	
	sl_bool operator==(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const Ptr<O>& other) const;
	
	sl_bool operator!=(const AtomicPtr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const AtomicPtr<O>& other) const;
	
	sl_bool operator!=(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const Ptr<O>& other) const;
	
public:
	T* _retain(Ref<Referable>& reference) const;
	
	void _replace(T* pointer, const Ref<Referable>& reference);
	
	void _move_init(void* _other);
	
	void _move_assign(void* _other);

private:
	SpinLock m_lock;

};


template <class T>
class SLIB_EXPORT PtrLocker
{
private:
	Ptr<T> m_ptr;
	
public:
	PtrLocker(const Ptr<T>& ptr);
	
	PtrLocker(const AtomicPtr<T>& ptr);
	
public:
	void unlock();

	T* get();
	
	sl_bool isNull();

	sl_bool isNotNull();

	T& operator*() const;

	T* operator->() const;
	
};

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

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
SLIB_INLINE Ptr<T>::Ptr(const T* pointer) : _ptr(const_cast<T*>(pointer))
{
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(const Ref<T>& reference) : _ptr(reference._ptr), ref(reference)
{
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const Ref<O>& reference) : _ptr(reference._ptr), ref(reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const T* pointer, const Ref<O>& reference) : _ptr(const_cast<T*>(pointer)), ref(reference)
{
}

template <class T>
Ptr<T>::Ptr(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
template <class O>
Ptr<T>::Ptr(const AtomicRef<O>& _reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
template <class O>
Ptr<T>::Ptr(const T* pointer, const AtomicRef<O>& reference) : _ptr(const_cast<T*>(pointer)), ref(reference)
{
}

template <class T>
Ptr<T>::Ptr(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
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
SLIB_INLINE Ptr<T>::Ptr(const T* pointer, const WeakRef<O>& weak) : _ptr(const_cast<T*>(pointer)), ref(weak._weak)
{
}

template <class T>
Ptr<T>::Ptr(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
Ptr<T>::Ptr(const AtomicWeakRef<O>& weak)
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
Ptr<T>::Ptr(const T* pointer, const AtomicWeakRef<O>& weak) : _ptr(const_cast<T*>(pointer)), ref(weak._weak)
{
}

template <class T>
SLIB_INLINE Ptr<T> Ptr<T>::fromPointer(const T* pointer)
{
	return Ptr<T>(pointer);
}

template <class T>
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const Ref<T>& reference)
{
	return Ptr<T>(reference);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const Ref<O>& reference)
{
	return Ptr<T>(reference);
}

template <class T>
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicRef<T>& reference)
{
	return Ptr<T>(reference);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const AtomicRef<O>& reference)
{
	return Ptr<T>(reference);
}

template <class T>
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const WeakRef<T>& weak)
{
	return Ptr<T>(weak);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const WeakRef<O>& weak)
{
	return Ptr<T>(weak);
}

template <class T>
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicWeakRef<T>& weak)
{
	return Ptr<T>(weak);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const AtomicWeakRef<O>& weak)
{
	return Ptr<T>(weak);
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
	return IsInstanceOf<CWeakRef>(ref);
}

template <class T>
Ptr<T> Ptr<T>::lock() const
{
	if (_ptr) {
		if (CWeakRef* weak = CastInstance<CWeakRef>(ref._ptr)) {
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
void Ptr<T>::setPointer(const T* pointer)
{
	_ptr = const_cast<T*>(pointer);
	ref.setNull();
}

template <class T>
void Ptr<T>::setRef(const Ref<T>& reference)
{
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
template <class O>
void Ptr<T>::setRef(const Ref<O>& reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
void Ptr<T>::setRef(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
template <class O>
void Ptr<T>::setRef(const AtomicRef<O>& _reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
}

template <class T>
void Ptr<T>::setWeak(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
void Ptr<T>::setWeak(const WeakRef<O>& weak)
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
void Ptr<T>::setWeak(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
void Ptr<T>::setWeak(const AtomicWeakRef<O>& weak)
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
Ptr<T>& Ptr<T>::operator=(const T* pointer)
{
	_ptr = const_cast<T*>(pointer);
	ref.setNull();
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const Ref<T>& reference)
{
	_ptr = reference._ptr;
	ref = reference;
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const Ref<O>& reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	_ptr = reference._ptr;
	ref = reference;
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const AtomicRef<O>& _reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> reference(_reference);
	_ptr = reference._ptr;
	ref = reference;
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
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
	}
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const AtomicWeakRef<O>& weak)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		ref = weak._weak;
	} else {
		_ptr = sl_null;
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
Atomic< Ptr<T> >::Atomic(const T* pointer) : _ptr(const_cast<T*>(pointer))
{
}

template <class T>
Atomic< Ptr<T> >::Atomic(const Ref<T>& reference) : _ptr(reference._ptr), _ref(reference)
{
}

template <class T>
template <class O>
Atomic< Ptr<T> >::Atomic(const Ref<O>& reference) : _ptr(reference._ptr), _ref(reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
}

template <class T>
template <class O>
Atomic< Ptr<T> >::Atomic(const T* pointer, const Ref<O>& reference) : _ptr(const_cast<T*>(pointer)), _ref(reference)
{
}

template <class T>
Atomic< Ptr<T> >::Atomic(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_ptr = reference._ptr;
	_ref = reference;
}

template <class T>
template <class O>
Atomic< Ptr<T> >::Atomic(const AtomicRef<O>& _reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> reference(_reference);
	_ptr = reference._ptr;
	_ref = reference;
}

template <class T>
template <class O>
Atomic< Ptr<T> >::Atomic(const T* pointer, const AtomicRef<O>& reference) : _ptr(const_cast<T*>(pointer)), _ref(reference)
{
}

template <class T>
Atomic< Ptr<T> >::Atomic(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
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
SLIB_INLINE Atomic< Ptr<T> >::Atomic(const T* pointer, const WeakRef<O>& weak) : _ptr(const_cast<T*>(pointer)), _ref(weak._weak)
{
}

template <class T>
Atomic< Ptr<T> >::Atomic(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o._ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
Atomic< Ptr<T> >::Atomic(const AtomicWeakRef<O>& weak)
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
Atomic< Ptr<T> >::Atomic(const T* pointer, const AtomicWeakRef<O>& weak) : _ptr(const_cast<T*>(pointer)), _ref(weak._weak)
{
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
void Atomic< Ptr<T> >::setPointer(const T* pointer)
{
	_replace(const_cast<T*>(pointer), Ref<Referable>::null());
}

template <class T>
void Atomic< Ptr<T> >::setRef(const Ref<T>& reference)
{
	_replace(reference._ptr, Ref<Referable>::from(reference));
}

template <class T>
template <class O>
void Atomic< Ptr<T> >::setRef(const Ref<O>& reference)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	_replace(reference._ptr, Ref<Referable>::from(reference));
}

template <class T>
void Atomic< Ptr<T> >::setRef(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
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
void Atomic< Ptr<T> >::setWeak(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
template <class O>
void Atomic< Ptr<T> >::setWeak(const WeakRef<O>& weak)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
void Atomic< Ptr<T> >::setWeak(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
template <class O>
void Atomic< Ptr<T> >::setWeak(const AtomicWeakRef<O>& weak)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
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
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const T* pointer)
{
	_replace(const_cast<T*>(pointer), Ref<Referable>::null());
	return *this;
}

template <class T>
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const Ref<T>& reference)
{
	_replace(reference._ptr, Ref<Referable>::from(reference));
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
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicRef<T>& _reference)
{
	Ref<T> reference(_reference);
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
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
template <class O>
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const WeakRef<O>& weak)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
template <class O>
AtomicPtr<T>& Atomic< Ptr<T> >::operator=(const AtomicWeakRef<O>& weak)
{
	SLIB_TRY_CONVERT_TYPE(O*, T*)
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o._ptr, Ref<Referable>::from(o));
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

SLIB_NAMESPACE_END

#endif
