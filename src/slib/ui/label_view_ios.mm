#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/label_view.h"

#include "view_ios.h"

@interface _Slib_iOS_LabelTextField : UILabel {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN
class _LabelView : public LabelView
{
public:
	void applyProperties(UILabel* handle)
	{
		[handle setText:(Apple::getNSStringFromString(m_text))];
		[handle setTextAlignment:translateAlignment(m_textAlignment)];
		if (isBorder()) {
			[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
			[handle.layer setBorderWidth:1];
		} else {
			[handle.layer setBorderWidth:0];
		}
		[handle setTextColor:(GraphicsPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(getBackgroundColor()))];
		Ref<Font> font = getFont();
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	static NSTextAlignment translateAlignment(Alignment _align)
	{
		Alignment align = _align & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			return NSTextAlignmentCenter;
		} else if (align == Alignment::Right) {
			return NSTextAlignmentRight;
		}
		return NSTextAlignmentLeft;
	}
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSTextAlignmentCenter) {
			return Alignment::Center;
		} else if (align == NSTextAlignmentRight) {
			return Alignment::Right;
		}
		return Alignment::Left;
	}
};

Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_LabelTextField* handle = [[_Slib_iOS_LabelTextField alloc] initWithFrame:frame];
	if (handle != nil) {
		((_LabelView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

void LabelView::_setText_NW(const String& value)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setText:(Apple::getNSStringFromString(value))];
	}
}

void LabelView::_setTextAlignment_NW(Alignment align)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setTextAlignment:_LabelView::translateAlignment(align)];
	}
}

void LabelView::_setTextColor_NW(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
	}
}

void LabelView::_setFont_NW(const Ref<Font>& font)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		UIFont* hFont = GraphicsPlatform::getUIFont(font.ptr, UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[tv setFont:hFont];
		}
	}
}

void LabelView::_setBorder_NW(sl_bool flag)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		if (flag) {
			[tv.layer setBorderColor:([[UIColor grayColor] CGColor])];
			[tv.layer setBorderWidth:1];
		} else {
			[tv.layer setBorderWidth:0];
		}
	}
}

void LabelView::_setBackgroundColor_NW(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
	}
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_iOS_LabelTextField
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(labelTapped)];
		tapGestureRecognizer.numberOfTapsRequired = 1;
		[self addGestureRecognizer:tapGestureRecognizer];
		self.userInteractionEnabled = YES;
	}
	return self;
}

- (void)labelTapped
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
