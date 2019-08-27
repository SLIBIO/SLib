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

#include "slib/graphics/canvas.h"

#include "slib/graphics/util.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CanvasExt, Canvas)
	
	CanvasExt::CanvasExt()
	{
	}
	
	CanvasExt::~CanvasExt()
	{
	}
	
	void CanvasExt::clipToRoundRect(const Rectangle& rect, const Size& radius)
	{
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addRoundRect(rect, radius);
			clipToPath(path);
		}
	}
	
	void CanvasExt::clipToEllipse(const Rectangle& rect)
	{
		Ref<GraphicsPath> path = GraphicsPath::create();
		if (path.isNotNull()) {
			path->addEllipse(rect);
			clipToPath(path);
		}
	}
	
	Size CanvasExt::measureText(const Ref<Font>& font, const String& text, sl_bool flagMultiLine)
	{
		if (font.isNotNull()) {
			return font->measureText(text, flagMultiLine);
		}
		return Size::zero();
	}
	
	Size CanvasExt::measureText16(const Ref<Font>& font, const String16& text, sl_bool flagMultiLine)
	{
		if (font.isNotNull()) {
			return font->measureText16(text, flagMultiLine);
		}
		return Size::zero();
	}
	
	void CanvasExt::drawText(const String& text, const Rectangle& rcDst, const Ref<Font>& _font, const Color& color, const Alignment& align, sl_bool flagMultiLine)
	{
		if (flagMultiLine) {
			drawText16(text, rcDst, _font, color, align, sl_true);
			return;
		}
		Ref<Font> font = _font;
		if (font.isNull()) {
			font = Font::getDefault();
			if (font.isNull()) {
				return;
			}
		}
		Size size = measureText(font, text, sl_false);
		Point pt = GraphicsUtil::calculateAlignPosition(rcDst, size.x, size.y, align);
		((Canvas*)this)->drawText(text, pt.x, pt.y, font, color);
	}
	
	void CanvasExt::drawText16(const String16& text, const Rectangle& rcDst, const Ref<Font>& _font, const Color& color, const Alignment& align, sl_bool flagMultiLine)
	{
		Ref<Font> font = _font;
		if (font.isNull()) {
			font = Font::getDefault();
			if (font.isNull()) {
				return;
			}
		}
		Size size = measureText16(font, text, flagMultiLine);
		Point pt = GraphicsUtil::calculateAlignPosition(rcDst, size.x, size.y, align);
		if (flagMultiLine) {
			Alignment hAlign = align & Alignment::HorizontalMask;
			sl_char16* sz = text.getData();
			sl_size len = text.getLength();
			sl_size startLine = 0;
			sl_size pos = 0;
			sl_real y = pt.y;
			while (pos <= len) {
				sl_char16 ch;
				if (pos < len) {
					ch = sz[pos];
				} else {
					ch = '\n';
				}
				if (ch == '\r' || ch == '\n') {
					if (pos > startLine) {
						String16 line(sz + startLine, pos - startLine);
						Size s = measureText16(font, line);
						sl_real x;
						if (hAlign == Alignment::Center) {
							x = pt.x + (size.x - s.x) / 2;
						} else if (hAlign == Alignment::Right) {
							x = pt.x + size.x - s.x;
						} else {
							x = pt.x;
						}
						((Canvas*)this)->drawText16(line, x, y, font, color);
						y += s.y;
					}
					if (ch == '\r' && pos + 1 < len) {
						if (sz[pos + 1] == '\n') {
							pos++;
						}
					}
					startLine = pos + 1;
				}
				pos++;
			}
		} else {
			((Canvas*)this)->drawText16(text, pt.x, pt.y, font, color);
		}
	}
	
	void CanvasExt::drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Color& fillColor)
	{
		((Canvas*)this)->drawRectangle(rect, pen, Brush::createSolidBrush(fillColor));
	}
	
	void CanvasExt::drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Color& fillColor)
	{
		((Canvas*)this)->drawRoundRect(rect, radius, pen, Brush::createSolidBrush(fillColor));
	}
	
	void CanvasExt::drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Color& fillColor)
	{
		((Canvas*)this)->drawEllipse(rect, pen, Brush::createSolidBrush(fillColor));
	}
	
	void CanvasExt::drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Color& fillColor, FillMode fillMode)
	{
		((Canvas*)this)->drawPolygon(points, countPoints, pen, Brush::createSolidBrush(fillColor), fillMode);
	}
	
	void CanvasExt::drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Color& fillColor)
	{
		((Canvas*)this)->drawPie(rect, startDegrees, sweepDegrees, pen, Brush::createSolidBrush(fillColor));
	}
	
	void CanvasExt::drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Color& fillColor)
	{
		((Canvas*)this)->drawPath(path, pen, Brush::createSolidBrush(fillColor));
	}
	
	void CanvasExt::draw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param)
	{
		if (src.isNull()) {
			return;
		}
		if (param.isTransparent()) {
			return;
		}
		if (rectDst.getWidth() < SLIB_EPSILON) {
			return;
		}
		if (rectDst.getHeight() < SLIB_EPSILON) {
			return;
		}
		if (rectSrc.getWidth() < SLIB_EPSILON) {
			return;
		}
		if (rectSrc.getHeight() < SLIB_EPSILON) {
			return;
		}
		onDraw(rectDst, src, rectSrc, param);
	}
	
	void CanvasExt::draw(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param)
	{
		if (src.isNull()) {
			return;
		}
		if (param.isTransparent()) {
			return;
		}
		if (rectDst.getWidth() < SLIB_EPSILON) {
			return;
		}
		if (rectDst.getHeight() < SLIB_EPSILON) {
			return;
		}
		sl_real sw = src->getDrawableWidth();
		if (sw < SLIB_EPSILON) {
			return;
		}
		sl_real sh = src->getDrawableHeight();
		if (sh < SLIB_EPSILON) {
			return;
		}
		onDrawAll(rectDst, src, param);
	}
	
	void CanvasExt::draw(sl_real xDst, sl_real yDst, const Ref<Drawable>& src, const DrawParam& param)
	{
		if (src.isNull()) {
			return;
		}
		if (param.isTransparent()) {
			return;
		}
		sl_real sw = src->getDrawableWidth();
		if (sw < SLIB_EPSILON) {
			return;
		}
		sl_real sh = src->getDrawableHeight();
		if (sh < SLIB_EPSILON) {
			return;
		}
		Rectangle rectDst(xDst, yDst, xDst + sw, yDst + sh);
		onDrawAll(rectDst, src, param);
	}
	
	void CanvasExt::draw(const Rectangle& rectDst, const Ref<Drawable>& source, ScaleMode scaleMode, const Alignment& alignment, const DrawParam& param)
	{
		if (source.isNull()) {
			return;
		}
		if (param.isTransparent()) {
			return;
		}
		if (scaleMode == ScaleMode::Cover) {
			sl_real dw = rectDst.getWidth();
			if (dw < SLIB_EPSILON) {
				return;
			}
			sl_real dh = rectDst.getHeight();
			if (dh < SLIB_EPSILON) {
				return;
			}
			sl_real sw = source->getDrawableWidth();
			if (sw < SLIB_EPSILON) {
				return;
			}
			sl_real sh = source->getDrawableHeight();
			if (sh < SLIB_EPSILON) {
				return;
			}
			sl_real fw = sw / dw;
			sl_real fh = sh / dh;
			sl_real tw, th;
			if (fw > fh) {
				th = sh;
				tw = dw * fh;
			} else {
				tw = sw;
				th = dh * fw;
			}
			Rectangle rectSrc;
			rectSrc.left = 0;
			rectSrc.top = 0;
			rectSrc.right = sw;
			rectSrc.bottom = sh;
			Point pt = GraphicsUtil::calculateAlignPosition(rectSrc, tw, th, alignment);
			rectSrc.left = pt.x;
			rectSrc.top = pt.y;
			rectSrc.right = rectSrc.left + tw;
			rectSrc.bottom = rectSrc.top + th;
			onDraw(rectDst, source, rectSrc, param);
		} else {
			Rectangle rectDraw;
			if (GraphicsUtil::calculateAlignRectangle(rectDraw, rectDst, source->getDrawableWidth(), source->getDrawableHeight(), scaleMode, alignment)) {
				onDrawAll(rectDraw, source, param);
			}
		}
	}
	
	void CanvasExt::onDraw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param)
	{
		src->onDraw(this, rectDst, rectSrc, param);
	}
	
	void CanvasExt::onDrawAll(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param)
	{
		src->onDrawAll(this, rectDst, param);
	}
	
	sl_bool CanvasExt::isSupportedDrawable(const Ref<Drawable>& drawable)
	{
		return sl_true;
	}
	
	Ref<Drawable> CanvasExt::createDrawableCacheForImage(const Ref<Image>& image)
	{
		return PlatformDrawable::create(image);
	}

}
