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

#include "slib/ui/tab_view.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_TabView : NSTabView<NSTabViewDelegate> {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	
}
@end

@interface _priv_Slib_macOS_TabView_EmptyView : NSView {
}
@end

namespace slib
{

	class _priv_TabView : public TabView
	{
	public:
		void _applyTabsCount(NSTabView* tv)
		{
			ObjectLocker lock(this);
			sl_uint32 nNew = (sl_uint32)(m_items.getCount());
			sl_uint32 nOrig = (sl_uint32)([tv numberOfTabViewItems]);
			if (nOrig == nNew) {
				return;
			}
			if (nOrig > nNew) {
				for (sl_uint32 i = nOrig; i > nNew; i--) {
					NSTabViewItem* item = [tv tabViewItemAtIndex:(i - 1)];
					if (item != nil) {
						[tv removeTabViewItem:item];
					}
				}
			} else {
				for (sl_uint32 i = nOrig; i < nNew; i++) {
					NSTabViewItem* item = [[NSTabViewItem alloc] initWithIdentifier:[NSString stringWithFormat:@"%d",i]];
					if (item != nil) {
						[tv addTabViewItem:item];
					}
				}
			}
		}
		
		void _copyTabs(NSTabView* tv)
		{
			ListLocker<TabViewItem> items(m_items);
			_applyTabsCount(tv);
			for (sl_uint32 i = 0; i < items.count; i++) {
				NSTabViewItem* t = [tv tabViewItemAtIndex:i];
				if (t != nil) {
					[t setLabel:Apple::getNSStringFromString(items[i].label)];
					_setTabContentView(tv, i, items[i].contentView);
				}
			}
			if ([tv numberOfTabViewItems] > 0) {
				[tv selectTabViewItemAtIndex:m_indexSelected];
			}
		}
		
		void _setTabContentView(NSTabView* tv, sl_uint32 index, const Ref<View>& view)
		{
			NSTabViewItem* item = [tv tabViewItemAtIndex:index];
			if (item == nil) {
				return;
			}
			NSView* handle = nil;
			if (view.isNotNull()) {
				Ref<ViewInstance> instance = view->attachToNewInstance(Ref<ViewInstance>::null());
				if (instance.isNotNull()) {
					handle = UIPlatform::getViewHandle(instance.get());
				}
				NSRect rc = [tv contentRect];
				view->setFrame((sl_ui_pos)(rc.origin.x), (sl_ui_pos)(rc.origin.y), (sl_ui_pos)(rc.size.width), (sl_ui_pos)(rc.size.height));
				view->setParent(this);
			}
			if (handle == nil) {
				handle = [[_priv_Slib_macOS_TabView_EmptyView alloc] init];
			}
			[handle setHidden:NO];
			[item setView:handle];
		}
		
		void _onSelectTab(NSTabView* tv)
		{
			dispatchSelectTab((sl_uint32)([tv indexOfTabViewItem:[tv selectedTabViewItem]]));
		}
		
		void _updateContentViewSize(_priv_Slib_macOS_TabView* tv)
		{
			NSRect rc = [tv contentRect];
			UIRect frame;
			sl_ui_pos w = (sl_ui_pos)(rc.size.width);
			sl_ui_pos h = (sl_ui_pos)(rc.size.height);
			frame.left = (sl_ui_pos)(rc.origin.x);
			frame.top = (sl_ui_pos)(rc.origin.y);
			frame.right = frame.left + w;
			frame.bottom = frame.top + h;
			ListLocker<TabViewItem> items(m_items);
			for (sl_size i = 0; i < items.count; i++) {
				Ref<View> view = items[i].contentView;
				if (view.isNotNull()) {
					view->setFrame(frame);
				}
			}
		}
	};

	Ref<ViewInstance> TabView::createNativeWidget(ViewInstance* _parent)
	{
		ObjectLocker lock(this);
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_TabView* handle = [[_priv_Slib_macOS_TabView alloc] initWithFrame: frame];
		if (handle != nil) {
			[handle setDelegate:handle];
			((_priv_TabView*)this)->_copyTabs(handle);
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			[handle setFont:hFont];
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void TabView::_refreshTabsCount_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_macOS_TabView class]]) {
			_priv_Slib_macOS_TabView* tv = (_priv_Slib_macOS_TabView*)handle;
			((_priv_TabView*)this)->_applyTabsCount(tv);
		}
	}

	void TabView::_refreshSize_NW()
	{
	}

	void TabView::_setTabLabel_NW(sl_uint32 index, const String& text)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
			NSTabView* tv = (NSTabView*)handle;
			NSTabViewItem* t = [tv tabViewItemAtIndex:index];
			if (t != nil) {
				[t setLabel:Apple::getNSStringFromString(text)];
			}
		}
	}

	void TabView::_setTabContentView_NW(sl_uint32 index, const Ref<View>& view)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_macOS_TabView class]]) {
			_priv_Slib_macOS_TabView* tv = (_priv_Slib_macOS_TabView*)handle;
			((_priv_TabView*)this)->_setTabContentView(tv, index, view);
		}
	}

	void TabView::_getSelectedTabIndex_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
			NSTabView* tv = (NSTabView*)handle;
			m_indexSelected = (sl_uint32)([tv indexOfTabViewItem:[tv selectedTabViewItem]]);
		}
	}

	void TabView::_selectTab_NW(sl_uint32 index)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
			NSTabView* tv = (NSTabView*)handle;
			[tv selectTabViewItemAtIndex:index];
		}
	}

	UISize TabView::_getContentViewSize_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
			NSTabView* tv = (NSTabView*)handle;
			NSRect rc = [tv contentRect];
			UISize ret;
			ret.x = (sl_ui_pos)(rc.size.width);
			ret.y = (sl_ui_pos)(rc.size.height);
			return ret;
		}
		return UISize::zero();
	}

	void TabView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTabView class]]) {
			NSTabView* tv = (NSTabView*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			[tv setFont:hFont];
		}
	}

}

@implementation _priv_Slib_macOS_TabView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}

-(void)setFrame:(NSRect)frame
{
	[super setFrame:frame];
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_priv_TabView* _view = slib::CastInstance<slib::_priv_TabView>(view.get())) {
			_view->_updateContentViewSize(self);
		}
	}
}

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_priv_TabView* _view = slib::CastInstance<slib::_priv_TabView>(view.get())) {
			_view->_onSelectTab(self);
		}
	}
}
@end

@implementation _priv_Slib_macOS_TabView_EmptyView

@end

#endif
