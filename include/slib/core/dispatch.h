/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_DISPATCH
#define CHECKHEADER_SLIB_CORE_DISPATCH

#include "definition.h"

#include "timer.h"

namespace slib
{
	
	class DispatchLoop;
	
	class SLIB_EXPORT Dispatch
	{
	public:
		static sl_bool dispatch(const Ref<DispatchLoop>& loop, const Function<void()>& task);

		static sl_bool dispatch(const Function<void()>& task);

		static sl_bool setTimeout(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 delay_ms);

		static sl_bool setTimeout(const Function<void()>& task, sl_uint64 delay_ms);
	
		static Ref<Timer> setInterval(const Ref<DispatchLoop>& loop, const Function<void(Timer*)>& task, sl_uint64 interval_ms);

		static Ref<Timer> setInterval(const Function<void(Timer*)>& task, sl_uint64 interval_ms);

	};
	
	class SLIB_EXPORT Dispatcher : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Dispatcher();

		~Dispatcher();

	public:
		virtual sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms = 0) = 0;

	};

}

#endif
