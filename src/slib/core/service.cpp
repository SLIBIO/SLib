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
	return Ref<Service>::null();
}

#define PID_FILE (System::getTempDirectory() + "/" + appName + ".pid")

void Service::startService()
{
	String appName = getServiceName();

	if (!(isUniqueInstanceRunning())) {

		String pathPID = PID_FILE;
		if (File::exists(pathPID)) {
			File::deleteFile(pathPID);
			if (File::exists(pathPID)) {
				SLIB_LOG(TAG, "Cannot remove temporary PID file of " + appName);
				return;
			}
		}

		SLIB_LOG(TAG, "Starting " + appName);
		
		String appPath = System::getApplicationPath();
		System::createProcess(appPath, sl_null, 0);

		for (int i = 0; i < WAIT_SECONDS*10; i++) {
			if (isUniqueInstanceRunning()) {
				if (File::exists(pathPID)) {
					if (isUniqueInstanceRunning()) {
						SLIB_LOG(TAG, appName + " is STARTED");
						return;
					} else {
						break;
					}
				}
			}
			System::sleep(100);
		}
		SLIB_LOG_ERROR(TAG, appName + " is NOT STARTED");
	} else {
		SLIB_LOG_ERROR(TAG, appName + " is ALREADY RUNNING");
	}
}

void Service::stopService()
{
	String appName = getServiceName();
	
	if (!(isUniqueInstanceRunning())) {
		
		SLIB_LOG_ERROR(TAG, appName + " is NOT RUNNING");
		
	} else {
		
		String pathPID = PID_FILE;
		if (File::exists(pathPID)) {
			SLIB_LOG(TAG, "Stopping " + appName);
			if (File::deleteFile(pathPID)) {
				for (int i = 0; i < WAIT_SECONDS * 10; i++) {
					if (!(isUniqueInstanceRunning())) {
						SLIB_LOG(TAG, appName + " is STOPPED");
						return;
					}
					System::sleep(100);
				}
				SLIB_LOG_ERROR(TAG, appName + " is NOT STOPPED");
			} else {
				SLIB_LOG_ERROR(TAG, "FAILED to delete PID file");
			}
		} else {
			SLIB_LOG_ERROR(TAG, "PID file is not exist");
		}
	}
}

void Service::statusService()
{
	String appName = getServiceName();
	if (isUniqueInstanceRunning()) {
		SLIB_LOG(TAG, appName + " is RUNNING");
	} else {
		SLIB_LOG(TAG, appName + " is NOT RUNNING");
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
	SLIB_LOG(TAG, "Can not run on mobile platforms");
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
		SLIB_LOG_ERROR(TAG, "INVALID COMMAND - " + command);
	}
#endif
}

void Service::onRunApp()
{
	String appName = getServiceName();

	if (!(onStartService())) {
		return;
	}
	
	String pidFileName = PID_FILE;
	if (!File::writeAllTextUTF8(pidFileName, String::fromUint64(System::getProcessId()))) {
		SLIB_LOG_ERROR(TAG, "FAILED to create PID file");
		return;
	}

	while (1) {
		if (File::exists(pidFileName)) {
			System::sleep(500);
		} else {
			break;
		}
	}
	
	onStopService();
	
	File::deleteFile(pidFileName);
	
}

String Service::getUniqueInstanceId()
{
	return getServiceName();
}

SLIB_NAMESPACE_END
