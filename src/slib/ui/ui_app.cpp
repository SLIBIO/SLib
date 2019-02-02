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

#include "slib/ui/app.h"
#include "slib/ui/core.h"
#include "slib/ui/window.h"
#include "slib/ui/menu.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(UIApp, Application)
	
	UIApp::UIApp()
	{
	}
	
	UIApp::~UIApp()
	{
	}

	Ref<UIApp> UIApp::getApp()
	{
		return CastRef<UIApp>(Application::getApp());
	}
	
	AppType UIApp::getAppType()
	{
		return AppType::UI;
	}
	
	void UIApp::quit()
	{
		UI::quitApp();
	}
	
	Ref<Window> UIApp::getMainWindow()
	{
		return m_mainWindow;
	}
	
	void UIApp::setMainWindow(const Ref<Window>& window)
	{
		void* _thiz = this;
		if (_thiz) {
			m_mainWindow = window;
#ifdef SLIB_PLATFORM_IS_DESKTOP
			if (window.isNotNull()) {
				window->addOnDestroy([](Window*) {
					UI::quitApp();
				});
			}
#endif
		}
	}
	
	Ref<Menu> UIApp::getMenu()
	{
		return m_mainMenu;
	}
	
#if !defined(SLIB_UI_IS_MACOS)
	void UIApp::setMenu(const Ref<Menu>& menu)
	{
		m_mainMenu = menu;
	}
#endif
	
	void UIApp::onRunApp()
	{
		UI::runApp();
	}
	
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, Start)
	
	void UIApp::dispatchStart()
	{
		SLIB_INVOKE_EVENT_HANDLER(Start)
	}
	
	void UIApp::dispatchStartToApp()
	{
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchStart();
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, Exit)

	void UIApp::dispatchExit()
	{
		SLIB_INVOKE_EVENT_HANDLER(Exit)
	}
	
	void UIApp::dispatchExitToApp()
	{
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchExit();
		}
	}
}
