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

#ifndef CHECKHEADER_SLIB_GRAPHICS_BRUSH
#define CHECKHEADER_SLIB_GRAPHICS_BRUSH

#include "definition.h"

#include "constants.h"
#include "color.h"

#include "../core/object.h"
#include "../math/point.h"

namespace slib
{
	
	class SLIB_EXPORT GradientBrushDetail : public Referable
	{
	public:
		Point point1;
		Point point2;
		sl_real radius;
		List<Color> colors;
		List<sl_real> locations;
		
	public:
		GradientBrushDetail();
		
		~GradientBrushDetail();

	};
	
	class Bitmap;
	
	class SLIB_EXPORT TextureBrushDetail : public Referable
	{
	public:
		Ref<Bitmap> pattern;
		
	public:
		TextureBrushDetail();
		
		~TextureBrushDetail();
		
	};
	
	class SLIB_EXPORT BrushDesc
	{
	public:
		BrushStyle style;
		Color color;
		Ref<Referable> detail;

	public:
		BrushDesc();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(BrushDesc)

	};
	
	class SLIB_EXPORT Brush : public Referable
	{
		SLIB_DECLARE_OBJECT

	protected:
		Brush();

		~Brush();

	public:
		static Ref<Brush> getDefault();

		static Ref<Brush> create(const BrushDesc& desc);

		static Ref<Brush> createSolidBrush(const Color& color);

		static Ref<Brush> createLinearGradientBrush(const Point& pt1, const Point& pt2, const Color& color1, const Color& color2);

		static Ref<Brush> createLinearGradientBrush(const Point& pt1, const Point& pt2, sl_uint32 nColors, const Color* colors, const sl_real* locations);

		static Ref<Brush> createRadialGradientBrush(const Point& ptCenter, sl_real radius, const Color& colorCenter, const Color& colorEdge);
		
		static Ref<Brush> createRadialGradientBrush(const Point& ptCenter, sl_real radius, sl_uint32 nColors, const Color* colors, const sl_real* locations);
		
		static Ref<Brush> createTextureBrush(const Ref<Bitmap>& bitmap);

	public:
		void getDesc(BrushDesc& desc);
		
		BrushDesc& getDesc();

		BrushStyle getStyle();

		Color getColor();

	protected:
		BrushDesc m_desc;

		Ref<Referable> m_platformObject;
		SpinLock m_lock;

	};

}

#endif
