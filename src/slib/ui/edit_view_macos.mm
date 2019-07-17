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

@interface SLIBEditViewHandle : NSTextField<NSTextFieldDelegate>
{	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;	
}
@end

@interface SLIBTextAreaHandle_TextView : NSTextView
{	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	@public NSAttributedString* m_placeholderString;
}
@end

@interface SLIBTextAreaHandle : NSScrollView<NSTextViewDelegate>
{	
	@public SLIBTextAreaHandle_TextView* m_textView;	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
}
@end

namespace slib
{

	namespace priv
	{
		namespace edit_view
		{

			class EditViewHelper : public EditView
			{
			public:
				void _applyPlaceholder(NSView* handle, NSFont* hFont)
				{
					NSAttributedString* attr = nil;
					if (hFont == nil) {
						Ref<Font> font = getFont();
						hFont = GraphicsPlatform::getNSFont(font.get());
					}
					if (hFont != nil) {
						String _text = m_hintText;
						if (_text.isNotEmpty()) {
							NSString* text = Apple::getNSStringFromString(_text, @"");
							NSColor* color = GraphicsPlatform::getNSColorFromColor(m_hintTextColor);
							NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
							[paragraphStyle setAlignment:translateAlignment(m_textAlignment)];
							attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle, NSFontAttributeName: hFont}];
						}
					}
					if ([handle isKindOfClass:[NSTextField class]]) {
						NSTextField* tv = (NSTextField*)handle;
						[[tv cell] setPlaceholderAttributedString:attr];
					} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
						SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
						tv->m_textView->m_placeholderString = attr;
						if (m_text.isEmpty() && attr != nil) {
							[tv setNeedsDisplay:YES];
						}
					}
				}

				void _applyProperties(NSTextField* handle)
				{
					if (m_flagPassword) {
						handle.cell = [[NSSecureTextFieldCell alloc] init];
					} else {
						handle.cell = [[NSTextFieldCell alloc] init];
					}
					
					Ref<Font> font = getFont();
					NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
					if (hFont != nil) {
						[handle setFont:hFont];
					}
					
					[handle setStringValue:(Apple::getNSStringFromString(m_text, @""))];
					[handle setAlignment:translateAlignment(m_textAlignment)];
					[handle setBordered: (isBorder() ? YES : NO)];
					[handle setBezeled: (isBorder() ? YES : NO)];
					[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
					[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
					[handle setEditable:(m_flagReadOnly? NO : YES)];
					[handle setSelectable:YES];
					
					_applyPlaceholder(handle, hFont);

				}
				
				void _applyProperties(SLIBTextAreaHandle* handle)
				{
					NSTextView* tv = handle->m_textView;

					[[tv textContainer] setWidthTracksTextView:(m_multiLine == MultiLineMode::WordWrap || m_multiLine == MultiLineMode::BreakWord) ? YES : NO];
					[[tv textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];

					[handle setHasHorizontalScroller:(isHorizontalScrollBarVisible()?YES:NO)];
					[handle setHasVerticalScroller:(isVerticalScrollBarVisible() && (!isHeightWrapping())?YES:NO)];
					
					Ref<Font> font = getFont();
					NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
					_applyFont(tv, hFont, sl_false);
					
					NSString* text = Apple::getNSStringFromString(m_text, @"");
					[tv setString:text];
					[tv setAlignment:translateAlignment(m_textAlignment)];
					[handle setBorderType:(isBorder() ? NSBezelBorder : NSNoBorder)];
					[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
					[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
					[tv setEditable:(m_flagReadOnly?NO:YES)];
					[tv setSelectable:YES];
					
					_applyPlaceholder(handle, hFont);
				}
				
				static void _applyFont(NSTextView* tv, NSFont* font, sl_bool flagSet)
				{
					if (font != nil) {
						[tv setFont:font];
					}
					NSMutableParagraphStyle *paragraph = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
					CGFloat height = font.leading + font.ascender - font.descender;
					[paragraph setLineSpacing:2];
					[paragraph setMaximumLineHeight:height];
					[paragraph setMinimumLineHeight:height];
					[tv setDefaultParagraphStyle:paragraph];
					if (flagSet) {
						NSMutableAttributedString* text = (NSMutableAttributedString*)([tv attributedString]);
						[text addAttribute:NSParagraphStyleAttributeName value:paragraph range:NSMakeRange(0, text.length)];
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
					if (EditViewHelper* view = CastInstance<EditViewHelper>(_view.get())) {
						String text = Apple::getStringFromNSString([control stringValue]);
						String textNew = text;
						view->dispatchChange(&textNew);
						if (text != textNew) {
							NSString* str = Apple::getNSStringFromString(textNew, @"");
							[control setStringValue:str];
						}
					}
				}
				
				static void onChangeTextArea(macOS_ViewInstance* instance, SLIBTextAreaHandle* control)
				{
					Ref<View> _view = instance->getView();
					if (EditViewHelper* view = CastInstance<EditViewHelper>(_view.get())) {
						String text = Apple::getStringFromNSString([control->m_textView string]);
						String textNew = text;
						view->dispatchChange(&textNew);
						if (text != textNew) {
							NSString* str = Apple::getNSStringFromString(textNew, @"");
							[control->m_textView setString:str];
						}
					}
				}

			};
			
		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBEditViewHandle* handle = [[SLIBEditViewHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditViewHelper*)this)->_applyProperties(handle);
			[handle setDelegate:handle];
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBTextAreaHandle* handle = [[SLIBTextAreaHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditViewHelper*)this)->_applyProperties(handle);
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
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
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
			NSString* value = Apple::getNSStringFromString(_value, @"");
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				[tv setStringValue:value];
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[tv->m_textView setString:value];
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
				[tv setAlignment:EditViewHelper::translateAlignment(align)];
				((EditViewHelper*)this)->_applyPlaceholder(handle, nil);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[tv->m_textView setAlignment:EditViewHelper::translateAlignment(align)];
				((EditViewHelper*)this)->_applyPlaceholder(handle, nil);
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
			((EditViewHelper*)this)->_applyPlaceholder(handle, nil);
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
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
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
					if ([tv.cell isKindOfClass:[NSSecureTextFieldCell class]]) {
						return;
					}
				} else {
					if (!([tv.cell isKindOfClass:[NSSecureTextFieldCell class]])) {
						return;
					}
				}
				((EditViewHelper*)this)->_applyProperties(tv);
			}
		}
	}
	
	void EditView::_setMultiLine_NW(MultiLineMode mode)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setMultiLine_NW, this, mode));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[[tv->m_textView textContainer] setWidthTracksTextView:(mode == MultiLineMode::WordWrap || mode == MultiLineMode::BreakWord) ? YES : NO];
				[[tv->m_textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
			}
		}
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
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[tv->m_textView setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
			}
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
			((EditViewHelper*)this)->_applyPlaceholder(handle, nil);
		}
	}
	
	sl_ui_len EditView::_measureHeight_NW()
	{
		sl_ui_len height = 0;
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				height = (sl_ui_len)([handle fittingSize].height);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* textArea = (SLIBTextAreaHandle*)handle;
				NSTextView* tv = textArea->m_textView;
				NSLayoutManager* layoutManager = tv.layoutManager;
				NSTextContainer* textContainer = tv.textContainer;
				[layoutManager ensureLayoutForTextContainer:textContainer];
				NSRect usedRect = [layoutManager usedRectForTextContainer:textContainer];
				NSSize inset = [tv textContainerInset];
				height = (sl_ui_len)(usedRect.size.height + inset.height * 2) + 4;
			}
		}
		return height;
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
					((EditViewHelper*)this)->_applyPlaceholder(handle, hFont);
				} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
					SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
					EditViewHelper::_applyFont(tv->m_textView, hFont, sl_true);
					((EditViewHelper*)this)->_applyPlaceholder(handle, hFont);
				}
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
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[tv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
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
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				[tv->m_textView setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
			}
		}
	}

	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setScrollBarsVisible_NW, this, flagHorizontal, flagVertical));
			return;
		}
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBTextAreaHandle class]]) {
			SLIBTextAreaHandle* sv = (SLIBTextAreaHandle*)handle;
			[sv setHasHorizontalScroller:(flagHorizontal?YES:NO)];
			[sv setHasVerticalScroller:(flagVertical?YES:NO)];
		}
	}

}

using namespace slib;
using namespace slib::priv::edit_view;

@implementation SLIBEditViewHandle

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

- (void)controlTextDidChange:(NSNotification *)obj
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		EditViewHelper::onChangeTextField(instance.get(), self);
	}
}

@end

@implementation SLIBTextAreaHandle

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		m_textView = [[SLIBTextAreaHandle_TextView alloc] init];
		if (m_textView == nil) {
			return nil;
		}
		[m_textView setTextContainerInset:NSMakeSize(3, 3)];
		[m_textView setMinSize:NSMakeSize(0.0, 0.0)];
		[m_textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		[m_textView setVerticallyResizable:YES];
		[m_textView setHorizontallyResizable:YES];
		[m_textView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		[self setDocumentView:m_textView];
		
		[m_textView setDelegate:self];
	}
	return self;
}

-(void)textDidChange:(NSNotification *)obj
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		EditViewHelper::onChangeTextArea(instance.get(), self);
	}
}

@end

@implementation SLIBTextAreaHandle_TextView

MACOS_VIEW_DEFINE_ON_KEY

- (BOOL)becomeFirstResponder
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onSetFocus();
	}
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
		if ([[self string] length] == 0 && self != [[self window] firstResponder]) {
			NSRect rc = [self bounds];
			rc.origin.x += 2;
			rc.size.width -= 4;
			[self->m_placeholderString drawInRect:rc];
		}
	}
}

@end

#endif
