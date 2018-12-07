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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_IOS)

#include "slib/ui/core.h"
#include "slib/core/platform_apple.h"

namespace slib
{
	sl_bool UI::isUiThread()
	{
		return [NSThread isMainThread];
	}
	
	void UI::dispatchToUiThread(const Function<void()>& _callback, sl_uint32 delayMillis)
	{
		const Function<void()> callback = _callback;
		if (callback.isNotNull()) {
			if (delayMillis == 0) {
				dispatch_async(dispatch_get_main_queue(), ^{
					callback();
				});
			} else {
				dispatch_time_t t = dispatch_time(DISPATCH_TIME_NOW, (sl_int64)(delayMillis) * NSEC_PER_MSEC);
				dispatch_after(t, dispatch_get_main_queue(), ^{
					callback();
				});
			}
		}
	}
}

#endif
