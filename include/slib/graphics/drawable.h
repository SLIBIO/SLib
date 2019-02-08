/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_DRAWABLE
#define CHECKHEADER_SLIB_GRAPHICS_DRAWABLE

#include "definition.h"

#include "color.h"
#include "pen.h"
#include "brush.h"

#include "../core/object.h"
#include "../math/rectangle.h"

namespace slib
{
	
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
		
		float time; // In seconds, used for animations

	public:
		DrawParam();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DrawParam)

	public:
		sl_bool isTransparent() const;

		sl_bool isOpaque() const;

		sl_bool isBlur() const;

		Color transformColor(const Color& src) const;

	};
	
	class DrawableAnimationInfo
	{
	public:
		float duration; // In seconds
		float framesPerSecond;
		
	public:
		DrawableAnimationInfo();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(DrawableAnimationInfo)
		
	};
	
	class ImageDesc;
	class Image;
	class Canvas;
	
	class SLIB_EXPORT Drawable : public Object
	{
		SLIB_DECLARE_OBJECT
	
	protected:
		Drawable();

		~Drawable();

	public:
		virtual sl_real getDrawableWidth();

		virtual sl_real getDrawableHeight();

		virtual Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height);

		virtual Ref<Drawable> scaleDrawable(sl_real width, sl_real height);

		virtual void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param);

		virtual void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param);
		
		// return sl_true for Animation
		virtual sl_bool getAnimationInfo(DrawableAnimationInfo* info);

	public:
		sl_bool isBitmap();

		sl_bool isImage();
		
		sl_bool isColor();
		
		sl_bool getColor(Color* color = sl_null);
		
		// Seconds, 0 means no animation
		float getAnimationDuration();
		
		float getAnimationFramesPerSecond();

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

	protected:
		ColorDrawable();

		~ColorDrawable();

	public:
		static Ref<Drawable> create(const Color& color);

		static sl_bool check(const Ref<Drawable>& drawable, Color* outColor = sl_null);

	public:
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

	protected:
		Color m_color;

	};
	
	class SLIB_EXPORT EmptyDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT
	
	protected:
		EmptyDrawable();

		~EmptyDrawable();

	public:
		static Ref<Drawable> get();

	public:
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;
	
	};
	
	
	class SLIB_EXPORT SubDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT

	protected:
		SubDrawable();

		~SubDrawable();

	public:
		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height);

	public:
		sl_real getDrawableWidth() override;

		sl_real getDrawableHeight() override;

		Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height) override;
	
		Ref<Drawable> scaleDrawable(sl_real width, sl_real height) override;

		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;
		
		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;
		
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

	protected:
		ScaledDrawable();

		~ScaledDrawable();

	public:
		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real width, sl_real height);

	public:
		sl_real getDrawableWidth() override;

		sl_real getDrawableHeight() override;

		Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height) override;

		Ref<Drawable> scaleDrawable(sl_real width, sl_real height) override;

		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;
		
		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;

	protected:
		Ref<Drawable> m_src;
		sl_real m_width;
		sl_real m_height;

	};
	
	class SLIB_EXPORT ScaledSubDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT

	protected:
		ScaledSubDrawable();

		~ScaledSubDrawable();

	public:
		static Ref<Drawable> create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height);

	public:
		sl_real getDrawableWidth() override;

		sl_real getDrawableHeight() override;

		Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height) override;

		Ref<Drawable> scaleDrawable(sl_real width, sl_real height) override;

		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;
		
	protected:
		Ref<Drawable> m_src;
		Rectangle m_rectSrc;
		sl_real m_width;
		sl_real m_height;

	};
	
	class SLIB_EXPORT FilterDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT

	protected:
		FilterDrawable();

		~FilterDrawable();

	public:
		static Ref<Drawable> create(const Ref<Drawable>& src, const ColorMatrix& colorMatrix, sl_real alpha = 1, sl_real blurRadius = 0);

		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real alpha, sl_real blurRadius = 0);

	public:
		sl_real getDrawableWidth() override;

		sl_real getDrawableHeight() override;

		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;
		
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

	protected:
		NinePiecesDrawable();

		~NinePiecesDrawable();

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
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

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

	protected:
		NinePatchDrawable();

		~NinePatchDrawable();

	public:
		static Ref<Drawable> create(sl_real leftWidthDst, sl_real rightWidthDst, sl_real topHeightDst, sl_real bottomHeightDst,
				const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);

		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);

		static Ref<Drawable> create(sl_int32 leftWidthDst, sl_int32 rightWidthDst, sl_int32 topHeightDst, sl_int32 bottomHeightDst,
									const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc);

	public:
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

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

	protected:
		HorizontalThreePatchDrawable();

		~HorizontalThreePatchDrawable();

	public:
		static Ref<Drawable> create(sl_real leftWidthDst, sl_real rightWidthDst,
									const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);

		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);

		static Ref<Drawable> create(sl_int32 leftWidthDst, sl_int32 rightWidthDst,
									const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc);

	public:
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

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

	protected:
		VerticalThreePatchDrawable();

		~VerticalThreePatchDrawable();

	public:
		static Ref<Drawable> create(sl_real topHeightDst, sl_real bottomHeightDst,
									const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);

		static Ref<Drawable> create(const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);

		static Ref<Drawable> create(sl_int32 topHeightDst, sl_int32 bottomHeightDst,
									const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc);

	public:
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

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

		~MipmapDrawable();

	public:
		sl_real getDrawableWidth() override;

		void setDrawableWidth(sl_real width);

		sl_real getDrawableHeight() override;

		void setDrawableHeight(sl_real height);
	
		void addSource(const Ref<Drawable>& source, sl_real width, sl_real height);

		void addSource(const Ref<Drawable>& source);

		Ref<Drawable> getSource(sl_size index);

		sl_size getSourcesCount();

		Ref<Drawable> getMatchingSource(sl_real requiredWidth, sl_real requiredHeight);

	public:
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;
		
	protected:
		struct Source
		{
			sl_real width;
			sl_real height;
			Ref<Drawable> drawable;
		};
		CList<Source> m_sources;

		sl_real m_width;
		sl_real m_height;

	};
	
	class AnimationDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AnimationDrawable();
		
		~AnimationDrawable();
		
	public:
		sl_real getDrawableWidth() override;
		
		void setDrawableWidth(sl_real width);
		
		sl_real getDrawableHeight() override;
		
		void setDrawableHeight(sl_real height);

		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;
		
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;
		
		void setAnimationDuration(float duration);
				
		List< Ref<Drawable> > getDrawables();
		
		void addDrawable(const Ref<Drawable>& drawable);
		
		// `time` in seconds
		Ref<Drawable> getDrawableAtTime(float time);

	protected:
		sl_real m_width;
		sl_real m_height;
		List< Ref<Drawable> > m_drawables;
		float m_duration;
		
	};

}

#endif
