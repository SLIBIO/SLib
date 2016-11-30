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

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_SERVICE

