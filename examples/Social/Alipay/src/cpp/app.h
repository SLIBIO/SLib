#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class ExampleAlipayApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleAlipayApp)
public:
	ExampleAlipayApp();
	
protected:
	Ref<View> getStartupPage() override;

	void onStart() override;
	
};
