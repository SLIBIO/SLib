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
