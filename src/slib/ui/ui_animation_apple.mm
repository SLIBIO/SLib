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

#if defined(SLIB_PLATFORM_IS_OSX)

#include "view_osx.h"
#include <Quartz/Quartz.h>

typedef NSView* NativeView;

#else

#include "view_ios.h"

typedef UIView* NativeView;

#endif

#include "slib/ui/core.h"
#include "slib/math/transform2d.h"

@interface _NativeAnimationDelegate : NSObject<CAAnimationDelegate>
{
	@public slib::Function<void()> onStop;
}
@end

namespace slib
{
	static void _UIAnimation_setAnimation(CAAnimation* ca, const Ref<Animation>& animation, sl_bool flagGroup)
	{
		if (flagGroup) {
			ca.duration = animation->getDuration();
			ca.beginTime = animation->getStartDelay();
		} else {
			ca.duration = animation->getDuration();
			ca.beginTime = 0;
			switch (animation->getAnimationCurve()) {
				case AnimationCurve::EaseIn:
					ca.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn];
					break;
				case AnimationCurve::EaseOut:
					ca.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
					break;
				case AnimationCurve::EaseInOut:
					ca.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
					break;
				default:
					ca.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
					break;
			}
			if (animation->isAutoReverse()) {
				ca.autoreverses = YES;
			}
		}
		sl_int32 nRepeat = animation->getRepeatCount();
		if (nRepeat < 0) {
			ca.repeatCount = HUGE_VALF;
		} else {
			if (nRepeat > 0) {
				ca.repeatCount = nRepeat;
			}
		}
		ca.removedOnCompletion = NO;
		ca.fillMode = kCAFillModeForwards;
		
	}
	
	static void _UIAnimation_applyAnimation(NativeView handle, const Ref<Animation>& animation,
											sl_bool flagTranslate, const Vector2& translateStart, const Vector2& translateEnd,
											sl_bool flagScale, const Vector2& scaleStart, const Vector2& scaleEnd,
											sl_bool flagRotate, sl_real rotateStart, sl_real rotateEnd,
											sl_bool flagAlpha, sl_real alphaStart, sl_real alphaEnd,
											Function<void()> onStop)
	{
		
		CAAnimationGroup* group = [CAAnimationGroup animation];
		NSMutableArray* arr = [NSMutableArray array];
		
		if (flagTranslate) {
			CABasicAnimation* ca = [CABasicAnimation animationWithKeyPath:@"transform.translation"];
#ifdef SLIB_PLATFORM_IS_OSX
			ca.fromValue = [NSValue valueWithSize:CGSizeMake(translateStart.x, translateStart.y)];
			ca.toValue = [NSValue valueWithSize:CGSizeMake(translateEnd.x, translateEnd.y)];
#else
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			ca.fromValue = [NSValue valueWithCGSize:CGSizeMake(translateStart.x / f, translateStart.y / f)];
			ca.toValue = [NSValue valueWithCGSize:CGSizeMake(translateEnd.x / f, translateEnd.y / f)];
#endif
			_UIAnimation_setAnimation(ca, animation, sl_false);
			[arr addObject:ca];
		}
		if (flagScale) {
			CABasicAnimation* ca;
			ca = [CABasicAnimation animationWithKeyPath:@"transform.scale.x"];
			ca.fromValue = [NSNumber numberWithFloat:scaleStart.x];
			ca.toValue = [NSNumber numberWithFloat:scaleEnd.x];
			_UIAnimation_setAnimation(ca, animation, sl_false);
			[arr addObject:ca];
			ca = [CABasicAnimation animationWithKeyPath:@"transform.scale.y"];
			ca.fromValue = [NSNumber numberWithFloat:scaleStart.y];
			ca.toValue = [NSNumber numberWithFloat:scaleEnd.y];
			_UIAnimation_setAnimation(ca, animation, sl_false);
			[arr addObject:ca];
		}
		if (flagRotate) {
			CABasicAnimation* ca = [CABasicAnimation animationWithKeyPath:@"transform.rotation.z"];
			ca.fromValue = [NSNumber numberWithFloat:rotateStart];
			ca.toValue = [NSNumber numberWithFloat:rotateEnd];
			_UIAnimation_setAnimation(ca, animation, sl_false);
			[arr addObject:ca];
		}
		if (flagAlpha) {
			CABasicAnimation* ca = [CABasicAnimation animationWithKeyPath:@"opacity"];
			ca.fromValue = [NSNumber numberWithFloat:alphaStart];
			ca.toValue = [NSNumber numberWithFloat:alphaEnd];
			_UIAnimation_setAnimation(ca, animation, sl_false);
			[arr addObject:ca];
		}
		
		group.animations = arr;
		
		_UIAnimation_setAnimation(group, animation, sl_true);
		
		_NativeAnimationDelegate* delegate = [[_NativeAnimationDelegate alloc] init];
		delegate->onStop = onStop;
		group.delegate = delegate;
		
		[handle.layer addAnimation:group forKey:@"_slib_native_animation"];
		
	}
	
	sl_bool UIAnimationLoop::_applyNativeAnimation(Animation* animation)
	{
		Ref<View> viewAnimate;
		sl_bool flagTranslate = sl_false;
		Vector2 translateStart(0, 0), translateEnd(0, 0);
		sl_bool flagScale = sl_false;
		Vector2 scaleStart(1, 1), scaleEnd(1, 1);
		sl_bool flagRotate = sl_false;
		sl_real rotateStart = 0, rotateEnd = 0;
		sl_bool flagAlpha = sl_false;
		sl_real alphaStart = 0, alphaEnd = 0;
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
		
		NativeView handle = UIPlatform::getViewHandle(viewAnimate.get());
		if (handle != nil) {
			
			Ref<Animation> _animation = animation;
			Function<void()> onStop = [=]() {
				if (flagTranslate) {
					viewAnimate->setTranslation(translateEnd);
				}
				if (flagScale) {
					viewAnimate->setScale(scaleEnd);
				}
				if (flagRotate) {
					viewAnimate->setRotation(rotateEnd);
				}
				if (flagAlpha) {
					viewAnimate->setAlpha(alphaEnd);
				}
				_stopAnimationFromNative(_animation.get());
			};
			if (UI::isUiThread()) {
				_UIAnimation_applyAnimation(handle, _animation, flagTranslate, translateStart, translateEnd, flagScale, scaleStart, scaleEnd, flagRotate, rotateStart, rotateEnd, flagAlpha, alphaStart, alphaEnd, onStop);
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					_UIAnimation_applyAnimation(handle, _animation, flagTranslate, translateStart, translateEnd, flagScale, scaleStart, scaleEnd, flagRotate, rotateStart, rotateEnd, flagAlpha, alphaStart, alphaEnd, onStop);
				});
			}
			
			_setNativeInstance(animation, viewAnimate.get());
			
			return sl_true;
		}
		return sl_false;
	}
	
	void UIAnimationLoop::_stopNativeAnimation(Animation* animation)
	{
		Ref<View> view = CastRef<View>(_getNativeInstance(animation));
		NativeView handle = UIPlatform::getViewHandle(view.get());
		if (handle != nil) {
			if (UI::isUiThread()) {
				[handle.layer removeAnimationForKey:@"_slib_native_animation"];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle.layer removeAnimationForKey:@"_slib_native_animation"];
				});
			}
		}
	}	
}

@implementation _NativeAnimationDelegate

- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
	onStop();
}

@end
