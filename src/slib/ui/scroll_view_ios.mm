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

namespace slib
{
	namespace priv
	{
		namespace scroll_view
		{
			class ScrollViewInstance;
		}
	}
}

@interface SLIBScrollViewHandle : UIScrollView<UIScrollViewDelegate>
{
	
	@public slib::WeakRef<slib::priv::scroll_view::ScrollViewInstance> m_viewInstance;
	
	@public UIView* m_contentView;

	sl_bool m_flagPaging;
	sl_ui_len m_pageWidth;
	sl_ui_len m_pageHeight;
	
}

- (void)setContentOffsetFromAPI:(CGPoint)contentOffset animated:(BOOL)animated;

- (void)setPaging:(sl_bool)flagPaging :(sl_ui_len)pageWidth :(sl_ui_len)pageHeight;

@end

@interface SLIBScrollViewHandle_GestureRecognizer : UIGestureRecognizer<UIGestureRecognizerDelegate>
{
	@public slib::WeakRef<slib::priv::scroll_view::ScrollViewInstance> m_viewInstance;
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
				void applyContentSize(SLIBScrollViewHandle* sv)
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
				
				void applyContent(SLIBScrollViewHandle* sv)
				{
					if (sv->m_contentView != nil) {
						[sv->m_contentView removeFromSuperview];
						sv->m_contentView = nil;
					}
					Ref<View> viewContent = m_viewContent;
					UIView* handle = nil;
					if (viewContent.isNotNull()) {
						Ref<ViewInstance> instance = viewContent->attachToNewInstance(sl_null);
						if (instance.isNotNull()) {
							handle = UIPlatform::getViewHandle(instance.get());
						}
						sv->m_contentView = handle;
						if (handle != nil) {
							[sv addSubview:handle];
						}
					}
					applyContentSize(sv);
				}
				
				using ScrollView::_onScroll_NW;
				
			};
			
			class ScrollViewInstance : public iOS_ViewInstance, public IScrollViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBScrollViewHandle* getHandle()
				{
					return (SLIBScrollViewHandle*)m_handle;
				}
				
				Ref<ScrollViewHelper> getHelper()
				{
					return CastRef<ScrollViewHelper>(getView());
				}
				
				void refreshContentSize(ScrollView* view) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						static_cast<ScrollViewHelper*>(view)->applyContentSize(handle);
					}
				}
				
				void setContentView(ScrollView* view, const Ref<View>& content) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						static_cast<ScrollViewHelper*>(view)->applyContent(handle);
					}
				}
				
				sl_bool getScrollPosition(ScrollView* view, ScrollPoint& _out) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						CGPoint pt = handle.contentOffset;
						_out.x = (sl_scroll_pos)(pt.x * f);
						_out.y = (sl_scroll_pos)(pt.y * f);
						return sl_true;
					}
					return sl_false;
				}
				
				sl_bool getScrollRange(ScrollView* view, ScrollPoint& _out) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						CGSize sizeContent = handle.contentSize;
						CGSize sizeFrame = handle.bounds.size;
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						_out.x = (sl_scroll_pos)((sizeContent.width - sizeFrame.width) * f);
						if (_out.x < 0) {
							_out.x = 0;
						}
						_out.y = (sl_scroll_pos)((sizeContent.height - sizeFrame.height) * f);
						if (_out.y < 0) {
							_out.y = 0;
						}
						return sl_true;
					}
					return sl_false;
				}
				
				void scrollTo(View* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						[handle setContentOffsetFromAPI:CGPointMake((CGFloat)(x) / f, (CGFloat)(y) / f) animated:flagAnimate];
					}
				}
				
				void setPaging(View* view, sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle setPaging:flagPaging :pageWidth :pageHeight];
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						handle.backgroundColor = GraphicsPlatform::getUIColorFromColor(color);
					}
				}
				
				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle != nil) {
						handle.showsHorizontalScrollIndicator = flagHorizontal ? YES : NO;
						handle.showsVerticalScrollIndicator = flagVertical ? YES : NO;
					}
				}
				
				void apply(ScrollView* view)
				{
					SLIBScrollViewHandle* handle = getHandle();
					if (handle == nil) {
						return;
					}
					
					handle.backgroundColor = GraphicsPlatform::getUIColorFromColor(view->getBackgroundColor());
					if (view->isPaging()) {
						[handle setPaging:sl_true :view->getPageWidth() :view->getPageHeight()];
					}
					handle.showsHorizontalScrollIndicator = view->isHorizontalScrollBarVisible() ? YES : NO;
					handle.showsVerticalScrollIndicator = view->isVerticalScrollBarVisible() ? YES : NO;
					
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					[handle setContentOffsetFromAPI:CGPointMake((CGFloat)(view->getScrollX()) / f, (CGFloat)(view->getScrollY()) / f) animated:NO];
					
					static_cast<ScrollViewHelper*>(view)->applyContent(handle);
					
					SLIBScrollViewHandle_GestureRecognizer* gesture = [[SLIBScrollViewHandle_GestureRecognizer alloc] init];
					gesture->m_viewInstance = this;
					[handle addGestureRecognizer:gesture];
				}
				
				void onScroll(UIScrollView* sv)
				{
					CGPoint pt= sv.contentOffset;
					Ref<ScrollViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						CGFloat f = UIPlatform::getGlobalScaleFactor();
						helper->_onScroll_NW((sl_scroll_pos)(pt.x * f), (sl_scroll_pos)(pt.y * f));
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(ScrollViewInstance, iOS_ViewInstance)

		}
	}

	using namespace priv::scroll_view;
	
	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
	{
		Ref<ScrollViewInstance> ret = iOS_ViewInstance::create<ScrollViewInstance, SLIBScrollViewHandle>(this, parent);
		if (ret.isNotNull()) {
			ret->apply(this);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IScrollViewInstance> ScrollView::getScrollViewInstance()
	{
		return CastRef<ScrollViewInstance>(getViewInstance());
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
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onScroll(self);
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

-(id)init
{
	self = [super init];
	if (self != nil) {
		self.cancelsTouchesInView = NO;
		self.delegate = self;
	}
	return self;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	self.state = UIGestureRecognizerStateBegan;
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchBegin, touches, theEvent, sl_false);
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchMove, touches, theEvent, sl_false);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	self.state = UIGestureRecognizerStateEnded;
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchEnd, touches, theEvent, sl_false);
	}
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventTouch(UIAction::TouchCancel, touches, theEvent, sl_false);
	}
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
	return YES;
}

@end

#endif
