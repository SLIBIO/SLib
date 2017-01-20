#include "ui_animation.h"

#include "view_ios.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/math/transform2d.h"

SLIB_UI_NAMESPACE_BEGIN

static Matrix3 _UIAnimation_calcTransform(const Vector2& anchor, sl_bool flagTranslate, const Vector2& translate, sl_bool flagScale, const Vector2& scale, sl_bool flagRotate, sl_real rotate)
{
	Matrix3 mat = Matrix3::identity();
	if (flagScale || flagRotate) {
		sl_real ax = anchor.x;
		sl_real ay = anchor.y;
		sl_bool flagAnchor = !(Math::isAlmostZero(ax) && Math::isAlmostZero(ay));
		if (flagAnchor) {
			Transform2::translate(mat, -ax, -ay);
		}
		if (flagRotate) {
			Transform2::rotate(mat, rotate);
		}
		if (flagScale) {
			Transform2::scale(mat, scale.x, scale.y);
		}
		if (flagAnchor) {
			Transform2::translate(mat, ax, ay);
		}
	}
	if (flagTranslate) {
		Transform2::translate(mat, translate.x, translate.y);
	}
	return mat;
}

static void _UIAnimation_applyAnimation(UIView* handle, const Ref<Animation>& animation, sl_bool flagTransform, const CGAffineTransform& ts, const CGAffineTransform& te, sl_bool flagAlpha, sl_real alphaStart, sl_real alphaEnd, Function<void()> onStop)
{
	UIViewAnimationOptions options = 0;
	switch (animation->getAnimationCurve()) {
		case AnimationCurve::EaseIn:
			options |= UIViewAnimationOptionCurveEaseIn;
			break;
		case AnimationCurve::EaseOut:
			options |= UIViewAnimationOptionCurveEaseOut;
			break;
		case AnimationCurve::EaseInOut:
			options |= UIViewAnimationOptionCurveEaseInOut;
			break;
		default:
			options |= UIViewAnimationOptionCurveLinear;
			break;
	}
	
	sl_int32 nRepeat = animation->getRepeatCount();
	if (nRepeat < 0) {
		options |= UIViewAnimationOptionRepeat;
	}
	if (animation->isAutoReverse()) {
		options |= (UIViewAnimationOptionRepeat | UIViewAnimationOptionAutoreverse);
	}

	if (flagTransform) {
		handle.transform = ts;
	}
	if (flagAlpha) {
		handle.alpha = alphaStart;
	}
	
	[UIView animateWithDuration:animation->getDuration()
						  delay:animation->getStartDelay()
						options:options
					 animations:^() {
						 if (flagTransform) {
							 handle.transform = te;
						 }
						 if (flagAlpha) {
							 handle.alpha = alphaEnd;
						 }
					 }
					 completion:^(BOOL finished) {
						 onStop();
					 }];
	
}

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
	
	UIView* handle = UIPlatform::getViewHandle(viewAnimate.get());
	if (handle != nil) {
		sl_bool flagTransform = flagTranslate || flagScale || flagRotate;
		Matrix3 matStart;
		Matrix3 matEnd;
		
		CGAffineTransform ts, te;
		if (flagTransform) {
			matStart = _UIAnimation_calcTransform(viewAnimate->getAnchorOffset(), flagTranslate, translateStart, flagScale, scaleStart, flagRotate, rotateStart);
			matEnd = _UIAnimation_calcTransform(viewAnimate->getAnchorOffset(), flagTranslate, translateEnd, flagScale, scaleEnd, flagRotate, rotateEnd);
			GraphicsPlatform::getCGAffineTransform(ts, matStart, UIPlatform::getGlobalScaleFactor(), 0, 0);
			GraphicsPlatform::getCGAffineTransform(te, matEnd, UIPlatform::getGlobalScaleFactor(), 0, 0);
		}
		
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
		if (UI::isUiThread()) {
			_UIAnimation_applyAnimation(handle, _animation, flagTransform, ts, te, flagAlpha, alphaStart, alphaEnd, onStop);
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				_UIAnimation_applyAnimation(handle, _animation, flagTransform, ts, te, flagAlpha, alphaStart, alphaEnd, onStop);
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
	UIView* handle = UIPlatform::getViewHandle(view.get());
	if (handle != nil) {
		if (UI::isUiThread()) {
			[handle.layer removeAllAnimations];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle.layer removeAllAnimations];
			});
		}
	}
}

SLIB_UI_NAMESPACE_END
