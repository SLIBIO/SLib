/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/graphics/util.h"

#include "slib/core/math.h"
#include "slib/graphics/canvas.h"
#include "slib/graphics/drawable.h"

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
			x = radius.x + x - t;
		}
		y -= rect.top;
		if (y > radius.y) {
			sl_real t = rect.bottom - rect.top - radius.y;
			if (y < t) {
				return sl_true;
			}
			y = radius.y + y - t;
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

	sl_real GraphicsUtil::calculateAlignX(sl_real leftDst, sl_real rightDst, sl_real widthSrc, Alignment align)
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

	sl_real GraphicsUtil::calculateAlignY(sl_real topDst, sl_real bottomDst, sl_real heightSrc, Alignment align)
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

	Point GraphicsUtil::calculateAlignPosition(const Rectangle &rcDst, sl_real widthSrc, sl_real heightSrc, Alignment align)
	{
		Point ret;
		ret.x = calculateAlignX(rcDst.left, rcDst.right, widthSrc, align);
		ret.y = calculateAlignY(rcDst.top, rcDst.bottom, heightSrc, align);
		return ret;
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

}
