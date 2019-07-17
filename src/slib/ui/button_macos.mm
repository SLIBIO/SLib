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

namespace slib
{

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBButtonHandle* handle = [[SLIBButtonHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			handle.title = Apple::getNSStringFromString(m_text);
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

	sl_bool Button::_measureSize_NW(UISize& _out)
	{
		return UIPlatform::measureNativeWidgetFittingSize(this, _out);
	}
	
	void Button::_setFont_NW(const Ref<Font>& font)
	{
	}

}

@implementation SLIBButtonHandle

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

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

@end

#endif
