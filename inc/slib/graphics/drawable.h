#ifndef CHECKHEADER_SLIB_GRAPHICS_DRAWABLE
#define CHECKHEADER_SLIB_GRAPHICS_DRAWABLE

#include "definition.h"

#include "color.h"
#include "pen.h"
#include "brush.h"

#include "../core/object.h"
#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Canvas;

class BrushDrawable;
class ColorDrawable;

class SLIB_EXPORT Drawable : public Object
{
	SLIB_DECLARE_OBJECT

public:
	virtual sl_real getDrawableWidth();
	
	virtual sl_real getDrawableHeight();
	
	virtual Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);
	
	virtual Ref<Drawable> scaleDrawable(sl_real width, sl_real height);
	
	virtual void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
	virtual void onDrawAll(Canvas* canvas, const Rectangle& rectDst);
	
public:
	sl_bool isBitmap();
	
	sl_bool isImage();
	
public:
	static Ref<BrushDrawable> createBrushDrawable(const Ref<Brush>& brush);
	
	static Ref<ColorDrawable> createColorDrawable(const Color& color);
	
	static Ref<Drawable> createEmptyDrawable();
	
	static Ref<Drawable> createSubDrawable(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height);
	
	static Ref<Drawable> createSubDrawable(const Ref<Drawable>& src, const Rectangle& rectSrc);
	
	static Ref<Drawable> createScaledDrawable(const Ref<Drawable>& src, sl_real width, sl_real height);
	
	static Ref<Drawable> createScaledDrawable(const Ref<Drawable>& src, const Size& size);
	
};

class SLIB_EXPORT BrushDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<BrushDrawable> create(const Ref<Brush>& brush);
	
public:
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);

protected:
	Ref<Brush> m_brush;
	
};

class SLIB_EXPORT ColorDrawable : public BrushDrawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<ColorDrawable> create(const Color& color);
	
};

class SLIB_EXPORT EmptyDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT

public:
	static Ref<EmptyDrawable> create();
	
public:
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
protected:
	Ref<Brush> m_brush;
};

class SLIB_EXPORT RectangleDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<RectangleDrawable> create(const Ref<Pen>& pen, const Ref<Brush>& brush);
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst);
	
protected:
	Ref<Pen> m_pen;
	Ref<Brush> m_brush;
	
};

class SLIB_EXPORT SubDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<SubDrawable> create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height);
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);

	// override
	Ref<Drawable> scaleDrawable(sl_real width, sl_real height);
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst);
	
protected:
	Ref<Drawable> m_src;
	sl_real m_x;
	sl_real m_y;
	sl_real m_width;
	sl_real m_height;

};

class SLIB_EXPORT ScaledDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<ScaledDrawable> create(const Ref<Drawable>& src, sl_real width, sl_real height);
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);
	
	// override
	Ref<Drawable> scaleDrawable(sl_real width, sl_real height);
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst);
	
protected:
	Ref<Drawable> m_src;
	sl_real m_width;
	sl_real m_height;
	
};

class SLIB_EXPORT ScaledSubDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<ScaledSubDrawable> create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height);
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);
	
	// override
	Ref<Drawable> scaleDrawable(sl_real width, sl_real height);
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst);
	
protected:
	Ref<Drawable> m_src;
	Rectangle m_rectSrc;
	sl_real m_width;
	sl_real m_height;
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
