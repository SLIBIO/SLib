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

#include "slib/core/service.h"

#include "slib/core/system.h"
#include "slib/core/log.h"
#include "slib/core/file.h"

#define TAG "Service"
#define WAIT_SECONDS 300

namespace slib
{

	SLIB_DEFINE_OBJECT(Service, Object)

	Service::Service()
	{
		setCrashRecoverySupport(sl_true);
	}

	Service::~Service()
	{
	}

	AppType Service::getAppType()
	{
		return AppType::Service;
	}

	Ref<Service> Service::getApp()
	{
		Ref<Application> app = Application::getApp();
		if (app.isNotNull() && app->getAppType() == AppType::Service) {
			return Ref<Service>::from(app);
		}
		return sl_null;
	}

	sl_bool Service::dispatchStartService()
	{
		return onStartService();
	}
	
	void Service::dispatchStopService()
	{
		onStopService();
	}

#define START_ID "_STARTED"
#define STOP_ID "_STOPPING"

	void Service::startService()
	{
		String appName = getServiceName();
		if (appName.isEmpty()) {
			LogError(TAG, "SERVICE NAME IS EMPTY");
			return;
		}
		
		if (GlobalUniqueInstance::exists(appName + STOP_ID)) {
			LogError(TAG, "OTHER PROCESS IS STOPPING %s", appName);
			return;
		}
	
		if (!(isUniqueInstanceRunning())) {

			Log(TAG, "STARING %s", appName);

			String appPath = System::getApplicationPath();
			System::createProcess(appPath, sl_null, 0);

			for (int i = 0; i < WAIT_SECONDS*10; i++) {
				if (GlobalUniqueInstance::exists(appName + START_ID)) {
					Log(TAG, "%s IS STARTED", appName);
					return;
				}
				System::sleep(100);
			}
			LogError(TAG, "%s IS NOT STARTED", appName);
		} else {
			LogError(TAG, "%s IS ALREADY RUNNING", appName);
		}
	}

	void Service::stopService()
	{
		String appName = getServiceName();

		if (!(isUniqueInstanceRunning())) {
			LogError(TAG, "%s IS NOT RUNNING", appName);
		} else {
			Ref<GlobalUniqueInstance> stopInstance = GlobalUniqueInstance::create(appName + STOP_ID);
			if (stopInstance.isNotNull()) {
				Log(TAG, "STOPPING %s", appName);
				for (int i = 0; i < WAIT_SECONDS * 10; i++) {
					if (!(isUniqueInstanceRunning())) {
						Log(TAG, "%s IS STOPPED", appName);
						return;
					}
					System::sleep(100);
				}
				LogError(TAG, "%s IS NOT STOPPED", appName);
			} else {
				LogError(TAG, "OTHER PROCESS IS STOPPING %s", appName);
			}
		}
	}

	void Service::statusService()
	{
		String appName = getServiceName();
		if (isUniqueInstanceRunning()) {
			Log(TAG, "%s IS RUNNING", appName);
		} else {
			Log(TAG, "%s IS NOT RUNNING", appName);
		}
	}

	void Service::runService()
	{
		Application::doRun();
	}

	sl_bool Service::onStartService()
	{
		return sl_true;
	}
	
	void Service::onStopService()
	{
	}
	
	void Service::doRun()
	{
#if defined(SLIB_PLATFORM_IS_MOBILE)
		Log(TAG, "Can not run on mobile platforms");
#else
		String command = getCommand();
		if (command == "start") {
			startService();
		} else if (command == "stop") {
			stopService();
		} else if (command == "restart") {
			stopService();
			startService();
		} else if (command == "status") {
			statusService();
		} else if (command.isEmpty()) {
			runService();
		} else {
			LogError(TAG, "INVALID COMMAND - %s", command);
		}
#endif
	}
	
	void Service::onRunApp()
	{
		String appName = getServiceName();

		if (!(dispatchStartService())) {
			dispatchStopService();
			return;
		}

		Ref<GlobalUniqueInstance> startInstance = GlobalUniqueInstance::create(appName + START_ID);

		String stopId = appName + STOP_ID;

		while (1) {
			if (GlobalUniqueInstance::exists(stopId)) {
				break;
			}
			System::sleep(500);
		}

		dispatchStopService();

	}
	
	String Service::getUniqueInstanceId()
	{
		return getServiceName();
	}

}
