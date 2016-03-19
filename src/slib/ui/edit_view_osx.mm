#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/edit_view.h"

#include "view_osx.h"

@interface _Slib_OSX_TextField : NSTextField<NSTextFieldDelegate> {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
	@public sl_bool m_flagMultiLine;
	
}
@end

@interface _Slib_OSX_SecureTextField : NSSecureTextField<NSTextFieldDelegate> {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}
@end

@interface _Slib_OSX_TextArea_TextView : NSTextView {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}
@end

@interface _Slib_OSX_TextArea : NSScrollView<NSTextViewDelegate> {
	
	@public _Slib_OSX_TextArea_TextView* textView;
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _EditView : public EditView
{
public:
	void __applyProperties(NSTextField* handle)
	{
		[handle setStringValue:(Apple::getNSStringFromString(m_text))];
		[handle setAlignment:translateAlignment(m_textAlignment)];
		[handle setBordered: (m_flagBorder ? TRUE : FALSE)];
		[handle setBezeled: (m_flagBorder ? TRUE : FALSE)];
		[[handle cell] setPlaceholderString:(Apple::getNSStringFromString(m_hintText))];
		[handle setTextColor:(UIPlatform::getNSColorFromColor(m_textColor))];
		[handle setBackgroundColor:(UIPlatform::getNSColorFromColor(m_backgroundColor))];
		[handle setEditable:(m_flagReadOnly?FALSE:TRUE)];
		[handle setSelectable:TRUE];
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	void __applyProperties(_Slib_OSX_TextArea* handle)
	{
		NSTextView* tv = handle->textView;
		[tv setString:(Apple::getNSStringFromString(m_text))];
		[tv setAlignment:translateAlignment(m_textAlignment)];
		[handle setBorderType:(m_flagBorder ? NSBezelBorder : NSNoBorder)];
		[tv setTextColor:(UIPlatform::getNSColorFromColor(m_textColor))];
		[tv setBackgroundColor:(UIPlatform::getNSColorFromColor(m_backgroundColor))];
		[tv setEditable:(m_flagReadOnly?FALSE:TRUE)];
		[tv setSelectable:TRUE];
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[tv setFont:hFont];
		}
	}
	
	static NSTextAlignment translateAlignment(Alignment _align)
	{
		Alignment align = (Alignment)((int)_align & (int)Alignment::HorizontalMask);
		if (align == Alignment::Center) {
			return NSCenterTextAlignment;
		} else if (align == Alignment::Right) {
			return NSRightTextAlignment;
		}
		return NSLeftTextAlignment;
	}
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSCenterTextAlignment) {
			return Alignment::Center;
		} else if (align == NSRightTextAlignment) {
			return Alignment::Right;
		}
		return Alignment::Left;
	}
	
	static void onChangeTextField(OSX_ViewInstance* instance, NSTextField* control)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view.ptr)) {
			_EditView* view = (_EditView*)(_view.ptr);
			String text = Apple::getStringFromNSString([control stringValue]);
			String textNew = view->dispatchChange(text);
			if (text != textNew) {
				NSString* str = Apple::getNSStringFromString(textNew);
				[control setStringValue:str];
			}
		}
	}
	
	static void onChangeTextArea(OSX_ViewInstance* instance, _Slib_OSX_TextArea* control)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view.ptr)) {
			_EditView* view = (_EditView*)(_view.ptr);
			String text = Apple::getStringFromNSString([control->textView string]);
			String textNew = view->dispatchChange(text);
			if (text != textNew) {
				NSString* str = Apple::getNSStringFromString(textNew);
				[control->textView setString:str];
			}
		}
	}

};

Ref<ViewInstance> EditView::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_TextField* handle = [[_Slib_OSX_TextField alloc] initWithFrame:frame];
	if (handle != nil) {
		handle->m_flagMultiLine = m_flagMultiLine;
		((_EditView*)this)->__applyProperties(handle);
		[handle setDelegate:handle];
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

Ref<ViewInstance> PasswordView::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_SecureTextField* handle = [[_Slib_OSX_SecureTextField alloc] initWithFrame:frame];
	if (handle != nil) {
		((_EditView*)this)->__applyProperties(handle);
		[handle setDelegate:handle];
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

Ref<ViewInstance> TextArea::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_TextArea* handle = [[_Slib_OSX_TextArea alloc] initWithFrame:frame];
	if (handle != nil) {
		((_EditView*)this)->__applyProperties(handle);
	}
	OSX_VIEW_CREATE_INSTANCE_END
	if (handle != nil) {
		handle->textView->m_viewInstance = ret;
	}
	return ret;
}

String EditView::getText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			NSString* s = [tv stringValue];
			m_text = Apple::getStringFromNSString(s);
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			NSString* s = [tv->textView string];
			m_text = Apple::getStringFromNSString(s);
		}
	}
	return m_text;
}

void EditView::setText(const String& value)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setStringValue:(Apple::getNSStringFromString(value))];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv->textView setString:(Apple::getNSStringFromString(value))];
		}
	}
	m_text = value;
}

sl_bool EditView::isBorder()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			m_flagBorder = tv.bordered ? sl_true : sl_false;
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			m_flagBorder = tv.borderType != NSNoBorder ? sl_true : sl_false;
		}
	}
	return m_flagBorder;
}

void EditView::setBorder(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBordered:(flag?TRUE:FALSE)];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
		}
	}
	m_flagBorder = flag;
}

Alignment EditView::getTextAlignment()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			m_textAlignment = _EditView::translateAlignmentReverse(tv.alignment);
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			m_textAlignment = _EditView::translateAlignmentReverse(tv->textView.alignment);
		}
	}
	return m_textAlignment;
}

void EditView::setTextAlignment(Alignment align)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setAlignment:_EditView::translateAlignment(align)];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv->textView setAlignment:_EditView::translateAlignment(align)];
		}
	}
	m_textAlignment = align;
}

String EditView::getHintText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		NSTextFieldCell* cell = [tv cell];
		m_hintText = Apple::getStringFromNSString(cell.placeholderString);
	}
	return m_hintText;
}

void EditView::setHintText(const String& value)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		NSString* s = Apple::getNSStringFromString(value);
		[[tv cell] setPlaceholderString:s];
	}
	m_hintText = value;
}

sl_bool EditView::isReadOnly()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			m_flagReadOnly = tv.editable ? sl_false : sl_true;
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			m_flagReadOnly = tv->textView.editable ? sl_false : sl_true;
		}
	}
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setEditable:(flag ? FALSE : TRUE)];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv->textView setEditable:(flag ? FALSE : TRUE)];
		}
	}
	m_flagReadOnly = flag;
}

sl_bool EditView::isMultiLine()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TextField class]]) {
		_Slib_OSX_TextField* tv = (_Slib_OSX_TextField*)handle;
		m_flagMultiLine = tv->m_flagMultiLine;
	}
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TextField class]]) {
		_Slib_OSX_TextField* tv = (_Slib_OSX_TextField*)handle;
		tv->m_flagMultiLine = flag;
	}
	m_flagMultiLine = flag;
}

Color EditView::getTextColor()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			m_textColor = UIPlatform::getColorFromNSColor(tv.textColor);
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			m_textColor = UIPlatform::getColorFromNSColor(tv->textView.textColor);
		}
	}
	return m_textColor;
}

void EditView::setTextColor(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setTextColor:(UIPlatform::getNSColorFromColor(color))];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv->textView setTextColor:(UIPlatform::getNSColorFromColor(color))];
		}
	}
	m_textColor = color;
}

Color EditView::getBackgroundColor()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			m_backgroundColor = UIPlatform::getColorFromNSColor(tv.backgroundColor);
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			m_backgroundColor = UIPlatform::getColorFromNSColor(tv->textView.backgroundColor);
		}
	}
	return m_backgroundColor;
}

void EditView::setBackgroundColor(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBackgroundColor:(UIPlatform::getNSColorFromColor(color))];
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			[tv->textView setBackgroundColor:(UIPlatform::getNSColorFromColor(color))];
		}
	}
	m_backgroundColor = color;
}

void EditView::setFont(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			Ref<FontInstance> fontInstance;
			NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
			_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
			Ref<FontInstance> fontInstance;
			NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
			if (hFont != nil) {
				[tv->textView setFont:hFont];
			}
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_TextField
-(void)controlTextDidChange:(NSNotification *)obj
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextField(instance.ptr, self);
	}
}

- (BOOL)control:(NSControl*)control textView:(NSTextView*)textView doCommandBySelector:(SEL)commandSelector
{
	BOOL result = NO;
	if (m_flagMultiLine) {
		if (commandSelector == @selector(insertNewline:)) {
			[textView insertNewlineIgnoringFieldEditor:self];
			result = YES;
		}
		else if (commandSelector == @selector(insertTab:)) {
			[textView insertTabIgnoringFieldEditor:self];
			result = YES;
		}
	}
	return result;
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyUp:theEvent];
}
@end

@implementation _Slib_OSX_SecureTextField
-(void)controlTextDidChange:(NSNotification *)obj
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextField(instance.ptr, self);
	}
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyUp:theEvent];
}
@end

@implementation _Slib_OSX_TextArea
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		textView = [[_Slib_OSX_TextArea_TextView alloc] init];
		if (textView == nil) {
			return nil;
		}
		[textView setMinSize:NSMakeSize(0.0, 0.0)];
		[textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		[textView setVerticallyResizable:YES];
		[textView setHorizontallyResizable:YES];
		[textView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		
		[[textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
		
		[self setDocumentView:textView];
		[self setHasVerticalScroller:TRUE];
		[self setHasHorizontalScroller:TRUE];
		
		[textView setDelegate:self];
	}
	return self;
}

-(void)controlTextDidChange:(NSNotification *)obj
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextArea(instance.ptr, self);
	}
}
@end

@implementation _Slib_OSX_TextArea_TextView

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyUp:theEvent];
}

@end

#endif
