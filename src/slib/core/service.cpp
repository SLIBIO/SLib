#include "../../../inc/slib/core/service.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/file.h"

#define TAG "Service"
#define WAIT_SECONDS 15

SLIB_NAMESPACE_BEGIN

Service::Service()
{
}

Service::~Service()
{
	release();
}

void Service::release()
{
	Ref<Thread> thread = m_threadRun;
	if (thread.isNotNull()) {
		thread->finishAndWait();
		m_threadRun.setNull();
	}
}

Ref<Thread> Service::getThread()
{
	return m_threadRun;
}

Ref<Service> Service::getApp()
{
	Ref<Application> app = Application::getApp();
	if (app.isNotNull() && app->getAppType() == typeService) {
		return Ref<Service>::from(app);
	}
	return Ref<Service>::null();
}

void Service::run(const String& command)
{
	if (command == "start") {
		startService();
	} else if (command == "stop") {
		stopService();
	} else if (command == "status") {
		statusService();
	} else if (command.isEmpty()) {
		runService();
	} else {
		SLIB_LOG_ERROR(TAG, "INVALID COMMAND - " + command);
	}
}

void Service::run()
{
	Application::run();
}

Application::Type Service::getAppType()
{
	return typeService;
}

#define CHECK_MOBILE \
	if (System::isMobilePlatform()) { \
		SLIB_LOG(TAG, "Can not run on mobile platforms"); \
		return; \
	}

#define PID_FILE (System::getTempDirectory() + "/" + appName + ".pid")

void Service::startService()
{
	CHECK_MOBILE;
	String appName = getServiceName();
	void* appInst = System::createGlobalUniqueInstance(appName);
	if (appInst) {
		System::freeGlobalUniqueInstance(appInst);

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
			if (File::exists(pathPID)) {
				SLIB_LOG(TAG, appName + " is STARTED");
				return;
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
	CHECK_MOBILE;
	String appName = getServiceName();
	void* appInst = System::createGlobalUniqueInstance(appName);
	if (appInst) {
		System::freeGlobalUniqueInstance(appInst);
		SLIB_LOG_ERROR(TAG, appName + " is NOT RUNNING");
	} else {
		String pathPID = PID_FILE;
		if (File::exists(pathPID)) {
			SLIB_LOG(TAG, "Stopping " + appName);
			if (File::deleteFile(pathPID)) {
				for (int i = 0; i < WAIT_SECONDS * 10; i++) {
					void* appInst = System::createGlobalUniqueInstance(appName);
					if (appInst) {
						System::freeGlobalUniqueInstance(appInst);
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
	CHECK_MOBILE;
	String appName = getServiceName();
	void* appInst = System::createGlobalUniqueInstance(appName);
	if (appInst) {
		SLIB_LOG("ServiceStatus", appName + " is NOT RUNNING");
		System::freeGlobalUniqueInstance(appInst);
	} else {
		SLIB_LOG("ServiceStatus", appName + " is RUNNING");
	}
}

void Service::runService()
{
	CHECK_MOBILE;
	String appName = getServiceName();
	void* appInst = System::createGlobalUniqueInstance(appName);
	if (! appInst) {
		SLIB_LOG_ERROR(TAG, appName + " is ALREADY RUNNING");
		return;
	}

	String pidFileName = PID_FILE;
	if (!File::writeUtf8Text(pidFileName, String::fromUint64(System::getProcessId()))) {
		SLIB_LOG_ERROR(TAG, "FAILED to create PID file");
		return;
	}
	
	if (onStartService()) {
		m_threadRun = Thread::start(SLIB_CALLBACK_CLASS(Service, onRunService, this));
	}
	while (1)
	{
		if (File::exists(pidFileName)) {
			System::sleep(500);
		} else {
			break;
		}
	}
	onStopService();
	release();
	System::freeGlobalUniqueInstance(appInst);
}

sl_bool Service::onStartService()
{
	return sl_true;
}

void Service::onStopService()
{
}

SLIB_NAMESPACE_END
