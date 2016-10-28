#include "../../../inc/slib/render/canvas.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/math/transform2d.h"
#include "../../../inc/slib/core/variant.h"
#include "../../../inc/slib/graphics/util.h"
#include "../../../inc/slib/graphics/font_atlas.h"

#define MAX_PROGRAM_COUNT 256
#define MAX_SHADER_CLIP 8

SLIB_RENDER_NAMESPACE_BEGIN

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
	RenderCanvasClip()
	: flagTransform(sl_false)
	{
	}
	
};

class RenderCanvasStatus : public Referable
{
public:
	Matrix3 matrix;
	sl_bool flagClipRect;
	Rectangle clipRect;
	Queue<RenderCanvasClip> clips;
	
public:
	RenderCanvasStatus()
	: matrix(Matrix3::identity()), flagClipRect(sl_false)
	{
	}
	
	RenderCanvasStatus(RenderCanvasStatus* other)
	: matrix(other->matrix), flagClipRect(other->flagClipRect), clipRect(other->clipRect)
	{
		clips.pushAll(&(other->clips));
	}
	
};

class RenderCanvasProgramParam
{
public:
	RenderCanvasStatus* status;
	sl_bool flagIgnoreRectClip;
	sl_bool flagUseTexture;
	sl_bool flagUseColorFilter;
	
public:
	RenderCanvasProgramParam()
	{
		flagIgnoreRectClip = sl_false;
		flagUseTexture = sl_false;
		flagUseColorFilter = sl_false;
	}
	
};

class RenderCanvasProgramState : public RenderProgramState
{
public:
	Matrix3 transform;
	Color4f color;
	SafeRef<Texture> texture;
	Vector4 colorFilterR;
	Vector4 colorFilterG;
	Vector4 colorFilterB;
	Vector4 colorFilterA;
	Vector4 colorFilterC;
	Rectangle rectDst;
	Rectangle rectSrc;
	Rectangle rectClip;
	Matrix3 clipTransforms[MAX_SHADER_CLIP];
	Vector4 clipRects[MAX_SHADER_CLIP];
	
	sl_int32 gl_uniformTransform;		// Matrix3
	sl_int32 gl_uniformColor;			// Vector4
	sl_int32 gl_uniformTexture;			// samplers
	sl_int32 gl_uniformColorFilterR;	// Vector4
	sl_int32 gl_uniformColorFilterG;	// Vector4
	sl_int32 gl_uniformColorFilterB;	// Vector4
	sl_int32 gl_uniformColorFilterA;	// Vector4
	sl_int32 gl_uniformColorFilterC;	// Vector4
	sl_int32 gl_uniformRectDst;			// Vector4
	sl_int32 gl_uniformRectSrc;			// Vector4
	sl_int32 gl_uniformRectClip;		// Vector4
	sl_int32 gl_uniformClipTransforms[MAX_SHADER_CLIP];	// Matrix3
	sl_int32 gl_uniformClipRects[MAX_SHADER_CLIP];		// Vector4
	
	sl_int32 gl_attrPosition;		// Vector2
	
public:
	RenderCanvasProgramState()
	: transform(Matrix3::identity()), color(1, 1, 1, 1)
	{
	}
	
};

class RenderCanvasProgram : public RenderProgramT<RenderProgram, RenderCanvasProgramState>
{
public:
	struct VertexData
	{
		Vector2 position;
	};
	
public:
	sl_bool m_flagUseTexture;
	sl_bool m_flagUseRectClip;
	sl_bool m_flagUseColorFilter;
	sl_uint32 m_countClips;
	
	String m_vertexShader;
	String m_fragmentShader;
	
public:
	RenderCanvasProgram()
	{
		m_flagUseTexture = sl_false;
		m_flagUseRectClip = sl_false;
		m_flagUseColorFilter = sl_false;
		m_countClips = 0;
	}
	
public:
	// override
	sl_bool onInit(RenderEngine* _engine, RenderProgramState* _state)
	{
		RenderEngineType type = _engine->getEngineType();
		
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			RenderCanvasProgramState* state = (RenderCanvasProgramState*)_state;
			
			sl_uint32 program = state->gl_program;
			state->gl_attrPosition = engine->getAttributeLocation(program, "a_Position");
			
			state->gl_uniformTransform = engine->getUniformLocation(program, "u_Transform");
			state->gl_uniformColor = engine->getUniformLocation(program, "u_Color");
			state->gl_uniformRectDst = engine->getUniformLocation(program, "u_rectDst");
			if (m_flagUseTexture) {
				state->gl_uniformRectSrc = engine->getUniformLocation(program, "u_rectSrc");
				state->gl_uniformTexture = engine->getUniformLocation(program, "u_Texture");
			}
			if (m_flagUseColorFilter) {
				state->gl_uniformColorFilterR = engine->getUniformLocation(program, "u_ColorFilterR");
				state->gl_uniformColorFilterG = engine->getUniformLocation(program, "u_ColorFilterG");
				state->gl_uniformColorFilterB = engine->getUniformLocation(program, "u_ColorFilterB");
				state->gl_uniformColorFilterA = engine->getUniformLocation(program, "u_ColorFilterA");
				state->gl_uniformColorFilterC = engine->getUniformLocation(program, "u_ColorFilterC");
			}
			if (m_flagUseRectClip) {
				state->gl_uniformRectClip = engine->getUniformLocation(program, "u_rectClip");
			}
			
			for (sl_uint32 i = 0; i < m_countClips; i++) {
				state->gl_uniformClipRects[i] = engine->getUniformLocation(program, String::format("u_clipRect%d", i));
				state->gl_uniformClipTransforms[i] = engine->getUniformLocation(program, String::format("u_clipTransform%d", i));
			}
			
			return sl_true;
		}
		return sl_false;
	}
	
	// override
	sl_bool onUpdate(RenderEngine* _engine, RenderProgramState* _state)
	{
		RenderEngineType type = _engine->getEngineType();
		
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			RenderCanvasProgramState* state = (RenderCanvasProgramState*)_state;

			engine->setUniformMatrix3Value(state->gl_uniformTransform, state->transform);
			engine->setUniformFloat4Value(state->gl_uniformColor, state->color);
			engine->setUniformFloat4Value(state->gl_uniformRectDst, Vector4(state->rectDst.left, state->rectDst.top, state->rectDst.getWidth(), state->rectDst.getHeight()));
			if (m_flagUseTexture) {
				engine->setUniformFloat4Value(state->gl_uniformRectSrc, Vector4(state->rectSrc.left, state->rectSrc.top, state->rectSrc.getWidth(), state->rectSrc.getHeight()));
				engine->setUniformTextureSampler(state->gl_uniformTexture, 0);
				engine->applyTexture(0, state->texture);
			}
			if (m_flagUseColorFilter) {
				engine->setUniformFloat4Value(state->gl_uniformColorFilterR, state->colorFilterR);
				engine->setUniformFloat4Value(state->gl_uniformColorFilterG, state->colorFilterG);
				engine->setUniformFloat4Value(state->gl_uniformColorFilterB, state->colorFilterB);
				engine->setUniformFloat4Value(state->gl_uniformColorFilterA, state->colorFilterA);
				engine->setUniformFloat4Value(state->gl_uniformColorFilterC, state->colorFilterC);
			}
			if (m_flagUseRectClip) {
				engine->setUniformFloat4Value(state->gl_uniformRectClip, Vector4(state->rectClip.left, state->rectClip.top, state->rectClip.right, state->rectClip.bottom));
			}
			
			for (sl_uint32 i = 0; i < m_countClips; i++) {
				engine->setUniformFloat4Value(state->gl_uniformClipRects[i], state->clipRects[i]);
				engine->setUniformMatrix3Value(state->gl_uniformClipTransforms[i], state->clipTransforms[i]);
			}
			
			return sl_true;
		}
		
		return sl_false;
	}
	
	// override
	sl_bool onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
	{
		RenderEngineType type = _engine->getEngineType();
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			RenderCanvasProgramState* state = (RenderCanvasProgramState*)_state;
			SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
			return sl_true;
		}
		return sl_false;
	}
	
	// override
	void onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
	{
		RenderEngineType type = _engine->getEngineType();
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			RenderCanvasProgramState* state = (RenderCanvasProgramState*)_state;
			engine->disableVertexArrayAttribute(state->gl_attrPosition);
		}
	}

	// override
	String getGLSLVertexShader(RenderEngine* engine)
	{
		return m_vertexShader;
	}
	
	// override
	String getGLSLFragmentShader(RenderEngine* engine)
	{
		return m_fragmentShader;
	}
	
	static void generateShaderSources(const RenderCanvasProgramParam& param, StringBuffer* signatures, StringBuffer* bufVertexShader, StringBuffer* bufFragmentShader, sl_bool* outFlagUseRectClip, sl_uint32* outCountClips)
	{
		StringBuffer bufVBHeader;
		StringBuffer bufVBContent;
		StringBuffer bufFBHeader;
		StringBuffer bufFBContent;
		
		if (signatures) {
			SLIB_STATIC_STRING(s, "S");
			signatures->add(s);
		}
		
		if (bufVertexShader) {
			bufVBHeader.add(SLIB_STRINGIFY(
				uniform mat3 u_Transform;
				uniform vec4 u_rectDst;
				attribute vec2 a_Position;
			));
			bufVBContent.add("void main() {");
			bufVBContent.add(SLIB_STRINGIFY(
				vec2 pos = a_Position * u_rectDst.zw + u_rectDst.xy;
				gl_Position = vec4((vec3(pos, 1.0) * u_Transform).xy, 0.0, 1.0);
			));
			bufFBHeader.add(SLIB_STRINGIFY(
				uniform vec4 u_Color;
			));
			bufFBContent.add("void main() {");
		}
		
		sl_bool flagUseRectClip = sl_false;
		if (!(param.flagIgnoreRectClip) && (param.status->flagClipRect)) {
			flagUseRectClip = sl_true;
			if (signatures) {
				SLIB_STATIC_STRING(s, "C");
				signatures->add(s);
			}
			if (bufVertexShader) {
				bufVBHeader.add(SLIB_STRINGIFY(
					varying vec2 v_Pos;
				));
				bufVBContent.add(SLIB_STRINGIFY(
					v_Pos = pos;
				));
				bufFBHeader.add(SLIB_STRINGIFY(
					varying vec2 v_Pos;
					uniform vec4 u_rectClip;
				));
				bufFBContent.add(SLIB_STRINGIFY(
					float fClip = step(u_rectClip.x, v_Pos.x) * step(u_rectClip.y, v_Pos.y) * (1.0 - step(u_rectClip.z, v_Pos.x)) * (1.0 - step(u_rectClip.w, v_Pos.y));
					if (fClip < 0.5) {
						discard;
					}
				));
			}
		}
		
		sl_uint32 countClips = 0;
		
		if (param.flagUseTexture) {
			if (signatures) {
				SLIB_STATIC_STRING(s, "T");
				signatures->add(s);
			}
			if (bufVertexShader) {
				bufVBHeader.add(SLIB_STRINGIFY(
					uniform vec4 u_rectSrc;
					varying vec2 v_TexCoord;
				));
				bufVBContent.add(SLIB_STRINGIFY(
					v_TexCoord = a_Position * u_rectSrc.zw + u_rectSrc.xy;
				));
				bufFBHeader.add(SLIB_STRINGIFY(
					uniform sampler2D u_Texture;
					varying vec2 v_TexCoord;
				));
			}
			
			if (param.flagUseColorFilter) {
				if (signatures) {
					SLIB_STATIC_STRING(s, "F");
					signatures->add(s);
				}
				if (bufVertexShader) {
					bufFBHeader.add(SLIB_STRINGIFY(
						uniform vec4 u_ColorFilterR;
						uniform vec4 u_ColorFilterG;
						uniform vec4 u_ColorFilterB;
						uniform vec4 u_ColorFilterA;
						uniform vec4 u_ColorFilterC;
					));
					bufFBContent.add(SLIB_STRINGIFY(
						vec4 color = texture2D(u_Texture, v_TexCoord);
						color = vec4(dot(color, u_ColorFilterR), dot(color, u_ColorFilterG), dot(color, u_ColorFilterB), dot(color, u_ColorFilterA)) + u_ColorFilterC;
						color = color * u_Color;
					));
				}
			} else {
				if (bufVertexShader) {
					bufFBContent.add(SLIB_STRINGIFY(
						vec4 color = texture2D(u_Texture, v_TexCoord) * u_Color;
					));
				}
			}
		} else {
			if (bufVertexShader) {
				bufFBContent.add(SLIB_STRINGIFY(
					vec4 color = u_Color;
				));
			}
		}
		
		
		if (bufVertexShader) {
			bufFBContent.add(SLIB_STRINGIFY(
				gl_FragColor = color;
			));
			bufVBContent.add("}");
			bufFBContent.add("}");
			bufVertexShader->link(bufVBHeader);
			bufVertexShader->link(bufVBContent);
			bufFragmentShader->link(bufFBHeader);
			bufFragmentShader->link(bufFBContent);
		}
		
		if (outFlagUseRectClip) {
			*outFlagUseRectClip = flagUseRectClip;
		}
		
		if (outCountClips) {
			*outCountClips = countClips;
		}
		
	}
	
	static Ref<RenderCanvasProgram> create(const RenderCanvasProgramParam& param)
	{
		StringBuffer sbVB;
		StringBuffer sbFB;
		sl_bool flagUseRectClip;
		sl_uint32 countClips;
		RenderCanvasProgram::generateShaderSources(param, sl_null, &sbVB, &sbFB, &flagUseRectClip, &countClips);
		String vertexShader = sbVB.merge();
		String fragmentShader = sbFB.merge();
		if (vertexShader.isNotEmpty() && fragmentShader.isNotEmpty()) {
			Ref<RenderCanvasProgram> ret = new RenderCanvasProgram;
			if (ret.isNotNull()) {
				ret->m_vertexShader = vertexShader;
				ret->m_fragmentShader = fragmentShader;
				ret->m_countClips = countClips;
				ret->m_flagUseTexture = param.flagUseTexture;
				ret->m_flagUseColorFilter = param.flagUseColorFilter;
				ret->m_flagUseRectClip = flagUseRectClip;				
				return ret;
			}
		}
		return Ref<RenderCanvasProgram>::null();
	}
	
};

class _RenderCanvas_Shared
{
public:
	HashMap< String, Ref<RenderCanvasProgram> > programs;
	Ref<VertexBuffer> vbRectangle;
	
public:
	_RenderCanvas_Shared()
	{
		static RenderProgram2D_Position::VertexData v[] = {
			{ { 0, 0 } }
			, { { 1, 0 } }
			, { { 0, 1 } }
			, { { 1, 1 } }
		};
		vbRectangle = VertexBuffer::create(v, sizeof(v));
	}

	Ref<RenderCanvasProgram> getProgram(const RenderCanvasProgramParam& param)
	{
		StringBuffer sb;
		RenderCanvasProgram::generateShaderSources(param, &sb, sl_null, sl_null, sl_null, sl_null);
		Ref<RenderCanvasProgram> program;
		String sig = sb.merge();
		if (sig.isNotEmpty()) {
			if (!(programs.get_NoLock(sig, &program))) {
				program = RenderCanvasProgram::create(param);
				if (program.isNull()) {
					return Ref<RenderCanvasProgram>::null();
				}
				if (programs.getCount() > MAX_PROGRAM_COUNT) {
					programs.removeAll_NoLock();
				}
				programs.put_NoLock(sig, program);
			}
			return program;
		}
		return Ref<RenderCanvasProgram>::null();
	}
	
};

SLIB_SAFE_STATIC_GETTER(_RenderCanvas_Shared, _RenderCanvas_getShared)


SLIB_DEFINE_OBJECT(RenderCanvas, Canvas)

RenderCanvas::RenderCanvas()
{
}

Ref<RenderCanvas> RenderCanvas::create(const Ref<RenderEngine>& engine, sl_real width, sl_real height)
{
	if (engine.isNotNull()) {
		
		Ref<RenderCanvasStatus> status = new RenderCanvasStatus;
		
		if (status.isNotNull()) {
			
			Ref<RenderCanvas> ret = new RenderCanvas;
			if (ret.isNotNull()) {
				ret->m_engine = engine;
				ret->m_width = width;
				ret->m_height = height;
				ret->m_matViewport = Matrix3(2/width, 0, 0, 0, -2/height, 0, -1, 1, 1);
				
				ret->m_status = status;
				
				ret->setType(CanvasType::Render);
				ret->setSize(Size(width, height));
				
				return ret;
			}
			
		}
		
	}
	return Ref<RenderCanvas>::null();
}

void RenderCanvas::save()
{
	RenderCanvasStatus* statusOld = m_status.ptr;
	Ref<RenderCanvasStatus> statusNew = new RenderCanvasStatus(statusOld);
	if (statusNew.isNotNull()) {
		m_stackStatus.push_NoLock(statusOld);
		m_status = statusNew;
	}
}

void RenderCanvas::restore()
{
	Ref<RenderCanvasStatus> statusBack;
	if (m_stackStatus.pop_NoLock(&statusBack)) {
		m_status = statusBack;
	}
}

Rectangle RenderCanvas::getClipBounds()
{
	Ref<RenderCanvasStatus>& status = m_status;
	Rectangle rect;
	if (status->flagClipRect) {
		rect = status->clipRect;
	} else {
		rect = Rectangle(0, 0, m_width, m_height);
	}
	Link<RenderCanvasClip>* link = status->clips.getBegin();
	while (link) {
		Rectangle r = link->value.region;
		if (link->value.flagTransform) {
			r.transform(link->value.transform.inverse());
		}
		if (!(rect.intersectRectangle(r, &rect))) {
			return Rectangle::zero();
		}
		link = link->next;
	}
	return rect;
}

void RenderCanvas::clipToRectangle(const Rectangle& rect)
{
	RenderCanvasStatus* status = m_status.ptr;
	if (status->flagClipRect) {
		status->clipRect.intersectRectangle(rect, &(status->clipRect));
	} else {
		status->flagClipRect = sl_true;
		status->clipRect = rect;
	}
}

void RenderCanvas::clipToPath(const Ref<GraphicsPath>& path)
{
}

void RenderCanvas::clipToRoundRect(const Rectangle& rect, const Size& radius)
{
	RenderCanvasStatus* status = m_status.ptr;
	RenderCanvasClip clip;
	clip.type = RenderCanvasClipType::RoundRect;
	clip.region = rect;
	clip.rx = radius.x;
	clip.ry = radius.y;
	status->clips.push_NoLock(clip);
}

void RenderCanvas::clipToEllipse(const Rectangle& rect)
{
	RenderCanvasStatus* status = m_status.ptr;
	RenderCanvasClip clip;
	clip.type = RenderCanvasClipType::Ellipse;
	clip.region = rect;
	status->clips.push_NoLock(clip);
}

void RenderCanvas::concatMatrix(const Matrix3& matrix)
{
	RenderCanvasStatus* status = m_status.ptr;
	status->matrix = matrix * status->matrix;
	if (Math::isAlmostZero(matrix.m00 - 1) && Math::isAlmostZero(matrix.m01) && Math::isAlmostZero(matrix.m10) && Math::isAlmostZero(matrix.m11 - 1)) {
		sl_real tx = matrix.m20;
		sl_real ty = matrix.m21;
		if (status->flagClipRect) {
			status->clipRect.left -= tx;
			status->clipRect.top -= ty;
			status->clipRect.right -= tx;
			status->clipRect.bottom -= ty;
		}
		Link<RenderCanvasClip>* link = status->clips.getBegin();
		while (link) {
			RenderCanvasClip& clip = link->value;
			if (clip.flagTransform) {
				clip.transform = matrix * clip.transform;
			} else {
				clip.region.left -= tx;
				clip.region.top -= ty;
				clip.region.right -= tx;
				clip.region.bottom -= ty;
			}
			link = link->next;
		}
	} else {
		Link<RenderCanvasClip>* link = status->clips.getBegin();
		while (link) {
			RenderCanvasClip& clip = link->value;
			if (clip.flagTransform) {
				clip.transform = matrix * clip.transform;
			} else {
				clip.flagTransform = sl_true;
				clip.transform = matrix;
			}
			link = link->next;
		}
		RenderCanvasClip clip;
		clip.type = RenderCanvasClipType::Rectangle;
		clip.region = status->clipRect;
		clip.flagTransform = sl_true;
		clip.transform = matrix;
		status->clips.push_NoLock(clip);
		status->flagClipRect = sl_false;
	}
}

class _RenderCanvasFont : public Font
{
public:
	friend class RenderCanvas;
};

void RenderCanvas::drawText16(const String16& text, sl_real x, sl_real y, const Ref<Font>& _font, const Color& _color)
{
	if (text.isEmpty()) {
		return;
	}
	Ref<Font> font = _font;
	if (font.isNull()) {
		font = Font::getDefault();
		if (font.isNull()) {
			return;
		}
	}
	_RenderCanvasFont* rfont = (_RenderCanvasFont*)(font.ptr);
	Ref<Referable> cache = rfont->m_renderingCache;
	if (cache.isNull()) {
		cache = FontAtlas::getShared(font);
		if (cache.isNull()) {
			return;
		}
		rfont->m_renderingCache = cache;
	}
	FontAtlas* fa = (FontAtlas*)(cache.ptr);
	sl_char16* arrChar = text.getData();
	sl_size len = text.getLength();
	FontAtlasChar fac;
	DrawParam dp;
	Color4f color = _color;
	for (sl_size i = 0; i < len; i++) {
		sl_char16 ch = arrChar[i];
		if (fa->getChar(ch, fac)) {
			Ref<Texture> texture = Texture::getBitmapRenderingCache(fac.bitmap);
			if (texture.isNotNull()) {
				sl_real xn = x + (sl_real)(fac.fontWidth);
				drawTexture(Rectangle(x, y, xn, y + (sl_real)(fac.fontHeight)), texture, fac.region, dp, color);
				x = xn;
			}
		}
	}
}

void RenderCanvas::drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& pen)
{
}

void RenderCanvas::drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen)
{
}

void RenderCanvas::drawArc(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen)
{
}

void RenderCanvas::drawRectangle(const Rectangle& _rect, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	_RenderCanvas_Shared* shared = _RenderCanvas_getShared();
	if (!shared) {
		return;
	}
	
	RenderCanvasStatus* status = m_status.ptr;
	
	if (brush.isNotNull()) {
		Rectangle rect = _rect;
		if (status->flagClipRect) {
			if (!(status->clipRect.intersectRectangle(rect, &rect))) {
				return;
			}
		}
		
		RenderCanvasProgramParam pp;
		pp.status = status;
		pp.flagIgnoreRectClip = sl_true;
		
		RenderProgramScope<RenderCanvasProgramState> scope;
		if (scope.begin(m_engine.ptr, shared->getProgram(pp))) {
			scope->transform = status->matrix * m_matViewport;
			scope->rectDst = rect;
			scope->color = brush->getColor();
			scope->color.w *= getAlpha();
			m_engine->drawPrimitive(4, shared->vbRectangle, PrimitiveType::TriangleStrip);
		}
	}
	
}

void RenderCanvas::drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	
}

void RenderCanvas::drawEllipse(const Rectangle& rect, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	
}

void RenderCanvas::drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& pen, const Ref<Brush>& brush, FillMode fillMode)
{
}

void RenderCanvas::drawPie(const Rectangle& rect, sl_real startDegrees, sl_real sweepDegrees, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
}

void RenderCanvas::drawPath(const Ref<GraphicsPath>& path, const Ref<Pen>& pen, const Ref<Brush>& brush)
{
}

void RenderCanvas::drawTexture(const Rectangle& _rectDst, const Ref<Texture>& texture, const Rectangle& _rectSrc, const DrawParam& param, const Color4f& color)
{
	
	_RenderCanvas_Shared* shared = _RenderCanvas_getShared();
	if (!shared) {
		return;
	}
	
	RenderCanvasStatus* status = m_status.ptr;
	
	Rectangle rectDst = _rectDst;
	Rectangle rectSrc = _rectSrc;
	sl_real sw = (sl_real)(texture->getWidth());
	sl_real sh = (sl_real)(texture->getHeight());
	if (status->flagClipRect) {
		Rectangle rectIntersectClip;
		if (status->clipRect.intersectRectangle(rectDst, &rectIntersectClip)) {
			if (!(rectDst.isAlmostEqual(rectIntersectClip))) {
				rectSrc = GraphicsUtil::transformRectangle(rectSrc, rectDst, rectIntersectClip);
				rectDst = rectIntersectClip;
			}
		} else {
			return;
		}
	}
	rectSrc.left /= sw;
	rectSrc.top /= sh;
	rectSrc.right /= sw;
	rectSrc.bottom /= sh;
	
	RenderCanvasProgramParam pp;
	pp.status = status;
	pp.flagUseTexture = sl_true;
	pp.flagIgnoreRectClip = sl_true;
	if (param.useColorMatrix) {
		pp.flagUseColorFilter = sl_true;
	}
	
	RenderProgramScope<RenderCanvasProgramState> scope;
	if (scope.begin(m_engine.ptr, shared->getProgram(pp))) {
		scope->texture = texture;
		scope->transform = status->matrix * m_matViewport;
		scope->rectDst = rectDst;
		scope->rectSrc = rectSrc;
		if (param.useColorMatrix) {
			scope->colorFilterR = param.colorMatrix.red;
			scope->colorFilterG = param.colorMatrix.green;
			scope->colorFilterB = param.colorMatrix.blue;
			scope->colorFilterA = param.colorMatrix.alpha;
			scope->colorFilterC = param.colorMatrix.bias;
		}
		if (param.useAlpha) {
			scope->color = Color4f(color.x, color.y, color.z, color.w * param.alpha * getAlpha());
		} else {
			scope->color = Color4f(color.x, color.y, color.z, color.w * getAlpha());
		}
		m_engine->drawPrimitive(4, shared->vbRectangle, PrimitiveType::TriangleStrip);
	}
	
}

void RenderCanvas::drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param)
{
	drawTexture(rectDst, texture, rectSrc, param, Color4f(1, 1, 1, 1));
}

void RenderCanvas::drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
{
	drawTexture(rectDst, texture, rectSrc, DrawParam(), Color4f(1, 1, 1, alpha));
}

void RenderCanvas::drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const DrawParam& param, const Color4f& color)
{
	if (texture.isNotNull()) {
		drawTexture(rectDst, texture, Rectangle(0, 0, (sl_real)(texture->getWidth()), (sl_real)(texture->getHeight())), param, color);
	}
}

void RenderCanvas::drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, const DrawParam& param)
{
	drawTexture(rectDst, texture, param, Color4f(1, 1, 1, 1));
}

void RenderCanvas::drawTexture(const Rectangle& rectDst, const Ref<Texture>& texture, sl_real alpha)
{
	drawTexture(rectDst, texture, DrawParam(), Color4f(1, 1, 1, alpha));
}

void RenderCanvas::_drawBitmap(const Rectangle& rectDst, Bitmap* src, const Rectangle& rectSrc, const DrawParam& param)
{
	Ref<Texture> texture = Texture::getBitmapRenderingCache(src);
	if (texture.isNull()) {
		return;
	}
	drawTexture(rectDst, texture, rectSrc, param, Color4f(1, 1, 1, 1));
}

void RenderCanvas::onDraw(const Rectangle& rectDst, const Ref<Drawable>& src, const Rectangle& rectSrc, const DrawParam& param)
{
	if (src->isBitmap()) {
		_drawBitmap(rectDst, (Bitmap*)(src.ptr), rectSrc, param);
	} else {
		Canvas::onDraw(rectDst, src, rectSrc, param);
	}
}

void RenderCanvas::onDrawAll(const Rectangle& rectDst, const Ref<Drawable>& src, const DrawParam& param)
{
	if (src->isBitmap()) {
		_drawBitmap(rectDst, (Bitmap*)(src.ptr), Rectangle(0, 0, src->getDrawableWidth(), src->getDrawableHeight()), param);
	} else {
		Canvas::onDrawAll(rectDst, src, param);
	}
}

void RenderCanvas::_setAlpha(sl_real alpha)
{
}

void RenderCanvas::_setAntiAlias(sl_bool flag)
{
}

SLIB_RENDER_NAMESPACE_END
