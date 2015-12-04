#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/font.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

class _Apple_FontInstance : public FontInstance
{
	SLIB_DECLARE_OBJECT(_Apple_FontInstance, FontInstance);
public:
	FontDesc m_desc;
	
	CTFontRef m_fontCoreText;
	sl_bool m_flagCreatedCoreText;
	
#if defined(SLIB_PLATFORM_IS_OSX)
	NSFont* m_fontUI;
#elif defined(SLIB_PLATFORM_IS_IOS)
	UIFont* m_fontUI;
#endif
	sl_bool m_flagCreatedUI;
	
	_Apple_FontInstance()
	{
		m_fontCoreText = nil;
		m_flagCreatedCoreText = sl_false;
		
		m_fontUI = nil;
		m_flagCreatedUI = sl_false;
	}
	
	~_Apple_FontInstance()
	{
		if (m_fontCoreText) {
			CFRelease(m_fontCoreText);
		}
		m_fontUI = nil;
	}
	
	static Ref<_Apple_FontInstance> _create(const FontDesc& desc)
	{
		Ref<_Apple_FontInstance> ret;
		ret = new _Apple_FontInstance();
		if (ret.isNotNull()) {
			ret->m_desc = desc;
		}
		return ret;
	}
	
	void _createCoreText()
	{
		MutexLocker lock(getLocker());
		if (m_flagCreatedCoreText) {
			return;
		}
		
		m_flagCreatedCoreText = sl_true;
		
		const FontDesc& desc = m_desc;
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
	
	void _createUI()
	{
		MutexLocker lock(getLocker());
		if (m_flagCreatedUI) {
			return;
		}
		
		m_flagCreatedUI = sl_true;
		
		const FontDesc& desc = m_desc;
		float size = desc.size;
		NSString* familyName = Apple::getNSStringFromString(desc.familyName);
		uint32_t traits = 0;
#if defined(SLIB_PLATFORM_IS_OSX)
		NSFontDescriptor* nsFontDesc = [NSFontDescriptor fontDescriptorWithName:familyName size:size];
		if (nsFontDesc == nil) {
			return;
		}
		if (desc.flagBold) {
			traits |= NSFontBoldTrait;
		}
		if (desc.flagItalic) {
			traits |= NSFontItalicTrait;
		}
		nsFontDesc = [nsFontDesc fontDescriptorWithSymbolicTraits:traits];
		if (nsFontDesc == nil) {
			return;
		}
		m_fontUI = [NSFont fontWithDescriptor:nsFontDesc size:size];
#elif defined(SLIB_PLATFORM_IS_IOS)
		UIFontDescriptor* uiFontDesc = [UIFontDescriptor fontDescriptorWithName:familyName size:size];
		if (uiFontDesc == nil) {
			return;
		}
		if (desc.flagBold) {
			traits |= UIFontDescriptorTraitBold;
		}
		if (desc.flagItalic) {
			traits |= UIFontDescriptorTraitItalic;
		}
		uiFontDesc = [uiFontDesc fontDescriptorWithSymbolicTraits:traits];
		if (uiFontDesc == nil) {
			return;
		}
		m_fontUI = [UIFont fontWithDescriptor:uiFontDesc size:size];
#endif
	}
	
	static Ref<_Apple_FontInstance> getInstance(Font* font)
	{
		Ref<_Apple_FontInstance> ret;
		if (font) {
			Ref<FontInstance> _instance = font->getInstance();
			if (_Apple_FontInstance::checkInstance(_instance)) {
				ret = Ref<_Apple_FontInstance>::from(_instance);
			} else {
				FontDesc desc;
				font->getDesc(desc);
				ret = _create(desc);
				if (ret.isNotNull()) {
					font->setInstance(ret);
				}
			}
		}
		return ret;
	}
};



CTFontRef UIPlatform::getCoreTextFont(Font* font, Ref<FontInstance>& instanceOut)
{
	Ref<_Apple_FontInstance> instance = _Apple_FontInstance::getInstance(font);
	instanceOut = instance;
	if (instance.isNotNull()) {
		instance->_createCoreText();
		return instance->m_fontCoreText;
	}
	return nil;
}

#if defined(SLIB_PLATFORM_IS_OSX)
NSFont* UIPlatform::getNSFont(Font* font, Ref<FontInstance>& instanceOut)
#elif defined(SLIB_PLATFORM_IS_IOS)
UIFont* UIPlatform::getUIFont(Font* font, Ref<FontInstance>& instanceOut)
#endif
{
	Ref<_Apple_FontInstance> instance = _Apple_FontInstance::getInstance(font);
	instanceOut = instance;
	if (instance.isNotNull()) {
		instance->_createUI();
		return instance->m_fontUI;
	}
	return nil;
}

Size UI::getFontTextSize(const Ref<Font>& _font, const String& _text)
{
	Size ret(0, 0);
	
	Ref<Font> font = _font;
	if (font.isNull()) {
		return ret;
	}
	
	Ref<FontInstance> instance;
	CTFontRef handle = UIPlatform::getCoreTextFont(font.get(), instance);
	if (!handle) {
		return ret;
	}
	
	NSString* text = Apple::getNSStringFromString(_text);
	CFStringRef string = (__bridge CFStringRef)text;
	
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
				CGRect rect = CTLineGetBoundsWithOptions(line, kCTLineBoundsUseGlyphPathBounds);
				ret.width = (sl_real)(rect.size.width);
				ret.height = (sl_real)(rect.size.height);
				CFRelease(line);
			}
			CFRelease(attrString);
		}
		CFRelease(attributes);
	}
	return ret;
}


SLIB_UI_NAMESPACE_END

#endif
