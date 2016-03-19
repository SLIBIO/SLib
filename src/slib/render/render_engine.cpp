#include "../../../inc/slib/render/engine.h"

#include "../../../inc/slib/render/program.h"
#include "../../../inc/slib/graphics/canvas.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/math/transform3d.h"

SLIB_RENDER_NAMESPACE_BEGIN

Primitive::Primitive()
{
	type = PrimitiveType::Triangles;
	countElements = 0;
}

RenderClearParam::RenderClearParam()
{
	flagColor = sl_true;
	color = Color::Blue;
	flagDepth = sl_true;
	depth = 1.0f;
	flagStencil = sl_false;
	stencil = 0;
}

RenderBlendingParam::RenderBlendingParam()
{
	operation = RenderBlendingOperation::Add;
	operationAlpha = RenderBlendingOperation::Add;
	blendDst = RenderBlendingFactor::OneMinusSrcAlpha;
	blendDstAlpha = RenderBlendingFactor::OneMinusSrcAlpha;
	blendSrc = RenderBlendingFactor::SrcAlpha;
	blendSrcAlpha = RenderBlendingFactor::SrcAlpha;
}

RendererParam::RendererParam()
{
	nRedBits = 8;
	nGreenBits = 8;
	nBlueBits = 8;
	nAlphaBits = 8;
	nAccumBits = 0;
	nDepthBits = 24;
	nStencilBits = 8;
	flagMultisample = sl_false;
}


SLIB_DEFINE_OBJECT(Renderer, Object)

Renderer::Renderer()
{
	setRenderingContinuously(sl_false);
}


SLIB_DEFINE_OBJECT(RenderEngine, Object)

RenderEngine::RenderEngine()
{
	m_viewportWidth = 0;
	m_viewportHeight = 0;

	m_nCountDrawnElementsOnLastScene = 0;
	m_nCountDrawnPrimitivesOnLastScene = 0;
	m_nRenderMillisecondsOnLastScene = 0;
}

sl_bool RenderEngine::isOpenGL()
{
	RenderEngineType type = getEngineType();
	return type == RenderEngineType::OpenGL;
}

sl_bool RenderEngine::isOpenGL_ES()
{
	RenderEngineType type = getEngineType();
	return type == RenderEngineType::OpenGL_ES;
}

sl_bool RenderEngine::isD3D()
{
	RenderEngineType type = getEngineType();
	return type == RenderEngineType::D3D9 || type == RenderEngineType::D3D11;
}

void RenderEngine::release()
{
	_release();
}

sl_bool RenderEngine::isValid()
{
	return _isValid();
}

sl_bool RenderEngine::beginScene()
{
	m_nCountDrawnElementsOnLastScene = 0;
	m_nCountDrawnPrimitivesOnLastScene = 0;
	m_timeBeginLastScene = Time::now();
	return _beginScene();
}

void RenderEngine::endScene()
{
	_endScene();
	m_nRenderMillisecondsOnLastScene = (sl_uint32)((Time::now() - m_timeBeginLastScene).getMillisecondsCount());
}

void RenderEngine::setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	_setViewport(x, y, width, height);
}

void RenderEngine::clear(const RenderClearParam& param)
{
	_clear(param);
}

void RenderEngine::clearColor(const Color& color)
{
	RenderClearParam param;
	param.flagColor = sl_true;
	param.color = color;
	param.flagDepth = sl_false;
	_clear(param);
}

void RenderEngine::clearColorDepth(const Color& color, float depth)
{
	RenderClearParam param;
	param.flagColor = sl_true;
	param.color = color;
	param.flagDepth = sl_true;
	param.depth = depth;
	_clear(param);
}

void RenderEngine::clearDepth(float depth)
{
	RenderClearParam param;
	param.flagColor = sl_false;
	param.flagDepth = sl_true;
	param.depth = depth;
	_clear(param);
}

void RenderEngine::setDepthTest(sl_bool flagEnableDepthTest)
{
	_setDepthTest(flagEnableDepthTest);
}

void RenderEngine::setDepthWriteEnabled(sl_bool flagEnableDepthWrite)
{
	_setDepthWriteEnabled(flagEnableDepthWrite);
}

void RenderEngine::setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW)
{
	_setCullFace(flagEnableCull, flagCullCCW);
}

void RenderEngine::setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param)
{
	_setBlending(flagEnableBlending, param);
}

void RenderEngine::setBlending(sl_bool flagEnableBlending)
{
	RenderBlendingParam param;
	_setBlending(flagEnableBlending, param);
}

sl_bool RenderEngine::beginProgram(const Ref<RenderProgram>& program)
{
	return _beginProgram(program.ptr);
}

void RenderEngine::endProgram()
{
	_endProgram();
}

void RenderEngine::drawPrimitive(Primitive* primitive)
{
	_drawPrimitive(primitive);
	m_nCountDrawnElementsOnLastScene += primitive->countElements;
	m_nCountDrawnPrimitivesOnLastScene++;
}

void RenderEngine::applyTexture(const void* sampler, const Ref<Texture>& texture)
{
	_applyTexture(sampler, texture.ptr);
}

void RenderEngine::applyTexture(sl_reg sampler, const Ref<Texture>& texture)
{
	_applyTexture((void*)sampler, texture.ptr);
}

sl_bool RenderEngine::linkTexture(const Ref<Texture>& texture)
{
	return _linkTexture(texture.ptr);
}

sl_bool RenderEngine::linkVertexBuffer(const Ref<VertexBuffer>& vb)
{
	return _linkVertexBuffer(vb.ptr);
}

sl_bool RenderEngine::linkIndexBuffer(const Ref<IndexBuffer>& ib)
{
	return _linkIndexBuffer(ib.ptr);
}

void RenderEngine::setLineWidth(sl_real width)
{
	_setLineWidth(width);
}

void RenderEngine::draw(const Ref<RenderProgram>& program, Primitive* primitives, sl_uint32 count)
{
	if (beginProgram(program)) {
		for (sl_uint32 i = 0; i < count; i++) {
			drawPrimitive(primitives + i);
		}
		endProgram();
	}
}

void RenderEngine::draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib, PrimitiveType type)
{
	Primitive p;
	p.type = type;
	p.countElements = countElements;
	p.vertexBuffer = vb;
	p.indexBuffer = ib;
	draw(program, &p);
}

void RenderEngine::draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, PrimitiveType type)
{
	Primitive p;
	p.type = type;
	p.countElements = countElements;
	p.vertexBuffer = vb;
	draw(program, &p);
}

void RenderEngine::drawRectangle2D(const Ref<RenderProgram>& program)
{
	Ref<VertexBuffer> vb = _getDefaultVertexBufferForDrawRectangle2D();
	draw(program, 4, vb, PrimitiveType::TriangleStrip);
}

static void _RenderEngine_makeTransform2D(Matrix3& mat, const Rectangle& rectDst)
{
	sl_real x = rectDst.left;
	sl_real y = rectDst.bottom;
	sl_real w = rectDst.right - rectDst.left;
	sl_real h = rectDst.top - rectDst.bottom;
	mat.m00 = w; mat.m01 = 0; mat.m02 = 0;
	mat.m10 = 0; mat.m11 = h; mat.m12 = 0;
	mat.m20 = x; mat.m21 = y; mat.m22 = 1;
}

static void _RenderEngine_makeTextureTransform2D(Matrix3& mat, const Rectangle& rt)
{
	sl_real x = rt.left;
	sl_real y = rt.top;
	sl_real w = rt.right - rt.left;
	sl_real h = rt.bottom - rt.top;
	mat.m00 = w; mat.m01 = 0; mat.m02 = 0;
	mat.m10 = 0; mat.m11 = h; mat.m12 = 0;
	mat.m20 = x; mat.m21 = y; mat.m22 = 1;
}

void RenderEngine::drawRectangle2D(const Matrix3& transform, const Ref<RenderProgram2D>& program)
{
	if (program.isNotNull()) {
		program->setTransform(transform);
		drawRectangle2D(program);
	}
}

void RenderEngine::drawRectangle2D(const Rectangle& rectDst, const Ref<RenderProgram2D>& program)
{
	if (program.isNotNull()) {
		Matrix3 matTransform;
		_RenderEngine_makeTransform2D(matTransform, rectDst);
		program->setTransform(matTransform);
		drawRectangle2D(program);
	}
}

void RenderEngine::drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<RenderProgram2D>& program)
{
	drawRectangle2D(Rectangle(dx, dy, dx + dw, dy + dh), program);
}

void RenderEngine::drawRectangle2D(const Matrix3& transform, const Color4f& color)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawRectangle2D();
	if (p.isNotNull()) {
		p->setColor(color.xyz());
		p->setAlpha(color.w);
		drawRectangle2D(transform, p);
	}
}

void RenderEngine::drawRectangle2D(const Rectangle& rectDst, const Color4f& color)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawRectangle2D();
	if (p.isNotNull()) {
		p->setColor(color.xyz());
		p->setAlpha(color.w);
		drawRectangle2D(rectDst, p);
	}
}

void RenderEngine::drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Color4f& color)
{
	drawRectangle2D(Rectangle(dx, dy, dx + dw, dy + dh), color);
}

Ref<VertexBuffer> RenderEngine::_getDefaultVertexBufferForDrawRectangle2D()
{
	static RenderProgram2D_Position::VertexData v[] = {
		{ { 0, 0 } }
		, { { 1, 0 } }
		, { { 0, 1 } }
		, { { 1, 1 } }
	};
	Ref<VertexBuffer> ret = m_defaultVertexBufferForDrawRectangle2D;
	if (ret.isNull()) {
		ret = VertexBuffer::create(v, sizeof(v));
		m_defaultVertexBufferForDrawRectangle2D = ret;
	}
	return ret;
}

Ref<RenderProgram2D> RenderEngine::_getDefaultRenderProgramForDrawRectangle2D()
{
	Ref<RenderProgram2D> ret = m_defaultRenderProgramForDrawRectangle2D;
	if (ret.isNull()) {
		ret = new RenderProgram2D_Position;
		m_defaultRenderProgramForDrawRectangle2D = ret;
	}
	return ret;
}

void RenderEngine::drawTexture2D(const Ref<RenderProgram>& program)
{
	Ref<VertexBuffer> vb = _getDefaultVertexBufferForDrawTexture2D();
	draw(program, 4, vb, PrimitiveType::TriangleStrip);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program)
{
	if (program.isNotNull()) {
		if (texture.isNotNull()) {
			Matrix3 mt;
			_RenderEngine_makeTextureTransform2D(mt, rectSrc);
			program->setTransform(transform);
			program->setTextureTransform(mt);
			program->setTexture(texture);
			drawTexture2D(program);
		}
	}
}

void RenderEngine::RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program)
{
	drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), program);
}

void RenderEngine::RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
{
	if (texture.isNotNull()) {
		drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), program);
	}
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program)
{
	if (program.isNotNull()) {
		if (texture.isNotNull()) {
			Matrix3 matTransform;
			_RenderEngine_makeTransform2D(matTransform, rectDst);
			Matrix3 mt;
			_RenderEngine_makeTextureTransform2D(mt, rectSrc);
			program->setTransform(matTransform);
			program->setTextureTransform(mt);
			program->setTexture(texture);
			drawTexture2D(program);
		}
	}
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), program);
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
{
	drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), program);
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), program);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawTexture2D();
	if (p.isNotNull()) {
		p->setColor(Color::White);
		p->setAlpha(alpha);
		drawTexture2D(transform, texture, rectSrc, p);
	}
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha)
{
	drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), alpha);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha)
{
	drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), alpha);
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawTexture2D();
	if (p.isNotNull()) {
		p->setColor(Color::White);
		p->setAlpha(alpha);
		drawTexture2D(rectDst, texture, rectSrc, p);
	}
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), alpha);
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha)
{
	drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), alpha);
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real alpha)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), alpha);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color)
{
	drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), color);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color)
{
	drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), color);
}

void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawTexture2D();
	if (p.isNotNull()) {
		p->setColor(color.xyz());
		p->setAlpha(color.w);
		drawTexture2D(transform, texture, rectSrc, p);
	}
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
{
	Ref<RenderProgram2D> p = _getDefaultRenderProgramForDrawTexture2D();
	if (p.isNotNull()) {
		p->setColor(color.xyz());
		p->setAlpha(color.w);
		drawTexture2D(rectDst, texture, rectSrc, p);
	}
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), color);
}

void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color)
{
	drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), color);
}

void RenderEngine::drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Color4f& color)
{
	drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), color);
}

Ref<VertexBuffer> RenderEngine::_getDefaultVertexBufferForDrawTexture2D()
{
	static RenderProgram2D_PositionTexture::VertexData v[] = {
		{ { 0, 0 }, { 0, 0 } }
		, { { 1, 0 }, { 1, 0 } }
		, { { 0, 1 }, { 0, 1 } }
		, { { 1, 1 }, { 1, 1 } }
	};
	Ref<VertexBuffer> ret = m_defaultVertexBufferForDrawTexture2D;
	if (ret.isNull()) {
		ret = VertexBuffer::create(v, sizeof(v));
		m_defaultVertexBufferForDrawTexture2D = ret;
	}
	return ret;
}

Ref<RenderProgram2D> RenderEngine::_getDefaultRenderProgramForDrawTexture2D()
{
	Ref<RenderProgram2D> ret = m_defaultRenderProgramForDrawTexture2D;
	if (ret.isNull()) {
		ret = new RenderProgram2D_PositionTexture;
		m_defaultRenderProgramForDrawTexture2D = ret;
	}
	return ret;
}

void RenderEngine::drawLines(const Ref<RenderProgram>& program, Line3* lines, sl_uint32 n)
{
	if (program.isNotNull()) {
		Ref<VertexBuffer> vb = VertexBuffer::create(lines, sizeof(Line3)*n);
		draw(program, n * 2, vb, PrimitiveType::Lines);
	}
}

#define DEBUG_WIDTH 512
#define DEBUG_HEIGHT 20
void RenderEngine::drawDebugText()
{
	Ref<Texture> texture = m_textureDebug;
	if (texture.isNull()) {
		texture = Texture::create(UI::createBitmap(DEBUG_WIDTH, DEBUG_HEIGHT));
		if (texture.isNull()) {
			return;
		}
		m_textureDebug = texture;
	}
	Ref<Bitmap> bitmap = texture->getSource();
	if (bitmap.isNull()) {
		return;
	}
	
	Ref<Font> font = m_fontDebug;
	if (font.isNull()) {
		font = Font::create("Arial", 12);
		if (font.isNull()) {
			return;
		}
	}
	setDepthTest(sl_false);
	String text;
	text = "FPS:";
	sl_uint32 n = m_nRenderMillisecondsOnLastScene;
	if (n > 0) {
		text += String::fromDouble(1000.0 / n, 2);
	} else {
		text += "inf";
	}
	text += " Vertices: ";
	text += m_nCountDrawnElementsOnLastScene;
	text += " Primitives: ";
	text += m_nCountDrawnPrimitivesOnLastScene;
	Size size = Size::zero();
	bitmap->resetPixels(Color::Black);
	{
		Ref<Canvas> canvas = bitmap->getCanvas();
		if (canvas.isNotNull()) {
			size = canvas->getTextSize(font, text);
			size.x += 5;
			canvas->drawText(text, 0, 3, font, Color::Red);
		}
	}
	texture->update(0, 0, (sl_uint32)(size.x) + 1, DEBUG_HEIGHT);
	drawTexture2D(
		screenToViewport(0, 0, size.x, DEBUG_HEIGHT)
		, texture
		, Rectangle(0, 0, size.x / DEBUG_WIDTH, 1));
}

Point RenderEngine::screenToViewport(const Point& ptViewport)
{
	return Transform3::convertScreenToViewport(ptViewport, (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Point RenderEngine::screenToViewport(sl_real x, sl_real y)
{
	return Transform3::convertScreenToViewport(Point(x, y), (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Point RenderEngine::viewportToScreen(const Point& ptScreen)
{
	return Transform3::convertViewportToScreen(ptScreen, (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Point RenderEngine::viewportToScreen(sl_real x, sl_real y)
{
	return Transform3::convertViewportToScreen(Point(x, y), (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Rectangle RenderEngine::screenToViewport(const Rectangle& rc)
{
	return Transform3::convertScreenToViewport(rc, (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Rectangle RenderEngine::screenToViewport(sl_real x, sl_real y, sl_real width, sl_real height)
{
	return Transform3::convertScreenToViewport(Rectangle(x, y, x + width, y + height), (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Rectangle RenderEngine::viewportToScreen(const Rectangle& rc)
{
	return Transform3::convertViewportToScreen(rc, (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

Rectangle RenderEngine::viewportToScreen(sl_real x, sl_real y, sl_real width, sl_real height)
{
	return Transform3::convertViewportToScreen(Rectangle(x, y, x + width, y + height), (sl_real)m_viewportWidth, (sl_real)m_viewportHeight);
}

sl_uint32 RenderEngine::getViewportWidth()
{
	return m_viewportWidth;
}

sl_uint32 RenderEngine::getViewportHeight()
{
	return m_viewportHeight;
}

sl_uint32 RenderEngine::getCountOfDrawnElementsOnLastScene()
{
	return m_nCountDrawnElementsOnLastScene;
}

sl_uint32 RenderEngine::getCountOfDrawnPrimitivesOnLastScene()
{
	return m_nCountDrawnPrimitivesOnLastScene;
}

SLIB_RENDER_NAMESPACE_END

