#ifndef CHECKHEADER_SLIB_GRAPHICS_CONTEXT
#define CHECKHEADER_SLIB_GRAPHICS_CONTEXT

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"
#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Font;
class FontInstance;
class GraphicsPath;
class GraphicsPathInstance;
class Drawable;
class Bitmap;
struct ImageDesc;
class Image;
class Canvas;

class SLIB_EXPORT GraphicsContext : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	virtual Size getFontTextSize(const Ref<Font>& font, const String& text) = 0;
	
	
	virtual Rectangle getPathBounds(const Ref<GraphicsPath>& path) = 0;
	
	virtual sl_bool checkPointInPath(const Ref<GraphicsPath>& path, const Point& pt) = 0;
	
	
	virtual Ref<Drawable> createDrawableFromImage(const ImageDesc& desc) = 0;
	
	virtual Ref<Drawable> loadDrawableFromMemory(const void* mem, sl_size size) = 0;

	
	virtual Ref<Bitmap> createBitmap(sl_uint32 width, sl_uint32 height) = 0;
	
	virtual Ref<Drawable> createDrawableFromBitmap(const Ref<Bitmap>& bitmap) = 0;
	
	virtual Ref<Bitmap> loadBitmapFromMemory(const void* mem, sl_size size) = 0;
	
public:
	Ref<Drawable> createDrawableFromImage(const Ref<Image>& image);
	
	Ref<Drawable> loadDrawableFromMemory(const Memory& mem);
	
	
	Ref<Bitmap> createBitmapFromImage(const ImageDesc& desc);
	
	Ref<Bitmap> createBitmapFromImage(const Ref<Image>& image);
	
	Ref<Bitmap> loadBitmapFromMemory(const Memory& mem);
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
