#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/scroll_view.h"

#include "ui_view_ios.h"

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
				handle = UIPlatform::getViewHandle(instance.get());
			}
			viewContent->setParent(this);
			Size sizeContent = viewContent->getSize();
			[sv setContentSize:CGSizeMake(sizeContent.width, sizeContent.height)];
			if (handle != nil) {
				[sv addSubview:handle];
			}
		}
	}
	
	void __applyProperties(UIScrollView* handle)
	{
		handle.backgroundColor = UIPlatform::getUIColorFromColor(m_backgroundColor);
		__applyContent(handle);
	}
};

Ref<ViewInstance> ScrollView::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_ScrollView* handle = [[_Slib_iOS_ScrollView alloc] initWithFrame:frame];
	if (handle != nil) {
		((_ScrollView*)this)->__applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

void ScrollView::onResize()
{
	View::onResize();
}

void ScrollView::onResizeChild(View* child)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		Ref<View> viewContent = m_viewContent;
		if (viewContent.isNotNull()) {
			Size sizeContent = viewContent->getSize();
			[sv setContentSize:CGSizeMake(sizeContent.width, sizeContent.height)];
		}
	}
}

void ScrollView::_setBorder(sl_bool flag)
{
}

void ScrollView::_setBackgroundColor(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		sv.backgroundColor = UIPlatform::getUIColorFromColor(color);
	}
}

void ScrollView::_setContentView(const Ref<View>& view)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		((_ScrollView*)this)->__applyContent(sv);
	}
}

void ScrollView::scrollTo(sl_real x, sl_real y)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		[sv setContentOffset:CGPointMake(x, y)];
	}
}

Point ScrollView::getScrollPosition()
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

Size ScrollView::getScrollRange()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		CGSize sizeContent = sv.contentSize;
		CGSize sizeFrame = sv.bounds.size;
		Size ret;
		ret.width = sizeContent.width - sizeFrame.width;
		if (ret.width < 0) {
			ret.width = 0;
		}
		ret.height = sizeContent.height - sizeFrame.height;
		if (ret.height < 0) {
			ret.height = 0;
		}
		return ret;
	}
	return Size::zero();
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
