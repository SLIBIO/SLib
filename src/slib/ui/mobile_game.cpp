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

#include "slib/ui/mobile_game.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(MobileGame, MobileApp)
	
	MobileGame::MobileGame()
	{
	}

	MobileGame::~MobileGame()
	{
	}

	void MobileGame::init()
	{
		MobileApp::init();
		
		m_gameView = new MobileGameView;
		
		m_contentView->removeAllChildren(UIUpdateMode::Init);
		m_contentView->addChild(m_gameView, UIUpdateMode::Init);
		m_contentView = m_gameView;
		
		m_gameView->setWidthFilling(1, UIUpdateMode::Init);
		m_gameView->setHeightFilling(1, UIUpdateMode::Init);
		m_gameView->setOpaque(sl_true, UIUpdateMode::Init);
		m_gameView->addChild(m_navigationController, UIUpdateMode::Init);
		
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
		setRedrawMode(RedrawMode::WhenDirty);
		setDispatchingEventsToRenderingThread(sl_true);		
	}

	MobileGameView::~MobileGameView()
	{
	}

}
