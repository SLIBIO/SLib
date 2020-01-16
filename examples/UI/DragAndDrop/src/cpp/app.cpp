#include "app.h"

#include "MainWindow.h"

SLIB_DEFINE_APPLICATION(DragAndDropApp, UIApp)

DragAndDropApp::DragAndDropApp()
{
}

void DragAndDropApp::onStart()
{
	Ref<MainWindow> window = new MainWindow;
	window->create();
	setMainWindow(window);
}
