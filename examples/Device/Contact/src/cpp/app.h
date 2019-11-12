#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class ExampleContactApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleContactApp)
public:
	ExampleContactApp();
	
protected:
	Ref<View> getStartupPage() override;

	void onStart() override;
	
};
