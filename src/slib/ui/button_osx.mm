#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/button.h"

#include "view_osx.h"

@interface _Slib_OSX_Button : NSButton {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN

Ref<ViewInstance> Button::createNativeWidget(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_Button* handle = [[_Slib_OSX_Button alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.title = Apple::getNSStringFromString(m_text);
		
		Ref<Font> font = getFont();
		NSFont* hFont = GraphicsPlatform::getNSFont(font.ptr);
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
			v.bezelStyle = NSRoundedBezelStyle;
		} else {
			[v setKeyEquivalent:@""];
			v.bezelStyle = NSRoundedBezelStyle;
			//v.bezelStyle = NSRegularSquareBezelStyle;
		}
	}
}

void Button::_setFont_NW(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSButton class]]) {
		NSButton* v = (NSButton*)handle;
		NSFont* hFont = GraphicsPlatform::getNSFont(font.ptr);
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
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
