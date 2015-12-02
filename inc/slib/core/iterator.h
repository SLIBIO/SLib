#ifndef CHECKHEADER_SLIB_CORE_ITERATOR
#define CHECKHEADER_SLIB_CORE_ITERATOR

#include "definition.h"
#include "object.h"

/************************************************************************/
/* IIterator Interface Declaration                                      */
/************************************************************************/

SLIB_NAMESPACE_BEGIN
template <class TYPE>
class SLIB_EXPORT IIterator : public Object
{
	SLIB_DECLARE_OBJECT(IIterator, Object)
public:
	IIterator() {}
	virtual sl_bool hasNext() = 0;
	virtual sl_bool next(TYPE* out = sl_null) = 0;
	virtual sl_reg getIndex() = 0;
};
SLIB_NAMESPACE_END

/************************************************************************
* Iterator class Definition
************************************************************************/
SLIB_NAMESPACE_BEGIN

/** auto-referencing object **/
template <class TYPE >
class SLIB_EXPORT Iterator
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(Iterator<TYPE>, IIterator<TYPE>)
	SLIB_DECLARE_OBJECT_WRAPPER(Iterator, Iterator<TYPE>, IIterator<TYPE>, Ref< IIterator<TYPE> >)

public:

	SLIB_INLINE sl_bool hasNext() const
    {
		Ref< IIterator<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->hasNext();
		} else {
			return sl_false;
		}
	}
	SLIB_INLINE sl_reg getIndex() const
    {
		Ref< IIterator<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->getIndex();
		} else {
			return -1;
		}
	}
	SLIB_INLINE sl_bool next(TYPE* out) const
    {
		Ref< IIterator<TYPE> > object = m_object;
		if (object.isNotNull()) {
			return object->next(out);
		} else {
			return sl_false;
		}
	};
};
SLIB_NAMESPACE_END

#endif
