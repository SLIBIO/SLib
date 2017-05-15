/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/list.h"

namespace slib
{

	const char _priv_List_ClassID[] = "list";	
	
	#define PRIV_SLIB_LIST_CAPACITY_MIN 5
	
	sl_bool _priv_CList::setCapacity(void** pData, sl_size elementSize, sl_size* pCapacity, sl_size *pCount, sl_size capacity) noexcept
	{
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

	sl_bool _priv_CList::adjustCapacity(void** pData, sl_size elementSize, sl_size* pCapacity, sl_size* pCount, sl_size count) noexcept
	{
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
