/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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
