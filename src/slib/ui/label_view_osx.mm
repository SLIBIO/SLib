#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/label_view.h"

#include "view_osx.h"

@interface _Slib_OSX_LabelTextField : NSTextField {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _LabelView : public LabelView
{
public:
	void __applyProperties(NSTextField* handle)
	{
		[handle setStringValue:(Apple::getNSStringFromString(m_text))];
		[handle setAlignment:translateAlignment(m_textAlignment)];
		[handle setBordered: (m_flagBorder ? TRUE : FALSE)];
		[handle setBezeled: (m_flagBorder ? TRUE : FALSE)];
		[handle setTextColor:(UIPlatform::getNSColorFromColor(m_textColor))];
		[handle setBackgroundColor:(UIPlatform::getNSColorFromColor(m_backgroundColor))];
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	
	static NSTextAlignment translateAlignment(Alignment _align)
	{
		Alignment align = _align & Alignment::HorizontalMask;
		if (align == Alignment::Center) {
			return NSCenterTextAlignment;
		} else if (align == Alignment::Right) {
			return NSRightTextAlignment;
		}
		return NSLeftTextAlignment;
	}
	
	static Alignment translateAlignmentReverse(NSTextAlignment align)
	{
		if (align == NSCenterTextAlignment) {
			return Alignment::Center;
		} else if (align == NSRightTextAlignment) {
			return Alignment::Right;
		}
		return Alignment::Left;
	}
};

Ref<ViewInstance> LabelView::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_OSX_LabelTextField* handle = [[_Slib_OSX_LabelTextField alloc] initWithFrame:frame];
	if (handle != nil) {
		[handle setEditable:NO];
		[handle setSelectable:NO];
		((_LabelView*)this)->__applyProperties(handle);
	}
	OSX_VIEW_CREATE_INSTANCE_END
	return ret;
}

String LabelView::getText()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		NSString* s = [tv stringValue];
		m_text = Apple::getStringFromNSString(s);
	}
	return m_text;
}

void LabelView::setText(const String& value)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		[tv setStringValue:(Apple::getNSStringFromString(value))];
	}
	m_text = value;
}

sl_bool LabelView::isBorder()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		m_flagBorder = tv.bordered ? sl_true : sl_false;
	}
	return m_flagBorder;
}

void LabelView::setBorder(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		[tv setBordered:(flag?TRUE:FALSE)];
	}
	m_flagBorder = flag;
}

Alignment LabelView::getTextAlignment()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		m_textAlignment = _LabelView::translateAlignmentReverse(tv.alignment);
	}
	return m_textAlignment;
}

void LabelView::setTextAlignment(Alignment align)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		[tv setAlignment:_LabelView::translateAlignment(align)];
	}
	m_textAlignment = align;
}

Color LabelView::getTextColor()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		m_textColor = UIPlatform::getColorFromNSColor(tv.textColor);
	}
	return m_textColor;
}

void LabelView::setTextColor(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		[tv setTextColor:(UIPlatform::getNSColorFromColor(color))];
	}
	m_textColor = color;
}

Color LabelView::getBackgroundColor()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		m_backgroundColor = UIPlatform::getColorFromNSColor(tv.backgroundColor);
	}
	return m_backgroundColor;
}

void LabelView::setBackgroundColor(const Color& color)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		[tv setBackgroundColor:(UIPlatform::getNSColorFromColor(color))];
	}
	m_backgroundColor = color;
}

void LabelView::setFont(const Ref<Font>& font)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
		NSTextField* tv = (NSTextField*)handle;
		
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		NSFont* hFont = UIPlatform::getNSFont(font.ptr, fontInstance);
		if (hFont != nil) {
			[tv setFont:hFont];
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

@implementation _Slib_OSX_LabelTextField
-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
	}
	return self;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onClick();
	}
}
@end

#endif
