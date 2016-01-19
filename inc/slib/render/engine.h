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

enum PrimitiveType
{
    primitiveType_Lines = 0,
    primitiveType_LineStrip = 1,
    primitiveType_Triangles = 2,
    primitiveType_TriangleStrip = 3,
    primitiveType_TriangleFan = 4,
    primitiveType_Points = 5
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
	
};

enum RenderBlendingOperation
{
    renderBlendingOperation_Add = 0,
    renderBlendingOperation_Subtract = 1,
    renderBlendingOperation_ReverseSubtract = 2
};

enum RenderBlendingFactor
{
    renderBlendingFactor_One = 0,
    renderBlendingFactor_Zero = 1,
    renderBlendingFactor_SrcAlpha = 2,
    renderBlendingFactor_OneMinusSrcAlpha = 3,
    renderBlendingFactor_DstAlpha = 4,
    renderBlendingFactor_OneMinusDstAlpha = 5,
    renderBlendingFactor_SrcColor = 6,
    renderBlendingFactor_OneMinusSrcColor = 7,
    renderBlendingFactor_DstColor = 8,
    renderBlendingFactor_OneMinusDstColor = 9,
    renderBlendingFactor_SrcAlphaSaturate = 10, // f = min(As, 1 - Ad)
    renderBlendingFactor_Constant = 11,
    renderBlendingFactor_OneMinusConstant = 12,
    renderBlendingFactor_ConstantAlpha = 13,
    renderBlendingFactor_OneMinusConstantAlpha = 14
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
	
};


class SLIB_EXPORT IRenderCallback
{
public:
	virtual void onFrame(RenderEngine* engine) = 0;
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
	
	Ptr<IRenderCallback> callback;
	
public:
	RendererParam();
	
};

class SLIB_EXPORT Renderer : public Object
{
	SLIB_DECLARE_OBJECT(Renderer, Object)
protected:
	Renderer();
	
public:
	virtual void release() = 0;
	
	virtual void requestRender() = 0;

public:
	SLIB_BOOLEAN_PROPERTY_INLINE(RenderingContinuously)

protected:
	Ptr<IRenderCallback> m_callback;

};


enum RenderEngineType
{
    renderEngineType_OpenGL = 0x01010001,
    renderEngineType_OpenGL_ES = 0x01020001,
    renderEngineType_D3D9 = 0x02010901,
    renderEngineType_D3D11 = 0x02010B01
};

class SLIB_EXPORT RenderEngine : public Object
{
	SLIB_DECLARE_OBJECT(RenderEngine, Object)
protected:
	RenderEngine();

public:
	virtual RenderEngineType getEngineType() = 0;
	
	sl_bool isOpenGL();
	
	sl_bool isOpenGL_ES();
	
	sl_bool isD3D();

public:
	void release();

	sl_bool isValid();

	sl_bool beginScene();
	
	void endScene();

	void setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
	
	void clear(const RenderClearParam& param);
	
	void clearColor(const Color& color);
	
	void clearColorDepth(const Color& color, float depth = 1.0f);
	
	void clearDepth(float depth = 1.0f);
	
	void setDepthTest(sl_bool flagEnableDepthTest);

	void setDepthWriteEnabled(sl_bool flagEnableDepthWrite);

	void setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW = sl_true);

	void setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param);
	
	void setBlending(sl_bool flagEnableBlending);
	
	sl_bool beginProgram(const Ref<RenderProgram>& program);
	
	void endProgram();
	
	void drawPrimitive(Primitive* primitive);

	void applyTexture(const void* sampler, const Ref<Texture>& texture);
	
	void applyTexture(sl_reg sampler, const Ref<Texture>& texture);

	sl_bool linkTexture(const Ref<Texture>& texture);

	sl_bool linkVertexBuffer(const Ref<VertexBuffer>& vb);
	
	sl_bool linkIndexBuffer(const Ref<IndexBuffer>& ib);

	void setLineWidth(sl_real width);

public:
	void draw(const Ref<RenderProgram>& program, Primitive* primitives, sl_uint32 count = 1);

	void draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib, PrimitiveType type = primitiveType_Triangles);

	void draw(const Ref<RenderProgram>& program, sl_uint32 countElements, const Ref<VertexBuffer>& vb, PrimitiveType type = primitiveType_Triangles);

	// Position(0, 0, 1, 1)
	void drawRectangle2D(const Ref<RenderProgram>& program);
	
	void drawRectangle2D(const Matrix3& transform, const Ref<RenderProgram2D>& program);

	void drawRectangle2D(const Rectangle& rectDst, const Ref<RenderProgram2D>& program);

	void drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<RenderProgram2D>& program);

	void drawRectangle2D(const Matrix3& transform, const Color4f& color);
	
	void drawRectangle2D(const Rectangle& rectDst, const Color4f& color);
	
	void drawRectangle2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Color4f& color);

	// Position(0, 0, 1, 1) TexCoord(0, 0, 1, 1)
	void drawTexture2D(const Ref<RenderProgram>& program);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program);

	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Ref<RenderProgram2D>& program);

	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Ref<RenderProgram2D>& program);

	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program);

	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Ref<RenderProgram2D>& program);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
	
	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha = 1);
	
	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha = 1);
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha = 1);
	
	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, sl_real alpha = 1);
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha = 1);
	
	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real alpha = 1);

	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
	
	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color);
	
	void drawTexture2D(const Matrix3& transform, const Ref<Texture>& texture, const Color4f& color);
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const Color4f& color);
	
	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, sl_real sx, sl_real sy, sl_real sw, sl_real sh, const Color& color);
	
	void drawTexture2D(const Rectangle& rectDst, const Ref<Texture>& texture, const Color4f& color);
	
	void drawTexture2D(sl_real dx, sl_real dy, sl_real dw, sl_real dh, const Ref<Texture>& texture, const Color4f& color);
	
	void drawLines(const Ref<RenderProgram>& program, Line3* lines, sl_uint32 n = 1);
	
	void drawDebugText();

	Point screenToViewport(const Point& ptViewport);
	
	Point screenToViewport(sl_real x, sl_real y);
	
	Point viewportToScreen(const Point& ptScreen);
	
	Point viewportToScreen(sl_real x, sl_real y);
	
	Rectangle screenToViewport(const Rectangle& rc);
	
	Rectangle screenToViewport(sl_real x, sl_real y, sl_real width, sl_real height);
	
	Rectangle viewportToScreen(const Rectangle& rc);
	
	Rectangle viewportToScreen(sl_real x, sl_real y, sl_real width, sl_real height);

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
	
private:
	Ref<VertexBuffer> _getDefaultVertexBufferForDrawRectangle2D();
	
	Ref<RenderProgram2D> _getDefaultRenderProgramForDrawRectangle2D();
	
	Ref<VertexBuffer> _getDefaultVertexBufferForDrawTexture2D();
	
	Ref<RenderProgram2D> _getDefaultRenderProgramForDrawTexture2D();
	
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

	SafeRef<VertexBuffer> m_defaultVertexBufferForDrawRectangle2D;
	SafeRef<RenderProgram2D> m_defaultRenderProgramForDrawRectangle2D;
	
	SafeRef<VertexBuffer> m_defaultVertexBufferForDrawTexture2D;
	SafeRef<RenderProgram2D> m_defaultRenderProgramForDrawTexture2D;
	

};

SLIB_RENDER_NAMESPACE_END

#endif
