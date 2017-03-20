/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_DETAIL_SORT
#define CHECKHEADER_SLIB_CORE_DETAIL_SORT

#include "../sort.h"

namespace slib
{
	
	template <class TYPE, class COMPARE>
	void SelectionSort::sortAsc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		sl_size size_1 = size - 1;
		for (sl_size i = 0; i < size_1; i++) {
			sl_size sel = i;
			for (sl_size j = i + 1; j < size; j++) {
				if (compare(list[sel], list[j]) > 0) {
					sel = j;
				}
			}
			if (sel != i) {
				swap(list[sel], list[i]);
			}
		}
	}
	
	template <class TYPE, class COMPARE>
	void SelectionSort::sortDesc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		sl_size size_1 = size - 1;
		for (sl_size i = 0; i < size_1; i++) {
			sl_size sel = i;
			for (sl_size j = i + 1; j < size; j++) {
				if (compare(list[sel], list[j]) < 0) {
					sel = j;
				}
			}
			if (sel != i) {
				swap(list[sel], list[i]);
			}
		}
	}
	
	template <class TYPE, class COMPARE>
	void SelectionSort::sort(TYPE* list, sl_size size, sl_bool flagAsc, const COMPARE& compare)
	{
		if (flagAsc) {
			sortAsc(list, size, compare);
		} else {
			sortDesc(list, size, compare);
		}
	}
	
	
	template <class TYPE, class COMPARE>
	void InsertionSort::sortAsc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		for (sl_size i = 1; i < size; i++) {
			TYPE x = list[i];
			sl_size j = i;
			while (j > 0) {
				if (compare(list[j - 1], x) <= 0) {
					break;
				}
				list[j] = list[j - 1];
				j--;
			}
			if (j != i) {
				list[j] = x;
			}
		}
	}
	
	template <class TYPE, class COMPARE>
	void InsertionSort::sortDesc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		for (sl_size i = 1; i < size; i++) {
			TYPE x = list[i];
			sl_size j = i;
			while (j > 0) {
				if (compare(list[j - 1], x) >= 0) {
					break;
				}
				list[j] = list[j - 1];
				j--;
			}
			if (j != i) {
				list[j] = x;
			}
		}
	}

	template <class TYPE, class COMPARE>
	void InsertionSort::sort(TYPE* list, sl_size size, sl_bool flagAsc, const COMPARE& compare)
	{
		if (flagAsc) {
			sortAsc(list, size, compare);
		} else {
			sortDesc(list, size, compare);
		}
	}

	template <class TYPE, class COMPARE>
	void InsertionSort::sortAsc(const TYPE* src, TYPE* dst, sl_size size, const COMPARE& compare)
	{
		if (src == dst) {
			sort(dst, size);
		}
		if (size < 2) {
			return;
		}
		sl_size size_1 = size - 1;
		dst[0] = src[0];
		for (sl_size i = 1; i < size_1; i++) {
			sl_size j = i;
			while (j > 0) {
				if (compare(dst[j - 1], src[i]) <= 0) {
					break;
				}
				dst[j] = dst[j - 1];
				j--;
			}
			dst[j] = src[i];
		}
	}

	template <class TYPE, class COMPARE>
	void InsertionSort::sortDesc(const TYPE* src, TYPE* dst, sl_size size, const COMPARE& compare)
	{
		if (src == dst) {
			sort(dst, size);
		}
		if (size < 2) {
			return;
		}
		sl_size size_1 = size - 1;
		dst[0] = src[0];
		for (sl_size i = 1; i < size_1; i++) {
			sl_size j = i;
			while (j > 0) {
				if (compare(dst[j - 1], src[i]) >= 0) {
					break;
				}
				dst[j] = dst[j - 1];
				j--;
			}
			dst[j] = src[i];
		}
	}

	template <class TYPE, class COMPARE>
	void InsertionSort::sort(const TYPE* src, TYPE* dst, sl_size size, sl_bool flagAsc, const COMPARE& compare)
	{
		if (flagAsc) {
			sortAsc(src, dst, size, compare);
		} else {
			sortDesc(src, dst, size, compare);
		}
	}


	template <class TYPE, class COMPARE>
	void QuickSort::sortAsc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		sl_size nStack = 0;
		sl_size stack_start[64], stack_end[64];
		sl_size start = 0;
		sl_size end = size - 1;
		for(;;) {
			sl_size n = end - start + 1;
			if (n < 8) {
				InsertionSort::sortAsc(list + start, n);
			} else {
				sl_size mid = start + (n / 2);
				Swap(list[mid], list[start]);
				sl_size border2 = start;
				sl_size border1 = end + 1;
				for (;;) {
					for (;;) {
						border2++;
						if (border2 > end) {
							break;
						}
						if (compare(list[border2], list[start]) > 0) {
							break;
						}
					};
					for (;;) {
						border1--;
						if (border1 <= start) {
							break;
						}
						if (compare(list[border1], list[start]) < 0) {
							break;
						}
					}
					if (border1 < border2) {
						break;
					}
					Swap(list[border1], list[border2]);
				}
				if (border1 != start) {
					Swap(list[border1], list[start]);
				}
				if (border1 - start < end + 1 - border2) {
					if (border2 < end) {
						stack_start[nStack] = border2;
						stack_end[nStack] = end;
						nStack++;
					}
					if (border1 > start + 1) {
						stack_start[nStack] = start;
						stack_end[nStack] = border1 - 1;
						nStack++;
					}
				} else {
					if (border1 > start + 1) {
						stack_start[nStack] = start;
						stack_end[nStack] = border1 - 1;
						nStack++;
					}
					if (border2 < end) {
						stack_start[nStack] = border2;
						stack_end[nStack] = end;
						nStack++;
					}
				}
			}
			if (nStack == 0) {
				return;
			}
			nStack--;
			start = stack_start[nStack];
			end = stack_end[nStack];
		}
	}

	template <class TYPE, class COMPARE>
	void QuickSort::sortDesc(TYPE* list, sl_size size, const COMPARE& compare)
	{
		if (size < 2) {
			return;
		}
		sl_size nStack = 0;
		sl_size stack_start[64], stack_end[64];
		sl_size start = 0;
		sl_size end = size - 1;
		for(;;) {
			sl_size n = end - start + 1;
			if (n < 8) {
				InsertionSort::sortDesc(list + start, n);
			} else {
				sl_size mid = start + (n / 2);
				Swap(list[mid], list[start]);
				sl_size border2 = start;
				sl_size border1 = end + 1;
				for (;;) {
					for (;;) {
						border2++;
						if (border2 > end) {
							break;
						}
						if (compare(list[border2], list[start]) < 0) {
							break;
						}
					};
					for (;;) {
						border1--;
						if (border1 <= start) {
							break;
						}
						if (compare(list[border1], list[start]) > 0) {
							break;
						}
					}
					if (border1 < border2) {
						break;
					}
					Swap(list[border1], list[border2]);
				}
				if (border1 != start) {
					Swap(list[border1], list[start]);
				}
				if (border1 - start < end + 1 - border2) {
					if (border2 < end) {
						stack_start[nStack] = border2;
						stack_end[nStack] = end;
						nStack++;
					}
					if (border1 > start + 1) {
						stack_start[nStack] = start;
						stack_end[nStack] = border1 - 1;
						nStack++;
					}
				} else {
					if (border1 > start + 1) {
						stack_start[nStack] = start;
						stack_end[nStack] = border1 - 1;
						nStack++;
					}
					if (border2 < end) {
						stack_start[nStack] = border2;
						stack_end[nStack] = end;
						nStack++;
					}
				}
			}
			if (nStack == 0) {
				return;
			}
			nStack--;
			start = stack_start[nStack];
			end = stack_end[nStack];
		}
	}

	template <class TYPE, class COMPARE>
	void QuickSort::sort(TYPE* list, sl_size size, sl_bool flagAsc, const COMPARE& compare)
	{
		if (flagAsc) {
			sortAsc(list, size, compare);
		} else {
			sortDesc(list, size, compare);
		}
	}

}

#endif
