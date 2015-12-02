#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/radio_button.h"

#include "ui_view_osx.h"

@interface _Slib_OSX_RadioButton : NSButton {
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
}

@end

SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> RadioButton::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_RadioButton* handle = [[_Slib_OSX_RadioButton alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.title = Apple::getNSStringFromString(m_text);
		[handle setButtonType:NSRadioButton];
		[handle setState: (m_flagSelected ? NSOnState : NSOffState)];
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

String RadioButton::getText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		m_text = Apple::getStringFromNSString(v.title);
	}
	return m_text;
}

void RadioButton::setText(const String& text)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		v.title = Apple::getNSStringFromString(text);
	}
	m_text = text;
}

void RadioButton::setFont(const Ref<Font>& _font)
{
	Ref<Font> font = _font;
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
	m_font = font;
}

sl_bool RadioButton::isChecked()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		m_flagSelected = (v.state == NSOnState ? sl_true : sl_false);
	}
	return m_flagSelected;
}

void RadioButton::setChecked(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		[v setState: (flag ? NSOnState : NSOffState)];
	}
	m_flagSelected = flag;
}
SLIB_UI_NAMESPACE_END

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
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
