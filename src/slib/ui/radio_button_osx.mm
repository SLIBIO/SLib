#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/radio_button.h"

#include "view_osx.h"

@interface _Slib_OSX_RadioButton : NSButton {
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
}

@end

SLIB_UI_NAMESPACE_BEGIN

Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_RadioButton* handle = [[_Slib_OSX_RadioButton alloc] initWithFrame:frame];
	if (handle != nil) {
		handle.title = Apple::getNSStringFromString(getText());
		[handle setButtonType:NSRadioButton];
		[handle setState: (isChecked() ? NSOnState : NSOffState)];
		
		Ref<Font> font = getFont();
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
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
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
