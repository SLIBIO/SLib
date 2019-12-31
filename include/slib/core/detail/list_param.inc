/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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
	SLIB_INLINE ListParam<T>::ListParam() noexcept: _value(sl_null), _count(0)
	{		
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(sl_null_t) noexcept: _value(sl_null), _count(0)
	{
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(ListParam<T>&& other) noexcept
	{
		_value = other._value;
		_count = other._count;
		other._value = sl_null;
		other._count = 0;
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const ListParam<T>& other) noexcept
	{
		_count = other._count;
		_value = other._value;
		if (_count == (sl_reg)(ListType::List_Ref)) {
			_count = (sl_reg)(ListType::List_NoRef);
		}
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(List<T>&& list) noexcept
	{
		if (list.isNotNull()) {
			_count = (sl_reg)(ListType::List_Ref);
			new (reinterpret_cast<List<T>*>(&_value)) List<T>(Move(list));
		} else {
			_value = sl_null;
			_count = 0;
		}
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const List<T>& list) noexcept
	{
		_value = list.ref._ptr;
		if (_value) {
			_count = (sl_reg)(ListType::List_NoRef);
		} else {
			_count = 0;
		}
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(AtomicList<T>&& list) noexcept: ListParam(List<T>(Move(list)))
	{
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const AtomicList<T>& list) noexcept: ListParam(List<T>(list))
	{
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const CList<T>& list) noexcept
	{
		_count = (sl_reg)(ListType::CList);
		_value = &list;
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const T* data, sl_size count) noexcept
	{
		if (data && count) {
			_count = count & SLIB_SIZE_MASK_NO_SIGN_BITS;
			_value = (void*)data;
		} else {
			_count = 0;
			_value = sl_null;
		}
	}

	template <class T>
	template <sl_size N>
	SLIB_INLINE ListParam<T>::ListParam(T(&data)[N]) noexcept
	{
		_count = N & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = data;
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const ListLocker<T>& list) noexcept
	{
		_count = list.count & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = list.data;
	}

	template <class T>
	SLIB_INLINE ListParam<T>::ListParam(const ListElements<T>& list) noexcept
	{
		_count = list.count & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = list.data;
	}

	template <class T>
	SLIB_INLINE ListParam<T>::~ListParam()
	{
		_free();
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(sl_null_t) noexcept
	{
		_free();
		_value = sl_null;
		_count = 0;
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(ListParam<T>&& other) noexcept
	{
		_value = other._value;
		_count = other._count;
		other._value = sl_null;
		other._count = 0;
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const ListParam<T>& other) noexcept
	{
		_free();
		_count = other._count;
		_value = other._value;
		if (_count == (sl_reg)(ListType::List_Ref)) {
			_count = (sl_reg)(ListType::List_NoRef);
		}
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(List<T>&& list) noexcept
	{
		_free();
		if (list.isNotNull()) {
			_count = (sl_reg)(ListType::List_Ref);
			new (reinterpret_cast<List<T>*>(&_value)) List<T>(Move(list));
		} else {
			_value = sl_null;
			_count = 0;
		}
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const List<T>& list) noexcept
	{
		_free();
		_value = list.ref._ptr;
		if (_value) {
			_count = (sl_reg)(ListType::List_NoRef);
		} else {
			_count = 0;
		}
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(AtomicList<T>&& list) noexcept
	{
		return *this = List<T>(Move(list));
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const AtomicList<T>& list) noexcept
	{
		return *this = List<T>(list);
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const CList<T>& list) noexcept
	{
		_free();
		_count = (sl_reg)(ListType::CList);
		_value = &list;
		return *this;
	}

	template <class T>
	template <sl_size N>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(T(&data)[N]) noexcept
	{
		_free();
		_count = N & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = (void*)data;
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const ListLocker<T>& list) noexcept
	{
		_free();
		_count = list.count & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = (void*)(list.data);
		return *this;
	}

	template <class T>
	SLIB_INLINE ListParam<T>& ListParam<T>::operator=(const ListElements<T>& list) noexcept
	{
		_free();
		_count = list.count & SLIB_SIZE_MASK_NO_SIGN_BITS;
		_value = (void*)(list.data);
		return *this;
	}

	template <class T>
	SLIB_INLINE T& ListParam<T>::operator[](sl_reg index) const noexcept
	{
		return (getData())[index];
	}

	template <class T>
	SLIB_INLINE void ListParam<T>::setNull() noexcept
	{
		_free();
		_value = sl_null;
		_count = 0;
	}

	template <class T>
	SLIB_INLINE sl_bool ListParam<T>::isNull() const noexcept
	{
		return !_value;
	}

	template <class T>
	SLIB_INLINE sl_bool ListParam<T>::isNotNull() const noexcept
	{
		return _value != sl_null;
	}

	template <class T>
	SLIB_INLINE sl_size ListParam<T>::getCount() const noexcept
	{
		if (_count < 0) {
			return ((CList<T>*)_value)->getCount();
		} else {
			return _count;
		}
	}

	template <class T>
	SLIB_INLINE T* ListParam<T>::getData() const noexcept
	{
		if (_count < 0) {
			return ((CList<T>*)_value)->getData();
		} else {
			return (T*)_value;
		}
	}

	template <class T>
	SLIB_INLINE CList<T>* ListParam<T>::getObject() const noexcept
	{
		if (_count < 0) {
			return (CList<T>*)_value;
		} else {
			return sl_null;
		}
	}

	template <class T>
	List<T> ListParam<T>::toList() const noexcept
	{
		if (_count == (sl_reg)(ListType::List_Ref) || _count == (sl_reg)(ListType::List_NoRef)) {
			return (CList<T>*)_value;
		} else if (_count == (sl_reg)(ListType::CList)) {
			return ((CList<T>*)_value)->duplicate();
		} else {
			return List<T>::create((T*)_value, _count);
		}
	}

	template <class T>
	template <class... ARGS>
	sl_bool ListParam<T>::add(ARGS&&... args) noexcept
	{
		if (_count < 0) {
			return ((CList<T>*)_value)->add_NoLock(Forward<ARGS>(args)...);
		} else {
			if (_count) {
				return sl_false;
			}
			_count = (sl_reg)(ListType::List_Ref);
			return (reinterpret_cast<List<T>*>(&_value))->add_NoLock(Forward<ARGS>(args)...);
		}
	}

	template <class T>
	SLIB_INLINE const List<T>& ListParam<T>::_getList() const noexcept
	{
		if (_count == (sl_reg)(ListType::List_Ref)) {
			return *(reinterpret_cast<const List<T>*>(&_value));
		} else {
			return List<T>::null();
		}
	}

	template <class T>
	SLIB_INLINE void ListParam<T>::_free() noexcept
	{
		if (_count == (sl_reg)(ListType::List_Ref)) {
			(*(reinterpret_cast<List<T>*>(&_value))).List<T>::~List();
		}
	}

	template <class T>
	SLIB_INLINE T* ListParam<T>::begin() const noexcept
	{
		return getData();
	}

	template <class T>
	SLIB_INLINE T* ListParam<T>::end() const noexcept
	{
		return getData() + getCount();
	}

}
