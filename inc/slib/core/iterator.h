#ifndef CHECKHEADER_SLIB_CORE_ITERATOR
#define CHECKHEADER_SLIB_CORE_ITERATOR

#include "definition.h"

#include "reference.h"

SLIB_NAMESPACE_BEGIN

template <class TYPE>
class SLIB_EXPORT IIterator : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(IIterator)
public:
	SLIB_INLINE IIterator() {}

public:
	virtual sl_bool hasNext() = 0;
	
	virtual sl_bool next(TYPE* _out = sl_null) = 0;
	
	virtual sl_reg getIndex() = 0;
};

template <class TYPE>
class SafeIterator;

/** auto-referencing object **/
template <class TYPE >
class SLIB_EXPORT Iterator
{
	typedef IIterator<TYPE> _Obj;
	typedef Iterator<TYPE> _Type;
	typedef SafeIterator<TYPE> _SafeType;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(Iterator, _Obj, _Type, _SafeType)

public:
	SLIB_INLINE sl_bool hasNext() const
    {
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->hasNext();
		}
		return sl_false;
	}

	SLIB_INLINE sl_reg getIndex() const
    {
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->getIndex();
		}
		return -1;
	}

	SLIB_INLINE sl_bool next(TYPE* _out) const
    {
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->next(_out);
		}
		return sl_false;
	};
};


/** auto-referencing object **/
template <class TYPE >
class SLIB_EXPORT SafeIterator
{
	typedef IIterator<TYPE> _Obj;
	typedef SafeIterator<TYPE> _Type;
	typedef Iterator<TYPE> _LocalType;
	SLIB_DECLARE_OBJECT_TYPE_FROM(_Type, _Obj)
	SLIB_DECLARE_OBJECT_SAFE_WRAPPER(SafeIterator, _Obj, _Type, _LocalType)
	
public:
	SLIB_INLINE sl_bool hasNext() const
	{
		_LocalType obj(m_object);
		if (obj.isNotNull()) {
			return obj->hasNext();
		}
		return sl_false;
	}
	
	SLIB_INLINE sl_reg getIndex() const
	{
		_LocalType obj(m_object);
		if (obj.isNotNull()) {
			return obj->getIndex();
		}
		return -1;
	}
	
	SLIB_INLINE sl_bool next(TYPE* _out) const
	{
		_LocalType obj(m_object);
		if (obj.isNotNull()) {
			return obj->next(_out);
		}
		return sl_false;
	};
};

SLIB_NAMESPACE_END

#endif
