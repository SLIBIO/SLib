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
		AppType getAppType() override;
	
	protected:
		virtual String getServiceName() = 0;

		virtual sl_bool onStartService() = 0;

		virtual void onStopService() = 0;

		void doRun() override;
	
		void onRunApp() override;

		String getUniqueInstanceId() override;

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
