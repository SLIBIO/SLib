#ifndef CHECKHEADER_SLIB_CORE_REFERENCE
#define CHECKHEADER_SLIB_CORE_REFERENCE

#include "definition.h"
#include "base.h"
#include "spinlock.h"
#include "mutex.h"

#ifdef SLIB_DEBUG
#define SLIB_DEBUG_REFERENCE
#endif

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT RefCount
{
private:
	sl_reg m_nRef;
	
public:
	SLIB_INLINE RefCount()
	{
		m_nRef = 0;
	}
	
	SLIB_INLINE ~RefCount()
	{
	}
	
	SLIB_INLINE sl_reg add()
	{
		return Base::interlockedIncrement(&m_nRef);
	}
	
	SLIB_INLINE sl_reg release()
	{
		return Base::interlockedDecrement(&m_nRef);
	}
	
	SLIB_INLINE sl_reg getCount()
	{
		return m_nRef;
	}
	
	sl_bool waitNoRef(sl_int32 timeout = -1);
	
};
SLIB_NAMESPACE_END

typedef const void* sl_class_type;

sl_bool sl_compare_class_type(sl_class_type a, sl_class_type b);


SLIB_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT ClassType
{
public:
	static sl_uint8 _id;
};

template <class T>
sl_uint8 ClassType<T>::_id = 0;

#define SLIB_CLASS_TYPE(CLASS) &(slib::ClassType<CLASS>::_id)

SLIB_NAMESPACE_END


#define SLIB_DECLARE_OBJECT_TYPE_BASE(CLASS) \
	template <class _ObjectClass> \
	static sl_bool checkInstance(const slib::Ref<_ObjectClass>& _object) \
	{ \
		slib::Ref<_ObjectClass> object = _object; \
		if (object.isNotNull()) { \
			return object->checkClassType(CLASS::ClassType()); \
		} else { \
			return sl_false; \
		} \
	} \
	static sl_bool checkInstance(const slib::Referable* object) \
	{ \
		if (object) { \
			return object->checkClassType(CLASS::ClassType()); \
		} else { \
			return sl_false; \
		} \
	}

#define SLIB_DECLARE_OBJECT_TYPE(CLASS) \
public: \
	static sl_class_type ClassType() \
	{ \
		return SLIB_CLASS_TYPE(CLASS); \
	} \
	static const char* ClassTypeName() \
	{ \
		return #CLASS; \
	} \
	SLIB_DECLARE_OBJECT_TYPE_BASE(CLASS)


#define SLIB_DECLARE_OBJECT_TYPE_FROM(CLASS, FROM) \
public: \
	static sl_class_type ClassType() \
	{ \
		return FROM::ClassType(); \
	} \
	static const char* ClassTypeName() \
	{ \
		return FROM::ClassTypeName(); \
	} \
	SLIB_DECLARE_OBJECT_TYPE_BASE(CLASS)


#define SLIB_DECLARE_ROOT_REFERABLE(CLASS) \
public: \
	virtual sl_class_type getClassType() const \
	{ \
		return CLASS::ClassType(); \
	} \
	virtual const char* getClassTypeName() const \
	{ \
		return CLASS::ClassTypeName(); \
	} \
	static sl_bool _checkClassType(sl_class_type type) \
	{ \
		return sl_compare_class_type(type, CLASS::ClassType()); \
	} \
	virtual sl_bool checkClassType(sl_class_type type) const \
	{ \
		return _checkClassType(type); \
	} \
	SLIB_DECLARE_OBJECT_TYPE(CLASS)

#define SLIB_DECLARE_ROOT_OBJECT(CLASS) \
	SLIB_DECLARE_ROOT_REFERABLE(CLASS); \
	SLIB_DECLARE_CLASS_NOCOPY(CLASS);

#define SLIB_DECLARE_REFERABLE(CLASS, BASE) \
public: \
	virtual sl_class_type getClassType() const \
	{ \
		return CLASS::ClassType(); \
	} \
	virtual const char* getClassTypeName() const \
	{ \
		return CLASS::ClassTypeName(); \
	} \
	static sl_bool _checkClassType(sl_class_type type) \
	{ \
		if (sl_compare_class_type(type, CLASS::ClassType())) { \
			return sl_true; \
		} else { \
			return BASE::_checkClassType(type); \
		} \
	} \
	virtual sl_bool checkClassType(sl_class_type type) const \
	{ \
		return _checkClassType(type); \
	} \
	SLIB_DECLARE_OBJECT_TYPE(CLASS)

#define SLIB_DECLARE_OBJECT(CLASS, BASE) \
	SLIB_DECLARE_REFERABLE(CLASS, BASE) \
	SLIB_DECLARE_CLASS_NOCOPY(CLASS)

#define SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(CLASS_NAME, CLASS_TYPE, OBJ_TYPE, REF_TYPE) \
protected: \
	REF_TYPE m_object; \
public: \
	SLIB_INLINE CLASS_NAME() {} \
	SLIB_INLINE CLASS_NAME(const CLASS_TYPE& other) : m_object(other.m_object) \
	{ \
	} \
	SLIB_INLINE CLASS_NAME(OBJ_TYPE* object) : m_object(object) \
	{ \
	} \
	SLIB_INLINE static const CLASS_TYPE& null() \
	{ \
		return *((CLASS_TYPE*)((void*)(&_Ref_null))); \
	} \
	SLIB_INLINE sl_bool isNull() const \
	{ \
		return m_object.isNull(); \
	} \
	SLIB_INLINE sl_bool isNotNull() const \
	{ \
		return m_object.isNotNull(); \
	} \
	SLIB_INLINE void setNull() \
	{ \
		m_object.setNull(); \
	} \
	SLIB_INLINE const REF_TYPE& getObjectReference() const \
	{ \
		return m_object; \
	} \
	SLIB_INLINE Ref<Referable> getReference() const \
	{ \
		return m_object.getReference(); \
	} \
	template <class _ObjectClass> \
	SLIB_INLINE static const CLASS_TYPE& fromReference(const slib::Ref<_ObjectClass>& object) \
	{ \
		return *((CLASS_TYPE*)((void*)&object)); \
	}

#define SLIB_DECLARE_OBJECT_WRAPPER(CLASS_NAME, CLASS_TYPE, OBJ_TYPE, REF_TYPE) \
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(CLASS_NAME, CLASS_TYPE, OBJ_TYPE, REF_TYPE) \
	SLIB_INLINE CLASS_NAME(const slib::Ref<OBJ_TYPE>& object) : m_object(object) \
	{ \
	} \
	SLIB_INLINE sl_bool operator==(const CLASS_TYPE& other) const \
	{ \
		return m_object == other.m_object; \
	} \
	SLIB_INLINE sl_bool operator!=(const CLASS_TYPE& other) const \
	{ \
		return m_object != other.m_object; \
	} \
	SLIB_INLINE CLASS_TYPE& operator=(const CLASS_TYPE& other) \
	{ \
		m_object = other.m_object; \
		return *this; \
	} \
	SLIB_INLINE CLASS_TYPE& operator=(OBJ_TYPE* object) \
	{ \
		m_object = object; \
		return *this; \
	} \
	template <class _ObjectClass> \
	SLIB_INLINE CLASS_TYPE& operator=(const slib::Ref<_ObjectClass>& object) \
	{ \
		m_object = object; \
		return *this; \
	}


/************************************************************************
					Ref Class Definition
	Objects are required to inherit from Referable
************************************************************************/

SLIB_NAMESPACE_BEGIN

class WeakRefObject;

class SLIB_EXPORT Referable
{
public:
	Referable();
	virtual ~Referable();

public:
	SLIB_INLINE sl_reg increaseReference()
	{
		return _increaseReference();
	}

	SLIB_INLINE sl_reg decreaseReference()
	{
		sl_reg nRef = _decreaseReference();
		if (nRef == 0) {
			_free();
		}
		return nRef;
	}
	
	SLIB_INLINE void setNoRef()
	{
		m_flagNoRef = sl_true;
		m_nRefCount = SLIB_SIZE_MAX >> 1;
	}
	
public:
	SLIB_INLINE sl_reg _increaseReference()
	{
#ifdef SLIB_DEBUG_REFERENCE
		_checkValid();
#endif
		return Base::interlockedIncrement(&(m_nRefCount));
	}

	SLIB_INLINE sl_reg _decreaseReference()
	{
#ifdef SLIB_DEBUG_REFERENCE
		_checkValid();
#endif
		return Base::interlockedDecrement(&(m_nRefCount));
	}

	WeakRefObject* _getWeak();

private:
	void _clearWeak();
public:
	void _free();

#if defined(SLIB_DEBUG_REFERENCE)
private:
	sl_reg m_signature;
	void _checkValid();
#endif

private:
	sl_reg m_nRefCount;
	WeakRefObject* m_weak;
	sl_bool m_flagNoRef;

public:
	virtual sl_class_type getClassType() const;
	virtual const char* getClassTypeName() const;
	virtual sl_bool checkClassType(sl_class_type type) const;
};

extern void* const _Ref_null;

template <class ObjectClass>
class SLIB_EXPORT Ref
{
	typedef Ref<ObjectClass> _Type;
public:
	ObjectClass* m_object;

public:
	SLIB_INLINE Ref()
	{
		m_object = sl_null;
	}
	
	SLIB_INLINE ~Ref()
	{
		if (m_object) {
			((Referable*)m_object)->decreaseReference();
			m_object = sl_null;
		}
	}

	SLIB_INLINE Ref(const Ref<ObjectClass>& other)
	{
		ObjectClass* object = other._cloneObject();
		m_object = object;
	}

	template <class _ObjectClass>
	SLIB_INLINE Ref(const Ref<_ObjectClass>& other)
	{
		ObjectClass* object = other._cloneObject();
		m_object = object;
	}
	
	SLIB_INLINE Ref(const ObjectClass* _object)
	{
		Referable* object = (Referable*)_object;
		if (object) {
			object->increaseReference();
		}
		m_object = (ObjectClass*)object;
	}
	
	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_null)));
	}

	SLIB_INLINE sl_bool isNull() const
	{
		return (m_object == sl_null);
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return (m_object != sl_null);
	}

	SLIB_INLINE void setNull()
	{
		_replaceObject(sl_null);
	}

	SLIB_INLINE ObjectClass* getObject() const
	{
		return m_object;
	}

	SLIB_INLINE ObjectClass* get() const
	{
		return m_object;
	}

	SLIB_INLINE _Type& operator=(const Ref<ObjectClass>& other)
	{
		if ((void*)this != (void*)(&other) && m_object != other.m_object) {
			ObjectClass* object = other._cloneObject();
			_replaceObject(object);
		}
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const Ref<_ObjectClass>& other)
	{
		if ((void*)this != (void*)(&other) && m_object != other.m_object) {
			ObjectClass* object = other._cloneObject();
			_replaceObject(object);
		}
		return *this;
	}

	SLIB_INLINE _Type& operator=(const ObjectClass* _other)
	{
		Referable* other = (Referable*)_other;
		if (((Referable*)m_object) != other) {
			if (other) {
				other->increaseReference();
			}
			_replaceObject((ObjectClass*)other);
		}
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(ObjectClass* _other)
	{
		Referable* other = (Referable*)_other;
		if (((Referable*)m_object) != other) {
			if (other) {
				other->increaseReference();
			}
			_replaceObject((ObjectClass*)other);
		}
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const ObjectClass* other) const
	{
		return (m_object == other);
	}
	
	SLIB_INLINE sl_bool operator==(ObjectClass* other) const
	{
		return (m_object == other);
	}
	
	SLIB_INLINE sl_bool operator==(const Ref<ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other.m_object));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other.m_object));
	}

	SLIB_INLINE sl_bool operator!=(const ObjectClass* other) const
	{
		return (m_object != other);
	}
	
	SLIB_INLINE sl_bool operator!=(ObjectClass* other) const
	{
		return (m_object != other);
	}

	SLIB_INLINE sl_bool operator!=(const Ref<ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other.m_object));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other.m_object));
	}

	SLIB_INLINE friend sl_bool operator==(const ObjectClass* a, const Ref<ObjectClass>& b)
	{
		return (a == b.m_object);
	}
	
	SLIB_INLINE ObjectClass& operator*() const
	{
		return *((ObjectClass*)m_object);
	}

	SLIB_INLINE ObjectClass* operator->() const
	{
		return (ObjectClass*)m_object;
	}

	template <class _ObjectClass>
	SLIB_INLINE static const _Type& from(const Ref<_ObjectClass>& other)
	{
		return *((_Type*)((void*)&other));
	}

	SLIB_INLINE const Ref<Referable>& getReference() const
	{
		return *((Ref<Referable>*)((void*)this));
	}

public:
	ObjectClass* _cloneObject() const
	{
		ObjectClass* const * source = &m_object;
		if ((void*)source == (void*)(&_Ref_null)) {
			return sl_null;
		}
		SpinLocker lock(SpinLockPoolForReference::get(source));
		ObjectClass* object = *source;
		if (object) {
			((Referable*)object)->increaseReference();
		}
		return object;
	}

	void _replaceObject(ObjectClass* object)
	{
		ObjectClass** target = &m_object;
		SpinLocker lock(SpinLockPoolForReference::get(target));
		ObjectClass* before = *target;
		*target = object;
		if (before) {
			sl_reg nRef = ((Referable*)before)->_decreaseReference();
			if (nRef == 0) {
				lock.unlock();
				((Referable*)before)->_free();
			}
		}
	}
};

class SLIB_EXPORT WeakRefObject : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(WeakRefObject)
public:
	WeakRefObject();

	Referable* object;
	SpinLock lock;
};

template <class ObjectClass>
class SLIB_EXPORT WeakRef
{
	typedef WeakRef<ObjectClass> _Type;
	typedef Ref<WeakRefObject> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, WeakRefObject)
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(WeakRef, _Type, WeakRefObject, _Ref)

public:
	SLIB_INLINE WeakRef(const Ref<ObjectClass>& _ref)
	{
		Ref<ObjectClass> ref(_ref);
		_setFromReference((Referable*)(ref.getObject()));
	}

	SLIB_INLINE WeakRef(const ObjectClass* ref)
	{
		_setFromReference((Referable*)ref);
	}

	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		m_object = other.m_object;
		return *this;
	}

	SLIB_INLINE _Type& operator=(const WeakRefObject* object)
	{
		m_object = object;
		return *this;
	}

	SLIB_INLINE _Type& operator=(const Ref<ObjectClass>& _ref)
	{
		Ref<ObjectClass> ref(_ref);
		_setFromReference((Referable*)(ref.getObject()));
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const ObjectClass* ref)
	{
		_setFromReference((Referable*)ref);
		return *this;
	}

	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return m_object == other.m_object;
	}

	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return m_object != other.m_object;
	}

	Ref<ObjectClass> lock() const
	{
		Ref<ObjectClass> ret;
		_Ref refWeak(m_object);
		if (refWeak.isNotNull()) {
			WeakRefObject* weak = refWeak.getObject();
			SpinLocker lock(&(weak->lock));
			Referable* ref = (Referable*)(weak->object);
			if (ref) {
				sl_reg n = ref->_increaseReference();
				if (n > 1) {
					ret = (ObjectClass*)(ref);
				}
				ref->_decreaseReference();
			}
		}
		return ret;
	}

	SLIB_INLINE static _Type from(const Referable* referable)
	{
		if (referable) {
			if (WeakRefObject::checkInstance(referable)) {
				return _Type((WeakRefObject*)referable);
			} else {
				WeakRefObject* w = ((Referable*)referable)->_getWeak();
				return _Type(w);
			}
		} else {
			return _Type::null();
		}
	}
	
private:
	SLIB_INLINE void _setFromReference(Referable* ref)
	{
		if (ref) {
			m_object = ref->_getWeak();
		} else {
			m_object.setNull();
		}
	}
};

class SLIB_EXPORT _ReferableConstructor
{
public:
	Referable* m_object;
	_ReferableConstructor(Referable* object)
	{
		m_object = object;
		m_object->_increaseReference();
	}
	~_ReferableConstructor()
	{
		m_object->_decreaseReference();
	}
};
SLIB_NAMESPACE_END

#define SLIB_REFERABLE_CONSTRUCTOR slib::_ReferableConstructor _slib_referable_constructor(this);

#define SLIB_NEW_REF(CLASS, NAME, ...) \
	CLASS* NAME = new CLASS(__VA_ARGS__); \
	Ref<CLASS> _ref_##NAME = NAME;

#endif// CHECK_HEADER_SLIB_CORE_REFERENCE
