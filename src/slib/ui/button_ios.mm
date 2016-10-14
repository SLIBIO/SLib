#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/button.h"

#include "view_ios.h"

@interface _Slib_iOS_Button : UIButton {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN

Ref<ViewInstance> Button::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_Button* handle = [[_Slib_iOS_Button alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setTitle:(Apple::getNSStringFromString(m_text)) forState:UIControlStateNormal];
		Ref<Font> font = getFont();
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			handle.titleLabel.font = hFont;
		}
		[handle setTitleColor:[UIColor colorWithRed:0 green:0.4 blue:0.8 alpha:1] forState:UIControlStateNormal];
		[handle setTitleColor:[UIColor colorWithRed:0 green:0.2 blue:0.4 alpha:1] forState:UIControlStateHighlighted];
		[handle setTitleShadowColor:[UIColor colorWithRed:0.2 green:0.2 blue:0.2 alpha:1] forState:UIControlStateNormal];
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

void Button::_setText_NW(const String& text)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIButton class]]) {
		UIButton* v = (UIButton*)handle;
		[v setTitle: (Apple::getNSStringFromString(text)) forState:UIControlStateNormal];
	}
}

void Button::_setDefaultButton_NW(sl_bool flag)
{
}

void Button::_setFont_NW(const Ref<Font>& font)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIButton class]]) {
		UIButton* v = (UIButton*)handle;
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			v.titleLabel.font = hFont;
		}
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_Button
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self addTarget:self action:@selector(onClick) forControlEvents:UIControlEventTouchUpInside];
	}
	return self;
}

-(void)onClick
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
