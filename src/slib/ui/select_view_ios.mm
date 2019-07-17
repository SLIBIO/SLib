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

#include "slib/ui/select_view.h"
#include "slib/ui/core.h"

#include "view_ios.h"

@interface SLIBSelectViewHandle : UITextField<UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate>
{
	@public UIPickerView* m_picker;
	
	@public sl_uint32 m_selectionBefore;
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace select_view
		{
			
			class SelectViewHelper : public SelectView
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
				
				void _onSelectItem(SLIBSelectViewHandle* v, sl_uint32 row)
				{
					v.text = _getItemTitle(row);
					dispatchSelectItem(row);
				}
				
				void _onStartSelection(SLIBSelectViewHandle* v)
				{
					sl_uint32 n = m_indexSelected;
					v->m_selectionBefore = n;
					UIPickerView* picker = v->m_picker;
					dispatch_async(dispatch_get_main_queue(), ^{
						[picker selectRow:n inComponent:0 animated:NO];
					});
				}
				
				void _onCancelSelection(SLIBSelectViewHandle* v)
				{
					_onSelectItem(v, v->m_selectionBefore);
				}
				
				void _selectItem(SLIBSelectViewHandle* v, sl_uint32 row)
				{
					v.text = _getItemTitle(row);
					[v->m_picker selectRow:row inComponent:0 animated:NO];
				}
				
				static NSTextAlignment translateAlignment(Alignment _align)
				{
					Alignment align = _align & Alignment::HorizontalMask;
					if (align == Alignment::Center) {
						return NSTextAlignmentCenter;
					} else if (align == Alignment::Right) {
						return NSTextAlignmentRight;
					}
					return NSTextAlignmentLeft;
				}
				
				static void setBorder(SLIBSelectViewHandle* handle, sl_bool flagBorder)
				{
					if (flagBorder) {
						[handle.layer setBorderColor:[UIColor grayColor].CGColor];
						[handle.layer setBorderWidth:(UI::dpToPixel(1) / UIPlatform::getGlobalScaleFactor())];
						[handle.layer setCornerRadius:(UI::dpToPixel(5) / UIPlatform::getGlobalScaleFactor())];
					} else {
						[handle.layer setBorderWidth:0];
						[handle.layer setCornerRadius:0];
					}
				}
				
			};

		}
	}

	using namespace priv::select_view;
	
	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBSelectViewHandle* handle = [[SLIBSelectViewHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((SelectViewHelper*)this)->_selectItem(handle, m_indexSelected);
			
			[handle setTextAlignment:(SelectViewHelper::translateAlignment(m_textAlignment))];
			[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
			SelectViewHelper::setBorder(handle, isBorder());
			Color backColor = getBackgroundColor();
			[handle setBackgroundColor:(backColor.isZero() ? nil : GraphicsPlatform::getUIColorFromColor(backColor))];
			
			Ref<Font> font = getFont();
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		IOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
	
	void SelectView::_select_NW(sl_uint32 index)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _select_NW, this, index));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			((SelectViewHelper*)this)->_selectItem(v, index);
		}
	}
	
	void SelectView::_refreshItemsCount_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(SelectView, _refreshItemsCount_NW, this));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v->m_picker reloadAllComponents];
		}
	}
	
	void SelectView::_refreshItemsContent_NW()
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_FUNCTION_WEAKREF(SelectView, _refreshItemsContent_NW, this));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v->m_picker reloadAllComponents];
		}
	}
	
	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setItemTitle_NW, this, index, title));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v->m_picker reloadAllComponents];
		}
	}
	
	void SelectView::_setTextAlignment_NW(Alignment align)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setTextAlignment_NW, this, align));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v setTextAlignment:(SelectViewHelper::translateAlignment(align))];
		}
	}
	
	void SelectView::_setTextColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setTextColor_NW, this, color));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
		}
	}
	
	void SelectView::_setBorder_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setBorder_NW, this, flag));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			SelectViewHelper::setBorder(v, flag);
		}
	}
	
	void SelectView::_setBackgroundColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setBackgroundColor_NW, this, color));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			[v setBackgroundColor:(color.isZero() ? nil : GraphicsPlatform::getUIColorFromColor(color))];
		}
	}
	
	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SelectView, _setFont_NW, this, font));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBSelectViewHandle class]]) {
			SLIBSelectViewHandle* v = (SLIBSelectViewHandle*)handle;
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[v setFont:hFont];
			}
		}
	}
	
}

using namespace slib;
using namespace slib::priv::select_view;

#define DROP_ICON_WIDTH 20
#define DROP_ICON_HEIGHT 12

@interface SLIBSelectViewHandle_DropIcon : UIView
{
	@public __weak SLIBSelectViewHandle* parent;
}
@end

@implementation SLIBSelectViewHandle_DropIcon

- (id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setClipsToBounds:YES];
		[self setOpaque:FALSE];
	}
	return self;
}

- (void)drawRect:(CGRect)dirtyRect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextBeginPath(context);
	float pl = DROP_ICON_WIDTH * 0.2f;
	float pr = DROP_ICON_WIDTH * 0.3f;
	float ph = DROP_ICON_HEIGHT * 0.3f;
	CGContextSetAllowsAntialiasing(context, YES);
	CGContextSetShouldAntialias(context, YES);
	CGContextMoveToPoint(context, pl, ph);
	CGContextAddLineToPoint(context, pl + (DROP_ICON_WIDTH - pl - pr) / 2, DROP_ICON_HEIGHT - ph);
	CGContextAddLineToPoint(context, DROP_ICON_WIDTH - pr, ph);
	CGContextSetLineWidth(context, UI::dpToPixel(2) / UIPlatform::getGlobalScaleFactor());
	CGContextSetLineCap(context, kCGLineCapRound);
	CGContextSetLineJoin(context, kCGLineJoinRound);
	CGContextSetRGBStrokeColor(context, 0.5, 0.5, 0.5, 1);
	CGContextStrokePath(context);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(::UIEvent *)theEvent
{
	[super touchesBegan:touches withEvent:theEvent];
	[parent becomeFirstResponder];
}
@end

@implementation SLIBSelectViewHandle

IOS_VIEW_DEFINE_ON_FOCUS

-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if (self != nil) {
	
		self->m_selectionBefore = 0;
		
		[self setDelegate:self];
		
		// hide the caret and its blinking
		[[self valueForKey:@"textInputTraits"] setValue:[UIColor clearColor] forKey:@"insertionPointColor"];
		
		// add icon
		SLIBSelectViewHandle_DropIcon* icon = [[SLIBSelectViewHandle_DropIcon alloc] initWithFrame:(CGRectMake(0, 0, DROP_ICON_WIDTH, DROP_ICON_HEIGHT))];
		icon->parent = self;
		self.rightView =  icon;
		self.rightViewMode = UITextFieldViewModeAlways;
		
		// picker
		UIPickerView* picker = [[UIPickerView alloc] init];
		picker.showsSelectionIndicator = YES;
		picker.dataSource = self;
		picker.delegate = self;
		self->m_picker = picker;
	
		// toolbar
		UIToolbar* toolbar = [[UIToolbar alloc] init];
		toolbar.barStyle = UIBarStyleDefault;
		[toolbar sizeToFit];
		UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
		UIBarButtonItem* doneButton = [[UIBarButtonItem alloc] initWithTitle:@"Done" style:UIBarButtonItemStyleDone target:self action:@selector(doneClicked:)];
		UIBarButtonItem* cancelButton = [[UIBarButtonItem alloc] initWithTitle:@"Cancel" style:UIBarButtonItemStylePlain target:self action:@selector(cancelClicked:)];
		[toolbar setItems:[NSArray arrayWithObjects:cancelButton, flexibleSpace, doneButton, nil]];

		self.inputView = picker;
		self.inputAccessoryView = toolbar;
	}
	return self;
}

- (CGRect)textRectForBounds:(CGRect)bounds
{
	bounds.origin.x += UI::dpToPixel(4) / UIPlatform::getGlobalScaleFactor();
	bounds.size.width -= UI::dpToPixel(8) / UIPlatform::getGlobalScaleFactor();
	return bounds;
}

- (CGRect)editingRectForBounds:(CGRect)bounds
{
	return [self textRectForBounds:bounds];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView;
{
	return 1;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			_view->_onSelectItem(self, (sl_uint32)row);
		}
	}
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			return (NSInteger)(_view->_getItemsCount());
		}
	}
	return 0;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component;
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			return _view->_getItemTitle((sl_uint32)row);
		}
	}
	return @"";
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)aTextField
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			if ((NSInteger)(_view->_getItemsCount()) > 0) {
				return YES;
			}
		}
	}
	return NO;
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	[self sendActionsForControlEvents:UIControlEventEditingDidBegin];
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			_view->_onStartSelection(self);
		}
	}
}

- (void)textFieldDidEndEditing:(UITextField *)aTextField {
	aTextField.userInteractionEnabled = YES;
	[self sendActionsForControlEvents:UIControlEventEditingDidEnd];
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	return NO;
}

-(void)doneClicked:(id) sender
{
	[self resignFirstResponder];
	[self sendActionsForControlEvents:UIControlEventValueChanged];
}

-(void)cancelClicked:(id)sender
{
	[self resignFirstResponder];
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (SelectViewHelper* _view = CastInstance<SelectViewHelper>(view.get())) {
			_view->_onCancelSelection(self);
		}
	}
}
@end

#endif
