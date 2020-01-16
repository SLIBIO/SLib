#pragma once

#include <slib.h>

#include "../gen/resources.h"

using namespace slib;
using namespace sapp;

class DragAndDropApp : public UIApp
{
	SLIB_DECLARE_APPLICATION(DragAndDropApp)
public:
	DragAndDropApp();
	
protected:
	void onStart() override;
	
};
