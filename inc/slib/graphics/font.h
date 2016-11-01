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

struct SLIB_EXPORT FontMetrics
{
	sl_real ascent;
	sl_real descent;
	sl_real leading;
};

class SLIB_EXPORT Font : public Referable
{
	SLIB_DECLARE_OBJECT
	
protected:
	Font();
	
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
	
	sl_real getFontHeight();
	
	sl_real getFontAscent();
	
	sl_real getFontDescent();
	
	Ref<Referable> getPlatformObject();
	
	Size getTextSize(const String& text);
	
	sl_bool getFontMetrics(FontMetrics& _out);
	
protected:
	FontDesc m_desc;
	
	Ref<Referable> m_platformObject;
	SpinLock m_lock;
	
	SafeRef<Referable> m_renderingCache;
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
