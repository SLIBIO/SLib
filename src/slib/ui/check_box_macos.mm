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

#include "slib/ui/check_box.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_CheckBox : NSButton {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;

}

@end

namespace slib
{

	Ref<ViewInstance> CheckBox::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_CheckBox* handle = [[_priv_Slib_macOS_CheckBox alloc] initWithFrame:frame];
		if (handle != nil) {
			handle.title = Apple::getNSStringFromString(getText());
			[handle setButtonType:NSSwitchButton];
			[handle setState: (m_flagChecked ? NSOnState : NSOffState)];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void CheckBox::_getChecked_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* v = (NSButton*)handle;
			m_flagChecked = (v.state == NSOnState ? sl_true : sl_false);
		}
	}

	void CheckBox::_setChecked_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
			NSButton* v = (NSButton*)handle;
			[v setState: (flag ? NSOnState : NSOffState)];
		}
	}

}

@implementation _priv_Slib_macOS_CheckBox
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
@end

#endif
