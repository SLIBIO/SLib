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

#include "slib/ui/label_view.h"

#include "view_macos.h"

@interface _priv_Slib_macOS_LabelTextField : NSTextField {
	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	
}
@end

namespace slib
{

	class _priv_LabelView : public LabelView
	{
	public:
		void _applyProperties(NSTextField* handle)
		{
			[handle setStringValue:(Apple::getNSStringFromString(m_text))];
			[handle setAlignment:translateAlignment(m_textAlignment)];
			[handle setBordered: (isBorder() ? TRUE : FALSE)];
			[handle setBezeled: (isBorder() ? TRUE : FALSE)];
			[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[handle setFont:hFont];
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
	};

	Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_LabelTextField* handle = [[_priv_Slib_macOS_LabelTextField alloc] initWithFrame:frame];
		if (handle != nil) {
			[handle setEditable:NO];
			[handle setSelectable:NO];
			((_priv_LabelView*)this)->_applyProperties(handle);
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void LabelView::_setText_NW(const String& value)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setStringValue:(Apple::getNSStringFromString(value))];
		}
	}

	void LabelView::_setTextAlignment_NW(Alignment align)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setAlignment:_priv_LabelView::translateAlignment(align)];
		}
	}

	void LabelView::_setTextColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
		}
	}

	void LabelView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
	}

	void LabelView::_setBorder_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBordered:(flag?TRUE:FALSE)];
		}
	}

	void LabelView::_setBackgroundColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
		}
	}

}

@implementation _priv_Slib_macOS_LabelTextField
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
