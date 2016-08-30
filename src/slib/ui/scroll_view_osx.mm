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
				handle = UIPlatform::getViewHandle(instance.ptr);
			}
		}
		[sv setDocumentView:handle];
	}
	
	void __applyProperties(NSScrollView* handle)
	{
		[handle setBorderType:(isBorder()?NSBezelBorder:NSNoBorder)];
		Color backgroundColor = getBackgroundColor();
		if (backgroundColor.a == 0) {
			handle.drawsBackground = FALSE;
		} else {
			handle.drawsBackground = TRUE;
			handle.backgroundColor = UIPlatform::getNSColorFromColor(backgroundColor);
		}
		__applyContent(handle);
	}
	
	static void __onScroll(OSX_ViewInstance* instance, NSScrollView* sv)
	{
		NSClipView* clip = [sv contentView];
		if (clip != nil) {
			NSPoint pt=[clip bounds].origin;
			Ref<View> _view = instance->getView();
			if (ScrollView::checkInstance(_view.ptr)) {
				_ScrollView* view = (_ScrollView*)(_view.ptr);
				view->_onScroll_NW((sl_real)(pt.x), (sl_real)(pt.y));
			}
		}
	}
};

Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_ScrollView* handle = [[_Slib_OSX_ScrollView alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setHasVerticalScroller:(m_flagVerticalScroll?YES:NO)];
		[handle setHasHorizontalScroller:(m_flagHorizontalScroll?YES:NO)];
		((_ScrollView*)this)->__applyProperties(handle);
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

void ScrollView::_refreshContentSize_NW()
{
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		((_ScrollView*)this)->__applyContent(sv);
	}
}

void ScrollView::_scrollTo_NW(sl_real x, sl_real y)
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

Point ScrollView::_getScrollPosition_NW()
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

Size ScrollView::_getScrollRange_NW()
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
			ret.x = (sl_real)(NSMaxX(rcDoc) - NSWidth(rcClip));
			ret.y = (sl_real)(NSMaxY(rcDoc) - NSHeight(rcClip));
			if (ret.x < 0) {
				ret.x = 0;
			}
			if (ret.y < 0) {
				ret.y = 0;
			}
			return ret;
		}
	}
	return Size::zero();
}

void ScrollView::_setBorder_NW(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		[sv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
	}
}

void ScrollView::_setBackgroundColor_NW(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
		NSScrollView* sv = (NSScrollView*)handle;
		if (color.a == 0) {
			sv.drawsBackground = FALSE;
		} else {
			sv.drawsBackground = TRUE;
			sv.backgroundColor = UIPlatform::getNSColorFromColor(color);
		}
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_ScrollView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		NSView *contentView = [self contentView];
		if (contentView != nil) {
			[contentView setPostsBoundsChangedNotifications:YES];
			[[NSNotificationCenter defaultCenter] addObserver:self
													 selector:@selector(boundDidChange:)
														 name:NSViewBoundsDidChangeNotification
													   object:contentView];
		}
	}
	return self;
}

- (void)boundDidChange:(NSNotification *)notification {
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_ScrollView::__onScroll(instance.ptr, self);
	}
}
@end

#endif
