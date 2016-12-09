#ifndef CHECKHEADER_SLIB_CORE_SORT
#define CHECKHEADER_SLIB_CORE_SORT

#include "definition.h"

#include "cpp.h"

SLIB_NAMESPACE_BEGIN

template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT SelectionSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);
	
	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAscending = sl_true);
	
};

template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT InsertionSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);
	
	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAsc = sl_true);
	
	static void sortAsc(const TYPE* src, TYPE* dst, sl_size size);
	
	static void sortDesc(const TYPE* src, TYPE* dst, sl_size size);
	
	static void sort(const TYPE* src, TYPE* dst, sl_size size, sl_bool flagAscending = sl_true);
	
};

template < class TYPE, class COMPARE = Compare<TYPE> >
class SLIB_EXPORT QuickSort
{
public:
	static void sortAsc(TYPE* list, sl_size size);
	
	static void sortDesc(TYPE* list, sl_size size);
	
	static void sort(TYPE* list, sl_size size, sl_bool flagAscending = sl_true);
	
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
			swap(list[sel], list[i]);
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
			swap(list[sel], list[i]);
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
			Swap(list[mid], list[start]);
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
			Swap(list[mid], list[start]);
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
