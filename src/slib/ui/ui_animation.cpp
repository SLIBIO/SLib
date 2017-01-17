#include "../../../inc/slib/ui/animation.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(UIAnimationLoop, AnimationLoop)

UIAnimationLoop::UIAnimationLoop()
{
	m_flagRunning = sl_false;
}

UIAnimationLoop::~UIAnimationLoop()
{
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

SLIB_UI_NAMESPACE_END
