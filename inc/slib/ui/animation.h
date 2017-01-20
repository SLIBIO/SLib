#ifndef CHECKHEADER_SLIB_UI_ANIMATION
#define CHECKHEADER_SLIB_UI_ANIMATION

#include "definition.h"

#include "../core/animation.h"

SLIB_UI_NAMESPACE_BEGIN

class UIAnimationLoop : public AnimationLoop
{
	SLIB_DECLARE_OBJECT
	
private:
	UIAnimationLoop();
	
	~UIAnimationLoop();
	
public:
	static Ref<UIAnimationLoop> getInstance();

public:
	// override
	sl_bool startNativeAnimation(Animation* animation);
	
	// override
	void stopNativeAnimation(Animation* animation);

protected:
	// override
	void _wake();
	
	void _run();
	
	sl_bool _applyNativeAnimation(Animation* animation);
	
	void _stopNativeAnimation(Animation* animation);
	
private:
	sl_bool m_flagRunning;
	
};

SLIB_UI_NAMESPACE_END

#endif
