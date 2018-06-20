/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui_animation.h"

#include "slib/ui/core.h"
#include "slib/core/safe_static.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(UIAnimationLoop, AnimationLoop)
	
	UIAnimationLoop::UIAnimationLoop()
	{
		m_flagRunning = sl_false;
	}
	
	UIAnimationLoop::~UIAnimationLoop()
	{
	}
	
	sl_bool UIAnimationLoop::startNativeAnimation(Animation* animation)
	{
		if (!animation) {
			return sl_false;
		}
		if (_applyNativeAnimation(animation)) {
			return sl_true;
		}
		return sl_false;
	}
	
	void UIAnimationLoop::stopNativeAnimation(Animation* animation)
	{
		if (!animation) {
			return;
		}
		_stopNativeAnimation(animation);
	}
	
	void UIAnimationLoop::_wake()
	{
		if (!m_flagRunning) {
			m_flagRunning = sl_true;
			UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(UIAnimationLoop, _run, this));
		}
	}
	
	void UIAnimationLoop::_run()
	{
		sl_int32 n = _runStep();
		if (n < 0) {
			m_flagRunning = sl_false;
		} else {
			UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(UIAnimationLoop, _run, this), n);
		}
	}
	
	Ref<UIAnimationLoop> UIAnimationLoop::getInstance()
	{
		SLIB_SAFE_STATIC(Ref<UIAnimationLoop>, ret, new UIAnimationLoop)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		return ret;
	}
	
#if !defined(SLIB_UI_IS_ANDROID) && !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_MACOS)
	sl_bool UIAnimationLoop::_applyNativeAnimation(Animation* animation)
	{
		return sl_false;
	}
	
	void UIAnimationLoop::_stopNativeAnimation(Animation* animation)
	{
	}
#endif
	
	
	SLIB_DEFINE_OBJECT(ViewTransformAnimationTarget, AnimationTargetT<Matrix3>)
	
	ViewTransformAnimationTarget::ViewTransformAnimationTarget(const Ref<View>& view, const AnimationFrames<Matrix3>& frames):
 AnimationTargetT<Matrix3>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewTransformAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewTransformAnimationTarget::update(float fraction, const Matrix3& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setTransform(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewTranslateAnimationTarget, AnimationTargetT<Vector2>)
	
	ViewTranslateAnimationTarget::ViewTranslateAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames):
 AnimationTargetT<Vector2>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewTranslateAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewTranslateAnimationTarget::update(float fraction, const Vector2& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setTranslation(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewScaleAnimationTarget, AnimationTargetT<Vector2>)
	
	ViewScaleAnimationTarget::ViewScaleAnimationTarget(const Ref<View>& view, const AnimationFrames<Vector2>& frames):
 AnimationTargetT<Vector2>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewScaleAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewScaleAnimationTarget::update(float fraction, const Vector2& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setScale(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewRotateAnimationTarget, AnimationTargetT<sl_real>)
	
	ViewRotateAnimationTarget::ViewRotateAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames):
 AnimationTargetT<sl_real>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewRotateAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewRotateAnimationTarget::update(float fraction, const sl_real& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setRotation(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewFrameAnimationTarget, AnimationTargetT<Rectangle>)
	
	ViewFrameAnimationTarget::ViewFrameAnimationTarget(const Ref<View>& view, const AnimationFrames<Rectangle>& frames):
 AnimationTargetT<Rectangle>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewFrameAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewFrameAnimationTarget::update(float fraction, const Rectangle& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->requestFrame(value);
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewAlphaAnimationTarget, AnimationTargetT<sl_real>)
	
	ViewAlphaAnimationTarget::ViewAlphaAnimationTarget(const Ref<View>& view, const AnimationFrames<sl_real>& frames):
	AnimationTargetT<sl_real>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewAlphaAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewAlphaAnimationTarget::update(float fraction, const sl_real& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setAlpha(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
	
	SLIB_DEFINE_OBJECT(ViewBackgroundColorAnimationTarget, AnimationTargetT<Color4f>)
	
	ViewBackgroundColorAnimationTarget::ViewBackgroundColorAnimationTarget(const Ref<View>& view, const AnimationFrames<Color4f>& frames):
 AnimationTargetT<Color4f>(frames), m_view(view)
	{
	}
	
	Ref<View> ViewBackgroundColorAnimationTarget::getView()
	{
		return m_view;
	}
	
	void ViewBackgroundColorAnimationTarget::update(float fraction, const Color4f& value)
	{
		Ref<View> view(m_view);
		if (view.isNotNull()) {
			view->setBackgroundColor(value);
		} else {
			Ref<Animation> animation = getAnimation();
			if (animation.isNotNull()) {
				animation->removeTarget(this);
			}
		}
	}
}
