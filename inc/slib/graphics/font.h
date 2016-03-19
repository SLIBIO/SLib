#ifndef CHECKHEADER_SLIB_GRAPHICS_FONT
#define CHECKHEADER_SLIB_GRAPHICS_FONT

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../math/size.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT FontDesc
{
public:
	String familyName;
	sl_real size;
	sl_bool flagBold;
	sl_bool flagItalic;
	sl_bool flagUnderline;

public:
	FontDesc();
};

class GraphicsContext;

class FontInstance;

class SLIB_EXPORT Font : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Font() = default;
	
public:
	static Ref<Font> getDefault();
	
	static Ref<Font> create(const FontDesc& desc);
	
	static Ref<Font> create(String familyName, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false);

	static sl_real getDefaultFontSize();
	
	static void setDefaultFontSize(sl_real size);
	
public:
	void getDesc(FontDesc& desc);
	
	String getFamilyName();
	
	sl_real getSize();
	
	sl_bool isBold();
	
	sl_bool isItalic();
	
	sl_bool isUnderline();
	
	Size getTextSize(const Ref<GraphicsContext>& context, const String& text);

public:
	SLIB_REF_PROPERTY(FontInstance, Instance)
	
protected:
	FontDesc m_desc;
	
};

class SLIB_EXPORT FontInstance : public Object
{
	SLIB_DECLARE_OBJECT
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
