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

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/picker_view.h"

#include "view_ios.h"

@interface _priv_Slib_iOS_PickerView : UIPickerView<UIPickerViewDelegate, UIPickerViewDataSource>
{
	@public sl_uint32 m_selectionBefore;
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	@public slib::WeakRef<slib::PickerView> m_view;
	
	@public UIFont* m_font;
	
}
@end

namespace slib
{
	class _priv_PickerView : public PickerView
	{
	public:
		sl_uint32 _getItemsCount()
		{
			return (sl_uint32)(m_titles.getCount());
		}
		
		NSString* _getItemTitle(sl_uint32 row)
		{
			String s = m_titles.getValueAt(row);
			return Apple::getNSStringFromString(s);
		}
		
		void _onSelectItem(_priv_Slib_iOS_PickerView* v, sl_uint32 row)
		{
			m_indexSelected = row;
			dispatchSelectItem(row);
		}
		
		void _selectItem(_priv_Slib_iOS_PickerView* v, sl_uint32 row)
		{
			[v selectRow:row inComponent:0 animated:NO];
		}
	};
	
	Ref<ViewInstance> PickerView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_iOS_PickerView* handle = [[_priv_Slib_iOS_PickerView alloc] initWithFrame:frame];
		if (handle != nil) {
			Ref<Font> font = getFont();
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				handle->m_font = hFont;
			}
			handle->m_view = this;
		}
		IOS_VIEW_CREATE_INSTANCE_END
		if (handle != nil) {
			((_priv_PickerView*)this)->_selectItem(handle, m_indexSelected);
		}
		return ret;
	}
	
	void PickerView::_getSelectedIndex_NW()
	{
	}
	
	void PickerView::_select_NW(sl_uint32 index)
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_select_NW(index);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_PickerView class]]) {
			_priv_Slib_iOS_PickerView* v = (_priv_Slib_iOS_PickerView*)handle;
			((_priv_PickerView*)this)->_selectItem(v, index);
		}
	}
	
	void PickerView::_refreshItemsCount_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_refreshItemsCount_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_PickerView class]]) {
			_priv_Slib_iOS_PickerView* v = (_priv_Slib_iOS_PickerView*)handle;
			[v reloadAllComponents];
			((_priv_PickerView*)this)->_selectItem(v, m_indexSelected);
		}
	}
	
	void PickerView::_refreshItemsContent_NW()
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_refreshItemsContent_NW();
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_PickerView class]]) {
			_priv_Slib_iOS_PickerView* v = (_priv_Slib_iOS_PickerView*)handle;
			[v reloadAllComponents];
			((_priv_PickerView*)this)->_selectItem(v, m_indexSelected);
		}
	}
	
	void PickerView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		if (![NSThread isMainThread]) {
			String _title = title;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setItemTitle_NW(index, _title);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_PickerView class]]) {
			_priv_Slib_iOS_PickerView* v = (_priv_Slib_iOS_PickerView*)handle;
			[v reloadAllComponents];
		}
	}
	
	void PickerView::_setFont_NW(const Ref<Font>& font)
	{
		if (![NSThread isMainThread]) {
			Ref<Font> _font = font;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setFont_NW(_font);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_priv_Slib_iOS_PickerView class]]) {
			_priv_Slib_iOS_PickerView* v = (_priv_Slib_iOS_PickerView*)handle;
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				v->m_font = hFont;
				[v reloadAllComponents];
			}
		}
	}	
}

@implementation _priv_Slib_iOS_PickerView
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if (self != nil) {
	
		self->m_selectionBefore = 0;
		
		self.showsSelectionIndicator = YES;
		self.dataSource = self;
		self.delegate = self;

	}
	return self;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView;
{
	return 1;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	slib::Ref<slib::PickerView> view = m_view;
	if (view.isNotNull()) {
		((slib::_priv_PickerView*)(view.get()))->_onSelectItem(self, (sl_uint32)row);
	}
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;
{
	slib::Ref<slib::PickerView> view = m_view;
	if (view.isNotNull()) {
		return (NSInteger)(((slib::_priv_PickerView*)(view.get()))->_getItemsCount());
	}
	return 0;
}

- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(UIView *)view{
	UILabel* label = (UILabel*)view;
	if (!label){
		label = [[UILabel alloc] init];
		label.font = m_font;
		label.textAlignment = NSTextAlignmentCenter;
	}
	slib::Ref<slib::PickerView> picker = m_view;
	if (picker.isNotNull()) {
		label.text = ((slib::_priv_PickerView*)(picker.get()))->_getItemTitle((sl_uint32)row);
	}
	return label;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
	UIFont* font = m_font;
	if (font != nil) {
		CGFloat h = font.lineHeight;
		if (h < 20) {
			h = 20;
		}
		return h;
	} else {
		return 20;
	}
}

@end

#endif
