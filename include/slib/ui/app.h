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

#ifndef CHECKHEADER_SLIB_UI_APP
#define CHECKHEADER_SLIB_UI_APP

#include "definition.h"

#include "../core/app.h"
#include "../core/function.h"

namespace slib
{

	class Window;
	class Menu;
	
	class SLIB_EXPORT UIApp : public Application
	{
		SLIB_DECLARE_OBJECT
		
	public:
		UIApp();
		
		~UIApp();

	public:
		static Ref<UIApp> getApp();
		
	public:
		AppType getAppType() override;
		
		static void quit();
		
		
		Ref<Window> getMainWindow();
		
		void setMainWindow(const Ref<Window>& window);
		
		Ref<Menu> getMenu();
		
		void setMenu(const Ref<Menu>& menu);
		
		
		static sl_bool isMenuBarVisible();
		
		static void setMenuBarVisible(sl_bool flagVisible);
				
		static void setVisibleOnDock(sl_bool flagVisible);
		
		static void activate(sl_bool flagIgnoreOtherApps = sl_true);
		
	protected:
		void onRunApp() override;
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(UIApp, Start)
		SLIB_DECLARE_EVENT_HANDLER(UIApp, Exit)
	
		// macOS
		SLIB_DECLARE_EVENT_HANDLER(UIApp, Reopen, sl_bool flagHasVisibleWindows, sl_bool& outFlagPerformNormalTasks)

	public:
		static void dispatchStartToApp();
		static void dispatchExitToApp();
		
		static void dispatchReopenToApp(sl_bool flagHasVisibleWindows, sl_bool& outFlagPerformNormalTasks);

	private:
		AtomicRef<Window> m_mainWindow;
		AtomicRef<Menu> m_mainMenu;
		
	};

}

#endif
