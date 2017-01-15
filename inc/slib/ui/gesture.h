#ifndef CHECKHEADER_SLIB_UI_GESTURE
#define CHECKHEADER_SLIB_UI_GESTURE

#include "definition.h"

#include "motion_tracker.h"

SLIB_UI_NAMESPACE_BEGIN

enum class GestureType
{
	SwipeLeft = 0,
	SwipeRight = 1,
	SwipeUp = 2,
	SwipeDown = 3,
	
	Count = 4
};

class View;
class UIEvent;

class GestureRecognizer;

class GestureDetector : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	GestureDetector(const Ref<View>& view);
	
public:
	void enable(GestureType type);
	
	void enableNative();

	void processEvent(UIEvent* ev);
	
private:
	static sl_bool _enableNative(const Ref<View>& view, GestureType type);
	
protected:
	WeakRef<View> m_view;
	MotionTracker m_tracker;
	Ref<GestureRecognizer> m_recognizers[(int)(GestureType::Count)];
	
	friend class GestureRecognizer;
};

SLIB_UI_NAMESPACE_END

#endif
