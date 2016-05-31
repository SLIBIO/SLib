#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/scroll_view.h"

#include "view_ios.h"

@interface _Slib_iOS_ScrollView : UIScrollView {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _ScrollView : public ScrollView
{
public:
	void __applyContent(UIScrollView* sv)
	{
		Ref<View> viewContent = m_viewContent;
		UIView* handle = nil;
		if (viewContent.isNotNull()) {
			Ref<ViewInstance> instance = viewContent->attachToNewInstance(Ref<ViewInstance>::null());
			if (instance.isNotNull()) {
				handle = UIPlatform::getViewHandle(instance.ptr);
			}
			viewContent->setParent(this);
			Size sizeContent = viewContent->getSize();
			[sv setContentSize:CGSizeMake(sizeContent.x, sizeContent.y)];
			if (handle != nil) {
				[sv addSubview:handle];
			}
		}
	}
	
	void __applyProperties(UIScrollView* handle)
	{
		handle.backgroundColor = UIPlatform::getUIColorFromColor(getBackgroundColor());
		__applyContent(handle);
	}
};

Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_ScrollView* handle = [[_Slib_iOS_ScrollView alloc] initWithFrame:frame];
	if (handle != nil) {
		((_ScrollView*)this)->__applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

void ScrollView::_refreshContentSize_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		Ref<View> viewContent = m_viewContent;
		if (viewContent.isNotNull()) {
			Size sizeContent = viewContent->getSize();
			[sv setContentSize:CGSizeMake(sizeContent.x, sizeContent.y)];
		}
	}
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		((_ScrollView*)this)->__applyContent(sv);
	}
}

void ScrollView::_scrollTo_NW(sl_real x, sl_real y)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		[sv setContentOffset:CGPointMake(x, y)];
	}
}

Point ScrollView::_getScrollPosition_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		Point ret;
		CGPoint pt = sv.contentOffset;
		ret.x = (sl_real)(pt.x);
		ret.y = (sl_real)(pt.y);
		return ret;
	}
	return Point::zero();
}

Size ScrollView::_getScrollRange_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		CGSize sizeContent = sv.contentSize;
		CGSize sizeFrame = sv.bounds.size;
		Size ret;
		ret.x = sizeContent.width - sizeFrame.width;
		if (ret.x < 0) {
			ret.x = 0;
		}
		ret.y = sizeContent.height - sizeFrame.height;
		if (ret.y < 0) {
			ret.y = 0;
		}
		return ret;
	}
	return Size::zero();
}

void ScrollView::_setBorder_NW(sl_bool flag)
{
}

void ScrollView::_setBackgroundColor_NW(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		sv.backgroundColor = UIPlatform::getUIColorFromColor(color);
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_ScrollView
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}
@end

#endif
