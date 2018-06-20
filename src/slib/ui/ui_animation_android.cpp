/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "ui_animation.h"

#include "view_android.h"

#include "slib/core/safe_static.h"

namespace slib
{

	void _priv_AndroidAnimation_onStop(JNIEnv* env, jobject _this, jlong ptr);

	SLIB_JNI_BEGIN_CLASS(JAndroidAnimation, "slib/platform/android/ui/UiAnimation")
		SLIB_JNI_STATIC_METHOD(start, "start", "(Landroid/view/View;JFFIIZFFZFFFFZFFFFZFFZFF)Lslib/platform/android/ui/UiAnimation;");
		SLIB_JNI_METHOD(stop, "stop", "()V");
		SLIB_JNI_NATIVE(nativeOnStop, "nativeOnStop", "(J)V", _priv_AndroidAnimation_onStop);
	SLIB_JNI_END_CLASS

	typedef CHashMap< sl_reg, Function<void()> > _priv_AndroidAnimationStopMap;
	SLIB_SAFE_STATIC_GETTER(_priv_AndroidAnimationStopMap, _priv_AndroidAnimationStopMap_get);

	sl_bool UIAnimationLoop::_applyNativeAnimation(Animation* animation)
	{
		Ref<View> viewAnimate;
		sl_bool flagTranslate = sl_false;
		Vector2 translateStart, translateEnd;
		sl_bool flagScale = sl_false;
		Vector2 scaleStart, scaleEnd;
		sl_bool flagRotate = sl_false;
		sl_real rotateStart, rotateEnd;
		sl_bool flagAlpha = sl_false;
		sl_real alphaStart, alphaEnd;
		{
			ListLocker< Ref<AnimationTarget> > targets(animation->getTargets());
			for (sl_size i = 0; i < targets.count; i++) {
				AnimationTarget* target = targets[i].get();
				if (target) {
					Ref<View> view;
					if (ViewTranslateAnimationTarget* transform = CastInstance<ViewTranslateAnimationTarget>(target)) {
						if (transform->getFramesCount() > 0) {
							return sl_false;
						}
						view = transform->getView();
						translateStart = transform->getStartValue();
						translateEnd = transform->getEndValue();
						flagTranslate = sl_true;
					} else if (ViewScaleAnimationTarget* transform = CastInstance<ViewScaleAnimationTarget>(target)) {
						if (transform->getFramesCount() > 0) {
							return sl_false;
						}
						view = transform->getView();
						scaleStart = transform->getStartValue();
						scaleEnd = transform->getEndValue();
						flagScale = sl_true;
					} else if (ViewRotateAnimationTarget* transform = CastInstance<ViewRotateAnimationTarget>(target)) {
						if (transform->getFramesCount() > 0) {
							return sl_false;
						}
						view = transform->getView();
						rotateStart = transform->getStartValue();
						rotateEnd = transform->getEndValue();
						flagRotate = sl_true;
					} else if (ViewAlphaAnimationTarget* transform = CastInstance<ViewAlphaAnimationTarget>(target)) {
						if (transform->getFramesCount() > 0) {
							return sl_false;
						}
						view = transform->getView();
						alphaStart = transform->getStartValue();
						alphaEnd = transform->getEndValue();
						flagAlpha = sl_true;
					} else {
						return sl_false;
					}
					if (view.isNull()) {
						return sl_false;
					}
					if (viewAnimate.isNotNull() && viewAnimate != view) {
						return sl_false;
					}
					viewAnimate = view;
				}
			}
		}

		if (viewAnimate.isNull()) {
			return sl_false;
		}

		jobject handle = UIPlatform::getViewHandle(viewAnimate.get());

		if (handle) {

			sl_reg id = animation->getId();

			_priv_AndroidAnimationStopMap* map = _priv_AndroidAnimationStopMap_get();
			if (map) {
				Ref<Animation> _animation = animation;
				Function<void()> onStop = [=](){
					if (flagTranslate) {
						viewAnimate->setTranslation(translateEnd, UIUpdateMode::NoRedraw);
					}
					if (flagScale) {
						viewAnimate->setScale(scaleEnd, UIUpdateMode::NoRedraw);
					}
					if (flagRotate) {
						viewAnimate->setRotation(rotateEnd, UIUpdateMode::NoRedraw);
					}
					if (flagAlpha) {
						viewAnimate->setAlpha(alphaEnd);
					}
					_stopAnimationFromNative(_animation.get());
				};
				map->put(id, onStop);

				Vector2 anchor;
				if (flagTranslate || flagScale || flagRotate) {
					if (!flagTranslate) {
						translateStart = viewAnimate->getTranslation();
					}
					if (!flagScale) {
						scaleStart = viewAnimate->getScale();
					}
					if (!flagRotate) {
						rotateStart = viewAnimate->getRotation();
					}
					if (flagScale || flagRotate) {
						anchor = viewAnimate->getAnchorOffset();
					}
				}
				JniLocal<jobject> ret = JAndroidAnimation::start.callObject(sl_null, handle, (jlong)id, animation->getDuration(), animation->getStartDelay(),
											(int)(animation->getAnimationCurve()), animation->getRepeatCount(), animation->isAutoReverse(),
											anchor.x, anchor.y,
											flagTranslate, translateStart.x, translateStart.y, translateEnd.x, translateEnd.y,
											flagScale, scaleStart.x, scaleStart.y, scaleEnd.x, scaleEnd.y,
											flagRotate, rotateStart, rotateEnd,
											flagAlpha, alphaStart, alphaEnd);
				if (ret.isNotNull()) {
					JniGlobal<jobject> animator = ret.get();
					_setNativeInstance(animation, animator.ref.get());
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	void UIAnimationLoop::_stopNativeAnimation(Animation* animation)
	{
		Ref<Referable> _animator = _getNativeInstance(animation);
		JniGlobal<jobject> animator = CastInstance< CJniGlobal<jobject> >(_animator.get());
		if (animator.isNotNull()) {
			JAndroidAnimation::stop.call(animator.get());
		}
	}

	void _priv_AndroidAnimation_onStop(JNIEnv* env, jobject _this, jlong id)
	{
		_priv_AndroidAnimationStopMap* map = _priv_AndroidAnimationStopMap_get();
		if (map) {
			Function<void()> onStop;
			map->remove((sl_reg)id, &onStop);
			if (onStop.isNotNull()) {
				onStop();
			}
		}
	}

}

#endif
