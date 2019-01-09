#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class SLIB_TEMPLATE_APP_NAMEApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(SLIB_TEMPLATE_APP_NAMEApp)
public:
	SLIB_TEMPLATE_APP_NAMEApp();
	
protected:
	Ref<View> getStartupPage() override;

	void onStart() override;
	
};
