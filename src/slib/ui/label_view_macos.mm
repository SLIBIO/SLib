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
