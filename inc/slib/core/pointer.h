#ifndef CHECKHEADER_SLIB_CORE_POINTER
#define CHECKHEADER_SLIB_CORE_POINTER

#include "definition.h"
#include "base.h"
#include "reference.h"

SLIB_NAMESPACE_BEGIN

extern sl_ptr _RefPtr_null[2];

struct SLIB_EXPORT _RefPtr_Container
{
	void* m_pointer;
	Ref<Referable> m_reference;
};

void _RefPtr_copy(_RefPtr_Container* target, _RefPtr_Container* source);
void _RefPtr_set(_RefPtr_Container* target, _RefPtr_Container* source);
void _RefPtr_set(_RefPtr_Container* target, const void* pointer);
void _RefPtr_set(_RefPtr_Container* target, const Ref<Referable>& ref);
void _RefPtr_set(_RefPtr_Container* target, const void* pointer, const Ref<Referable>& ref);

template <class TYPE>
class SLIB_EXPORT RefPtr
{
private:
	TYPE* m_pointer;
	Ref<Referable> m_reference;

public:
	SLIB_INLINE RefPtr()
	{
		m_pointer = sl_null;
	}

	SLIB_INLINE RefPtr(const RefPtr<TYPE>& other)
	{
		_RefPtr_copy((_RefPtr_Container*)((void*)this), (_RefPtr_Container*)((void*)(&other)));
	}

	SLIB_INLINE RefPtr(const TYPE* pointer)
	{
		m_pointer = (TYPE*)pointer;
	}

	template <class O>
	SLIB_INLINE RefPtr(const Ref<O>& reference)
	{
		m_reference = reference;
		m_pointer = (O*)(m_reference.getObject());
	}
	
	template <class O>
	SLIB_INLINE RefPtr(const TYPE* pointer, const Ref<O>& reference)
	{
		m_reference = reference;
		m_pointer = (TYPE*)pointer;
	}
	
public:
	SLIB_INLINE static const RefPtr<TYPE>& null()
	{
		return *((RefPtr<TYPE>*)((void*)(&_RefPtr_null)));
	}

	SLIB_INLINE RefPtr<TYPE>& operator=(const RefPtr<TYPE>& other)
	{
		_RefPtr_set((_RefPtr_Container*)((void*)this), (_RefPtr_Container*)((void*)(&other)));
		return *this;
	}

	SLIB_INLINE RefPtr<TYPE>& operator=(const TYPE* pointer)
	{
		_RefPtr_set((_RefPtr_Container*)((void*)this), pointer);
		return *this;
	}

	SLIB_INLINE RefPtr<TYPE>& operator=(const Ref<TYPE>& reference)
	{
		_RefPtr_set((_RefPtr_Container*)((void*)this), Ref<Referable>::from(reference));
		return *this;
	}

	template <class O>
	SLIB_INLINE RefPtr<TYPE>& operator=(const Ref<O>& _reference)
	{
		Ref<O> reference = _reference;
		TYPE* pointer = reference.get();
		_RefPtr_set((_RefPtr_Container*)((void*)this), pointer, Ref<Referable>::from(reference));
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const RefPtr<TYPE>& other) const
	{
		return m_pointer == other.m_pointer && m_reference == other.m_reference;
	}

	SLIB_INLINE sl_bool operator!=(const RefPtr<TYPE>& other) const
	{
		return m_pointer != other.m_pointer || m_reference != other.m_reference;
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
		*this = null();
	}

	SLIB_INLINE TYPE* getValue() const
	{
		return m_pointer;
	}

	SLIB_INLINE TYPE* get() const
	{
		return m_pointer;
	}

	SLIB_INLINE TYPE& operator*()
	{
		return *m_pointer;
	}

	SLIB_INLINE TYPE* operator->() const
	{
		return m_pointer;
	}

	SLIB_INLINE const Ref<Referable>& getReference() const
	{
		return m_reference;
	}
};

extern const sl_ptr _Ptr_null[2];

struct SLIB_EXPORT _Ptr_Container
{
	void* m_pointer;
	Ref<Referable> m_reference;
};

void _Ptr_copy(_Ptr_Container* target, _Ptr_Container* source);
void _Ptr_set(_Ptr_Container* target, _Ptr_Container* source);
void _Ptr_set(_Ptr_Container* target, const void* pointer);
void _Ptr_set(_Ptr_Container* target, const Ref<Referable>& ref);
void _Ptr_set(_Ptr_Container* target, const void* pointer, const Ref<Referable>& ref);
void _Ptr_lock(_Ptr_Container* target, _Ptr_Container* source);

template <class TYPE>
class SLIB_EXPORT Ptr {
private:
	TYPE* m_pointer;
	Ref<Referable> m_reference;

public:
	SLIB_INLINE Ptr()
	{
		m_pointer = sl_null;
	}

	SLIB_INLINE Ptr(const Ptr<TYPE>& other)
	{
		_Ptr_copy((_Ptr_Container*)((void*)this), (_Ptr_Container*)((void*)(&other)));
	}

	template <class O>
	SLIB_INLINE Ptr(const Ptr<O>& other)
	{
		TYPE* pointer = other.getValue();
		SLIB_UNUSED(pointer);
		_Ptr_copy((_Ptr_Container*)((void*)this), (_Ptr_Container*)((void*)(&other)));
	}

	SLIB_INLINE Ptr(const TYPE* pointer)
	{
		m_pointer = (TYPE*)pointer;
	}

	template <class O>
	SLIB_INLINE Ptr(const Ref<O>& reference)
	{
		m_reference = reference;
		m_pointer = (O*)(m_reference.getObject());
	}

	template <class O>
	SLIB_INLINE Ptr(const TYPE* pointer, const Ref<O>& reference)
	{
		m_pointer = (TYPE*)pointer;
		m_reference = reference;
	}

	template <class O>
	SLIB_INLINE Ptr(const WeakRef<O>& weak)
	{
		Ref<O> o = weak.lock();
		if (o.isNotNull()) {
			m_pointer = (O*)(o.getObject());
			m_reference = weak.getReference();
		} else {
			m_pointer = sl_null;
		}
	}

	template <class O>
	SLIB_INLINE Ptr(const TYPE* pointer, const WeakRef<O>& weak)
	{
		m_pointer = (TYPE*)pointer;
		m_reference = weak.getReference();
	}

public:
	SLIB_INLINE static const Ptr<TYPE>& null()
	{
		return *((Ptr<TYPE>*)((void*)(&_Ptr_null)));
	}

	SLIB_INLINE Ptr<TYPE>& operator=(const Ptr<TYPE>& other)
	{
		_Ptr_set((_Ptr_Container*)((void*)this), (_Ptr_Container*)((void*)(&other)));
		return *this;
	}

	SLIB_INLINE Ptr<TYPE>& operator=(const TYPE* pointer)
	{
		_Ptr_set((_Ptr_Container*)((void*)this), pointer);
		return *this;
	}

	SLIB_INLINE Ptr<TYPE>& operator=(const Ref<TYPE>& reference)
	{
		_Ptr_set((_Ptr_Container*)((void*)this), Ref<Referable>::from(reference));
		return *this;
	}

	template <class O>
	SLIB_INLINE Ptr<TYPE>& operator=(const Ref<O>& _reference)
	{
		Ref<O> reference = _reference;
		TYPE* pointer = reference.get();
		_Ptr_set((_Ptr_Container*)((void*)this), pointer, Ref<Referable>::from(reference));
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const Ptr<TYPE>& other) const
	{
		return m_pointer == other.m_pointer && m_reference == other.m_reference;
	}

	SLIB_INLINE sl_bool operator!=(const Ptr<TYPE>& other) const
	{
		return m_pointer != other.m_pointer || m_reference != other.m_reference;
	}

	SLIB_INLINE sl_bool isWeakReference() const
	{
		Ref<Referable> ref = m_reference;
		return (WeakRefObject::checkInstance(ref));
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
		*this = null();
	}

	SLIB_INLINE TYPE* getValue() const
	{
		return m_pointer;
	}

	SLIB_INLINE TYPE* get() const
	{
		return m_pointer;
	}

	SLIB_INLINE const Ref<Referable>& getReference() const
	{
		return m_reference;
	}

	SLIB_INLINE RefPtr<TYPE> lock() const
	{
		if (m_pointer) {
			_Ptr_Container ret;
			ret.m_pointer = sl_null;
			_Ptr_lock(&ret, (_Ptr_Container*)((void*)this));
			if (ret.m_pointer) {
				return RefPtr<TYPE>((TYPE*)(ret.m_pointer), ret.m_reference);
			}
		}
		return RefPtr<TYPE>::null();
	}

	template <class O>
	SLIB_INLINE static Ptr<TYPE> from(const Ptr<O>& _other)
	{
		Ptr<O> other = _other;
		return Ptr<TYPE>((TYPE*)(other.getValue()), other.getReference());
	}
};

template <class TYPE>
class SLIB_EXPORT PtrLocker
{
private:
	RefPtr<TYPE> m_object;
	TYPE* m_ptr;

public:
	SLIB_INLINE PtrLocker(const Ptr<TYPE>& object)
	{
		m_object = object.lock();
		m_ptr = m_object.getValue();
	}
	SLIB_INLINE ~PtrLocker()
	{
	}

	SLIB_INLINE void unlock()
	{
		m_object.setNull();
		m_ptr = sl_null;
	}

	SLIB_INLINE TYPE* getValue()
	{
		return m_ptr;
	}

	SLIB_INLINE TYPE* get()
	{
		return m_ptr;
	}

	SLIB_INLINE sl_bool isNull()
	{
		return m_ptr == sl_null;
	}

	SLIB_INLINE sl_bool isNotNull()
	{
		return m_ptr != sl_null;
	}

	SLIB_INLINE TYPE& operator*()
	{
		return *m_ptr;
	}

	SLIB_INLINE TYPE* operator->()
	{
		return m_ptr;
	}
};

SLIB_NAMESPACE_END

#endif
