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
	
	void PickerView::_select_NW(sl_uint32 index)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), PickerView, _select_NW, this, index));
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(PickerView, _refreshItemsCount_NW, this));
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(PickerView, _refreshItemsContent_NW, this));
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), PickerView, _setItemTitle_NW, this, index, title));
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), PickerView, _setFont_NW, this, font));
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

IOS_VIEW_DEFINE_ON_FOCUS

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
