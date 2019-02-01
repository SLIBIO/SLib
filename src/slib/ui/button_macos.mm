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

#include "slib/ui/button.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_Button : NSButton {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;

}

@end

namespace slib
{

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_Button* handle = [[_priv_Slib_macOS_Button alloc] initWithFrame:frame];
		if (handle != nil) {
			handle.title = Apple::getNSStringFromString(m_text);
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
			if (m_flagDefaultButton) {
				[handle setKeyEquivalent:@"\r"];
			}
			handle.bezelStyle = NSRoundedBezelStyle;
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void Button::_setText_NW(const String& text)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* v = (NSButton*)handle;
			v.title = Apple::getNSStringFromString(text);
		}
	}

	void Button::_setDefaultButton_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* v = (NSButton*)handle;
			if (flag) {
				[v setKeyEquivalent:@"\r"];
			} else {
				[v setKeyEquivalent:@""];
			}
		}
	}

	void Button::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* v = (NSButton*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[v setFont:hFont];
			}
		}
	}
	
	UISize _priv_Button_macOS_measureSize(Button* view)
	{
		NSView* handle = UIPlatform::getViewHandle(view);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* button = (NSButton*)handle;
			NSSize size = button.fittingSize;
			return UISize((sl_ui_len)(size.width), (sl_ui_len)(size.height));
		} else {
			NSButton* button = [[NSButton alloc] init];
			if (button != nil) {
				button.title = Apple::getNSStringFromString(view->getText());
				Ref<Font> font = view->getFont();
				NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
				if (hFont != nil) {
					[button setFont:hFont];
				}
				button.bezelStyle = NSRoundedBezelStyle;
				NSSize size = button.fittingSize;
				return UISize((sl_ui_len)(size.width), (sl_ui_len)(size.height));
			}
		}
		return UISize::zero();
	}
	
}

@implementation _priv_Slib_macOS_Button

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setAction: @selector(onClick)];
		[self setTarget:self];
	}
	return self;
}

-(void)onClick
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}

- (BOOL)performKeyEquivalent:(NSEvent *)anEvent
{
	return NO;
}

- (void)setFrame:(NSRect)frame
{
	[super setFrame:frame];
}

- (void)keyDown:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
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

#endif
