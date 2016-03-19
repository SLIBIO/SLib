#ifndef CHECKHEADER_SLIB_CORE_POINTER
#define CHECKHEADER_SLIB_CORE_POINTER

#include "definition.h"

#include "reference.h"

SLIB_NAMESPACE_BEGIN

template <class T>
class SafePtr;

template <class T>
class SLIB_EXPORT Ptr
{
public:
	T* ptr;
	Ref<Referable> ref;

public:
	Ptr();
	
	Ptr(Ptr<T>&& other);

	Ptr(const Ptr<T>& other);
	
	template <class O>
	Ptr(Ptr<O>&& other);

	template <class O>
	Ptr(const Ptr<O>& other);
	
	Ptr(SafePtr<T>&& other);
	
	Ptr(const SafePtr<T>& other);
	
	template <class O>
	Ptr(SafePtr<O>&& other);
	
	template <class O>
	Ptr(const SafePtr<O>& other);
	
	Ptr(const T* pointer);
	
	Ptr(const Ref<T>& reference);

	template <class O>
	Ptr(const Ref<O>& reference);
	
	template <class O>
	Ptr(const T* pointer, const Ref<O>& reference);
	
	Ptr(const SafeRef<T>& reference);

	template <class O>
	Ptr(const SafeRef<O>& reference);
	
	template <class O>
	Ptr(const T* pointer, const SafeRef<O>& reference);
	
	Ptr(const WeakRef<T>& weak);
	
	template <class O>
	Ptr(const WeakRef<O>& weak);
	
	template <class O>
	Ptr(const T* pointer, const WeakRef<O>& weak);
	
	Ptr(const SafeWeakRef<T>& weak);
	
	template <class O>
	Ptr(const SafeWeakRef<O>& weak);
	
	template <class O>
	Ptr(const T* pointer, const SafeWeakRef<O>& weak);
	
public:
	static Ptr<T> fromPointer(const T* pointer);
	
	static Ptr<T> fromRef(const Ref<T>& reference);
	
	template <class O>
	static Ptr<T> fromRef(const Ref<O>& reference);
	
	static Ptr<T> fromRef(const SafeRef<T>& reference);
	
	template <class O>
	static Ptr<T> fromRef(const SafeRef<O>& reference);
	
	static Ptr<T> fromWeak(const WeakRef<T>& weak);
	
	template <class O>
	static Ptr<T> fromWeak(const WeakRef<O>& weak);
	
	static Ptr<T> fromWeak(const SafeWeakRef<T>& weak);
	
	template <class O>
	static Ptr<T> fromWeak(const SafeWeakRef<O>& weak);
	
public:
	static const Ptr<T>& null();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();

	sl_bool isWeak() const;
	
	Ptr<T> lock() const;
	
	template <class O>
	static const Ptr<T>& from(const Ptr<O>& other);
	
public:
	void setPointer(const T* pointer);
	
	void setRef(const Ref<T>& reference);
	
	template <class O>
	void setRef(const Ref<O>& reference);
	
	void setRef(const SafeRef<T>& reference);
	
	template <class O>
	void setRef(const SafeRef<O>& reference);
	
	void setWeak(const WeakRef<T>& weak);
	
	template <class O>
	void setWeak(const WeakRef<O>& weak);
	
	void setWeak(const SafeWeakRef<T>& weak);
	
	template <class O>
	void setWeak(const SafeWeakRef<O>& weak);
	
public:
	Ptr<T>& operator=(Ptr<T>&& other);
	
	Ptr<T>& operator=(const Ptr<T>& other);
	
	template <class O>
	Ptr<T>& operator=(Ptr<O>&& other);

	template <class O>
	Ptr<T>& operator=(const Ptr<O>& other);
	
	Ptr<T>& operator=(SafePtr<T>&& other);
	
	Ptr<T>& operator=(const SafePtr<T>& other);
	
	template <class O>
	Ptr<T>& operator=(SafePtr<O>&& other);
	
	template <class O>
	Ptr<T>& operator=(const SafePtr<O>& other);
	
	Ptr<T>& operator=(const T* pointer);

	Ptr<T>& operator=(const Ref<T>& reference);
	
	template <class O>
	Ptr<T>& operator=(const Ref<O>& reference);
	
	Ptr<T>& operator=(const SafeRef<T>& reference);
	
	template <class O>
	Ptr<T>& operator=(const SafeRef<O>& reference);
	
	Ptr<T>& operator=(const WeakRef<T>& weak);
	
	template <class O>
	Ptr<T>& operator=(const WeakRef<O>& weak);
	
	Ptr<T>& operator=(const SafeWeakRef<T>& weak);
	
	template <class O>
	Ptr<T>& operator=(const SafeWeakRef<O>& weak);
	
	
	sl_bool operator==(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const Ptr<O>& other) const;

	sl_bool operator==(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const SafePtr<O>& other) const;
	
	sl_bool operator!=(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const Ptr<O>& other) const;
	
	sl_bool operator!=(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const SafePtr<O>& other) const;
	
	
	T& operator*() const;
	
	T* operator->() const;

	
public:
	void _move_init(void* _other);
	
	void _move_init(void* _other, T* dummy);
	
	void _move_assign(void* _other);
	
	void _move_assign(void* _other, T* dummy);
	
};


template <class T>
class SLIB_EXPORT SafePtr {
public:
	T* _ptr;
	Ref<Referable> _ref;
private:
	SpinLock m_lock;
	
public:
	SafePtr();
	
	SafePtr(SafePtr<T>&& other);
	
	SafePtr(const SafePtr<T>& other);
	
	template <class O>
	SafePtr(SafePtr<O>&& other);
	
	template <class O>
	SafePtr(const SafePtr<O>& other);
	
	SafePtr(Ptr<T>&& other);
	
	SafePtr(const Ptr<T>& other);
	
	template <class O>
	SafePtr(Ptr<O>&& other);

	template <class O>
	SafePtr(const Ptr<O>& other);
	
	SafePtr(const T* pointer);
	
	SafePtr(const Ref<T>& reference);
	
	template <class O>
	SafePtr(const Ref<O>& reference);
	
	template <class O>
	SafePtr(const T* pointer, const Ref<O>& reference);
	
	SafePtr(const SafeRef<T>& reference);
	
	template <class O>
	SafePtr(const SafeRef<O>& reference);
	
	template <class O>
	SafePtr(const T* pointer, const SafeRef<O>& reference);
	
	SafePtr(const WeakRef<T>& weak);
	
	template <class O>
	SafePtr(const WeakRef<O>& weak);
	
	template <class O>
	SafePtr(const T* pointer, const WeakRef<O>& weak);
	
	SafePtr(const SafeWeakRef<T>& weak);
	
	template <class O>
	SafePtr(const SafeWeakRef<O>& weak);
	
	template <class O>
	SafePtr(const T* pointer, const SafeWeakRef<O>& weak);
	
public:
	static const SafePtr<T>& null();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();
	
	Ptr<T> lock() const;
	
	template <class O>
	static const SafePtr<T>& from(const SafePtr<O>& other);
	
public:
	void setPointer(const T* pointer);
	
	void setRef(const Ref<T>& reference);
	
	template <class O>
	void setRef(const Ref<O>& reference);
	
	void setRef(const SafeRef<T>& reference);
	
	template <class O>
	void setRef(const SafeRef<O>& reference);
	
	void setWeak(const WeakRef<T>& weak);
	
	template <class O>
	void setWeak(const WeakRef<O>& weak);
	
	void setWeak(const SafeWeakRef<T>& weak);
	
	template <class O>
	void setWeak(const SafeWeakRef<O>& weak);
	
public:
	SafePtr<T>& operator=(SafePtr<T>&& other);
	
	SafePtr<T>& operator=(const SafePtr<T>& other);
	
	template <class O>
	SafePtr<T>& operator=(SafePtr<O>&& other);

	template <class O>
	SafePtr<T>& operator=(const SafePtr<O>& other);
	
	SafePtr<T>& operator=(Ptr<T>&& other);

	SafePtr<T>& operator=(const Ptr<T>& other);
	
	template <class O>
	SafePtr<T>& operator=(Ptr<O>&& other);

	template <class O>
	SafePtr<T>& operator=(const Ptr<O>& other);
	
	SafePtr<T>& operator=(const T* pointer);
	
	SafePtr<T>& operator=(const Ref<T>& reference);
	
	template <class O>
	SafePtr<T>& operator=(const Ref<O>& reference);
	
	SafePtr<T>& operator=(const SafeRef<T>& reference);
	
	template <class O>
	SafePtr<T>& operator=(const SafeRef<O>& reference);
	
	SafePtr<T>& operator=(const WeakRef<T>& weak);
	
	template <class O>
	SafePtr<T>& operator=(const WeakRef<O>& weak);
	
	SafePtr<T>& operator=(const SafeWeakRef<T>& weak);
	
	template <class O>
	SafePtr<T>& operator=(const SafeWeakRef<O>& weak);
	
	
	sl_bool operator==(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const SafePtr<O>& other) const;
	
	sl_bool operator==(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const Ptr<O>& other) const;
	
	sl_bool operator!=(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const SafePtr<O>& other) const;
	
	sl_bool operator!=(const Ptr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const Ptr<O>& other) const;
	
public:
	T* _retain(Ref<Referable>& reference) const;
	
	void _replace(T* pointer, const Ref<Referable>& reference);
	
	void _move_init(void* _other);
	
	void _move_init(void* _other, T* dummy);
	
	void _move_assign(void* _other);
	
	void _move_assign(void* _other, T* dummy);

};


template <class T>
class SLIB_EXPORT PtrLocker
{
private:
	Ptr<T> m_ptr;
	
public:
	PtrLocker(const Ptr<T>& ptr);
	
	PtrLocker(const SafePtr<T>& ptr);
	
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

struct _Ptr_Const
{
	void* ptr;
	void* ref;
	sl_int32 lock;
};

extern const _Ptr_Const _Ptr_Null;

template <class T>
SLIB_INLINE Ptr<T>::Ptr() : ptr(sl_null)
{
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(Ptr<T>&& other)
{
	_move_init(&other);
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(const Ptr<T>& other) : ptr(other.ptr), ref(other.ref)
{
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(Ptr<O>&& other)
{
	_move_init(&other, (O*)(0));
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const Ptr<O>& other) : ptr(other.ptr), ref(other.ref)
{
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(SafePtr<T>&& other)
{
	_move_init(&other);
}

template <class T>
Ptr<T>::Ptr(const SafePtr<T>& other)
{
	ptr = other._retain(ref);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(SafePtr<O>&& other)
{
	_move_init(&other, (O*)(0));
}

template <class T>
template <class O>
Ptr<T>::Ptr(const SafePtr<O>& other)
{
	ptr = other._retain(ref);
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(const T* pointer) : ptr((T*)pointer)
{
}

template <class T>
SLIB_INLINE Ptr<T>::Ptr(const Ref<T>& reference) : ptr(reference.ptr), ref(reference)
{
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const Ref<O>& reference) : ptr(reference.ptr), ref(reference)
{
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const T* pointer, const Ref<O>& reference) : ptr((T*)pointer), ref(reference)
{
}

template <class T>
Ptr<T>::Ptr(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
template <class O>
Ptr<T>::Ptr(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
template <class O>
Ptr<T>::Ptr(const T* pointer, const SafeRef<O>& reference) : ptr((T*)pointer), ref(reference)
{
}

template <class T>
Ptr<T>::Ptr(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
Ptr<T>::Ptr(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const T* pointer, const WeakRef<O>& weak) : ptr((T*)pointer), ref(weak._weak)
{
}

template <class T>
Ptr<T>::Ptr(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
Ptr<T>::Ptr(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
Ptr<T>::Ptr(const T* pointer, const SafeWeakRef<O>& weak) : ptr((T*)pointer), ref(weak._weak)
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
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const SafeRef<T>& reference)
{
	return Ptr<T>(reference);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromRef(const SafeRef<O>& reference)
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
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const SafeWeakRef<T>& weak)
{
	return Ptr<T>(weak);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T> Ptr<T>::fromWeak(const SafeWeakRef<O>& weak)
{
	return Ptr<T>(weak);
}

template <class T>
SLIB_INLINE const Ptr<T>& Ptr<T>::null()
{
	return *((Ptr<T>*)((void*)(&_Ptr_Null)));
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::isNull() const
{
	return ptr == sl_null;
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::isNotNull() const
{
	return ptr != sl_null;
}

template <class T>
SLIB_INLINE void Ptr<T>::setNull()
{
	ptr = sl_null;
	ref.setNull();
}

template <class T>
sl_bool Ptr<T>::isWeak() const
{
	return (CWeakRef::checkInstance(ref.ptr));
}

template <class T>
Ptr<T> Ptr<T>::lock() const
{
	if (ptr) {
		Referable* obj = ref.ptr;
		if (CWeakRef::checkInstance(obj)) {
			CWeakRef* weak = (CWeakRef*)obj;
			Ref<Referable> r(weak->lock());
			if (r.isNotNull()) {
				return Ptr<T>(ptr, r);
			}
		} else {
			return *this;
		}
	}
	return Ptr<T>::null();
}

template <class T>
template <class O>
SLIB_INLINE const Ptr<T>& Ptr<T>::from(const Ptr<O>& other)
{
	return *((const Ptr<T>*)((void*)(&other)));
}


template <class T>
SLIB_INLINE void Ptr<T>::setPointer(const T* pointer)
{
	ptr = (T*)pointer;
	ref.setNull();
}

template <class T>
SLIB_INLINE void Ptr<T>::setRef(const Ref<T>& reference)
{
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
template <class O>
SLIB_INLINE void Ptr<T>::setRef(const Ref<O>& reference)
{
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
void Ptr<T>::setRef(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
template <class O>
void Ptr<T>::setRef(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
}

template <class T>
void Ptr<T>::setWeak(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
void Ptr<T>::setWeak(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
void Ptr<T>::setWeak(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
template <class O>
void Ptr<T>::setWeak(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(Ptr<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const Ptr<T>& other)
{
	ptr = other.ptr;
	ref = other.ref;
	return *this;
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(Ptr<O>&& other)
{
	_move_assign(&other, (O*)(0));
	return *this;
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const Ptr<O>& other)
{
	ptr = other.ptr;
	ref = other.ref;
	return *this;
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(SafePtr<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const SafePtr<T>& other)
{
	ptr = other._retain(ref);
	return *this;
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(SafePtr<O>&& other)
{
	_move_assign(&other, (O*)(0));
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const SafePtr<O>& other)
{
	ptr = other._retain(ref);
	return *this;
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const T* pointer)
{
	ptr = (T*)pointer;
	ref.setNull();
	return *this;
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const Ref<T>& reference)
{
	ptr = reference.ptr;
	ref = reference;
	return *this;
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const Ref<O>& reference)
{
	ptr = reference.ptr;
	ref = reference;
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	ptr = reference.ptr;
	ref = reference;
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
	return *this;
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
	return *this;
}

template <class T>
template <class O>
Ptr<T>& Ptr<T>::operator=(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		ptr = o.ptr;
		ref = weak._weak;
	} else {
		ptr = sl_null;
	}
	return *this;
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<T>& other) const
{
	return ptr == other.ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator==(const Ptr<O>& other) const
{
	return ptr == other.ptr;
}


template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator==(const SafePtr<T>& other) const
{
	return ptr == other._ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator==(const SafePtr<O>& other) const
{
	return ptr == other._ptr;
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<T>& other) const
{
	return ptr != other.ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const Ptr<O>& other) const
{
	return ptr != other.ptr;
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const SafePtr<T>& other) const
{
	return ptr != other._ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const SafePtr<O>& other) const
{
	return ptr != other._ptr;
}

template <class T>
SLIB_INLINE T& Ptr<T>::operator*() const
{
	return *ptr;
}

template <class T>
SLIB_INLINE T* Ptr<T>::operator->() const
{
	return ptr;
}

template <class T>
SLIB_INLINE void Ptr<T>::_move_init(void* _other)
{
	Ptr<T>& other = *((Ptr<T>*)_other);
	ptr = other.ptr;
	ref._move_init(&(other.ref));
}

template <class T>
SLIB_INLINE void Ptr<T>::_move_init(void* _other, T* dummy)
{
	Ptr<T>& other = *((Ptr<T>*)_other);
	ptr = other.ptr;
	ref._move_init(&(other.ref));
}

template <class T>
SLIB_INLINE void Ptr<T>::_move_assign(void* _other)
{
	if ((void*)this != _other) {
		Ptr<T>& other = *((Ptr<T>*)_other);
		ptr = other.ptr;
		ref._move_assign(&(other.ref));
	}
}

template <class T>
SLIB_INLINE void Ptr<T>::_move_assign(void* _other, T* dummy)
{
	if ((void*)this != _other) {
		Ptr<T>& other = *((Ptr<T>*)_other);
		ptr = other.ptr;
		ref._move_assign(&(other.ref));
	}
}


template <class T>
SLIB_INLINE SafePtr<T>::SafePtr() : _ptr(sl_null)
{
}

template <class T>
SLIB_INLINE SafePtr<T>::SafePtr(SafePtr<T>&& other)
{
	_move_init(&other);
}

template <class T>
SafePtr<T>::SafePtr(const SafePtr<T>& other)
{
	_ptr = other._retain(_ref);
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(SafePtr<O>&& other)
{
	_move_init(&other, (O*)(0));
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const SafePtr<O>& other)
{
	_ptr = other._retain(_ref);
}

template <class T>
SLIB_INLINE SafePtr<T>::SafePtr(Ptr<T>&& other)
{
	_move_init(&other);
}

template <class T>
SLIB_INLINE SafePtr<T>::SafePtr(const Ptr<T>& other) : _ptr(other.ptr), _ref(other.ref)
{
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(Ptr<O>&& other)
{
	_move_init(&other, (O*)(0));
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(const Ptr<O>& other) : _ptr(other.ptr), _ref(other.ref)
{
}

template <class T>
SLIB_INLINE SafePtr<T>::SafePtr(const T* pointer) : _ptr((T*)pointer)
{
}

template <class T>
SLIB_INLINE SafePtr<T>::SafePtr(const Ref<T>& reference) : _ptr(reference.ptr), _ref(reference)
{
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(const Ref<O>& reference) : _ptr(reference.ptr), _ref(reference)
{
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(const T* pointer, const Ref<O>& reference) : _ptr((T*)pointer), _ref(reference)
{
}

template <class T>
SafePtr<T>::SafePtr(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_ptr = reference.ptr;
	_ref = reference;
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	_ptr = reference.ptr;
	_ref = reference;
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const T* pointer, const SafeRef<O>& reference) : _ptr((T*)pointer), _ref(reference)
{
}

template <class T>
SafePtr<T>::SafePtr(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o.ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_ptr = o.ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
SLIB_INLINE SafePtr<T>::SafePtr(const T* pointer, const WeakRef<O>& weak) : _ptr((T*)pointer), _ref(weak._weak)
{
}

template <class T>
SafePtr<T>::SafePtr(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_ptr = o.ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_ptr = o.ptr;
		_ref = weak._weak;
	} else {
		_ptr = sl_null;
	}
}

template <class T>
template <class O>
SafePtr<T>::SafePtr(const T* pointer, const SafeWeakRef<O>& weak) : _ptr((T*)pointer), _ref(weak._weak)
{
}

template <class T>
SLIB_INLINE const SafePtr<T>& SafePtr<T>::null()
{
	return *((SafePtr<T>*)((void*)(&_Ptr_Null)));
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::isNull() const
{
	return _ptr == sl_null;
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::isNotNull() const
{
	return _ptr != sl_null;
}

template <class T>
void SafePtr<T>::setNull()
{
	_replace(sl_null, Ref<Referable>::null());
}

template <class T>
Ptr<T> SafePtr<T>::lock() const
{
	Ptr<T> p(*this);
	return p.lock();
}

template <class T>
template <class O>
SLIB_INLINE const SafePtr<T>& SafePtr<T>::from(const SafePtr<O>& other)
{
	return *((const SafePtr<T>*)((void*)(&other)));
}

template <class T>
void SafePtr<T>::setPointer(const T* pointer)
{
	_replace((T*)pointer, Ref<Referable>::null());
}

template <class T>
void SafePtr<T>::setRef(const Ref<T>& reference)
{
	_replace(reference.ptr, Ref<Referable>::from(reference));
}

template <class T>
template <class O>
void SafePtr<T>::setRef(const Ref<O>& reference)
{
	_replace(reference.ptr, Ref<Referable>::from(reference));
}

template <class T>
void SafePtr<T>::setRef(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_replace(reference.ptr, Ref<Referable>::from(reference));
}

template <class T>
template <class O>
void SafePtr<T>::setRef(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	_replace(reference.ptr, Ref<Referable>::from(reference));
}

template <class T>
void SafePtr<T>::setWeak(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
template <class O>
void SafePtr<T>::setWeak(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
void SafePtr<T>::setWeak(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
template <class O>
void SafePtr<T>::setWeak(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(SafePtr<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const SafePtr<T>& other)
{
	Ref<Referable> reference;
	T* pointer = other._retain(reference);
	_replace(pointer, reference);
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(SafePtr<O>&& other)
{
	_move_assign(&other, (O*)(0));
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const SafePtr<O>& other)
{
	Ref<Referable> reference;
	T* pointer = other._retain(reference);
	_replace(pointer, reference);
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(Ptr<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const Ptr<T>& other)
{
	_replace(other.ptr, other.ref);
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(Ptr<O>&& other)
{
	_move_assign(&other, (O*)(0));
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const Ptr<O>& other)
{
	_replace(other.ptr, other.ref);
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const T* pointer)
{
	_replace((T*)pointer, Ref<Referable>::null());
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const Ref<T>& reference)
{
	_replace(reference.ptr, Ref<Referable>::from(reference));
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const Ref<O>& reference)
{
	_replace(reference.ptr, Ref<Referable>::from(reference));
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const SafeRef<T>& _reference)
{
	Ref<T> reference(_reference);
	_replace(reference.ptr, Ref<Referable>::from(reference));
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const SafeRef<O>& _reference)
{
	Ref<O> reference(_reference);
	_replace(reference.ptr, Ref<Referable>::from(reference));
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const WeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const WeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
SafePtr<T>& SafePtr<T>::operator=(const SafeWeakRef<T>& weak)
{
	Ref<T> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
template <class O>
SafePtr<T>& SafePtr<T>::operator=(const SafeWeakRef<O>& weak)
{
	Ref<O> o(weak);
	if (o.isNotNull()) {
		_replace(o.ptr, Ref<Referable>::from(o));
	} else {
		_replace(sl_null, Ref<Referable>::null());
	}
	return *this;
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::operator==(const SafePtr<T>& other) const
{
	return _ptr == other._ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool SafePtr<T>::operator==(const SafePtr<O>& other) const
{
	return _ptr == other._ptr;
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::operator==(const Ptr<T>& other) const
{
	return _ptr == other.ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool SafePtr<T>::operator==(const Ptr<O>& other) const
{
	return _ptr == other.ptr;
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::operator!=(const SafePtr<T>& other) const
{
	return _ptr != other._ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool SafePtr<T>::operator!=(const SafePtr<O>& other) const
{
	return _ptr != other._ptr;
}

template <class T>
SLIB_INLINE sl_bool SafePtr<T>::operator!=(const Ptr<T>& other) const
{
	return _ptr != other.ptr;
}

template <class T>
template <class O>
SLIB_INLINE sl_bool SafePtr<T>::operator!=(const Ptr<O>& other) const
{
	return _ptr != other.ptr;
}

template <class T>
SLIB_INLINE T* SafePtr<T>::_retain(Ref<Referable>& reference) const
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
SLIB_INLINE void SafePtr<T>::_replace(T* pointer, const Ref<Referable>& reference)
{
	Referable* refOld;
	{
		SpinLocker lock(&m_lock);
		_ptr = pointer;
		refOld = _ref.ptr;
		new (&_ref) Ref<Referable>(reference);
	}
	if (refOld) {
		refOld->decreaseReference();
	}
}

template <class T>
SLIB_INLINE void SafePtr<T>::_move_init(void* _other)
{
	SafePtr<T>& other = *((SafePtr<T>*)(_other));
	_ptr = other._ptr;
	_ref._move_init(&(other._ref));
}

template <class T>
SLIB_INLINE void SafePtr<T>::_move_init(void* _other, T* dummy)
{
	SafePtr<T>& other = *((SafePtr<T>*)(_other));
	_ptr = other._ptr;
	_ref._move_init(&(other._ref));
}

template <class T>
SLIB_INLINE void SafePtr<T>::_move_assign(void* _other)
{
	if ((void*)this != _other) {
		SafePtr<T>& other = *((SafePtr<T>*)(_other));
		Referable* refOld;
		{
			SpinLocker lock(&m_lock);
			_ptr = other._ptr;
			refOld = _ref.ptr;
			_ref._move_init(&(other._ref));
		}
		if (refOld) {
			refOld->decreaseReference();
		}
	}
}

template <class T>
SLIB_INLINE void SafePtr<T>::_move_assign(void* _other, T* dummy)
{
	if ((void*)this != _other) {
		SafePtr<T>& other = *((SafePtr<T>*)(_other));
		Referable* refOld;
		{
			SpinLocker lock(&m_lock);
			_ptr = other._ptr;
			refOld = _ref.ptr;
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
SLIB_INLINE PtrLocker<T>::PtrLocker(const SafePtr<T>& ptr) : m_ptr(ptr.lock())
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
	return m_ptr.ptr;
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
	return *(m_ptr.ptr);
}

template <class T>
SLIB_INLINE T* PtrLocker<T>::operator->() const
{
	return m_ptr.ptr;
}

SLIB_NAMESPACE_END

#endif
