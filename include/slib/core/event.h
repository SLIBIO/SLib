/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_EVENT
#define CHECKHEADER_SLIB_CORE_EVENT

#include "definition.h"

#include "ref.h"

namespace slib
{
	
	class SLIB_EXPORT Event : public Referable
	{
		SLIB_DECLARE_OBJECT

	protected:
		Event();

		~Event();
	
	public:
		static Ref<Event> create(sl_bool flagAutoReset = sl_true);

	public:
		void set();

		void reset();

		// milliseconds. negative means INFINITE
		sl_bool wait(sl_int32 timeout = -1);
	
	protected:
		virtual void __set() = 0;

		virtual void __reset() = 0;

		virtual sl_bool __wait(sl_int32 timeout) = 0;
	
	};

}

#endif
