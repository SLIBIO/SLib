/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/render/engine.h"

#include "slib/render/program.h"
#include "slib/graphics/canvas.h"
#include "slib/ui/core.h"
#include "slib/math/transform3d.h"

namespace slib
{

	Primitive::Primitive(): type(PrimitiveType::Triangle), countElements(0)
	{
	}
	
	Primitive::~Primitive()
	{
	}
	
	
	EnginePrimitive::EnginePrimitive(const Primitive& primitive): Primitive(primitive)
	{
	}
	
	EnginePrimitive::~EnginePrimitive()
	{
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
	
	RenderClearParam::~RenderClearParam()
	{
	}
	
	RenderBlendingParam::RenderBlendingParam()
	{
		operation = RenderBlendingOperation::Add;
		operationAlpha = RenderBlendingOperation::Add;
		blendDst = RenderBlendingFactor::OneMinusSrcAlpha;
		blendDstAlpha = RenderBlendingFactor::OneMinusSrcAlpha;
		blendSrc = RenderBlendingFactor::SrcAlpha;
		blendSrcAlpha = RenderBlendingFactor::One;
		blendConstant = Vector4::zero();
	}
	
	RenderBlendingParam::~RenderBlendingParam()
	{
	}
	
	
	IRenderCallback::IRenderCallback()
	{
	}

	IRenderCallback::~IRenderCallback()
	{
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
	
	RendererParam::~RendererParam()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(Renderer, Object)
	
	Renderer::Renderer()
	{
		setRenderingContinuously(sl_false);
	}
	
	Renderer::~Renderer()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(RenderEngine, Object)
	
	RenderEngine::RenderEngine()
	{
		static sl_int64 _id = 0;
		m_uniqueId = Base::interlockedIncrement64(&_id);
		
		m_viewportWidth = 0;
		m_viewportHeight = 0;
		
		m_nCountDrawnElementsOnLastScene = 0;
		m_nCountDrawnPrimitivesOnLastScene = 0;
		m_timeLastDebugText.setZero();
	}
	
	RenderEngine::~RenderEngine()
	{
	}
	
	sl_uint64 RenderEngine::getUniqueId()
	{
		return m_uniqueId;
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
	
	sl_bool RenderEngine::beginScene()
	{
		m_nCountDrawnElementsOnLastScene = 0;
		m_nCountDrawnPrimitivesOnLastScene = 0;
		return _beginScene();
	}
	
	void RenderEngine::endScene()
	{
		_endScene();
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
	
	void RenderEngine::setDepthFunction(RenderFunctionOperation op)
	{
		_setDepthFunction(op);
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
	
	sl_bool RenderEngine::beginProgram(const Ref<RenderProgram>& program, RenderProgramState** ppState)
	{
		if (program.isNotNull()) {
			Ref<RenderProgramInstance> instance = linkProgram(program);
			if (instance.isNotNull()) {
				return _beginProgram(program.get(), instance.get(), ppState);
			}
		}
		return sl_false;
	}
	
	void RenderEngine::endProgram()
	{
		_endProgram();
	}
	
	void RenderEngine::resetCurrentBuffers()
	{
		_resetCurrentBuffers();
	}
	
	void RenderEngine::drawPrimitive(Primitive* primitive)
	{
		if (primitive->countElements > 0 && primitive->vertexBuffer.isNotNull()) {
			EnginePrimitive ep(*primitive);
			ep.vertexBufferInstance = linkVertexBuffer(primitive->vertexBuffer);
			if (ep.vertexBufferInstance.isNull()) {
				return;
			}
			if (primitive->indexBuffer.isNotNull()) {
				ep.indexBufferInstance = linkIndexBuffer(primitive->indexBuffer);
				if (ep.indexBufferInstance.isNull()) {
					return;
				}
			}
			_drawPrimitive(&ep);
			m_nCountDrawnElementsOnLastScene += primitive->countElements;
			m_nCountDrawnPrimitivesOnLastScene++;
		}
	}
	
	void RenderEngine::drawPrimitives(Primitive* primitives, sl_uint32 count)
	{
		for (sl_uint32 i = 0; i < count; i++) {
			drawPrimitive(primitives + i);
		}
	}
	
	void RenderEngine::drawPrimitive(sl_uint32 countElements, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib, PrimitiveType type)
	{
		Primitive p;
		p.type = type;
		p.countElements = countElements;
		p.vertexBuffer = vb;
		p.indexBuffer = ib;
		drawPrimitive(&p);
	}
	
	void RenderEngine::drawPrimitive(sl_uint32 countElements, const Ref<VertexBuffer>& vb, PrimitiveType type)
	{
		Primitive p;
		p.type = type;
		p.countElements = countElements;
		p.vertexBuffer = vb;
		drawPrimitive(&p);
	}
	
	void RenderEngine::applyTexture(const Ref<Texture>& _texture, sl_reg sampler)
	{
		Texture* texture = _texture.get();
		if (texture) {
			if (IsInstanceOf<EngineTexture>(texture)) {
				_applyTexture(texture, sl_null, sampler);
			} else {
				Ref<TextureInstance> instance = linkTexture(_texture);
				if (instance.isNotNull()) {
					_applyTexture(texture, instance.get(), sampler);
				} else {
					_applyTexture(sl_null, sl_null, sampler);
				}
			}
		} else {
			_applyTexture(sl_null, sl_null, sampler);
		}
	}
	
	Ref<TextureInstance> RenderEngine::linkTexture(const Ref<Texture>& texture)
	{
		if (texture.isNotNull()) {
			Ref<TextureInstance> instance = texture->getInstance(this);
			if (instance.isNotNull()) {
				return instance;
			}
			instance = _createTextureInstance(texture.get());
			if (instance.isNotNull()) {
				return instance;
			}
		}
		return sl_null;
	}
	
	Ref<VertexBufferInstance> RenderEngine::linkVertexBuffer(const Ref<VertexBuffer>& vb)
	{
		if (vb.isNotNull()) {
			Ref<VertexBufferInstance> instance = vb->getInstance(this);
			if (instance.isNotNull()) {
				return instance;
			}
			instance = _createVertexBufferInstance(vb.get());
			if (instance.isNotNull()) {
				return instance;
			}
		}
		return sl_null;
	}
	
	Ref<IndexBufferInstance> RenderEngine::linkIndexBuffer(const Ref<IndexBuffer>& ib)
	{
		if (ib.isNotNull()) {
			Ref<IndexBufferInstance> instance = ib->getInstance(this);
			if (instance.isNotNull()) {
				return instance;
			}
			instance = _createIndexBufferInstance(ib.get());
			if (instance.isNotNull()) {
				return instance;
			}
		}
		return sl_null;
	}
	
	Ref<RenderProgramInstance> RenderEngine::linkProgram(const Ref<RenderProgram>& program)
	{
		if (program.isNotNull()) {
			Ref<RenderProgramInstance> instance = program->getInstance(this);
			if (instance.isNotNull()) {
				return instance;
			}
			instance = _createProgramInstance(program.get());
			if (instance.isNotNull()) {
				return instance;
			}
		}
		return sl_null;
	}
	
	void RenderEngine::setLineWidth(sl_real width)
	{
		_setLineWidth(width);
	}
	
	void RenderEngine::drawRectangle2D()
	{
		Ref<VertexBuffer> vb = getDefaultVertexBufferForDrawRectangle2D();
		drawPrimitive(4, vb, PrimitiveType::TriangleStrip);
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
	
	void RenderEngine::drawRectangle2D(const Ref<RenderProgram2D_Position>& program, const Matrix3& transform, const Color4f& color)
	{
		RenderProgramScope<RenderProgramState2D_Position> scope;
		if (scope.begin(this, program)) {
			scope->setTransform(transform);
			scope->setColor(color);
			drawRectangle2D();
		}
	}
	
	void RenderEngine::drawRectangle2D(const Matrix3& transform, const Color4f& color)
	{
		drawRectangle2D(getDefaultRenderProgramForDrawRectangle2D(), transform, color);
	}
	
	void RenderEngine::drawRectangle2D(const Ref<RenderProgram2D_Position>& program, const Rectangle& rectDst, const Color4f& color)
	{
		Matrix3 transform;
		_RenderEngine_makeTransform2D(transform, rectDst);
		drawRectangle2D(program, transform, color);
	}
	
	void RenderEngine::drawRectangle2D(const Rectangle& rectDst, const Color4f& color)
	{
		Matrix3 transform;
		_RenderEngine_makeTransform2D(transform, rectDst);
		drawRectangle2D(getDefaultRenderProgramForDrawRectangle2D(), transform, color);
	}
	
	void RenderEngine::drawTexture2D()
	{
		Ref<VertexBuffer> vb = getDefaultVertexBufferForDrawTexture2D();
		drawPrimitive(4, vb, PrimitiveType::TriangleStrip);
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
	{
		if (texture.isNotNull() && program.isNotNull()) {
			RenderProgramScope<RenderProgramState2D_PositionTexture> scope;
			if (scope.begin(this, program)) {
				scope->setTransform(transform);
				scope->setTexture(texture);
				Matrix3 textureTransform;
				_RenderEngine_makeTextureTransform2D(textureTransform, rectSrc);
				scope->setTextureTransform(textureTransform);
				scope->setColor(color);
				drawTexture2D();
			}
		}
	}
	
	void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
	{
		drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, rectSrc, color);
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
	{
		drawTexture2D(program, transform, texture, rectSrc, Vector4(1, 1, 1, alpha));
	}
	
	void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
	{
		drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, rectSrc, Vector4(1, 1, 1, alpha));
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(program, transform, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha)
	{
		drawTexture2D(program, transform, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	void RenderEngine::drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha)
	{
		drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
	{
		if (texture.isNotNull()) {
			Matrix3 transform;
			_RenderEngine_makeTransform2D(transform, rectDst);
			drawTexture2D(program, transform, texture, rectSrc, color);
		}
	}
	
	void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color)
	{
		if (texture.isNotNull()) {
			Matrix3 transform;
			_RenderEngine_makeTransform2D(transform, rectDst);
			drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, rectSrc, color);
		}
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
	{
		if (texture.isNotNull()) {
			Matrix3 transform;
			_RenderEngine_makeTransform2D(transform, rectDst);
			drawTexture2D(program, transform, texture, rectSrc, Vector4(1, 1, 1, alpha));
		}
	}
	
	void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
	{
		if (texture.isNotNull()) {
			Matrix3 transform;
			_RenderEngine_makeTransform2D(transform, rectDst);
			drawTexture2D(getDefaultRenderProgramForDrawTexture2D(), transform, texture, rectSrc, Vector4(1, 1, 1, alpha));
		}
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(program, rectDst, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	void RenderEngine::drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha)
	{
		drawTexture2D(program, rectDst, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	void RenderEngine::drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha)
	{
		drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	Ref<VertexBuffer> RenderEngine::getDefaultVertexBufferForDrawRectangle2D()
	{
		static RenderVertex2D_Position v[] = {
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
	
	Ref<RenderProgram2D_Position> RenderEngine::getDefaultRenderProgramForDrawRectangle2D()
	{
		Ref<RenderProgram2D_Position> ret = m_defaultRenderProgramForDrawRectangle2D;
		if (ret.isNull()) {
			ret = new RenderProgram2D_Position;
			m_defaultRenderProgramForDrawRectangle2D = ret;
		}
		return ret;
	}
	
	Ref<VertexBuffer> RenderEngine::getDefaultVertexBufferForDrawTexture2D()
	{
		static RenderVertex2D_PositionTexture v[] = {
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
	
	Ref<RenderProgram2D_PositionTexture> RenderEngine::getDefaultRenderProgramForDrawTexture2D()
	{
		Ref<RenderProgram2D_PositionTexture> ret = m_defaultRenderProgramForDrawTexture2D;
		if (ret.isNull()) {
			ret = new RenderProgram2D_PositionTexture;
			m_defaultRenderProgramForDrawTexture2D = ret;
		}
		return ret;
	}
	
	void RenderEngine::drawLines(LineSegment* lines, sl_uint32 n, const Color4f& color)
	{
		if (n) {
			Ref<VertexBuffer> vb = VertexBuffer::create(lines, sizeof(LineSegment)*n);
			if (vb.isNotNull()) {
				RenderProgramScope<RenderProgramState2D_Position> scope;
				if (scope.begin(this, getDefaultRenderProgramForDrawLine2D())) {
					scope->setTransform(Matrix3::identity());
					scope->setColor(color);
					drawPrimitive(n * 2, vb, PrimitiveType::Line);
				}
			}
		}
	}
	
	Ref<RenderProgram2D_Position> RenderEngine::getDefaultRenderProgramForDrawLine2D()
	{
		Ref<RenderProgram2D_Position> ret = m_defaultRenderProgramForDrawLine2D;
		if (ret.isNull()) {
			ret = new RenderProgram2D_Position;
			m_defaultRenderProgramForDrawLine2D = ret;
		}
		return ret;
	}
	
	void RenderEngine::drawLines(Line3* lines, sl_uint32 n, const Color4f& color)
	{
		if (n) {
			Ref<VertexBuffer> vb = VertexBuffer::create(lines, sizeof(Line3)*n);
			if (vb.isNotNull()) {
				RenderProgramScope<RenderProgramState3D_Position> scope;
				if (scope.begin(this, getDefaultRenderProgramForDrawLine3D())) {
					scope->setTransform(Matrix4::identity());
					scope->setColor(color);
					drawPrimitive(n * 2, vb, PrimitiveType::Line);
				}
			}
		}
	}
	
	Ref<RenderProgram3D_Position> RenderEngine::getDefaultRenderProgramForDrawLine3D()
	{
		Ref<RenderProgram3D_Position> ret = m_defaultRenderProgramForDrawLine3D;
		if (ret.isNull()) {
			ret = new RenderProgram3D_Position;
			m_defaultRenderProgramForDrawLine3D = ret;
		}
		return ret;
	}
	
#define DEBUG_WIDTH 512
#define DEBUG_HEIGHT 30
	
	void RenderEngine::drawDebugText()
	{
		Time now = Time::now();
		if (m_timeLastDebugText.isZero()) {
			m_timeLastDebugText = now;
			return;
		}
		
		Ref<Texture> texture = m_textureDebug;
		if (texture.isNull()) {
			texture = Texture::create(Bitmap::create(DEBUG_WIDTH, DEBUG_HEIGHT));
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
			font = Font::create("Arial", 20);
			if (font.isNull()) {
				return;
			}
		}
		setDepthTest(sl_false);
		String text;
		text = "FPS:";
		double duration = (now - m_timeLastDebugText).getMillisecondsCountf();
		if (duration > 1) {
			text += String::fromDouble(1000.0 / duration, 1, sl_true);
		} else {
			text += "Inf";
		}
		m_timeLastDebugText = now;
		
		text += " Vertices: ";
		text += m_nCountDrawnElementsOnLastScene;
		text += " Primitives: ";
		text += m_nCountDrawnPrimitivesOnLastScene;
		Size size = Size::zero();
		bitmap->resetPixels(Color(0, 0, 0, 150));
		{
			Ref<Canvas> canvas = bitmap->getCanvas();
			if (canvas.isNotNull()) {
				size = canvas->measureText(font, text);
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
	
}
