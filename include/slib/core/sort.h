/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SORT
#define CHECKHEADER_SLIB_CORE_SORT

#include "definition.h"

#include "cpp.h"
#include "compare.h"

namespace slib
{
	
	class SLIB_EXPORT SelectionSort
	{
	public:
		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortAsc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortDesc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sort(TYPE* list, sl_size size, sl_bool flagAscending, const COMPARE& compare = COMPARE());

	};
	
	class SLIB_EXPORT InsertionSort
	{
	public:
		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortAsc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortDesc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sort(TYPE* list, sl_size size, sl_bool flagAsc, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortAsc(const TYPE* src, TYPE* dst, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortDesc(const TYPE* src, TYPE* dst, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sort(const TYPE* src, TYPE* dst, sl_size size, sl_bool flagAscending, const COMPARE& compare = COMPARE());

	};
	
	class SLIB_EXPORT QuickSort
	{
	public:
		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortAsc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sortDesc(TYPE* list, sl_size size, const COMPARE& compare = COMPARE());

		template < class TYPE, class COMPARE = Compare<TYPE> >
		static void sort(TYPE* list, sl_size size, sl_bool flagAscending, const COMPARE& compare = COMPARE());

	};

}

#include "detail/sort.inc"

#endif
