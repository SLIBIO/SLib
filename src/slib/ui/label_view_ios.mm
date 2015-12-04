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
		if (m_flagBorder) {
			[handle.layer setBorderColor:([[UIColor grayColor] CGColor])];
			[handle.layer setBorderWidth:1];
		} else {
			[handle.layer setBorderWidth:0];
		}
		[handle setTextColor:(UIPlatform::getUIColorFromColor(m_textColor))];
		[handle setBackgroundColor:(UIPlatform::getUIColorFromColor(m_backgroundColor))];
		Ref<FontInstance> fontInstance;
		Ref<Font> font = m_font;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	static NSTextAlignment translateAlignment(Alignment _align)
	{
		sl_uint32 align = _align & alignHorizontalMask;
		if (align == alignCenter) {
			return NSTextAlignmentCenter;
		} else if (align == alignRight) {
			return NSTextAlignmentRight;
		}
		return NSTextAlignmentLeft;
	}
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSTextAlignmentCenter) {
			return alignCenter;
		} else if (align == NSTextAlignmentRight) {
			return alignRight;
		}
		return alignLeft;
	}
};

Ref<ViewInstance> LabelView::createInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_LabelTextField* handle = [[_Slib_iOS_LabelTextField alloc] initWithFrame:frame];
	if (handle != nil) {
		((_LabelView*)this)->applyProperties(handle);
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

String LabelView::getText()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		NSString* s = [tv text];
		m_text = Apple::getStringFromNSString(s);
	}
	return m_text;
}

void LabelView::setText(const String& value)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setText:(Apple::getNSStringFromString(value))];
	}
	m_text = value;
}

sl_bool LabelView::isBorder()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		m_flagBorder = [tv.layer borderWidth] > 0;
	}
	return m_flagBorder;
}

void LabelView::setBorder(sl_bool flag)
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
	m_flagBorder = flag;
}

Alignment LabelView::getTextAlignment()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		m_textAlignment = _LabelView::translateAlignmentReverse(tv.textAlignment);
	}
	return m_textAlignment;
}

void LabelView::setTextAlignment(Alignment align)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setTextAlignment:_LabelView::translateAlignment(align)];
	}
	m_textAlignment = align;
}

Color LabelView::getTextColor()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		m_textColor = UIPlatform::getColorFromUIColor(tv.textColor);
	}
	return m_textColor;
}

void LabelView::setTextColor(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setTextColor:(UIPlatform::getUIColorFromColor(color))];
	}
	m_textColor = color;
}

Color LabelView::getBackgroundColor()
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		m_backgroundColor = UIPlatform::getColorFromUIColor(tv.backgroundColor);
	}
	return m_backgroundColor;
}

void LabelView::setBackgroundColor(const Color& color)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		[tv setBackgroundColor:(UIPlatform::getUIColorFromColor(color))];
	}
	m_backgroundColor = color;
}

void LabelView::setFont(const Ref<Font>& _font)
{
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
		UILabel* tv = (UILabel*)handle;
		Ref<FontInstance> fontInstance;
		Ref<Font> font = _font;
		UIFont* hFont = UIPlatform::getUIFont(font.get(), fontInstance);
		if (hFont != nil) {
			[tv setFont:hFont];
		}
	}
	m_font = _font;
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
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
