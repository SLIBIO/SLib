#ifndef CHECKHEADER_SLIB_GRAPHICS_FONT
#define CHECKHEADER_SLIB_GRAPHICS_FONT

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../math/size.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

struct SLIB_EXPORT FontDesc
{
	String familyName;
	sl_real size;
	sl_bool flagBold;
	sl_bool flagItalic;
	sl_bool flagUnderline;
	
	FontDesc();
};

class GraphicsContext;

class FontInstance;

class SLIB_EXPORT Font : public Object
{
	SLIB_DECLARE_OBJECT(Font, Object)
protected:
	Font();
	
public:
	static Ref<Font> getDefault();
	static Ref<Font> create(const FontDesc& desc);
	static Ref<Font> create(String familyName, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false);

	static sl_real getDefaultFontSize();
	static void setDefaultFontSize(sl_real size);
	
public:
	SLIB_INLINE void getDesc(FontDesc& desc)
	{
		desc = m_desc;
	}
	
	SLIB_INLINE String getFamilyName()
	{
		return m_desc.familyName;
	}
	
	SLIB_INLINE sl_real getSize()
	{
		return m_desc.size;
	}
	
	SLIB_INLINE sl_bool isBold()
	{
		return m_desc.flagBold;
	}
	
	SLIB_INLINE sl_bool isItalic()
	{
		return m_desc.flagItalic;
	}
	
	SLIB_INLINE sl_bool isUnderline()
	{
		return m_desc.flagUnderline;
	}
	
public:
	Size getTextSize(const Ref<GraphicsContext>& context, const String& text);

public:
	const Ref<FontInstance>& getInstance();
	void setInstance(const Ref<FontInstance>& instance);
	
protected:
	FontDesc m_desc;
	Ref<FontInstance> m_instance;
};

class SLIB_EXPORT FontInstance : public Object
{
	SLIB_DECLARE_OBJECT(FontInstance, Object)
protected:
	FontInstance();
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
