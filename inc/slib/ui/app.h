#ifndef CHECKHEADER_SLIB_UI_APP
#define CHECKHEADER_SLIB_UI_APP

#include "definition.h"

#include "../core/app.h"

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
		// override
		AppType getAppType();
		
		static void quit();
		
		Ref<Window> getMainWindow();
		
		void setMainWindow(const Ref<Window>& window);
		
		// Menu Bar (Mainly used in OSX)
		Ref<Menu> getMenu();
		
		// Menu Bar (Mainly used in OSX)
		void setMenu(const Ref<Menu>& menu);
		
	protected:
		virtual void onStart();
		
		virtual void onExit();
		
		// override
		void onRunApp();
		
	public:
		virtual void dispatchStart();
		
		static void dispatchStartToApp();
		
		virtual void dispatchExit();
		
		static void dispatchExitToApp();
		
	private:
		AtomicRef<Window> m_mainWindow;
		AtomicRef<Menu> m_mainMenu;
		
	};

}

#endif
