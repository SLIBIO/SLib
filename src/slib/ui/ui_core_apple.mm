/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/core/platform_apple.h"

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
