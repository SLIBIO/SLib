/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_ANIMATION
#define CHECKHEADER_SLIB_UI_ANIMATION

#include "definition.h"

#include "../core/animation.h"

namespace slib
{

	class UIAnimationLoop : public AnimationLoop
	{
		SLIB_DECLARE_OBJECT
		
	private:
		UIAnimationLoop();
		
		~UIAnimationLoop();
		
	public:
		static Ref<UIAnimationLoop> getInstance();
		
	public:
		sl_bool startNativeAnimation(Animation* animation) override;
		
		void stopNativeAnimation(Animation* animation) override;
		
	protected:
		void _wake() override;
		
		void _run();
		
		sl_bool _applyNativeAnimation(Animation* animation);
		
		void _stopNativeAnimation(Animation* animation);
		
	private:
		sl_bool m_flagRunning;
		
	};

}

#endif
