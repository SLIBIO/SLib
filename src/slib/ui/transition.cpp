#include "../../../inc/slib/ui/transition.h"

SLIB_UI_NAMESPACE_BEGIN

Transition::Transition()
{
	type = TransitionType::Default;
	direction = TransitionDirection::Default;
	duration = 0;
	curve = AnimationCurve::Default;
}

Transition::Transition(const Transition& other) = default;

Transition& Transition::operator=(const Transition& other) = default;

Ref<Animation> Transition::start(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Ref<Runnable>& onStop)
{
	if (view.isNull()) {
		return Ref<Animation>::null();
	}
	Ref<Animation> animation = createAnimation(view, transition, pageAction, onStop);
	if (animation.isNotNull()) {
		animation->start();
		return animation;
	} else {
		if (onStop.isNotNull()) {
			onStop->run();
		}
		return Ref<Animation>::null();
	}
}

Ref<Animation> Transition::createAnimation(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Ref<Runnable>& onStop)
{
	if (view.isNull()) {
		return Ref<Animation>::null();
	}
	
	TransitionType type = transition.type;
	if (type == TransitionType::Default || type == TransitionType::None) {
		return Ref<Animation>::null();
	}
	
	float duration = transition.duration;
	if (duration < SLIB_EPSILON) {
		return Ref<Animation>::null();
	}
	
	TransitionDirection direction = transition.direction;
	AnimationCurve curve = transition.curve;
	
	Ref<Animation> animation = Animation::create(duration);
	if (animation.isNotNull()) {
		switch (type) {
			case TransitionType::None:
			case TransitionType::Default:
				break;
			case TransitionType::Push:
			case TransitionType::Slide:
			case TransitionType::Cover:
				{
					sl_real factor;
					if (type == TransitionType::Push) {
						factor = 1;
					} else if (type == TransitionType::Slide) {
						if (pageAction == UIPageAction::Push || pageAction == UIPageAction::Pop) {
							factor = 1;
						} else {
							factor = 0.5f;
						}
					} else if (type == TransitionType::Cover) {
						if (pageAction == UIPageAction::Push || pageAction == UIPageAction::Pop) {
							factor = 1;
						} else {
							factor = 0;
						}
					} else {
						factor = 0;
					}
					sl_real startFactor;
					sl_real endFactor;
					if (pageAction == UIPageAction::Push || pageAction == UIPageAction::Resume) {
						startFactor = factor;
						endFactor = 0;
					} else {
						startFactor = 0;
						endFactor = -factor;
					}
					if (direction == TransitionDirection::FromLeftToRight) {
						direction = TransitionDirection::FromRightToLeft;
						startFactor = -startFactor;
						endFactor = -endFactor;
					} else if (direction == TransitionDirection::FromTopToBottom) {
						direction = TransitionDirection::FromBottomToTop;
						startFactor = -startFactor;
						endFactor = -endFactor;
					}
					if (direction == TransitionDirection::FromRightToLeft) {
						sl_real width = (sl_real)(view->getWidth());
						view->setTranslateAnimation(animation, Vector2(width * startFactor, 0), Vector2(width * endFactor, 0), sl_false);
					} else {
						sl_real height = (sl_real)(view->getHeight());
						view->setTranslateAnimation(animation, Vector2(0, height * startFactor), Vector2(0, height * endFactor), sl_false);
					}
				}
				break;
			case TransitionType::Zoom:
				if (pageAction == UIPageAction::Push || pageAction == UIPageAction::Resume) {
					view->setScaleAnimation(animation, 0.5f, 1.0f, sl_false);
				} else {
					view->setScaleAnimation(animation, 1.0f, 0.5f, sl_false);
				}
				break;
			case TransitionType::Fade:
				if (pageAction == UIPageAction::Push) {
					view->setAlphaAnimation(animation, 0.01f, 1.0f, sl_false);
				} else if (pageAction == UIPageAction::Pop){
					view->setAlphaAnimation(animation, 1.0f, 0.01f, sl_false);
				}
				break;
		}
		animation->setAnimationCurve(curve);
		animation->setOnStop(onStop);
		return animation;
	}
	
	return Ref<Animation>::null();
}

SLIB_UI_NAMESPACE_END

