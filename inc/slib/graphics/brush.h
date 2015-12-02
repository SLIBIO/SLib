#ifndef CHECKHEADER_SLIB_GRAPHICS_BRUSH
#define CHECKHEADER_SLIB_GRAPHICS_BRUSH

#include "definition.h"
#include "constants.h"

#include "color.h"
#include "../core/object.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

struct SLIB_EXPORT BrushDesc
{
	BrushStyle style;
	Color color;
	
	BrushDesc();
};

class BrushInstance;

class SLIB_EXPORT Brush : public Object
{
	SLIB_DECLARE_OBJECT(Brush, Object)
protected:
	Brush();
	
public:
	static Ref<Brush> getDefault();
	static Ref<Brush> create(const BrushDesc& desc);
	static Ref<Brush> createSolidBrush(Color color);
	
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
	const Ref<BrushInstance>& getInstance();
	void setInstance(const Ref<BrushInstance>& instance);

protected:
	BrushDesc m_desc;
	Ref<BrushInstance> m_instance;
};

class SLIB_EXPORT BrushInstance : public Object
{
	SLIB_DECLARE_OBJECT(BrushInstance, Object)
protected:
	BrushInstance();

};

SLIB_GRAPHICS_NAMESPACE_END

#endif
