#ifndef CHECKHEADER_SLIB_GRAPHICS_DRAWABLE
#define CHECKHEADER_SLIB_GRAPHICS_DRAWABLE

#include "definition.h"

#include "color.h"
#include "pen.h"
#include "brush.h"

#include "../core/object.h"
#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT DrawParam
{
public:
	sl_bool useAlpha;
	sl_real alpha;
	
	sl_bool tiled;
	
	sl_bool useColorMatrix;
	ColorMatrix colorMatrix;
	
	sl_bool useBlur;
	sl_real blurRadius;
	
public:
	DrawParam();
	
	DrawParam(const DrawParam& other);
	
	DrawParam& operator=(const DrawParam& other);
	
public:
	sl_bool isTransparent() const;
	
	sl_bool isOpaque() const;
	
	sl_bool isBlur() const;
	
	Color transformColor(const Color& src) const;
	
};

struct ImageDesc;
class Image;
class Canvas;

class SLIB_EXPORT Drawable : public Object
{
	SLIB_DECLARE_OBJECT

public:
	virtual sl_real getDrawableWidth();
	
	virtual sl_real getDrawableHeight();
	
	virtual Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);
	
	virtual Ref<Drawable> scaleDrawable(sl_real width, sl_real height);
	
	virtual void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	virtual void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
public:
	sl_bool isBitmap();
	
	sl_bool isImage();
	
public:
	Ref<Drawable> filter(const ColorMatrix& colorMatrix, sl_real alpha = 1, sl_real blurRadius = 0);
	
	Ref<Drawable> filter(sl_real alpha, sl_real blurRadius = 0);
	
public:
	static Ref<Drawable> createColorDrawable(const Color& color);
	
	static Ref<Drawable> getEmptyDrawable();
	
	static Ref<Drawable> createSubDrawable(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height);
	
	static Ref<Drawable> createSubDrawable(const Ref<Drawable>& src, const Rectangle& rectSrc);
	
	static Ref<Drawable> createScaledDrawable(const Ref<Drawable>& src, sl_real width, sl_real height);
	
	static Ref<Drawable> createScaledDrawable(const Ref<Drawable>& src, const Size& size);
	
	static Ref<Drawable> filter(const Ref<Drawable>& src, const ColorMatrix& colorMatrix, sl_real alpha = 1, sl_real blurRadius = 0);
	
	static Ref<Drawable> filter(const Ref<Drawable>& src, sl_real alpha, sl_real blurRadius = 0);
	
};

class SLIB_EXPORT PlatformDrawable
{
public:
	static Ref<Drawable> create(const ImageDesc& desc);
	
	static Ref<Drawable> create(const Ref<Image>& image);
	
	static Ref<Drawable> loadFromMemory(const void* mem, sl_size size);
	
	static Ref<Drawable> loadFromMemory(const Memory& mem);
	
	static Ref<Drawable> loadFromFile(const String& filePath);
	
	static Ref<Drawable> loadFromAsset(const String& path);
	
};

class SLIB_EXPORT ColorDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(const Color& color);
	
	static sl_bool check(const Ref<Drawable>& drawable, Color* outColor = sl_null);
	
public:
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	Color m_color;
	
};

class SLIB_EXPORT EmptyDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT

public:
	static Ref<Drawable> get();
	
public:
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);

};


class SLIB_EXPORT SubDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height);
	
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
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
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
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real width, sl_real height);
	
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
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	Ref<Drawable> m_src;
	sl_real m_width;
	sl_real m_height;
	
};

class SLIB_EXPORT ScaledSubDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height);
	
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
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	Ref<Drawable> m_src;
	Rectangle m_rectSrc;
	sl_real m_width;
	sl_real m_height;
	
};

class SLIB_EXPORT FilterDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(const Ref<Drawable>& src, const ColorMatrix& colorMatrix, sl_real alpha = 1, sl_real blurRadius = 0);
	
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real alpha, sl_real blurRadius = 0);
	
public:
	// override
	sl_real getDrawableWidth();
	
	// override
	sl_real getDrawableHeight();
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	void _prepareParam(DrawParam& dst, const DrawParam& src);
	
protected:
	Ref<Drawable> m_src;
	sl_bool m_flagUseColorMatrix;
	ColorMatrix m_colorMatrix;
	sl_real m_alpha;
	sl_real m_blurRadius;
	
};

class SLIB_EXPORT NinePiecesDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(sl_real leftWidth, sl_real rightWidth, sl_real topHeight, sl_real bottomHeight,
								const Ref<Drawable>& topLeft, const Ref<Drawable>& top, const Ref<Drawable>& topRight,
								const Ref<Drawable>& left, const Ref<Drawable>& center, const Ref<Drawable>& right,
								const Ref<Drawable>& bottomLeft, const Ref<Drawable>& bottom, const Ref<Drawable>& bottomRight);
	
	static Ref<Drawable> create(sl_int32 leftWidth, sl_int32 rightWidth, sl_int32 topHeight, sl_int32 bottomHeight,
								const Ref<Drawable>& topLeft, const Ref<Drawable>& top, const Ref<Drawable>& topRight,
								const Ref<Drawable>& left, const Ref<Drawable>& center, const Ref<Drawable>& right,
								const Ref<Drawable>& bottomLeft, const Ref<Drawable>& bottom, const Ref<Drawable>& bottomRight);
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	sl_real m_widthLeft;
	sl_real m_widthRight;
	sl_real m_heightTop;
	sl_real m_heightBottom;
	Ref<Drawable> m_partTopLeft;
	Ref<Drawable> m_partTop;
	Ref<Drawable> m_partTopRight;
	Ref<Drawable> m_partLeft;
	Ref<Drawable> m_partCenter;
	Ref<Drawable> m_partRight;
	Ref<Drawable> m_partBottomLeft;
	Ref<Drawable> m_partBottom;
	Ref<Drawable> m_partBottomRight;
	
};

class SLIB_EXPORT NinePatchDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(sl_real leftWidthDst, sl_real rightWidthDst, sl_real topHeightDst, sl_real bottomHeightDst,
			const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
	static Ref<Drawable> create(sl_int32 leftWidthDst, sl_int32 rightWidthDst, sl_int32 topHeightDst, sl_int32 bottomHeightDst,
								const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	sl_real m_widthLeftDst;
	sl_real m_widthRightDst;
	sl_real m_heightTopDst;
	sl_real m_heightBottomDst;
	Ref<Drawable> m_src;
	sl_real m_widthLeftSrc;
	sl_real m_widthRightSrc;
	sl_real m_heightTopSrc;
	sl_real m_heightBottomSrc;
	
};

class SLIB_EXPORT HorizontalThreePatchDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(sl_real leftWidthDst, sl_real rightWidthDst,
								const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);
	
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);
	
	static Ref<Drawable> create(sl_int32 leftWidthDst, sl_int32 rightWidthDst,
								const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	sl_real m_widthLeftDst;
	sl_real m_widthRightDst;
	Ref<Drawable> m_src;
	sl_real m_widthLeftSrc;
	sl_real m_widthRightSrc;
	
};

class SLIB_EXPORT VerticalThreePatchDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<Drawable> create(sl_real topHeightDst, sl_real bottomHeightDst,
								const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
	static Ref<Drawable> create(const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
	static Ref<Drawable> create(sl_int32 topHeightDst, sl_int32 bottomHeightDst,
								const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	sl_real m_heightTopDst;
	sl_real m_heightBottomDst;
	Ref<Drawable> m_src;
	sl_real m_heightTopSrc;
	sl_real m_heightBottomSrc;
	
};

class MipmapDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
public:
	MipmapDrawable();
	
public:
	// override
	sl_real getDrawableWidth();
	
	void setDrawableWidth(sl_real width);
	
	// override
	sl_real getDrawableHeight();
	
	void setDrawableHeight(sl_real height);

	void addSource(const Ref<Drawable>& source, sl_real width, sl_real height);
	
	void addSource(const Ref<Drawable>& source);
	
	Ref<Drawable> getSource(sl_size index);
	
	sl_size getSourcesCount();
	
	Ref<Drawable> getMatchingSource(sl_real requiredWidth, sl_real requiredHeight);
	
public:
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
	
protected:
	struct _Source
	{
		sl_real width;
		sl_real height;
		Ref<Drawable> drawable;
	};
	CList<_Source> m_sources;
	
	sl_real m_width;
	sl_real m_height;
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
