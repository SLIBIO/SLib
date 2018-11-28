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
	CArray<T>::CArray(const std::initializer_list<T>& l) noexcept : CArray(l.begin(), l.size())
	{
	}

	template <class T>
	CArray<T>* CArray<T>::create(const std::initializer_list<T>& l) noexcept
	{
		return create(l.begin(), l.size());
	}
	
	
	template <class T>
	Array<T>::Array(const std::initializer_list<T>& l) noexcept : ref(CArray<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Array<T> Array<T>::create(const std::initializer_list<T>& l) noexcept
	{
		return create(l.begin(), l.size());
	}
	
	template <class T>
	Array<T>& Array<T>::operator=(const std::initializer_list<T>& l) noexcept
	{
		ref = CArray<T>::create(l.begin(), l.size());
		return *this;
	}
	
	
	template <class T>
	Atomic< Array<T> >::Atomic(const std::initializer_list<T>& l) noexcept : ref(CArray<T>::create(l.begin(), l.size()))
	{
	}
	
	template <class T>
	Atomic< Array<T> >& Atomic< Array<T> >::operator=(const std::initializer_list<T>& l) noexcept
	{
		ref = CArray<T>::create(l.begin(), l.size());
		return *this;
	}
		
}
