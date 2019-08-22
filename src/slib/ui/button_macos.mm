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

#include "slib/ui/core.h"

#include "button_macos.h"

namespace slib
{
	
	namespace priv
	{
		namespace button
		{
			
			SLIB_DEFINE_OBJECT(ButtonInstance, macOS_ViewInstance)

			ButtonInstance::ButtonInstance()
			{
			}
			
			ButtonInstance::~ButtonInstance()
			{
			}
			
			void ButtonInstance::setText(const String& text)
			{
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThreadUrgently(SLIB_BIND_WEAKREF(void(), ButtonInstance, setText, this, text));
					return;
				}
				NSButton* handle = getHandleOf<NSButton>();
				if (handle != nil) {
					handle.title = Apple::getNSStringFromString(text);
				}
			}
			
			void ButtonInstance::setDefaultButton(sl_bool flag)
			{
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThreadUrgently(SLIB_BIND_WEAKREF(void(), ButtonInstance, setDefaultButton, this, flag));
					return;
				}
				NSButton* handle = getHandleOf<NSButton>();
				if (handle != nil) {
					if (flag) {
						[handle setKeyEquivalent:@"\r"];
					} else {
						[handle setKeyEquivalent:@""];
					}
				}
			}
			
			sl_bool ButtonInstance::measureSize(UISize& _out)
			{
				return UIPlatform::measureNativeWidgetFittingSize(this, _out);
			}
			
			void ButtonInstance::getChecked(sl_bool& _out)
			{
				NSButton* handle = getHandleOf<NSButton>();
				if (handle != nil) {
					_out = (handle.state == NSOnState ? sl_true : sl_false);
				}
			}
			
			void ButtonInstance::setChecked(sl_bool flag)
			{
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThreadUrgently(SLIB_BIND_WEAKREF(void(), ButtonInstance, setChecked, this, flag));
					return;
				}
				NSButton* handle = getHandleOf<NSButton>();
				if (handle != nil) {
					[handle setState: (flag ? NSOnState : NSOffState)];
				}
			}
			
		}
	}
	
	using namespace priv::button;

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* parent)
	{
		Ref<ButtonInstance> ret = macOS_ViewInstance::create<ButtonInstance, SLIBButtonHandle>(this, parent);
		if (ret.isNotNull()) {
			SLIBButtonHandle* handle = (SLIBButtonHandle*)(ret->getHandle());
			handle.title = Apple::getNSStringFromString(m_text);
			if (m_flagDefaultButton) {
				[handle setKeyEquivalent:@"\r"];
			}
			handle.bezelStyle = NSRoundedBezelStyle;
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IButtonInstance> Button::getButtonInstance()
	{
		return CastRef<ButtonInstance>(getViewInstance());
	}
	
}

using namespace slib;
using namespace slib::priv::button;

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
	Ref<ButtonInstance> instance = m_viewInstance;
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
