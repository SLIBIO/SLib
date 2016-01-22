#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/edit_view.h"

#include "view_ios.h"

@interface _Slib_iOS_TextField : UITextField<UITextFieldDelegate> {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
}
@end

@interface _Slib_iOS_TextArea : UITextView<UITextViewDelegate> {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _EditView : public EditView
{
public:
	void applyProperties(UITextField* handle)
	{
		[handle setText:(Apple::getNSStringFromString(m_text))];
		[handle setTextAlignment:translateAlignment(m_textAlignment)];
		[handle setBorderStyle:(m_flagBorder?UITextBorderStyleBezel:UITextBorderStyleNone)];
		[handle setPlaceholder:(Apple::getNSStringFromString(m_hintText))];
		[handle setTextColor:(UIPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(UIPlatform::getUIColorFromColor(m_backgroundColor))];
		[handle setEnabled:(m_flagReadOnly ? NO : YES)];
		Ref<FontInstance> fontInstance;
		Ref<Font> font = m_font;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	void applyProperties(UITextView* handle)
	{
		[handle setText:(Apple::getNSStringFromString(m_text))];
		[handle setTextAlignment:translateAlignment(m_textAlignment)];
		if (m_flagBorder) {
			[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
			[handle.layer setBorderWidth:1];
		} else {
			[handle.layer setBorderWidth:0];
		}
		[handle setTextColor:(UIPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(UIPlatform::getUIColorFromColor(m_backgroundColor))];
		[handle setEditable:(m_flagReadOnly?FALSE:TRUE)];
		[handle setSelectable:TRUE];
		Ref<FontInstance> fontInstance;
		Ref<Font> font = m_font;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	static NSTextAlignment translateAlignment(Alignment _align)
	{
		sl_uint32 align = _align & alignHorizontalMask;
		if (align == alignCenter) {
			return NSTextAlignmentCenter;
		} else if (align == alignRight) {
			return NSTextAlignmentRight;
		}
		return NSTextAlignmentLeft;
	}
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSTextAlignmentCenter) {
			return alignCenter;
		} else if (align == NSTextAlignmentRight) {
			return alignRight;
		}
		return alignLeft;
	}
	
	static void onChangeTextField(iOS_ViewInstance* instance, UITextField* control)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view)) {
			_EditView* view = (_EditView*)(_view.get());
			String text = Apple::getStringFromNSString([control text]);
			String textNew = view->dispatchChange(text);
			if (text != textNew) {
				NSString* str = Apple::getNSStringFromString(textNew);
				[control setText:str];
			}
		}
	}
	
	static void onChangeTextArea(iOS_ViewInstance* instance, UITextView* control)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view)) {
			_EditView* view = (_EditView*)(_view.get());
			String text = Apple::getStringFromNSString([control text]);
			String textNew = view->dispatchChange(text);
			if (text != textNew) {
				NSString* str = Apple::getNSStringFromString(textNew);
				[control setText:str];
			}
		}
	}
};

Ref<ViewInstance> EditView::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_TextField* handle = [[_Slib_iOS_TextField alloc] initWithFrame:frame];
	if (handle != nil) {
		((_EditView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

Ref<ViewInstance> PasswordView::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_TextField* handle = [[_Slib_iOS_TextField alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.secureTextEntry = TRUE;
		((_EditView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

Ref<ViewInstance> TextArea::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_TextArea* handle = [[_Slib_iOS_TextArea alloc] initWithFrame:frame];
	if (handle != nil) {
		((_EditView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}


String EditView::getText()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			NSString* s = [tv text];
			m_text = Apple::getStringFromNSString(s);
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			NSString* s = [tv text];
			m_text = Apple::getStringFromNSString(s);
		}
	}
	return m_text;
}

void EditView::setText(const String& value)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setText:(Apple::getNSStringFromString(value))];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setText:(Apple::getNSStringFromString(value))];
		}
	}
	m_text = value;
}

sl_bool EditView::isBorder()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_flagBorder = tv.borderStyle != UITextBorderStyleNone;
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			m_flagBorder = [tv.layer borderWidth] > 0;
		}
	}
	return m_flagBorder;
}

void EditView::setBorder(sl_bool flag)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (flag) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setBorderStyle:(flag?UITextBorderStyleBezel:UITextBorderStyleNone)];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			if (flag) {
				[tv.layer setBorderColor:([[UIColor grayColor] CGColor])];
				[tv.layer setBorderWidth:1];
			} else {
				[tv.layer setBorderWidth:0];
			}
		}
	}
	m_flagBorder = flag;
}

Alignment EditView::getTextAlignment()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_textAlignment = _EditView::translateAlignmentReverse([tv textAlignment]);
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			m_textAlignment = _EditView::translateAlignmentReverse([tv textAlignment]);
		}
	}
	return m_textAlignment;
}

void EditView::setTextAlignment(Alignment align)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setTextAlignment:_EditView::translateAlignment(align)];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setTextAlignment:_EditView::translateAlignment(align)];
		}
	}
	m_textAlignment = align;
}

String EditView::getHintText()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_hintText = Apple::getStringFromNSString(tv.placeholder);
		}
	}
	return m_hintText;
}

void EditView::setHintText(const String& value)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			NSString* s = Apple::getNSStringFromString(value);
			[tv setPlaceholder:s];
		}
	}
	m_hintText = value;
}

sl_bool EditView::isReadOnly()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_flagReadOnly = tv.enabled ? FALSE : TRUE;
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			m_flagReadOnly = tv.editable ? FALSE : TRUE;
		}
	}
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setEnabled:(flag?NO:YES)];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setEditable:(flag?NO:YES)];
		}
	}
	m_flagReadOnly = flag;
}

sl_bool EditView::isMultiLine()
{
	m_flagMultiLine = sl_false;
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag)
{
	m_flagMultiLine = sl_false;
}

Color EditView::getTextColor()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_textColor = UIPlatform::getColorFromUIColor(tv.textColor);
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			m_textColor = UIPlatform::getColorFromUIColor(tv.textColor);
		}
	}
	return m_textColor;
}

void EditView::setTextColor(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setTextColor:(UIPlatform::getUIColorFromColor(color))];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setTextColor:(UIPlatform::getUIColorFromColor(color))];
		}
	}
	m_textColor = color;
}

Color EditView::getBackgroundColor()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_backgroundColor = UIPlatform::getColorFromUIColor(tv.backgroundColor);
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			m_backgroundColor = UIPlatform::getColorFromUIColor(tv.backgroundColor);
		}
	}
	return m_backgroundColor;
}

void EditView::setBackgroundColor(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setBackgroundColor:(UIPlatform::getUIColorFromColor(color))];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setBackgroundColor:(UIPlatform::getUIColorFromColor(color))];
		}
	}
	m_backgroundColor = color;
}

void EditView::setFont(const Ref<Font>& font)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			Ref<FontInstance> fontInstance;
			UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			Ref<FontInstance> fontInstance;
			UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_TextField
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self addTarget:self action:@selector(textFieldDidChange) forControlEvents:UIControlEventEditingChanged];
	}
	return self;
}

-(void)textFieldDidChange
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextField(instance.get(), self);
	}
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
}

-(BOOL) textFieldShouldReturn:(UITextField *)textField{
	
	[textField resignFirstResponder];
	return YES;
}
@end

@implementation _Slib_iOS_TextArea
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setScrollEnabled:TRUE];
		[self setDelegate:self];
	}
	return self;
}

-(void)textViewDidChange:(UITextView *)textView
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextArea(instance.get(), self);
	}
}
@end

#endif
