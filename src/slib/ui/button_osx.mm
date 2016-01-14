#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/button.h"

#include "view_osx.h"

@interface _Slib_OSX_Button : NSButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> Button::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_Button* handle = [[_Slib_OSX_Button alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.title = Apple::getNSStringFromString(m_text);
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
		
		if (m_flagDefaultButton) {
			[handle setKeyEquivalent:@"\r"];
			handle.bezelStyle = NSRoundedBezelStyle;
		} else {
			handle.bezelStyle = NSRoundedBezelStyle;
			//handle.bezelStyle = NSRegularSquareBezelStyle;
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

String Button::getText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		m_text = Apple::getStringFromNSString(v.title);
	}
	return m_text;
}

void Button::setText(const String& text)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		v.title = Apple::getNSStringFromString(text);
	}
	m_text = text;
}

void Button::setFont(const Ref<Font>& font)
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

void Button::setDefaultButton(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		if (flag) {
			[v setKeyEquivalent:@"\r"];
			v.bezelStyle = NSRoundedBezelStyle;
		} else {
			[v setKeyEquivalent:@""];
			v.bezelStyle = NSRoundedBezelStyle;
			//v.bezelStyle = NSRegularSquareBezelStyle;
		}
	}
	m_flagDefaultButton = flag;
}
SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_Button
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

- (BOOL)performKeyEquivalent:(NSEvent *)anEvent
{
	return NO;
}
@end

#endif
