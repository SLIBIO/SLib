#include "MainMenu.h"

void MainMenu::onOpen()
{
	btnHelloWorld->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickHelloWorld, this));
	btnViewPager->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickViewPager, this));
}

void MainMenu::onClickHelloWorld(View* view)
{
	goToPage(new example::ui::HelloWorld);
}

void MainMenu::onClickViewPager(View* view)
{
	goToPage(new example::ui::ViewPager);
}
