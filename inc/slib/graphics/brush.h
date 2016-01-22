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
	SLIB_DECLARE_OBJECT(Brush, Object)
protected:
	SLIB_INLINE Brush()
	{
	}
	
public:
	static Ref<Brush> getDefault();
	
	static Ref<Brush> create(const BrushDesc& desc);
	
	static Ref<Brush> createSolidBrush(const Color& color);
	
public:
	SLIB_INLINE void getDesc(BrushDesc& desc)
	{
		desc = m_desc;
	}

	SLIB_INLINE BrushStyle getStyle()
	{
		return m_desc.style;
	}

	SLIB_INLINE Color getColor()
	{
		return m_desc.color;
	}

public:
	SLIB_REF_PROPERTY(BrushInstance, Instance)

protected:
	BrushDesc m_desc;
	
};

class SLIB_EXPORT BrushInstance : public Object
{
	SLIB_DECLARE_OBJECT(BrushInstance, Object)
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
