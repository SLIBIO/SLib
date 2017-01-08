#ifndef CHECKHEADER_SLIB_CORE_ITERATOR
#define CHECKHEADER_SLIB_CORE_ITERATOR

#include "definition.h"

#include "ref.h"

SLIB_NAMESPACE_BEGIN

class IIteratorBase : public Referable
{
	SLIB_DECLARE_OBJECT
};

template <class T>
class SLIB_EXPORT IIterator : public IIteratorBase
{
public:
	virtual sl_bool hasNext() = 0;
	
	virtual sl_bool next(T* _out = sl_null) = 0;
	
	virtual sl_reg getIndex() = 0;
	
};

template <class T>
class SLIB_EXPORT IteratorPosition
{
public:
	IteratorPosition();
	
	IteratorPosition(const Ref< IIterator<T> >& iterator);
	
	IteratorPosition(const IteratorPosition<T>& other);
	
	IteratorPosition(IteratorPosition<T>&& other);
	
public:
	T& operator*();
	
	sl_bool operator!=(const IteratorPosition<T>& p);
	
	IteratorPosition<T>& operator++();
	
private:
	Ref< IIterator<T> > ref;
	T value;
	sl_bool flagValid;
	
};

template <class T>
class SLIB_EXPORT Iterator
{
public:
	Ref< IIterator<T> > ref;
	SLIB_REF_WRAPPER(Iterator, IIterator<T>)
	
public:
	sl_bool hasNext() const;
	
	sl_reg getIndex() const;
	
	sl_bool next(T* _out) const;
	
	// range-based for loop
	IteratorPosition<T> begin() const;
	
	IteratorPosition<T> end() const;
	
};

template <class T>
class SLIB_EXPORT Atomic< Iterator<T> >
{
public:
	AtomicRef< IIterator<T> > ref;
	SLIB_ATOMIC_REF_WRAPPER(IIterator<T>)

public:
	sl_bool hasNext() const;
	
	sl_reg getIndex() const;
	
	sl_bool next(T* _out) const;
	
	// range-based for loop
	IteratorPosition<T> begin() const;
	
	IteratorPosition<T> end() const;
	
};

template <class T>
using AtomicIterator = Atomic< Iterator<T> >;

SLIB_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T>
SLIB_INLINE IteratorPosition<T>::IteratorPosition()
{
}

template <class T>
SLIB_INLINE IteratorPosition<T>::IteratorPosition(const Ref< IIterator<T> >& iterator) : ref(iterator)
{
	if (iterator.isNotNull()) {
		flagValid = iterator->next(&value);
	} else {
		flagValid = sl_false;
	}
}

template <class T>
IteratorPosition<T>::IteratorPosition(const IteratorPosition<T>& other) = default;

template <class T>
IteratorPosition<T>::IteratorPosition(IteratorPosition<T>&& other) = default;

template <class T>
SLIB_INLINE T& IteratorPosition<T>::operator*()
{
	return value;
}

template <class T>
SLIB_INLINE sl_bool IteratorPosition<T>::operator!=(const IteratorPosition<T>& p)
{
	return flagValid;
}

template <class T>
SLIB_INLINE IteratorPosition<T>& IteratorPosition<T>::operator++()
{
	flagValid = ref->next(&value);
	return *this;
}


template <class T>
SLIB_INLINE sl_bool Iterator<T>::hasNext() const
{
	IIterator<T>* obj = ref._ptr;
	if (obj) {
		return obj->hasNext();
	}
	return sl_false;
}

template <class T>
SLIB_INLINE sl_reg Iterator<T>::getIndex() const
{
	IIterator<T>* obj = ref._ptr;
	if (obj) {
		return obj->getIndex();
	}
	return -1;
}

template <class T>
SLIB_INLINE sl_bool Iterator<T>::next(T* _out) const
{
	IIterator<T>* obj = ref._ptr;
	if (obj) {
		return obj->next(_out);
	}
	return sl_false;
};

template <class T>
SLIB_INLINE IteratorPosition<T> Iterator<T>::begin() const
{
	return IteratorPosition<T>(ref);
}

template <class T>
SLIB_INLINE IteratorPosition<T> Iterator<T>::end() const
{
	return IteratorPosition<T>();
}


template <class T>
sl_bool Atomic< Iterator<T> >::hasNext() const
{
	Ref< IIterator<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->hasNext();
	}
	return sl_false;
}

template <class T>
sl_reg Atomic< Iterator<T> >::getIndex() const
{
	Ref< IIterator<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->getIndex();
	}
	return -1;
}

template <class T>
sl_bool Atomic< Iterator<T> >::next(T* _out) const
{
	Ref< IIterator<T> > obj(ref);
	if (obj.isNotNull()) {
		return obj->next(_out);
	}
	return sl_false;
};

template <class T>
IteratorPosition<T> Atomic< Iterator<T> >::begin() const
{
	return IteratorPosition<T>(ref);
}

template <class T>
IteratorPosition<T> Atomic< Iterator<T> >::end() const
{
	return IteratorPosition<T>();
}

SLIB_NAMESPACE_END

#endif
