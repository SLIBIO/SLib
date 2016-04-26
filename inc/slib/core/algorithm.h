#ifndef CHECKHEADER_SLIB_CORE_ALGORITHM
#define CHECKHEADER_SLIB_CORE_ALGORITHM

#include "definition.h"

SLIB_NAMESPACE_BEGIN

enum class MapPutMode
{
	AddOrReplace = 0,
	ReplaceExisting = 1,
	AddNew = 2,
	AddAlways = 3,
	Default = AddOrReplace
};

/*************************************************************************
	Compare<TYPE>::compare(v1, v2) returns
		negative: v1 < v2
		0		: v1 = v2
		positive: v1 > v2
*************************************************************************/
template <class TYPE1, class TYPE2=TYPE1>
class SLIB_EXPORT Compare
{
public:
	static int compare(const TYPE1& a, const TYPE2& b);

	static sl_bool equals(const TYPE1& a, const TYPE2& b);
	
};

template <class TYPE1, class TYPE2>
SLIB_INLINE int Compare<TYPE1, TYPE2>::compare(const TYPE1& a, const TYPE2& b)
{
	return (a < b) ? -1 : (a > b);
}

template <class TYPE1, class TYPE2>
SLIB_INLINE sl_bool Compare<TYPE1, TYPE2>::equals(const TYPE1& a, const TYPE2& b)
{
	return a == b;
}


template <class TYPE1, class TYPE2=TYPE1>
class SLIB_EXPORT CompareDescending
{
public:
	static int compare(const TYPE1& a, const TYPE2& b);

	static sl_bool eqauls(const TYPE1& a, const TYPE2& b);
	
};

template <class TYPE1, class TYPE2>
SLIB_INLINE int CompareDescending<TYPE1, TYPE2>::compare(const TYPE1& a, const TYPE2& b)
{
	return - (Compare<TYPE1, TYPE2>::compare(a, b));
}

template <class TYPE1, class TYPE2>
SLIB_INLINE sl_bool CompareDescending<TYPE1, TYPE2>::eqauls(const TYPE1& a, const TYPE2& b)
{
	return a == b;
}

/**************************************************************************
	Hash Functions
**************************************************************************/
SLIB_INLINE sl_uint32 sl_rehash(sl_uint32 code)
{
	code ^= (code >> 20) ^ (code >> 12);
	code ^= (code >> 7) ^ (code >> 4);
	return code;
}

SLIB_INLINE sl_uint32 sl_hash(char n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned char n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(short n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned short n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(int n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned int n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(long n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(unsigned long n)
{
	return sl_rehash((sl_uint32)n);
}

SLIB_INLINE sl_uint32 sl_hash(long long n)
{
	return sl_rehash((sl_uint32)(n ^ (n >> 32)));
}

SLIB_INLINE sl_uint32 sl_hash(unsigned long long n)
{
	return sl_rehash((sl_uint32)(n ^ (n >> 32)));
}

SLIB_INLINE sl_uint32 sl_hash(const void* ptr)
{
#ifdef SLIB_ARCH_IS_64BIT
	return sl_hash((sl_uint64)ptr);
#else
	return sl_hash((sl_uint32)ptr);
#endif
}

SLIB_INLINE sl_uint32 sl_hash(float f)
{
	return sl_rehash(*(sl_uint32*)(void*)(&f));
}

SLIB_INLINE sl_uint32 sl_hash(double f)
{
	return sl_hash(*(sl_uint64*)(void*)(&f));
}

sl_uint32 sl_hash(const void* data, sl_size size);

template <class TYPE>
class SLIB_EXPORT Hash
{
public:
	static sl_uint32 hash(const TYPE& v);
	
};

template <class TYPE>
SLIB_INLINE sl_uint32 Hash<TYPE>::hash(const TYPE& v)
{
	return sl_hash(v);
}


/**************************************************************************
	Binary Search
**************************************************************************/
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


/**************************************************************************
	SelectionSort
**************************************************************************/
template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT SelectionSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);
	
	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAsc = sl_true);

};

template <class TYPE, class COMPARE>
void SelectionSort<TYPE, COMPARE>::sortAsc(TYPE* list, sl_size size)
{
	if (size < 2) {
		return;
	}
	sl_size size_1 = size - 1;
	for (sl_size i = 0; i < size_1; i++) {
		sl_size sel = i;
		for (sl_size j = i + 1; j < size; j++) {
			int c = COMPARE::compare(list[sel], list[j]);
			if (c > 0) {
				sel = j;
			}
		}
		if (sel != i) {
			SLIB_SWAP(list[sel], list[i]);
		}
	}
}

template <class TYPE, class COMPARE>
void SelectionSort<TYPE, COMPARE>::sortDesc(TYPE* list, sl_size size)
{
	if (size < 2) {
		return;
	}
	sl_size size_1 = size - 1;
	for (sl_size i = 0; i < size_1; i++) {
		sl_size sel = i;
		for (sl_size j = i + 1; j < size; j++) {
			int c = COMPARE::compare(list[sel], list[j]);
			if (c < 0) {
				sel = j;
			}
		}
		if (sel != i) {
			SLIB_SWAP(list[sel], list[i]);
		}
	}
}

template <class TYPE, class COMPARE>
void SelectionSort<TYPE, COMPARE>::sort(TYPE* list, sl_size size, sl_bool flagAsc)
{
	if (flagAsc) {
		sortAsc(list, size);
	} else {
		sortDesc(list, size);
	}
}

/**************************************************************************
	InsertionSort
**************************************************************************/
template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT InsertionSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);
	
	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAsc = sl_true);

	static void sortAsc(const TYPE* src, TYPE* dst, sl_size size);
	
	static void sortDesc(const TYPE* src, TYPE* dst, sl_size size);
	
	static void sort(const TYPE* src, TYPE* dst, sl_size size, sl_bool flagAsc = sl_true);

};

template <class TYPE, class COMPARE>
void InsertionSort<TYPE, COMPARE>::sortAsc(TYPE* list, sl_size size)
{
	if (size < 2) {
		return;
	}
	for (sl_size i = 1; i < size; i++) {
		TYPE x = list[i];
		sl_size j = i;
		while (j > 0) {
			int c = COMPARE::compare(list[j - 1], x);
			if (c <= 0) {
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
void InsertionSort<TYPE, COMPARE>::sortDesc(TYPE* list, sl_size size)
{
	if (size < 2) {
		return;
	}
	for (sl_size i = 1; i < size; i++) {
		TYPE x = list[i];
		sl_size j = i;
		while (j > 0) {
			int c = COMPARE::compare(list[j - 1], x);
			if (c >= 0) {
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
void InsertionSort<TYPE, COMPARE>::sort(TYPE* list, sl_size size, sl_bool flagAsc)
{
	if (flagAsc) {
		sortAsc(list, size);
	} else {
		sortDesc(list, size);
	}
}

template <class TYPE, class COMPARE>
void InsertionSort<TYPE, COMPARE>::sortAsc(const TYPE* src, TYPE* dst, sl_size size)
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
			int c = COMPARE::compare(dst[j - 1], src[i]);
			if (c <= 0) {
				break;
			}
			dst[j] = dst[j - 1];
			j--;
		}
		dst[j] = src[i];
	}
}

template <class TYPE, class COMPARE>
void InsertionSort<TYPE, COMPARE>::sortDesc(const TYPE* src, TYPE* dst, sl_size size)
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
			int c = COMPARE::compare(dst[j - 1], src[i]);
			if (c >= 0) {
				break;
			}
			dst[j] = dst[j - 1];
			j--;
		}
		dst[j] = src[i];
	}
}

template <class TYPE, class COMPARE>
void InsertionSort<TYPE, COMPARE>::sort(const TYPE* src, TYPE* dst, sl_size size, sl_bool flagAsc)
{
	if (flagAsc) {
		sortAsc(src, dst, size);
	} else {
		sortDesc(src, dst, size);
	}
}


/**************************************************************************
	QuickSort
**************************************************************************/
template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT QuickSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);

	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAsc = sl_true);
	
};

template <class TYPE, class COMPARE>
void QuickSort<TYPE, COMPARE>::sortAsc(TYPE* list, sl_size size)
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
			InsertionSort<TYPE, COMPARE>::sortAsc(list + start, n);
		} else {
			sl_size mid = start + (n / 2);
			SLIB_SWAP(list[mid], list[start]);
			sl_size border2 = start;
			sl_size border1 = end + 1;
			for (;;) {
				for (;;) {
					border2++;
					if (border2 > end) {
						break;
					}
					int c = COMPARE::compare(list[border2], list[start]);
					if (c > 0) {
						break;
					}
				};
				for (;;) {
					border1--;
					if (border1 <= start) {
						break;
					}
					int c = COMPARE::compare(list[border1], list[start]);
					if (c < 0) {
						break;
					}
				}
				if (border1 < border2) {
					break;
				}
				SLIB_SWAP(list[border1], list[border2]);
			}
			if (border1 != start) {
				SLIB_SWAP(list[border1], list[start]);
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
void QuickSort<TYPE, COMPARE>::sortDesc(TYPE* list, sl_size size)
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
			InsertionSort<TYPE, COMPARE>::sortDesc(list + start, n);
		} else {
			sl_size mid = start + (n / 2);
			SLIB_SWAP(list[mid], list[start]);
			sl_size border2 = start;
			sl_size border1 = end + 1;
			for (;;) {
				for (;;) {
					border2++;
					if (border2 > end) {
						break;
					}
					int c = COMPARE::compare(list[border2], list[start]);
					if (c < 0) {
						break;
					}
				};
				for (;;) {
					border1--;
					if (border1 <= start) {
						break;
					}
					int c = COMPARE::compare(list[border1], list[start]);
					if (c > 0) {
						break;
					}
				}
				if (border1 < border2) {
					break;
				}
				SLIB_SWAP(list[border1], list[border2]);
			}
			if (border1 != start) {
				SLIB_SWAP(list[border1], list[start]);
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
void QuickSort<TYPE, COMPARE>::sort(TYPE* list, sl_size size, sl_bool flagAsc)
{
	if (flagAsc) {
		sortAsc(list, size);
	} else {
		sortDesc(list, size);
	}
}

SLIB_NAMESPACE_END

#endif
