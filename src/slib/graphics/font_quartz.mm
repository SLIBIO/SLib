/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_QUARTZ)

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"

#if defined(SLIB_PLATFORM_IS_MACOS)
typedef NSFontDescriptor UIFontDescriptor;
#endif

namespace slib
{

	namespace priv
	{
		namespace quartz
		{

			class FontPlatformObject : public Referable
			{
			public:
				UIFont* m_font;
				sl_bool m_flagCreatedFont;
				CGFloat m_lastUIScaleFactor;
				
				SpinLock m_lock;
				
			public:
				FontPlatformObject()
				{
					m_font = nil;
					m_flagCreatedFont = sl_false;
					m_lastUIScaleFactor = 0;
				}
				
				~FontPlatformObject()
				{
					m_font = nil;
				}
				
			public:
				UIFont* _createFont(const FontDesc& desc, CGFloat scaleFactor)
				{
					SpinLocker lock(&m_lock);
					
					if (m_flagCreatedFont && m_lastUIScaleFactor == scaleFactor) {
						return m_font;
					}
					
					m_flagCreatedFont = sl_true;
					
					float size = desc.size / scaleFactor;
					NSString* familyName = Apple::getNSStringFromString(desc.familyName);
					uint32_t traits = 0;
					UIFontDescriptor* descriptor = [UIFontDescriptor fontDescriptorWithName:familyName size:size];
					if (descriptor == nil) {
						return nil;
					}
#if defined(SLIB_PLATFORM_IS_MACOS)
					if (desc.flagBold) {
						traits |= NSFontBoldTrait;
					}
					if (desc.flagItalic) {
						traits |= NSFontItalicTrait;
					}
#else
					if (desc.flagBold) {
						traits |= UIFontDescriptorTraitBold;
					}
					if (desc.flagItalic) {
						traits |= UIFontDescriptorTraitItalic;
					}
#endif
					if (traits) {
						UIFontDescriptor* descriptorWithTraits = [descriptor fontDescriptorWithSymbolicTraits:traits];
						if (descriptorWithTraits != nil) {
							descriptor = descriptorWithTraits;
						}
					}
					m_font = [UIFont fontWithDescriptor:descriptor size:size];
					m_lastUIScaleFactor = scaleFactor;
					return m_font;
				}

			};

			class FontHelper : public Font
			{
			public:
				FontPlatformObject* getPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new FontPlatformObject;
						}
					}
					return (FontPlatformObject*)(m_platformObject.get());;
				}
								
				UIFont* getFontObject(CGFloat scaleFactor)
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						return po->_createFont(m_desc, scaleFactor);
					}
					return nil;
				}
				
			};

		}
	}
	
	using namespace priv::quartz;

	sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
	{
		UIFont* hFont = GraphicsPlatform::getNativeFont(this);
		if (hFont == nil) {
			return sl_false;
		}
		_out.ascent = hFont.ascender;
		_out.descent = hFont.descender;
		_out.leading = hFont.leading;
		return sl_true;
	}

	Size Font::_measureText_PO(const StringParam& _text)
	{
		UIFont* hFont = GraphicsPlatform::getNativeFont(this);
		if (hFont == nil) {
			return Size::zero();
		}
		NSString* text = Apple::getNSStringFromString(_text);
		if (text == nil) {
			return Size::zero();
		}
		NSAttributedString* attrText = [[NSAttributedString alloc] initWithString:text attributes:@{NSFontAttributeName: hFont}];
		if (text == nil) {
			return Size::zero();
		}
		Size ret(0, 0);
#if defined(SLIB_PLATFORM_IS_MACOS)
		NSRect bounds = [attrText boundingRectWithSize:NSMakeSize(CGFLOAT_MAX, CGFLOAT_MAX) options:0 context:nil];
#else
		CGRect bounds = [attrText boundingRectWithSize:CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX) options:0 context:nil];
#endif
		ret.x = (sl_real)(bounds.size.width);
		ret.y = (sl_real)(bounds.size.height);
		return ret;
	}

	UIFont* GraphicsPlatform::getNativeFont(Font* _font, CGFloat scaleFactor)
	{
		if (_font) {
			FontHelper* font = (FontHelper*)_font;
			return font->getFontObject(scaleFactor);
		}
		return nil;
	}

}

#endif
