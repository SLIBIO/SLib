#ifndef CHECKHEADER_SLIB_TRANSITION
#define CHECKHEADER_SLIB_TRANSITION

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

enum class TransitionType
{
	None = 0,
	Push = 1,
	Slide = 2,
	Cover = 3,
	Zoom = 4,
	Fade = 5,
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
	
public:
	static Ref<Animation> start(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Ref<Runnable>& onStop);

	static Ref<Animation> createAnimation(const Ref<View>& view, const Transition& transition, UIPageAction pageAction, const Ref<Runnable>& onStop);
	
};

SLIB_UI_NAMESPACE_END

#endif
