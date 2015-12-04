#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/scroll_view.h"

#include "view_osx.h"

@interface _Slib_OSX_ScrollView : NSScrollView {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN
class _ScrollView : public ScrollView
{
public:
	void __applyContent(NSScrollView* sv)
	{
		Ref<View> viewContent = m_viewContent;
		NSView* handle = nil;
		if (viewContent.isNotNull()) {
			Ref<ViewInstance> instance = viewContent->attachToNewInstance(Ref<ViewInstance>::null());
			if (instance.isNotNull()) {
				handle = UIPlatform::getViewHandle(instance.get());
			}
		}
		[sv setDocumentView:handle];
	}
	
	void __applyProperties(NSScrollView* handle)
	{
		[handle setBorderType:(m_flagBorder?NSBezelBorder:NSNoBorder)];
		if (m_backgroundColor.getAlpha() == 0) {
			handle.drawsBackground = FALSE;
		} else {
			handle.drawsBackground = TRUE;
			handle.backgroundColor = UIPlatform::getNSColorFromColor(m_backgroundColor);
		}
		__applyContent(handle);
	}
};

Ref<ViewInstance> ScrollView::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_ScrollView* handle = [[_Slib_OSX_ScrollView alloc] initWithFrame:frame];
	if (handle != nil) {
		((_ScrollView*)this)->__applyProperties(handle);
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

void ScrollView::onResize()
{
	View::onResize();
}

void ScrollView::onResizeChild(View* child)
{
}

void ScrollView::_setBorder(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		[sv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
	}
}

void ScrollView::_setBackgroundColor(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		if (color.getAlpha() == 0) {
			sv.drawsBackground = FALSE;
		} else {
			sv.drawsBackground = TRUE;
			sv.backgroundColor = UIPlatform::getNSColorFromColor(color);
		}
	}
}

void ScrollView::_setContentView(const Ref<View>& view)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		((_ScrollView*)this)->__applyContent(sv);
	}
}

void ScrollView::scrollTo(sl_real x, sl_real y)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		NSClipView* clip = [sv contentView];
		if (clip != nil) {
			NSPoint pt;
			pt.x = x;
			pt.y = y;
			[clip scrollToPoint:pt];
			[sv reflectScrolledClipView:clip];
		}
	}
}

Point ScrollView::getScrollPosition()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		NSClipView* clip = [sv contentView];
		if (clip != nil) {
			NSPoint pt=[clip bounds].origin;
			Point ret;
			ret.x = (sl_real)(pt.x);
			ret.y = (sl_real)(pt.y);
			return ret;
		}
	}
	return Point::zero();
}

Size ScrollView::getScrollRange()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		NSClipView* clip = [sv contentView];
		NSView* doc = [sv documentView];
		if (clip != nil && doc != nil) {
			Size ret;
			NSRect rcDoc = [doc frame];
			NSRect rcClip = [clip bounds];
			ret.width = (sl_real)(NSMaxX(rcDoc) - NSWidth(rcClip));
			ret.height = (sl_real)(NSMaxY(rcDoc) - NSHeight(rcClip));
			if (ret.width < 0) {
				ret.width = 0;
			}
			if (ret.height < 0) {
				ret.height = 0;
			}
			return ret;
		}
	}
	return Size::zero();
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_ScrollView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setHasVerticalScroller:TRUE];
		[self setHasHorizontalScroller:TRUE];
	}
	return self;
}
@end

#endif
