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

#ifndef CHECKHEADER_SLIB_RENDER_ENGINE
#define CHECKHEADER_SLIB_RENDER_ENGINE

#include "definition.h"

#include "base.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "program.h"

#include "../core/time.h"
#include "../core/function.h"
#include "../graphics/color.h"
#include "../graphics/font.h"
#include "../math/line_segment.h"
#include "../math/line3.h"

namespace slib
{

	enum class PrimitiveType
	{
		Triangle = 0,
		TriangleStrip = 1,
		TriangleFan = 2,
		Line = 3,
		LineLoop = 4,
		LineStrip = 5,
		Point = 6
	};
	
	class SLIB_EXPORT Primitive
	{
	public:
		PrimitiveType type;
		sl_uint32 countElements;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;
		
	public:
		Primitive();
		
		~Primitive();
		
	};
	
	class SLIB_EXPORT EnginePrimitive : public Primitive
	{
	public:
		EnginePrimitive(const Primitive& primitive);
		
		~EnginePrimitive();
		
	public:
		Ref<VertexBufferInstance> vertexBufferInstance;
		Ref<IndexBufferInstance> indexBufferInstance;
		
	};
	
	class SLIB_EXPORT RenderClearParam
	{
	public:
		sl_bool flagColor;
		Color color;
		sl_bool flagDepth;
		float depth;
		sl_bool flagStencil;
		sl_uint32 stencil;
		
	public:
		RenderClearParam();
		
		~RenderClearParam();
		
	};
	
	enum class RenderBlendingOperation
	{
		Add = 0,
		Subtract = 1,
		ReverseSubtract = 2
	};
	
	enum class RenderBlendingFactor
	{
		One = 0,
		Zero = 1,
		SrcAlpha = 2,
		OneMinusSrcAlpha = 3,
		DstAlpha = 4,
		OneMinusDstAlpha = 5,
		SrcColor = 6,
		OneMinusSrcColor = 7,
		DstColor = 8,
		OneMinusDstColor = 9,
		SrcAlphaSaturate = 10, // f = min(As, 1 - Ad)
		Constant = 11,
		OneMinusConstant = 12,
		ConstantAlpha = 13,
		OneMinusConstantAlpha = 14
	};
	
	class SLIB_EXPORT RenderBlendingParam
	{
	public:
		RenderBlendingOperation operation;
		RenderBlendingOperation operationAlpha;
		RenderBlendingFactor blendSrc;
		RenderBlendingFactor blendSrcAlpha;
		RenderBlendingFactor blendDst;
		RenderBlendingFactor blendDstAlpha;
		Vector4 blendConstant;
		
	public:
		RenderBlendingParam();
		
		~RenderBlendingParam();
		
	};
	
	enum class RenderFunctionOperation
	{
		Never = 0,
		Always = 1,
		Equal = 2,
		NotEqual = 3,
		Less = 4,
		LessEqual = 5,
		Greater = 6,
		GreaterEqual = 7,
	};
	
	class SLIB_EXPORT RendererParam
	{
	public:
		sl_int32 nRedBits;
		sl_int32 nGreenBits;
		sl_int32 nBlueBits;
		sl_int32 nAlphaBits;
		sl_int32 nAccumBits;
		sl_int32 nDepthBits;
		sl_int32 nStencilBits;
		sl_bool flagMultisample;
		
		Function<void(RenderEngine*)> onFrame;
		
	public:
		RendererParam();
		
		~RendererParam();
		
	};
	
	class RenderEngine;
	
	class SLIB_EXPORT Renderer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Renderer();
		
		~Renderer();
		
	public:
		virtual void release() = 0;
		
		virtual void requestRender() = 0;
		
	public:
		SLIB_BOOLEAN_PROPERTY(RenderingContinuously)
		
	protected:
		void initWithParam(const RendererParam& param);
		
		void dispatchFrame(RenderEngine* engine);
		
	protected:
		Function<void(RenderEngine*)> m_onFrame;
		
	};
	
	
	enum class RenderEngineType
	{
		OpenGL = 0x01010001,
		OpenGL_ES = 0x01020001,
		D3D9 = 0x02010901,
		D3D11 = 0x02010B01
	};
	
	class SLIB_EXPORT RenderEngine : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		RenderEngine();
		
		~RenderEngine();
		
	public:
		sl_uint64 getUniqueId();
		
		virtual RenderEngineType getEngineType() = 0;
		
		sl_bool isOpenGL();
		
		sl_bool isOpenGL_ES();
		
		sl_bool isD3D();
		
		sl_bool beginScene();
		
		void endScene();
		
		void setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
		
		void clear(const RenderClearParam& param);
		
		void clearColor(const Color& color);
		
		void clearColorDepth(const Color& color, float depth = 1.0f);
		
		void clearDepth(float depth = 1.0f);
		
		void setDepthTest(sl_bool flagEnableDepthTest);
		
		void setDepthWriteEnabled(sl_bool flagEnableDepthWrite);
		
		void setDepthFunction(RenderFunctionOperation op);
		
		void setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW = sl_true);
		
		void setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param);
		
		void setBlending(sl_bool flagEnableBlending);
		
		sl_bool beginProgram(const Ref<RenderProgram>& program, RenderProgramState** ppState = sl_null /* RenderProgramState** */);
		
		void endProgram();
		
		void resetCurrentBuffers();
		
		void drawPrimitive(Primitive* primitive);
		
		void drawPrimitives(Primitive* primitives, sl_uint32 count);
		
		void drawPrimitive(sl_uint32 countElements, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib, PrimitiveType type = PrimitiveType::Triangle);
		
		void drawPrimitive(sl_uint32 countElements, const Ref<VertexBuffer>& vb, PrimitiveType type = PrimitiveType::Triangle);
		
		void applyTexture(const Ref<Texture>& texture, sl_reg sampler);
		
		Ref<TextureInstance> linkTexture(const Ref<Texture>& texture);
		
		Ref<VertexBufferInstance> linkVertexBuffer(const Ref<VertexBuffer>& vb);
		
		Ref<IndexBufferInstance> linkIndexBuffer(const Ref<IndexBuffer>& ib);
		
		Ref<RenderProgramInstance> linkProgram(const Ref<RenderProgram>& program);
		
		void setLineWidth(sl_real width);
		
		// Position(0, 0, 1, 1)
		void drawRectangle2D();
		
		void drawRectangle2D(const Ref<RenderProgram2D_Position>& program, const Matrix3& transform, const Color4f& color);
		
		void drawRectangle2D(const Matrix3& transform, const Color4f& color);
		
		void drawRectangle2D(const Ref<RenderProgram2D_Position>& program, const Rectangle& rectDst, const Color4f& color);
		
		void drawRectangle2D(const Rectangle& rectDst, const Color4f& color);
		
		// Position(0, 0, 1, 1) TexCoord(0, 0, 1, 1)
		void drawTexture2D();
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
		
		void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color);
		
		void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha = 1);
		
		void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha = 1);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
		
		void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color);
		
		void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color);
		
		void drawTexture2D(const Ref<RenderProgram2D_PositionTexture>& program, const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha = 1);
		
		void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha = 1);
		
		void drawLines(const Ref<RenderProgram2D_Position>& program, LineSegment* lines, sl_uint32 n, const Color4f& color);
		
		void drawLines(LineSegment* lines, sl_uint32 n, const Color4f& color);
		
		void drawLines(const Ref<RenderProgram3D_Position>& program, Line3* lines, sl_uint32 n, const Color4f& color);
		
		void drawLines(Line3* lines, sl_uint32 n, const Color4f& color);
		
		void drawDebugText();
		
		Point screenToViewport(const Point& ptViewport);
		
		Point screenToViewport(sl_real x, sl_real y);
		
		Point viewportToScreen(const Point& ptScreen);
		
		Point viewportToScreen(sl_real x, sl_real y);
		
		Rectangle screenToViewport(const Rectangle& rc);
		
		Rectangle screenToViewport(sl_real x, sl_real y, sl_real width, sl_real height);
		
		Rectangle viewportToScreen(const Rectangle& rc);
		
		Rectangle viewportToScreen(sl_real x, sl_real y, sl_real width, sl_real height);
		
		sl_uint32 getViewportWidth();
		
		sl_uint32 getViewportHeight();
		
		sl_uint32 getCountOfDrawnElementsOnLastScene();
		
		sl_uint32 getCountOfDrawnPrimitivesOnLastScene();
		
		Ref<VertexBuffer> getDefaultVertexBufferForDrawRectangle2D();
		
		Ref<RenderProgram2D_Position> getDefaultRenderProgramForDrawRectangle2D();
		
		Ref<VertexBuffer> getDefaultVertexBufferForDrawTexture2D();
		
		Ref<RenderProgram2D_PositionTexture> getDefaultRenderProgramForDrawTexture2D();
		
		Ref<RenderProgram2D_Position> getDefaultRenderProgramForDrawLine2D();
		
		Ref<RenderProgram3D_Position> getDefaultRenderProgramForDrawLine3D();
		
	protected:
		virtual Ref<RenderProgramInstance> _createProgramInstance(RenderProgram* program) = 0;
		
		virtual Ref<VertexBufferInstance> _createVertexBufferInstance(VertexBuffer* buffer) = 0;
		
		virtual Ref<IndexBufferInstance> _createIndexBufferInstance(IndexBuffer* buffer) = 0;
		
		virtual Ref<TextureInstance> _createTextureInstance(Texture* texture) = 0;
		
		virtual sl_bool _beginScene() = 0;
		
		virtual void _endScene() = 0;
		
		virtual void _setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) = 0;
		
		virtual void _clear(const RenderClearParam& param) = 0;
		
		virtual void _setDepthTest(sl_bool flagEnableDepthTest) = 0;
		
		virtual void _setDepthWriteEnabled(sl_bool flagEnableDepthWrite) = 0;
		
		virtual void _setDepthFunction(RenderFunctionOperation op) = 0;
		
		virtual void _setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW) = 0;
		
		virtual void _setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param) = 0;
		
		virtual sl_bool _beginProgram(RenderProgram* program, RenderProgramInstance* instance, RenderProgramState** ppState) = 0;
		
		virtual void _endProgram() = 0;
		
		virtual void _resetCurrentBuffers() = 0;
		
		virtual void _drawPrimitive(EnginePrimitive* primitive) = 0;
		
		virtual void _applyTexture(Texture* texture, TextureInstance* instance, sl_reg sampler) = 0;
		
		virtual void _setLineWidth(sl_real width) = 0;
		
	protected:
		sl_uint64 m_uniqueId;
		
		sl_uint32 m_viewportWidth;
		sl_uint32 m_viewportHeight;
		
		// debug
		sl_uint32 m_nCountDrawnElementsOnLastScene;
		sl_uint32 m_nCountDrawnPrimitivesOnLastScene;
		Time m_timeLastDebugText;
		Ref<Texture> m_textureDebug;
		Ref<Font> m_fontDebug;
		
		AtomicRef<VertexBuffer> m_defaultVertexBufferForDrawRectangle2D;
		AtomicRef<RenderProgram2D_Position> m_defaultRenderProgramForDrawRectangle2D;
		
		AtomicRef<VertexBuffer> m_defaultVertexBufferForDrawTexture2D;
		AtomicRef<RenderProgram2D_PositionTexture> m_defaultRenderProgramForDrawTexture2D;
		
		AtomicRef<RenderProgram2D_Position> m_defaultRenderProgramForDrawLine2D;
		AtomicRef<RenderProgram3D_Position> m_defaultRenderProgramForDrawLine3D;
		
	};
	
	template <class StateType>
	class SLIB_EXPORT RenderProgramScope
	{
	public:
		SLIB_INLINE RenderProgramScope():
	 m_engine(sl_null), m_program(sl_null), m_state(sl_null)
		{};
		
		SLIB_INLINE ~RenderProgramScope()
		{
			end();
		}
		
	public:
		SLIB_INLINE sl_bool begin(RenderEngine* engine, const Ref<RenderProgram>& program)
		{
			if (program.isNotNull()) {
				if (engine->beginProgram(program, reinterpret_cast<RenderProgramState**>(&m_state))) {
					m_engine = engine;
					m_program = program;
					return sl_true;
				}
			}
			return sl_false;
		}
		
		SLIB_INLINE sl_bool begin(const Ref<RenderEngine>& engine, const Ref<RenderProgram>& program)
		{
			return begin(engine.get(), program);
		}
		
		SLIB_INLINE void end()
		{
			if (m_engine) {
				m_engine->endProgram();
				m_engine = sl_null;
			}
		}
		
		SLIB_INLINE StateType* operator->()
		{
			return m_state;
		}
		
		SLIB_INLINE StateType* getState()
		{
			return m_state;
		}
		
	public:
		RenderEngine* m_engine;
		Ref<RenderProgram> m_program;
		StateType* m_state;
		
	};

}

#endif
