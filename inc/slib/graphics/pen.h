#ifndef CHECKHEADER_SLIB_GRAPHICS_PEN
#define CHECKHEADER_SLIB_GRAPHICS_PEN

#include "definition.h"
#include "constants.h"

#include "color.h"
#include "../core/object.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
struct SLIB_EXPORT PenDesc
{
	PenStyle style;
	LineCap cap;
	LineJoin join;
	sl_real width;
	Color color;
	sl_real miterLimit;
	
	PenDesc();
};

class PenInstance;

class SLIB_EXPORT Pen : public Object
{
	SLIB_DECLARE_OBJECT(Pen, Object)
protected:
	Pen();
	
public:
	static Ref<Pen> getDefault();
	static Ref<Pen> create(const PenDesc& desc);
	static Ref<Pen> create(PenStyle style, sl_real width = 1, Color color = Color::Black);
	static Ref<Pen> createSolidPen(sl_real width = 1, Color color = Color::Black);
	
public:
	SLIB_INLINE void getDesc(PenDesc& desc)
	{
		desc = m_desc;
	}

	SLIB_INLINE PenStyle getStyle()
	{
		return m_desc.style;
	}

	SLIB_INLINE sl_real getWidth()
	{
		return m_desc.width;
	}

	SLIB_INLINE Color getColor()
	{
		return m_desc.color;
	}

	SLIB_INLINE LineCap getCap()
	{
		return m_desc.cap;
	}

	SLIB_INLINE LineJoin getJoin()
	{
		return m_desc.join;
	}

	SLIB_INLINE sl_real getMiterLimit()
	{
		return m_desc.miterLimit;
	}

public:
	const Ref<PenInstance>& getInstance();
	void setInstance(const Ref<PenInstance>& instance);

protected:
	PenDesc m_desc;
	Ref<PenInstance> m_instance;
};

class SLIB_EXPORT PenInstance : public Object
{
	SLIB_DECLARE_OBJECT(PenInstance, Object)
protected:
	PenInstance();
};
SLIB_GRAPHICS_NAMESPACE_END

#endif
