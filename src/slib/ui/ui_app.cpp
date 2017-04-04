/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		}
	}
	
	Ref<Menu> UIApp::getMenu()
	{
		return m_mainMenu;
	}
	
#if !defined(SLIB_PLATFORM_IS_OSX)
	
	void UIApp::setMenu(const Ref<Menu>& menu)
	{
		m_mainMenu = menu;
	}
	
#endif
	
	void UIApp::onStart()
	{
	}
	
	void UIApp::onExit()
	{
	}
	
	void UIApp::onRunApp()
	{
		UI::runApp();
	}
	
	void UIApp::dispatchStart()
	{
		onStart();
	}
	
	void UIApp::dispatchStartToApp()
	{
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchStart();
		}
	}
	
	void UIApp::dispatchExit()
	{
		onExit();
	}
	
	void UIApp::dispatchExitToApp()
	{
		Ref<UIApp> app = getApp();
		if (app.isNotNull()) {
			app->dispatchExit();
		}
	}
}
