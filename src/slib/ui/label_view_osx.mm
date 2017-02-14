#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/label_view.h"

#include "view_osx.h"

@interface _Slib_OSX_LabelTextField : NSTextField {
	
	@public slib::WeakRef<slib::OSX_ViewInstance> m_viewInstance;
	
}
@end

namespace slib
{

	class _LabelView : public LabelView
	{
	public:
		void __applyProperties(NSTextField* handle)
		{
			[handle setStringValue:(Apple::getNSStringFromString(m_text))];
			[handle setAlignment:translateAlignment(m_textAlignment)];
			[handle setBordered: (isBorder() ? TRUE : FALSE)];
			[handle setBezeled: (isBorder() ? TRUE : FALSE)];
			[handle setTextColor:(GraphicsPlatform::getNSColorFromColor(m_textColor))];
			[handle setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(getBackgroundColor()))];
			
			Ref<Font> font = getFont();
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
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

	Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* _parent)
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

	void LabelView::_setText_NW(const String& value)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setStringValue:(Apple::getNSStringFromString(value))];
		}
	}

	void LabelView::_setTextAlignment_NW(Alignment align)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setAlignment:_LabelView::translateAlignment(align)];
		}
	}

	void LabelView::_setTextColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setTextColor:(GraphicsPlatform::getNSColorFromColor(color))];
		}
	}

	void LabelView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
	}

	void LabelView::_setBorder_NW(sl_bool flag)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBordered:(flag?TRUE:FALSE)];
		}
	}

	void LabelView::_setBackgroundColor_NW(const Color& color)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSTextField class]]) {
			NSTextField* tv = (NSTextField*)handle;
			[tv setBackgroundColor:(GraphicsPlatform::getNSColorFromColor(color))];
		}
	}

}

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
