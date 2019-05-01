/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "MainMenu.h"

#include "ListViewPage.h"
#include "CameraViewPage.h"
#include "ControlScreenPage.h"
#include "QRCodeScannerPage.h"

void MainMenu::onOpen()
{
	btnHelloWorld->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickHelloWorld, this));
	btnViewPager->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickViewPager, this));
	btnListView->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickListView, this));
	btnDrawer->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickDrawer, this));
	btnVideoView->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickVideoView, this));
	btnCameraView->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickCameraView, this));
	btnWebView->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickWebView, this));
	btnControlScreen->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickControlScreen, this));
	btnLoginPage->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickLoginPage, this));
	btnQRCodeScanner->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickQRCodeScanner, this));
	btnToggleTorch->setOnClick(SLIB_FUNCTION_WEAKREF(MainMenu, onClickToggleTorch, this));
}

void MainMenu::onClickHelloWorld(View* view)
{
	goToPage(new example::ui::HelloWorld);
}

void MainMenu::onClickViewPager(View* view)
{
	goToPage(new example::ui::ViewPager);
}

void MainMenu::onClickListView(View* view)
{
	goToPage(new ListViewPage);
}

void MainMenu::onClickDrawer(View* view)
{
	goToPage(new example::ui::Drawer);
}

void MainMenu::onClickVideoView(View* view)
{
	goToPage(new example::ui::VideoView);
}

void MainMenu::onClickCameraView(View* view)
{
	goToPage(new CameraViewPage);
}

void MainMenu::onClickWebView(View* view)
{
	goToPage(new example::ui::WebView);
}

void MainMenu::onClickControlScreen(View* view)
{
	goToPage(new ControlScreenPage);
}

void MainMenu::onClickLoginPage(View* view)
{
	goToPage(new example::ui::LoginPage);
}

void MainMenu::onClickQRCodeScanner(View* view)
{
	goToPage(new QRCodeScannerPage);
}

void MainMenu::onClickToggleTorch(View* view)
{
	if (Camera::isMobileDeviceTorchActive()) {
		Camera::setMobileDeviceTorchMode(CameraTorchMode::Off);
	} else {
		Camera::setMobileDeviceTorchMode(CameraTorchMode::On);
	}
}
