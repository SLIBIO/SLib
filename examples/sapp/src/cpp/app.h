#include <slib.h>

using namespace slib;

class SAppExampleApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(SAppExampleApp)
public:
	SAppExampleApp();
	
protected:
	void onStart() override;
	
};
