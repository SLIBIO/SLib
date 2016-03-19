#ifndef CHECKHEADER_SLIB_CORE_SERVICE
#define CHECKHEADER_SLIB_CORE_SERVICE

#include "definition.h"

#include "app.h"
#include "string.h"
#include "thread.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Service : public Application
{
	SLIB_DECLARE_OBJECT
	
public:
	Service();
	
	~Service();
	
public:
	// override
	AppType getAppType();
	
	// override
	void run(const String& command);
	
	void run();
	

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

	
	Ref<Thread> getThread();
	
	static Ref<Service> getApp();

private:
	void _release();

private:
	SafeRef<Thread> m_threadRun;
	
};

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_SERVICE

