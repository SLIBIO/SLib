#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/font.h"
#include "../../../inc/slib/graphics/platform.h"


#if defined(SLIB_PLATFORM_IS_OSX)
typedef NSFont UIFont;
#endif

SLIB_GRAPHICS_NAMESPACE_BEGIN

sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
{
	CTFontRef handle = GraphicsPlatform::getCoreTextFont(this);
	if (!handle) {
		return sl_false;
	}
	_out.ascent = CTFontGetAscent(handle);
	_out.descent = CTFontGetDescent(handle);
	_out.leading = CTFontGetLeading(handle);
	return sl_true;
}

Size Font::_measureText_PO(const String& text)
{
	CTFontRef handle = GraphicsPlatform::getCoreTextFont(this);
	if (!handle) {
		return Size::zero();
	}
	
	Size ret(0, 0);
	
	NSString* ns_text = Apple::getNSStringFromString(text);
	
	CFStringRef string = (__bridge CFStringRef)ns_text;
	
	CFStringRef keys[] = { kCTFontAttributeName };
	CFTypeRef values[] = { handle };
	CFDictionaryRef attributes = CFDictionaryCreate(
													kCFAllocatorDefault
													, (const void**)&keys, (const void**)&values
													, sizeof(keys) / sizeof(keys[0])
													, &kCFCopyStringDictionaryKeyCallBacks
													, &kCFTypeDictionaryValueCallBacks);
	if (attributes) {
		CFAttributedStringRef attrString = CFAttributedStringCreate(kCFAllocatorDefault, string, attributes);
		if (attrString) {
			CTLineRef line = CTLineCreateWithAttributedString(attrString);
			if (line) {
				CGRect rect = CTLineGetBoundsWithOptions(line, 0);
				ret.x = (sl_real)(rect.size.width);
				ret.y = (sl_real)(rect.size.height);
				CFRelease(line);
			}
			CFRelease(attrString);
		}
		CFRelease(attributes);
	}
	return ret;
}

class _Apple_FontObject : public Referable
{
public:
	CTFontRef m_fontCoreText;
	sl_bool m_flagCreatedCoreText;
	
	UIFont* m_fontUI;
	sl_bool m_flagCreatedUI;
	CGFloat m_lastUIScaleFactor;
	
	SpinLock m_lock;
	
public:
	_Apple_FontObject()
	{
		m_fontCoreText = nil;
		m_flagCreatedCoreText = sl_false;
		
		m_fontUI = nil;
		m_flagCreatedUI = sl_false;
		m_lastUIScaleFactor = 0;
	}
	
	~_Apple_FontObject()
	{
		if (m_fontCoreText) {
			CFRelease(m_fontCoreText);
		}
		m_fontUI = nil;
	}
	
public:
	void _createCoreText(const FontDesc& desc)
	{
		if (m_flagCreatedCoreText) {
			return;
		}
		
		SpinLocker lock(&m_lock);
		
		if (m_flagCreatedCoreText) {
			return;
		}
		
		m_flagCreatedCoreText = sl_true;
		
		float size = desc.size;
		
		NSMutableDictionary* attributes = [NSMutableDictionary dictionary];
		NSString* familyName = Apple::getNSStringFromString(desc.familyName);
		[attributes setObject:familyName forKey:(id)kCTFontFamilyNameAttribute];
		
		NSMutableDictionary* traits = [NSMutableDictionary dictionary];
		CTFontSymbolicTraits symbolicTraits = 0;
		if (desc.flagBold) {
			symbolicTraits |= kCTFontTraitBold;
		}
		if (desc.flagItalic) {
			symbolicTraits |= kCTFontTraitItalic;
		}
		[traits setObject:[NSNumber numberWithUnsignedInt:symbolicTraits] forKey:(id)kCTFontSymbolicTrait];
		[attributes setObject:traits forKey:(id)kCTFontTraitsAttribute];
		
		[attributes setObject:[NSNumber numberWithFloat:size] forKey:(id)kCTFontSizeAttribute];
		
		CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes((CFDictionaryRef)attributes);
		if (descriptor) {
			CTFontRef font = CTFontCreateWithFontDescriptor(descriptor, size, NULL);
			if (font) {
				m_fontCoreText = font;
			}
			CFRelease(descriptor);
		}
	}
	
	
	UIFont* _createUI(const FontDesc& desc, CGFloat scaleFactor)
	{
		SpinLocker lock(&m_lock);
		
		if (m_flagCreatedUI && m_lastUIScaleFactor == scaleFactor) {
			return m_fontUI;
		}
		
		m_flagCreatedUI = sl_true;
		
		float size = desc.size / scaleFactor;
		NSString* familyName = Apple::getNSStringFromString(desc.familyName);
		uint32_t traits = 0;
#if defined(SLIB_PLATFORM_IS_OSX)
		NSFontDescriptor* nsFontDesc = [NSFontDescriptor fontDescriptorWithName:familyName size:size];
		if (nsFontDesc == nil) {
			return nil;
		}
		if (desc.flagBold) {
			traits |= NSFontBoldTrait;
		}
		if (desc.flagItalic) {
			traits |= NSFontItalicTrait;
		}
		nsFontDesc = [nsFontDesc fontDescriptorWithSymbolicTraits:traits];
		if (nsFontDesc == nil) {
			return nil;
		}
		m_fontUI = [NSFont fontWithDescriptor:nsFontDesc size:size];
#elif defined(SLIB_PLATFORM_IS_IOS)
		UIFontDescriptor* uiFontDesc = [UIFontDescriptor fontDescriptorWithName:familyName size:size];
		if (uiFontDesc == nil) {
			return nil;
		}
		if (desc.flagBold) {
			traits |= UIFontDescriptorTraitBold;
		}
		if (desc.flagItalic) {
			traits |= UIFontDescriptorTraitItalic;
		}
		uiFontDesc = [uiFontDesc fontDescriptorWithSymbolicTraits:traits];
		if (uiFontDesc == nil) {
			return nil;
		}
		m_fontUI = [UIFont fontWithDescriptor:uiFontDesc size:size];
#endif
		
		m_lastUIScaleFactor = scaleFactor;
		return m_fontUI;
	}

};

class _Font : public Font
{
public:
	_Apple_FontObject* getPlatformObject()
	{
		if (m_platformObject.isNull()) {
			SpinLocker lock(&m_lock);
			if (m_platformObject.isNull()) {
				m_platformObject = new _Apple_FontObject;
			}
		}
		return (_Apple_FontObject*)(m_platformObject.get());;
	}
	
	CTFontRef getCoreText()
	{
		_Apple_FontObject* po = getPlatformObject();
		if (po) {
			po->_createCoreText(m_desc);
			return po->m_fontCoreText;
		}
		return NULL;
	}
	
	UIFont* getUI(CGFloat scaleFactor)
	{
		_Apple_FontObject* po = getPlatformObject();
		if (po) {
			return po->_createUI(m_desc, scaleFactor);
		}
		return nil;
	}
	
};

CTFontRef GraphicsPlatform::getCoreTextFont(Font* _font)
{
	if (_font) {
		_Font* font = (_Font*)_font;
		return font->getCoreText();
	}
	return NULL;
}

#if defined(SLIB_PLATFORM_IS_OSX)
NSFont* GraphicsPlatform::getNSFont(Font* _font, CGFloat scaleFactor)
#elif defined(SLIB_PLATFORM_IS_IOS)
UIFont* GraphicsPlatform::getUIFont(Font* _font, CGFloat scaleFactor)
#endif
{
	if (_font) {
		_Font* font = (_Font*)_font;
		return font->getUI(scaleFactor);
	}
	return nil;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
