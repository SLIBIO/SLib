#pragma once

#include "app.h"

class MainPage : public ui::MainPage
{
	SLIB_DECLARE_SINGLETON(MainPage)

public:
	void initCallbacks();

protected:
	void onOpen() override;

	void onResume() override;

};
