#ifndef CHECKHEADER_SLIB_CORE_REFERENCE
#define CHECKHEADER_SLIB_CORE_REFERENCE

#include "definition.h"

#include "base.h"
#include "spinlock.h"

#ifdef SLIB_DEBUG
#define SLIB_DEBUG_REFERENCE
#endif

typedef const void* sl_class_type;

SLIB_NAMESPACE_BEGIN

template <class T>
class Ref;

template <class T>
class SafeRef;

template <class T>
class WeakRef;

template <class T>
class SafeWeakRef;


#define SLIB_DECLARE_OBJECT_TYPE \
public: \
	static sl_bool checkInstance(const slib::Referable* object); \
	static sl_class_type ClassType();


#define SLIB_DECLARE_OBJECT \
public: \
	virtual sl_class_type getClassType() const; \
	static sl_bool _checkClassType(sl_class_type type); \
	virtual sl_bool checkClassType(sl_class_type type) const; \
	SLIB_DECLARE_OBJECT_TYPE

class CWeakRef;

class SLIB_EXPORT Referable
{
public:
	Referable();
	
	Referable(const Referable& other);
	
	virtual ~Referable();
	
public:
	sl_reg increaseReference();
	
	sl_reg decreaseReference();
	
	sl_reg decreaseReferenceNoFree();
	
	CWeakRef* getWeakObject();
	
	void makeNeverFree();
	
public:
	virtual sl_class_type getClassType() const;
	
	virtual sl_bool checkClassType(sl_class_type type) const;
	
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
	
};

template <class T>
class SLIB_EXPORT Ref
{
public:
	T* ptr;

public:
	Ref();
	
	Ref(const T* _other);
	
	Ref(Ref<T>&& other);

	Ref(const Ref<T>& other);

	template <class _T>
	Ref(Ref<_T>&& other);
	
	template <class _T>
	Ref(const Ref<_T>& other);

	Ref(SafeRef<T>&& other);
	
	Ref(const SafeRef<T>& other);
	
	template <class _T>
	Ref(SafeRef<_T>&& other);
	
	template <class _T>
	Ref(const SafeRef<_T>& other);
	
	Ref(const WeakRef<T>& other);
	
	template <class _T>
	Ref(const WeakRef<_T>& other);
	
	Ref(const SafeWeakRef<T>& other);
	
	template <class _T>
	Ref(const SafeWeakRef<_T>& other);
	
	~Ref();

public:
	static const Ref<T>& null();

	sl_bool isNull() const;

	sl_bool isNotNull() const;

	void setNull();
	
	template <class _T>
	static const Ref<T>& from(const Ref<_T>& other);
	
	const Ref<Referable>& getReference() const;
	
public:
	Ref<T>& operator=(const T* other);

	Ref<T>& operator=(Ref<T>&& other);
	
	Ref<T>& operator=(const Ref<T>& other);

	template <class _T>
	Ref<T>& operator=(Ref<_T>&& other);
	
	template <class _T>
	Ref<T>& operator=(const Ref<_T>& other);
	
	Ref<T>& operator=(SafeRef<T>&& other);
	
	Ref<T>& operator=(const SafeRef<T>& other);
	
	template <class _T>
	Ref<T>& operator=(SafeRef<_T>&& other);
	
	template <class _T>
	Ref<T>& operator=(const SafeRef<_T>& other);
	
	Ref<T>& operator=(const WeakRef<T>& other);
	
	template <class _T>
	Ref<T>& operator=(const WeakRef<_T>& other);
	
	Ref<T>& operator=(const SafeWeakRef<T>& other);
	
	template <class _T>
	Ref<T>& operator=(const SafeWeakRef<_T>& other);

public:
	sl_bool operator==(const T* other) const;
	
	sl_bool operator==(const Ref<T>& other) const;

	template <class _T>
	sl_bool operator==(const Ref<_T>& other) const;
	
	sl_bool operator==(const SafeRef<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const SafeRef<_T>& other) const;

	sl_bool operator!=(const T* other) const;
	
	sl_bool operator!=(const Ref<T>& other) const;

	template <class _T>
	sl_bool operator!=(const Ref<_T>& other) const;
	
	sl_bool operator!=(const SafeRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const SafeRef<_T>& other) const;

public:
	T& operator*() const;

	T* operator->() const;

public:
	void _replaceObject(const T* other);
	
	void _move_init(void* other);
	
	void _move_init(void* other, T* dummy);

	void _move_assign(void* other);
	
	void _move_assign(void* other, T* dummy);

};


template <class T>
class SLIB_EXPORT SafeRef
{
public:
	T* _ptr;
private:
	SpinLock m_lock;

public:
	SafeRef();
	
	SafeRef(const T* other);
	
	SafeRef(SafeRef<T>&& other);

	SafeRef(const SafeRef<T>& other);
	
	template <class _T>
	SafeRef(SafeRef<_T>&& other);

	template <class _T>
	SafeRef(const SafeRef<_T>& other);
	
	SafeRef(Ref<T>&& other);
	
	SafeRef(const Ref<T>& other);
	
	template <class _T>
	SafeRef(Ref<_T>&& other);

	template <class _T>
	SafeRef(const Ref<_T>& other);
	
	SafeRef(const WeakRef<T>& other);
	
	template <class _T>
	SafeRef(const WeakRef<_T>& other);
	
	SafeRef(const SafeWeakRef<T>& other);
	
	template <class _T>
	SafeRef(const SafeWeakRef<_T>& other);

	~SafeRef();

public:
	static const SafeRef<T>& null();
	
	sl_bool isNull() const;

	sl_bool isNotNull() const;

	void setNull();
	
	template <class _T>
	static const SafeRef<T>& from(const SafeRef<_T>& other);
	
public:
	SafeRef<T>& operator=(const T* other);
	
	SafeRef<T>& operator=(SafeRef<T>&& other);
	
	SafeRef<T>& operator=(const SafeRef<T>& other);
	
	template <class _T>
	SafeRef<T>& operator=(SafeRef<_T>&& other);

	template <class _T>
	SafeRef<T>& operator=(const SafeRef<_T>& other);
	
	SafeRef<T>& operator=(Ref<T>&& other);

	SafeRef<T>& operator=(const Ref<T>& other);
	
	template <class _T>
	SafeRef<T>& operator=(Ref<_T>&& other);

	template <class _T>
	SafeRef<T>& operator=(const Ref<_T>& other);
	
	SafeRef<T>& operator=(const WeakRef<T>& other);
	
	template <class _T>
	SafeRef<T>& operator=(const WeakRef<_T>& other);
	
	SafeRef<T>& operator=(const SafeWeakRef<T>& other);
	
	template <class _T>
	SafeRef<T>& operator=(const SafeWeakRef<_T>& other);

public:
	sl_bool operator==(const T* other) const;
	
	sl_bool operator==(const SafeRef<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const SafeRef<_T>& other) const;
	
	sl_bool operator==(const Ref<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const Ref<_T>& other) const;
	
	sl_bool operator!=(const T* other) const;
	
	sl_bool operator!=(const SafeRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const SafeRef<_T>& other) const;
	
	sl_bool operator!=(const Ref<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const Ref<_T>& other) const;
	
public:
	T* _retainObject() const;
	
	void _replaceObject(const T* other);
	
	void _move_init(void* other);
	
	void _move_init(void* other, T* dummy);

	void _move_assign(void* other);
	
	void _move_assign(void* other, T* dummy);

};


template <class T>
class SLIB_EXPORT WeakRef
{
public:
	Ref<CWeakRef> _weak;

public:
	WeakRef();
	
	WeakRef(const T* other);

	WeakRef(WeakRef<T>&& other);
	
	WeakRef(const WeakRef<T>& other);
	
	template <class _T>
	WeakRef(WeakRef<_T>&& other);

	template <class _T>
	WeakRef(const WeakRef<_T>& other);
	
	WeakRef(SafeWeakRef<T>&& other);

	WeakRef(const SafeWeakRef<T>& other);
	
	template <class _T>
	WeakRef(SafeWeakRef<_T>&& other);
	
	template <class _T>
	WeakRef(const SafeWeakRef<_T>& other);
	
	WeakRef(const Ref<T>& other);

	template <class _T>
	WeakRef(const Ref<_T>& other);

	WeakRef(const SafeRef<T>& other);
	
	template <class _T>
	WeakRef(const SafeRef<_T>& other);
	
public:
	static const WeakRef<T>& null();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();
	
	Ref<T> lock() const;
	
	static WeakRef<T> fromReferable(const Referable* referable);
	
	template <class _T>
	static const WeakRef<T>& from(const WeakRef<_T>& other);
	
public:
	WeakRef<T>& operator=(const T* other);

	WeakRef<T>& operator=(WeakRef<T>&& other);
	
	WeakRef<T>& operator=(const WeakRef<T>& other);
	
	template <class _T>
	WeakRef<T>& operator=(WeakRef<_T>&& other);

	template <class _T>
	WeakRef<T>& operator=(const WeakRef<_T>& other);
	
	WeakRef<T>& operator=(SafeWeakRef<T>&& other);
	
	WeakRef<T>& operator=(const SafeWeakRef<T>& other);
	
	template <class _T>
	WeakRef<T>& operator=(SafeWeakRef<_T>&& other);
	
	template <class _T>
	WeakRef<T>& operator=(const SafeWeakRef<_T>& other);

	WeakRef<T>& operator=(const Ref<T>& other);

	template <class _T>
	WeakRef<T>& operator=(const Ref<_T>& other);

	WeakRef<T>& operator=(const SafeRef<T>& other);
	
	template <class _T>
	WeakRef<T>& operator=(const SafeRef<_T>& other);
	
public:
	sl_bool operator==(const WeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const WeakRef<_T>& other) const;
	
	sl_bool operator==(const SafeWeakRef<T>& other) const;

	template <class _T>
	sl_bool operator==(const SafeWeakRef<_T>& other) const;

	sl_bool operator!=(const WeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const WeakRef<_T>& other) const;
	
	sl_bool operator!=(const SafeWeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const SafeWeakRef<_T>& other) const;

private:
	void _set(const T* other);
	
};


template <class T>
class SLIB_EXPORT SafeWeakRef
{
public:
	SafeRef<CWeakRef> _weak;
	
public:
	SafeWeakRef();
	
	SafeWeakRef(const T* other);

	SafeWeakRef(SafeWeakRef<T>&& other);
	
	SafeWeakRef(const SafeWeakRef<T>& other);
	
	template <class _T>
	SafeWeakRef(SafeWeakRef<_T>&& other);
	
	template <class _T>
	SafeWeakRef(const SafeWeakRef<_T>& other);
	
	SafeWeakRef(WeakRef<T>&& other);
	
	SafeWeakRef(const WeakRef<T>& other);
	
	template <class _T>
	SafeWeakRef(WeakRef<_T>&& other);
	
	template <class _T>
	SafeWeakRef(const WeakRef<_T>& other);
	
	SafeWeakRef(const Ref<T>& other);
	
	template <class _T>
	SafeWeakRef(const Ref<_T>& other);
	
	SafeWeakRef(const SafeRef<T>& other);
	
	template <class _T>
	SafeWeakRef(const SafeRef<_T>& other);
	
public:
	static const SafeWeakRef<T>& null();
	
	sl_bool isNull() const;
	
	sl_bool isNotNull() const;
	
	void setNull();
	
	Ref<T> lock() const;
	
	template <class _T>
	static const SafeWeakRef<T>& from(const SafeWeakRef<_T>& other);
	
public:
	SafeWeakRef<T>& operator=(const T* other);

	SafeWeakRef<T>& operator=(SafeWeakRef<T>&& other);
	
	SafeWeakRef<T>& operator=(const SafeWeakRef<T>& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(SafeWeakRef<_T>&& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(const SafeWeakRef<_T>& other);
	
	SafeWeakRef<T>& operator=(WeakRef<T>&& other);
	
	SafeWeakRef<T>& operator=(const WeakRef<T>& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(WeakRef<_T>&& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(const WeakRef<_T>& other);
	
	SafeWeakRef<T>& operator=(const Ref<T>& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(const Ref<_T>& other);
	
	SafeWeakRef<T>& operator=(const SafeRef<T>& other);
	
	template <class _T>
	SafeWeakRef<T>& operator=(const SafeRef<_T>& other);
	
public:
	sl_bool operator==(const SafeWeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const SafeWeakRef<_T>& other) const;
	
	sl_bool operator==(const WeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator==(const WeakRef<_T>& other) const;
	
	sl_bool operator!=(const SafeWeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const SafeWeakRef<_T>& other) const;
	
	sl_bool operator!=(const WeakRef<T>& other) const;
	
	template <class _T>
	sl_bool operator!=(const WeakRef<_T>& other) const;
	
private:
	void _set(const T* other);
	
};


class SLIB_EXPORT CWeakRef : public Referable
{
	SLIB_DECLARE_OBJECT
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

struct _Ref_Const
{
	void* ptr;
	sl_int32 lock;
};

extern const _Ref_Const _Ref_Null;

class SLIB_EXPORT _ReferableConstructor
{
public:
	Referable* m_object;
	_ReferableConstructor(Referable* object)
	{
		m_object = object;
		m_object->increaseReference();
	}
	~_ReferableConstructor()
	{
		m_object->decreaseReferenceNoFree();
	}
};

SLIB_NAMESPACE_END

#define SLIB_REFERABLE_CONSTRUCTOR slib::_ReferableConstructor _slib_referable_constructor(this);

#define SLIB_NEW_REF(CLASS, NAME, ...) \
	CLASS* NAME = new CLASS(__VA_ARGS__); \
	Ref<CLASS> _ref_##NAME = NAME;


#define SLIB_DEFINE_OBJECT_TYPE(CLASS) \
    char _g_classId_##CLASS[] = #CLASS; \
	sl_class_type CLASS::ClassType() \
	{ \
		return _g_classId_##CLASS; \
	} \
	sl_bool CLASS::checkInstance(const slib::Referable* object) \
	{ \
		if (object) { \
			return object->checkClassType(CLASS::ClassType()); \
		} else { \
			return sl_false; \
		} \
	}

#define SLIB_DEFINE_OBJECT_TYPE_FROM(CLASS, FROM) \
	sl_class_type CLASS::ClassType() \
	{ \
		return FROM::ClassType(); \
	} \
	sl_bool CLASS::checkInstance(const slib::Referable* object) \
	{ \
		return FROM::checkInstance(object); \
	}

#define SLIB_DEFINE_ROOT_OBJECT(CLASS) \
	sl_class_type CLASS::getClassType() const \
	{ \
		return CLASS::ClassType(); \
	} \
	sl_bool CLASS::_checkClassType(sl_class_type type) \
	{ \
		return type == CLASS::ClassType(); \
	} \
	sl_bool CLASS::checkClassType(sl_class_type type) const \
	{ \
		return _checkClassType(type); \
	} \
	SLIB_DEFINE_OBJECT_TYPE(CLASS)

#define SLIB_DEFINE_OBJECT(CLASS, BASE) \
	sl_class_type CLASS::getClassType() const \
	{ \
		return CLASS::ClassType(); \
	} \
	sl_bool CLASS::_checkClassType(sl_class_type type) \
	{ \
		if (type == CLASS::ClassType()) { \
			return sl_true; \
		} else { \
			return BASE::_checkClassType(type); \
		} \
	} \
	sl_bool CLASS::checkClassType(sl_class_type type) const \
	{ \
		return _checkClassType(type); \
	} \
	SLIB_DEFINE_OBJECT_TYPE(CLASS)



#define SLIB_DEFINE_TEMPLATE_OBJECT(CLASS, BASE, ID) \
	template <SLIB_TEMPLATE_DEF_PARAMS_##CLASS> \
	sl_class_type CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::ClassType() \
	{ \
		return ID; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##CLASS> \
	sl_bool CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::checkInstance(const slib::Referable* object) \
	{ \
		if (object) { \
			return object->checkClassType(CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::ClassType()); \
		} else { \
			return sl_false; \
		} \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##CLASS> \
	sl_class_type CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::getClassType() const \
	{ \
		return CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::ClassType(); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##CLASS> \
	sl_bool CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::_checkClassType(sl_class_type type) \
	{ \
		if (type == CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::ClassType()) { \
			return sl_true; \
		} else { \
			return BASE::_checkClassType(type); \
		} \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##CLASS> \
	sl_bool CLASS<SLIB_TEMPLATE_PARAMS_##CLASS>::checkClassType(sl_class_type type) const \
	{ \
		return _checkClassType(type); \
	}


#define SLIB_DECLARE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ) \
public: \
	TYPE(); \
	TYPE(const OBJ* object); \
	TYPE(const TYPE& other); \
	TYPE(TYPE&& other); \
	TYPE(const OTHER_TYPE& other); \
	TYPE(OTHER_TYPE&& other); \
	static const TYPE& null(); \
	sl_bool isNull() const; \
	sl_bool isNotNull() const; \
	void setNull(); \
	TYPE& operator=(const OBJ* object); \
	TYPE& operator=(const TYPE& other); \
	TYPE& operator=(TYPE&& other); \
	TYPE& operator=(const OTHER_TYPE& other); \
	TYPE& operator=(OTHER_TYPE&& other); \
	SLIB_DECLARE_OBJECT_TYPE

#define SLIB_DECLARE_REF_WRAPPER(TYPE, OTHER_TYPE, OBJ) \
	SLIB_DECLARE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ) \
	sl_bool operator==(const TYPE& other) const; \
	sl_bool operator==(const OTHER_TYPE& other) const; \
	sl_bool operator!=(const TYPE& other) const; \
	sl_bool operator!=(const OTHER_TYPE& other) const;


#define SLIB_DEFINE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ, NAME) \
	TYPE::TYPE() {} \
	TYPE::TYPE(const OBJ* other) : NAME(other) \
	{ \
	} \
	TYPE::TYPE(const TYPE& other) : NAME(other.NAME) \
	{ \
	} \
	TYPE::TYPE(TYPE&& other) \
	{ \
		NAME._move_init(&other); \
	} \
	TYPE::TYPE(const OTHER_TYPE& other) : NAME(other.NAME) \
	{ \
	} \
	TYPE::TYPE(OTHER_TYPE&& other) \
	{ \
		NAME._move_init(&other); \
	} \
	const TYPE& TYPE::null() \
	{ \
		return *((TYPE*)((void*)(&_Ref_Null))); \
	} \
	sl_bool TYPE::isNull() const \
	{ \
		return NAME.isNull(); \
	} \
	sl_bool TYPE::isNotNull() const \
	{ \
		return NAME.isNotNull(); \
	} \
	void TYPE::setNull() \
	{ \
		NAME.setNull(); \
	} \
	TYPE& TYPE::operator=(const OBJ* other) \
	{ \
		NAME = other; \
		return *this; \
	} \
	TYPE& TYPE::operator=(const TYPE& other) \
	{ \
		NAME = other.NAME; \
		return *this; \
	} \
	TYPE& TYPE::operator=(TYPE&& other) \
	{ \
		NAME._move_assign(&other); \
		return *this; \
	} \
	TYPE& TYPE::operator=(const OTHER_TYPE& other) \
	{ \
		NAME = other.NAME; \
		return *this; \
	} \
	TYPE& TYPE::operator=(OTHER_TYPE&& other) \
	{ \
		NAME._move_assign(&other); \
		return *this; \
	} \
	SLIB_DEFINE_OBJECT_TYPE_FROM(TYPE, OBJ)


#define SLIB_DEFINE_REF_WRAPPER(TYPE, OTHER_TYPE, OBJ, NAME) \
	SLIB_DEFINE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ, NAME) \
	sl_bool TYPE::operator==(const TYPE& other) const \
	{ \
		return NAME == other.NAME; \
	} \
	sl_bool TYPE::operator==(const OTHER_TYPE& other) const \
	{ \
		return NAME == other.NAME; \
	} \
	sl_bool TYPE::operator!=(const TYPE& other) const \
	{ \
		return NAME != other.NAME; \
	} \
	sl_bool TYPE::operator!=(const OTHER_TYPE& other) const \
	{ \
		return NAME != other.NAME; \
	}


#define SLIB_DECLARE_TEMPLATE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ) \
public: \
	TYPE(); \
	TYPE(const OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>* object); \
	TYPE(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other); \
	TYPE(TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other); \
	TYPE(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other); \
	TYPE(OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other); \
	static const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& null(); \
	sl_bool isNull() const; \
	sl_bool isNotNull() const; \
	void setNull(); \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& operator=(const OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>* object); \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& operator=(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other); \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& operator=(TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other); \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& operator=(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other); \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& operator=(OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other); \
	SLIB_DECLARE_OBJECT_TYPE

#define SLIB_DECLARE_TEMPLATE_REF_WRAPPER(TYPE, OTHER_TYPE, OBJ) \
SLIB_DECLARE_TEMPLATE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ) \
	sl_bool operator==(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const; \
	sl_bool operator==(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const; \
	sl_bool operator!=(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const; \
	sl_bool operator!=(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const;


#define SLIB_DEFINE_TEMPLATE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ, NAME) \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE() \
	{ \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE(const OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>* other) : NAME(other) \
	{ \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) : NAME(other.NAME) \
	{ \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE(TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other) \
	{ \
		NAME._move_init(&other); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) : NAME(other.NAME) \
	{ \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::TYPE(OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other) \
	{ \
		NAME._move_init(&other); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::null() \
	{ \
		return *((TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>*)((void*)(&_Ref_Null))); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::isNull() const \
	{ \
		return NAME.isNull(); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::isNotNull() const \
	{ \
		return NAME.isNotNull(); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	void TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::setNull() \
	{ \
		NAME.setNull(); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator=(const OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>* other) \
	{ \
		NAME = other; \
		return *this; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator=(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) \
	{ \
		NAME = other.NAME; \
		return *this; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator=(TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other) \
	{ \
		NAME._move_assign(&other); \
		return *this; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator=(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) \
	{ \
		NAME = other.NAME; \
		return *this; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator=(OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>&& other) \
	{ \
		NAME._move_assign(&other); \
		return *this; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_class_type TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::ClassType() \
	{ \
		return OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>::ClassType(); \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::checkInstance(const slib::Referable* object) \
	{ \
		return OBJ<SLIB_TEMPLATE_PARAMS_##OBJ>::checkInstance(object); \
	}


#define SLIB_DEFINE_TEMPLATE_REF_WRAPPER(TYPE, OTHER_TYPE, OBJ, NAME) \
	SLIB_DEFINE_TEMPLATE_REF_WRAPPER_NO_OP(TYPE, OTHER_TYPE, OBJ, NAME) \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator==(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const \
	{ \
		return NAME == other.NAME; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator==(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const \
	{ \
		return NAME == other.NAME; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator!=(const TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const \
	{ \
		return NAME != other.NAME; \
	} \
	template <SLIB_TEMPLATE_DEF_PARAMS_##OBJ> \
	sl_bool TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>::operator!=(const OTHER_TYPE<SLIB_TEMPLATE_PARAMS_##OBJ>& other) const \
	{ \
		return NAME != other.NAME; \
	}


SLIB_NAMESPACE_BEGIN

template <class T>
SLIB_INLINE Ref<T>::Ref() : ptr(sl_null)
{
}

template <class T>
SLIB_INLINE Ref<T>::Ref(const T* other)
{
	T* o = (T*)other;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
SLIB_INLINE Ref<T>::Ref(Ref<T>&& other)
{
	_move_init(&other);
}

template <class T>
SLIB_INLINE Ref<T>::Ref(const Ref<T>& other)
{
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>::Ref(Ref<_T>&& other)
{
	_move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>::Ref(const Ref<_T>& other)
{
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
SLIB_INLINE Ref<T>::Ref(SafeRef<T>&& other)
{
	_move_init(&other);
}

template <class T>
Ref<T>::Ref(const SafeRef<T>& other)
{
	T* o = other._retainObject();
	ptr = o;
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>::Ref(SafeRef<_T>&& other)
{
	_move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
Ref<T>::Ref(const SafeRef<_T>& other)
{
	T* o = other._retainObject();
	ptr = o;
}

template <class T>
Ref<T>::Ref(const WeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
template <class _T>
Ref<T>::Ref(const WeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
template <class _T>
Ref<T>::Ref(const SafeWeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
Ref<T>::Ref(const SafeWeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	ptr = o;
}

template <class T>
SLIB_INLINE Ref<T>::~Ref()
{
	if (ptr) {
		((Referable*)ptr)->decreaseReference();
	}
}

template <class T>
SLIB_INLINE const Ref<T>& Ref<T>::null()
{
	return *((Ref<T>*)((void*)(&_Ref_Null)));
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::isNull() const
{
	return (ptr == sl_null);
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::isNotNull() const
{
	return (ptr != sl_null);
}

template <class T>
SLIB_INLINE void Ref<T>::setNull()
{
	_replaceObject(sl_null);
}

template <class T>
template <class _T>
SLIB_INLINE const Ref<T>& Ref<T>::from(const Ref<_T>& other)
{
	return *((Ref<T>*)((void*)&other));
}

template <class T>
SLIB_INLINE const Ref<Referable>& Ref<T>::getReference() const
{
	return *((Ref<Referable>*)((void*)this));
}

template <class T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(const T* other)
{
	T* o = (T*)other;
	if (ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(Ref<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(const Ref<T>& other)
{
	T* o = other.ptr;
	if (ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(Ref<_T>&& other)
{
	_move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(const Ref<_T>& other)
{
	T* o = other.ptr;
	if (ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(SafeRef<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
Ref<T>& Ref<T>::operator=(const SafeRef<T>& other)
{
	T* o = other._retainObject();
	_replaceObject(o);
	return *this;
}

template <class T>
template <class _T>
SLIB_INLINE Ref<T>& Ref<T>::operator=(SafeRef<_T>&& other)
{
	_move_assign(&other, (_T*)(0));
	return *this;
}


template <class T>
template <class _T>
Ref<T>& Ref<T>::operator=(const SafeRef<_T>& other)
{
	T* o = other._retainObject();
	_replaceObject(o);
	return *this;
}

template <class T>
Ref<T>& Ref<T>::operator=(const WeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
template <class _T>
Ref<T>& Ref<T>::operator=(const WeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
Ref<T>& Ref<T>::operator=(const SafeWeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
template <class _T>
Ref<T>& Ref<T>::operator=(const SafeWeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator==(const T* other) const
{
	return ptr == other;
}

template <class T>
SLIB_INLINE sl_bool operator==(const T* a, const Ref<T>& b)
{
	return a == b.ptr;
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator==(const Ref<T>& other) const
{
	return (void*)ptr == (void*)(other.ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool Ref<T>::operator==(const Ref<_T>& other) const
{
	return (void*)ptr == (void*)(other.ptr);
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator==(const SafeRef<T>& other) const
{
	return ptr == other._ptr;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool Ref<T>::operator==(const SafeRef<_T>& other) const
{
	return (void*)ptr == (void*)(other._ptr);
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator!=(const T* other) const
{
	return ptr != other;
}

template <class T>
SLIB_INLINE sl_bool operator!=(const T* a, const Ref<T>& b)
{
	return a != b.ptr;
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator!=(const Ref<T>& other) const
{
	return (void*)ptr != (void*)(other.ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool Ref<T>::operator!=(const Ref<_T>& other) const
{
	return (void*)ptr != (void*)(other.ptr);
}

template <class T>
SLIB_INLINE sl_bool Ref<T>::operator!=(const SafeRef<T>& other) const
{
	return ptr != other._ptr;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool Ref<T>::operator!=(const SafeRef<_T>& other) const
{
	return (void*)ptr != (void*)(other._ptr);
}

template <class T>
SLIB_INLINE T& Ref<T>::operator*() const
{
	return *((T*)ptr);
}

template <class T>
SLIB_INLINE T* Ref<T>::operator->() const
{
	return (T*)ptr;
}

template <class T>
SLIB_INLINE void Ref<T>::_replaceObject(const T* other)
{
	if (ptr) {
		((Referable*)ptr)->decreaseReference();
	}
	ptr = (T*)other;
}

template <class T>
SLIB_INLINE void Ref<T>::_move_init(void* _other)
{
	Ref<T>& other = *((Ref<T>*)_other);
	ptr = other.ptr;
	other.ptr = sl_null;
}

template <class T>
SLIB_INLINE void Ref<T>::_move_init(void* _other, T* dummy)
{
	Ref<T>& other = *((Ref<T>*)_other);
	ptr = other.ptr;
	other.ptr = sl_null;
}

template <class T>
SLIB_INLINE void Ref<T>::_move_assign(void* _other)
{
	if ((void*)this != _other) {
		Ref<T>& other = *((Ref<T>*)_other);
		_replaceObject(other.ptr);
		other.ptr = sl_null;
	}
}

template <class T>
SLIB_INLINE void Ref<T>::_move_assign(void* _other, T* dummy)
{
	if ((void*)this != _other) {
		Ref<T>& other = *((Ref<T>*)_other);
		_replaceObject(other.ptr);
		other.ptr = sl_null;
	}
}


template <class T>
SLIB_INLINE SafeRef<T>::SafeRef() : _ptr(sl_null)
{
}

template <class T>
SLIB_INLINE SafeRef<T>::SafeRef(const T* other)
{
	T* o = (T*)other;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
SLIB_INLINE SafeRef<T>::SafeRef(SafeRef<T>&& other)
{
	_move_init(&other);
}

template <class T>
SafeRef<T>::SafeRef(const SafeRef<T>& other)
{
	T* o = other._retainObject();
	_ptr = o;
}

template <class T>
template <class _T>
SLIB_INLINE SafeRef<T>::SafeRef(SafeRef<_T>&& other)
{
	_move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SafeRef<T>::SafeRef(const SafeRef<_T>& other)
{
	T* o = other._retainObject();
	_ptr = o;
}

template <class T>
SLIB_INLINE SafeRef<T>::SafeRef(Ref<T>&& other)
{
	_move_init(&other);
}

template <class T>
SLIB_INLINE SafeRef<T>::SafeRef(const Ref<T>& other)
{
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
template <class _T>
SLIB_INLINE SafeRef<T>::SafeRef(Ref<_T>&& other)
{
	_move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SLIB_INLINE SafeRef<T>::SafeRef(const Ref<_T>& other)
{
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
SafeRef<T>::SafeRef(const WeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
template <class _T>
SafeRef<T>::SafeRef(const WeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
SafeRef<T>::SafeRef(const SafeWeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
template <class _T>
SafeRef<T>::SafeRef(const SafeWeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_ptr = o;
}

template <class T>
SLIB_INLINE SafeRef<T>::~SafeRef()
{
	if (_ptr) {
		((Referable*)_ptr)->decreaseReference();
	}
}

template <class T>
SLIB_INLINE const SafeRef<T>& SafeRef<T>::null()
{
	return *((SafeRef<T>*)((void*)(&_Ref_Null)));
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::isNull() const
{
	return (_ptr == sl_null);
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::isNotNull() const
{
	return (_ptr != sl_null);
}

template <class T>
void SafeRef<T>::setNull()
{
	_replaceObject(sl_null);
}

template <class T>
template <class _T>
SLIB_INLINE const SafeRef<T>& SafeRef<T>::from(const SafeRef<_T>& other)
{
	return *((SafeRef<T>*)((void*)&other));
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(const T* other)
{
	T* o = (T*)other;
	if (_ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(SafeRef<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(const SafeRef<T>& other)
{
	if (_ptr != other._ptr) {
		T* o = other._retainObject();
		_replaceObject(o);
	}
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(SafeRef<_T>&& other)
{
	_move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(const SafeRef<_T>& other)
{
	if (_ptr != other._ptr) {
		T* o = other._retainObject();
		_replaceObject(o);
	}
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(Ref<T>&& other)
{
	_move_assign(&other);
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(const Ref<T>& other)
{
	T* o = other.ptr;
	if (_ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(Ref<_T>&& other)
{
	_move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(const Ref<_T>& other)
{
	T* o = other.ptr;
	if (_ptr != o) {
		if (o) {
			((Referable*)o)->increaseReference();
		}
		_replaceObject(o);
	}
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(const WeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(const WeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
SafeRef<T>& SafeRef<T>::operator=(const SafeWeakRef<T>& _other)
{
	Ref<T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
template <class _T>
SafeRef<T>& SafeRef<T>::operator=(const SafeWeakRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	T* o = other.ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	_replaceObject(o);
	return *this;
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator==(const T* other) const
{
	return _ptr == other;
}

template <class T>
SLIB_INLINE sl_bool operator==(const T* a, const SafeRef<T>& b)
{
	return a == b._ptr;
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator==(const SafeRef<T>& other) const
{
	return (void*)_ptr == (void*)(other._ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeRef<T>::operator==(const SafeRef<_T>& other) const
{
	return (void*)_ptr == (void*)(other._ptr);
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator==(const Ref<T>& other) const
{
	return (void*)_ptr == (void*)(other.ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeRef<T>::operator==(const Ref<_T>& other) const
{
	return (void*)_ptr == (void*)(other.ptr);
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator!=(const T* other) const
{
	return _ptr != other;
}

template <class T>
SLIB_INLINE sl_bool operator!=(const T* a, const SafeRef<T>& b)
{
	return a != b._ptr;
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator!=(const SafeRef<T>& other) const
{
	return (void*)_ptr != (void*)(other._ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeRef<T>::operator!=(const SafeRef<_T>& other) const
{
	return (void*)_ptr != (void*)(other._ptr);
}

template <class T>
SLIB_INLINE sl_bool SafeRef<T>::operator!=(const Ref<T>& other) const
{
	return (void*)_ptr != (void*)(other.ptr);
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeRef<T>::operator!=(const Ref<_T>& other) const
{
	return (void*)_ptr != (void*)(other.ptr);
}

template <class T>
SLIB_INLINE T* SafeRef<T>::_retainObject() const
{
	if ((void*)this == (void*)(&_Ref_Null)) {
		return sl_null;
	}
	SpinLocker lock(&m_lock);
	T* o = _ptr;
	if (o) {
		((Referable*)o)->increaseReference();
	}
	return o;
}

template <class T>
SLIB_INLINE void SafeRef<T>::_replaceObject(const T* other)
{
	T* before;
	{
		SpinLocker lock(&m_lock);
		before = _ptr;
		_ptr = (T*)other;
	}
	if (before) {
		((Referable*)before)->decreaseReference();
	}
}

template <class T>
SLIB_INLINE void SafeRef<T>::_move_init(void* _other)
{
	SafeRef<T>& other = *((SafeRef<T>*)_other);
	_ptr = other._ptr;
	other._ptr = sl_null;
}

template <class T>
SLIB_INLINE void SafeRef<T>::_move_init(void* _other, T* dummy)
{
	SafeRef<T>& other = *((SafeRef<T>*)_other);
	_ptr = other._ptr;
	other._ptr = sl_null;
}

template <class T>
SLIB_INLINE void SafeRef<T>::_move_assign(void* _other)
{
	if ((void*)this != _other) {
		SafeRef<T>& other = *((SafeRef<T>*)_other);
		_replaceObject(other._ptr);
		other._ptr = sl_null;
	}
}

template <class T>
SLIB_INLINE void SafeRef<T>::_move_assign(void* _other, T* dummy)
{
	if ((void*)this != _other) {
		SafeRef<T>& other = *((SafeRef<T>*)_other);
		_replaceObject(other._ptr);
		other._ptr = sl_null;
	}
}


template <class T>
SLIB_INLINE WeakRef<T>::WeakRef()
{
}

template <class T>
SLIB_INLINE WeakRef<T>::WeakRef(const T* _other)
{
	Ref<T> other(_other);
	_set(other.ptr);
}

template <class T>
SLIB_INLINE WeakRef<T>::WeakRef(WeakRef<T>&& other)
{
	_weak._move_init(&other);
}

template <class T>
SLIB_INLINE WeakRef<T>::WeakRef(const WeakRef<T>& other) : _weak(other._weak)
{
}

template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>::WeakRef(WeakRef<_T>&& other)
{
	_weak._move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>::WeakRef(const WeakRef<_T>& other) : _weak(other._weak)
{
}

template <class T>
SLIB_INLINE WeakRef<T>::WeakRef(SafeWeakRef<T>&& other)
{
	_weak._move_init(&other);
}


template <class T>
WeakRef<T>::WeakRef(const SafeWeakRef<T>& other) : _weak(other._weak)
{
}

template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>::WeakRef(SafeWeakRef<_T>&& other)
{
	_weak._move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
WeakRef<T>::WeakRef(const SafeWeakRef<_T>& other) : _weak(other._weak)
{
}

template <class T>
WeakRef<T>::WeakRef(const Ref<T>& other)
{
	_set(other.ptr);
}

template <class T>
template <class _T>
WeakRef<T>::WeakRef(const Ref<_T>& other)
{
	_set(other.ptr);
}

template <class T>
WeakRef<T>::WeakRef(const SafeRef<T>& _other)
{
	Ref<T> other(_other.lock());
	_set(other.ptr);
}

template <class T>
template <class _T>
WeakRef<T>::WeakRef(const SafeRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	_set(other.ptr);
}

template <class T>
SLIB_INLINE const WeakRef<T>& WeakRef<T>::null()
{
	return *((WeakRef<T>*)((void*)(&_Ref_Null)));
}

template <class T>
SLIB_INLINE sl_bool WeakRef<T>::isNull() const
{
	return _weak.isNull();
}

template <class T>
SLIB_INLINE sl_bool WeakRef<T>::isNotNull() const
{
	return _weak.isNotNull();
}

template <class T>
SLIB_INLINE void WeakRef<T>::setNull()
{
	_weak.setNull();
}

template <class T>
Ref<T> WeakRef<T>::lock() const
{
	if (_weak.isNotNull()) {
		return Ref<T>::from(_weak->lock());
	}
	return Ref<T>::null();
}

template <class T>
WeakRef<T> WeakRef<T>::fromReferable(const Referable* referable)
{
	if (referable) {
		WeakRef<T> ret;
		if (CWeakRef::checkInstance(referable)) {
			ret._weak = (CWeakRef*)referable;
		} else {
			ret._weak = ((Referable*)referable)->getWeakObject();
		}
		return ret;
	} else {
		return WeakRef<T>::null();
	}
}

template <class T>
template <class _T>
SLIB_INLINE const WeakRef<T>& WeakRef<T>::from(const WeakRef<_T>& other)
{
	return *((WeakRef<T>*)((void*)&other));
}

template <class T>
WeakRef<T>& WeakRef<T>::operator=(const T* _other)
{
	Ref<T> other(_other);
	_set(other.ptr);
	return *this;
}

template <class T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(WeakRef<T>&& other)
{
	_weak._move_assign(&other);
	return *this;
}

template <class T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(const WeakRef<T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(WeakRef<_T>&& other)
{
	_weak._move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(const WeakRef<_T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(SafeWeakRef<T>&& other)
{
	_weak._move_assign(&other);
	return *this;
}


template <class T>
WeakRef<T>& WeakRef<T>::operator=(const SafeWeakRef<T>& other)
{
	_weak = other._weak;
	return *this;
}


template <class T>
template <class _T>
SLIB_INLINE WeakRef<T>& WeakRef<T>::operator=(SafeWeakRef<_T>&& other)
{
	_weak._move_assign(&other, (_T*)(0));
	return *this;
}


template <class T>
template <class _T>
WeakRef<T>& WeakRef<T>::operator=(const SafeWeakRef<_T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
WeakRef<T>& WeakRef<T>::operator=(const Ref<T>& other)
{
	_set(other.ptr);
	return *this;
}

template <class T>
template <class _T>
WeakRef<T>& WeakRef<T>::operator=(const Ref<_T>& other)
{
	_set(other.ptr);
	return *this;
}

template <class T>
WeakRef<T>& WeakRef<T>::operator=(const SafeRef<T>& _other)
{
	Ref<T> other(_other.lock());
	_set(other.ptr);
	return *this;
}

template <class T>
template <class _T>
WeakRef<T>& WeakRef<T>::operator=(const SafeRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	_set(other.ptr);
	return *this;
}

template <class T>
SLIB_INLINE sl_bool WeakRef<T>::operator==(const WeakRef<T>& other) const
{
	return _weak == other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool WeakRef<T>::operator==(const WeakRef<_T>& other) const
{
	return _weak == other._weak;
}


template <class T>
SLIB_INLINE sl_bool WeakRef<T>::operator==(const SafeWeakRef<T>& other) const
{
	return _weak == other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool WeakRef<T>::operator==(const SafeWeakRef<_T>& other) const
{
	return _weak == other._weak;
}


template <class T>
SLIB_INLINE sl_bool WeakRef<T>::operator!=(const WeakRef<T>& other) const
{
	return _weak != other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool WeakRef<T>::operator!=(const WeakRef<_T>& other) const
{
	return _weak != other._weak;
}


template <class T>
SLIB_INLINE sl_bool WeakRef<T>::operator!=(const SafeWeakRef<T>& other) const
{
	return _weak != other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool WeakRef<T>::operator!=(const SafeWeakRef<_T>& other) const
{
	return _weak != other._weak;
}

template <class T>
SLIB_INLINE void WeakRef<T>::_set(const T* object)
{
	if (object) {
		_weak = ((Referable*)object)->getWeakObject();
	} else {
		_weak.setNull();
	}
}


template <class T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef()
{
}

template <class T>
SafeWeakRef<T>::SafeWeakRef(const T* _other)
{
	Ref<T> other(_other);
	_set(other.ptr);
}

template <class T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(SafeWeakRef<T>&& other)
{
	_weak._move_init(&other);
}

template <class T>
SafeWeakRef<T>::SafeWeakRef(const SafeWeakRef<T>& other) : _weak(other._weak)
{
}

template <class T>
template <class _T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(SafeWeakRef<_T>&& other)
{
	_weak._move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SafeWeakRef<T>::SafeWeakRef(const SafeWeakRef<_T>& other) : _weak(other._weak)
{
}

template <class T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(WeakRef<T>&& other)
{
	_weak._move_init(&other);
}

template <class T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(const WeakRef<T>& other) : _weak(other._weak)
{
}

template <class T>
template <class _T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(WeakRef<_T>&& other)
{
	_weak._move_init(&other, (_T*)(0));
}

template <class T>
template <class _T>
SLIB_INLINE SafeWeakRef<T>::SafeWeakRef(const WeakRef<_T>& other) : _weak(other._weak)
{
}

template <class T>
SafeWeakRef<T>::SafeWeakRef(const Ref<T>& other)
{
	_set(other.ptr);
}

template <class T>
template <class _T>
SafeWeakRef<T>::SafeWeakRef(const Ref<_T>& other)
{
	_set(other.ptr);
}

template <class T>
SafeWeakRef<T>::SafeWeakRef(const SafeRef<T>& _other)
{
	Ref<T> other(_other.lock());
	_set(other.ptr);
}

template <class T>
template <class _T>
SafeWeakRef<T>::SafeWeakRef(const SafeRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	_set(other.ptr);
}

template <class T>
SLIB_INLINE const SafeWeakRef<T>& SafeWeakRef<T>::null()
{
	return *((SafeWeakRef<T>*)((void*)(&_Ref_Null)));
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::isNull() const
{
	return _weak.isNull();
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::isNotNull() const
{
	return _weak.isNotNull();
}

template <class T>
SLIB_INLINE void SafeWeakRef<T>::setNull()
{
	_weak.setNull();
}

template <class T>
Ref<T> SafeWeakRef<T>::lock() const
{
	Ref<CWeakRef> weak(_weak);
	if (weak.isNotNull()) {
		return Ref<T>::from(weak->lock());
	}
	return Ref<T>::null();
}

template <class T>
template <class _T>
SLIB_INLINE const SafeWeakRef<T>& SafeWeakRef<T>::from(const SafeWeakRef<_T>& other)
{
	return *((SafeWeakRef<T>*)((void*)&other));
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const T* _other)
{
	Ref<T> other(_other);
	_set(other.ptr);
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(SafeWeakRef<T>&& other)
{
	_weak._move_assign(&other);
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const SafeWeakRef<T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(SafeWeakRef<_T>&& other)
{
	_weak._move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const SafeWeakRef<_T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(WeakRef<T>&& other)
{
	_weak._move_assign(&other);
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const WeakRef<T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(WeakRef<_T>&& other)
{
	_weak._move_assign(&other, (_T*)(0));
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const WeakRef<_T>& other)
{
	_weak = other._weak;
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const Ref<T>& other)
{
	_set(other.ptr);
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const Ref<_T>& other)
{
	_set(other.ptr);
	return *this;
}

template <class T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const SafeRef<T>& _other)
{
	Ref<T> other(_other.lock());
	_set(other.ptr);
	return *this;
}

template <class T>
template <class _T>
SafeWeakRef<T>& SafeWeakRef<T>::operator=(const SafeRef<_T>& _other)
{
	Ref<_T> other(_other.lock());
	_set(other.ptr);
	return *this;
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator==(const SafeWeakRef<T>& other) const
{
	return _weak == other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator==(const SafeWeakRef<_T>& other) const
{
	return _weak == other._weak;
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator==(const WeakRef<T>& other) const
{
	return _weak == other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator==(const WeakRef<_T>& other) const
{
	return _weak == other._weak;
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator!=(const SafeWeakRef<T>& other) const
{
	return _weak != other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator!=(const SafeWeakRef<_T>& other) const
{
	return _weak != other._weak;
}

template <class T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator!=(const WeakRef<T>& other) const
{
	return _weak != other._weak;
}

template <class T>
template <class _T>
SLIB_INLINE sl_bool SafeWeakRef<T>::operator!=(const WeakRef<_T>& other) const
{
	return _weak != other._weak;
}

template <class T>
SLIB_INLINE void SafeWeakRef<T>::_set(const T* object)
{
	if (object) {
		_weak = ((Referable*)object)->getWeakObject();
	} else {
		_weak.setNull();
	}
}

template <class T>
SLIB_INLINE sl_bool operator>(const Ref<T>& a, const Ref<T>& b)
{
	return a.ptr > b.ptr;
}

template <class T>
SLIB_INLINE sl_bool operator<(const Ref<T>& a, const Ref<T>& b)
{
	return a.ptr < b.ptr;
}

SLIB_NAMESPACE_END

#endif
