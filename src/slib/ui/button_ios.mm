#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/button.h"

#include "view_ios.h"

@interface _Slib_iOS_Button : UIButton {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;

}

@end

SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> Button::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_Button* handle = [[_Slib_iOS_Button alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setTitle:(Apple::getNSStringFromString(m_text)) forState:UIControlStateNormal];
		Ref<FontInstance> fontInstance;
		Ref<Font> font = m_font;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			handle.titleLabel.font = hFont;
		}
		[handle setTitleColor:[UIColor colorWithRed:0 green:0.4 blue:0.8 alpha:1] forState:UIControlStateNormal];
		[handle setTitleColor:[UIColor colorWithRed:0 green:0.2 blue:0.4 alpha:1] forState:UIControlStateHighlighted];
		[handle setTitleShadowColor:[UIColor colorWithRed:0.2 green:0.2 blue:0.2 alpha:1] forState:UIControlStateNormal];
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

String Button::getText()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIButton class]]) {
		UIButton* v = (UIButton*)handle;
		m_text = Apple::getStringFromNSString([v titleForState:UIControlStateNormal]);
	}
	return m_text;
}

void Button::setText(const String& text)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIButton class]]) {
		UIButton* v = (UIButton*)handle;
		[v setTitle: (Apple::getNSStringFromString(text)) forState:UIControlStateNormal];
	}
	m_text = text;
}

void Button::setFont(const Ref<Font>& font)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UIButton class]]) {
		UIButton* v = (UIButton*)handle;
		Ref<FontInstance> fontInstance;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			v.titleLabel.font = hFont;
		}
	}
	m_font = font;
}

void Button::setDefaultButton(sl_bool flag)
{
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
