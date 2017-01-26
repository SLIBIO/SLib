#include "../../../inc/slib/ui/mobile_game.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(MobileGame, MobileApp)

MobileGame::MobileGame()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	m_gameView = new MobileGameView;

	m_contentView->removeAllChildren(UIUpdateMode::Init);
	m_contentView->addChild(m_gameView, UIUpdateMode::Init);
	m_contentView = m_gameView;
	m_gameView->addChild(m_pager, UIUpdateMode::Init);

}

Ref<MobileGame> MobileGame::getApp()
{
	return CastRef<MobileGame>(Application::getApp());
}

Ref<MobileGameView> MobileGame::getGameView()
{
	return m_gameView;
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
	
	setRedrawMode(RedrawMode::WhenDirty);
	setDispatchingEventsToRenderingThread(sl_true);
	
}

SLIB_UI_NAMESPACE_END

