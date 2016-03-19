#ifndef CHECKHEADER_SLIB_GRAPHICS_PEN
#define CHECKHEADER_SLIB_GRAPHICS_PEN

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT PenDesc
{
public:
	PenStyle style;
	LineCap cap;
	LineJoin join;
	sl_real width;
	Color color;
	sl_real miterLimit;
	
public:
	PenDesc();
};

class PenInstance;

class SLIB_EXPORT Pen : public Object
{
	SLIB_DECLARE_OBJECT;
	
protected:
	Pen() = default;
	
public:
	static Ref<Pen> getDefault();
	
	static Ref<Pen> create(const PenDesc& desc);
	
	static Ref<Pen> create(PenStyle style, sl_real width = 1, Color color = Color::Black);
	
	static Ref<Pen> createSolidPen(sl_real width = 1, Color color = Color::Black);
	
public:
	void getDesc(PenDesc& desc);

	PenStyle getStyle();

	sl_real getWidth();

	Color getColor();

	LineCap getCap();

	LineJoin getJoin();

	sl_real getMiterLimit();

public:
	SLIB_REF_PROPERTY(PenInstance, Instance)

protected:
	PenDesc m_desc;
	
};

class SLIB_EXPORT PenInstance : public Object
{
	SLIB_DECLARE_OBJECT
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
