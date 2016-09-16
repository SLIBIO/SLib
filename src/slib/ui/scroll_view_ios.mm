#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/scroll_view.h"

#include "view_ios.h"

@interface _Slib_iOS_ScrollView : UIScrollView {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
	@public UIView* m_contentView;
	
}

- (void)setContentOffsetFromAPI:(CGPoint)contentOffset;

@end

SLIB_UI_NAMESPACE_BEGIN

class _ScrollView : public ScrollView
{
public:
	void __applyContentSize(_Slib_iOS_ScrollView* sv)
	{
		ScrollPoint size = getContentSize();
		if (!m_flagHorizontalScroll) {
			size.x = 0;
		}
		if (!m_flagVerticalScroll) {
			size.y = 0;
		}
		[sv setContentSize:CGSizeMake((CGFloat)(size.x), (CGFloat)(size.y))];
	}
	
	void __applyContent(_Slib_iOS_ScrollView* sv)
	{
		if (sv->m_contentView != nil) {
			[sv->m_contentView removeFromSuperview];
			sv->m_contentView = nil;
		}
		Ref<View> viewContent = m_viewContent;
		UIView* handle = nil;
		if (viewContent.isNotNull()) {
			Ref<ViewInstance> instance = viewContent->attachToNewInstance(Ref<ViewInstance>::null());
			if (instance.isNotNull()) {
				handle = UIPlatform::getViewHandle(instance.ptr);
			}
			sv->m_contentView = handle;
			if (handle != nil) {
				[sv addSubview:handle];
			}
		}
		__applyContentSize(sv);
	}
	
	void __applyProperties(_Slib_iOS_ScrollView* handle)
	{
		handle.backgroundColor = UIPlatform::getUIColorFromColor(getBackgroundColor());
		__applyContent(handle);
	}
	
	static void __onScroll(iOS_ViewInstance* instance, UIScrollView* sv)
	{
		CGPoint pt= sv.contentOffset;
		Ref<View> _view = instance->getView();
		if (ScrollView::checkInstance(_view.ptr)) {
			_ScrollView* view = (_ScrollView*)(_view.ptr);
			view->_onScroll_NW((sl_scroll_pos)(pt.x), (sl_scroll_pos)(pt.y));
		}
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
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_ScrollView class]]) {
		_Slib_iOS_ScrollView* sv = (_Slib_iOS_ScrollView*)handle;
		((_ScrollView*)this)->__applyContentSize(sv);
	}
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_ScrollView class]]) {
		_Slib_iOS_ScrollView* sv = (_Slib_iOS_ScrollView*)handle;
		((_ScrollView*)this)->__applyContent(sv);
	}
}

void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[_Slib_iOS_ScrollView class]]) {
			_Slib_iOS_ScrollView* sv = (_Slib_iOS_ScrollView*)handle;
			[sv setContentOffsetFromAPI:CGPointMake((CGFloat)x, (CGFloat)y)];
		} else if ([handle isKindOfClass:[UIScrollView class]]) {
			UIScrollView* sv = (UIScrollView*)handle;
			[sv setContentOffset:CGPointMake((CGFloat)x, (CGFloat)y)];
		}
	}
}

ScrollPoint ScrollView::_getScrollPosition_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		Point ret;
		CGPoint pt = sv.contentOffset;
		ret.x = (sl_scroll_pos)(pt.x);
		ret.y = (sl_scroll_pos)(pt.y);
		return ret;
	}
	return ScrollPoint::zero();
}

ScrollPoint ScrollView::_getScrollRange_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
		UIScrollView* sv = (UIScrollView*)handle;
		CGSize sizeContent = sv.contentSize;
		CGSize sizeFrame = sv.bounds.size;
		ScrollPoint ret;
		ret.x = (sl_scroll_pos)(sizeContent.width - sizeFrame.width);
		if (ret.x < 0) {
			ret.x = 0;
		}
		ret.y = (sl_scroll_pos)(sizeContent.height - sizeFrame.height);
		if (ret.y < 0) {
			ret.y = 0;
		}
		return ret;
	}
	return ScrollPoint::zero();
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
	return self;
}

- (void)setContentOffset:(CGPoint)contentOffset
{
	[super setContentOffset:contentOffset];
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_ScrollView::__onScroll(instance.ptr, self);
	}
}

- (void)setContentOffsetFromAPI:(CGPoint)contentOffset
{
	[super setContentOffset:contentOffset];
}

@end

#endif
