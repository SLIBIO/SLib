/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/scroll_view.h"

#include "view_ios.h"

@interface SLIBScrollViewHandle : UIScrollView<UIScrollViewDelegate>
{
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
	@public UIView* m_contentView;

	sl_bool m_flagPaging;
	sl_ui_len m_pageWidth;
	sl_ui_len m_pageHeight;
	
}

- (void)setContentOffsetFromAPI:(CGPoint)contentOffset animated:(BOOL) animated;

- (void)setPaging:(sl_bool)flagPaging :(sl_ui_len)pageWidth :(sl_ui_len)pageHeight;

@end

@interface SLIBScrollViewHandle_GestureRecognizer : UIGestureRecognizer<UIGestureRecognizerDelegate>
{
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
}
@end

namespace slib
{

	namespace priv
	{
		namespace scroll_view
		{
		
			class ScrollViewHelper : public ScrollView
			{
			public:
				void _applyContentSize(SLIBScrollViewHandle* sv)
				{
					ScrollPoint size = getContentSize();
					if (!(isHorizontalScrolling())) {
						size.x = 0;
					}
					if (!(isVerticalScrolling())) {
						size.y = 0;
					}
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					[sv setContentSize:CGSizeMake((CGFloat)(size.x) / f, (CGFloat)(size.y) / f)];
				}
				
				void _applyContent(SLIBScrollViewHandle* sv)
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
							handle = UIPlatform::getViewHandle(instance.get());
						}
						sv->m_contentView = handle;
						if (handle != nil) {
							[sv addSubview:handle];
						}
					}
					_applyContentSize(sv);
				}
				
				void _applyProperties(SLIBScrollViewHandle* handle)
				{
					handle.backgroundColor = GraphicsPlatform::getUIColorFromColor(getBackgroundColor());
					
					if (isPaging()) {
						[handle setPaging:sl_true :getPageWidth() :getPageHeight()];
					}
					
					handle.showsHorizontalScrollIndicator = isHorizontalScrollBarVisible() ? YES : NO;
					handle.showsVerticalScrollIndicator = isVerticalScrollBarVisible() ? YES : NO;
					
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					[handle setContentOffsetFromAPI:CGPointMake((CGFloat)(getScrollX()) / f, (CGFloat)(getScrollY()) / f) animated:NO];

					_applyContent(handle);
				}
				
				static void _onScroll(iOS_ViewInstance* instance, UIScrollView* sv)
				{
					CGPoint pt= sv.contentOffset;
					Ref<View> _view = instance->getView();
					if (ScrollViewHelper* view = CastInstance<ScrollViewHelper>(_view.get())) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						view->_onScroll_NW((sl_scroll_pos)(pt.x * f), (sl_scroll_pos)(pt.y * f));
					}
				}
				
			};

		}
	}

	using namespace priv::scroll_view;
	
	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBScrollViewHandle* handle = [[SLIBScrollViewHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((ScrollViewHelper*)this)->_applyProperties(handle);
		}
		IOS_VIEW_CREATE_INSTANCE_END
		if (handle != nil && handle->m_viewInstance.isNotNull()) {
			SLIBScrollViewHandle_GestureRecognizer* gesture = [[SLIBScrollViewHandle_GestureRecognizer alloc] init];
			gesture->m_viewInstance = handle->m_viewInstance;
			gesture.cancelsTouchesInView = NO;
			gesture.delegate = gesture;
			[handle addGestureRecognizer:gesture];
		}
		return ret;
	}
	
	void ScrollView::_refreshContentSize_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(ScrollView, _refreshContentSize_NW, this));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBScrollViewHandle class]]) {
			SLIBScrollViewHandle* sv = (SLIBScrollViewHandle*)handle;
			((ScrollViewHelper*)this)->_applyContentSize(sv);
		}
	}
	
	void ScrollView::_setContentView_NW(const Ref<View>& view)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setContentView_NW, this, view));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBScrollViewHandle class]]) {
			SLIBScrollViewHandle* sv = (SLIBScrollViewHandle*)handle;
			((ScrollViewHelper*)this)->_applyContent(sv);
		}
	}
	
	void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _scrollTo_NW, this, x, y, flagAnimate));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			if ([handle isKindOfClass:[SLIBScrollViewHandle class]]) {
				SLIBScrollViewHandle* sv = (SLIBScrollViewHandle*)handle;
				[sv setContentOffsetFromAPI:CGPointMake((CGFloat)(x) / f, (CGFloat)(y) / f) animated:flagAnimate];
			} else if ([handle isKindOfClass:[UIScrollView class]]) {
				UIScrollView* sv = (UIScrollView*)handle;
				[sv setContentOffset:CGPointMake((CGFloat)(x) / f, (CGFloat)(y) / f) animated:flagAnimate];
			}
		}
	}
	
	ScrollPoint ScrollView::_getScrollPosition_NW()
	{
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
			UIScrollView* sv = (UIScrollView*)handle;
			Point ret;
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			CGPoint pt = sv.contentOffset;
			ret.x = (sl_scroll_pos)(pt.x * f);
			ret.y = (sl_scroll_pos)(pt.y * f);
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
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			ScrollPoint ret;
			ret.x = (sl_scroll_pos)((sizeContent.width - sizeFrame.width) * f);
			if (ret.x < 0) {
				ret.x = 0;
			}
			ret.y = (sl_scroll_pos)((sizeContent.height - sizeFrame.height) * f);
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setBackgroundColor_NW, this, color));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
			UIScrollView* sv = (UIScrollView*)handle;
			sv.backgroundColor = GraphicsPlatform::getUIColorFromColor(color);
		}
	}
	
	void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setScrollBarsVisible_NW, this, flagHorizontal, flagVertical));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UIScrollView class]]) {
			UIScrollView* sv = (UIScrollView*)handle;
			sv.showsHorizontalScrollIndicator = flagHorizontal ? YES : NO;
			sv.showsVerticalScrollIndicator = flagVertical ? YES : NO;
		}
	}
	
	void ScrollView::_setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setPaging_NW, this, flagPaging, pageWidth, pageHeight));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBScrollViewHandle class]]) {
			SLIBScrollViewHandle* sv = (SLIBScrollViewHandle*)handle;
			[sv setPaging:flagPaging :pageWidth :pageHeight];
		}
	}
	
}

using namespace slib;
using namespace slib::priv::scroll_view;

@implementation SLIBScrollViewHandle

-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		if (@available(iOS 11.0, *)) {
			self.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
			self.delaysContentTouches = NO;
		}
		[self setDelegate:self];
	}
	return self;
}

- (void)setContentOffset:(CGPoint)contentOffset
{
	[super setContentOffset:contentOffset];
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		ScrollViewHelper::_onScroll(instance.get(), self);
	}
}

- (void)setContentOffsetFromAPI:(CGPoint)contentOffset animated:(BOOL)animated
{
	[super setContentOffset:contentOffset animated:animated];
}

- (void)setPaging: (sl_bool)flagPaging :(sl_ui_len)pageWidth :(sl_ui_len)pageHeight
{
	m_flagPaging = flagPaging;
	CGFloat f = UIPlatform::getGlobalScaleFactor();
	m_pageWidth = (sl_ui_len)(pageWidth / f);
	m_pageHeight = (sl_ui_len)(pageHeight / f);
	self.pagingEnabled = flagPaging && pageWidth <= 0 && pageHeight <= 0 ? YES : NO;
}

- (void)scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset
{
	if (!m_flagPaging) {
		return;
	}
	if (m_pageWidth <= 0 && m_pageHeight <= 0) {
		return;
	}
	CGFloat tx = scrollView.contentOffset.x + velocity.x * 60.0;
	targetContentOffset->x = round(tx / m_pageWidth) * m_pageWidth;
	CGFloat ty = scrollView.contentOffset.y + velocity.y * 60.0;
	targetContentOffset->y = round(ty / m_pageHeight) * 100;
}

@end

@implementation SLIBScrollViewHandle_GestureRecognizer

- (void)touchesBegan:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	self.state = UIGestureRecognizerStateBegan;
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchBegin, touches, theEvent, sl_false);
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchMove, touches, theEvent, sl_false);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	self.state = UIGestureRecognizerStateEnded;
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchEnd, touches, theEvent, sl_false);
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchCancel, touches, theEvent, sl_false);
	}
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
	return YES;
}

@end

#endif
