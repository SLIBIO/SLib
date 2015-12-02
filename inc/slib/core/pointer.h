#ifndef CHECKHEADER_SLIB_CORE_POINTER
#define CHECKHEADER_SLIB_CORE_POINTER

#include "definition.h"
#include "base.h"
#include "reference.h"

SLIB_NAMESPACE_BEGIN
template <class TYPE>
class SLIB_EXPORT _SharedPtrContainer : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(_SharedPtrContainer)
public:
	TYPE* obj;
	
	_SharedPtrContainer(TYPE* o)
	{
		obj = o;
	}
	
	~_SharedPtrContainer()
	{
		delete obj;
	}
};

template <class TYPE, sl_bool flagThreadSafe>
class WeakPtr;

/** auto-referencing object **/
template <class TYPE, sl_bool flagThreadSafe = sl_true>
class SLIB_EXPORT SharedPtr
{
	typedef SharedPtr<TYPE, flagThreadSafe> _Type;
	typedef _SharedPtrContainer<TYPE> _Obj;
	typedef Ref<_SharedPtrContainer<TYPE>, flagThreadSafe> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(SharedPtr, _Type, _Obj, _Ref)

public:
	SLIB_INLINE SharedPtr(TYPE* obj) : m_object(new _Obj(obj))
	{
	}
	
	SLIB_INLINE _Type& operator=(TYPE* obj)
	{
		m_object = new _Obj(obj);
		return *this;
	}

	SLIB_INLINE TYPE& operator*() const
	{
		return *(m_object.getObject()->obj);
	}

	SLIB_INLINE TYPE* operator->() const
	{
		return (m_object.getObject()->obj);
	}

	SLIB_INLINE TYPE* getPointer() const
	{
		_Obj* container = m_object.getObject();
		if (container) {
			return container->obj;
		} else {
			return sl_null;
		}
	}


public:
	static _Type create(TYPE* obj)
	{
		_Type ret;
		ret.m_object = new _Obj(obj);
		return ret;
	}
	
	friend class WeakPtr<TYPE, flagThreadSafe>;
};

/** auto-referencing object **/
template <class TYPE, sl_bool flagThreadSafe = sl_true>
class SLIB_EXPORT WeakPtr
{
private:
	typedef WeakPtr<TYPE, flagThreadSafe> _Type;
	WeakRef< _SharedPtrContainer<TYPE>, flagThreadSafe > m_object;
	
public:
	SLIB_INLINE WeakPtr()
	{
	}
	SLIB_INLINE WeakPtr(const _Type& other) : m_object(other.m_object)
	{
	}

	template <class _TYPE, sl_bool _flagThreadSafe>
	SLIB_INLINE WeakPtr(const SharedPtr<TYPE, _flagThreadSafe>& other) : m_object(other.m_object)
	{
	}

	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_null)));
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return m_object.isNull();
	}

	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_object.isNotNull();
	}

	SLIB_INLINE void setNull()
	{
		m_object.setNull();
	}

	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		m_object = other.m_object;
		return *this;
	}

	template <class _TYPE, sl_bool _flagThreadSafe>
	SLIB_INLINE _Type& operator=(const SharedPtr<_TYPE, _flagThreadSafe>& other)
	{
		m_object = other.m_object;
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return (m_object == other.m_object);
	}

	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return (m_object != other.m_object);
	}
	
	SLIB_INLINE SharedPtr<TYPE, sl_false> lock()
	{
		SharedPtr<TYPE, sl_false> ret;
		ret.m_object = m_object.lock();
		return ret;
	}

	SLIB_INLINE Ref<Referable, sl_false> getReference()
	{
		return m_object.getReference();
	}
};


/** auto-referencing object **/
class SLIB_EXPORT _SafePtrContainer : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(_SafePtrContainer)
public:
	void* obj;
	SpinLock locker;
	sl_int32 nRefCount;

	SLIB_INLINE _SafePtrContainer(void* o)
	{
		obj = o;
		nRefCount = 0;
	}
};

void* _SafePtr_lock(const Ref<_SafePtrContainer>& object);
void _SafePtr_free(const Ref<_SafePtrContainer>& object);

template <class TYPE>
class SLIB_EXPORT SafePtr
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(SafePtr<TYPE>, _SafePtrContainer)
	SLIB_DECLARE_OBJECT_WRAPPER(SafePtr, SafePtr<TYPE>, _SafePtrContainer, Ref<_SafePtrContainer>)

public:
	SLIB_INLINE SafePtr(TYPE* obj) : m_object(new _SafePtrContainer(obj))
	{
	}

	SLIB_INLINE SafePtr<TYPE>& operator=(TYPE* obj)
	{
		m_object = new _SafePtrContainer(obj);
		return *this;
	}

	SLIB_INLINE TYPE* lock() const
	{
		return (TYPE*)(_SafePtr_lock(m_object));
	}

	SLIB_INLINE void unlock() const
	{
		Ref<_SafePtrContainer> o = m_object;
		_SafePtrContainer* container = o.getObject();
		if (container) {
			Base::interlockedDecrement32(&(container->nRefCount));
		}
	}

	SLIB_INLINE void free() const
	{
		_SafePtr_free(m_object);
	}

	SLIB_INLINE static SafePtr<TYPE> create(TYPE* obj)
	{
		SafePtr<TYPE> ret;
		ret.m_object = new _SafePtrContainer(obj);
		return ret;
	}
};

template <class TYPE>
class SLIB_EXPORT SafePtrLocker
{
private:
	SafePtr<TYPE> m_ref;
	TYPE* m_ptr;

public:
	SLIB_INLINE SafePtrLocker(const SafePtr<TYPE>& ref)
	{
		m_ref = ref;
		m_ptr = m_ref.lock();
	}

	SLIB_INLINE ~SafePtrLocker()
	{
		m_ref.unlock();
	}

	SLIB_INLINE void unlock()
	{
		m_ref.unlock();
		m_ref.setNull();
	}

	SLIB_INLINE TYPE* getValue()
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

/** auto-referencing object **/
template <class TYPE>
class SLIB_EXPORT _UnitedPtrContainer : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(_UnitedPtrContainer)
public:
	_UnitedPtrContainer() {}
	enum Type {
		typePointer = 0,
		typeReference = 1,
		typeWeak = 2,
		typeShared = 3,
		typeWeakPtr = 4,
		typeSafe = 5
	};
	Type type;
	TYPE* pointer;
	Ref<Referable> ref;
	Ref<TYPE> reference;
	WeakRef<TYPE> weak;
	SharedPtr<TYPE> shared_ptr;
	WeakPtr<TYPE> weak_ptr;
	SafePtr<TYPE> safe_ptr;
};

template <class TYPE>
class SLIB_EXPORT UnitedPtr
{
	typedef _UnitedPtrContainer<TYPE> _Container;
	SLIB_DECLARE_OBJECT_TYPE_FROM(UnitedPtr<TYPE>, _Container)
	SLIB_DECLARE_OBJECT_WRAPPER(UnitedPtr, UnitedPtr<TYPE>, _Container, Ref<_Container>)

public:
	SLIB_INLINE UnitedPtr(TYPE* pointer)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typePointer;
		container->pointer = (TYPE*)pointer;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(TYPE* pointer, const Ref<Referable>& ref)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typePointer;
		container->pointer = pointer;
		container->ref = ref;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(const Ref<TYPE>& reference)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeReference;
		container->reference = reference;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(const WeakRef<TYPE>& weak)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeWeak;
		container->weak = weak;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(const SharedPtr<TYPE>& shared)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeShared;
		container->shared = shared;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(const WeakPtr<TYPE>& weak_ptr)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeShared;
		container->weak_ptr = weak_ptr;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr(const SafePtr<TYPE>& safe_ptr)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeSafe;
		container->safe_ptr = safe_ptr;
		m_object = container;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(TYPE* pointer)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typePointer;
		container->pointer = pointer;
		m_object = container;
		return *this;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(const Ref<TYPE>& reference)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeReference;
		container->reference = reference;
		m_object = container;
		return *this;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(const WeakRef<TYPE>& weak)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeWeak;
		container->weak = weak;
		m_object = container;
		return *this;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(const SharedPtr<TYPE>& shared)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeShared;
		container->shared = shared;
		m_object = container;
		return *this;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(const WeakPtr<TYPE>& weak_ptr)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeShared;
		container->weak_ptr = weak_ptr;
		m_object = container;
		return *this;
	}

	SLIB_INLINE UnitedPtr<TYPE>& operator=(const SafePtr<TYPE>& safe_ptr)
	{
		_UnitedPtrContainer<TYPE>* container = new _UnitedPtrContainer<TYPE>();
		container->type = _Container::typeSafe;
		container->safe_ptr = safe_ptr;
		m_object = container;
		return *this;
	}

	TYPE* lock(Ref<Referable>& reference) const
	{
		Ref< _UnitedPtrContainer<TYPE> > o = m_object;
		_UnitedPtrContainer<TYPE>* container = o.getObject();
		if (container) {
			switch (container->type) {
			case _Container::typePointer:
				return container->pointer;
			case _Container::typeReference:
				return container->reference.getObject();
			case _Container::typeWeak:
				{
					Ref<TYPE> ref = container->weak.lock();
					reference = ref.getReference();
					return ref.getObject();
				}
			case _Container::typeShared:
				return container->shared_ptr.getPointer();
			case _Container::typeWeakPtr:
				{
					SharedPtr<TYPE> ref = container->weak_ptr.lock();
					reference = ref.getReference();
					return ref.getPointer();
				}
			case _Container::typeSafe:
				return container->safe_ptr.lock();
			default:
				return sl_null;
			}
		} else {
			return sl_null;
		}
	}

	void unlock() const
	{
		Ref< _UnitedPtrContainer<TYPE> > o = m_object;
		_UnitedPtrContainer<TYPE>* container = o.getObject();
		if (container) {
			if (container->type == _Container::typeSafe) {
				container->safe_ptr.unlock();
			}
		}
	}

	void free() const
	{
		Ref< _UnitedPtrContainer<TYPE> > o = m_object;
		_UnitedPtrContainer<TYPE>* container = o.getObject();
		if (container) {
			container->ref.setNull();
			container->pointer = sl_null;
			container->type = _Container::typePointer;
			container->reference.setNull();
			container->weak.setNull();
			container->shared_ptr.setNull();
			container->weak_ptr.setNull();
			container->safe_ptr.free();
			container->safe_ptr.setNull();
		}
	}
};

template <class TYPE>
class SLIB_EXPORT UnitedPtrLocker
{
private:
	UnitedPtr<TYPE> m_object;
	Ref<Referable> m_ref;
	TYPE* m_ptr;

public:
	SLIB_INLINE UnitedPtrLocker(const UnitedPtr<TYPE>& object)
	{
		m_object = object;
		m_ptr = m_object.lock(m_ref);
	}

	SLIB_INLINE ~UnitedPtrLocker()
	{
		m_object.unlock();
	}

	SLIB_INLINE void unlock()
	{
		m_object.unlock();
		m_object.setNull();
		m_ref.setNull();
	}

	SLIB_INLINE TYPE* getValue()
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

extern sl_ptr _RefPtr_null[2];

struct SLIB_EXPORT _RefPtr_Container
{
	void* m_pointer;
	Ref<Referable> m_reference;
};

void _RefPtr_copy(_RefPtr_Container* target, _RefPtr_Container* source);
void _RefPtr_set(_RefPtr_Container* target, _RefPtr_Container* source);
void _RefPtr_set(_RefPtr_Container* target, const void* pointer);
void _RefPtr_set(_RefPtr_Container* target, const Ref<Referable, sl_true>& ref);
void _RefPtr_set(_RefPtr_Container* target, const Ref<Referable, sl_false>& ref);
void _RefPtr_set(_RefPtr_Container* target, const void* pointer, const Ref<Referable, sl_true>& ref);
void _RefPtr_set(_RefPtr_Container* target, const void* pointer, const Ref<Referable, sl_false>& ref);

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

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE RefPtr(const Ref<O, _flagThreadSafe>& reference)
	{
		m_reference = reference;
		m_pointer = (O*)(m_reference.getObject());
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE RefPtr(const TYPE* pointer, const Ref<O, _flagThreadSafe>& reference)
	{
		m_reference = reference;
		m_pointer = (TYPE*)pointer;
	}

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

	SLIB_INLINE RefPtr<TYPE>& operator=(const Ref<TYPE, sl_true>& reference)
	{
		_RefPtr_set((_RefPtr_Container*)((void*)this), Ref<Referable, sl_true>::from(reference));
		return *this;
	}

	SLIB_INLINE RefPtr<TYPE>& operator=(const Ref<TYPE, sl_false>& reference)
	{
		_RefPtr_set((_RefPtr_Container*)((void*)this), Ref<Referable, sl_false>::from(reference));
		return *this;
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE RefPtr<TYPE>& operator=(const Ref<O, _flagThreadSafe>& _reference)
	{
		Ref<O, _flagThreadSafe> reference = _reference;
		TYPE* pointer = reference.get();
		_RefPtr_set((_RefPtr_Container*)((void*)this), pointer, Ref<Referable, _flagThreadSafe>::from(reference));
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
void _Ptr_set(_Ptr_Container* target, const Ref<Referable, sl_true>& ref);
void _Ptr_set(_Ptr_Container* target, const Ref<Referable, sl_false>& ref);
void _Ptr_set(_Ptr_Container* target, const void* pointer, const Ref<Referable, sl_true>& ref);
void _Ptr_set(_Ptr_Container* target, const void* pointer, const Ref<Referable, sl_false>& ref);
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

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const Ref<O, _flagThreadSafe>& reference)
	{
		m_reference = reference;
		m_pointer = (O*)(m_reference.getObject());
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const TYPE* pointer, const Ref<O, _flagThreadSafe>& reference)
	{
		m_reference = reference;
		m_pointer = (TYPE*)pointer;
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const WeakRef<O, _flagThreadSafe>& weak)
	{
		Ref<O, sl_false> o = weak.lock();
		if (o.isNotNull()) {
			m_pointer = (O*)(o.getObject());
			m_reference = weak.getReference();
		} else {
			m_pointer = sl_null;
		}
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const TYPE* pointer, const WeakRef<O, _flagThreadSafe>& weak)
	{
		m_pointer = (TYPE*)pointer;
		m_reference = weak.getReference();
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const SharedPtr<O, _flagThreadSafe>& _ptr)
	{
		SharedPtr<O, _flagThreadSafe> ptr = _ptr;
		m_reference = ptr.getReference();
		m_pointer = (O*)(ptr.getPointer());
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr(const WeakPtr<O, _flagThreadSafe>& _ptr)
	{
		WeakPtr<O, _flagThreadSafe> ptr = _ptr;
		SharedPtr<O, sl_false> o = ptr.lock();
		if (o.isNotNull()) {
			m_pointer = (TYPE*)(o.getPointer());
			m_reference = ptr.getReference();
		} else {
			m_pointer = sl_null;
		}
	}

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

	SLIB_INLINE Ptr<TYPE>& operator=(const Ref<TYPE, sl_true>& reference)
	{
		_Ptr_set((_Ptr_Container*)((void*)this), Ref<Referable, sl_true>::from(reference));
		return *this;
	}

	SLIB_INLINE Ptr<TYPE>& operator=(const Ref<TYPE, sl_false>& reference)
	{
		_Ptr_set((_Ptr_Container*)((void*)this), Ref<Referable, sl_false>::from(reference));
		return *this;
	}

	template <class O, sl_bool _flagThreadSafe>
	SLIB_INLINE Ptr<TYPE>& operator=(const Ref<O, _flagThreadSafe>& _reference)
	{
		Ref<O, _flagThreadSafe> reference = _reference;
		TYPE* pointer = reference.get();
		_Ptr_set((_Ptr_Container*)((void*)this), pointer, Ref<Referable, _flagThreadSafe>::from(reference));
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
