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

#include "slib/graphics/drawable.h"

#include "slib/graphics/canvas.h"
#include "slib/graphics/bitmap.h"
#include "slib/graphics/image.h"
#include "slib/graphics/util.h"

#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DrawParam)
	
	DrawParam::DrawParam()
	 : useAlpha(sl_false), alpha(1), useColorMatrix(sl_false), useBlur(sl_false), blurRadius(10), time(0)
	{
	}

	sl_bool DrawParam::isTransparent() const
	{
		if (isOpaque()) {
			return sl_false;
		}
		return alpha < 0.005;
	}

	sl_bool DrawParam::isOpaque() const
	{
		if (useAlpha) {
			if (alpha > 0.995) {
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return sl_true;
		}
	}

	sl_bool DrawParam::isBlur() const
	{
		if (useBlur) {
			if (blurRadius > 0.1f) {
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			return sl_false;
		}
	}

	Color DrawParam::transformColor(const Color& src) const
	{
		if (isOpaque()) {
			if (useColorMatrix) {
				return colorMatrix.transformColor(src);
			} else {
				return src;
			}
		} else {
			if (useColorMatrix) {
				Color4f ret = colorMatrix.transformColor(src);
				ret.w *= alpha;
				return ret;
			} else {
				Color ret;
				ret.r = src.r;
				ret.g = src.g;
				ret.b = src.b;
				ret.a = Math::clamp0_255((sl_int32)(alpha * (sl_real)(src.a)));
				return ret;
			}
		}
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DrawableAnimationInfo)
	
	DrawableAnimationInfo::DrawableAnimationInfo()
	 : duration(0), framesPerSecond(0)
	{
	}
	

	SLIB_DEFINE_OBJECT(Drawable, Object)

	Drawable::Drawable()
	{
	}

	Drawable::~Drawable()
	{
	}

	sl_real Drawable::getDrawableWidth()
	{
		return 1;
	}

	sl_real Drawable::getDrawableHeight()
	{
		return 1;
	}

	Ref<Drawable> Drawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		return SubDrawable::create(this, x, y, width, height);
	}

	Ref<Drawable> Drawable::scaleDrawable(sl_real width, sl_real height)
	{
		return ScaledDrawable::create(this, width, height);
	}

	void Drawable::onDraw(Canvas* canvas, const Rectangle& _rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Rectangle rectDst = GraphicsUtil::transformRectangle(_rectDst, rectSrc, Rectangle(0, 0, getDrawableWidth(), getDrawableHeight()));
		onDrawAll(canvas, rectDst, param);
	}

	void Drawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		onDraw(canvas, rectDst, Rectangle(0, 0, getDrawableWidth(), getDrawableHeight()), param);
	}
	
	sl_bool Drawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return sl_false;
	}

	sl_bool Drawable::isBitmap()
	{
		return IsInstanceOf<Bitmap>(this);
	}
	
	sl_bool Drawable::isImage()
	{
		return IsInstanceOf<Image>(this);
	}
	
	sl_bool Drawable::isColor()
	{
		return ColorDrawable::check(this, sl_null);
	}

	sl_bool Drawable::getColor(Color* color)
	{
		return ColorDrawable::check(this, color);
	}

	float Drawable::getAnimationDuration()
	{
		DrawableAnimationInfo info;
		if (getAnimationInfo(&info)) {
			return info.duration;
		}
		return 0;
	}
	
	float Drawable::getAnimationFramesPerSecond()
	{
		DrawableAnimationInfo info;
		if (getAnimationInfo(&info)) {
			return info.framesPerSecond;
		}
		return 0;
	}

	Ref<Drawable> Drawable::filter(const ColorMatrix& colorMatrix, sl_real alpha, sl_real blurRadius)
	{
		return FilterDrawable::create(this, colorMatrix, alpha, blurRadius);
	}

	Ref<Drawable> Drawable::filter(sl_real alpha, sl_real blurRadius)
	{
		return FilterDrawable::create(this, alpha, blurRadius);
	}

	Ref<Drawable> Drawable::rotate(RotationMode rotate, FlipMode flip)
	{
		return RotateFlipDrawable::apply(this, rotate, flip);
	}
	
	Ref<Drawable> Drawable::flip(FlipMode flip)
	{
		return RotateFlipDrawable::apply(this, RotationMode::Rotate0, flip);
	}

	Ref<Drawable> Drawable::clipEllipse()
	{
		return ClipEllipseDrawable::create(this);
	}

	Ref<Drawable> Drawable::clipRoundRect(const Size& radius)
	{
		return ClipRoundRectDrawable::create(this, radius);
	}

	Ref<Drawable> Drawable::clipSquare(const Alignment& align)
	{
		return clipSquare(this, align);
	}

	Ref<Drawable> Drawable::clipSquare()
	{
		return clipSquare(this, Alignment::MiddleCenter);
	}

	Ref<Drawable> Drawable::createColorDrawable(const Color& color)
	{
		return ColorDrawable::create(color);
	}

	Ref<Drawable> Drawable::getEmptyDrawable()
	{
		return EmptyDrawable::get();
	}

	Ref<Drawable> Drawable::createSubDrawable(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height)
	{
		if (src.isNotNull()) {
			return src->subDrawable(x, y, width, height);
		}
		return sl_null;
	}

	Ref<Drawable> Drawable::createSubDrawable(const Ref<Drawable>& src, const Rectangle& rectSrc)
	{
		if (src.isNotNull()) {
			return src->subDrawable(rectSrc.left, rectSrc.top, rectSrc.getWidth(), rectSrc.getHeight());
		}
		return sl_null;
	}

	Ref<Drawable> Drawable::createScaledDrawable(const Ref<Drawable>& src, sl_real width, sl_real height)
	{
		if (src.isNotNull()) {
			return src->scaleDrawable(width, height);
		}
		return sl_null;
	}

	Ref<Drawable> Drawable::createScaledDrawable(const Ref<Drawable>& src, const Size& size)
	{
		if (src.isNotNull()) {
			return src->scaleDrawable(size.x, size.y);
		}
		return sl_null;
	}

	Ref<Drawable> Drawable::filter(const Ref<Drawable>& src, const ColorMatrix& colorMatrix, sl_real alpha, sl_real blurRadius)
	{
		return FilterDrawable::create(src, colorMatrix, alpha, blurRadius);
	}

	Ref<Drawable> Drawable::filter(const Ref<Drawable>& src, sl_real alpha, sl_real blurRadius)
	{
		return FilterDrawable::create(src, alpha, blurRadius);
	}

	Ref<Drawable> Drawable::rotate(const Ref<Drawable>& src, RotationMode rotate, FlipMode flip)
	{
		return RotateFlipDrawable::apply(src, rotate, flip);
	}
	
	Ref<Drawable> Drawable::flip(const Ref<Drawable>& src, FlipMode flip)
	{
		return RotateFlipDrawable::apply(src, RotationMode::Rotate0, flip);
	}

	Ref<Drawable> Drawable::clipEllipse(const Ref<Drawable>& src)
	{
		return ClipEllipseDrawable::create(src);
	}

	Ref<Drawable> Drawable::clipRoundRect(const Ref<Drawable>& src, const Size& radius)
	{
		return ClipRoundRectDrawable::create(src, radius);
	}

	Ref<Drawable> Drawable::clipSquare(const Ref<Drawable>& src, const Alignment& align)
	{
		if (src.isNull()) {
			return sl_null;
		}
		sl_real width = src->getDrawableWidth();
		sl_real height = src->getDrawableHeight();
		if (width > height) {
			sl_real x = 0;
			Alignment halign = align & Alignment::HorizontalMask;
			if (halign == Alignment::Center) {
				x = (width - height) / 2;
			} else if (halign == Alignment::Right) {
				x = width - height;
			}
			return src->subDrawable(x, 0, height, height);
		} else if (width < height) {
			sl_real y = 0;
			Alignment valign = align & Alignment::VerticalMask;
			if (valign == Alignment::Middle) {
				y = (height - width) / 2;
			} else if (valign == Alignment::Bottom) {
				y = height - width;
			}
			return src->subDrawable(0, y, width, width);
		} else {
			return src;
		}
	}
	
	Ref<Drawable> Drawable::clipSquare(const Ref<Drawable>& src)
	{
		return clipSquare(src, Alignment::MiddleCenter);
	}
	
	
	SLIB_DEFINE_OBJECT(ColorDrawable, Drawable)

	ColorDrawable::ColorDrawable()
	{
	}

	ColorDrawable::~ColorDrawable()
	{
	}

	Ref<Drawable> ColorDrawable::create(const Color& color)
	{
		if (color.isZero()) {
			return EmptyDrawable::get();
		}
		Ref<ColorDrawable> ret;
		Ref<Brush> brush = Brush::createSolidBrush(color);
		if (brush.isNull()) {
			return ret;
		}
		ret = new ColorDrawable();
		if (ret.isNotNull()) {
			ret->m_color = color;
		}
		return ret;
	}

	sl_bool ColorDrawable::check(const Ref<Drawable>& drawable, Color* outColor)
	{
		if (ColorDrawable* c = CastInstance<ColorDrawable>(drawable.get())) {
			if (outColor) {
				*outColor = c->m_color;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void ColorDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Color color = param.transformColor(m_color);
		if (color.a > 0) {
			canvas->fillRectangle(rectDst, color);
		}
	}

	void ColorDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Color color = param.transformColor(m_color);
		if (color.a > 0) {
			canvas->fillRectangle(rectDst, color);
		}
	}

	SLIB_DEFINE_OBJECT(EmptyDrawable, Drawable)

	EmptyDrawable::EmptyDrawable()
	{
	}

	EmptyDrawable::~EmptyDrawable()
	{
	}

	Ref<Drawable> EmptyDrawable::get()
	{
		SLIB_SAFE_STATIC(Ref<Drawable>, ret, new EmptyDrawable)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return new EmptyDrawable;
		}
		return ret;
	}

	void EmptyDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
	}

	void EmptyDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
	}


	SLIB_DEFINE_OBJECT(SubDrawable, Drawable)

	SubDrawable::SubDrawable()
	{
		m_x = 0;
		m_y = 0;
		m_width = 0;
		m_height = 0;
	}

	SubDrawable::~SubDrawable()
	{
	}

	Ref<Drawable> SubDrawable::create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height)
	{
		if (src.isNotNull() && width > 0 && height > 0) {
			Ref<SubDrawable> sub = new SubDrawable;
			if (sub.isNotNull()) {
				sub->m_src = src;
				sub->m_x = x;
				sub->m_y = y;
				sub->m_width = width;
				sub->m_height = height;
				return sub;
			}
		}
		return sl_null;
	}

	sl_real SubDrawable::getDrawableWidth()
	{
		return m_width;
	}

	sl_real SubDrawable::getDrawableHeight()
	{
		return m_height;
	}

	Ref<Drawable> SubDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		return SubDrawable::create(m_src, m_x + x, m_y + y, width, height);
	}

	Ref<Drawable> SubDrawable::scaleDrawable(sl_real width, sl_real height)
	{
		return ScaledSubDrawable::create(m_src, Rectangle(m_x, m_y, m_x + m_width, m_y + m_height), width, height);
	}

	void SubDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc, const DrawParam& param)
	{
		Rectangle rectSrc;
		rectSrc.left = m_x + _rectSrc.left;
		rectSrc.top = m_y + _rectSrc.top;
		rectSrc.right = m_x + _rectSrc.right;
		rectSrc.bottom = m_y + _rectSrc.bottom;
		canvas->draw(rectDst, m_src, rectSrc, param);
	}

	void SubDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		canvas->draw(rectDst, m_src, Rectangle(m_x, m_y, m_x + m_width, m_y + m_height), param);
	}

	sl_bool SubDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	
	SLIB_DEFINE_OBJECT(ScaledDrawable, Drawable)

	ScaledDrawable::ScaledDrawable()
	{
		m_width = 0;
		m_height = 0;
	}

	ScaledDrawable::~ScaledDrawable()
	{
	}

	Ref<Drawable> ScaledDrawable::create(const Ref<Drawable>& src, sl_real width, sl_real height)
	{
		if (src.isNotNull() && width > 0 && height > 0) {
			Ref<ScaledDrawable> ret = new ScaledDrawable;
			if (ret.isNotNull()) {
				ret->m_src = src;
				ret->m_width = width;
				ret->m_height = height;
				return ret;
			}
		}
		return sl_null;
	}

	sl_real ScaledDrawable::getDrawableWidth()
	{
		return m_width;
	}

	sl_real ScaledDrawable::getDrawableHeight()
	{
		return m_height;
	}

	Ref<Drawable> ScaledDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		sl_real rsx = m_src->getDrawableWidth() / m_width;
		sl_real rsy = m_src->getDrawableHeight() / m_height;
		return ScaledSubDrawable::create(m_src, Rectangle(rsx * x, rsy * y, rsx * (x + width), rsy * (y + height)), width, height);
	}

	Ref<Drawable> ScaledDrawable::scaleDrawable(sl_real width, sl_real height)
	{
		return ScaledDrawable::create(m_src, width, height);
	}

	void ScaledDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc, const DrawParam& param)
	{
		Rectangle rectSrc;
		sl_real rsx = m_src->getDrawableWidth() / m_width;
		sl_real rsy = m_src->getDrawableHeight() / m_height;
		rectSrc.left = rsx * _rectSrc.left;
		rectSrc.top = rsy * _rectSrc.top;
		rectSrc.right = rsx * _rectSrc.right;
		rectSrc.bottom = rsy * _rectSrc.bottom;
		canvas->draw(rectDst, m_src, rectSrc, param);
	}

	void ScaledDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		canvas->draw(rectDst, m_src, param);
	}

	sl_bool ScaledDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	

	SLIB_DEFINE_OBJECT(ScaledSubDrawable, Drawable)

	ScaledSubDrawable::ScaledSubDrawable()
	{
		m_width = 0;
		m_height = 0;
	}

	ScaledSubDrawable::~ScaledSubDrawable()
	{
	}

	Ref<Drawable> ScaledSubDrawable::create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height)
	{
		if (src.isNotNull() && width > 0 && height > 0 && rectSrc.getWidth() > 0 && rectSrc.getHeight() > 0) {
			Ref<ScaledSubDrawable> ret = new ScaledSubDrawable;
			if (ret.isNotNull()) {
				ret->m_src = src;
				ret->m_rectSrc = rectSrc;
				ret->m_width = width;
				ret->m_height = height;
				return ret;
			}
		}
		return sl_null;
	}

	sl_real ScaledSubDrawable::getDrawableWidth()
	{
		return m_width;
	}

	sl_real ScaledSubDrawable::getDrawableHeight()
	{
		return m_height;
	}

	Ref<Drawable> ScaledSubDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
	{
		sl_real rsx = m_rectSrc.getWidth() / m_width;
		sl_real rsy = m_rectSrc.getHeight() / m_height;
		Rectangle rectSrc;
		rectSrc.left = rsx * x + m_rectSrc.left;
		rectSrc.top = rsy * y + m_rectSrc.top;
		rectSrc.right = rectSrc.left + rsx * width;
		rectSrc.bottom = rectSrc.top + rsy * height;
		return ScaledSubDrawable::create(m_src, rectSrc, width, height);
	}

	Ref<Drawable> ScaledSubDrawable::scaleDrawable(sl_real width, sl_real height)
	{
		return ScaledSubDrawable::create(m_src, m_rectSrc, width, height);
	}

	void ScaledSubDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc, const DrawParam& param)
	{
		sl_real rsx = m_rectSrc.getWidth() / m_width;
		sl_real rsy = m_rectSrc.getHeight() / m_height;
		Rectangle rectSrc;
		rectSrc.left = rsx * _rectSrc.left + m_rectSrc.left;
		rectSrc.top = rsy * _rectSrc.top + m_rectSrc.top;
		rectSrc.right = rsx * _rectSrc.right + m_rectSrc.left;
		rectSrc.bottom = rsy * _rectSrc.bottom + m_rectSrc.top;
		canvas->draw(rectDst, m_src, rectSrc, param);
	}

	void ScaledSubDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		canvas->draw(rectDst, m_src, m_rectSrc, param);
	}

	sl_bool ScaledSubDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	
	
	SLIB_DEFINE_OBJECT(RotateFlipDrawable, Drawable)
	
	RotateFlipDrawable::RotateFlipDrawable()
	{
		m_rotate = RotationMode::Rotate0;
		m_flip = FlipMode::None;
	}
	
	RotateFlipDrawable::~RotateFlipDrawable()
	{
	}
	
	Ref<Drawable> RotateFlipDrawable::apply(const Ref<Drawable>& src, RotationMode rotate, FlipMode flip)
	{
		if (src.isNull()) {
			return sl_null;
		}
		NormalizeRotateAndFlip(rotate, flip);
		if (rotate == RotationMode::Rotate0 && flip == FlipMode::None) {
			return src;
		}
		if (RotateFlipDrawable* rf = CastInstance<RotateFlipDrawable>(src.get())) {
			RotationMode rotateNew = rf->m_rotate;
			FlipMode flipNew = rf->m_flip;
			if (flipNew == FlipMode::None) {
				rotateNew = rotateNew + rotate;
			} else {
				rotateNew = rotateNew - rotate;
			}
			flipNew = flipNew * flip;
			NormalizeRotateAndFlip(rotateNew, flipNew);
			if (rotateNew == RotationMode::Rotate0 && flipNew == FlipMode::None) {
				return rf->m_src;
			}
			Ref<RotateFlipDrawable> ret = new RotateFlipDrawable;
			if (ret.isNotNull()) {
				ret->m_src = rf->m_src;
				ret->m_rotate = rotateNew;
				ret->m_flip = flipNew;
				return ret;
			}
		} else {
			Ref<RotateFlipDrawable> ret = new RotateFlipDrawable;
			if (ret.isNotNull()) {
				ret->m_src = src;
				ret->m_rotate = rotate;
				ret->m_flip = flip;
				return ret;
			}
		}
		return sl_null;
	}

	const Ref<Drawable>& RotateFlipDrawable::getSource()
	{
		return m_src;
	}

	RotationMode RotateFlipDrawable::getRotation()
	{
		return m_rotate;
	}
	
	FlipMode RotateFlipDrawable::getFlip()
	{
		return m_flip;
	}
	
	sl_real RotateFlipDrawable::getDrawableWidth()
	{
		if (m_rotate != RotationMode::Rotate90 && m_rotate != RotationMode::Rotate270) {
			return m_src->getDrawableWidth();
		} else {
			return m_src->getDrawableHeight();
		}
	}
	
	sl_real RotateFlipDrawable::getDrawableHeight()
	{
		if (m_rotate != RotationMode::Rotate90 && m_rotate != RotationMode::Rotate270) {
			return m_src->getDrawableHeight();
		} else {
			return m_src->getDrawableWidth();
		}
	}
	
	void RotateFlipDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		CanvasStateScope scope(canvas);
		Point center = rectDst.getCenter();
		canvas->translate(center.x, center.y);
		if (m_flip == FlipMode::Horizontal) {
			canvas->scale(-1, 1);
		} else if (m_flip == FlipMode::Vertical) {
			canvas->scale(1, -1);
		}
		canvas->rotate((sl_real)((int)m_rotate) * SLIB_PI_DUAL / 360.0f);
		sl_real width_half = rectDst.getWidth() / 2;
		sl_real height_half = rectDst.getHeight() / 2;
		if (m_rotate == RotationMode::Rotate90 || m_rotate == RotationMode::Rotate270) {
			Swap(width_half, height_half);
		}
		canvas->draw(Rectangle(-width_half, -height_half, width_half, height_half), m_src, param);
	}
	
	sl_bool RotateFlipDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	
	
	SLIB_DEFINE_OBJECT(ClipEllipseDrawable, Drawable)
	
	ClipEllipseDrawable::ClipEllipseDrawable()
	{
	}
	
	ClipEllipseDrawable::~ClipEllipseDrawable()
	{
	}
	
	Ref<Drawable> ClipEllipseDrawable::create(const Ref<Drawable>& src)
	{
		if (src.isNull()) {
			return sl_null;
		}
		Ref<ClipEllipseDrawable> ret = new ClipEllipseDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			return ret;
		}
		return sl_null;
	}
	
	sl_real ClipEllipseDrawable::getDrawableWidth()
	{
		return m_src->getDrawableWidth();
	}
	
	sl_real ClipEllipseDrawable::getDrawableHeight()
	{
		return m_src->getDrawableHeight();
	}
	
	void ClipEllipseDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		CanvasStateScope scope(canvas);
		canvas->clipToEllipse(rectDst);
		canvas->draw(rectDst, m_src, param);
	}
	
	sl_bool ClipEllipseDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	
	
	SLIB_DEFINE_OBJECT(ClipRoundRectDrawable, Drawable)
	
	ClipRoundRectDrawable::ClipRoundRectDrawable()
	{
	}
	
	ClipRoundRectDrawable::~ClipRoundRectDrawable()
	{
	}
	
	Ref<Drawable> ClipRoundRectDrawable::create(const Ref<Drawable>& src, const Size& radius)
	{
		if (src.isNull()) {
			return sl_null;
		}
		Ref<ClipRoundRectDrawable> ret = new ClipRoundRectDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			ret->m_radius = radius;
			return ret;
		}
		return sl_null;
	}
	
	sl_real ClipRoundRectDrawable::getDrawableWidth()
	{
		return m_src->getDrawableWidth();
	}
	
	sl_real ClipRoundRectDrawable::getDrawableHeight()
	{
		return m_src->getDrawableHeight();
	}
	
	void ClipRoundRectDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		CanvasStateScope scope(canvas);
		canvas->clipToRoundRect(rectDst, m_radius);
		canvas->draw(rectDst, m_src, param);
	}
	
	sl_bool ClipRoundRectDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}


	SLIB_DEFINE_OBJECT(FilterDrawable, Drawable)

	FilterDrawable::FilterDrawable()
	{
		m_flagUseColorMatrix = sl_false;
		m_alpha = 1;
		m_blurRadius = 1;
	}

	FilterDrawable::~FilterDrawable()
	{
	}

	Ref<Drawable> FilterDrawable::create(const Ref<Drawable>& src, const ColorMatrix& colorMatrix, sl_real alpha, sl_real blurRadius)
	{
		if (src.isNull()) {
			return sl_null;
		}
		Ref<FilterDrawable> ret = new FilterDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			ret->m_flagUseColorMatrix = sl_true;
			ret->m_colorMatrix = colorMatrix;
			ret->m_alpha = alpha;
			ret->m_blurRadius = blurRadius;
			return ret;
		}
		return sl_null;
	}

	Ref<Drawable> FilterDrawable::create(const Ref<Drawable>& src, sl_real alpha, sl_real blurRadius)
	{
		if (src.isNull()) {
			return sl_null;
		}
		Ref<FilterDrawable> ret = new FilterDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			ret->m_flagUseColorMatrix = sl_false;
			ret->m_alpha = alpha;
			ret->m_blurRadius = blurRadius;
			return ret;
		}
		return sl_null;
	}

	sl_real FilterDrawable::getDrawableWidth()
	{
		return m_src->getDrawableWidth();
	}

	sl_real FilterDrawable::getDrawableHeight()
	{
		return m_src->getDrawableHeight();
	}

	void FilterDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		DrawParam paramNew(param);
		_prepareParam(paramNew, param);
		canvas->draw(rectDst, m_src, rectSrc, paramNew);
	}

	void FilterDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		DrawParam paramNew(param);
		_prepareParam(paramNew, param);
		canvas->draw(rectDst, m_src, paramNew);
	}

	void FilterDrawable::_prepareParam(DrawParam& dst, const DrawParam& src)
	{
		if (m_flagUseColorMatrix) {
			dst.useColorMatrix = sl_true;
			dst.colorMatrix = m_colorMatrix;
		}
		if (m_alpha < 0.995f) {
			dst.useAlpha = sl_true;
			if (src.useAlpha) {
				dst.alpha = src.alpha * m_alpha;
			} else {
				dst.alpha = m_alpha;
			}
		}
		if (m_blurRadius > 0.495f) {
			dst.useBlur = sl_true;
			if (src.useBlur) {
				dst.blurRadius = src.blurRadius + m_blurRadius;
			} else {
				dst.blurRadius = m_blurRadius;
			}
		}
	}

	sl_bool FilterDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		return m_src->getAnimationInfo(info);
	}
	

	SLIB_DEFINE_OBJECT(NinePiecesDrawable, Drawable)

	NinePiecesDrawable::NinePiecesDrawable()
	{
		m_widthLeft = 0;
		m_widthRight = 0;
		m_heightTop = 0;
		m_heightBottom = 0;
	}

	NinePiecesDrawable::~NinePiecesDrawable()
	{
	}

	Ref<Drawable> NinePiecesDrawable::create(sl_real leftWidth, sl_real rightWidth, sl_real topHeight, sl_real bottomHeight,
										const Ref<Drawable>& topLeft, const Ref<Drawable>& top, const Ref<Drawable>& topRight,
										const Ref<Drawable>& left, const Ref<Drawable>& center, const Ref<Drawable>& right,
										const Ref<Drawable>& bottomLeft, const Ref<Drawable>& bottom, const Ref<Drawable>& bottomRight)
	{
		Ref<NinePiecesDrawable> ret = new NinePiecesDrawable;
		if (ret.isNotNull()) {
			if (leftWidth < 0) {
				leftWidth = 0;
			}
			if (rightWidth < 0) {
				rightWidth = 0;
			}
			if (topHeight < 0) {
				topHeight = 0;
			}
			if (bottomHeight < 0) {
				bottomHeight = 0;
			}
			ret->m_widthLeft = leftWidth;
			ret->m_widthRight = rightWidth;
			ret->m_heightTop = topHeight;
			ret->m_heightBottom = bottomHeight;
			ret->m_partTopLeft = topLeft;
			ret->m_partTop = top;
			ret->m_partTopRight = topRight;
			ret->m_partLeft = left;
			ret->m_partCenter = center;
			ret->m_partRight = right;
			ret->m_partBottomLeft = bottomLeft;
			ret->m_partBottom = bottom;
			ret->m_partTopRight = bottomRight;
			return ret;
		}
		return sl_null;
	}

	Ref<Drawable> NinePiecesDrawable::create(sl_int32 leftWidth, sl_int32 rightWidth, sl_int32 topHeight, sl_int32 bottomHeight,
											const Ref<Drawable>& topLeft, const Ref<Drawable>& top, const Ref<Drawable>& topRight,
											const Ref<Drawable>& left, const Ref<Drawable>& center, const Ref<Drawable>& right,
											const Ref<Drawable>& bottomLeft, const Ref<Drawable>& bottom, const Ref<Drawable>& bottomRight)
	{
		return create((sl_real)leftWidth, (sl_real)(rightWidth), (sl_real)topHeight, (sl_real)bottomHeight, topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight);
	}

	void NinePiecesDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		sl_real x = rectDst.left;
		sl_real y = rectDst.top;
		sl_real widthLeft = m_widthLeft;
		sl_real widthRight = m_widthRight;
		sl_real heightTop = m_heightTop;
		sl_real heightBottom = m_heightBottom;
		sl_real width = rectDst.getWidth();
		sl_real height = rectDst.getHeight();
		if (width < widthLeft + widthRight || height < heightTop + heightBottom) {
			canvas->draw(rectDst, m_partCenter, param);
			return;
		}
		if (heightTop > 0) {
			if (widthLeft > 0 && m_partTopLeft.isNotNull()) {
				canvas->draw(Rectangle(x, y, x + widthLeft, y + heightTop), m_partTopLeft, param);
			}
			if (widthRight > 0 && m_partTopRight.isNotNull()) {
				canvas->draw(Rectangle(x + width - widthRight, y, x + width, y + heightTop), m_partTopRight, param);
			}
			if (m_partTop.isNotNull()) {
				canvas->draw(Rectangle(x + widthLeft, y, x + width - widthRight, y + heightTop), m_partTop, param);
			}
		}
		if (heightBottom > 0) {
			if (widthLeft > 0 && m_partBottomLeft.isNotNull()) {
				canvas->draw(Rectangle(x, y + height - heightBottom, x + widthLeft, y + height), m_partBottomLeft, param);
			}
			if (widthRight > 0 && m_partBottomRight.isNotNull()) {
				canvas->draw(Rectangle(x + width - widthRight, y + height - heightBottom, x + width, y + height), m_partBottomRight, param);
			}
			if (m_partBottom.isNotNull()) {
				canvas->draw(Rectangle(x + widthLeft, y + height - heightBottom, x + width - widthRight, y + height), m_partBottom, param);
			}
		}
		if (widthLeft > 0 && m_partLeft.isNotNull()) {
			canvas->draw(Rectangle(x, y + heightTop, x + widthLeft, y + height - heightBottom), m_partLeft, param);
		}
		if (widthRight > 0 && m_partRight.isNotNull()) {
			canvas->draw(Rectangle(x + width - widthRight, y + heightTop, x + width, y + height - heightBottom), m_partRight, param);
		}
		if (m_partCenter.isNotNull()) {
			canvas->draw(Rectangle(x + widthLeft, y + heightTop, x + width - widthRight, y + height - heightBottom), m_partCenter, param);
		}
	}


	SLIB_DEFINE_OBJECT(NinePatchDrawable, Drawable)

	NinePatchDrawable::NinePatchDrawable()
	{
		m_widthLeftDst = 0;
		m_widthRightDst = 0;
		m_heightTopDst = 0;
		m_heightBottomDst = 0;
		m_widthLeftSrc = 0;
		m_widthRightSrc = 0;
		m_heightTopSrc = 0;
		m_heightBottomSrc = 0;
	}

	NinePatchDrawable::~NinePatchDrawable()
	{
	}

	Ref<Drawable> NinePatchDrawable::create(sl_real leftWidthDst, sl_real rightWidthDst, sl_real topHeightDst, sl_real bottomHeightDst,
					const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		if (src.isNull()) {
			return sl_null;
		}
		
		if (leftWidthSrc + rightWidthSrc + SLIB_EPSILON > src->getDrawableWidth()) {
			return sl_null;
		}
		if (topHeightSrc + bottomHeightSrc + SLIB_EPSILON > src->getDrawableWidth()) {
			return sl_null;
		}
		
		Ref<NinePatchDrawable> ret = new NinePatchDrawable;
		
		if (ret.isNotNull()) {
			if (leftWidthDst < 0) {
				leftWidthDst = 0;
			}
			if (rightWidthDst < 0) {
				rightWidthDst = 0;
			}
			if (topHeightDst < 0) {
				topHeightDst = 0;
			}
			if (bottomHeightDst < 0) {
				bottomHeightDst = 0;
			}
			if (leftWidthSrc < 0) {
				leftWidthSrc = 0;
			}
			if (rightWidthSrc < 0) {
				rightWidthSrc = 0;
			}
			if (topHeightSrc < 0) {
				topHeightSrc = 0;
			}
			if (bottomHeightSrc < 0) {
				bottomHeightSrc = 0;
			}

			ret->m_widthLeftDst = leftWidthDst;
			ret->m_widthRightDst = rightWidthDst;
			ret->m_heightTopDst = topHeightDst;
			ret->m_heightBottomDst = bottomHeightDst;
			ret->m_src = src;
			ret->m_widthLeftSrc = leftWidthSrc;
			ret->m_widthRightSrc = rightWidthSrc;
			ret->m_heightTopSrc = topHeightSrc;
			ret->m_heightBottomSrc = bottomHeightSrc;
			
			return ret;
			
		}
		return sl_null;
	}

	Ref<Drawable> NinePatchDrawable::create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		return create(leftWidthSrc, rightWidthSrc, topHeightSrc, bottomHeightSrc, src, leftWidthSrc, rightWidthSrc, topHeightSrc, bottomHeightSrc);
	}

	Ref<Drawable> NinePatchDrawable::create(sl_int32 leftWidthDst, sl_int32 rightWidthDst, sl_int32 topHeightDst, sl_int32 bottomHeightDst,
											const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		return create((sl_real)leftWidthDst, (sl_real)rightWidthDst, (sl_real)topHeightDst, (sl_real)bottomHeightDst, src, leftWidthSrc, rightWidthSrc, topHeightSrc, bottomHeightSrc);
	}

	void NinePatchDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Ref<Drawable> src = m_src;
		if (src.isNull()) {
			return;
		}
		sl_real xDst = rectDst.left;
		sl_real yDst = rectDst.top;
		sl_real widthDst = rectDst.getWidth();
		sl_real heightDst = rectDst.getHeight();
		sl_real widthLeftDst = m_widthLeftDst;
		sl_real widthRightDst = m_widthRightDst;
		sl_real heightTopDst = m_heightTopDst;
		sl_real heightBottomDst = m_heightBottomDst;
		if (widthDst < widthLeftDst + widthRightDst || heightDst < heightTopDst + heightBottomDst) {
			canvas->draw(rectDst, src, param);
			return;
		}
		sl_real widthLeftSrc = m_widthLeftSrc;
		sl_real widthRightSrc = m_widthRightSrc;
		sl_real heightTopSrc = m_heightTopSrc;
		sl_real heightBottomSrc = m_heightBottomSrc;
		sl_real widthSrc = src->getDrawableWidth();
		sl_real heightSrc = src->getDrawableHeight();
		if (widthSrc < widthLeftSrc + widthRightSrc || heightSrc < heightTopSrc + heightBottomSrc) {
			canvas->draw(rectDst, src, param);
			return;
		}
		if (heightTopDst > 0 && heightTopSrc > 0) {
			sl_real topDst = yDst;
			sl_real bottomDst = yDst + heightTopDst;
			sl_real topSrc = 0;
			sl_real bottomSrc = heightTopSrc;
			if (widthLeftDst > 0 && widthLeftSrc > 0) {
				canvas->draw(Rectangle(xDst, topDst, xDst + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc), param);
			}
			if (widthRightDst > 0 && widthRightSrc > 0) {
				canvas->draw(Rectangle(xDst + widthDst - widthRightDst, topDst, xDst + widthDst, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc), param);
			}
			canvas->draw(Rectangle(xDst + widthLeftDst, topDst, xDst + widthDst - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc), param);
		}
		if (heightBottomDst > 0 && heightBottomSrc > 0) {
			sl_real topDst = yDst + heightDst - heightBottomDst;
			sl_real bottomDst = yDst + heightDst;
			sl_real topSrc = heightSrc - heightBottomSrc;
			sl_real bottomSrc = heightSrc;
			if (widthLeftDst > 0 && widthLeftSrc > 0) {
				canvas->draw(Rectangle(xDst, topDst, xDst + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc), param);
			}
			if (widthRightDst > 0 && widthRightSrc > 0) {
				canvas->draw(Rectangle(xDst + widthDst - widthRightDst, topDst, xDst + widthDst, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc), param);
			}
			canvas->draw(Rectangle(xDst + widthLeftDst, topDst, xDst + widthDst - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc), param);
		}
		{
			sl_real topDst = yDst + heightTopDst;
			sl_real bottomDst = yDst + heightDst - heightBottomDst;
			sl_real topSrc = heightTopSrc;
			sl_real bottomSrc = heightSrc - heightBottomSrc;
			if (widthLeftDst > 0 && widthLeftSrc > 0) {
				canvas->draw(Rectangle(xDst, topDst, xDst + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc), param);
			}
			if (widthRightDst > 0 && widthRightSrc > 0) {
				canvas->draw(Rectangle(xDst + widthDst - widthRightDst, topDst, xDst + widthDst, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc), param);
			}
			canvas->draw(Rectangle(xDst + widthLeftDst, topDst, xDst + widthDst - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc), param);
		}
	}


	SLIB_DEFINE_OBJECT(HorizontalThreePatchDrawable, Drawable)

	HorizontalThreePatchDrawable::HorizontalThreePatchDrawable()
	{
		m_widthLeftDst = 0;
		m_widthRightDst = 0;
		m_widthLeftSrc = 0;
		m_widthRightSrc = 0;
	}

	HorizontalThreePatchDrawable::~HorizontalThreePatchDrawable()
	{
	}

	Ref<Drawable> HorizontalThreePatchDrawable::create(sl_real leftWidthDst, sl_real rightWidthDst,
											const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc)
	{
		if (src.isNull()) {
			return sl_null;
		}
		
		if (leftWidthSrc + rightWidthSrc + SLIB_EPSILON > src->getDrawableWidth()) {
			return sl_null;
		}

		Ref<HorizontalThreePatchDrawable> ret = new HorizontalThreePatchDrawable;
		
		if (ret.isNotNull()) {
			if (leftWidthDst < 0) {
				leftWidthDst = 0;
			}
			if (rightWidthDst < 0) {
				rightWidthDst = 0;
			}
			if (leftWidthSrc < 0) {
				leftWidthSrc = 0;
			}
			if (rightWidthSrc < 0) {
				rightWidthSrc = 0;
			}
			
			ret->m_widthLeftDst = leftWidthDst;
			ret->m_widthRightDst = rightWidthDst;
			ret->m_src = src;
			ret->m_widthLeftSrc = leftWidthSrc;
			ret->m_widthRightSrc = rightWidthSrc;

			return ret;
			
		}
		return sl_null;
	}

	Ref<Drawable> HorizontalThreePatchDrawable::create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc)
	{
		return create(leftWidthSrc, rightWidthSrc, src, leftWidthSrc, rightWidthSrc);
	}

	Ref<Drawable> HorizontalThreePatchDrawable::create(sl_int32 leftWidthDst, sl_int32 rightWidthDst,
											const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc)
	{
		return create((sl_real)leftWidthDst, (sl_real)rightWidthDst, src, leftWidthSrc, rightWidthSrc);
	}

	void HorizontalThreePatchDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Ref<Drawable> src = m_src;
		if (src.isNull()) {
			return;
		}
		sl_real xDst = rectDst.left;
		sl_real yDst = rectDst.top;
		sl_real widthDst = rectDst.getWidth();
		sl_real heightDst = rectDst.getHeight();
		sl_real widthLeftDst = m_widthLeftDst;
		sl_real widthRightDst = m_widthRightDst;
		if (widthDst < widthLeftDst + widthRightDst) {
			canvas->draw(rectDst, src, param);
			return;
		}
		sl_real widthLeftSrc = m_widthLeftSrc;
		sl_real widthRightSrc = m_widthRightSrc;
		sl_real widthSrc = src->getDrawableWidth();
		sl_real heightSrc = src->getDrawableHeight();
		if (widthSrc < widthLeftSrc + widthRightSrc) {
			canvas->draw(rectDst, src, param);
			return;
		}
		if (widthLeftDst > 0 && widthLeftSrc > 0) {
			canvas->draw(Rectangle(xDst, yDst, xDst + widthLeftDst, yDst + heightDst), src, Rectangle(0, 0, widthLeftSrc, heightSrc), param);
		}
		if (widthRightDst > 0 && widthRightSrc > 0) {
			canvas->draw(Rectangle(xDst + widthDst - widthRightDst, yDst, xDst + widthDst, yDst + heightDst), src, Rectangle(widthSrc - widthRightSrc, 0, widthSrc, heightSrc), param);
		}
		canvas->draw(Rectangle(xDst + widthLeftDst, yDst, xDst + widthDst - widthRightDst, yDst + heightDst), src, Rectangle(widthLeftSrc, 0, widthSrc - widthRightSrc, heightSrc), param);
	}


	SLIB_DEFINE_OBJECT(VerticalThreePatchDrawable, Drawable)

	VerticalThreePatchDrawable::VerticalThreePatchDrawable()
	{
		m_heightTopDst = 0;
		m_heightBottomDst = 0;
		m_heightTopSrc = 0;
		m_heightBottomSrc = 0;
	}

	VerticalThreePatchDrawable::~VerticalThreePatchDrawable()
	{
	}

	Ref<Drawable> VerticalThreePatchDrawable::create(sl_real topHeightDst, sl_real bottomHeightDst,
											const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		if (src.isNull()) {
			return sl_null;
		}
		
		if (topHeightSrc + bottomHeightSrc + SLIB_EPSILON > src->getDrawableHeight()) {
			return sl_null;
		}
		
		Ref<VerticalThreePatchDrawable> ret = new VerticalThreePatchDrawable;
		
		if (ret.isNotNull()) {
			if (topHeightDst < 0) {
				topHeightDst = 0;
			}
			if (bottomHeightDst < 0) {
				bottomHeightDst = 0;
			}
			if (topHeightSrc < 0) {
				topHeightSrc = 0;
			}
			if (bottomHeightSrc < 0) {
				bottomHeightSrc = 0;
			}
			
			ret->m_heightTopDst = topHeightDst;
			ret->m_heightBottomDst = bottomHeightDst;
			ret->m_src = src;
			ret->m_heightTopSrc = topHeightSrc;
			ret->m_heightBottomSrc = bottomHeightSrc;
			
			return ret;
			
		}
		return sl_null;
	}

	Ref<Drawable> VerticalThreePatchDrawable::create(const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		return create(topHeightSrc, bottomHeightSrc, src, topHeightSrc, bottomHeightSrc);
	}

	Ref<Drawable> VerticalThreePatchDrawable::create(sl_int32 topHeightDst, sl_int32 bottomHeightDst,
											const Ref<Drawable>& src, sl_real topHeightSrc, sl_real bottomHeightSrc)
	{
		return create((sl_real)topHeightDst, (sl_real)bottomHeightDst, src, topHeightSrc, bottomHeightSrc);
	}

	void VerticalThreePatchDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Ref<Drawable> src = m_src;
		if (src.isNull()) {
			return;
		}
		sl_real xDst = rectDst.left;
		sl_real yDst = rectDst.top;
		sl_real widthDst = rectDst.getWidth();
		sl_real heightDst = rectDst.getHeight();
		sl_real heightTopDst = m_heightTopDst;
		sl_real heightBottomDst = m_heightBottomDst;
		if (heightDst < heightTopDst + heightBottomDst) {
			canvas->draw(rectDst, src, param);
			return;
		}
		sl_real heightTopSrc = m_heightTopSrc;
		sl_real heightBottomSrc = m_heightBottomSrc;
		sl_real widthSrc = src->getDrawableWidth();
		sl_real heightSrc = src->getDrawableHeight();
		if (heightSrc < heightTopSrc + heightBottomSrc) {
			canvas->draw(rectDst, src, param);
			return;
		}
		if (heightTopDst > 0 && heightTopSrc > 0) {
			canvas->draw(Rectangle(xDst, yDst, xDst + widthDst, yDst + heightTopDst), src, Rectangle(0, 0, widthSrc, heightTopSrc), param);
		}
		if (heightBottomDst > 0 && heightBottomSrc > 0) {
			canvas->draw(Rectangle(xDst, yDst + heightDst - heightBottomDst, xDst + widthDst, yDst + heightDst), src, Rectangle(0, heightSrc - heightBottomSrc, widthSrc, heightSrc), param);
		}
		canvas->draw(Rectangle(xDst, yDst + heightTopDst, xDst + widthDst, yDst + heightDst - heightBottomDst), src, Rectangle(0, heightTopSrc, widthSrc, heightSrc - heightBottomSrc), param);
	}


	SLIB_DEFINE_OBJECT(MipmapDrawable, Drawable)

	MipmapDrawable::MipmapDrawable()
	{
		m_width = 1;
		m_height = 1;
	}

	MipmapDrawable::~MipmapDrawable()
	{
	}

	sl_real MipmapDrawable::getDrawableWidth()
	{
		return m_width;
	}

	void MipmapDrawable::setDrawableWidth(sl_real width)
	{
		m_width = width;
	}

	sl_real MipmapDrawable::getDrawableHeight()
	{
		return m_height;
	}

	void MipmapDrawable::setDrawableHeight(sl_real height)
	{
		m_height = height;
	}

	void MipmapDrawable::addSource(const Ref<Drawable>& source, sl_real width, sl_real height)
	{
		if (source.isNotNull()) {
			Source src;
			src.drawable = source;
			src.width = width;
			src.height = height;
			m_sources.add(src);
		}
	}

	void MipmapDrawable::addSource(const Ref<Drawable>& source)
	{
		if (source.isNotNull()) {
			Source src;
			src.drawable = source;
			src.width = source->getDrawableWidth();
			src.height = source->getDrawableHeight();
			m_sources.add(src);
		}
	}

	Ref<Drawable> MipmapDrawable::getSource(sl_size index)
	{
		Source source;
		if (m_sources.getAt(index, &source)) {
			return source.drawable;
		}
		return sl_null;
	}

	sl_size MipmapDrawable::getSourcesCount()
	{
		return m_sources.getCount();
	}

	Ref<Drawable> MipmapDrawable::getMatchingSource(sl_real requiredWidth, sl_real requiredHeight)
	{
		if (requiredWidth == 0 && requiredHeight == 0) {
			return getSource(0);
		}
		
		ListLocker<Source> sources(m_sources);
		if (sources.count == 0) {
			return sl_null;
		}
		if (sources.count == 1) {
			return sources[0].drawable;
		}
		
		sl_size i;
		sl_real minSize = 0;
		Ref<Drawable> minSrc;
		for (i = 0; i < sources.count; i++) {
			Source& s = sources[i];
			sl_real width = s.width;
			sl_real height = s.height;
			if (width >= requiredWidth && height >= requiredHeight) {
				sl_real size = width * height;
				if (minSize == 0 || size < minSize) {
					minSize = size;
					minSrc = s.drawable;
				}
			}
		}
		if (minSrc.isNotNull()) {
			return minSrc;
		}
		
		sl_real maxSize = 0;
		Ref<Drawable> maxSrc;
		for (i = 0; i < sources.count; i++) {
			Source& s = sources[i];
			sl_real width = s.width;
			sl_real height = s.height;
			if (width < requiredWidth || height < requiredHeight) {
				sl_real size = width * height;
				if (maxSize == 0 || size > maxSize) {
					maxSize = size;
					maxSrc = s.drawable;
				}
			}
		}
		if (maxSrc.isNotNull()) {
			return maxSrc;
		}
		return sl_null;
	}

	void MipmapDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, m_width, m_height));
		sl_real width = rectDstWhole.getWidth();
		sl_real height = rectDstWhole.getHeight();
		if (width > 0 && height > 0) {
			Ref<Drawable> drawable = getMatchingSource(width, height);
			if (drawable.isNotNull()) {
				float fx = drawable->getDrawableWidth() / m_width;
				float fy = drawable->getDrawableHeight() / m_height;
				Rectangle r;
				r.left = rectSrc.left * fx;
				r.top = rectSrc.top * fy;
				r.right = rectSrc.right * fx;
				r.bottom = rectSrc.bottom * fy;
				canvas->draw(rectDst, drawable, r, param);
			}
		}
	}

	void MipmapDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		sl_real width = rectDst.getWidth();
		sl_real height = rectDst.getHeight();
		if (width > 0 && height > 0) {
			Ref<Drawable> drawable = getMatchingSource(width, height);
			if (drawable.isNotNull()) {
				canvas->draw(rectDst, drawable, param);
			}
		}
	}

	sl_bool MipmapDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		Source source;
		if (m_sources.getAt(0, &source)) {
			return source.drawable->getAnimationInfo(info);
		}
		return sl_false;
	}
	
	
	SLIB_DEFINE_OBJECT(AnimationDrawable, Drawable)
	
	AnimationDrawable::AnimationDrawable()
	{
		m_width = 0;
		m_height = 0;
		m_drawables = List< Ref<Drawable> >::create();
		m_duration = 1;
	}
	
	AnimationDrawable::~AnimationDrawable()
	{
	}

	sl_real AnimationDrawable::getDrawableWidth()
	{
		sl_real width = m_width;
		if (width > 0) {
			return width;
		}
		return 1;
	}
	
	void AnimationDrawable::setDrawableWidth(sl_real width)
	{
		m_width = width;
	}
	
	sl_real AnimationDrawable::getDrawableHeight()
	{
		sl_real height = m_height;
		if (height > 0) {
			return height;
		}
		return 1;
	}

	void AnimationDrawable::setDrawableHeight(sl_real height)
	{
		m_height = height;
	}
	
	void AnimationDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		Ref<Drawable> drawable = getDrawableAtTime(param.time);
		if (drawable.isNotNull())  {
			drawable->onDraw(canvas, rectDst, rectSrc, param);
		}
	}
	
	void AnimationDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		Ref<Drawable> drawable = getDrawableAtTime(param.time);
		if (drawable.isNotNull())  {
			drawable->onDrawAll(canvas, rectDst, param);
		}
	}
	
	sl_bool AnimationDrawable::getAnimationInfo(DrawableAnimationInfo* info)
	{
		if (info) {
			float duration = m_duration;
			info->duration = duration;
			info->framesPerSecond = duration < 0.001f ? 0 : (float)(m_drawables.getCount()) / duration;
		}
		return sl_true;
	}
	
	void AnimationDrawable::setAnimationDuration(float duration)
	{
		m_duration = duration;
	}
	
	List< Ref<Drawable> > AnimationDrawable::getDrawables()
	{
		return m_drawables;
	}
	
	void AnimationDrawable::addDrawable(const Ref<Drawable>& drawable)
	{
		m_drawables.add(drawable);
		if (drawable.isNotNull()) {
			if (m_width == 0) {
				m_width = drawable->getDrawableWidth();
			}
			if (m_height == 0) {
				m_height = drawable->getDrawableHeight();
			}
		}
	}
	
	Ref<Drawable> AnimationDrawable::getDrawableAtTime(float time)
	{
		if (time < 0.001f) {
			return m_drawables.getValueAt(0);
		}
		float duration = m_duration;
		if (duration < 0.001f) {
			return m_drawables.getValueAt(0);
		}
		float n = Math::floor(time / duration);
		float t = time - n * duration;
		sl_size count = m_drawables.getCount();
		sl_size index = (sl_size)((float)(count) * t / duration);
		if (index >= count) {
			index = count - 1;
		}
		return m_drawables.getValueAt(index);
	}
	
}
