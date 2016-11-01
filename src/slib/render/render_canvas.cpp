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

class RenderCanvasState : public Referable
{
public:
	Matrix3 matrix;
	sl_bool flagClipRect;
	Rectangle clipRect;
	Queue<RenderCanvasClip> clips;
	
public:
	RenderCanvasState()
	: matrix(Matrix3::identity()), flagClipRect(sl_false)
	{
	}
	
	RenderCanvasState(RenderCanvasState* other)
	: matrix(other->matrix), flagClipRect(other->flagClipRect), clipRect(other->clipRect)
	{
		clips.pushAll(&(other->clips));
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
	Rectangle rectSrc;
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
	sl_int32 gl_uniformRectSrc;			// Vector4
	sl_int32 gl_uniformClipTransforms[MAX_SHADER_CLIP];	// Matrix3
	sl_int32 gl_uniformClipRects[MAX_SHADER_CLIP];		// Vector4
	
	sl_int32 gl_attrPosition;		// Vector2
	
public:
	RenderCanvasProgramState()
	: transform(Matrix3::identity()), color(1, 1, 1, 1)
	{
	}
	
};

class RenderCanvasProgramParam
{
public:
	sl_bool flagUseTexture;
	sl_bool flagUseColorFilter;
	RenderCanvasClip* clips[MAX_SHADER_CLIP];
	sl_uint32 countClips;
	
public:
	RenderCanvasProgramParam()
	{
		flagUseTexture = sl_false;
		flagUseColorFilter = sl_false;
		countClips = 0;
	}
	
	void prepare(RenderCanvasState* state, sl_bool flagIgnoreRectClip)
	{
		countClips = 0;
		if (!flagIgnoreRectClip && state->flagClipRect) {
			storageRectClip.type = RenderCanvasClipType::Rectangle;
			storageRectClip.region = state->clipRect;
			clips[0] = &storageRectClip;
			countClips++;
		}
		Link<RenderCanvasClip>* clip = state->clips.getEnd();
		while (clip) {
			clips[countClips] = &(clip->value);
			countClips++;
			if (countClips >= MAX_SHADER_CLIP) {
				break;
			}
			clip = clip->before;
		}
	}
	
	void applyToProgramState(RenderCanvasProgramState* state, const Matrix3& transform)
	{
		for (sl_uint32 i = 0; i < countClips; i++) {
			Rectangle& r = clips[i]->region;
			state->clipRects[i] = Vector4(r.left, r.top, r.right, r.bottom);
			if (clips[i]->flagTransform) {
				state->clipTransforms[i] = transform * clips[i]->transform;
			} else {
				state->clipTransforms[i] = transform;
			}
		}
	}
	
private:
	RenderCanvasClip storageRectClip;
	
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
			if (m_flagUseTexture) {
				state->gl_uniformRectSrc = engine->getUniformLocation(program, "u_RectSrc");
				state->gl_uniformTexture = engine->getUniformLocation(program, "u_Texture");
			}
			if (m_flagUseColorFilter) {
				state->gl_uniformColorFilterR = engine->getUniformLocation(program, "u_ColorFilterR");
				state->gl_uniformColorFilterG = engine->getUniformLocation(program, "u_ColorFilterG");
				state->gl_uniformColorFilterB = engine->getUniformLocation(program, "u_ColorFilterB");
				state->gl_uniformColorFilterA = engine->getUniformLocation(program, "u_ColorFilterA");
				state->gl_uniformColorFilterC = engine->getUniformLocation(program, "u_ColorFilterC");
			}
			for (sl_uint32 i = 0; i < m_countClips; i++) {
				state->gl_uniformClipRects[i] = engine->getUniformLocation(program, String::format("u_ClipRect%d", i));
				state->gl_uniformClipTransforms[i] = engine->getUniformLocation(program, String::format("u_ClipTransform%d", i));
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
	
	static void generateShaderSources(const RenderCanvasProgramParam& param, StringBuffer* signatures, StringBuffer* bufVertexShader, StringBuffer* bufFragmentShader)
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
				attribute vec2 a_Position;
			));
			bufVBContent.add("void main() {");
			bufVBContent.add(SLIB_STRINGIFY(
				gl_Position = vec4((vec3(a_Position, 1.0) * u_Transform).xy, 0.0, 1.0);
			));
			bufFBHeader.add(SLIB_STRINGIFY(
				uniform vec4 u_Color;
			));
			bufFBContent.add("void main() {");
		}
		
		for (sl_uint32 i = 0; i < param.countClips; i++) {
			if (signatures) {
				SLIB_STATIC_STRING(s, "C");
				signatures->add(s);
			}
			if (bufVertexShader) {
				RenderCanvasClip& clip = *(param.clips[i]);
				bufVBHeader.add(String::format(SLIB_STRINGIFY(
					varying vec2 v_ClipPos%d;
					uniform mat3 u_ClipTransform%d;
				), i));
				bufVBContent.add(String::format(SLIB_STRINGIFY(
					v_ClipPos%d = (vec3(a_Position, 1.0) * u_ClipTransform%d).xy;
				), i));
				bufFBHeader.add(String::format(SLIB_STRINGIFY(
					varying vec2 v_ClipPos%d;
					uniform vec4 u_ClipRect%d;
				), i));
				bufFBContent.add(String::format(SLIB_STRINGIFY(
					float fClip%d = step(u_ClipRect%d.x, v_ClipPos%d.x) * step(u_ClipRect%d.y, v_ClipPos%d.y) * step(v_ClipPos%d.x, u_ClipRect%d.z) * step(v_ClipPos%d.y, u_ClipRect%d.w);
					if (fClip%d < 0.5) {
						discard;
					}
				), i));
			}
		}
		
		if (param.flagUseTexture) {
			if (signatures) {
				SLIB_STATIC_STRING(s, "T");
				signatures->add(s);
			}
			if (bufVertexShader) {
				bufVBHeader.add(SLIB_STRINGIFY(
					uniform vec4 u_RectSrc;
					varying vec2 v_TexCoord;
				));
				bufVBContent.add(SLIB_STRINGIFY(
					v_TexCoord = a_Position * u_RectSrc.zw + u_RectSrc.xy;
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
		
	}
	
	static Ref<RenderCanvasProgram> create(const RenderCanvasProgramParam& param)
	{
		StringBuffer sbVB;
		StringBuffer sbFB;
		RenderCanvasProgram::generateShaderSources(param, sl_null, &sbVB, &sbFB);
		String vertexShader = sbVB.merge();
		String fragmentShader = sbFB.merge();
		if (vertexShader.isNotEmpty() && fragmentShader.isNotEmpty()) {
			Ref<RenderCanvasProgram> ret = new RenderCanvasProgram;
			if (ret.isNotNull()) {
				ret->m_vertexShader = vertexShader;
				ret->m_fragmentShader = fragmentShader;
				ret->m_countClips = param.countClips;
				ret->m_flagUseTexture = param.flagUseTexture;
				ret->m_flagUseColorFilter = param.flagUseColorFilter;
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
		RenderCanvasProgram::generateShaderSources(param, &sb, sl_null, sl_null);
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
		
		Ref<RenderCanvasState> state = new RenderCanvasState;
		
		if (state.isNotNull()) {
			
			Ref<RenderCanvas> ret = new RenderCanvas;
			if (ret.isNotNull()) {
				ret->m_engine = engine;
				ret->m_width = width;
				ret->m_height = height;
				ret->m_matViewport = Matrix3(2/width, 0, 0, 0, -2/height, 0, -1, 1, 1);
				
				ret->m_state = state;
				
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
	RenderCanvasState* stateOld = m_state.ptr;
	Ref<RenderCanvasState> stateNew = new RenderCanvasState(stateOld);
	if (stateNew.isNotNull()) {
		m_stackStates.push_NoLock(stateOld);
		m_state = stateNew;
	}
}

void RenderCanvas::restore()
{
	Ref<RenderCanvasState> stateBack;
	if (m_stackStates.pop_NoLock(&stateBack)) {
		m_state = stateBack;
	}
}

Rectangle RenderCanvas::getClipBounds()
{
	Ref<RenderCanvasState>& state = m_state;
	Rectangle rect;
	if (state->flagClipRect) {
		rect = state->clipRect;
	} else {
		rect = Rectangle(0, 0, m_width, m_height);
	}
	Link<RenderCanvasClip>* link = state->clips.getBegin();
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
	RenderCanvasState* state = m_state.ptr;
	if (state->flagClipRect) {
		state->clipRect.intersectRectangle(rect, &(state->clipRect));
	} else {
		state->flagClipRect = sl_true;
		state->clipRect = rect;
	}
}

void RenderCanvas::clipToPath(const Ref<GraphicsPath>& path)
{
}

void RenderCanvas::clipToRoundRect(const Rectangle& rect, const Size& radius)
{
	RenderCanvasState* state = m_state.ptr;
	RenderCanvasClip clip;
	clip.type = RenderCanvasClipType::RoundRect;
	clip.region = rect;
	clip.rx = radius.x;
	clip.ry = radius.y;
	state->clips.push_NoLock(clip);
}

void RenderCanvas::clipToEllipse(const Rectangle& rect)
{
	RenderCanvasState* state = m_state.ptr;
	RenderCanvasClip clip;
	clip.type = RenderCanvasClipType::Ellipse;
	clip.region = rect;
	state->clips.push_NoLock(clip);
}

void RenderCanvas::concatMatrix(const Matrix3& matrix)
{
	RenderCanvasState* state = m_state.ptr;
	state->matrix = matrix * state->matrix;
	if (Math::isAlmostZero(matrix.m00 - 1) && Math::isAlmostZero(matrix.m01) && Math::isAlmostZero(matrix.m10) && Math::isAlmostZero(matrix.m11 - 1)) {
		sl_real tx = matrix.m20;
		sl_real ty = matrix.m21;
		if (state->flagClipRect) {
			state->clipRect.left -= tx;
			state->clipRect.top -= ty;
			state->clipRect.right -= tx;
			state->clipRect.bottom -= ty;
		}
		Link<RenderCanvasClip>* link = state->clips.getBegin();
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
		Link<RenderCanvasClip>* link = state->clips.getBegin();
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
		clip.region = state->clipRect;
		clip.flagTransform = sl_true;
		clip.transform = matrix;
		state->clips.push_NoLock(clip);
		state->flagClipRect = sl_false;
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
	sl_real fontHeight = font->getFontHeight();
	sl_bool fontItalic = font->isItalic();
	sl_real fx = x;
	
	for (sl_size i = 0; i < len; i++) {
		sl_char16 ch = arrChar[i];
		if (fa->getChar(ch, fac)) {
			Ref<Texture> texture = Texture::getBitmapRenderingCache(fac.bitmap);
			if (texture.isNotNull()) {
				sl_real fw = (sl_real)(fac.fontWidth);
				sl_real fh = (sl_real)(fac.fontHeight);
				sl_real fxn = fx + fw;
				sl_real fy = y + (fontHeight - fh) / 2;
				if (fontItalic) {
					Matrix3 transform;
					sl_real ratio = 0.2f;
					transform.m00 = fw; transform.m10 = -ratio * fh; transform.m20 = ratio * fh + fx;
					transform.m01 = 0; transform.m11 = fh; transform.m21 = fy;
					transform.m02 = 0; transform.m12 = 0; transform.m22 = 1;
					drawTexture(transform, texture, fac.region, dp, color);
				} else {
					drawTexture(Rectangle(fx, fy, fxn, fy + fh), texture, fac.region, dp, color);
				}
				fx = fxn;
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
	
	RenderCanvasState* state = m_state.ptr;
	
	if (brush.isNotNull()) {
		Rectangle rect = _rect;
		if (state->flagClipRect) {
			if (!(state->clipRect.intersectRectangle(rect, &rect))) {
				return;
			}
		}
		
		RenderCanvasProgramParam pp;
		pp.prepare(state, sl_true);
		
		RenderProgramScope<RenderCanvasProgramState> scope;
		if (scope.begin(m_engine.ptr, shared->getProgram(pp))) {
			Matrix3& mat = scope->transform;
			mat.m00 = rect.getWidth(); mat.m10 = 0; mat.m20 = rect.left;
			mat.m10 = 0; mat.m11 = rect.getHeight(); mat.m21 = rect.top;
			mat.m20 = 0; mat.m21 = 0; mat.m22 = 1;
			pp.applyToProgramState(scope.getState(), mat);
			mat *= state->matrix;
			mat *= m_matViewport;
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

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& _rectSrc, const DrawParam& param, const Color4f& color)
{
	
	_RenderCanvas_Shared* shared = _RenderCanvas_getShared();
	if (!shared) {
		return;
	}
	
	RenderCanvasState* state = m_state.ptr;
	
	Rectangle rectSrc = _rectSrc;
	sl_real sw = (sl_real)(texture->getWidth());
	sl_real sh = (sl_real)(texture->getHeight());
	rectSrc.left /= sw;
	rectSrc.top /= sh;
	rectSrc.right /= sw;
	rectSrc.bottom /= sh;
	
	RenderCanvasProgramParam pp;
	pp.prepare(state, sl_false);
	pp.flagUseTexture = sl_true;
	if (param.useColorMatrix) {
		pp.flagUseColorFilter = sl_true;
	}
	
	RenderProgramScope<RenderCanvasProgramState> scope;
	if (scope.begin(m_engine.ptr, shared->getProgram(pp))) {
		pp.applyToProgramState(scope.getState(), transform);
		scope->texture = texture;
		scope->transform = transform * state->matrix * m_matViewport;
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

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, const DrawParam& param)
{
	drawTexture(transform, texture, rectSrc, param, Color4f(1, 1, 1, 1));
}

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const Rectangle& rectSrc, sl_real alpha)
{
	drawTexture(transform, texture, rectSrc, DrawParam(), Color4f(1, 1, 1, alpha));
}

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const DrawParam& param, const Color4f& color)
{
	if (texture.isNotNull()) {
		drawTexture(transform, texture, Rectangle(0, 0, (sl_real)(texture->getWidth()), (sl_real)(texture->getHeight())), param, color);
	}
}

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, const DrawParam& param)
{
	drawTexture(transform, texture, param, Color4f(1, 1, 1, 1));
}

void RenderCanvas::drawTexture(const Matrix3& transform, const Ref<Texture>& texture, sl_real alpha)
{
	drawTexture(transform, texture, DrawParam(), Color4f(1, 1, 1, alpha));
}

void RenderCanvas::drawTexture(const Rectangle& _rectDst, const Ref<Texture>& texture, const Rectangle& _rectSrc, const DrawParam& param, const Color4f& color)
{
	
	_RenderCanvas_Shared* shared = _RenderCanvas_getShared();
	if (!shared) {
		return;
	}
	
	RenderCanvasState* state = m_state.ptr;
	
	Rectangle rectDst = _rectDst;
	Rectangle rectSrc = _rectSrc;
	sl_real sw = (sl_real)(texture->getWidth());
	sl_real sh = (sl_real)(texture->getHeight());
	if (state->flagClipRect) {
		Rectangle rectIntersectClip;
		if (state->clipRect.intersectRectangle(rectDst, &rectIntersectClip)) {
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
	pp.prepare(state, sl_true);
	pp.flagUseTexture = sl_true;
	if (param.useColorMatrix) {
		pp.flagUseColorFilter = sl_true;
	}
	
	RenderProgramScope<RenderCanvasProgramState> scope;
	if (scope.begin(m_engine.ptr, shared->getProgram(pp))) {
		scope->texture = texture;
		Matrix3& mat = scope->transform;
		mat.m00 = rectDst.getWidth(); mat.m10 = 0; mat.m20 = rectDst.left;
		mat.m01 = 0; mat.m11 = rectDst.getHeight(); mat.m21 = rectDst.top;
		mat.m02 = 0; mat.m12 = 0; mat.m22 = 1;
		pp.applyToProgramState(scope.getState(), mat);
		mat *= state->matrix;
		mat *= m_matViewport;
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
