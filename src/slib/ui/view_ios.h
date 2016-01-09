#ifndef CHECKHEADER_SLIB_UI_VIEW_IOS
#define CHECKHEADER_SLIB_UI_VIEW_IOS

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN
class iOS_ViewInstance : public ViewInstance
{
public:
	iOS_ViewInstance();
	~iOS_ViewInstance();
	
public:
	static Ref<iOS_ViewInstance> create(UIView* handle, sl_bool flagFreeOnRelease);
	static Ref<iOS_ViewInstance> create(UIView* handle, UIView* parent, View* view);
	
public:
	void release();
	static void freeHandle(UIView* handle);

	UIView* getHandle();
	
	void onDraw(CGRect rectDirty);
	sl_bool onEventTouch(UIEventAction action, NSSet* touches, ::UIEvent* event);
	
protected:
	UIView* m_handle;
	sl_bool m_flagFreeOnRelease;
	
};
SLIB_UI_NAMESPACE_END

@interface Slib_iOS_ViewHandle : UIView {

	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;

}
@end


#define IOS_VIEW_CREATE_INSTANCE_BEGIN \
	Ref<iOS_ViewInstance> ret; \
	UIView* parent = UIPlatform::getViewHandle(_parent); \
	CGRect frame; \
	Rectangle _frame = getFrame(); \
	frame.origin.x = _frame.left; \
	frame.origin.y = _frame.top; \
	frame.size.width = _frame.getWidth(); \
	frame.size.height = _frame.getHeight(); \

#define IOS_VIEW_CREATE_INSTANCE_END \
	if (handle != nil) { \
		ret = iOS_ViewInstance::create(handle, parent, this); \
		if (ret.isNotNull()) { \
			handle->m_viewInstance = ret; \
		} \
	}

#define IOS_VIEW_EVENTS \
- (BOOL)canBecomeFirstResponder \
{ \
	return TRUE; \
} \
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagPreventDefault = instance->onEventTouch(slib::actionTouchBegin, touches, theEvent); \
		if (flagPreventDefault) { \
			return; \
		} \
	} \
	[super touchesBegan:touches withEvent:theEvent];\
} \
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagPreventDefault = instance->onEventTouch(slib::actionTouchMove, touches, theEvent); \
		if (flagPreventDefault) { \
			return; \
		} \
	} \
	[super touchesMoved:touches withEvent:theEvent];\
} \
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagPreventDefault = instance->onEventTouch(slib::actionTouchEnd, touches, theEvent); \
		if (flagPreventDefault) { \
			return; \
		} \
	} \
	[super touchesEnded:touches withEvent:theEvent];\
} \
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagPreventDefault = instance->onEventTouch(slib::actionTouchCancel, touches, theEvent); \
		if (flagPreventDefault) { \
			return; \
		} \
	} \
	[super touchesCancelled:touches withEvent:theEvent];\
}

#endif

#endif
