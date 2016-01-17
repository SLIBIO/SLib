#ifndef CHECKHEADER_SLIB_CORE_POINTER
#define CHECKHEADER_SLIB_CORE_POINTER

#include "definition.h"

#include "reference.h"

SLIB_NAMESPACE_BEGIN

struct _Ptr_Const
{
	void* pointer;
	void* reference;
	sl_int32 lock;
};
extern const _Ptr_Const _Ptr_Null;


template <class T>
class SafePtr;

template <class T>
class SLIB_EXPORT Ptr {
private:
	T* m_pointer;
	Ref<Referable> m_reference;

public:
	SLIB_INLINE Ptr()
	{
		m_pointer = sl_null;
	}
	
	SLIB_INLINE Ptr(Ptr<T>&& other)
	{
		_move_init(&other);
	}

	SLIB_INLINE Ptr(const Ptr<T>& other) : m_pointer(other.m_pointer), m_reference(other.m_reference)
	{
	}
	
	template <class O>
	SLIB_INLINE Ptr(Ptr<O>&& other)
	{
		_move_init(&other, (O*)(0));
	}

	template <class O>
	SLIB_INLINE Ptr(const Ptr<O>& other) : m_pointer(other.get()), m_reference(other.getReference())
	{
	}
	
	SLIB_INLINE Ptr(SafePtr<T>&& other)
	{
		_move_init(&other);
	}
	
	Ptr(const SafePtr<T>& other);
	
	template <class O>
	SLIB_INLINE Ptr(SafePtr<O>&& other)
	{
		_move_init(&other, (O*)(0));
	}
	
	template <class O>
	Ptr(const SafePtr<O>& other);
	
	SLIB_INLINE Ptr(const T* pointer)
	{
		m_pointer = (T*)pointer;
	}
	
	SLIB_INLINE Ptr(const Ref<T>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
	}

	template <class O>
	SLIB_INLINE Ptr(const Ref<O>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE Ptr(const T* pointer, const Ref<O>& reference)
	{
		m_pointer = (T*)pointer;
		m_reference = reference;
	}
	
	SLIB_INLINE Ptr(const SafeRef<T>& _reference)
	{
		Ref<T> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
	}

	template <class O>
	SLIB_INLINE Ptr(const SafeRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE Ptr(const T* pointer, const SafeRef<O>& reference)
	{
		m_pointer = (T*)pointer;
		m_reference = reference;
	}
	
	Ptr(const WeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}

	template <class O>
	Ptr(const WeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}

	template <class O>
	SLIB_INLINE Ptr(const T* pointer, const WeakRef<O>& weak)
	{
		m_pointer = (T*)pointer;
		m_reference = weak.getWeakRef();
	}
	
	Ptr(const SafeWeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}

	template <class O>
	Ptr(const SafeWeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}
	
	template <class O>
	SLIB_INLINE Ptr(const T* pointer, const SafeWeakRef<O>& weak)
	{
		m_pointer = (T*)pointer;
		m_reference = weak.getWeakRef();
	}

public:
	SLIB_INLINE static const Ptr<T>& null()
	{
		return *((Ptr<T>*)((void*)(&_Ptr_Null)));
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_pointer == sl_null;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_pointer != sl_null;
	}
	
	SLIB_INLINE void setNull()
	{
		m_pointer = sl_null;
		m_reference.setNull();
	}

public:
	SLIB_INLINE T* get() const
	{
		return m_pointer;
	}
	
	SLIB_INLINE const Ref<Referable>& getReference() const
	{
		return m_reference;
	}
	
	SLIB_INLINE sl_bool isWeak() const
	{
		return (CWeakRef::checkInstance(m_reference));
	}
	
	Ptr<T> lock() const
	{
		if (m_pointer) {
			Referable* ref = m_reference.get();
			if (CWeakRef::checkInstance(ref)) {
				CWeakRef* weak = (CWeakRef*)ref;
				Ref<Referable> r(weak->lock());
				if (r.isNotNull()) {
					return Ptr<T>(m_pointer, r);
				}
			} else {
				return *this;
			}
		}
		return Ptr<T>::null();
	}
	
	template <class O>
	SLIB_INLINE static const Ptr<T>& from(const Ptr<O>& other)
	{
		return *((const Ptr<T>*)((void*)(&other)));
	}
	
public:
	SLIB_INLINE Ptr<T>& operator=(Ptr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	SLIB_INLINE Ptr<T>& operator=(const Ptr<T>& other)
	{
		m_pointer = other.m_pointer;
		m_reference = other.m_reference;
		return *this;
	}
	
	template <class O>
	SLIB_INLINE Ptr<T>& operator=(Ptr<O>&& other)
	{
		_move_assign(&other, (O*)(0));
		return *this;
	}

	template <class O>
	SLIB_INLINE Ptr<T>& operator=(const Ptr<O>& other)
	{
		m_pointer = other.get();
		m_reference = other.getReference();
		return *this;
	}
	
	SLIB_INLINE Ptr<T>& operator=(SafePtr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	Ptr<T>& operator=(const SafePtr<T>& other);
	
	template <class O>
	SLIB_INLINE Ptr<T>& operator=(SafePtr<O>&& other)
	{
		_move_assign(&other, (O*)(0));
		return *this;
	}
	
	template <class O>
	Ptr<T>& operator=(const SafePtr<O>& other);
	
	SLIB_INLINE Ptr<T>& operator=(const T* pointer)
	{
		m_pointer = pointer;
		m_reference.setNull();
		return *this;
	}

	SLIB_INLINE Ptr<T>& operator=(const Ref<T>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
		return *this;
	}

	template <class O>
	SLIB_INLINE Ptr<T>& operator=(const Ref<O>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
		return *this;
	}

	SLIB_INLINE Ptr<T>& operator=(const SafeRef<T>& _reference)
	{
		Ref<T> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
		return *this;
	}

	template <class O>
	SLIB_INLINE Ptr<T>& operator=(const SafeRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
		return *this;
	}

	Ptr<T>& operator=(const WeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
		return *this;
	}

	template <class O>
	Ptr<T>& operator=(const WeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
		return *this;
	}
	
	Ptr<T>& operator=(const SafeWeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
		return *this;
	}
	
	template <class O>
	Ptr<T>& operator=(const SafeWeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
		return *this;
	}

public:
	SLIB_INLINE sl_bool operator==(const Ptr<T>& other) const
	{
		return m_pointer == other.m_pointer;
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator==(const Ptr<O>& other) const
	{
		return m_pointer == other.get();
	}

	sl_bool operator==(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator==(const SafePtr<O>& other) const;
	
public:
	SLIB_INLINE sl_bool operator!=(const Ptr<T>& other) const
	{
		return m_pointer != other.m_pointer;
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator!=(const Ptr<O>& other) const
	{
		return m_pointer != other.get();
	}
	
	sl_bool operator!=(const SafePtr<T>& other) const;
	
	template <class O>
	sl_bool operator!=(const SafePtr<O>& other) const;
	
public:
	SLIB_INLINE T& operator*()
	{
		return *m_pointer;
	}
	
	SLIB_INLINE T* operator->()
	{
		return m_pointer;
	}

public:
	SLIB_INLINE void _move_init(void* _other)
	{
		Ptr<T>& other = *((Ptr<T>*)_other);
		m_pointer = other.m_pointer;
		m_reference._move_init(&(other.m_reference));
	}
	
	SLIB_INLINE void _move_init(void* _other, T* dummy)
	{
		Ptr<T>& other = *((Ptr<T>*)_other);
		m_pointer = other.m_pointer;
		m_reference._move_init(&(other.m_reference));
	}
	
	SLIB_INLINE void _move_assign(void* _other)
	{
		if ((void*)this != _other) {
			Ptr<T>& other = *((Ptr<T>*)_other);
			m_pointer = other.m_pointer;
			m_reference._move_assign(&(other.m_reference));
		}
	}
	
	SLIB_INLINE void _move_assign(void* _other, T* dummy)
	{
		if ((void*)this != _other) {
			Ptr<T>& other = *((Ptr<T>*)_other);
			m_pointer = other.m_pointer;
			m_reference._move_assign(&(other.m_reference));
		}
	}
};


template <class T>
class SLIB_EXPORT SafePtr {
private:
	T* m_pointer;
	Ref<Referable> m_reference;
	SpinLock m_lock;
	
public:
	SLIB_INLINE SafePtr()
	{
		m_pointer = sl_null;
	}
	
	SLIB_INLINE SafePtr(SafePtr<T>&& other)
	{
		_move_init(&other);
	}
	
	SLIB_INLINE SafePtr(const SafePtr<T>& other)
	{
		m_pointer = other._retain(m_reference);
	}
	
	template <class O>
	SLIB_INLINE SafePtr(SafePtr<O>&& other)
	{
		_move_init(&other, (O*)(0));
	}

	template <class O>
	SLIB_INLINE SafePtr(const SafePtr<O>& other)
	{
		m_pointer = other._retain(m_reference);
	}
	
	SLIB_INLINE SafePtr(Ptr<T>&& other)
	{
		_move_init(&other);
	}
	
	SLIB_INLINE SafePtr(const Ptr<T>& other) : m_pointer(other.m_pointer), m_reference(other.m_reference)
	{
	}
	
	template <class O>
	SLIB_INLINE SafePtr(Ptr<O>&& other)
	{
		_move_init(&other, (O*)(0));
	}

	template <class O>
	SLIB_INLINE SafePtr(const Ptr<O>& other) : m_pointer(other.get()), m_reference(other.getReference())
	{
	}
	
	SLIB_INLINE SafePtr(const T* pointer)
	{
		m_pointer = (T*)pointer;
	}
	
	SLIB_INLINE SafePtr(const Ref<T>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const Ref<O>& reference)
	{
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const T* pointer, const Ref<O>& reference)
	{
		m_pointer = (T*)pointer;
		m_reference = reference;
	}
	
	SLIB_INLINE SafePtr(const SafeRef<T>& _reference)
	{
		Ref<T> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const SafeRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		m_pointer = reference.get();
		m_reference = reference;
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const T* pointer, const SafeRef<O>& reference)
	{
		m_pointer = (T*)pointer;
		m_reference = reference;
	}
	
	SafePtr(const WeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}
	
	template <class O>
	SafePtr(const WeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const T* pointer, const WeakRef<O>& weak)
	{
		m_pointer = (T*)pointer;
		m_reference = weak.getWeakRef();
	}
	
	SafePtr(const SafeWeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}
	
	template <class O>
	SafePtr(const SafeWeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			m_pointer = o.get();
			m_reference = weak.getWeakRef();
		} else {
			m_pointer = sl_null;
		}
	}
	
	template <class O>
	SLIB_INLINE SafePtr(const T* pointer, const SafeWeakRef<O>& weak)
	{
		m_pointer = (T*)pointer;
		m_reference = weak.getWeakRef();
	}
	
public:
	SLIB_INLINE static const SafePtr<T>& null()
	{
		return *((SafePtr<T>*)((void*)(&_Ptr_Null)));
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_pointer == sl_null;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_pointer != sl_null;
	}
	
	SLIB_INLINE void setNull()
	{
		_replace(sl_null, Ref<Referable>::null());
	}
	
public:
	SLIB_INLINE Ptr<T> lock() const
	{
		Ptr<T> p(*this);
		return p.lock();
	}
	
	template <class O>
	SLIB_INLINE static const SafePtr<T>& from(const SafePtr<O>& other)
	{
		return *((const SafePtr<T>*)((void*)(&other)));
	}
	
public:
	SLIB_INLINE SafePtr<T>& operator=(SafePtr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	SLIB_INLINE SafePtr<T>& operator=(const SafePtr<T>& other)
	{
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(SafePtr<O>&& other)
	{
		_move_assign(&other, (O*)(0));
		return *this;
	}

	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(const SafePtr<O>& other)
	{
		Ref<Referable> reference;
		T* pointer = other._retain(reference);
		_replace(pointer, reference);
		return *this;
	}
	
	SLIB_INLINE SafePtr<T>& operator=(Ptr<T>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	SLIB_INLINE SafePtr<T>& operator=(const Ptr<T>& other)
	{
		_replace(other.get(), other.getReference());
		return *this;
	}
	
	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(Ptr<O>&& other)
	{
		_move_assign(&other, (O*)(0));
		return *this;
	}

	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(const Ptr<O>& other)
	{
		_replace(other.get(), other.getReference());
		return *this;
	}
	
	SLIB_INLINE SafePtr<T>& operator=(const T* pointer)
	{
		_replace(pointer, Ref<Referable>::null());
		return *this;
	}
	
	SLIB_INLINE SafePtr<T>& operator=(const Ref<T>& reference)
	{
		_replace(reference.get(), Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(const Ref<O>& reference)
	{
		_replace(reference.get(), Ref<Referable>::from(reference));
		return *this;
	}
	
	SLIB_INLINE SafePtr<T>& operator=(const SafeRef<T>& _reference)
	{
		Ref<T> reference(_reference);
		_replace(reference.get(), Ref<Referable>::from(reference));
		return *this;
	}
	
	template <class O>
	SLIB_INLINE SafePtr<T>& operator=(const SafeRef<O>& _reference)
	{
		Ref<O> reference(_reference);
		_replace(reference.get(), Ref<Referable>::from(reference));
		return *this;
	}
	
	SafePtr<T>& operator=(const WeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			_replace(o.get(), Ref<Referable>::from(o));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
	template <class O>
	SafePtr<T>& operator=(const WeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_replace(o.get(), Ref<Referable>::from(o));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
	SafePtr<T>& operator=(const SafeWeakRef<T>& weak)
	{
		Ref<T> o(weak);
		if (o.isNotNull()) {
			_replace(o.get(), Ref<Referable>::from(o));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
	template <class O>
	SafePtr<T>& operator=(const SafeWeakRef<O>& weak)
	{
		Ref<O> o(weak);
		if (o.isNotNull()) {
			_replace(o.get(), Ref<Referable>::from(o));
		} else {
			_replace(sl_null, Ref<Referable>::null());
		}
		return *this;
	}
	
public:
	SLIB_INLINE sl_bool operator==(const SafePtr<T>& other) const
	{
		return m_pointer == other.m_pointer;
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator==(const SafePtr<O>& other) const
	{
		return m_pointer == other._getPointer();
	}
	
	SLIB_INLINE sl_bool operator==(const Ptr<T>& other) const
	{
		return m_pointer == other.get();
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator==(const Ptr<O>& other) const
	{
		return m_pointer == other.get();
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const SafePtr<T>& other) const
	{
		return m_pointer != other.m_pointer;
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator!=(const SafePtr<O>& other) const
	{
		return m_pointer != other._getPointer();
	}
	
	SLIB_INLINE sl_bool operator!=(const Ptr<T>& other) const
	{
		return m_pointer != other.get();
	}
	
	template <class O>
	SLIB_INLINE sl_bool operator!=(const Ptr<O>& other) const
	{
		return m_pointer != other.get();
	}
	
public:
	SLIB_INLINE T* _getPointer() const
	{
		return m_pointer;
	}
	
	SLIB_INLINE const Ref<Referable>& _getReference() const
	{
		return m_reference;
	}
	
	T* _retain(Ref<Referable>& reference) const
	{
		if ((void*)this == (void*)(&_Ptr_Null)) {
			return sl_null;
		} else {
			SpinLocker lock(&m_lock);
			reference = m_reference;
			return m_pointer;
		}
	}
	
	void _replace(T* pointer, const Ref<Referable>& reference)
	{
		Referable* refOld;
		{
			SpinLocker lock(&m_lock);
			m_pointer = pointer;
			refOld = m_reference.get();
			new (&m_reference) Ref<Referable>(reference);
		}
		if (refOld) {
			refOld->decreaseReference();
		}
	}
	
	SLIB_INLINE void _move_init(void* _other)
	{
		SafePtr<T>& other = *((SafePtr<T>*)(_other));
		m_pointer = other.m_pointer;
		m_reference._move_init(&(other.m_reference));
	}
	
	SLIB_INLINE void _move_init(void* _other, T* dummy)
	{
		SafePtr<T>& other = *((SafePtr<T>*)(_other));
		m_pointer = other.m_pointer;
		m_reference._move_init(&(other.m_reference));
	}
	
	SLIB_INLINE void _move_assign(void* _other)
	{
		if ((void*)this != _other) {
			SafePtr<T>& other = *((SafePtr<T>*)(_other));
			Referable* refOld;
			{
				SpinLocker lock(&m_lock);
				m_pointer = other.m_pointer;
				refOld = m_reference.get();
				m_reference._move_init(&(other.m_reference));
			}
			if (refOld) {
				refOld->decreaseReference();
			}
		}
	}
	
	SLIB_INLINE void _move_assign(void* _other, T* dummy)
	{
		if ((void*)this != _other) {
			SafePtr<T>& other = *((SafePtr<T>*)(_other));
			Referable* refOld;
			{
				SpinLocker lock(&m_lock);
				m_pointer = other.m_pointer;
				refOld = m_reference.get();
				m_reference._move_init(&(other.m_reference));
			}
			if (refOld) {
				refOld->decreaseReference();
			}
		}
	}

};


template <class T>
SLIB_INLINE Ptr<T>::Ptr(const SafePtr<T>& other)
{
	m_pointer = other._retain(m_reference);
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>::Ptr(const SafePtr<O>& other)
{
	m_pointer = other._retain(m_reference);
}

template <class T>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const SafePtr<T>& other)
{
	m_pointer = other._retain(m_reference);
	return *this;
}

template <class T>
template <class O>
SLIB_INLINE Ptr<T>& Ptr<T>::operator=(const SafePtr<O>& other)
{
	m_pointer = other._retain(m_reference);
	return *this;
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator==(const SafePtr<T>& other) const
{
	return m_pointer == other._getPointer();
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator==(const SafePtr<O>& other) const
{
	return m_pointer == other._getPointer();
}

template <class T>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const SafePtr<T>& other) const
{
	return m_pointer != other._getPointer();
}

template <class T>
template <class O>
SLIB_INLINE sl_bool Ptr<T>::operator!=(const SafePtr<O>& other) const
{
	return m_pointer != other._getPointer();
}

template <class T>
class SLIB_EXPORT PtrLocker
{
private:
	Ptr<T> m_ptr;
	
public:
	SLIB_INLINE PtrLocker(const Ptr<T>& ptr)
	{
		m_ptr = ptr.lock();
	}
	
	SLIB_INLINE PtrLocker(const SafePtr<T>& ptr)
	{
		m_ptr = ptr.lock();
	}

public:
	SLIB_INLINE void unlock()
	{
		m_ptr.setNull();
	}

	SLIB_INLINE T* get()
	{
		return m_ptr.get();
	}
	
	SLIB_INLINE sl_bool isNull()
	{
		return m_ptr.isNull();
	}

	SLIB_INLINE sl_bool isNotNull()
	{
		return m_ptr.isNotNull();
	}

	SLIB_INLINE T& operator*()
	{
		return *(m_ptr.get());
	}

	SLIB_INLINE T* operator->()
	{
		return m_ptr.get();
	}
};

SLIB_NAMESPACE_END

#endif
