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

namespace slib
{
	namespace priv
	{
		namespace select_view
		{
			class SelectViewInstance;
		}
	}
}

@interface SLIBSelectViewHandle : UITextField<UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate>
{
	@public UIPickerView* m_picker;
	@public sl_uint32 m_selectionBefore;
	@public slib::WeakRef<slib::priv::select_view::SelectViewInstance> m_viewInstance;
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace select_view
		{
			
			static NSTextAlignment TranslateAlignment(Alignment _align)
			{
				Alignment align = _align & Alignment::HorizontalMask;
				if (align == Alignment::Center) {
					return NSTextAlignmentCenter;
				} else if (align == Alignment::Right) {
					return NSTextAlignmentRight;
				}
				return NSTextAlignmentLeft;
			}
			
			static void SetBorder(SLIBSelectViewHandle* handle, sl_bool flagBorder)
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
			
			class SelectViewHelper : public SelectView
			{
			public:
				sl_uint32 getItemsCount()
				{
					return (sl_uint32)(m_titles.getCount());
				}
				
				NSString* getItemTitle(sl_uint32 row)
				{
					String s = m_titles.getValueAt(row);
					return Apple::getNSStringFromString(s);
				}
				
				void selectItem(SLIBSelectViewHandle* handle, sl_uint32 row)
				{
					handle.text = getItemTitle(row);
					[handle->m_picker selectRow:row inComponent:0 animated:NO];
				}
				
			};
			
			class SelectViewInstance : public iOS_ViewInstance, public ISelectViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBSelectViewHandle* getHandle()
				{
					return (SLIBSelectViewHandle*)m_handle;
				}
				
				Ref<SelectViewHelper> getHelper()
				{
					return CastRef<SelectViewHelper>(getView());
				}
				
				void select(SelectView* view, sl_uint32 index) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						(static_cast<SelectViewHelper*>(view))->selectItem(handle, index);
					}
				}
				
				void refreshItemsCount(SelectView* view) override
				{
					refreshItemsContent(view);
				}
				
				void refreshItemsContent(SelectView* view) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle->m_picker reloadAllComponents];
					}
				}
				
				void setItemTitle(SelectView* view, sl_uint32 index, const String& title) override
				{
					refreshItemsContent(view);
				}
				
				void setGravity(SelectView* view, const Alignment& gravity) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle setTextAlignment:(TranslateAlignment(gravity))];
					}
				}
				
				void setTextColor(SelectView* view, const Color& color) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						SetBorder(handle, flag);
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle setBackgroundColor:(color.isZero() ? nil : GraphicsPlatform::getUIColorFromColor(color))];
					}
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					SLIBSelectViewHandle* handle = getHandle();
					if (handle != nil) {
						setHandleFont(handle, font);
					}
				}
				
				void onSelectItem(SLIBSelectViewHandle* handle, sl_uint32 row)
				{
					Ref<SelectViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						handle.text = helper->getItemTitle(row);
						helper->dispatchSelectItem(row);
					}
				}
				
				void onStartSelection(SLIBSelectViewHandle* handle)
				{
					Ref<SelectViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						sl_uint32 index = helper->getSelectedIndex();
						handle->m_selectionBefore = index;
						UIPickerView* picker = handle->m_picker;
						dispatch_async(dispatch_get_main_queue(), ^{
							[picker selectRow:index inComponent:0 animated:NO];
						});
					}
				}
				
				void onCancelSelection(SLIBSelectViewHandle* handle)
				{
					onSelectItem(handle, handle->m_selectionBefore);
				}
				
			};

			SLIB_DEFINE_OBJECT(SelectViewInstance, iOS_ViewInstance)
			
		}
	}

	using namespace priv::select_view;
	
	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		Ref<SelectViewInstance> ret = iOS_ViewInstance::create<SelectViewInstance, SLIBSelectViewHandle>(this, parent);
		if (ret.isNotNull()) {
			SLIBSelectViewHandle* handle = ret->getHandle();
			static_cast<SelectViewHelper*>(this)->selectItem(handle, m_indexSelected);
			[handle setTextAlignment:(TranslateAlignment(m_gravity))];
			[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
			SetBorder(handle, isBorder());
			Color backColor = getBackgroundColor();
			[handle setBackgroundColor:(backColor.isZero() ? nil : GraphicsPlatform::getUIColorFromColor(backColor))];
			iOS_ViewInstance::setHandleFont(handle, getFont());
			return ret;
		}
		return sl_null;
	}
	
	Ptr<ISelectViewInstance> SelectView::getSelectViewInstance()
	{
		return CastRef<SelectViewInstance>(getViewInstance());
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
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSelectItem(self, (sl_uint32)row);
	}
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;
{
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<SelectViewHelper> helper = instance->getHelper();
		if (helper.isNotNull()) {
			return (NSInteger)(helper->getItemsCount());
		}
	}
	return 0;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component;
{
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<SelectViewHelper> helper = instance->getHelper();
		if (helper.isNotNull()) {
			return helper->getItemTitle((sl_uint32)row);
		}
	}
	return @"";
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)aTextField
{
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<SelectViewHelper> helper = instance->getHelper();
		if (helper.isNotNull()) {
			if ((NSInteger)(helper->getItemsCount()) > 0) {
				return YES;
			}
		}
	}
	return NO;
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	[self sendActionsForControlEvents:UIControlEventEditingDidBegin];
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onStartSelection(self);
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
	Ref<SelectViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onCancelSelection(self);
	}
}
@end

#endif
