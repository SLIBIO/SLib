#include "../../../inc/slib/ui/mobile_game.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileGame, MobileApp)

MobileGame::MobileGame()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	m_gameView = new MobileGameView;
	if (m_gameView.isNotNull()) {
		//addView(m_gameView);
	}
}

Ref<MobileGame> MobileGame::getApp()
{
	Ref<Application> app(Application::getApp());
	if (MobileGame::checkInstance(app.ptr)) {
		return Ref<MobileGame>::from(app);
	}
	return Ref<MobileGame>::null();
}

Ref<MobileGameView> MobileGame::getGameView()
{
	void* _thiz = this;
	if (_thiz) {
		return m_gameView;
	}
	return Ref<MobileGameView>::null();
}

void MobileGame::onGameFrame(RenderEngine* engine)
{
}

void MobileGame::dispatchGameFrame(RenderEngine* engine)
{
	onGameFrame(engine);
}

void MobileGame::dispatchResize(sl_ui_len width, sl_ui_len height)
{
	m_gameView->setFrame(0, 0, width, height);
	MobileApp::dispatchResize(width, height);
}

SLIB_DEFINE_OBJECT(MobileGameView, RenderView)

MobileGameView::MobileGameView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setRedrawMode(RedrawMode::Continuously);
}

void MobileGameView::onFrame(RenderEngine* engine)
{
	Ref<MobileGame> app = MobileGame::getApp();
	if (app.isNotNull()) {
		app->dispatchGameFrame(engine);
	}
}

SLIB_UI_NAMESPACE_END

