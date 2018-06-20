/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/label_view.h"

#include "view_ios.h"

@interface _priv_Slib_iOS_LabelTextField : UILabel {
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
	
}
@end

namespace slib
{
	class _priv_LabelView : public LabelView
	{
	public:
		void applyProperties(UILabel* handle)
		{
			if (![NSThread isMainThread]) {
				dispatch_async(dispatch_get_main_queue(), ^{
					applyProperties(handle);
				});
				return;
			}
			
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
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
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
		_priv_Slib_iOS_LabelTextField* handle = [[_priv_Slib_iOS_LabelTextField alloc] initWithFrame:frame];
		if (handle != nil) {
			((_priv_LabelView*)this)->applyProperties(handle);
		}
		IOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}
	
	void LabelView::_setText_NW(const String& value)
	{
		if (![NSThread isMainThread]) {
			String _value = value;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setText_NW(_value);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
			UILabel* tv = (UILabel*)handle;
			[tv setText:(Apple::getNSStringFromString(value))];
		}
	}
	
	void LabelView::_setTextAlignment_NW(Alignment align)
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_setTextAlignment_NW(align);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
			UILabel* tv = (UILabel*)handle;
			[tv setTextAlignment:_priv_LabelView::translateAlignment(align)];
		}
	}
	
	void LabelView::_setTextColor_NW(const Color& color)
	{
		if (![NSThread isMainThread]) {
			Color _color = color;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setTextColor_NW(color);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
			UILabel* tv = (UILabel*)handle;
			[tv setTextColor:(GraphicsPlatform::getUIColorFromColor(color))];
		}
	}
	
	void LabelView::_setFont_NW(const Ref<Font>& font)
	{
		if (![NSThread isMainThread]) {
			Ref<Font> _font = font;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setFont_NW(_font);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
			UILabel* tv = (UILabel*)handle;
			UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
			if (hFont != nil) {
				[tv setFont:hFont];
			}
		}
	}
	
	void LabelView::_setBorder_NW(sl_bool flag)
	{
		if (![NSThread isMainThread]) {
			dispatch_async(dispatch_get_main_queue(), ^{
				_setBorder_NW(flag);
			});
			return;
		}
		
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
		if (![NSThread isMainThread]) {
			Color _color = color;
			dispatch_async(dispatch_get_main_queue(), ^{
				_setBackgroundColor_NW(_color);
			});
			return;
		}
		
		UIView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[UILabel class]]) {
			UILabel* tv = (UILabel*)handle;
			[tv setBackgroundColor:(GraphicsPlatform::getUIColorFromColor(color))];
		}
	}	
}

@implementation _priv_Slib_iOS_LabelTextField
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
