/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	template <class T>
	CList<T>::CList(const std::initializer_list<T>& l) noexcept
	 : CList(l.begin(), l.size())
	{
	}

	template <class T>
	CList<T>* CList<T>::create(const std::initializer_list<T>& l) noexcept
	{
		return create(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) noexcept
	{
		return insertElements_NoLock(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::insertElements(sl_size index, const std::initializer_list<T>& l) noexcept
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::addElements_NoLock(const std::initializer_list<T>& l) noexcept
	{
		return addElements_NoLock(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool CList<T>::addElements(const std::initializer_list<T>& l) noexcept
	{
		return addElements(l.begin(), l.size());
	}

	
	template <class T>
	List<T>::List(const std::initializer_list<T>& l) noexcept
	 : ref(CList<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	List<T> List<T>::create(const std::initializer_list<T>& l) noexcept
	{
		return create(l.begin(), l.size());
	}
	
	template <class T>
	List<T>& List<T>::operator=(const std::initializer_list<T>& l) noexcept
	{
		ref = CList<T>::create(l.begin(), l.size());
		return *this;
	}
	
	template <class T>
	sl_bool List<T>::insertElements_NoLock(sl_size index, const std::initializer_list<T>& l) noexcept
	{
		return insertElements_NoLock(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::insertElements(sl_size index, const std::initializer_list<T>& l) noexcept
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::addElements_NoLock(const std::initializer_list<T>& l) noexcept
	{
		return addElements_NoLock(l.begin(), l.size());
	}
	
	template <class T>
	sl_bool List<T>::addElements(const std::initializer_list<T>& l) noexcept
	{
		return addElements(l.begin(), l.size());
	}

	
	template <class T>
	Atomic< List<T> >::Atomic(const std::initializer_list<T>& l) noexcept
	 : ref(CList<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Atomic< List<T> >& Atomic< List<T> >::operator=(const std::initializer_list<T>& l) noexcept
	{
		ref = CList<T>::create(l.begin(), l.size());
		return *this;
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::insertElements(sl_size index, const std::initializer_list<T>& l) noexcept
	{
		return insertElements(index, l.begin(), l.size());
	}
	
	template <class T>
	sl_bool Atomic< List<T> >::addElements(const std::initializer_list<T>& l) noexcept
	{
		return addElements(l.begin(), l.size());
	}
	
}
