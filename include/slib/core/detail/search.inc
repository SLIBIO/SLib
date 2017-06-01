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
	sl_bool BinarySearch::search(const T1* list, sl_size size, const T2& what, sl_size* pIndexGreaterEqual, const COMPARE& compare) noexcept
	{
		sl_size mid;
		if (size > 0) {
			sl_size start = 0;
			sl_size end = size - 1;
			do {
				mid = (start + end) >> 1;
				int c = compare(list[mid], what);
				if (c == 0) {
					if (pIndexGreaterEqual) {
						*pIndexGreaterEqual = mid;
					}
					return sl_true;
				} else if (c < 0) {
					if (end == mid) {
						mid++;
						break;
					}
					start = mid + 1;
				} else {
					if (start == mid) {
						break;
					}
					end = mid - 1;
				}
			} while(1);
		} else {
			mid = 0;
		}
		if (pIndexGreaterEqual) {
			*pIndexGreaterEqual = mid;
		}
		return sl_false;
	}

}
