#ifndef CHECKHEADER_SLIB_GRAPHICS_BITMAP
#define CHECKHEADER_SLIB_GRAPHICS_BITMAP

#include "definition.h"
#include "color.h"
#include "drawable.h"
#include "canvas.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class GraphicsContext;

struct SLIB_EXPORT BitmapDesc
{
	sl_uint32 width;
	sl_uint32 height;
	ColorModel colorModel;
	void* data;
	sl_uint32 pitch;
	
	Ref<Referable> ref;
};

class SLIB_EXPORT Bitmap : public Drawable
{
	SLIB_DECLARE_OBJECT(Bitmap, Drawable)
protected:
	Bitmap();

public:
	virtual sl_uint32 getBitmapWidth() = 0;
	virtual sl_uint32 getBitmapHeight() = 0;
	
	virtual sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapDesc& desc) = 0;
	virtual sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapDesc& desc) = 0;
	virtual sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color) = 0;
	
	virtual Ref<Canvas> getCanvas() = 0;
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc);
	
public:
	sl_uint32 getWidth();
	sl_uint32 getHeight();
	
	void update();
	
public:
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, ColorModel model, void* data, sl_uint32 pitch = 0);
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_uint32 stride = 0);

	sl_bool writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_uint32 stride = 0);
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, ColorModel model, const void* data, sl_uint32 pitch = 0);
	
	sl_bool resetPixels(const Color& color);
	
protected:
	Ref<Drawable> m_drawableCached;
	WeakRef<GraphicsContext> m_contextCached;
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
