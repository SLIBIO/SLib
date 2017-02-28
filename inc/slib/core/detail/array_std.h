#ifndef CHECKHEADER_SLIB_CORE_DETAIL_ARRAY_STD
#define CHECKHEADER_SLIB_CORE_DETAIL_ARRAY_STD

#include "../array.h"

#include <initializer_list>

namespace slib
{
	
	template <class T>
	CArray<T>::CArray(const std::initializer_list<T>& l) : CArray(l.begin(), l.size())
	{
	}

	template <class T>
	CArray<T>* CArray<T>::create(const std::initializer_list<T>& l)
	{
		return createFromArray(l.begin(), l.size());
	}
	
	
	template <class T>
	Array<T>::Array(const std::initializer_list<T>& l) : ref(CArray<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Array<T> Array<T>::create(const std::initializer_list<T>& l)
	{
		return create(l.begin(), l.size());
	}
	
	template <class T>
	Array<T>& Array<T>::operator=(const std::initializer_list<T>& l)
	{
		ref = CArray<T>::create(l.begin(), l.size());
		return *this;
	}
	
	
	template <class T>
	Atomic< Array<T> >::Atomic(const std::initializer_list<T>& l) : ref(CArray<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Atomic< Array<T> >& Atomic< Array<T> >::operator=(const std::initializer_list<T>& l)
	{
		ref = CArray<T>::create(l.begin(), l.size());
		return *this;
	}
		
}

#endif
