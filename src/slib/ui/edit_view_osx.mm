#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/edit_view.h"
#include "../../../inc/slib/ui/core.h"

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

namespace slib
{

	class _EditView : public EditView
	{
	public:
		void __applyProperties(NSTextField* handle)
		{
			[handle setStringValue:(Apple::getNSStringFromString(m_text))];
			[handle setAlignment:translateAlignment(m_textAlignment)];
			[handle setBordered: (isBorder() ? TRUE : FALSE)];
			[handle setBezeled: (isBorder() ? TRUE : FALSE)];
			[[handle cell] setPlaceholderString:(Apple::getNSStringFromString(m_hintText))];
			[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			[handle setEditable:(m_flagReadOnly?FALSE:TRUE)];
			[handle setSelectable:TRUE];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		
		void __applyProperties(_Slib_OSX_TextArea* handle)
		{
			NSTextView* tv = handle->textView;
			[tv setString:(Apple::getNSStringFromString(m_text))];
			[tv setAlignment:translateAlignment(m_textAlignment)];
			[handle setBorderType:(isBorder() ? NSBezelBorder : NSNoBorder)];
			[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			[tv setEditable:(m_flagReadOnly?FALSE:TRUE)];
			[tv setSelectable:TRUE];
			
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
			if (_EditView* view = CastInstance<_EditView>(_view.get())) {
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
			if (_EditView* view = CastInstance<_EditView>(_view.get())) {
				String text = Apple::getStringFromNSString([control->textView string]);
				String textNew = view->dispatchChange(text);
				if (text != textNew) {
					NSString* str = Apple::getNSStringFromString(textNew);
					[control->textView setString:str];
				}
			}
		}

	};

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
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

	Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* _parent)
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

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
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

	void EditView::_getText_NW()
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
	}

	void EditView::_setText_NW(const String& _value)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			NSString* value = Apple::getNSStringFromString(_value);
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setStringValue:value];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setStringValue:value];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv->textView setString:value];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv->textView setString:value];
					});
				}
			}
		}
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setBordered:(flag?TRUE:FALSE)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setBordered:(flag?TRUE:FALSE)];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setBorderType:(flag?NSBezelBorder:NSNoBorder)];
					});
				}
			}
		}
	}

	void EditView::_setTextAlignment_NW(Alignment align)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setAlignment:_EditView::translateAlignment(align)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setAlignment:_EditView::translateAlignment(align)];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv->textView setAlignment:_EditView::translateAlignment(align)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv->textView setAlignment:_EditView::translateAlignment(align)];
					});
				}
			}
		}
	}

	void EditView::_setHintText_NW(const String& value)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			NSString* s = Apple::getNSStringFromString(value);
			if (UI::isUiThread()) {
				[[tv cell] setPlaceholderString:s];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[[tv cell] setPlaceholderString:s];
				});
			}
		}
	}


	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setEditable:(flag ? FALSE : TRUE)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setEditable:(flag ? FALSE : TRUE)];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv->textView setEditable:(flag ? FALSE : TRUE)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv->textView setEditable:(flag ? FALSE : TRUE)];
					});
				}
			}
		}
	}

	void EditView::_setMultiLine_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[_Slib_OSX_TextField class]]) {
			_Slib_OSX_TextField* tv = (_Slib_OSX_TextField*)handle;
			tv->m_flagMultiLine = flag;
		}
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv->textView setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv->textView setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
					});
				}
			}
		}
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				if (UI::isUiThread()) {
					[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
					});
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				if (UI::isUiThread()) {
					[tv->textView setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[tv->textView setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
					});
				}
			}
		}
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			if ([handle isKindOfClass:[NSTextField class]]) {
				NSTextField* tv = (NSTextField*)handle;
				NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
				if (hFont != nil) {
					if (UI::isUiThread()) {
						[tv setFont:hFont];
					} else {
						dispatch_async(dispatch_get_main_queue(), ^{
							[tv setFont:hFont];
						});
					}
				}
			} else if ([handle isKindOfClass:[_Slib_OSX_TextArea class]]) {
				_Slib_OSX_TextArea* tv = (_Slib_OSX_TextArea*)handle;
				NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
				if (hFont != nil) {
					if (UI::isUiThread()) {
						[tv->textView setFont:hFont];
					} else {
						dispatch_async(dispatch_get_main_queue(), ^{
							[tv->textView setFont:hFont];
						});
					}
				}
			}
		}
	}

}

@implementation _Slib_OSX_TextField
-(void)controlTextDidChange:(NSNotification *)obj
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::_EditView::onChangeTextField(instance.get(), self);
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
		slib::_EditView::onChangeTextField(instance.get(), self);
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
		slib::_EditView::onChangeTextArea(instance.get(), self);
	}
}

@end

@implementation _Slib_OSX_TextArea_TextView

- (void)keyDown:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_true, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super keyDown:theEvent];
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

#endif
