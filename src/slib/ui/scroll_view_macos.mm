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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/scroll_view.h"

#include "view_macos.h"

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

@interface SLIBScrollViewHandle : NSScrollView
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
				using ScrollView::_onScroll_NW;
				
			};
			
			class ScrollViewInstance : public macOS_ViewInstance, public IScrollViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				NSScrollView* getHandle()
				{
					return (NSScrollView*)m_handle;
				}
				
				void refreshContentSize(ScrollView* view) override
				{
				}
				
				void setContentView(ScrollView* view, const Ref<View>& content) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						applyContent(view, handle, content);
					}
				}
				
				sl_bool getScrollPosition(ScrollView* view, ScrollPoint& _out) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						NSClipView* clip = [handle contentView];
						if (clip != nil) {
							NSPoint pt = [clip bounds].origin;
							_out.x = (sl_scroll_pos)(pt.x);
							_out.y = (sl_scroll_pos)(pt.y);
							return sl_true;
						}
					}
					return sl_false;
				}
				
				sl_bool getScrollRange(ScrollView* view, ScrollPoint& _out) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						NSClipView* clip = [handle contentView];
						NSView* doc = [handle documentView];
						if (clip != nil && doc != nil) {
							NSRect rcDoc = [doc frame];
							NSRect rcClip = [clip bounds];
							_out.x = (sl_scroll_pos)(NSMaxX(rcDoc) - NSWidth(rcClip));
							_out.y = (sl_scroll_pos)(NSMaxY(rcDoc) - NSHeight(rcClip));
							if (_out.x < 0) {
								_out.x = 0;
							}
							if (_out.y < 0) {
								_out.y = 0;
							}
							return sl_true;
						}
					}
					return sl_false;
				}
				
				void scrollTo(View* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						_scrollTo(handle, x, y, flagAnimate);
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						[handle setBorderType:(flag ? NSBezelBorder : NSNoBorder)];
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						_setBackgroundColor(handle, color);
					}
				}
				
				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
					NSScrollView* handle = getHandle();
					if (handle != nil) {
						[handle setHasHorizontalScroller:(flagHorizontal ? YES : NO)];
						[handle setHasVerticalScroller:(flagVertical ? YES : NO)];
					}
				}
				
				void applyContent(ScrollView* view, NSScrollView* handle, const Ref<View>& viewContent)
				{
					NSView* handleContent = nil;
					if (viewContent.isNotNull()) {
						Ref<ViewInstance> instance = viewContent->attachToNewInstance(sl_null);
						if (instance.isNotNull()) {
							handleContent = UIPlatform::getViewHandle(instance.get());
						}
					}
					[handle setDocumentView:handleContent];
				}
				
				void _scrollTo(NSScrollView* handle, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
				{
					NSClipView* clip = [handle contentView];
					if (clip != nil) {
						NSPoint pt;
						pt.x = (CGFloat)x;
						pt.y = (CGFloat)y;
						[clip scrollToPoint:pt];
						[handle reflectScrolledClipView:clip];
					}
				}
				
				void _setBackgroundColor(NSScrollView* handle, const Color& color)
				{
					if (color.a == 0) {
						handle.drawsBackground = FALSE;
					} else {
						handle.drawsBackground = TRUE;
						handle.backgroundColor = GraphicsPlatform::getNSColorFromColor(color);
					}
				}
				
				void apply(ScrollView* view)
				{
					NSScrollView* handle = getHandle();
					if (handle == nil) {
						return;
					}
					[handle setHasHorizontalScroller:(view->isHorizontalScrollBarVisible() ? YES : NO)];
					[handle setHasVerticalScroller:(view->isVerticalScrollBarVisible() ? YES : NO)];
					[handle setBorderType:(view->isBorder() ? NSBezelBorder : NSNoBorder)];
					_setBackgroundColor(handle, view->getBackgroundColor());
					applyContent(view, handle, view->getContentView());
					_scrollTo(handle, view->getScrollX(), view->getScrollY(), sl_false);
				}
				
				void onScroll(NSScrollView* sv)
				{
					NSClipView* clip = [sv contentView];
					if (clip != nil) {
						NSPoint pt = [clip bounds].origin;
						Ref<ScrollViewHelper> helper = CastRef<ScrollViewHelper>(getView());
						if (helper.isNotNull()) {
							helper->_onScroll_NW((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y));
						}
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(ScrollViewInstance, macOS_ViewInstance)
			
		}
	}
	
	using namespace priv::scroll_view;

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
	{
		Ref<ScrollViewInstance> ret = macOS_ViewInstance::create<ScrollViewInstance, SLIBScrollViewHandle>(this, parent);
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

-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)boundDidChange:(NSNotification *)notification {
	Ref<ScrollViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onScroll(self);
	}
}
@end

#endif
