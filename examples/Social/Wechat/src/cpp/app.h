#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class ExampleWechatApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleWechatApp)
public:
	ExampleWechatApp();
	
protected:
	Ref<View> getStartupPage() override;

	void onStart() override;
	
};
