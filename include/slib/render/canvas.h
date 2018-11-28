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

#ifndef CHECKHEADER_SLIB_RENDER_CANVAS
#define CHECKHEADER_SLIB_RENDER_CANVAS

#include "definition.h"

#include "engine.h"

#include "../graphics/canvas.h"
#include "../core/queue.h"
#include "../math/matrix3.h"

namespace slib
{

	enum class RenderCanvasClipType
	{
		Rectangle,
		Ellipse,
		RoundRect
	};
	
	class RenderCanvasClip
	{
	public:
		RenderCanvasClipType type;
		Rectangle region;
		sl_real rx;
		sl_real ry;
		sl_bool flagTransform;
		Matrix3 transform;
		
	public:
		RenderCanvasClip();
		
		~RenderCanvasClip();
		
	};
	
	class RenderCanvasState : public Referable
	{
	public:
		Matrix3 matrix;
		sl_bool flagClipRect;
		Rectangle clipRect;
		List<RenderCanvasClip> clips;
		
	public:
		RenderCanvasState();
		
		RenderCanvasState(RenderCanvasState* other);
		
		~RenderCanvasState();
		
	public:
		void copyFrom(RenderCanvasState* other);
		
	};
	
	class SLIB_EXPORT RenderCanvas : public CanvasExt
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		RenderCanvas();
		
		~RenderCanvas();
		
	public:
		static Ref<RenderCanvas> create(const Ref<RenderEngine>& engine, sl_real width, sl_real height);
		
	public:
		const Ref<RenderEngine>& getEngine();
		
		RenderCanvasState* getCurrentState();
		
		void save() override;
		
		void restore() override;
		
		Rectangle getClipBounds() override;
		
		using Canvas::clipToRectangle;
		void clipToRectangle(const Rectangle& rect) override;
		
		using Canvas::clipToPath;
		void clipToPath(const Ref<GraphicsPath>& path) override;
		
		using Canvas::clipToRoundRect;
		void clipToRoundRect(const Rectangle& rect, const Size& radius) override;
		
		using Canvas::clipToEllipse;
		void clipToEllipse(const Rectangle& rect) override;
		
		using Canvas::concatMatrix;
		void concatMatrix(const Matrix3& matrix) override;
		
		using Canvas::translate;
		void translate(sl_real dx, sl_real dy) override;
		
		using Canvas::measureText;
		Size measureText(const Ref<Font>& font, const String& text, sl_bool flagMultiLine = sl_false) override;
		
		using Canvas::measureText16;
		Size measureText16(const Ref<Font>& font, const String16& text, sl_bool flagMultiLine = sl_false) override;
		
		static Size measureRenderingText(const Ref<Font>& font, const String16& text, sl_bool flagMultiLine = sl_false);
		
		using Canvas::drawText;
		void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& font, const Color& color) override;
		
		using Canvas::drawText16;
		void drawText16(const String16& text, sl_real x, sl_real y, const Ref<Font>& font, const Color& color) override;
		
		using Canvas::drawLine;
		void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& pen) override;
		
		using Canvas::drawLines;
		void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen) override;
		
		using Canvas::drawArc;
		void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen) override;
		
		using Canvas::drawRectangle;
		void drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) override;
		
		void drawRectangle(const Rectangle& rect, const Ref<Pen>& pen, const Color& fillColor) override;
		
		using Canvas::drawRoundRect;
		void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Ref<Brush>& brush) override;
		
		using Canvas::drawEllipse;
		void drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush) override;
		
		using Canvas::drawPolygon;
		void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode = FillMode::Alternate) override;
		
		using Canvas::drawPie;
		void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush) override;
		
		using Canvas::drawPath;
		void drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Ref<Brush>& brush) override;
		
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param, const Color4f& color);
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param);
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const DrawParam& param, const Color4f& color);
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const DrawParam& param);
		
		void drawTexture(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha = 1);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param, const Color4f& color);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const DrawParam& param, const Color4f& color);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const DrawParam& param);
		
		void drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha = 1);
		
		
		void translateFromSavedState(RenderCanvasState* savedState, sl_real dx, sl_real dy);
		
		Matrix3 getTransformMatrixForRectangle(const Rectangle& rect);
		
		void drawRectangle(const Rectangle& rect, RenderProgramState2D_Position* programState, const DrawParam& param);
		
		
	protected:
		void onDraw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param) override;
		
		void onDrawAll(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param) override;
		
	protected:
		void _setAlpha(sl_real alpha) override;
		
		void _setAntiAlias(sl_bool flag) override;
		
		void _drawBitmap(const Rectangle& rectDst, Bitmap* src, const Rectangle& rectSrc, const DrawParam& param);
		
		void _fillRectangle(const Rectangle& rect, const Color& color);
		
	protected:
		Ref<RenderEngine> m_engine;
		sl_real m_width;
		sl_real m_height;
		Matrix3 m_matViewport;
		Ref<RenderCanvasState> m_state;
		LinkedStack< Ref<RenderCanvasState> > m_stackStates;
		
	};

}

#endif

