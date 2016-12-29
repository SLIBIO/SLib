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
	static ::UIReturnKeyType convertReturnKeyType(UIReturnKeyType type)
	{
		switch (type) {
			case UIReturnKeyType::Search:
				return UIReturnKeySearch;
			case UIReturnKeyType::Next:
				return UIReturnKeyNext;
			case UIReturnKeyType::Continue:
				return UIReturnKeyContinue;
			case UIReturnKeyType::Go:
				return UIReturnKeyGo;
			case UIReturnKeyType::Send:
				return UIReturnKeySend;
			case UIReturnKeyType::Join:
				return UIReturnKeyJoin;
			case UIReturnKeyType::Route:
				return UIReturnKeyRoute;
			case UIReturnKeyType::EmergencyCall:
				return UIReturnKeyEmergencyCall;
			case UIReturnKeyType::Google:
				return UIReturnKeyGoogle;
			case UIReturnKeyType::Yahoo:
				return UIReturnKeyYahoo;
			case UIReturnKeyType::Return:
				return UIReturnKeyDefault;
			case UIReturnKeyType::Default:
			case UIReturnKeyType::Done:
			default:
				return UIReturnKeyDone;
		}
	}
	
	static ::UIKeyboardType convertKeyboardType(UIKeyboardType type)
	{
		switch (type) {
			case UIKeyboardType::Numpad:
				return UIKeyboardTypeNumberPad;
			case UIKeyboardType::Phone:
				return UIKeyboardTypePhonePad;
			case UIKeyboardType::Email:
				return UIKeyboardTypeEmailAddress;
			case UIKeyboardType::Alphabet:
				return UIKeyboardTypeAlphabet;
			case UIKeyboardType::Url:
				return UIKeyboardTypeURL;
			case UIKeyboardType::WebSearch:
				return UIKeyboardTypeWebSearch;
			case UIKeyboardType::Twitter:
				return UIKeyboardTypeTwitter;
			case UIKeyboardType::NumbersAndPunctation:
				return UIKeyboardTypeNumbersAndPunctuation;
			case UIKeyboardType::NamePhone:
				return UIKeyboardTypeNamePhonePad;
			case UIKeyboardType::Ascii:
				return UIKeyboardTypeASCIICapable;
			case UIKeyboardType::AsciiNumpad:
				return UIKeyboardTypeASCIICapableNumberPad;
			case UIKeyboardType::Default:
			default:
				return UIKeyboardTypeDefault;
		}
	}
	
#define ADD_TOOL_BAR(handle) UIToolbar *toolbar = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, 0, 44)];\
	toolbar.barTintColor = [UIColor colorWithRed:181.0/255 green:187.0/255 blue:193.0/255 alpha:1];\
	UIBarButtonItem *flex = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:handle action:nil];\
	UIBarButtonItem* barButtonDone;\
	barButtonDone = [[UIBarButtonItem alloc] initWithTitle:@"Done" style:UIBarButtonItemStylePlain target:handle action:@selector(resignFirstResponder)];\
	toolbar.items = [NSArray arrayWithObjects:flex, flex, barButtonDone, nil];\
	handle.inputAccessoryView = toolbar;

	void applyProperties(UITextField* handle)
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				applyProperties(handle);
			});
			return;
		}
		
		ADD_TOOL_BAR(handle)
		
		[handle setText:(Apple::getNSStringFromString(m_text))];
		[handle setTextAlignment:translateAlignment(m_textAlignment)];
		[handle setBorderStyle:(isBorder()?UITextBorderStyleBezel:UITextBorderStyleNone)];
		[handle setPlaceholder:(Apple::getNSStringFromString(m_hintText))];
		[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
		[handle setEnabled:(m_flagReadOnly ? NO : YES)];
		Ref<Font> font = getFont();
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[handle setFont:hFont];
		}
		[handle setReturnKeyType:convertReturnKeyType(m_returnKeyType)];
		[handle setKeyboardType:convertKeyboardType(m_keyboardType)];
	}
	
	void applyProperties(UITextView* handle)
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				applyProperties(handle);
			});
			return;
		}
		
		ADD_TOOL_BAR(handle)
		
		[handle setText:(Apple::getNSStringFromString(m_text))];
		[handle setTextAlignment:translateAlignment(m_textAlignment)];
		if (isBorder()) {
			[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
			[handle.layer setBorderWidth:1];
		} else {
			[handle.layer setBorderWidth:0];
		}
		[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
		[handle setEditable:(m_flagReadOnly?FALSE:TRUE)];
		[handle setSelectable:TRUE];
		Ref<Font> font = getFont();
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[handle setFont:hFont];
		}
		[handle setReturnKeyType:convertReturnKeyType(m_returnKeyType)];
		[handle setKeyboardType:convertKeyboardType(m_keyboardType)];
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
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSTextAlignmentCenter) {
			return Alignment::Center;
		} else if (align == NSTextAlignmentRight) {
			return Alignment::Right;
		}
		return Alignment::Left;
	}
	
	static void onChangeText(iOS_ViewInstance* instance, UITextField* field, UITextView* area)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view.ptr)) {
			_EditView* view = (_EditView*)(_view.ptr);
			String text;
			if (field != nil) {
				text = Apple::getStringFromNSString([field text]);
			} else if (area != nil) {
				text = Apple::getStringFromNSString([area text]);
			}
			String textNew = view->dispatchChange(text);
			if (text != textNew) {
				NSString* str = Apple::getNSStringFromString(textNew);
				if (field != nil) {
					[field setText:str];
				}
				if (area != nil) {
					[area setText:str];
				}
			}
		}
	}
	
	static void onEnterAction(iOS_ViewInstance* instance, UITextField* field, UITextView* area)
	{
		Ref<View> _view = instance->getView();
		if (EditView::checkInstance(_view.ptr)) {
			_EditView* view = (_EditView*)(_view.ptr);
			view->dispatchEnterAction();
			if (view->isAutoDismissKeyboard()) {
				if (field != nil) {
					[field resignFirstResponder];
				}
				if (area != nil) {
					[area resignFirstResponder];
				}
			}
		}
	}
};

Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_TextField* handle = [[_Slib_iOS_TextField alloc] initWithFrame:frame];

	if (handle != nil) {
		((_EditView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* _parent)
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

Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_TextArea* handle = [[_Slib_iOS_TextArea alloc] initWithFrame:frame];
	if (handle != nil) {
		((_EditView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}


void EditView::_getText_NW()
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
}

void EditView::_setText_NW(const String& value)
{
	if (![NSThread isMainThread]) {
		String _value = value;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setText_NW(_value);
		});
		return;
	}
	
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
}

void EditView::_setBorder_NW(sl_bool flag)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setBorder_NW(flag);
		});
		return;
	}
	
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
}

void EditView::_getTextAlignment_NW()
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
}

void EditView::_setTextAlignment_NW(Alignment align)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setTextAlignment_NW(align);
		});
		return;
	}
	
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
}

void EditView::_getHintText_NW()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			m_hintText = Apple::getStringFromNSString(tv.placeholder);
		}
	}
}

void EditView::_setHintText_NW(const String& value)
{
	if (![NSThread isMainThread]) {
		String _value = value;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setHintText_NW(_value);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			NSString* s = Apple::getNSStringFromString(value);
			[tv setPlaceholder:s];
		}
	}
}

void EditView::_isReadOnly_NW()
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
}

void EditView::_setReadOnly_NW(sl_bool flag)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setReadOnly_NW(flag);
		});
		return;
	}
	
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
}

void EditView::_isMultiLine_NW()
{
}

void EditView::_setMultiLine_NW(sl_bool flag)
{
}

void EditView::_setTextColor_NW(const Color& color)
{
	if (![NSThread isMainThread]) {
		Color _color = color;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setTextColor_NW(_color);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
		}
	}
}

void EditView::_setBackgroundColor_NW(const Color& color)
{
	if (![NSThread isMainThread]) {
		Color _color = color;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setBackgroundColor_NW(_color);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
		}
	}
}

void EditView::_setFont_NW(const Ref<Font>& font)
{
	if (![NSThread isMainThread]) {
		Ref<Font> _font = font;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setFont_NW(_font);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
	}
}

void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setReturnKeyType_NW(type);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setReturnKeyType:_EditView::convertReturnKeyType(type)];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setReturnKeyType:_EditView::convertReturnKeyType(type)];
		}
	}
}

void EditView::_setKeyboardType_NW(UIKeyboardType type)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_setKeyboardType_NW(type);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[UITextField class]]) {
			UITextField* tv = (UITextField*)handle;
			[tv setKeyboardType:_EditView::convertKeyboardType(type)];
		} else if ([handle isKindOfClass:[UITextView class]]) {
			UITextView* tv = (UITextView*)handle;
			[tv setKeyboardType:_EditView::convertKeyboardType(type)];
		}
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_TextField
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self addTarget:self action:@selector(textFieldDidChange) forControlEvents:UIControlEventEditingChanged];
		[self setDelegate:self];
	}
	return self;
}

-(void)textFieldDidChange
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeText(instance.ptr, self, nil);
	}
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
}

-(BOOL) textFieldShouldReturn:(UITextField *)textField{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onEnterAction(instance.ptr, self, nil);
	}
	return NO;
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
-(void)keyboardWillShow {
	
}
-(void)textViewDidChange:(UITextView *)textView
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeText(instance.ptr, nil, self);
	}
}
@end

#endif
