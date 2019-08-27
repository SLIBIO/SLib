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

#include "slib/graphics/util.h"

#include "slib/core/math.h"
#include "slib/graphics/canvas.h"
#include "slib/graphics/drawable.h"
#include "slib/device/device.h"

namespace slib
{

	sl_bool GraphicsUtil::containsPointInRoundRect(const Point& pt, const Rectangle& rect, const Size& radius)
	{
		sl_real x = pt.x;
		sl_real y = pt.y;
		if (x < rect.left) {
			return sl_false;
		}
		if (x > rect.right) {
			return sl_false;
		}
		if (y < rect.top) {
			return sl_false;
		}
		if (y > rect.bottom) {
			return sl_false;
		}
		x -= rect.left;
		if (x > radius.x) {
			sl_real t = rect.right - rect.left - radius.x;
			if (x < t) {
				return sl_true;
			}
			x = x - t;
		} else {
			x -= radius.x;
		}
		y -= rect.top;
		if (y > radius.y) {
			sl_real t = rect.bottom - rect.top - radius.y;
			if (y < t) {
				return sl_true;
			}
			y = y - t;
		} else {
			y -= radius.y;
		}
		if (Math::isAlmostZero(radius.x)) {
			return sl_false;
		}
		if (Math::isAlmostZero(radius.y)) {
			return sl_false;
		}
		sl_real rx2 = radius.x * radius.x;
		sl_real ry2 = radius.y * radius.y;
		return x * x * ry2 + y * y * rx2 <= rx2 * ry2;
	}

	sl_bool GraphicsUtil::containsPointInEllipse(const Point& pt, const Rectangle& rect)
	{
		sl_real rx = rect.right - rect.left;
		if (Math::isAlmostZero(rx)) {
			return sl_false;
		}
		sl_real ry = rect.bottom - rect.top;
		if (Math::isAlmostZero(ry)) {
			return sl_false;
		}
		sl_real rx2 = rx * rx;
		sl_real ry2 = ry * ry;
		sl_real x = (pt.x - (rect.right + rect.left) / 2);
		sl_real y = (pt.y - (rect.top + rect.bottom) / 2);
		return 4 * (x * x * ry2 + y * y * rx2) <= rx2 * ry2;
	}

	sl_real GraphicsUtil::calculateAlignX(sl_real leftDst, sl_real rightDst, sl_real widthSrc, const Alignment& align)
	{
		Alignment hAlign = align & Alignment::HorizontalMask;
		switch (hAlign) {
			case Alignment::Left:
				break;
			case Alignment::Center:
				return leftDst + (rightDst - leftDst - widthSrc) / 2;
			case Alignment::Right:
				return rightDst - widthSrc;
			default:
				break;
		}
		return leftDst;
	}

	sl_real GraphicsUtil::calculateAlignY(sl_real topDst, sl_real bottomDst, sl_real heightSrc, const Alignment& align)
	{
		Alignment vAlign = align & Alignment::VerticalMask;
		switch (vAlign) {
			case Alignment::Top:
				break;
			case Alignment::Middle:
				return topDst + (bottomDst - topDst - heightSrc) / 2;
			case Alignment::Bottom:
				return bottomDst - heightSrc;
			default:
				break;
		}
		return topDst;
	}

	Point GraphicsUtil::calculateAlignPosition(const Rectangle& rcDst, sl_real widthSrc, sl_real heightSrc, const Alignment& align)
	{
		Point ret;
		ret.x = calculateAlignX(rcDst.left, rcDst.right, widthSrc, align);
		ret.y = calculateAlignY(rcDst.top, rcDst.bottom, heightSrc, align);
		return ret;
	}
	
	sl_bool GraphicsUtil::calculateAlignRectangle(Rectangle& rectOutput, const Rectangle& rectDst, sl_real sw, sl_real sh, ScaleMode scaleMode, const Alignment& alignment)
	{
		if (sw < SLIB_EPSILON) {
			return sl_false;
		}
		if (sh < SLIB_EPSILON) {
			return sl_false;
		}
		sl_real dw = rectDst.getWidth();
		if (dw < SLIB_EPSILON) {
			return sl_false;
		}
		sl_real dh = rectDst.getHeight();
		if (dh < SLIB_EPSILON) {
			return sl_false;
		}
		switch (scaleMode) {
			case ScaleMode::None:
			{
				Point pt = calculateAlignPosition(rectDst, sw, sh, alignment);
				rectOutput.left = pt.x;
				rectOutput.top = pt.y;
				rectOutput.right = rectOutput.left + sw;
				rectOutput.bottom = rectOutput.top + sh;
				return sl_true;
			}
			case ScaleMode::Stretch:
			{
				rectOutput = rectDst;
				return sl_true;
			}
			case ScaleMode::Contain:
			{
				sl_real fw = dw / sw;
				sl_real fh = dh / sh;
				sl_real tw, th;
				if (fw > fh) {
					th = dh;
					tw = sw * fh;
				} else {
					tw = dw;
					th = sh * fw;
				}
				Point pt = GraphicsUtil::calculateAlignPosition(rectDst, tw, th, alignment);
				rectOutput.left = pt.x;
				rectOutput.top = pt.y;
				rectOutput.right = rectOutput.left + tw;
				rectOutput.bottom = rectOutput.top + th;
				return sl_true;
			}
			case ScaleMode::Cover:
			{
				sl_real fw = dw / sw;
				sl_real fh = dh / sh;
				sl_real tw, th;
				if (fw < fh) {
					th = dh;
					tw = sw * fh;
				} else {
					tw = dw;
					th = sh * fw;
				}
				Point pt = GraphicsUtil::calculateAlignPosition(rectDst, tw, th, alignment);
				rectOutput.left = pt.x;
				rectOutput.top = pt.y;
				rectOutput.right = rectOutput.left + tw;
				rectOutput.bottom = rectOutput.top + th;
				return sl_true;
			}
		}
		return sl_false;
	}

	Rectangle GraphicsUtil::transformRectangle(const Rectangle& rectTransform, const Rectangle& rectFrom, const Rectangle& rectTo)
	{
		sl_real w1 = rectFrom.getWidth();
		sl_real h1 = rectFrom.getHeight();
		if (Math::isAlmostZero(w1) || Math::isAlmostZero(h1)) {
			return Rectangle::zero();
		}
		sl_real w2 = rectTo.getWidth();
		sl_real h2 = rectTo.getHeight();
		if (Math::isAlmostZero(w2) || Math::isAlmostZero(h2)) {
			return Rectangle::zero();
		}
		sl_real sw = rectTransform.getWidth();
		sl_real sh = rectTransform.getHeight();
		sl_real w = sw * w2 / w1;
		sl_real h = sh * h2 / h1;
		Rectangle ret;
		ret.left = rectTransform.left + (rectTo.left - rectFrom.left) * sw / w1;
		ret.top = rectTransform.top + (rectTo.top - rectFrom.top) * sh / h1;
		ret.right = ret.left + w;
		ret.bottom = ret.top + h;
		return ret;
	}

	void GraphicsUtil::drawRepeat(Canvas* canvas, const Ref<Drawable>& source, sl_real _dx, sl_real _dy, sl_uint32 nRepeatX, sl_uint32 nRepeatY, const DrawParam& param)
	{
		if (source.isNull()) {
			return;
		}
		sl_real sw = source->getDrawableWidth();
		sl_real sh = source->getDrawableHeight();
		if (sw < SLIB_EPSILON || sh <= SLIB_EPSILON) {
			return;
		}
		Rectangle rectDraw;
		Rectangle rectSrc;
		sl_real dy = _dy;
		for (sl_uint32 j = 0; j < nRepeatY; j++) {
			if (j == 0) {
				rectDraw.top = 0;
				rectDraw.bottom = sh + dy;
				rectSrc.top = -dy;
				rectSrc.bottom = sh;
			} else {
				rectDraw.top = dy;
				rectDraw.bottom = rectDraw.top + sh;
				rectSrc.top = 0;
				rectSrc.bottom = sh;
			}
			sl_real dx = _dx;
			for (sl_uint32 i = 0; i < nRepeatX; i++) {
				if (i == 0) {
					rectDraw.left = 0;
					rectDraw.right = sw + dx;
					rectSrc.left = -dx;
					rectSrc.right = sw;
				} else {
					rectDraw.left = dx;
					rectDraw.right = rectDraw.left + sw;
					rectSrc.left = 0;
					rectSrc.right = sw;
				}
				canvas->draw(rectDraw, source, rectSrc, param);
				dx += sw;
			}
			dy += sh;
		}

	}
	
	sl_real GraphicsUtil::pixelToInch(sl_real px)
	{
		return (sl_real)(px / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::inchToPixel(sl_real inch)
	{
		return (sl_real)(inch * Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::pixelToMeter(sl_real px)
	{
		return (sl_real)(px * 0.0254 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::meterToPixel(sl_real meters)
	{
		return (sl_real)(meters * Device::getScreenPPI() * 39.3701);
	}
	
	sl_real GraphicsUtil::pixelToCentimeter(sl_real px)
	{
		return (sl_real)(px * 2.54 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::centimeterToPixel(sl_real cm)
	{
		return (sl_real)(cm * Device::getScreenPPI() * 0.393701);
	}
	
	sl_real GraphicsUtil::pixelToMillimeter(sl_real px)
	{
		return (sl_real)(px * 25.4 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::millimeterToPixel(sl_real mm)
	{
		return (sl_real)(mm * Device::getScreenPPI() * 0.0393701);
	}
	
	sl_real GraphicsUtil::pixelToPoint(sl_real px)
	{
		return (sl_real)(px * 72 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::pointToPixel(sl_real pt)
	{
		return (sl_real)(pt * Device::getScreenPPI() / 72);
	}
	
	sl_real GraphicsUtil::pixelToPicas(sl_real px)
	{
		return (sl_real)(px * 12 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::picasToPixel(sl_real pc)
	{
		return (sl_real)(pc * Device::getScreenPPI() / 12);
	}
	
	sl_real GraphicsUtil::pixelToDp(sl_real px)
	{
		return (sl_real)(px * 160 / Device::getScreenPPI());
	}
	
	sl_real GraphicsUtil::dpToPixel(sl_real dp)
	{
		return (sl_real)(dp * Device::getScreenPPI() / 160);
	}
	
}
