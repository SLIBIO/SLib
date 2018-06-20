/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
