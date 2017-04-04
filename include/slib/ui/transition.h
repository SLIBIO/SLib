/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_TRANSITION
#define CHECKHEADER_SLIB_TRANSITION

#include "definition.h"

#include "view.h"

namespace slib
{

	enum class TransitionType
	{
		None = 0,
		Push = 1,
		Slide = 2,
		Cover = 3,
		Zoom = 4,
		Fade = 5,
		FadeFrontAndBack = 6,
		Default = 100
	};
	
	enum class TransitionDirection
	{
		FromRightToLeft = 0,
		FromLeftToRight = 1,
		FromBottomToTop = 2,
		FromTopToBottom = 3,
		Default = 100
	};
	
	class SLIB_EXPORT Transition
	{
	public:
		TransitionType type;
		TransitionDirection direction;
		float duration; // seconds
		AnimationCurve curve; // curve
		
	public:
		Transition();

		Transition(const Transition& other);

		Transition& operator=(const Transition& other);
		
		~Transition();

	public:
		static Ref<Animation> start(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Function<void()>& onStop);
		
		static Ref<Animation> createAnimation(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Function<void()>& onStop);
		
		static Ref<Animation> startPopup(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Function<void()>& onStop);
		
		static Ref<Animation> createPopupAnimation(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Function<void()>& onStop);
		
	};

}

#endif
