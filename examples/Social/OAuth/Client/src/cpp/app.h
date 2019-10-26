#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class OAuthClientApp : public UIApp
{
	SLIB_DECLARE_APPLICATION(OAuthClientApp)
public:
	OAuthClientApp();
	
protected:
	void onStart() override;
	
};
