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
	
public:
	// override
	sl_bool isValid();
	
	// override
	void setFocus();
	
	// override
	void invalidate();
	
	// override
	void invalidate(const UIRect& rect);
	
	// override
	UIRect getFrame();
	
	// override
	void setFrame(const UIRect& frame);
	
	// override
	void setTransform(const Matrix3& transform);
	
	// override
	void setVisible(sl_bool flag);
	
	// override
	void setEnabled(sl_bool flag);
	
	// override
	void setOpaque(sl_bool flag);
	
	// override
	void setAlpha(sl_real alpha);
	
	// override
	UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen);
	
	// override
	UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView);
	
	// override
	void addChildInstance(const Ref<ViewInstance>& instance);
	
	// override
	void removeChildInstance(const Ref<ViewInstance>& instance);
	
	// override
	void bringToFront();
	
public:
	void onDraw(CGRect rectDirty);
	
	sl_bool onEventTouch(UIAction action, NSSet* touches, ::UIEvent* event);
	
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
	CGFloat f = UIPlatform::getGlobalScaleFactor();	\
	UIRect _frame = getFrame(); \
	frame.origin.x = (CGFloat)(_frame.left) / f; \
	frame.origin.y = (CGFloat)(_frame.top) / f; \
	frame.size.width = (CGFloat)(_frame.getWidth()) / f; \
	frame.size.height = (CGFloat)(_frame.getHeight()) / f; \

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
		sl_bool flagStopPropagation = instance->onEventTouch(slib::UIAction::TouchBegin, touches, theEvent); \
		if (flagStopPropagation) { \
			return; \
		} \
	} \
	[super touchesBegan:touches withEvent:theEvent];\
} \
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagStopPropagation = instance->onEventTouch(slib::UIAction::TouchMove, touches, theEvent); \
		if (flagStopPropagation) { \
			return; \
		} \
	} \
	[super touchesMoved:touches withEvent:theEvent];\
} \
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagStopPropagation = instance->onEventTouch(slib::UIAction::TouchEnd, touches, theEvent); \
		if (flagStopPropagation) { \
			return; \
		} \
	} \
	[super touchesEnded:touches withEvent:theEvent];\
} \
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		sl_bool flagStopPropagation = instance->onEventTouch(slib::UIAction::TouchCancel, touches, theEvent); \
		if (flagStopPropagation) { \
			return; \
		} \
	} \
	[super touchesCancelled:touches withEvent:theEvent];\
}

#endif

#endif
