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
	Atomic<T>::Atomic()
	{
	}
	
	template <class T>
	Atomic<T>::Atomic(const T& value)
	: m_value(value)
	{
	}
	
	template <class T>
	Atomic<T>::Atomic(T&& value)
	: m_value(Move(value))
	{
	}
	
	template <class T>
	Atomic<T>& Atomic<T>::operator=(const T& other)
	{
		SLIB_ALIGN(8) char t[sizeof(T)];
		{
			SpinLocker lock(&m_lock);
			((T*)t)->T::T(Move(m_value));
			m_value = other;
		}
		((T*)t)->~T();
		return *this;
	}
	
	template <class T>
	Atomic<T>& Atomic<T>::operator=(T&& other)
	{
		SLIB_ALIGN(8) char t[sizeof(T)];
		{
			SpinLocker lock(&m_lock);
			((T*)t)->T::T(Move(m_value));
			m_value = Move(other);
		}
		((T*)t)->~T();
		return *this;
	}
	
	template <class T>
	Atomic<T>::operator T() const
	{
		SLIB_ALIGN(8) char t[sizeof(T)];
		{
			SpinLocker lock(&m_lock);
			((T*)t)->T::T(m_value);
		}
		return *((T*)t);
	}

}
