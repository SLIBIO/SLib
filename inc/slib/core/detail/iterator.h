#ifndef CHECKHEADER_SLIB_CORE_DETAIL_ITERATOR
#define CHECKHEADER_SLIB_CORE_DETAIL_ITERATOR

#include "../iterator.h"

namespace slib
{

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

}

#endif
