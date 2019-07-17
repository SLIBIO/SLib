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

@interface SLIBScrollViewHandle : NSScrollView
{	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;	
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
				void _applyContent(NSScrollView* sv)
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
				
				void _applyProperties(NSScrollView* handle)
				{
					[handle setHasHorizontalScroller:(isHorizontalScrollBarVisible()?YES:NO)];
					[handle setHasVerticalScroller:(isVerticalScrollBarVisible()?YES:NO)];
					[handle setBorderType:(isBorder()?NSBezelBorder:NSNoBorder)];
					Color backgroundColor = getBackgroundColor();
					if (backgroundColor.a == 0) {
						handle.drawsBackground = FALSE;
					} else {
						handle.drawsBackground = TRUE;
						handle.backgroundColor = GraphicsPlatform::getNSColorFromColor(backgroundColor);
					}
					_applyContent(handle);
					_scrollTo(handle, getScrollX(), getScrollY(), sl_false);
				}
				
				static void _scrollTo(NSScrollView* sv, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
				{
					NSClipView* clip = [sv contentView];
					if (clip != nil) {
						NSPoint pt;
						pt.x = (CGFloat)x;
						pt.y = (CGFloat)y;
						[clip scrollToPoint:pt];
						[sv reflectScrolledClipView:clip];
					}
				}
				
				static void _onScroll(macOS_ViewInstance* instance, NSScrollView* sv)
				{
					NSClipView* clip = [sv contentView];
					if (clip != nil) {
						NSPoint pt=[clip bounds].origin;
						Ref<View> _view = instance->getView();
						if (ScrollViewHelper* view = CastInstance<ScrollViewHelper>(_view.get())) {
							view->_onScroll_NW((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y));
						}
					}
				}
			};
			
		}
	}
	
	using namespace priv::scroll_view;

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBScrollViewHandle* handle = [[SLIBScrollViewHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((ScrollViewHelper*)this)->_applyProperties(handle);
		}
		MACOS_VIEW_CREATE_INSTANCE_END
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
			((ScrollViewHelper*)this)->_applyContent(sv);
		}
	}

	void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			ScrollViewHelper::_scrollTo(sv, x, y, flagAnimate);
		}
	}

	ScrollPoint ScrollView::_getScrollPosition_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			NSClipView* clip = [sv contentView];
			if (clip != nil) {
				NSPoint pt=[clip bounds].origin;
				ScrollPoint ret;
				ret.x = (sl_scroll_pos)(pt.x);
				ret.y = (sl_scroll_pos)(pt.y);
				return ret;
			}
		}
		return ScrollPoint::zero();
	}

	ScrollPoint ScrollView::_getScrollRange_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			NSClipView* clip = [sv contentView];
			NSView* doc = [sv documentView];
			if (clip != nil && doc != nil) {
				ScrollPoint ret;
				NSRect rcDoc = [doc frame];
				NSRect rcClip = [clip bounds];
				ret.x = (sl_scroll_pos)(NSMaxX(rcDoc) - NSWidth(rcClip));
				ret.y = (sl_scroll_pos)(NSMaxY(rcDoc) - NSHeight(rcClip));
				if (ret.x < 0) {
					ret.x = 0;
				}
				if (ret.y < 0) {
					ret.y = 0;
				}
				return ret;
			}
		}
		return ScrollPoint::zero();
	}

	void ScrollView::_setBorder_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setBorder_NW, this, flag));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			[sv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
		}
	}

	void ScrollView::_setBackgroundColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setBackgroundColor_NW, this, color));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			if (color.a == 0) {
				sv.drawsBackground = FALSE;
			} else {
				sv.drawsBackground = TRUE;
				sv.backgroundColor = GraphicsPlatform::getNSColorFromColor(color);
			}
		}
	}
	
	void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setScrollBarsVisible_NW, this, flagHorizontal, flagVertical));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSScrollView class]]) {
			NSScrollView* sv = (NSScrollView*)handle;
			[sv setHasHorizontalScroller:(flagHorizontal?YES:NO)];
			[sv setHasVerticalScroller:(flagVertical?YES:NO)];
		}
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
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		ScrollViewHelper::_onScroll(instance.get(), self);
	}
}
@end

#endif
