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

#include "slib/core/list.h"

namespace slib
{

	const char _priv_List_ClassID[] = "list";	
	
	#define PRIV_SLIB_LIST_CAPACITY_MIN 5
	
	sl_bool _priv_CList::setCapacity(void* _pData, sl_size elementSize, sl_size* pCapacity, sl_size *pCount, sl_size capacity) noexcept
	{
		void** pData = reinterpret_cast<void**>(_pData);
		if (capacity < *pCount) {
			capacity = *pCount;
		}
		if (capacity == *pCapacity) {
			return sl_true;
		}
		void* data = *pData;
		if (data) {
			data = Base::reallocMemory(data, capacity * elementSize);
		} else {
			data = Base::createMemory(capacity * elementSize);
		}
		if (data) {
			*pData = data;
			*pCapacity = capacity;
			return sl_true;
		}
		return sl_false;
	}

	sl_bool _priv_CList::adjustCapacity(void* _pData, sl_size elementSize, sl_size* pCapacity, sl_size* pCount, sl_size count) noexcept
	{
		void** pData = reinterpret_cast<void**>(_pData);
		if (count < *pCount) {
			count = *pCount;
		}
		if (*pCapacity < count) {
			sl_size newCapacity = *pCapacity * 3 / 2 + 1;
			if (newCapacity < count) {
				newCapacity = count;
			}
			if (newCapacity < PRIV_SLIB_LIST_CAPACITY_MIN) {
				newCapacity = PRIV_SLIB_LIST_CAPACITY_MIN;
			}
			void* newData;
			if (*pData) {
				newData = Base::reallocMemory(*pData, newCapacity * elementSize);
			} else {
				newData = Base::createMemory(newCapacity * elementSize);
			}
			if (newData) {
				*pData = newData;
				*pCapacity = newCapacity;
			} else {
				return sl_false;
			}
		} else if (*pCapacity > PRIV_SLIB_LIST_CAPACITY_MIN && count < *pCapacity / 2) {
			sl_size newCapacity = count * 3 / 2 + 1;
			if (newCapacity < PRIV_SLIB_LIST_CAPACITY_MIN) {
				newCapacity = PRIV_SLIB_LIST_CAPACITY_MIN;
			}
			if (newCapacity < *pCapacity) {
				void* newData = Base::reallocMemory(*pData, newCapacity * elementSize);
				if (newData) {
					*pData = newData;
					*pCapacity = newCapacity;
				}
			}
		}
		
		return sl_true;
	}
	
}
