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

#include "slib/ui/select_view.h"

#include "view_osx.h"

@interface _Slib_OSX_SelectView : NSPopUpButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

namespace slib
{

	class _SelectView : public SelectView
	{
	public:
		void _applyItemsCount(NSPopUpButton* v)
		{
			ObjectLocker lock(this);
			sl_uint32 nOrig = (sl_uint32)([v numberOfItems]);
			sl_uint32 nNew = (sl_uint32)(m_titles.getCount());
			if (nOrig == nNew) {
				return;
			}
			if (nOrig > nNew) {
				if (nNew  > 0) {
					for (sl_uint32 i = nOrig; i > nNew; i--) {
						[v removeItemAtIndex:(i - 1)];
					}
				} else {
					[v removeAllItems];
				}
			} else {
				for (sl_uint32 i = nOrig; i < nNew; i++) {
					[v addItemWithTitle:[NSString stringWithFormat:@"%d",i]];
					NSMenuItem* item = [v lastItem];
					if (item != nil) {
						NSString* s = Apple::getNSStringFromString(m_titles.getValueAt(i));
						if (s == nil) {
							s = @"";
						}
						[item setTitle:s];
					}
				}
			}
		}
		
		void _copyItems(NSPopUpButton* v)
		{
			[v removeAllItems];
			_applyItemsCount(v);
			if (m_indexSelected >= m_titles.getCount()) {
				m_indexSelected = 0;
			}
			if ([v numberOfItems] > 0) {
				[v selectItemAtIndex:m_indexSelected];
			}
		}
		
		void _setItemTitle(NSPopUpButton* v, sl_uint32 index, const String& title)
		{
			NSString* s = Apple::getNSStringFromString(title);
			if (s == nil) {
				s = @"";
			}
			NSMenuItem* item = [v itemAtIndex:index];
			if (item != nil) {
				[item setTitle:s];
			}
		}
		
		sl_uint32 _getSelectedIndex(NSPopUpButton* v)
		{
			return (sl_uint32)([v indexOfSelectedItem]);
		}
		
		void _onSelectItem(NSPopUpButton* v)
		{
			dispatchSelectItem(_getSelectedIndex(v));
		}
	};

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
	{
		OSX_VIEW_CREATE_INSTANCE_BEGIN
		_Slib_OSX_SelectView* handle = [[_Slib_OSX_SelectView alloc] initWithFrame:frame];
		if (handle != nil) {
			[handle setPullsDown:NO];
			((_SelectView*)this)->_copyItems(handle);
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		OSX_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void SelectView::_getSelectedIndex_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			m_indexSelected = ((_SelectView*)this)->_getSelectedIndex(v);
		}
	}

	void SelectView::_select_NW(sl_uint32 index)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			[v selectItemAtIndex:index];
		}
	}

	void SelectView::_refreshItemsCount_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			((_SelectView*)this)->_applyItemsCount(v);
		}
	}

	void SelectView::_refreshItemsContent_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			((_SelectView*)this)->_copyItems(v);
		}
	}

	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			((_SelectView*)this)->_setItemTitle(v, index, title);
		}
	}

	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSPopUpButton class]]) {
			NSPopUpButton* v = (NSPopUpButton*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[v setFont:hFont];
			}
		}
	}

}

@implementation _Slib_OSX_SelectView
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setAction: @selector(onSelect)];
		[self setTarget:self];
	}
	return self;
}

-(void)onSelect
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			_view->_onSelectItem(self);
		}
	}
}
@end

#endif
