#ifndef CHECKHEADER_SLIB_CORE_SEARCH
#define CHECKHEADER_SLIB_CORE_SEARCH

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template < class TYPE1, class TYPE2=TYPE1, class COMPARE=Compare<TYPE1, TYPE2> >
class SLIB_EXPORT BinarySearch
{
public:
	static sl_bool search(const TYPE1* list, sl_size size, const TYPE2& what, sl_size* pIndexPrev = sl_null);
};


template <class TYPE1, class TYPE2, class COMPARE>
sl_bool BinarySearch<TYPE1, TYPE2, COMPARE>::search(const TYPE1* list, sl_size size, const TYPE2& what, sl_size* pIndexPrev)
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
			int c = COMPARE::compare(list[start], what);
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
			int c = COMPARE::compare(list[mid], what);
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

SLIB_NAMESPACE_END

#endif
