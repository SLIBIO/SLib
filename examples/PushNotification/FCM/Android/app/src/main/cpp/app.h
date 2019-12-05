#pragma once

#include <slib.h>

using namespace slib;

class ExampleFCMApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleFCMApp)
public:
	ExampleFCMApp();
	
protected:
	void onStart() override;
	
};
