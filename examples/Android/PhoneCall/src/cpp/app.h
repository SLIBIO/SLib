#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class PhoneCallApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(PhoneCallApp)
public:
	PhoneCallApp();
	
protected:
	Ref<View> getStartupPage() override;

	void onStart() override;
	
};
