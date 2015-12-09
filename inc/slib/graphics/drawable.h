#ifndef CHECKHEADER_SLIB_GRAPHICS_DRAWABLE
#define CHECKHEADER_SLIB_GRAPHICS_DRAWABLE

#include "definition.h"
#include "color.h"
#include "brush.h"

#include "../core/object.h"
#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Canvas;

class BrushDrawable;
class ColorDrawable;

class SLIB_EXPORT Drawable : public Object
{
	SLIB_DECLARE_OBJECT(Drawable, Object)
protected:
	Drawable();
	
public:
	virtual sl_real getDrawableWidth() = 0;
	virtual sl_real getDrawableHeight() = 0;
	virtual void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc) = 0;
	
public:
	static Ref<BrushDrawable> createBrushDrawable(const Ref<Brush>& brush);
	static Ref<ColorDrawable> createColorDrawable(const Color& color);
	static Ref<Drawable> createEmptyDrawable();
	
public:
	sl_bool isBitmap();
	sl_bool isImage();
};

class SLIB_EXPORT BrushDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT(BrushDrawable, Drawable)
protected:
	BrushDrawable();

public:
	static Ref<BrushDrawable> create(const Ref<Brush>& brush);
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);

protected:
	Ref<Brush> m_brush;
};

class SLIB_EXPORT ColorDrawable : public BrushDrawable
{
	SLIB_DECLARE_OBJECT(ColorDrawable, BrushDrawable)
protected:
	ColorDrawable();
	
public:
	static Ref<ColorDrawable> create(const Color& color);
};

class SLIB_EXPORT EmptyDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT(EmptyDrawable, Drawable)
protected:
	EmptyDrawable();
	
public:
	static Ref<EmptyDrawable> create();
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
protected:
	Ref<Brush> m_brush;
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
