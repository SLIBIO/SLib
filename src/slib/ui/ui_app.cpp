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

#include "slib/network/url_request.h"

#include "slib/ui/core.h"
#include "slib/ui/window.h"
#include "slib/ui/menu.h"
#include "slib/ui/platform.h"

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
#ifdef SLIB_UI_IS_MACOS
				Ref<Menu> menu = window->getMenu();
				if (menu.isNotNull()) {
					setMenu(menu);
				}
#endif
			}
#endif
		}
	}
	
	Ref<Menu> UIApp::getMenu()
	{
		return m_mainMenu;
	}

	sl_bool UIApp::shouldOpenUntitledFile()
	{
		return sl_false;
	}
	
#if !defined(SLIB_UI_IS_MACOS)
	void UIApp::setMenu(const Ref<Menu>& menu)
	{
		m_mainMenu = menu;
	}

	sl_bool UIApp::isMenuBarVisible()
	{
		return sl_false;
	}

	void UIApp::setMenuBarVisible(sl_bool flagVisible)
	{
	}

	void UIApp::setVisibleOnDock(sl_bool flagVisible)
	{
	}

	void UIApp::activate(sl_bool flagIgnoreOtherApps)
	{
	}
#endif
	
	void UIApp::onRunApp()
	{
		UI::runApp();
	}

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	void UIApp::onExistingInstance()
	{		
	}
#endif


	SLIB_DEFINE_EVENT_HANDLER(UIApp, Start)
	
	void UIApp::dispatchStart()
	{
		UrlRequest::setDefaultDispatcher(UI::getDispatcher());
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
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenUrl, const String& url, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenUrl(const String& url, sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenUrl, url, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenUrlToApp(const String& url)
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenUrl(url, bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenUrls, const List<String>& urls, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenUrls(const List<String>& urls, sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenUrls, urls, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenUrlsToApp(const List<String>& urls)
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenUrls(urls, bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenFile, const String& filePath, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenFile(const String& filePath, sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenFile, filePath, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenFileToApp(const String& filePath)
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenFile(filePath, bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenFiles, const List<String>& files, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenFiles(const List<String>& files, sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenFiles, files, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenFilesToApp(const List<String>& files)
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenFiles(files, bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenTempFile, const String& filePath, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenTempFile(const String& filePath, sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenTempFile, filePath, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenTempFileToApp(const String& filePath)
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenTempFile(filePath, bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, OpenUntitledFile, sl_bool& outFlagOpened)
	
	void UIApp::dispatchOpenUntitledFile(sl_bool& outFlagOpened)
	{
		SLIB_INVOKE_EVENT_HANDLER(OpenUntitledFile, outFlagOpened);
	}
	
	sl_bool UIApp::dispatchOpenUntitledFileToApp()
	{
		sl_bool bRet = sl_false;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchOpenUntitledFile(bRet);
		}
		return bRet;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(UIApp, Reopen, sl_bool flagHasVisibleWindows, sl_bool& outFlagPerformNormalTasks)
	
	void UIApp::dispatchReopen(sl_bool flagHasVisibleWindows, sl_bool& outFlagPerformNormalTasks)
	{
		SLIB_INVOKE_EVENT_HANDLER(Reopen, flagHasVisibleWindows, outFlagPerformNormalTasks)
	}
	
	sl_bool UIApp::dispatchReopenToApp(sl_bool flagHasVisibleWindows)
	{
		sl_bool bRet = sl_true;
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchReopen(flagHasVisibleWindows, bRet);
		}
		return bRet;
	}

}
