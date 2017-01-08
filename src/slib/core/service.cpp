#include "../../../inc/slib/core/service.h"

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/file.h"

#define TAG "Service"
#define WAIT_SECONDS 300

SLIB_NAMESPACE_BEGIN

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
	
	String stopId = appName + STOP_ID;
	void* stopInstance = System::createGlobalUniqueInstance(stopId);
	if (stopInstance) {
		System::freeGlobalUniqueInstance(stopInstance);
	} else {
		LogError(TAG, "OTHER PROCESS IS STOPPING %s", appName);
		return;
	}

	if (!(isUniqueInstanceRunning())) {

		Log(TAG, "STARING %s", appName);
		
		String appPath = System::getApplicationPath();
		System::createProcess(appPath, sl_null, 0);

		for (int i = 0; i < WAIT_SECONDS*10; i++) {
			String startId = appName + START_ID;
			void* startInstance = System::createGlobalUniqueInstance(startId);
			if (startInstance) {
				System::freeGlobalUniqueInstance(startInstance);
			} else {
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
		String stopId = appName + STOP_ID;
		void* stopInstance = System::createGlobalUniqueInstance(stopId);
		if (stopInstance) {
			Log(TAG, "STOPPING %s", appName);
			for (int i = 0; i < WAIT_SECONDS * 10; i++) {
				if (!(isUniqueInstanceRunning())) {
					System::freeGlobalUniqueInstance(stopInstance);
					Log(TAG, "%s IS STOPPED", appName);
					return;
				}
				System::sleep(100);
			}
			System::freeGlobalUniqueInstance(stopInstance);
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
	
	String startId = appName + START_ID;
	void* startInstance = sl_null;
	for (int i = 0; i < WAIT_SECONDS * 10; i++) {
		startInstance = System::createGlobalUniqueInstance(startId);
		if (startInstance) {
			break;
		}
		System::sleep(100);
	}
	if (!startInstance) {
		LogError(TAG, "FAILED TO CREATE STARTED SIGNAL");
		dispatchStopService();
		return;
	}

	String stopId = appName + STOP_ID;
	
	while (1) {
		void* stopInstance = System::createGlobalUniqueInstance(stopId);
		if (stopInstance) {
			System::freeGlobalUniqueInstance(stopInstance);
		} else {
			break;
		}
		System::sleep(500);
	}
	
	dispatchStopService();
	
	System::freeGlobalUniqueInstance(startInstance);
	
}

String Service::getUniqueInstanceId()
{
	return getServiceName();
}

SLIB_NAMESPACE_END
