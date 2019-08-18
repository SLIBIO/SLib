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

#include "slib/ui/edit_view.h"

#include "view_ios.h"

@interface SLIBEditViewHandle : UITextField<UITextFieldDelegate>
{	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;	
}
@end

@interface SLIBTextAreaHandle : UITextView<UITextViewDelegate>
{	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	@public UILabel* m_labelHintText;
	@public slib::Alignment m_hintTextVerticalAlignment;
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
						case UIKeyboardType::NumbersAndPunctuation:
						case UIKeyboardType::Decimal:
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
				
				static ::UITextAutocapitalizationType convertAutoCapitalizationType(UIAutoCapitalizationType type)
				{
					switch (type) {
						case UIAutoCapitalizationType::None :
							return UITextAutocapitalizationTypeNone;
						case UIAutoCapitalizationType::Words:
							return UITextAutocapitalizationTypeWords;
						case UIAutoCapitalizationType::Sentences:
							return UITextAutocapitalizationTypeSentences;
						case UIAutoCapitalizationType::AllCharacters:
							return UITextAutocapitalizationTypeAllCharacters;
						default:
							return UITextAutocapitalizationTypeSentences;
					}
				}

				void applyPlaceholder(UITextField* handle)
				{
					NSAttributedString* attr;
					String _text = m_hintText;
					if (_text.isEmpty()) {
						attr = nil;
					} else {
						UIFont* hFont = nil;
						Ref<Font> font = getHintFont();
						if (font.isNotNull()) {
							hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
						}
						NSString* text = Apple::getNSStringFromString(_text);
						UIColor* color = GraphicsPlatform::getUIColorFromColor(m_hintTextColor);
						NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
						[paragraphStyle setAlignment:translateAlignment(m_hintTextAlignment)];
						if (hFont != nil) {
							attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle, NSFontAttributeName: hFont}];
						} else {
							attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle}];
						}
					}
					[handle setAttributedPlaceholder: attr];
				}
				
				void applyPlaceholder(SLIBTextAreaHandle* handle)
				{
					String text = m_hintText;
					if (text.isEmpty()) {
						if (handle->m_labelHintText != nil) {
							handle->m_labelHintText.alpha = 0;
						}
					} else {
						UILabel* label = handle->m_labelHintText;
						if (label == nil) {
							label = [[UILabel alloc] initWithFrame:CGRectMake(8, 8, handle.bounds.size.width - 16, 0)];
							label.lineBreakMode = NSLineBreakByWordWrapping;
							label.numberOfLines = 0;
							label.backgroundColor = [UIColor clearColor];
							[handle addSubview:label];
							handle->m_labelHintText = label;
						}
						label.text = Apple::getNSStringFromString(text);
						label.textAlignment = translateAlignment(m_hintTextAlignment);
						label.textColor = GraphicsPlatform::getUIColorFromColor(m_hintTextColor);
						Ref<Font> font = getHintFont();
						if (font.isNotNull()) {
							UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
							if (hFont != nil) {
								label.font = hFont;
							}
						}
						label.alpha = 1;
						handle->m_hintTextVerticalAlignment = m_hintTextAlignment & Alignment::VerticalMask;
						setHintLabelFrame(handle, label);
						[handle sendSubviewToBack:label];
					}
				}
				
				static void setHintLabelFrame(SLIBTextAreaHandle* handle, UILabel* label)
				{
					if (label == nil) {
						return;
					}
					[label setFrame:CGRectMake(8, 8, handle.bounds.size.width - 16, handle.frame.size.height - 16)];
					if (handle->m_hintTextVerticalAlignment == Alignment::Top) {
						[label sizeToFit];
						[label setFrame:CGRectMake(8, 8, handle.bounds.size.width - 16, label.bounds.size.height)];
					} else if (handle->m_hintTextVerticalAlignment == Alignment::Bottom) {
						[label sizeToFit];
						[label setFrame:CGRectMake(8, handle.frame.size.height - 8 - label.bounds.size.height, handle.bounds.size.width - 16, label.bounds.size.height)];
					}
				}
				
				void applyProperties(UITextField* handle)
				{
					[handle setText:(Apple::getNSStringFromString(m_text))];
					[handle setTextAlignment:translateAlignment(m_textAlignment)];
					[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
					[handle setBorderStyle:(isBorder()?UITextBorderStyleRoundedRect:UITextBorderStyleNone)];
					[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
					[handle setEnabled:(m_flagReadOnly ? NO : YES)];
					[handle setSecureTextEntry:(m_flagPassword ? YES : NO)];
					[handle setReturnKeyType:convertReturnKeyType(m_returnKeyType)];
					[handle setKeyboardType:convertKeyboardType(m_keyboardType)];
					[handle setAutocapitalizationType:convertAutoCapitalizationType(m_autoCapitalizationType)];
					Ref<Font> font = getFont();
					UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
					if (hFont != nil) {
						[handle setFont:hFont];
					}
					applyPlaceholder(handle);
				}
				
				void applyProperties(SLIBTextAreaHandle* handle)
				{
					[handle setScrollEnabled:!(isHeightWrapping())];
					[handle setShowsHorizontalScrollIndicator:isHorizontalScrollBarVisible()?YES:NO];
					[handle setShowsVerticalScrollIndicator:(isVerticalScrollBarVisible() && !(isHeightWrapping()))?YES:NO];
					[handle setText:(Apple::getNSStringFromString(m_text))];
					[handle setTextAlignment:translateAlignment(m_textAlignment)];
					[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
					if (isBorder()) {
						[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
						[handle.layer setBorderWidth:1];
					} else {
						[handle.layer setBorderWidth:0];
					}
					[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
					[handle setEditable:(m_flagReadOnly?FALSE:TRUE)];
					[handle setSelectable:TRUE];
					[handle setReturnKeyType:convertReturnKeyType(m_returnKeyType)];
					[handle setKeyboardType:convertKeyboardType(m_keyboardType)];
					[handle setAutocapitalizationType:convertAutoCapitalizationType(m_autoCapitalizationType)];
					Ref<Font> font = getFont();
					UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
					if (hFont != nil) {
						[handle setFont:hFont];
					}
					applyPlaceholder(handle);
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
				
				static void onChangeText(iOS_ViewInstance* instance, UITextField* field, UITextView* area)
				{
					Ref<View> _view = instance->getView();
					if (EditViewHelper* view = CastInstance<EditViewHelper>(_view.get())) {
						String text;
						if (field != nil) {
							text = Apple::getStringFromNSString([field text]);
						} else if (area != nil) {
							text = Apple::getStringFromNSString([area text]);
						}
						String textNew = text;
						view->dispatchChange(&textNew);
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
					if (EditViewHelper* view = CastInstance<EditViewHelper>(_view.get())) {
						view->dispatchReturnKey();
						if (view->getMultiLine() == MultiLineMode::Single && view->isAutoDismissKeyboard()) {
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

		}
	}

	using namespace priv::edit_view;
	
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBEditViewHandle* handle = [[SLIBEditViewHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditViewHelper*)this)->applyProperties(handle);
		}
		IOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
	
	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		IOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBTextAreaHandle* handle = [[SLIBTextAreaHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			((EditViewHelper*)this)->applyProperties(handle);
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
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setText_NW, this, value));
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
	
	void EditView::_setTextAlignment_NW(Alignment align)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextAlignment_NW, this, align));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setTextAlignment:EditViewHelper::translateAlignment(align)];
				((EditViewHelper*)this)->applyPlaceholder(tv);
			} else if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				[tv setTextAlignment:EditViewHelper::translateAlignment(align)];
			}
		}
	}
	
	void EditView::_setTextColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setTextColor_NW, this, color));
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
	
	void EditView::_setHintText_NW(const String& value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintText_NW, this, value));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			}
		}
	}
	
	void EditView::_setHintTextAlignment_NW(Alignment value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintTextAlignment_NW, this, value));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			}
		}
	}
	
	void EditView::_setHintTextColor_NW(const Color& value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintTextColor_NW, this, value));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			}
		}
	}

	void EditView::_setHintFont_NW(const Ref<Font>& value)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setHintFont_NW, this, value));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			} else if ([handle isKindOfClass:[SLIBTextAreaHandle class]]) {
				SLIBTextAreaHandle* tv = (SLIBTextAreaHandle*)handle;
				((EditViewHelper*)this)->applyPlaceholder(tv);
			}
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setReadOnly_NW, this, flag));
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
	
	void EditView::_setPassword_NW(sl_bool flag)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setPassword_NW, this, flag));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setSecureTextEntry:(flag?YES:NO)];
			}
		}
	}
	
	void EditView::_setMultiLine_NW(MultiLineMode mode)
	{
	}
	
	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setReturnKeyType_NW, this, type));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setReturnKeyType:EditViewHelper::convertReturnKeyType(type)];
			} else if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				[tv setReturnKeyType:EditViewHelper::convertReturnKeyType(type)];
			}
		}
	}
	
	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setKeyboardType_NW, this, type));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setKeyboardType:EditViewHelper::convertKeyboardType(type)];
			} else if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				[tv setKeyboardType:EditViewHelper::convertKeyboardType(type)];
			}
		}
	}
	
	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setAutoCapitalizationType_NW, this, type));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setAutocapitalizationType:EditViewHelper::convertAutoCapitalizationType(type)];
			} else if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				[tv setAutocapitalizationType:EditViewHelper::convertAutoCapitalizationType(type)];
			}
		}
	}
	
	sl_ui_len EditView::_measureHeight_NW()
	{
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			return (sl_ui_len)([handle sizeThatFits:CGSizeMake((CGFloat)(getLayoutWidth() / f), CGFLOAT_MAX)].height * f);
		}
		return 0;
	}
	
	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setFont_NW, this, font));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
				if (hFont != nil) {
					[tv setFont:hFont];
					((EditViewHelper*)this)->applyPlaceholder(tv);
				}
			} else if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
				if (hFont != nil) {
					[tv setFont:hFont];
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
		UIView* handle = UIPlatform::getViewHandle(this);
		if (flag) {
			if ([handle isKindOfClass:[UITextField class]]) {
				UITextField* tv = (UITextField*)handle;
				[tv setBorderStyle:(flag?UITextBorderStyleRoundedRect:UITextBorderStyleNone)];
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
	
	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setBackgroundColor_NW, this, color));
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
	
	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!(isUiThread())) {
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), EditView, _setScrollBarsVisible_NW, this, flagHorizontal, flagVertical));
			return;
		}
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[UITextView class]]) {
				UITextView* tv = (UITextView*)handle;
				[tv setShowsHorizontalScrollIndicator:isHorizontalScrollBarVisible()?YES:NO];
				[tv setShowsVerticalScrollIndicator:(isVerticalScrollBarVisible() && !(isHeightWrapping()))?YES:NO];
			}
		}
	}
	
}

using namespace slib;
using namespace slib::priv::edit_view;

@implementation SLIBEditViewHandle

IOS_VIEW_DEFINE_ON_FOCUS

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
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		EditViewHelper::onChangeText(instance.get(), self, nil);
	}
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
}

-(BOOL) textFieldShouldReturn:(UITextField *)textField{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		EditViewHelper::onEnterAction(instance.get(), self, nil);
	}
	return NO;
}

@end

@implementation SLIBTextAreaHandle

IOS_VIEW_DEFINE_ON_FOCUS

-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setDelegate:self];
		self.contentInset = UIEdgeInsetsZero;
		self.clipsToBounds = YES;
	}
	return self;
}

-(void)setBounds:(CGRect)bounds
{
	[super setBounds:bounds];
	EditViewHelper::setHintLabelFrame(self, m_labelHintText);
}

-(void)textViewDidBeginEditing:(UITextView *)textView
{
	self.contentInset = UIEdgeInsetsZero;
	self.clipsToBounds = YES;
}

-(void)textViewDidChange:(UITextView *)textView
{
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		EditViewHelper::onChangeText(instance.get(), nil, self);
		UILabel* label = self->m_labelHintText;
		if (label != nil) {
			[UIView animateWithDuration:0.25f animations:^{
				if([[self text] length] == 0) {
					label.alpha = 1;
				} else {
					label.alpha = 0;
				}
			}];
		}
	}
}

- (void)insertText:(NSString *)text
{
	[super insertText:text];
	Ref<iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		if ([text isEqualToString:@"\n"]) {
			EditViewHelper::onEnterAction(instance.get(), nil, self);
		}
	}
}

@end

#endif
