/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "slib/ui/tab_view.h"

#include "view_osx.h"

@interface _Slib_OSX_TabView : NSTabView<NSTabViewDelegate> {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

@interface _Slib_OSX_TabView_EmptyView : NSView {
}
@end

namespace slib
{

	class _TabView : public TabView
	{
	public:
		void __applyTabsCount(NSTabView* tv)
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
		
		void __copyTabs(NSTabView* tv)
		{
			ListLocker<TabViewItem> items(m_items);
			__applyTabsCount(tv);
			for (sl_uint32 i = 0; i < items.count; i++) {
				NSTabViewItem* t = [tv tabViewItemAtIndex:i];
				if (t != nil) {
					[t setLabel:Apple::getNSStringFromString(items[i].label)];
					__setTabContentView(tv, i, items[i].contentView);
				}
			}
			if ([tv numberOfTabViewItems] > 0) {
				[tv selectTabViewItemAtIndex:m_indexSelected];
			}
		}
		
		void __setTabContentView(NSTabView* tv, sl_uint32 index, const Ref<View>& view)
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
				handle = [[_Slib_OSX_TabView_EmptyView alloc] init];
			}
			[handle setHidden:NO];
			[item setView:handle];
		}
		
		void __onSelectTab(NSTabView* tv)
		{
			dispatchSelectTab((sl_uint32)([tv indexOfTabViewItem:[tv selectedTabViewItem]]));
		}
		
		void __updateContentViewSize(_Slib_OSX_TabView* tv)
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
		OSX_VIEW_CREATE_INSTANCE_BEGIN
		_Slib_OSX_TabView* handle = [[_Slib_OSX_TabView alloc] initWithFrame: frame];
		if (handle != nil) {
			[handle setDelegate:handle];
			((_TabView*)this)->__copyTabs(handle);
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			[handle setFont:hFont];
		}
		OSX_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void TabView::_refreshTabsCount_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TabView class]]) {
			_Slib_OSX_TabView* tv = (_Slib_OSX_TabView*)handle;
			((_TabView*)this)->__applyTabsCount(tv);
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
		if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TabView class]]) {
			_Slib_OSX_TabView* tv = (_Slib_OSX_TabView*)handle;
			((_TabView*)this)->__setTabContentView(tv, index, view);
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

@implementation _Slib_OSX_TabView
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
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_TabView* _view = slib::CastInstance<slib::_TabView>(view.get())) {
			_view->__updateContentViewSize(self);
		}
	}
}

- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_TabView* _view = slib::CastInstance<slib::_TabView>(view.get())) {
			_view->__onSelectTab(self);
		}
	}
}
@end

@implementation _Slib_OSX_TabView_EmptyView

@end

#endif
