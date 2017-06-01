/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

	template <class T1, class T2, class COMPARE>
	sl_bool BinarySearch::search(const T1* list, sl_size size, const T2& what, sl_size* pIndexGreaterEqual, const COMPARE& compare)
	{
		if (size == 0) {
			if (pIndexGreaterEqual) {
				*pIndexGreaterEqual = 0;
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
					if (pIndexGreaterEqual) {
						*pIndexGreaterEqual = start;
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
					if (pIndexGreaterEqual) {
						*pIndexGreaterEqual = mid;
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
		if (pIndexGreaterEqual) {
			*pIndexGreaterEqual = index;
		}
		return sl_false;
	}

}
