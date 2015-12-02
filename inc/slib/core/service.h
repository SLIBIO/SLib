#ifndef CHECKHEADER_SLIB_CORE_SERVICE
#define CHECKHEADER_SLIB_CORE_SERVICE

#include "definition.h"
#include "app.h"
#include "string.h"
#include "thread.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Service : public Application
{
	SLIB_DECLARE_OBJECT(Service, Application)
public:
	Service();
	~Service();

protected:
	virtual String getServiceName() = 0;
	virtual void onRunService() = 0;

	virtual sl_bool onStartService();
	virtual void onStopService();
	
public:
	void startService();
	void stopService();
	void statusService();
	void runService();

	Type getAppType();
	void run();
	void run(const String& command);

	Ref<Thread> getThread();
	static Ref<Service> getApp();

private:
	Ref<Thread> m_threadRun;
	void release();
};
SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_SERVICE

