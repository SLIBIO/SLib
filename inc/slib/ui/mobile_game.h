/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_MOBILE_GAME
#define CHECKHEADER_SLIB_UI_MOBILE_GAME

#include "definition.h"

#include "mobile_app.h"
#include "render_view.h"

namespace slib
{

	class MobileGameView;
	
	class SLIB_EXPORT MobileGame : public MobileApp
	{
		SLIB_DECLARE_OBJECT
		
	public:
		MobileGame();
		
		~MobileGame();

	public:
		static Ref<MobileGame> getApp();
		
		Ref<MobileGameView> getGameView();
		
	private:
		Ref<MobileGameView> m_gameView;
		
	};
	
	class SLIB_EXPORT MobileGameView : public RenderView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		MobileGameView();
		
		~MobileGameView();

	};

}

#endif
