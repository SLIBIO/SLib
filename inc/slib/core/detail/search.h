#ifndef CHECKHEADER_SLIB_CORE_DETAIL_SEARCH
#define CHECKHEADER_SLIB_CORE_DETAIL_SEARCH

#include "../search.h"

namespace slib
{

	template <class T1, class T2, class COMPARE>
	sl_bool BinarySearch::search(const T1* list, sl_size size, const T2& what, sl_size* pIndexPrev, const COMPARE& compare)
	{
		if (size == 0) {
			if (pIndexPrev) {
				*pIndexPrev = 0;
			}
			return sl_false;
		}
		sl_size index = 0;
		sl_size start = 0;
		sl_size end = size - 1;
		while (1) {
			if (start == end) {
				int c = compare(list[start], what);
				if (c == 0) {
					if (pIndexPrev) {
						*pIndexPrev = start;
					}
					return sl_true;
				} else if (c < 0) {
					index = start + 1;
				} else {
					index = start;
				}
				break;
			} else {
				sl_size mid = (start + end) / 2;
				int c = compare(list[mid], what);
				if (c == 0) {
					if (pIndexPrev) {
						*pIndexPrev = mid;
					}
					return sl_true;
				} else if (c < 0) {
					start = mid + 1;
				} else {
					if (start >= mid) {
						index = mid;
						break;
					}
					end = mid - 1;
				}
			}
		}
		if (pIndexPrev) {
			*pIndexPrev = index;
		}
		return sl_false;
	}

}

#endif
