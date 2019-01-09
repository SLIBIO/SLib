#include "LaunchScreen.h"

#include "MainPage.h"

void LaunchScreen::onOpen()
{
	dispatchToUiThread([this]() {
		goToHomePage(new MainPage);
	}, 1000);
}
