#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class SLIB_TEMPLATE_APP_NAMEApp : public UIApp
{
	SLIB_DECLARE_APPLICATION(SLIB_TEMPLATE_APP_NAMEApp)
public:
	SLIB_TEMPLATE_APP_NAMEApp();
	
protected:
	void onStart() override;
	
};
