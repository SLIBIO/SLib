#include <slib.h>

using namespace slib;

class SLIB_TEMPLATE_APP_NAMEApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(SLIB_TEMPLATE_APP_NAMEApp)
public:
	SLIB_TEMPLATE_APP_NAMEApp();
	
protected:
	void onStart() override;
	
};
