#ifndef CHECKHEADER_SLIB_GRAPHICS_BRUSH
#define CHECKHEADER_SLIB_GRAPHICS_BRUSH

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT BrushDesc
{
public:
	BrushStyle style;
	Color color;
	
public:
	BrushDesc();
};

class BrushInstance;

class SLIB_EXPORT Brush : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Brush();
	
public:
	static Ref<Brush> getDefault();
	
	static Ref<Brush> create(const BrushDesc& desc);
	
	static Ref<Brush> createSolidBrush(const Color& color);
	
public:
	void getDesc(BrushDesc& desc);
	
	BrushStyle getStyle();
	
	Color getColor();
	
public:
	SLIB_REF_PROPERTY(BrushInstance, Instance)
	
protected:
	BrushDesc m_desc;
	
};

class SLIB_EXPORT BrushInstance : public Object
{
	SLIB_DECLARE_OBJECT
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
