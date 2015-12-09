#ifndef CHECKHEADER_SLIB_RENDER_ENGINE
#define CHECKHEADER_SLIB_RENDER_ENGINE

#include "definition.h"

#include "base.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include "program.h"

#include "../core/time.h"
#include "../core/pointer.h"
#include "../graphics/color.h"
#include "../graphics/font.h"
#include "../math/line3.h"

SLIB_RENDER_NAMESPACE_BEGIN

struct SLIB_EXPORT Primitive
{
	enum Type
	{
		typeLines = 0,
		typeLineStrip = 1,
		typeTriangles = 2,
		typeTriangleStrip = 3,
		typeTriangleFan = 4,
		typePoints = 5
	};
	Type type;
	sl_uint32 countElements;
	Ref<VertexBuffer> vertexBuffer;
	Ref<IndexBuffer> indexBuffer;

	SLIB_INLINE Primitive()
	{
		type = typeTriangles;
		countElements = 0;
	}
};

class SLIB_EXPORT RenderClearParam {
public:
	sl_bool flagColor;
	Color color;
	sl_bool flagDepth;
	float depth;
	sl_bool flagStencil;
	sl_uint32 stencil;

	SLIB_INLINE RenderClearParam()
	{
		flagColor = sl_true;
		color = Color::blue();
		flagDepth = sl_true;
		depth = 1.0f;
		flagStencil = sl_false;
		stencil = 0;
	}
};

class SLIB_EXPORT RenderBlendingParam
{
public:
	enum Operation {
		opAdd = 0,
		opSubtract = 1,
		opReverseSubtract = 2
	};
	Operation operation;
	Operation operationAlpha;
	enum Factor {
		factorOne = 0,
		factorZero = 1,
		factorSrcAlpha = 2,
		factorOneMinusSrcAlpha = 3,
		factorDstAlpha = 4,
		factorOneMinusDstAlpha = 5,
		factorSrcColor = 6,
		factorOneMinusSrcColor = 7,
		factorDstColor = 8,
		factorOneMinusDstColor = 9,
		factorSrcAlphaSaturate = 10, // f = min(As, 1 - Ad)
		factorConstant = 11,
		factorOneMinusConstant = 12,
		factorConstantAlpha = 13,
		factorOneMinusConstantAlpha = 14
	};
	Factor blendSrc;
	Factor blendSrcAlpha;
	Factor blendDst;
	Factor blendDstAlpha;
	Vector4 blendConstant;

	SLIB_INLINE RenderBlendingParam()
	{
		operation = opAdd;
		operationAlpha = opAdd;
		blendDst = factorOneMinusSrcAlpha;
		blendDstAlpha = factorOneMinusSrcAlpha;
		blendSrc = factorSrcAlpha;
		blendSrcAlpha = factorSrcAlpha;
	}
};

class SLIB_EXPORT RendererParam {
public:
	sl_int32 nRedBits;
	sl_int32 nGreenBits;
	sl_int32 nBlueBits;
	sl_int32 nAlphaBits;
	sl_int32 nAccumBits;
	sl_int32 nDepthBits;
	sl_int32 nStencilBits;
	sl_bool flagMultisample;

	RendererParam()
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
};

class SLIB_EXPORT IRenderCallback
{
public:
	virtual void onFrame(RenderEngine* engine) = 0;
};

class SLIB_EXPORT Renderer : public Object
{
	SLIB_DECLARE_OBJECT(Renderer, Object)
protected:
	SLIB_INLINE Renderer()
	{
		setRenderingContinuously(sl_false);
	}

public:
	virtual void release() = 0;
	virtual void requestRender() = 0;

	SLIB_PROPERTY_INLINE(Ptr<IRenderCallback>, Callback)
	SLIB_BOOLEAN_PROPERTY_INLINE(RenderingContinuously)
};

class SLIB_EXPORT RenderEngine : public Object
{
	SLIB_DECLARE_OBJECT(RenderEngine, Object)
protected:
	RenderEngine();
public:
	~RenderEngine();
	
public:
	enum EngineType
	{
		OPENGL = 0x01010001,
		OPENGL_ES = 0x01020001,
		D3D9 = 0x02010901,
		D3D11 = 0x02010B01
	};

public:
	virtual EngineType getEngineType() = 0;
	SLIB_INLINE sl_bool isOpenGL()
	{
		EngineType type = getEngineType();
		return type == OPENGL;
	}
	SLIB_INLINE sl_bool isOpenGL_ES()
	{
		EngineType type = getEngineType();
		return type == OPENGL_ES;
	}
	SLIB_INLINE sl_bool isD3D()
	{
		EngineType type = getEngineType();
		return type == D3D9 || type == D3D11;
	}

protected:
	virtual void _release() = 0;
	virtual sl_bool _isValid() = 0;

	virtual sl_bool _beginScene() = 0;
	virtual void _endScene() = 0;
	
	virtual void _setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) = 0;
	virtual void _clear(const RenderClearParam& param) = 0;

	virtual void _setDepthTest(sl_bool flagEnableDepthTest) = 0;
	virtual void _setDepthWriteEnabled(sl_bool flagEnableDepthWrite) = 0;
	virtual void _setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW) = 0;
	virtual void _setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param) = 0;

	virtual sl_bool _beginProgram(RenderProgram* program) = 0;
	virtual void _endProgram() = 0;
	virtual void _drawPrimitive(Primitive* primitive) = 0;

	virtual void _applyTexture(const void* sampler, Texture* texture) = 0;
	virtual sl_bool _linkTexture(Texture* texture) = 0;
	virtual sl_bool _linkVertexBuffer(VertexBuffer* buffer) = 0;
	virtual sl_bool _linkIndexBuffer(IndexBuffer* buffer) = 0;

	virtual void _setLineWidth(sl_real width) = 0;

public:
	virtual void _onFreeInstance(RenderBaseObjectInstance* instace) = 0;

	virtual void _onUpdateVertexBuffer(VertexBuffer* vertexBuffer, sl_size offset, sl_size size) = 0;
	virtual void _onUpdateIndexBuffer(IndexBuffer* vertexBuffer, sl_size offset, sl_size size) = 0;
	virtual void _onUpdateTexture(Texture* texture, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) = 0;

public:
	SLIB_INLINE void release()
	{
		_release();
	}

	SLIB_INLINE sl_bool isValid()
	{
		return _isValid();
	}

	sl_bool beginScene();
	
	void endScene();

	SLIB_INLINE void setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		_setViewport(x, y, width, height);
	}
	
	SLIB_INLINE void clear(const RenderClearParam& param)
	{
		_clear(param);
	}
	
	SLIB_INLINE void clearColor(const Color& color)
	{
		RenderClearParam param;
		param.flagColor = sl_true;
		param.color = color;
		param.flagDepth = sl_false;
		_clear(param);
	}
	
	SLIB_INLINE void clearColorDepth(const Color& color, float depth = 1.0f)
	{
		RenderClearParam param;
		param.flagColor = sl_true;
		param.color = color;
		param.flagDepth = sl_true;
		param.depth = depth;
		_clear(param);
	}
	
	SLIB_INLINE void clearDepth(float depth = 1.0f)
	{
		RenderClearParam param;
		param.flagColor = sl_false;
		param.flagDepth = sl_true;
		param.depth = depth;
		_clear(param);
	}

	SLIB_INLINE void setDepthTest(sl_bool flagEnableDepthTest)
	{
		_setDepthTest(flagEnableDepthTest);
	}

	SLIB_INLINE void setDepthWriteEnabled(sl_bool flagEnableDepthWrite)
	{
		_setDepthWriteEnabled(flagEnableDepthWrite);
	}

	SLIB_INLINE void setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW = sl_true)
	{
		_setCullFace(flagEnableCull, flagCullCCW);
	}

	SLIB_INLINE void setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param)
	{
		_setBlending(flagEnableBlending, param);
	}
	
	SLIB_INLINE void setBlending(sl_bool flagEnableBlending)
	{
		RenderBlendingParam param;
		_setBlending(flagEnableBlending, param);
	}

	SLIB_INLINE sl_bool beginProgram(Ref<RenderProgram> program)
	{
		return _beginProgram(program.get());
	}
	
	SLIB_INLINE void endProgram()
	{
		_endProgram();
	}
	
	void drawPrimitive(Primitive* primitive);

	SLIB_INLINE void applyTexture(const void* sampler, Ref<Texture> texture)
	{
		_applyTexture(sampler, texture.get());
	}
	
	SLIB_INLINE void applyTexture(sl_reg sampler, Ref<Texture> texture)
	{
		_applyTexture((void*)sampler, texture.get());
	}

	SLIB_INLINE sl_bool linkTexture(Ref<Texture> texture)
	{
		return _linkTexture(texture.get());
	}

	SLIB_INLINE sl_bool linkVertexBuffer(Ref<VertexBuffer> vb)
	{
		return _linkVertexBuffer(vb.get());
	}
	
	SLIB_INLINE sl_bool linkIndexBuffer(Ref<IndexBuffer> ib)
	{
		return _linkIndexBuffer(ib.get());
	}

	SLIB_INLINE void setLineWidth(sl_real width)
	{
		_setLineWidth(width);
	}

public:
	void draw(const Ref<RenderProgram>& program, Primitive* primitives, sl_uint32 count = 1);

	SLIB_INLINE void draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib, Primitive::Type type = Primitive::typeTriangles)
	{
		Primitive p;
		p.type = type;
		p.countElements = countElements;
		p.vertexBuffer = vb;
		p.indexBuffer = ib;
		draw(program, &p);
	}

	SLIB_INLINE void draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, Primitive::Type type = Primitive::typeTriangles)
	{
		Primitive p;
		p.type = type;
		p.countElements = countElements;
		p.vertexBuffer = vb;
		draw(program, &p);
	}

	// Position(0, 0, 1, 1)
	void drawRectangle2D(const Ref<RenderProgram>& program);
	
	void drawRectangle2D(const Matrix3& transform, const Ref<RenderProgram2D>& program);

	void drawRectangle2D(const Rectangle& rectDst, const Ref<RenderProgram2D>& program);

	SLIB_INLINE void drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<RenderProgram2D>& program)
	{
		drawRectangle2D(Rectangle(dx, dy, dx + dw, dy + dh), program);
	}

	void drawRectangle2D(const Matrix3& transform, const Color4f& color);
	
	void drawRectangle2D(const Rectangle& rectDst, const Color4f& color);
	
	SLIB_INLINE void drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Color4f& color)
	{
		drawRectangle2D(Rectangle(dx, dy, dx + dw, dy + dh), color);
	}

	// Position(0, 0, 1, 1) TexCoord(0, 0, 1, 1)
	void drawTexture2D(const Ref<RenderProgram>& program);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program);

	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program)
	{
		drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), program);
	}

	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
	{
		if (texture.isNotNull()) {
			drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), program);
		}
	}

	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program);

	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), program);
	}

	SLIB_INLINE void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
	{
		drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), program);
	}

	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), program);
	}

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
	
	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha = 1)
	{
		drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), alpha);
	}
	
	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha = 1)
	{
		drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
	
	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha = 1)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), alpha);
	}
	
	SLIB_INLINE void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha = 1)
	{
		drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), alpha);
	}
	
	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real alpha = 1)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), alpha);
	}

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
	
	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color)
	{
		drawTexture2D(transform, texture, Rectangle(sx, sy, sx + sw, sy + sh), color);
	}
	
	SLIB_INLINE void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(transform, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
	
	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(sx, sy, sx + sw, sy + sh), color);
	}
	
	SLIB_INLINE void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(rectDst, texture, Rectangle(0, 0, 1, 1), color);
	}
	
	SLIB_INLINE void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Color4f& color)
	{
		drawTexture2D(Rectangle(dx, dy, dx + dw, dy + dh), texture, Rectangle(0, 0, 1, 1), color);
	}

	Point screenToViewport(const Point& ptViewport);
	
	Point screenToViewport(sl_real x, sl_real y);
	
	Point viewportToScreen(const Point& ptScreen);
	
	Point viewportToScreen(sl_real x, sl_real y);
	
	Rectangle screenToViewport(const Rectangle& rc);
	
	Rectangle screenToViewport(sl_real x, sl_real y, sl_real width, sl_real height);
	
	Rectangle viewportToScreen(const Rectangle& rc);
	
	Rectangle viewportToScreen(sl_real x, sl_real y, sl_real width, sl_real height);

	void drawLines(const Ref<RenderProgram>& program, Line3* lines, sl_uint32 n = 1);
	
	void drawDebugText();

public:
	SLIB_INLINE sl_uint32 getViewportWidth()
	{
		return m_viewportWidth;
	}
	
	SLIB_INLINE sl_uint32 getViewportHeight()
	{
		return m_viewportHeight;
	}

	SLIB_INLINE sl_uint32 getCountOfDrawnElementsOnLastScene()
	{
		return m_nCountDrawnElementsOnLastScene;
	}

	SLIB_INLINE sl_uint32 getCountOfDrawnPrimitivesOnLastScene()
	{
		return m_nCountDrawnPrimitivesOnLastScene;
	}

protected:
	sl_uint32 m_viewportWidth;
	sl_uint32 m_viewportHeight;

	// debug 
	sl_uint32 m_nCountDrawnElementsOnLastScene;
	sl_uint32 m_nCountDrawnPrimitivesOnLastScene;
	Time m_timeBeginLastScene;
	sl_uint32 m_nRenderMillisecondsOnLastScene;
	Ref<Texture> m_textureDebug;
	Ref<Font> m_fontDebug;

	Ref<VertexBuffer> m_defaultVertexBufferForDrawRectangle2D;
	Ref<RenderProgram2D> m_defaultRenderProgramForDrawRectangle2D;
	
	Ref<VertexBuffer> m_defaultVertexBufferForDrawTexture2D;
	Ref<RenderProgram2D> m_defaultRenderProgramForDrawTexture2D;
	
private:
	Ref<VertexBuffer> _getDefaultVertexBufferForDrawRectangle2D();
	Ref<RenderProgram2D> _getDefaultRenderProgramForDrawRectangle2D();

	Ref<VertexBuffer> _getDefaultVertexBufferForDrawTexture2D();
	Ref<RenderProgram2D> _getDefaultRenderProgramForDrawTexture2D();

};

SLIB_RENDER_NAMESPACE_END

#endif
