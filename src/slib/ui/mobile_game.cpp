#include "../../../inc/slib/ui/mobile_game.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(MobileGame, MobileApp)
	
	MobileGame::MobileGame()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		m_gameView = new MobileGameView;
		
		m_contentView->removeAllChildren(UIUpdateMode::Init);
		m_contentView->addChild(m_gameView, UIUpdateMode::Init);
		m_contentView = m_gameView;
		
		m_gameView->setSizeFilling(1, 1, UIUpdateMode::Init);
		m_gameView->setOpaque(sl_true, UIUpdateMode::Init);
		m_gameView->addChild(m_pager, UIUpdateMode::Init);
		
	}
	
	MobileGame::~MobileGame()
	{
	}

	Ref<MobileGame> MobileGame::getApp()
	{
		return CastRef<MobileGame>(Application::getApp());
	}
	
	Ref<MobileGameView> MobileGame::getGameView()
	{
		return m_gameView;
	}
	
	
	SLIB_DEFINE_OBJECT(MobileGameView, RenderView)
	
	MobileGameView::MobileGameView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setRedrawMode(RedrawMode::WhenDirty);
		setDispatchingEventsToRenderingThread(sl_true);
		
	}

	MobileGameView::~MobileGameView()
	{
	}

}
