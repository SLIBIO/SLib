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

#if defined(SLIB_PLATFORM_IS_OSX)

#include "slib/ui/radio_button.h"

#include "view_osx.h"

@interface _Slib_OSX_RadioButton : NSButton {
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
}

@end

namespace slib
{

	Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* _parent)
	{
		OSX_VIEW_CREATE_INSTANCE_BEGIN
		_Slib_OSX_RadioButton* handle = [[_Slib_OSX_RadioButton alloc] initWithFrame:frame];
		if (handle != nil) {
			handle.title = Apple::getNSStringFromString(getText());
			[handle setButtonType:NSRadioButton];
			[handle setState: (isChecked() ? NSOnState : NSOffState)];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
			}
		}
		OSX_VIEW_CREATE_INSTANCE_END
		return ret;
	}

}

@implementation _Slib_OSX_RadioButton
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
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
