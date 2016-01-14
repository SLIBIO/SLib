#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/check_box.h"

#include "view_osx.h"

@interface _Slib_OSX_CheckBox : NSButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> CheckBox::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_CheckBox* handle = [[_Slib_OSX_CheckBox alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.title = Apple::getNSStringFromString(m_text);
		[handle setButtonType:NSSwitchButton];
		[handle setState: (m_flagChecked ? NSOnState : NSOffState)];
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

String CheckBox::getText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		m_text = Apple::getStringFromNSString(v.title);
	}
	return m_text;
}

void CheckBox::setText(const String& text)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		v.title = Apple::getNSStringFromString(text);
	}
	m_text = text;
}

void CheckBox::setFont(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
	m_font = font;
}

sl_bool CheckBox::isChecked()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		m_flagChecked = (v.state == NSOnState ? sl_true : sl_false);
	}
	return m_flagChecked;
}

void CheckBox::setChecked(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		[v setState: (flag ? NSOnState : NSOffState)];
	}
	m_flagChecked = flag;
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_CheckBox
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
