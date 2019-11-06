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

#include "slib/ui/core.h"

#include "view_ios.h"

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

@interface SLIBEditViewHandle : UITextField<UITextFieldDelegate>
{	
	@public slib::WeakRef<slib::priv::edit_view::EditViewInstance> m_viewInstance;
}
@end

@interface SLIBTextAreaHandle : UITextView<UITextViewDelegate>
{
	@public slib::WeakRef<slib::priv::edit_view::TextAreaInstance> m_viewInstance;
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
			
			static ::UIReturnKeyType ConvertReturnKeyType(UIReturnKeyType type)
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
			
			static ::UIKeyboardType ConvertKeyboardType(UIKeyboardType type)
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
			
			static ::UITextAutocapitalizationType ConvertAutoCapitalizationType(UIAutoCapitalizationType type)
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
			
			sl_ui_len MeasureHeight(UIView* handle, EditView* view)
			{
				if (handle != nil) {
					CGFloat f = UIPlatform::getGlobalScaleFactor();
					return (sl_ui_len)([handle sizeThatFits:CGSizeMake((CGFloat)(view->getLayoutWidth() / f), CGFLOAT_MAX)].height * f);
				}
				return 0;
			}
			
			class EditViewInstance : public iOS_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				UITextField* getHandle()
				{
					return (UITextField*)m_handle;
				}
				
				sl_bool getText(EditView* view, String& _out) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						_out = Apple::getStringFromNSString([handle text]);
						return sl_true;
					}
					return sl_false;
				}
				
				void setText(EditView* view, const String& text) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						NSString* value = Apple::getNSStringFromString(text, @"");
						[handle setText:value];
					}
				}
				
				void setGravity(EditView* view, const Alignment& gravity) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setTextAlignment:TranslateAlignment(gravity)];
					}
				}
				
				void setTextColor(EditView* view, const Color& color) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
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
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setEnabled:(flag ? FALSE : TRUE)];
					}
				}
				
				void setPassword(EditView* view, sl_bool flag) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setSecureTextEntry:(flag ? YES : NO)];
					}
				}
				
				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
				}
				
				void setReturnKeyType(EditView* view, UIReturnKeyType type) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setReturnKeyType:ConvertReturnKeyType(type)];
					}
				}
				
				void setKeyboardType(EditView* view, UIKeyboardType type) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setKeyboardType:ConvertKeyboardType(type)];
					}
				}
				
				void setAutoCapitalizationType(EditView* view, UIAutoCapitalizationType type) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setAutocapitalizationType:ConvertAutoCapitalizationType(type)];
					}
				}
				
				sl_ui_len measureHeight(EditView* view) override
				{
					return MeasureHeight(getHandle(), view);
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						setHandleFont(handle, font);
						if (IsInstanceOf<EditView>(view)) {
							updateHint((EditView*)view);
						}
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setBorderStyle:(flag ? UITextBorderStyleRoundedRect : UITextBorderStyleNone)];
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
					}
				}
				
				void updateHint(EditView* view)
				{
					UITextField* handle = getHandle();
					if (handle != nil) {
						applyHint(handle, view);
					}
				}
				
				static void applyHint(UITextField* handle, EditView* view)
				{
					NSAttributedString* attr;
					String _text = view->getHintText();
					if (_text.isEmpty()) {
						attr = nil;
					} else {
						UIFont* hFont = nil;
						Ref<Font> font = view->getHintFont();
						if (font.isNotNull()) {
							hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
						}
						NSString* text = Apple::getNSStringFromString(_text);
						UIColor* color = GraphicsPlatform::getUIColorFromColor(view->getHintTextColor());
						NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
						[paragraphStyle setAlignment:TranslateAlignment(view->getHintGravity())];
						if (hFont != nil) {
							attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle, NSFontAttributeName: hFont}];
						} else {
							attr = [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: color, NSParagraphStyleAttributeName: paragraphStyle}];
						}
					}
					[handle setAttributedPlaceholder: attr];
				}
				
				void apply(EditView* view)
				{
					UITextField* handle = getHandle();
					if (handle == nil) {
						return;
					}
					
					setHandleFont(handle, view->getFont());
					[handle setText:(Apple::getNSStringFromString(view->getText()))];
					[handle setTextAlignment:TranslateAlignment(view->getGravity())];
					[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(view->getTextColor()))];
					[handle setBorderStyle:(view->isBorder() ? UITextBorderStyleRoundedRect : UITextBorderStyleNone)];
					[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(view->getBackgroundColor()))];
					[handle setEnabled:(view->isReadOnly() ? NO : YES)];
					[handle setSecureTextEntry:(view->isPassword() ? YES : NO)];
					[handle setReturnKeyType:ConvertReturnKeyType(view->getReturnKeyType())];
					[handle setKeyboardType:ConvertKeyboardType(view->getKeyboardType())];
					[handle setAutocapitalizationType:ConvertAutoCapitalizationType(view->getAutoCaptializationType())];

					applyHint(handle, view);
				}
				
				void onChange(UITextField* control)
				{
					Ref<EditView> view = CastRef<EditView>(getView());
					if (view.isNotNull()) {
						String text = Apple::getStringFromNSString([control text]);
						String textNew = text;
						view->dispatchChange(&textNew);
						if (text != textNew) {
							NSString* str = Apple::getNSStringFromString(textNew, @"");
							[control setText:str];
						}
					}
				}
				
				void onEnterAction(UITextField* control)
				{
					Ref<EditView> view = CastRef<EditView>(getView());
					if (view.isNotNull()) {
						view->dispatchReturnKey();
					}
				}

			};
			
			SLIB_DEFINE_OBJECT(EditViewInstance, iOS_ViewInstance)

			class TextAreaInstance : public iOS_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBTextAreaHandle* getHandle()
				{
					return (SLIBTextAreaHandle*)m_handle;
				}
				
				sl_bool getText(EditView* view, String& _out) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						_out = Apple::getStringFromNSString([handle text]);
						return sl_true;
					}
					return sl_false;
				}
				
				void setText(EditView* view, const String& text) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						NSString* value = Apple::getNSStringFromString(text, @"");
						[handle setText:value];
					}
				}
				
				void setGravity(EditView* view, const Alignment& gravity) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setTextAlignment:TranslateAlignment(gravity)];
					}
				}
				
				void setTextColor(EditView* view, const Color& color) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
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
						[handle setEditable:(flag ? FALSE : TRUE)];
					}
				}
				
				void setPassword(EditView* view, sl_bool flag) override
				{
				}
				
				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
				}
				
				void setReturnKeyType(EditView* view, UIReturnKeyType type) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setReturnKeyType:ConvertReturnKeyType(type)];
					}
				}
				
				void setKeyboardType(EditView* view, UIKeyboardType type) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setKeyboardType:ConvertKeyboardType(type)];
					}
				}
				
				void setAutoCapitalizationType(EditView* view, UIAutoCapitalizationType type) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setAutocapitalizationType:ConvertAutoCapitalizationType(type)];
					}
				}
				
				sl_ui_len measureHeight(EditView* view) override
				{
					return MeasureHeight(getHandle(), view);
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						setHandleFont(handle, font);
						if (IsInstanceOf<EditView>(view)) {
							updateHint((EditView*)view);
						}
					}
				}
				
				void setBorder(View* view, sl_bool flag) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						if (flag) {
							[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
							[handle.layer setBorderWidth:1];
						} else {
							[handle.layer setBorderWidth:0];
						}
					}
				}
				
				void setBackgroundColor(View* view, const Color& color) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
					}
				}
				
				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle != nil) {
						[handle setShowsHorizontalScrollIndicator:(flagHorizontal ? YES : NO)];
						[handle setShowsVerticalScrollIndicator:(flagVertical ? YES : NO)];
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
					String text = view->getHintText();
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
						label.textAlignment = TranslateAlignment(view->getHintGravity());
						label.textColor = GraphicsPlatform::getUIColorFromColor(view->getHintTextColor());
						setHandleFont(label, view->getHintFont());
						label.alpha = 1;
						handle->m_hintTextVerticalAlignment = view->getHintGravity() & Alignment::VerticalMask;
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
				
				void apply(TextArea* view)
				{
					SLIBTextAreaHandle* handle = getHandle();
					if (handle == nil) {
						return;
					}
					
					setHandleFont(handle, view->getFont());
					[handle setScrollEnabled:YES];
					[handle setShowsHorizontalScrollIndicator:view->isHorizontalScrollBarVisible() ? YES : NO];
					[handle setShowsVerticalScrollIndicator:view->isVerticalScrollBarVisible() ? YES : NO];
					[handle setText:(Apple::getNSStringFromString(view->getText()))];
					[handle setTextAlignment:TranslateAlignment(view->getGravity())];
					[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(view->getTextColor()))];
					if (view->isBorder()) {
						[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
						[handle.layer setBorderWidth:1];
					} else {
						[handle.layer setBorderWidth:0];
					}
					[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(view->getBackgroundColor()))];
					[handle setEditable:(view->isReadOnly() ? FALSE : TRUE)];
					[handle setSelectable:TRUE];
					[handle setReturnKeyType:ConvertReturnKeyType(view->getReturnKeyType())];
					[handle setKeyboardType:ConvertKeyboardType(view->getKeyboardType())];
					[handle setAutocapitalizationType:ConvertAutoCapitalizationType(view->getAutoCaptializationType())];
					
					applyHint(handle, view);
				}
				
				void onChange(SLIBTextAreaHandle* control)
				{
					Ref<TextArea> view = CastRef<TextArea>(getView());
					if (view.isNotNull()) {
						String text = Apple::getStringFromNSString([control text]);
						String textNew = text;
						view->dispatchChange(&textNew);
						if (text != textNew) {
							NSString* str = Apple::getNSStringFromString(textNew, @"");
							[control setText:str];
						}
					}
				}
				
				void onEnterAction(SLIBTextAreaHandle* control)
				{
					Ref<TextArea> view = CastRef<TextArea>(getView());
					if (view.isNotNull()) {
						view->dispatchReturnKey();
						if (view->getMultiLine() == MultiLineMode::Single && view->isAutoDismissKeyboard()) {
							[control resignFirstResponder];
						}
					}
				}

			};
			
			SLIB_DEFINE_OBJECT(TextAreaInstance, iOS_ViewInstance)
			
		}
	}

	using namespace priv::edit_view;
	
	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
	{
		Ref<EditViewInstance> ret = iOS_ViewInstance::create<EditViewInstance, SLIBEditViewHandle>(this, parent);
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
		Ref<TextAreaInstance> ret = iOS_ViewInstance::create<TextAreaInstance, SLIBTextAreaHandle>(this, parent);
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
	Ref<EditViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onChange(self);
	}
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
}

-(BOOL) textFieldShouldReturn:(UITextField *)textField{
	Ref<EditViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEnterAction(self);
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
	TextAreaInstance::setHintLabelFrame(self, m_labelHintText);
}

-(void)textViewDidBeginEditing:(UITextView *)textView
{
	self.contentInset = UIEdgeInsetsZero;
	self.clipsToBounds = YES;
}

-(void)textViewDidChange:(UITextView *)textView
{
	Ref<TextAreaInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onChange(self);
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
	Ref<TextAreaInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		if ([text isEqualToString:@"\n"]) {
			instance->onEnterAction(self);
		}
	}
}

@end

#endif
