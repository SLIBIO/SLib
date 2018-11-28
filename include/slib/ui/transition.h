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
