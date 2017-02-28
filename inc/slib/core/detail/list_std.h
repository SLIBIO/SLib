#ifndef CHECKHEADER_SLIB_CORE_DETAIL_LIST_STD
#define CHECKHEADER_SLIB_CORE_DETAIL_LIST_STD

#include "../list.h"

#include <initializer_list>

namespace slib
{
	
	template <class T>
	CList<T>::CList(const std::initializer_list<T>& l) : CList(l.begin(), l.size())
	{
	}

	template <class T>
	CList<T>* CList<T>::create(const std::initializer_list<T>& l)
	{
		return createFromArray(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, const std::initializer_list<T>& l)
	{
		return insertElements_NoLock(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::insertElements(sl_size index, const std::initializer_list<T>& l)
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::addElements_NoLock(const std::initializer_list<T>& l)
	{
		return addElements_NoLock(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::addElements(const std::initializer_list<T>& l)
	{
		return addElements(l.begin(), l.size());
	}

	
	template <class T>
	List<T>::List(const std::initializer_list<T>& l) : ref(CList<T>::createFromArray(l.begin(), l.size()))
	{
	}
	
	template <class T>
	List<T> List<T>::create(const std::initializer_list<T>& l)
	{
		return createFromArray(l.begin(), l.size());
	}
	
	template <class T>
	List<T>& List<T>::operator=(const std::initializer_list<T>& l)
	{
		ref = CList<T>::createFromArray(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) const
	{
		return insertElements_NoLock(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::insertElements(sl_size index, const std::initializer_list<T>& l) const
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::addElements_NoLock(const std::initializer_list<T>& l)
	{
		return addElements_NoLock(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::addElements(const std::initializer_list<T>& l)
	{
		return addElements(l.begin(), l.size());
	}

	
	template <class T>
	Atomic< List<T> >::Atomic(const std::initializer_list<T>& l) : ref(CList<T>::createFromArray(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Atomic< List<T> >& Atomic< List<T> >::operator=(const std::initializer_list<T>& l)
	{
		ref = CList<T>::createFromArray(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::insertElements(sl_size index, const std::initializer_list<T>& l) const
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::addElements(const std::initializer_list<T>& l)
	{
		return addElements(l.begin(), l.size());
	}
	
	
}

#endif
