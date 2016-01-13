#ifndef CHECKHEADER_SLIB_CORE_REFERENCE
#define CHECKHEADER_SLIB_CORE_REFERENCE

#include "definition.h"

#include "base.h"
#include "spinlock.h"

#ifdef SLIB_DEBUG
#define SLIB_DEBUG_REFERENCE
#endif

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
	static sl_bool checkInstance(const slib::Ref<_ObjectClass>& object) \
	{ \
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

#define SLIB_DECLARE_OBJECT_WRAPPER_BASE(CLASS_NAME, TYPE, OBJ) \
	SLIB_INLINE CLASS_NAME() {} \
	SLIB_INLINE CLASS_NAME(const TYPE& other) : m_object(other.m_object) \
	{ \
	} \
	SLIB_INLINE CLASS_NAME(const OBJ* object) : m_object(object) \
	{ \
	} \
	template <class _T> \
	SLIB_INLINE CLASS_NAME(const slib::Ref<_T>& object) : m_object(object) \
	{ \
	} \
	template <class _T> \
	SLIB_INLINE CLASS_NAME(const slib::SafeRef<_T>& object) : m_object(object) \
	{ \
	} \
	SLIB_INLINE static const TYPE& null() \
	{ \
		return *((TYPE*)((void*)(&_Ref_Null))); \
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
	SLIB_INLINE TYPE& operator=(const TYPE& other) \
	{ \
		m_object = other.m_object; \
		return *this; \
	} \
	SLIB_INLINE TYPE& operator=(const OBJ* object) \
	{ \
		m_object = object; \
		return *this; \
	} \
	template <class _T> \
	SLIB_INLINE TYPE& operator=(slib::Ref<_T>&& object) \
	{ \
		m_object._replaceObject(object.move()); \
		return *this; \
	} \
	template <class _T> \
	SLIB_INLINE TYPE& operator=(const slib::Ref<_T>& object) \
	{ \
		m_object = object; \
		return *this; \
	} \
	template <class _T> \
	SLIB_INLINE TYPE& operator=(const slib::SafeRef<_T>& object) \
	{ \
		m_object = object; \
		return *this; \
	}


#define SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(CLASS_NAME, OBJ, TYPE, SAFE_TYPE) \
protected: \
	slib::Ref<OBJ> m_object; \
public: \
	SLIB_DECLARE_OBJECT_WRAPPER_BASE(CLASS_NAME, TYPE, OBJ) \
	SLIB_INLINE const slib::Ref<OBJ>& getRef() const \
	{ \
		return m_object; \
	} \
	SLIB_INLINE slib::Ref<OBJ>& getRef() \
	{ \
		return m_object; \
	} \
	SLIB_INLINE CLASS_NAME(TYPE&& other) \
	{ \
		m_object._move_init(&other); \
	} \
	SLIB_INLINE TYPE& operator=(TYPE&& other) \
	{ \
		m_object._move_assign(&other); \
		return *this; \
	} \
	SLIB_INLINE CLASS_NAME(SAFE_TYPE&& other) \
	{ \
		m_object._move_init(&other); \
	} \
	SLIB_INLINE TYPE& operator=(SAFE_TYPE&& other) \
	{ \
		m_object._move_assign(&other); \
		return *this; \
	} \
	SLIB_INLINE CLASS_NAME(const SAFE_TYPE& other) : m_object(*((slib::SafeRef<OBJ>*)((void*)&other))) \
	{ \
	} \
	SLIB_INLINE TYPE& operator=(const SAFE_TYPE& other) \
	{ \
		m_object = *((slib::SafeRef<OBJ>*)((void*)&other)); \
		return *this; \
	} \
	SLIB_INLINE OBJ* getObject() const \
	{ \
		return m_object.get(); \
	}

#define SLIB_DECLARE_OBJECT_WRAPPER(CLASS_NAME, OBJ, TYPE, SAFE_TYPE) \
	SLIB_DECLARE_OBJECT_WRAPPER_NO_OP(CLASS_NAME, OBJ, TYPE, SAFE_TYPE) \
	SLIB_INLINE sl_bool operator==(const TYPE& other) const \
	{ \
		return m_object == other.m_object; \
	} \
	SLIB_INLINE sl_bool operator==(const SAFE_TYPE& other) const \
	{ \
		return m_object == *((slib::SafeRef<OBJ>*)((void*)&other)); \
	} \
	SLIB_INLINE sl_bool operator!=(const TYPE& other) const \
	{ \
		return m_object != other.m_object; \
	} \
	SLIB_INLINE sl_bool operator!=(const SAFE_TYPE& other) const \
	{ \
		return m_object != *((slib::SafeRef<OBJ>*)((void*)&other)); \
	}


#define SLIB_DECLARE_OBJECT_SAFE_WRAPPER_NO_OP(CLASS_NAME, OBJ, SAFE_TYPE, TYPE) \
protected: \
	slib::SafeRef<OBJ> m_object; \
public: \
	SLIB_DECLARE_OBJECT_WRAPPER_BASE(CLASS_NAME, SAFE_TYPE, OBJ) \
	SLIB_INLINE const slib::SafeRef<OBJ>& getRef() const \
	{ \
		return m_object; \
	} \
	SLIB_INLINE slib::SafeRef<OBJ>& getRef() \
	{ \
		return m_object; \
	} \
	SLIB_INLINE CLASS_NAME(SAFE_TYPE&& other) \
	{ \
		m_object._move_init(&other); \
	} \
	SLIB_INLINE SAFE_TYPE& operator=(SAFE_TYPE&& other) \
	{ \
		m_object._move_assign(&other); \
		return *this; \
	} \
	SLIB_INLINE CLASS_NAME(TYPE&& other) \
	{ \
		m_object._move_init(&other); \
	} \
	SLIB_INLINE SAFE_TYPE& operator=(TYPE&& other) \
	{ \
		m_object._move_assign(&other); \
		return *this; \
	} \
	SLIB_INLINE CLASS_NAME(const TYPE& other) : m_object(other.getRef()) \
	{ \
	} \
	SLIB_INLINE SAFE_TYPE& operator=(const TYPE& other) \
	{ \
		m_object = other.getRef(); \
		return *this; \
	}

#define SLIB_DECLARE_OBJECT_SAFE_WRAPPER(CLASS_NAME, OBJ, SAFE_TYPE, TYPE) \
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER_NO_OP(CLASS_NAME, OBJ, SAFE_TYPE, TYPE) \
	SLIB_INLINE sl_bool operator==(const TYPE& other) const \
	{ \
		return m_object == other.getRef(); \
	} \
	SLIB_INLINE sl_bool operator==(const SAFE_TYPE& other) const \
	{ \
		return m_object == other.m_object; \
	} \
	SLIB_INLINE sl_bool operator!=(const TYPE& other) const \
	{ \
		return m_object != other.getRef(); \
	} \
	SLIB_INLINE sl_bool operator!=(const SAFE_TYPE& other) const \
	{ \
		return m_object != other.m_object; \
	}



/*

 Ref Class Definition
 
	Objects are required to inherit from Referable

*/

SLIB_NAMESPACE_BEGIN

class CWeakRef;

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

	CWeakRef* _getWeak();

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
	CWeakRef* m_weak;
	SpinLock m_lockWeak;

public:
	virtual sl_class_type getClassType() const;
	virtual const char* getClassTypeName() const;
	virtual sl_bool checkClassType(sl_class_type type) const;
};

struct _Ref_Const
{
	void* value;
	sl_int32 lock;
};
extern const _Ref_Const _Ref_Null;

template <class ObjectClass>
class SafeRef;

template <class ObjectClass>
class WeakRef;

template <class ObjectClass>
class SafeWeakRef;

template <class ObjectClass>
class SLIB_EXPORT Ref
{
	typedef Ref<ObjectClass> _Type;
private:
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
		}
	}
	
	SLIB_INLINE Ref(_Type&& other)
	{
		_move_init(&other);
	}

	SLIB_INLINE Ref(const _Type& other)
	{
		ObjectClass* object = other.m_object;
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}

	template <class _ObjectClass>
	SLIB_INLINE Ref(Ref<_ObjectClass>&& other)
	{
		_move_init(&other);
	}
	
	template <class _ObjectClass>
	SLIB_INLINE Ref(const Ref<_ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}

	SLIB_INLINE Ref(SafeRef<ObjectClass>&& other)
	{
		_move_init(&other);
	}
	
	Ref(const SafeRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	SLIB_INLINE Ref(SafeRef<_ObjectClass>&& other)
	{
		_move_init(&other);
	}
	
	template <class _ObjectClass>
	Ref(const SafeRef<_ObjectClass>& other);
	
	Ref(const WeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	Ref(const WeakRef<_ObjectClass>& other);
	
	Ref(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	Ref(const SafeWeakRef<_ObjectClass>& other);

	SLIB_INLINE Ref(const ObjectClass* _object)
	{
		ObjectClass* object = (ObjectClass*)_object;
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}

public:
	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_Null)));
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

	SLIB_INLINE ObjectClass* get() const
	{
		return m_object;
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
	SLIB_INLINE _Type& operator=(_Type&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		ObjectClass* object = other.m_object;
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(Ref<_ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const Ref<_ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(SafeRef<ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	_Type& operator=(const SafeRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(SafeRef<_ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	template <class _ObjectClass>
	_Type& operator=(const SafeRef<_ObjectClass>& other);
	
	_Type& operator=(const WeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	_Type& operator=(const WeakRef<_ObjectClass>& other);
	
	_Type& operator=(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	_Type& operator=(const SafeWeakRef<_ObjectClass>& other);

	SLIB_INLINE _Type& operator=(const ObjectClass* other)
	{
		ObjectClass* object = (ObjectClass*)other;
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
public:
	SLIB_INLINE sl_bool operator==(const ObjectClass* other) const
	{
		return (m_object == other);
	}
	
	SLIB_INLINE friend sl_bool operator==(const ObjectClass* a, const _Type& b)
	{
		return (a == b.m_object);
	}

	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return ((void*)m_object == (void*)(other.m_object));
	}

	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other.get()));
	}
	
	sl_bool operator==(const SafeRef<ObjectClass>& other) const;
	
	template <class _ObjectClass>
	sl_bool operator==(const SafeRef<_ObjectClass>& other) const;

public:
	SLIB_INLINE sl_bool operator!=(const ObjectClass* other) const
	{
		return (m_object != other);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const ObjectClass* a, const _Type& b)
	{
		return (a != b.m_object);
	}

	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return ((void*)m_object != (void*)(other.m_object));
	}

	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other.get()));
	}
	
	sl_bool operator!=(const SafeRef<ObjectClass>& other) const;
	
	template <class _ObjectClass>
	sl_bool operator!=(const SafeRef<_ObjectClass>& other) const;
	

public:
	SLIB_INLINE ObjectClass& operator*() const
	{
		return *((ObjectClass*)m_object);
	}

	SLIB_INLINE ObjectClass* operator->() const
	{
		return (ObjectClass*)m_object;
	}

public:
	SLIB_INLINE void _replaceObject(const ObjectClass* object)
	{
		if (m_object) {
			((Referable*)m_object)->decreaseReference();
		}
		m_object = (ObjectClass*)object;
	}
	
	SLIB_INLINE void _move_init(void* _other)
	{
		_Type& other = *((_Type*)_other);
		m_object = other.m_object;
		other.m_object = sl_null;
	}
	
	SLIB_INLINE void _move_assign(void* _other)
	{
		if ((void*)this != _other) {
			_Type& other = *((_Type*)_other);
			_replaceObject(other.m_object);
			other.m_object = sl_null;
		}
	}
};


template <class ObjectClass>
class SLIB_EXPORT SafeRef
{
	typedef SafeRef<ObjectClass> _Type;
private:
	ObjectClass* m_object;
	SpinLock m_lock;

public:
	SLIB_INLINE SafeRef()
	{
		m_object = sl_null;
	}

	SLIB_INLINE ~SafeRef()
	{
		if (m_object) {
			((Referable*)m_object)->decreaseReference();
		}
	}
	
	SLIB_INLINE SafeRef(_Type&& other)
	{
		_move_init(&other);
	}

	SLIB_INLINE SafeRef(const _Type& other)
	{
		ObjectClass* object = other._retainObject();
		m_object = object;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeRef(SafeRef<_ObjectClass>&& other)
	{
		_move_init(&other);
	}

	template <class _ObjectClass>
	SLIB_INLINE SafeRef(const SafeRef<_ObjectClass>& other)
	{
		ObjectClass* object = other._retainObject();
		m_object = object;
	}
	
	SLIB_INLINE SafeRef(Ref<ObjectClass>&& other)
	{
		_move_init(&other);
	}
	
	SLIB_INLINE SafeRef(const Ref<ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeRef(Ref<_ObjectClass>&& other)
	{
		_move_init(&other);
	}

	template <class _ObjectClass>
	SLIB_INLINE SafeRef(const Ref<_ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}
	
	SafeRef(const WeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	SafeRef(const WeakRef<_ObjectClass>& other);
	
	SafeRef(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	SafeRef(const SafeWeakRef<_ObjectClass>& other);

	SLIB_INLINE SafeRef(const ObjectClass* other)
	{
		ObjectClass* object = (ObjectClass*)other;
		if (object) {
			((Referable*)object)->increaseReference();
		}
		m_object = object;
	}
	
public:
	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_Null)));
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
	
	SLIB_INLINE Ref<ObjectClass> lock() const
	{
		ObjectClass* object = _retainObject();
		Ref<ObjectClass> ret = object;
		if (object) {
			((Referable*)object)->decreaseReference();
		}
		return ret;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE static const _Type& from(const SafeRef<_ObjectClass>& other)
	{
		return *((_Type*)((void*)&other));
	}
	
public:
	SLIB_INLINE _Type& operator=(_Type&& other)
	{
		_move_assign(&other);
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		if (this != &other && m_object != other.m_object) {
			ObjectClass* object = other._retainObject();
			_replaceObject(object);
		}
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(SafeRef<_ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const SafeRef<_ObjectClass>& other)
	{
		if (m_object != other.m_object) {
			ObjectClass* object = other._retainObject();
			_replaceObject(object);
		}
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(Ref<ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	SLIB_INLINE _Type& operator=(const Ref<ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(Ref<_ObjectClass>&& other)
	{
		_move_assign(&other);
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const Ref<_ObjectClass>& other)
	{
		ObjectClass* object = other.get();
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
	_Type& operator=(const WeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	_Type& operator=(const WeakRef<_ObjectClass>& other);
	
	_Type& operator=(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	_Type& operator=(const SafeWeakRef<_ObjectClass>& other);

	SLIB_INLINE _Type& operator=(const ObjectClass* other)
	{
		ObjectClass* object = (ObjectClass*)other;
		if (m_object != object) {
			if (object) {
				((Referable*)object)->increaseReference();
			}
			_replaceObject(object);
		}
		return *this;
	}
	
public:
	SLIB_INLINE sl_bool operator==(const ObjectClass* other) const
	{
		return (m_object == other);
	}
	
	SLIB_INLINE friend sl_bool operator==(const ObjectClass* a, const _Type& b)
	{
		return (a == b.m_object);
	}
	
	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return ((void*)m_object == (void*)(other.m_object));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const SafeRef<_ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other._getObject()));
	}
	
	SLIB_INLINE sl_bool operator==(const Ref<ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other.get()));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object == (void*)(other.get()));
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const ObjectClass* other) const
	{
		return (m_object != other);
	}
	
	SLIB_INLINE friend sl_bool operator!=(const ObjectClass* a, const _Type& b)
	{
		return (a != b.m_object);
	}
	
	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return ((void*)m_object != (void*)(other.m_object));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const SafeRef<_ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other._getObject()));
	}
	
	SLIB_INLINE sl_bool operator!=(const Ref<ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other.get()));
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const Ref<_ObjectClass>& other) const
	{
		return ((void*)m_object != (void*)(other.get()));
	}
	
public:
	ObjectClass* _getObject() const
	{
		return m_object;
	}
	
	ObjectClass* _retainObject() const
	{
		if ((void*)this == (void*)(&_Ref_Null)) {
			return sl_null;
		}
		SpinLocker lock(&m_lock);
		ObjectClass* object = m_object;
		if (object) {
			((Referable*)object)->increaseReference();
		}
		return object;
	}
	
	void _replaceObject(const ObjectClass* object)
	{
		ObjectClass* before;
		{
			SpinLocker lock(&m_lock);
			before = m_object;
			m_object = (ObjectClass*)object;
		}
		if (before) {
			((Referable*)before)->decreaseReference();
		}
	}
	
	SLIB_INLINE void _move_init(void* _other)
	{
		_Type& other = *((_Type*)_other);
		m_object = other.m_object;
		other.m_object = sl_null;
	}
	
	SLIB_INLINE void _move_assign(void* _other)
	{
		if ((void*)this != _other) {
			_Type& other = *((_Type*)_other);
			_replaceObject(other.m_object);
			other.m_object = sl_null;
		}
	}

};

class SLIB_EXPORT CWeakRef : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(CWeakRef)
public:
	CWeakRef();

public:
	Referable* m_object;
	SpinLock m_lock;
	
public:
	static CWeakRef* create(const Referable* object);
	
public:
	Ref<Referable> lock();
	
	void release();
};

template <class ObjectClass>
class SLIB_EXPORT WeakRef
{
	typedef WeakRef<ObjectClass> _Type;
private:
	Ref<CWeakRef> m_weak;

public:
	SLIB_INLINE WeakRef()
	{
	}

	SLIB_INLINE WeakRef(const _Type& other) : m_weak(other.m_weak)
	{
	}

	template <class _ObjectClass>
	SLIB_INLINE WeakRef(const WeakRef<_ObjectClass>& other) : m_weak(other.getWeakRef())
	{
	}

	WeakRef(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	WeakRef(const SafeWeakRef<_ObjectClass>& other);
	
	SLIB_INLINE WeakRef(const Ref<ObjectClass>& other)
	{
		_set(other.get());
	}

	template <class _ObjectClass>
	SLIB_INLINE WeakRef(const Ref<_ObjectClass>& other)
	{
		_set(other.get());
	}

	SLIB_INLINE WeakRef(const SafeRef<ObjectClass>& _other)
	{
		Ref<ObjectClass> other(_other.lock());
		_set(other.get());
	}

	template <class _ObjectClass>
	SLIB_INLINE WeakRef(const SafeRef<_ObjectClass>& _other)
	{
		Ref<_ObjectClass> other(_other.lock());
		_set(other.get());
	}

	SLIB_INLINE WeakRef(const ObjectClass* _other)
	{
		Ref<ObjectClass> other(_other);
		_set(other.get());
	}
	
public:
	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_Null)));
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_weak.isNull();
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_weak.isNotNull();
	}
	
	SLIB_INLINE void setNull()
	{
		m_weak.setNull();
	}
	
	SLIB_INLINE const Ref<CWeakRef>& getWeakRef() const
	{
		return m_weak;
	}
	
	SLIB_INLINE Ref<ObjectClass> lock() const
	{
		if (m_weak.isNotNull()) {
			return Ref<ObjectClass>::from(m_weak->lock());
		}
		return Ref<ObjectClass>::null();
	}
	
	SLIB_INLINE static _Type fromReferable(const Referable* referable)
	{
		if (referable) {
			_Type ret;
			if (CWeakRef::checkInstance(referable)) {
				ret.m_weak = (CWeakRef*)referable;
			} else {
				ret.m_weak = ((Referable*)referable)->_getWeak();
			}
			return ret;
		} else {
			return _Type::null();
		}
	}
	
	template <class _ObjectClass>
	SLIB_INLINE static const _Type& from(const WeakRef<_ObjectClass>& other)
	{
		return *((_Type*)((void*)&other));
	}

public:
	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		m_weak = other.m_weak;
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const WeakRef<_ObjectClass>& other)
	{
		m_weak = other.getWeakRef();
		return *this;
	}

	_Type& operator=(const SafeWeakRef<ObjectClass>& other);
	
	template <class _ObjectClass>
	_Type& operator=(const SafeWeakRef<_ObjectClass>& other);

	SLIB_INLINE _Type& operator=(const Ref<ObjectClass>& other)
	{
		_set(other.get());
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const Ref<_ObjectClass>& other)
	{
		_set(other.get());
		return *this;
	}

	SLIB_INLINE _Type& operator=(const SafeRef<ObjectClass>& _other)
	{
		Ref<ObjectClass> other(_other.lock());
		_set(other.get());
		return *this;
	}

	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const SafeRef<_ObjectClass>& _other)
	{
		Ref<_ObjectClass> other(_other.lock());
		_set(other.get());
		return *this;
	}

	SLIB_INLINE _Type& operator=(const ObjectClass* _other)
	{
		Ref<ObjectClass> other(_other);
		_set(other.get());
		return *this;
	}

public:
	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return m_weak == other.m_weak;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const WeakRef<_ObjectClass>& other) const
	{
		return m_weak == other.getWeakRef();
	}
	
	sl_bool operator==(const SafeWeakRef<ObjectClass>& other) const;

	template <class _ObjectClass>
	sl_bool operator==(const SafeWeakRef<_ObjectClass>& other) const;

public:
	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return m_weak != other.m_weak;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const WeakRef<_ObjectClass>& other) const
	{
		return m_weak != other.getWeakRef();
	}
	
	sl_bool operator!=(const SafeWeakRef<ObjectClass>& other) const;
	
	template <class _ObjectClass>
	sl_bool operator!=(const SafeWeakRef<_ObjectClass>& other) const;
	

private:
	SLIB_INLINE void _set(const ObjectClass* object)
	{
		if (object) {
			m_weak = ((Referable*)object)->_getWeak();
		} else {
			m_weak.setNull();
		}
	}
};


template <class ObjectClass>
class SLIB_EXPORT SafeWeakRef
{
	typedef SafeWeakRef<ObjectClass> _Type;
private:
	SafeRef<CWeakRef> m_weak;
	
public:
	SLIB_INLINE SafeWeakRef()
	{
	}
	
	SLIB_INLINE SafeWeakRef(const _Type& other) : m_weak(other.m_weak)
	{
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeWeakRef(const SafeWeakRef<_ObjectClass>& other) : m_weak(other.getWeakRef())
	{
	}
	
	SLIB_INLINE SafeWeakRef(const WeakRef<ObjectClass>& other) : m_weak(other.getWeakRef())
	{
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeWeakRef(const WeakRef<_ObjectClass>& other) : m_weak(other.getWeakRef())
	{
	}
	
	SLIB_INLINE SafeWeakRef(const Ref<ObjectClass>& other)
	{
		_set(other.get());
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeWeakRef(const Ref<_ObjectClass>& other)
	{
		_set(other.get());
	}
	
	SLIB_INLINE SafeWeakRef(const SafeRef<ObjectClass>& _other)
	{
		Ref<ObjectClass> other(_other.lock());
		_set(other.get());
	}
	
	template <class _ObjectClass>
	SLIB_INLINE SafeWeakRef(const SafeRef<_ObjectClass>& _other)
	{
		Ref<_ObjectClass> other(_other.lock());
		_set(other.get());
	}
	
	SLIB_INLINE SafeWeakRef(const ObjectClass* _other)
	{
		Ref<ObjectClass> other(_other);
		_set(other.get());
	}
	
public:
	SLIB_INLINE static const _Type& null()
	{
		return *((_Type*)((void*)(&_Ref_Null)));
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_weak.isNull();
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_weak.isNotNull();
	}
	
	SLIB_INLINE void setNull()
	{
		m_weak.setNull();
	}
	
	SLIB_INLINE const SafeRef<CWeakRef>& getWeakRef() const
	{
		return m_weak;
	}
	
	SLIB_INLINE Ref<ObjectClass> lock() const
	{
		Ref<CWeakRef> weak(m_weak);
		if (weak.isNotNull()) {
			return Ref<ObjectClass>::from(weak->lock());
		}
		return Ref<ObjectClass>::null();
	}
	
	template <class _ObjectClass>
	SLIB_INLINE static const _Type& from(const SafeWeakRef<_ObjectClass>& other)
	{
		return *((_Type*)((void*)&other));
	}
	
public:
	SLIB_INLINE _Type& operator=(const _Type& other)
	{
		m_weak = other.m_weak;
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const SafeWeakRef<_ObjectClass>& other)
	{
		m_weak = other.getWeakRef();
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const WeakRef<ObjectClass>& other)
	{
		m_weak = other.getWeakRef();
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const WeakRef<_ObjectClass>& other)
	{
		m_weak = other.getWeakRef();
	}
	
	SLIB_INLINE _Type& operator=(const Ref<ObjectClass>& other)
	{
		_set(other.get());
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const Ref<_ObjectClass>& other)
	{
		_set(other.get());
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const SafeRef<ObjectClass>& _other)
	{
		Ref<ObjectClass> other(_other.lock());
		_set(other.get());
		return *this;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE _Type& operator=(const SafeRef<_ObjectClass>& _other)
	{
		Ref<_ObjectClass> other(_other.lock());
		_set(other.get());
		return *this;
	}
	
	SLIB_INLINE _Type& operator=(const ObjectClass* _other)
	{
		Ref<ObjectClass> other(_other);
		_set(other.get());
		return *this;
	}
	
	
public:
	SLIB_INLINE sl_bool operator==(const _Type& other) const
	{
		return m_weak == other.m_weak;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const SafeWeakRef<_ObjectClass>& other) const
	{
		return m_weak == other.getWeakRef();
	}
	
	SLIB_INLINE sl_bool operator==(const WeakRef<ObjectClass>& other) const
	{
		return m_weak == other.getWeakRef();
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator==(const WeakRef<_ObjectClass>& other) const
	{
		return m_weak == other.getWeakRef();
	}
	
public:
	SLIB_INLINE sl_bool operator!=(const _Type& other) const
	{
		return m_weak != other.m_weak;
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const SafeWeakRef<_ObjectClass>& other) const
	{
		return m_weak != other.getWeakRef();
	}
	
	SLIB_INLINE sl_bool operator!=(const WeakRef<ObjectClass>& other) const
	{
		return m_weak != other.getWeakRef();
	}
	
	template <class _ObjectClass>
	SLIB_INLINE sl_bool operator!=(const WeakRef<_ObjectClass>& other) const
	{
		return m_weak != other.getWeakRef();
	}
	
private:
	SLIB_INLINE void _set(const ObjectClass* object)
	{
		if (object) {
			m_weak = ((Referable*)object)->_getWeak();
		} else {
			m_weak.setNull();
		}
	}
};

template <class ObjectClass>
SLIB_INLINE Ref<ObjectClass>::Ref(const SafeRef<ObjectClass>& other)
{
	ObjectClass* object = other._retainObject();
	m_object = object;
}

template <class ObjectClass>
SLIB_INLINE Ref<ObjectClass>& Ref<ObjectClass>::operator=(const SafeRef<ObjectClass>& other)
{
	ObjectClass* object = other._retainObject();
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE Ref<ObjectClass>::Ref(const SafeRef<_ObjectClass>& other)
{
	ObjectClass* object = other._retainObject();
	m_object = object;
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE Ref<ObjectClass>& Ref<ObjectClass>::operator=(const SafeRef<_ObjectClass>& other)
{
	ObjectClass* object = other._retainObject();
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
Ref<ObjectClass>::Ref(const WeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
Ref<ObjectClass>& Ref<ObjectClass>::operator=(const WeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
Ref<ObjectClass>::Ref(const WeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
template <class _ObjectClass>
Ref<ObjectClass>& Ref<ObjectClass>::operator=(const WeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
Ref<ObjectClass>::Ref(const SafeWeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
Ref<ObjectClass>& Ref<ObjectClass>::operator=(const SafeWeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
Ref<ObjectClass>::Ref(const SafeWeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
template <class _ObjectClass>
Ref<ObjectClass>& Ref<ObjectClass>::operator=(const SafeWeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
SLIB_INLINE sl_bool Ref<ObjectClass>::operator==(const SafeRef<ObjectClass>& other) const
{
	return m_object == other._getObject();
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE sl_bool Ref<ObjectClass>::operator==(const SafeRef<_ObjectClass>& other) const
{
	return (void*)m_object == (void*)(other._getObject());
}

template <class ObjectClass>
SLIB_INLINE sl_bool Ref<ObjectClass>::operator!=(const SafeRef<ObjectClass>& other) const
{
	return m_object != other._getObject();
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE sl_bool Ref<ObjectClass>::operator!=(const SafeRef<_ObjectClass>& other) const
{
	return (void*)m_object != (void*)(other._getObject());
}


template <class ObjectClass>
SafeRef<ObjectClass>::SafeRef(const WeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
SafeRef<ObjectClass>& SafeRef<ObjectClass>::operator=(const WeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
SafeRef<ObjectClass>::SafeRef(const WeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
template <class _ObjectClass>
SafeRef<ObjectClass>& SafeRef<ObjectClass>::operator=(const WeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
SafeRef<ObjectClass>::SafeRef(const SafeWeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
SafeRef<ObjectClass>& SafeRef<ObjectClass>::operator=(const SafeWeakRef<ObjectClass>& _other)
{
	Ref<ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
SafeRef<ObjectClass>::SafeRef(const SafeWeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	m_object = object;
}

template <class ObjectClass>
template <class _ObjectClass>
SafeRef<ObjectClass>& SafeRef<ObjectClass>::operator=(const SafeWeakRef<_ObjectClass>& _other)
{
	Ref<_ObjectClass> other(_other.lock());
	ObjectClass* object = other.m_object;
	if (object) {
		((Referable*)object)->increaseReference();
	}
	_replaceObject(object);
	return *this;
}


template <class ObjectClass>
SLIB_INLINE WeakRef<ObjectClass>::WeakRef(const SafeWeakRef<ObjectClass>& other) : m_weak(other.getWeakRef())
{
}

template <class ObjectClass>
WeakRef<ObjectClass>& WeakRef<ObjectClass>::operator=(const SafeWeakRef<ObjectClass>& other)
{
	m_weak = other.getWeakRef();
	return *this;
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE WeakRef<ObjectClass>::WeakRef(const SafeWeakRef<_ObjectClass>& other) : m_weak(other.getWeakRef())
{
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE WeakRef<ObjectClass>& WeakRef<ObjectClass>::operator=(const SafeWeakRef<_ObjectClass>& other)
{
	m_weak = other.getWeakRef();
	return *this;
}

template <class ObjectClass>
SLIB_INLINE sl_bool WeakRef<ObjectClass>::operator==(const SafeWeakRef<ObjectClass>& other) const
{
	return m_weak == other.getWeakRef();
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE sl_bool WeakRef<ObjectClass>::operator==(const SafeWeakRef<_ObjectClass>& other) const
{
	return m_weak == other.getWeakRef();
}

template <class ObjectClass>
SLIB_INLINE sl_bool WeakRef<ObjectClass>::operator!=(const SafeWeakRef<ObjectClass>& other) const
{
	return m_weak != other.getWeakRef();
}

template <class ObjectClass>
template <class _ObjectClass>
SLIB_INLINE sl_bool WeakRef<ObjectClass>::operator!=(const SafeWeakRef<_ObjectClass>& other) const
{
	return m_weak != other.getWeakRef();
}


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

#endif
