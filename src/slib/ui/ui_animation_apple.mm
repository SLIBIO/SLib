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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS) || defined(SLIB_UI_IS_MACOS)

#include "ui_animation.h"

#if defined(SLIB_UI_IS_MACOS)

#include "view_macos.h"
#include <Quartz/Quartz.h>

typedef NSView* NativeView;

#else

#include "view_ios.h"

typedef UIView* NativeView;

#endif

#include "slib/ui/core.h"
#include "slib/math/transform2d.h"

@interface SLIBUINativeAnimation : NSObject<CAAnimationDelegate>
{
	@public __weak CALayer* layer;
	@public slib::Function<void()> onStop;
}
@end

namespace slib
{

	namespace priv
	{
		namespace ui_animation
		{

			static void SetAnimation(CAAnimation* ca, const Ref<Animation>& animation, sl_bool flagGroup)
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
			
			static void ApplyAnimation(NativeView handle, const Ref<Animation>& animation,
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
#ifdef SLIB_UI_IS_MACOS
					ca.fromValue = [NSValue valueWithSize:CGSizeMake(translateStart.x, translateStart.y)];
					ca.toValue = [NSValue valueWithSize:CGSizeMake(translateEnd.x, translateEnd.y)];
#else
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					ca.fromValue = [NSValue valueWithCGSize:CGSizeMake(translateStart.x / f, translateStart.y / f)];
					ca.toValue = [NSValue valueWithCGSize:CGSizeMake(translateEnd.x / f, translateEnd.y / f)];
#endif
					SetAnimation(ca, animation, sl_false);
					[arr addObject:ca];
				}
				if (flagScale) {
					CABasicAnimation* ca;
					ca = [CABasicAnimation animationWithKeyPath:@"transform.scale.x"];
					ca.fromValue = [NSNumber numberWithFloat:scaleStart.x];
					ca.toValue = [NSNumber numberWithFloat:scaleEnd.x];
					SetAnimation(ca, animation, sl_false);
					[arr addObject:ca];
					ca = [CABasicAnimation animationWithKeyPath:@"transform.scale.y"];
					ca.fromValue = [NSNumber numberWithFloat:scaleStart.y];
					ca.toValue = [NSNumber numberWithFloat:scaleEnd.y];
					SetAnimation(ca, animation, sl_false);
					[arr addObject:ca];
				}
				if (flagRotate) {
					CABasicAnimation* ca = [CABasicAnimation animationWithKeyPath:@"transform.rotation.z"];
					ca.fromValue = [NSNumber numberWithFloat:rotateStart];
					ca.toValue = [NSNumber numberWithFloat:rotateEnd];
					SetAnimation(ca, animation, sl_false);
					[arr addObject:ca];
				}
				if (flagAlpha) {
					CABasicAnimation* ca = [CABasicAnimation animationWithKeyPath:@"opacity"];
					ca.fromValue = [NSNumber numberWithFloat:alphaStart];
					ca.toValue = [NSNumber numberWithFloat:alphaEnd];
					SetAnimation(ca, animation, sl_false);
					[arr addObject:ca];
				}
				
				group.animations = arr;
				
				SetAnimation(group, animation, sl_true);
				
				SLIBUINativeAnimation* delegate = [[SLIBUINativeAnimation alloc] init];
				delegate->layer = handle.layer;
				delegate->onStop = onStop;
				group.delegate = delegate;
				
				[handle.layer addAnimation:group forKey:@"_slib_native_animation"];
				
			}

		}
	}

	using namespace priv::ui_animation;

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
			
			CALayer* layer = handle.layer;
			if (layer == nil) {
				return sl_false;
			}
			
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
				ApplyAnimation(handle, _animation, flagTranslate, translateStart, translateEnd, flagScale, scaleStart, scaleEnd, flagRotate, rotateStart, rotateEnd, flagAlpha, alphaStart, alphaEnd, onStop);
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					ApplyAnimation(handle, _animation, flagTranslate, translateStart, translateEnd, flagScale, scaleStart, scaleEnd, flagRotate, rotateStart, rotateEnd, flagAlpha, alphaStart, alphaEnd, onStop);
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

@implementation SLIBUINativeAnimation

- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
	[layer removeAnimationForKey:@"_slib_native_animation"];
	onStop();
}

@end

#endif
