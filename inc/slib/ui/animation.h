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
	
protected:
	// override
	void _wake();
	
	void _run();
	
private:
	sl_bool m_flagRunning;
	
};

SLIB_UI_NAMESPACE_END

#endif
