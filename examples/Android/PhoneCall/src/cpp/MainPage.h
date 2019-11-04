#pragma once

#include "app.h"

class MainPage : public ui::MainPage
{
protected:
	void init() override;

	void onOpen() override;

	void onResume() override;

};
