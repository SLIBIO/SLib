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
	SLIB_INLINE ScopedPtr<T>::ScopedPtr() : ptr(sl_null)
	{
	}

	template <class T>
	SLIB_INLINE ScopedPtr<T>::ScopedPtr(T* _ptr) : ptr(_ptr)
	{
	}
	
	template <class T>
	SLIB_INLINE ScopedPtr<T>::~ScopedPtr()
	{
		release();
	}
	
	template <class T>
	SLIB_INLINE void ScopedPtr<T>::release()
	{
		delete ptr;
		ptr = sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool ScopedPtr<T>::isNull()
	{
		return ptr == sl_null;
	}
	
	template <class T>
	SLIB_INLINE sl_bool ScopedPtr<T>::isNotNull()
	{
		return ptr != sl_null;
	}
	
	template <class T>
	SLIB_INLINE T& ScopedPtr<T>::operator*()
	{
		return *(ptr);
	}
	
	template <class T>
	SLIB_INLINE T* ScopedPtr<T>::operator->()
	{
		return ptr;
	}
	
	
	template <class T>
	SLIB_INLINE ScopedPtrNew<T>::ScopedPtrNew() : ScopedPtr<T>(new T)
	{
	}
	
	template <class T>
	SLIB_INLINE T& ScopedPtrNew<T>::operator*()
	{
		return *(this->ptr);
	}
	
	template <class T>
	SLIB_INLINE T* ScopedPtrNew<T>::operator->()
	{
		return this->ptr;
	}


	template <class T>
	SLIB_INLINE ScopedArray<T>::ScopedArray() : data(sl_null), count(0)
	{
	}

	template <class T>
	SLIB_INLINE ScopedArray<T>::ScopedArray(T* _data, sl_size _count) : data(_data), count(_count)
	{
	}

	template <class T>
	SLIB_INLINE ScopedArray<T>::ScopedArray(sl_size _count)
	{
		data = NewHelper<T>::create(_count);
		if (data) {
			count = _count;
		} else {
			count = 0;
		}
	}

	template <class T>
	SLIB_INLINE ScopedArray<T>::~ScopedArray()
	{
		release();
	}

	template <class T>
	SLIB_INLINE void ScopedArray<T>::release()
	{
		if (data) {
			NewHelper<T>::free(data, count);
			data = sl_null;
		}
		count = 0;
	}

	template <class T>
	SLIB_INLINE sl_bool ScopedArray<T>::isNull()
	{
		return data == sl_null;
	}

	template <class T>
	SLIB_INLINE sl_bool ScopedArray<T>::isNotNull()
	{
		return data != sl_null;
	}

	template <class T>
	SLIB_INLINE T& ScopedArray<T>::operator[](sl_size index)
	{
		return data[index];
	}

	template <class T>
	SLIB_INLINE T* ScopedArray<T>::operator+(sl_size offset)
	{
		return data + offset;
	}


	template <class T, sl_size countStack>
	SLIB_INLINE ScopedBuffer<T, countStack>::ScopedBuffer(sl_size _count)
	{
		if (_count < countStack) {
			data = stack;
		} else {
			data = NewHelper<T>::create(_count);
		}
		count = _count;
	}

	template <class T, sl_size countStack>
	SLIB_INLINE ScopedBuffer<T, countStack>::~ScopedBuffer()
	{
		release();
	}

	template <class T, sl_size countStack>
	SLIB_INLINE void ScopedBuffer<T, countStack>::release()
	{
		if (data) {
			if (data != stack) {
				NewHelper<T>::free(data, count);
			}
			data = sl_null;
		}
		count = 0;
	}

	template <class T, sl_size countStack>
	SLIB_INLINE sl_bool ScopedBuffer<T, countStack>::isNull()
	{
		return data == sl_null;
	}

	template <class T, sl_size countStack>
	SLIB_INLINE sl_bool ScopedBuffer<T, countStack>::isNotNull()
	{
		return data != sl_null;
	}

	template <class T, sl_size countStack>
	SLIB_INLINE T& ScopedBuffer<T, countStack>::operator[](sl_size index)
	{
		return data[index];
	}

	template <class T, sl_size countStack>
	SLIB_INLINE T* ScopedBuffer<T, countStack>::operator+(sl_size offset)
	{
		return data + offset;
	}

}
