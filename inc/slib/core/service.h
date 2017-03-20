/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SERVICE
#define CHECKHEADER_SLIB_CORE_SERVICE

#include "definition.h"

#include "app.h"
#include "string.h"
#include "thread.h"

namespace slib
{
	
	class SLIB_EXPORT Service : public Application
	{
		SLIB_DECLARE_OBJECT

	public:
		Service();

		~Service();

	public:
		// override
		AppType getAppType();
	
	protected:
		virtual String getServiceName() = 0;

		virtual sl_bool onStartService() = 0;

		virtual void onStopService() = 0;

		// override
		void doRun();
	
		// override
		void onRunApp();

		// override
		String getUniqueInstanceId();

	protected:
		virtual sl_bool dispatchStartService();

		virtual void dispatchStopService();

	public:
		void startService();

		void stopService();

		void statusService();

		void runService();
	
		static Ref<Service> getApp();
	
	};

}

#endif
