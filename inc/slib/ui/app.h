#ifndef CHECKHEADER_SLIB_UI_APP
#define CHECKHEADER_SLIB_UI_APP

#include "definition.h"

#include "../core/app.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class Menu;

class SLIB_EXPORT UIApp : public Application
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<UIApp> getApp();
	
public:
	// override
	AppType getAppType();
	
	// override
	void run(const String& param);
	
	void run();
	
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

public:
	virtual void dispatchStart();
	
	static void dispatchStartToApp();
	
	virtual void dispatchExit();
	
	static void dispatchExitToApp();
	
private:
	SafeRef<Window> m_mainWindow;
	SafeRef<Menu> m_mainMenu;
	
};

SLIB_UI_NAMESPACE_END

#endif
