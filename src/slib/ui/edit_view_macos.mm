/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "view_macos.h"

namespace slib
{
	namespace priv
	{
		namespace edit_view
		{
			class EditViewInstance;
			class TextAreaInstance;
		}
	}
}

@interface SLIBEditViewHandle : NSTextField<NSTextFieldDelegate>
{	
	@public slib::WeakRef<slib::priv::edit_view::EditViewInstance> m_viewInstance;
}
@end

@interface SLIBTextAreaHandle_TextView : NSTextView
{	
	@public slib::WeakRef<slib::priv::edit_view::TextAreaInstance> m_viewInstance;
	@public NSAttributedString* m_placeholderString;
	@public slib::Alignment m_placeholderVerticalAlignment;
}
@end

@interface SLIBTextAreaHandle : NSScrollView<NSTextViewDelegate>
{	
	@public slib::WeakRef<slib::priv::edit_view::TextAreaInstance> m_viewInstance;
	@public SLIBTextAreaHandle_TextView* m_textView;
}
@end

namespace slib
{

	namespace priv
	{
		namespace edit_view
		{
			
			static NSTextAlignment TranslateAlignment(Alignment _align)
			{
				Alignment align = _align & Alignment::HorizontalMask;
				if (align == Alignment::Center) {
					return NSCenterTextAlignment;
				} else if (align == Alignment::Right) {
					return NSRightTextAlignment;
				}
				return NSLeftTextAlignment;
			}
			
			static NSAttributedString* GenerateHintString(EditView* view)
			{
				Ref<Font> font = view->getHintFont();
				NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
				if (hFont != nil) {
					String _text = view->getHintText();
					if (_text.isNotEmpty()) {
						NSString* text = Apple::getNSStringFromString(_text, @"");
						NSColor* color = GraphicsPlatform::getNSColorFromColor(view->getHintTextColor());
						NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
						[paragraphStyle setAlignment:TranslateAlignment(view->getHintGravity())];
						NSAttributedString* attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle, NSFontAttributeName: hFont}];
						return attr;
					}
				}
				return nil;
			}
			
			class EditViewInstance : public macOS_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				NSTextField* getHandle()
				{
					return (NSTextField*)m_handle;
				}
				
				Ref<EditView> getView()
				{
					return CastRef<EditView>(macOS_ViewInstance::getView());
				}
				
				sl_bool getText(EditView* view, String& _out) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						_out = Apple::getStringFromNSString([handle stringValue]);
						return sl_true;
					}
					return sl_false;
				}
				
				void setText(EditView* view, const String& text) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						NSString* value = Apple::getNSStringFromString(text, @"");
						[handle setStringValue:value];
					}
				}
				
				void setGravity(EditView* view, const Alignment& gravity) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						[handle setAlignment:TranslateAlignment(gravity)];
					}
				}
				
				void setTextColor(EditView* view, const Color& color) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
					}
				}
				
				void setHintText(EditView* view, const String& text) override
				{
					updateHint(view);
				}
				
				void setHintGravity(EditView* view, const Alignment& gravity) override
				{
					updateHint(view);
				}
				
				void setHintTextColor(EditView* view, const Color& color) override
				{
					updateHint(view);
				}
				
				void setHintFont(EditView* view, const Ref<Font>& font) override
				{
					updateHint(view);
				}
				
				void setReadOnly(EditView* view, sl_bool flag) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						[handle setEditable:(flag ? FALSE : TRUE)];
					}
				}
				
				void setPassword(EditView* view, sl_bool flag) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						if (flag) {
							if ([handle.cell isKindOfClass:[NSSecureTextFieldCell class]]) {
								return;
							}
						} else {
							if (!([handle.cell isKindOfClass:[NSSecureTextFieldCell class]])) {
								return;
							}
						}
						apply(view);
					}
				}
				
				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
				}
				
				sl_ui_len measureHeight(EditView* view) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						return (sl_ui_len)([handle fittingSize].height);
					}
					return 0;
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						setHandleFont(handle, font);
						if (EditView* edit = CastInstance<EditView>(view)) {
							updateHint(edit);
						}
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						[handle setBordered:(flag ? YES : NO)];
						[handle setBezeled:(flag ? YES : NO)];
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
					}
				}
				
				void updateHint(EditView* view)
				{
					NSTextField* handle = getHandle();
					if (handle != nil) {
						applyHint(handle, view);
					}
				}
				
				static void applyHint(NSTextField* handle, EditView* view)
				{
					NSAttributedString* str = GenerateHintString(view);
					[[handle cell] setPlaceholderAttributedString:str];
				}
				
				void apply(EditView* view)
				{
					NSTextField* handle = getHandle();
					if (handle == nil) {
						return;
					}
					
					if (view->isPassword()) {
						handle.cell = [[NSSecureTextFieldCell alloc] init];
					} else {
						handle.cell = [[NSTextFieldCell alloc] init];
					}
					
					setHandleFont(handle, view->getFont());
					[handle setStringValue:(Apple::getNSStringFromString(view->getText(), @""))];
					[handle setAlignment:TranslateAlignment(view->getGravity())];
					[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(view->getTextColor()))];
					[handle setBordered: (view->isBorder() ? YES : NO)];
					[handle setBezeled: (view->isBorder() ? YES : NO)];
					[handle setEditable:(view->isReadOnly()? NO : YES)];
					[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(view->getBackgroundColor()))];
					[handle setSelectable:YES];

					applyHint(handle, view);
				}
				
				void onChange(NSTextField* control)
				{
					Ref<EditView> view = getView();
					if (view.isNotNull()) {
						String text = Apple::getStringFromNSString([control stringValue]);
						String textNew = text;
						view->dispatchChange(&textNew);
						if (text != textNew) {
							NSString* str = Apple::getNSStringFromString(textNew, @"");
							[control setStringValue:str];
						}
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(EditViewInstance, macOS_ViewInstance)
			
			class TextAreaInstance : public macOS_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBTextAreaHandle* getHandle()
				{
					return (SLIBTextAreaHandle*)m_handle;
				}
				
				Ref<TextArea> getView()
				{
					return CastRef<TextArea>(macOS_ViewInstance::getView());
				}
				
				sl_bool getText(EditView* view, String& _out) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						_out = Apple::getStringFromNSString([handle->m_textView string]);
						return sl_true;
					}
					return sl_false;
				}
				
				void setText(EditView* view, const String& text) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						NSString* value = Apple::getNSStringFromString(text, @"");
						[handle->m_textView setString:value];
					}
				}
				
				void setGravity(EditView* view, const Alignment& gravity) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle->m_textView setAlignment:TranslateAlignment(gravity)];
					}
				}
				
				void setTextColor(EditView* view, const Color& color) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle->m_textView setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
					}
				}
				
				void setHintText(EditView* view, const String& text) override
				{
					updateHint(view);
				}
				
				void setHintGravity(EditView* view, const Alignment& gravity) override
				{
					updateHint(view);
				}
				
				void setHintTextColor(EditView* view, const Color& color) override
				{
					updateHint(view);
				}
				
				void setHintFont(EditView* view, const Ref<Font>& font) override
				{
					updateHint(view);
				}
				
				void setReadOnly(EditView* view, sl_bool flag) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle->m_textView setEditable:(flag ? FALSE : TRUE)];
					}
				}
				
				void setPassword(EditView* view, sl_bool flag) override
				{
				}
				
				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[[handle->m_textView textContainer] setWidthTracksTextView:(mode == MultiLineMode::WordWrap || mode == MultiLineMode::BreakWord) ? YES : NO];
						[[handle->m_textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
					}
				}
				
				sl_ui_len measureHeight(EditView* view) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						NSTextView* tv = handle->m_textView;
						NSLayoutManager* layoutManager = tv.layoutManager;
						NSTextContainer* textContainer = tv.textContainer;
						[layoutManager ensureLayoutForTextContainer:textContainer];
						NSRect usedRect = [layoutManager usedRectForTextContainer:textContainer];
						NSSize inset = [tv textContainerInset];
						return (sl_ui_len)(usedRect.size.height + inset.height * 2) + 4;
					}
					return 0;
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						updateFont(handle->m_textView, font, sl_true);
						if (EditView* edit = CastInstance<EditView>(view)) {
							updateHint(edit);
						}
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setBorderType:(flag ? NSBezelBorder : NSNoBorder)];
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle->m_textView setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
					}
				}
				
				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setHasHorizontalScroller:(flagHorizontal ? YES : NO)];
						[handle setHasVerticalScroller:(flagVertical ? YES : NO)];
					}
				}

				void updateHint(EditView* view)
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						applyHint(handle, view);
					}
				}
				
				static void applyHint(SLIBTextAreaHandle* handle, EditView* view)
				{
					NSAttributedString* str = GenerateHintString(view);
					handle->m_textView->m_placeholderString = str;
					handle->m_textView->m_placeholderVerticalAlignment = view->getHintGravity() & Alignment::VerticalMask;
					if (view->getText().isEmpty() && str != nil) {
						[handle setNeedsDisplay:YES];
					}
				}
				
				static void updateFont(NSTextView* tv, const Ref<Font>& font, sl_bool flagSet)
				{
					if (font.isNull()) {
						return;
					}
					NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
					if (font == nil) {
						return;
					}
					[tv setFont:hFont];
					NSMutableParagraphStyle *paragraph = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
					CGFloat height = hFont.leading + hFont.ascender - hFont.descender;
					[paragraph setLineSpacing:2];
					[paragraph setMaximumLineHeight:height];
					[paragraph setMinimumLineHeight:height];
					[tv setDefaultParagraphStyle:paragraph];
					if (flagSet) {
						NSMutableAttributedString* text = (NSMutableAttributedString*)([tv attributedString]);
						[text addAttribute:NSParagraphStyleAttributeName value:paragraph range:NSMakeRange(0, text.length)];
					}
				}
				
				void apply(EditView* view)
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle == nil) {
						return;
					}

					[handle setHasHorizontalScroller:(view->isHorizontalScrollBarVisible()?YES:NO)];
					[handle setHasVerticalScroller:(view->isVerticalScrollBarVisible()?YES:NO)];
					
					SLIBTextAreaHandle_TextView* tv = handle->m_textView;
					
					[[tv textContainer] setWidthTracksTextView:(view->getMultiLine() == MultiLineMode::WordWrap || view->getMultiLine() == MultiLineMode::BreakWord) ? YES : NO];
					[[tv textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
					
					updateFont(tv, view->getFont(), sl_false);
					
					[handle setBorderType:(view->isBorder() ? NSBezelBorder : NSNoBorder)];
					[tv setString:Apple::getNSStringFromString(view->getText(), @"")];
					[tv setAlignment:TranslateAlignment(view->getGravity())];
					[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(view->getTextColor()))];
					[tv setEditable:(view->isReadOnly() ? NO : YES)];
					[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(view->getBackgroundColor()))];
					[tv setSelectable:YES];
					
					tv->m_viewInstance = this;

					applyHint(handle, view);
				}
			
				void onChange(SLIBTextAreaHandle* control)
				{
					Ref<TextArea> view = getView();
					if (view.isNotNull()) {
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
			
			SLIB_DEFINE_OBJECT(TextAreaInstance, macOS_ViewInstance)
			
		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		Ref<EditViewInstance> ret = macOS_ViewInstance::create<EditViewInstance, SLIBEditViewHandle>(this, parent);
		if (ret.isNotNull()) {
			ret->apply(this);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IEditViewInstance> EditView::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}
	
	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
	{
		Ref<TextAreaInstance> ret = macOS_ViewInstance::create<TextAreaInstance, SLIBTextAreaHandle>(this, parent);
		if (ret.isNotNull()) {
			ret->apply(this);
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IEditViewInstance> TextArea::getEditViewInstance()
	{
		return CastRef<TextAreaInstance>(getViewInstance());
	}

}

using namespace slib;
using namespace slib::priv::edit_view;

@implementation SLIBEditViewHandle

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setDelegate:self];
	}
	return self;
}

- (void)controlTextDidChange:(NSNotification *)obj
{
	Ref<EditViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onChange(self);
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
		[m_textView setAllowsUndo:YES];
		
		[self setDocumentView:m_textView];
		
		[m_textView setDelegate:self];
	}
	return self;
}

-(void)textDidChange:(NSNotification *)obj
{
	Ref<TextAreaInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onChange(self);
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
	return [super becomeFirstResponder];
}

- (void)drawRect:(NSRect)rect
{
	[super drawRect:rect];
	if (self->m_placeholderString != nil) {
		if ([[self string] length] == 0) {
			NSSize inset = [self textContainerInset];
			CGFloat paddingLeft = inset.width;
			CGFloat paddingRight = inset.width;
			NSTextContainer* container = [self textContainer];
			if (container != nil) {
				CGFloat f = [container lineFragmentPadding];
				paddingLeft += f;
				paddingRight += f;
			}
			NSParagraphStyle* style = [self defaultParagraphStyle];
			if (style != nil) {
				paddingLeft += [style firstLineHeadIndent];
				paddingLeft += [style headIndent];
				paddingRight += [style tailIndent];
			}
			NSRect rect = [self bounds];
			rect.origin.x += paddingLeft;
			rect.size.width -= paddingLeft + paddingRight;
			if (self->m_placeholderVerticalAlignment == Alignment::Top) {
				[self->m_placeholderString drawInRect:rect];
			} else if (self->m_placeholderVerticalAlignment == Alignment::Bottom) {
				CGSize size = [self->m_placeholderString size];
				rect.origin.y += rect.size.height - size.height;
				rect.size.height = size.height;
				[self->m_placeholderString drawInRect:rect];
			} else {
				CGSize size = [self->m_placeholderString size];
				rect.origin.y += (rect.size.height - size.height) / 2;
				rect.size.height = size.height;
				[self->m_placeholderString drawInRect:rect];
			}
		}
	}
}

@end

#endif
