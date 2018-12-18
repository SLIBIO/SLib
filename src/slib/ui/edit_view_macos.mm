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

#include "slib/ui/edit_view.h"
#include "slib/ui/core.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_TextField : NSTextField<NSTextFieldDelegate> {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	
}
@end

@interface _priv_Slib_macOS_TextArea_TextView : NSTextView {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;

	@public NSAttributedString* m_placeholderString;
	
}
@end

@interface _priv_Slib_macOS_TextArea : NSScrollView<NSTextViewDelegate> {
	
	@public _priv_Slib_macOS_TextArea_TextView* m_textView;
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;

}
@end

@interface _priv_Slib_macOS_TextFieldCell : NSTextFieldCell {
}
@end

@interface _priv_Slib_macOS_SecureTextFieldCell : NSSecureTextFieldCell {
}
@end

namespace slib
{

	class EditView_Impl : public EditView
	{
	public:
		void _applyPlaceholder(NSView* handle)
		{
			NSAttributedString* attr;
			
			String _text = m_hintText;
			if (_text.isEmpty()) {
				attr = nil;
			} else {
				NSString* text = Apple::getNSStringFromString(_text);
				NSColor* color = GraphicsPlatform::getNSColorFromColor(m_hintTextColor);
				NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
				[paragraphStyle setAlignment:translateAlignment(m_textAlignment)];
				NSFont* hFont = nil;
				if ([handle isKindOfClass:[NSTextField class]]) {
					NSTextField* tv = (NSTextField*)handle;
					hFont = tv.font;
				} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
					_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
					hFont = tv->m_textView.font;
				}

				if (hFont != nil) {
					attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle, NSFontAttributeName: hFont}];
				} else {
					attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle}];
				}
			}
			
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (attr != [[tv cell] placeholderAttributedString]) {
					[[tv cell] setPlaceholderAttributedString:attr];
				}
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				tv->m_textView->m_placeholderString = attr;
				if (m_text.isEmpty() && attr != nil) {
					[tv setNeedsDisplay:YES];
				}
			}
		}

		void _applyProperties(NSTextField* handle)
		{
			NSTextFieldCell* cell;
			if (m_flagPassword) {
				cell = [[_priv_Slib_macOS_SecureTextFieldCell alloc] init];
			} else {
				cell = [[_priv_Slib_macOS_TextFieldCell alloc] init];
			}
			handle.cell = cell;
			[handle setStringValue:(Apple::getNSStringFromString(m_text))];
			[handle setAlignment:translateAlignment(m_textAlignment)];
			[handle setBordered: (isBorder() ? YES : NO)];
			[handle setBezeled: (isBorder() ? YES : NO)];
			_applyPlaceholder(handle);
			[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			[handle setEditable:(m_flagReadOnly? NO : YES)];
			[handle setSelectable:YES];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		
		void _applyProperties(_priv_Slib_macOS_TextArea* handle)
		{
			NSTextView* tv = handle->m_textView;
			[tv setString:(Apple::getNSStringFromString(m_text))];
			[tv setAlignment:translateAlignment(m_textAlignment)];
			[handle setBorderType:(isBorder() ? NSBezelBorder : NSNoBorder)];
			_applyPlaceholder(handle);
			[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			[tv setEditable:(m_flagReadOnly?NO:YES)];
			[tv setSelectable:YES];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
		
		static NSTextAlignment translateAlignment(Alignment _align)
		{
			Alignment align = _align & Alignment::HorizontalMask;
			if (align == Alignment::Center) {
				return NSCenterTextAlignment;
			} else if (align == Alignment::Right) {
				return NSRightTextAlignment;
			}
			return NSLeftTextAlignment;
		}
		
		static void onChangeTextField(macOS_ViewInstance* instance, NSTextField* control)
		{
			Ref<View> _view = instance->getView();
			if (EditView_Impl* view = CastInstance<EditView_Impl>(_view.get())) {
				String text = Apple::getStringFromNSString([control stringValue]);
				String textNew = view->dispatchChange(text);
				if (text != textNew) {
					NSString* str = Apple::getNSStringFromString(textNew);
					[control setStringValue:str];
				}
			}
		}
		
		static void onChangeTextArea(macOS_ViewInstance* instance, _priv_Slib_macOS_TextArea* control)
		{
			Ref<View> _view = instance->getView();
			if (EditView_Impl* view = CastInstance<EditView_Impl>(_view.get())) {
				String text = Apple::getStringFromNSString([control->m_textView string]);
				String textNew = view->dispatchChange(text);
				if (text != textNew) {
					NSString* str = Apple::getNSStringFromString(textNew);
					[control->m_textView setString:str];
				}
			}
		}

	};

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_TextField* handle = [[_priv_Slib_macOS_TextField alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditView_Impl*)this)->_applyProperties(handle);
			[handle setDelegate:handle];
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_TextArea* handle = [[_priv_Slib_macOS_TextArea alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditView_Impl*)this)->_applyProperties(handle);
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		if (handle != nil) {
			handle->m_textView->m_viewInstance = ret;
		}
		return ret;
	}

	void EditView::_getText_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				NSString* s = [tv stringValue];
				m_text = Apple::getStringFromNSString(s);
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				NSString* s = [tv->m_textView string];
				m_text = Apple::getStringFromNSString(s);
			}
		}
	}

	void EditView::_setText_NW(const String& _value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setText_NW, this, _value));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			NSString* value = Apple::getNSStringFromString(_value);
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setStringValue:value];
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv->m_textView setString:value];
			}
		}
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setBorder_NW, this, flag));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setBordered:(flag?TRUE:FALSE)];
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
			}
		}
	}

	void EditView::_setTextAlignment_NW(Alignment align)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextAlignment_NW, this, align));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setAlignment:EditView_Impl::translateAlignment(align)];
				((EditView_Impl*)this)->_applyPlaceholder(handle);
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv->m_textView setAlignment:EditView_Impl::translateAlignment(align)];
				((EditView_Impl*)this)->_applyPlaceholder(handle);
			}
		}
	}

	void EditView::_setHintText_NW(const String& value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintText_NW, this, value));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			((EditView_Impl*)this)->_applyPlaceholder(handle);
		}
	}
	
	void EditView::_setHintTextColor_NW(const Color& value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintTextColor_NW, this, value));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			((EditView_Impl*)this)->_applyPlaceholder(handle);
		}
	}
	
	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setReadOnly_NW, this, flag));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setEditable:(flag ? FALSE : TRUE)];
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv->m_textView setEditable:(flag ? FALSE : TRUE)];
			}
		}
	}
	
	void EditView::_setPassword_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setPassword_NW, this, flag));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (m_flagPassword) {
					if ([tv.cell isKindOfClass:[_priv_Slib_macOS_SecureTextFieldCell class]]) {
						return;
					}
				} else {
					if ([tv.cell isKindOfClass:[_priv_Slib_macOS_TextFieldCell class]]) {
						return;
					}
				}
				((EditView_Impl*)this)->_applyProperties(tv);
			}
		}
	}
	
	void EditView::_setMultiLine_NW(sl_bool flag)
	{
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextColor_NW, this, color));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv->m_textView setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
			}
		}
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setBackgroundColor_NW, this, color));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
			} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
				_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
				[tv->m_textView setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
			}
		}
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setFont_NW, this, font));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				if ([handle isKindOfClass:[NSTextField class]]) {
					NSTextField* tv = (NSTextField*)handle;
					[tv setFont:hFont];
					((EditView_Impl*)this)->_applyPlaceholder(handle);
				} else if ([handle isKindOfClass:[_priv_Slib_macOS_TextArea class]]) {
					_priv_Slib_macOS_TextArea* tv = (_priv_Slib_macOS_TextArea*)handle;
					[tv->m_textView setFont:hFont];
					((EditView_Impl*)this)->_applyPlaceholder(handle);
				}
			}
		}
	}

}

@implementation _priv_Slib_macOS_TextField

- (void)controlTextDidChange:(NSNotification *)obj
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::EditView_Impl::onChangeTextField(instance.get(), self);
	}
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyUp:theEvent];
}

@end

@implementation _priv_Slib_macOS_TextArea

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		m_textView = [[_priv_Slib_macOS_TextArea_TextView alloc] init];
		if (m_textView == nil) {
			return nil;
		}
		[m_textView setMinSize:NSMakeSize(0.0, 0.0)];
		[m_textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		[m_textView setVerticallyResizable:YES];
		[m_textView setHorizontallyResizable:YES];
		[m_textView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		[[m_textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		
		[self setDocumentView:m_textView];
		[self setHasVerticalScroller:TRUE];
		[self setHasHorizontalScroller:TRUE];
		
		[m_textView setDelegate:self];
	}
	return self;
}

-(void)textDidChange:(NSNotification *)obj
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::EditView_Impl::onChangeTextArea(instance.get(), self);
	}
}

@end

@implementation _priv_Slib_macOS_TextArea_TextView

- (void)keyDown:(NSEvent*)theEvent
{
	if ([theEvent keyCode] == 0x35) {
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			sl_bool flagNoDefault = instance->onEventKey(sl_true, theEvent);
			if (flagNoDefault) {
				return;
			}
		}
	}
	[super keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyUp:theEvent];
}

- (BOOL)becomeFirstResponder
{
	if (m_placeholderString != nil) {
		[self setNeedsDisplay:YES];
	}
	return [super becomeFirstResponder];
}

- (BOOL)resignFirstResponder
{
	if (m_placeholderString != nil) {
		[self setNeedsDisplay:YES];
	}
	return [super resignFirstResponder];
}

- (void)drawRect:(NSRect)rect
{
	[super drawRect:rect];
	if (self->m_placeholderString != nil) {
		if ([[self string] isEqualToString:@""] && self != [[self window] firstResponder]) {
			[self->m_placeholderString drawInRect:[self bounds]];
		}
	}
}

@end

@implementation _priv_Slib_macOS_TextFieldCell

#define TEXT_HEIGHT_FACTOR 1.5
#define TEXT_PADDING_FACTOR 0.4
#define LABEL_PADDING_FACTOR 0.1

- (void)drawInteriorWithFrame:(NSRect)cellFrame inView:(NSView *)controlView
{
	if (((NSTextField*)controlView).currentEditor == nil) {
		cellFrame = [self drawingRectForBounds:cellFrame];
		if (self.font != nil) {
			cellFrame.origin.y -= self.font.pointSize * LABEL_PADDING_FACTOR;
		}
		[self.attributedStringValue drawInRect:cellFrame];
	}
}

- (NSRect)drawingRectForBounds:(NSRect)rect
{
	if (self.font != nil) {
		rect = [super drawingRectForBounds: rect];
		rect.origin.y -= self.font.pointSize * TEXT_PADDING_FACTOR;
		rect.size.height = self.font.pointSize * TEXT_HEIGHT_FACTOR;
		return rect;
	} else {
		return [super drawingRectForBounds: rect];
	}
}

@end

@implementation _priv_Slib_macOS_SecureTextFieldCell

- (void)drawInteriorWithFrame:(NSRect)cellFrame inView:(NSView *)controlView
{
	if (((NSTextField*)controlView).currentEditor == nil) {
		cellFrame = [self drawingRectForBounds:cellFrame];
		if (self.font != nil) {
			cellFrame.origin.y -= self.font.pointSize * LABEL_PADDING_FACTOR;
		}
		NSMutableAttributedString* password = [self.attributedStringValue mutableCopy];
		NSMutableString* str = password.mutableString;
		[str setString:[@"" stringByPaddingToLength:str.length withString:@"•" startingAtIndex:0]];
		[password drawInRect:cellFrame];
	}
}

- (NSRect)drawingRectForBounds:(NSRect)rect
{
	if (self.font != nil) {
		rect = [super drawingRectForBounds: rect];
		rect.origin.y -= self.font.pointSize * TEXT_PADDING_FACTOR;
		rect.size.height = self.font.pointSize * TEXT_HEIGHT_FACTOR;
		return rect;
	} else {
		return [super drawingRectForBounds: rect];
	}
}

@end

#endif
