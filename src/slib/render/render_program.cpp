
#include "../../../inc/slib/render/program.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"

SLIB_RENDER_NAMESPACE_BEGIN

/*******************************
	RenderProgram
*******************************/

SLIB_DEFINE_OBJECT(RenderProgram, RenderBaseObject)

sl_bool RenderProgram::onInit(RenderEngine* engine, RenderProgramInfo* info)
{
	return sl_true;
}

sl_bool RenderProgram::onBeginProgram(RenderEngine* engine, RenderProgramInfo* info)
{
	return sl_true;
}

void RenderProgram::onEndProgram(RenderEngine* engine, RenderProgramInfo* info)
{
}

sl_bool RenderProgram::onPreRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive)
{
	return sl_true;
}

void RenderProgram::onPostRender(RenderEngine* engine, RenderProgramInfo* info, Primitive* primitive)
{
}

String RenderProgram::getGLSLVertexShader(RenderEngine* engine)
{
	return String::null();
}

String RenderProgram::getGLSLFragmentShader(RenderEngine* engine)
{
	return String::null();
}

/*******************************
	RenderProgram2D
*******************************/
RenderProgram2D::RenderProgram2D()
{
	setTransform(Matrix3::identity());
	setTextureTransform(Matrix3::identity());

	setColor(Color::White);
	setAlpha(1.0f);
}

Ref<RenderProgramInfo> RenderProgram2D::create(RenderEngine* engine)
{
	Ref<RenderProgramInfo> ret;
	RenderEngineType type = engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		ret = new Info_GL;
	}
	return ret;
}

sl_bool RenderProgram2D::onInit(RenderEngine* _engine, RenderProgramInfo* _info)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		
		sl_uint32 program = info->program_GL;
		info->attrPosition = engine->getAttributeLocation(program, "a_Position");
		info->attrColor = engine->getAttributeLocation(program, "a_Color");
		info->attrTexCoord = engine->getAttributeLocation(program, "a_TexCoord");
		
		info->uniformTransform = engine->getUniformLocation(program, "u_Transform");
		
		info->uniformTextures[0] = engine->getUniformLocation(program, "u_Texture");
		info->uniformTextures[1] = engine->getUniformLocation(program, "u_Texture1");
		info->uniformTextures[2] = engine->getUniformLocation(program, "u_Texture2");
		info->uniformTextures[3] = engine->getUniformLocation(program, "u_Texture3");
		info->uniformTextures[4] = engine->getUniformLocation(program, "u_Texture4");
		info->uniformTextures[5] = engine->getUniformLocation(program, "u_Texture5");
		info->uniformTextures[6] = engine->getUniformLocation(program, "u_Texture6");
		info->uniformTextures[7] = engine->getUniformLocation(program, "u_Texture7");
		info->uniformTextures[8] = engine->getUniformLocation(program, "u_Texture8");
		info->uniformTextures[9] = engine->getUniformLocation(program, "u_Texture9");
		info->uniformTextures[10] = engine->getUniformLocation(program, "u_Texture10");
		info->uniformTextures[11] = engine->getUniformLocation(program, "u_Texture11");
		info->uniformTextures[12] = engine->getUniformLocation(program, "u_Texture12");
		info->uniformTextures[13] = engine->getUniformLocation(program, "u_Texture13");
		info->uniformTextures[14] = engine->getUniformLocation(program, "u_Texture14");
		info->uniformTextures[15] = engine->getUniformLocation(program, "u_Texture15");
		
		info->uniformTransformTexture = engine->getUniformLocation(program, "u_TransformTexture");
		
		info->uniformColor = engine->getUniformLocation(program, "u_Color");
		info->uniformAlpha = engine->getUniformLocation(program, "u_Alpha");
		
		return sl_true;
	}
	return sl_false;
}

sl_bool RenderProgram2D::onBeginProgram(RenderEngine* _engine, RenderProgramInfo* _info)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		
		sl_uint32 i;
		
		engine->setUniformMatrix3Value(info->uniformTransform, getTransform());
		
		for (i = 0; i < SLIB_RENDER_MAX_TEXTURE_SAMPLERS; i++) {
			if (info->uniformTextures[i] != -1) {
				engine->setUniformTextureSampler(info->uniformTextures[i], i);
				engine->applyTexture(i, m_textures[i]);
			}
		}
		engine->setUniformMatrix3Value(info->uniformTransformTexture, getTextureTransform());
		
		engine->setUniformFloat3Value(info->uniformColor, getColor());
		engine->setUniformFloatValue(info->uniformAlpha, getAlpha());
		
		return sl_true;
	}
	
	return sl_false;
}

Ref<Texture> RenderProgram2D::getTexture(sl_uint32 no)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		return m_textures[no];
	} else {
		return Ref<Texture>::null();
	}
}

Ref<Texture> RenderProgram2D::getTexture()
{
	return m_textures[0];
}

void RenderProgram2D::setTexture(sl_uint32 no, const Ref<Texture>& texture)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		m_textures[no] = texture;
	}
}

void RenderProgram2D::setTexture(const Ref<Texture>& texture)
{
	m_textures[0] = texture;
}

/*******************************
	RenderProgram2D_PositionTexture
*******************************/
String RenderProgram2D_PositionTexture::getGLSLVertexShader(RenderEngine* engine)
{
	String source;
	source = SLIB_STRINGIFY(
		uniform mat3 u_Transform;
		uniform mat3 u_TransformTexture;
		attribute vec2 a_Position;
		attribute vec2 a_TexCoord;
		varying vec2 v_TexCoord;
		void main() {
			vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
			gl_Position = vec4(P.x, P.y, 0.0, 1.0);
			vec3 t = vec3(a_TexCoord, 1.0) * u_TransformTexture;
			v_TexCoord = t.xy;
		}
	);
	return source;
}

String RenderProgram2D_PositionTexture::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform vec3 u_Color;
		uniform float u_Alpha;
		uniform sampler2D u_Texture;
		varying vec2 v_TexCoord; 
		void main() { 
			vec4 colorTexture = texture2D(u_Texture, v_TexCoord);
			vec4 C = vec4(u_Color, u_Alpha);
			gl_FragColor = colorTexture * C;
		}
	);
	return source;
}

sl_bool RenderProgram2D_PositionTexture::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_PositionTexture::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrTexCoord);
	}
}

/*******************************
	RenderProgram2D_PositionTextureYUV
*******************************/
String RenderProgram2D_PositionTextureYUV::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
	   uniform vec3 u_Color;
	   uniform float u_Alpha;
	   uniform sampler2D u_Texture;
	   varying vec2 v_TexCoord;
	   void main() {
		   vec4 YUV = texture2D(u_Texture, v_TexCoord);
		   float R = YUV.r + 1.370705*(YUV.b-0.5);
		   float G = YUV.r - 0.698001*(YUV.g-0.5) - 0.337633*(YUV.b-0.5);
		   float B = YUV.r + 1.732446*(YUV.g-0.5);
		   vec4 C = vec4(u_Color, u_Alpha);
		   gl_FragColor = vec4(R, G, B, YUV.a) * C;
	   }
	);
	return source;
}

/*******************************
	RenderProgram2D_PositionColor
*******************************/
String RenderProgram2D_PositionColor::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat3 u_Transform;
		uniform vec3 u_Color;
		uniform float u_Alpha;
		attribute vec2 a_Position;
		attribute vec4 a_Color;
		varying vec4 v_Color;
		void main() {
			vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
			gl_Position = vec4(P.x, P.y, 0.0, 1.0);
			vec4 C = vec4(u_Color, u_Alpha);
			v_Color = a_Color * C;
		}
	);
	return source;
}

String RenderProgram2D_PositionColor::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		varying vec4 v_Color;
		void main() {
			gl_FragColor = v_Color;
		}
	);
	return source;
}

sl_bool RenderProgram2D_PositionColor::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_PositionColor::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrColor);
	}
}

/*******************************
	RenderProgram2D_Position
*******************************/
String RenderProgram2D_Position::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat3 u_Transform;
		attribute vec2 a_Position;
		void main() {
			vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
			gl_Position = vec4(P.x, P.y, 0.0, 1.0);
		}
	);
	return source;
}

String RenderProgram2D_Position::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform vec3 u_Color;
		uniform float u_Alpha;
		void main() {
			gl_FragColor = vec4(u_Color, u_Alpha);
		}
	);
	return source;
}

sl_bool RenderProgram2D_Position::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_Position::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
	}
}

/*******************************
	RenderProgram3D
*******************************/
RenderProgram3D::RenderProgram3D()
{
	setDirectionalLight(Vector3(0.0f, -1.0f, 1.0f));
	setDiffuseColor(Color::White);
	setAmbientColor(Color::Black);
	setAlpha(1.0f);

	setTextureTransform(Matrix3::identity());	

	m_matrixModel = Matrix4::identity();
	m_matrixView = Matrix4::identity();
	m_matrixProjection = Matrix4::identity();
	m_matrixTransform = Matrix4::identity();
	m_flagValidMatrixTransform = sl_true;
	m_flagValidMatrixModelView = sl_false;
	m_flagValidMatrixModelViewIT = sl_false;
	m_flagValidMatrixViewProjection = sl_false;
}

Ref<RenderProgramInfo> RenderProgram3D::create(RenderEngine* engine)
{
	Ref<RenderProgramInfo> ret;
	RenderEngineType type = engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		ret = new Info_GL;
	}
	return ret;
}

sl_bool RenderProgram3D::onInit(RenderEngine* _engine, RenderProgramInfo* _info)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		
		sl_uint32 program = info->program_GL;
		
		info->attrPosition = engine->getAttributeLocation(program, "a_Position");
		info->attrNormal = engine->getAttributeLocation(program, "a_Normal");
		info->attrColor = engine->getAttributeLocation(program, "a_Color");
		info->attrTexCoord = engine->getAttributeLocation(program, "a_TexCoord");
		
		info->uniformMatrixModel = engine->getUniformLocation(program, "u_MatrixModel");
		info->uniformMatrixView = engine->getUniformLocation(program, "u_MatrixView");
		info->uniformMatrixProjection = engine->getUniformLocation(program, "u_MatrixProjection");
		info->uniformTransform = engine->getUniformLocation(program, "u_Transform");
		info->uniformMatrixModelView = engine->getUniformLocation(program, "u_MatrixModelView");
		info->uniformMatrixModelViewIT = engine->getUniformLocation(program, "u_MatrixModelViewIT");
		info->uniformMatrixViewProjection = engine->getUniformLocation(program, "u_MatrixViewProjection");
		
		info->uniformTextures[0] = engine->getUniformLocation(program, "u_Texture");
		info->uniformTextures[1] = engine->getUniformLocation(program, "u_Texture1");
		info->uniformTextures[2] = engine->getUniformLocation(program, "u_Texture2");
		info->uniformTextures[3] = engine->getUniformLocation(program, "u_Texture3");
		info->uniformTextures[4] = engine->getUniformLocation(program, "u_Texture4");
		info->uniformTextures[5] = engine->getUniformLocation(program, "u_Texture5");
		info->uniformTextures[6] = engine->getUniformLocation(program, "u_Texture6");
		info->uniformTextures[7] = engine->getUniformLocation(program, "u_Texture7");
		info->uniformTextures[8] = engine->getUniformLocation(program, "u_Texture8");
		info->uniformTextures[9] = engine->getUniformLocation(program, "u_Texture9");
		info->uniformTextures[10] = engine->getUniformLocation(program, "u_Texture10");
		info->uniformTextures[11] = engine->getUniformLocation(program, "u_Texture11");
		info->uniformTextures[12] = engine->getUniformLocation(program, "u_Texture12");
		info->uniformTextures[13] = engine->getUniformLocation(program, "u_Texture13");
		info->uniformTextures[14] = engine->getUniformLocation(program, "u_Texture14");
		info->uniformTextures[15] = engine->getUniformLocation(program, "u_Texture15");
		
		info->uniformTextureAlphas[0] = engine->getUniformLocation(program, "u_TextureAlpha0");
		info->uniformTextureAlphas[1] = engine->getUniformLocation(program, "u_TextureAlpha1");
		info->uniformTextureAlphas[2] = engine->getUniformLocation(program, "u_TextureAlpha2");
		info->uniformTextureAlphas[3] = engine->getUniformLocation(program, "u_TextureAlpha3");
		info->uniformTextureAlphas[4] = engine->getUniformLocation(program, "u_TextureAlpha4");
		info->uniformTextureAlphas[5] = engine->getUniformLocation(program, "u_TextureAlpha5");
		info->uniformTextureAlphas[6] = engine->getUniformLocation(program, "u_TextureAlpha6");
		info->uniformTextureAlphas[7] = engine->getUniformLocation(program, "u_TextureAlpha7");
		info->uniformTextureAlphas[8] = engine->getUniformLocation(program, "u_TextureAlpha8");
		info->uniformTextureAlphas[9] = engine->getUniformLocation(program, "u_TextureAlpha9");
		info->uniformTextureAlphas[10] = engine->getUniformLocation(program, "u_TextureAlpha10");
		info->uniformTextureAlphas[11] = engine->getUniformLocation(program, "u_TextureAlpha11");
		info->uniformTextureAlphas[12] = engine->getUniformLocation(program, "u_TextureAlpha12");
		info->uniformTextureAlphas[13] = engine->getUniformLocation(program, "u_TextureAlpha13");
		info->uniformTextureAlphas[14] = engine->getUniformLocation(program, "u_TextureAlpha14");
		info->uniformTextureAlphas[15] = engine->getUniformLocation(program, "u_TextureAlpha15");
		
		info->uniformTransformTexture = engine->getUniformLocation(program, "u_TransformTexture");
		
		info->uniformDirectionalLight = engine->getUniformLocation(program, "u_DirectionalLight");
		info->uniformDiffuseColor = engine->getUniformLocation(program, "u_DiffuseColor");
		info->uniformAmbientColor = engine->getUniformLocation(program, "u_AmbientColor");
		info->uniformAlpha = engine->getUniformLocation(program, "u_Alpha");
		return sl_true;
	}
	return sl_false;
}

sl_bool RenderProgram3D::onBeginProgram(RenderEngine* _engine, RenderProgramInfo* _info)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		
		sl_uint32 i;
		
		engine->setUniformMatrix4Value(info->uniformMatrixModel, getModelMatrix());
		engine->setUniformMatrix4Value(info->uniformMatrixView, getViewMatrix());
		engine->setUniformMatrix4Value(info->uniformMatrixProjection, getProjectionMatrix());
		engine->setUniformMatrix4Value(info->uniformTransform, getTransform());
		engine->setUniformMatrix4Value(info->uniformMatrixModelView, getModelViewMatrix());
		engine->setUniformMatrix4Value(info->uniformMatrixModelViewIT, getModelViewMatrixInverseTranspose());
		engine->setUniformMatrix4Value(info->uniformMatrixViewProjection, getViewProjectionMatrix());
		
		for (i = 0; i < SLIB_RENDER_MAX_TEXTURE_SAMPLERS; i++) {
			if (info->uniformTextures[i] != -1) {
				engine->setUniformTextureSampler(info->uniformTextures[i], i);
				engine->applyTexture(i, m_textures[i]);
				engine->setUniformFloatValue(info->uniformTextureAlphas[i], m_textureAlphas[i]);
			}
		}
		engine->setUniformMatrix3Value(info->uniformTransformTexture, getTextureTransform());
		
		engine->setUniformFloat3Value(info->uniformDirectionalLight, getDirectionalLight());
		engine->setUniformFloat3Value(info->uniformDiffuseColor, getDiffuseColor());
		engine->setUniformFloat3Value(info->uniformAmbientColor, getAmbientColor());
		engine->setUniformFloatValue(info->uniformAlpha, getAlpha());
		return sl_true;
	}
	return sl_false;
}

const Matrix4& RenderProgram3D::getModelMatrix()
{
	return m_matrixModel;
}

void RenderProgram3D::setModelMatrix(const Matrix4& t)
{
	m_matrixModel = t;
	m_flagValidMatrixTransform = sl_false;
	m_flagValidMatrixModelView = sl_false;
	m_flagValidMatrixModelViewIT = sl_false;
}

const Matrix4& RenderProgram3D::getViewMatrix()
{
	return m_matrixView;
}

void RenderProgram3D::setViewMatrix(const Matrix4& t)
{
	m_matrixView = t;
	m_flagValidMatrixTransform = sl_false;
	m_flagValidMatrixModelView = sl_false;
	m_flagValidMatrixModelViewIT = sl_false;
	m_flagValidMatrixViewProjection = sl_false;
}

const Matrix4& RenderProgram3D::getProjectionMatrix()
{
	return m_matrixProjection;
}

void RenderProgram3D::setProjectionMatrix(const Matrix4& t, sl_bool flagUpdateTransform)
{
	m_matrixProjection = t;
	m_flagValidMatrixTransform = sl_false;
	m_flagValidMatrixViewProjection = sl_false;
}

const Matrix4& RenderProgram3D::getTransform()
{
	if (!m_flagValidMatrixTransform) {
		Matrix4 t = m_matrixModel;
		t.multiply(m_matrixView);
		t.multiply(m_matrixProjection);
		m_matrixTransform = t;
		m_flagValidMatrixTransform = sl_true;
	}
	return m_matrixTransform;
}

void RenderProgram3D::setTransform(const Matrix4& t)
{
	m_flagValidMatrixTransform = sl_true;
	m_matrixTransform = t;
}

const Matrix4& RenderProgram3D::getModelViewMatrix()
{
	if (!m_flagValidMatrixModelView) {
		Matrix4 t = m_matrixModel;
		t.multiply(m_matrixView);
		m_matrixModelView = t;
		m_flagValidMatrixModelView = sl_true;
	}
	return m_matrixModelView;
}

void RenderProgram3D::setModelViewMatrix(const Matrix4& t)
{
	m_flagValidMatrixModelView = sl_true;
	m_matrixModelView = t;
}

const Matrix4& RenderProgram3D::getModelViewMatrixInverseTranspose()
{
	if (!m_flagValidMatrixModelViewIT) {
		Matrix4 t = getModelViewMatrix();
		t.makeInverseTranspose();
		m_matrixModelViewIT = t;
		m_flagValidMatrixModelViewIT = sl_true;
	}
	return m_matrixModelViewIT;
}

void RenderProgram3D::setModelViewMatrixInverseTranspose(const Matrix4& t)
{
	m_flagValidMatrixModelViewIT = sl_true;
	m_matrixModelViewIT = t;
}

const Matrix4& RenderProgram3D::getViewProjectionMatrix()
{
	if (!m_flagValidMatrixViewProjection) {
		Matrix4 t = m_matrixView;
		t.multiply(m_matrixProjection);
		m_matrixViewProjection = t;
		m_flagValidMatrixViewProjection = sl_true;
	}
	return m_matrixViewProjection;
}

void RenderProgram3D::setViewProjectionMatrix(const Matrix4& t)
{
	m_flagValidMatrixViewProjection = sl_true;
	m_matrixViewProjection = t;
}


Ref<Texture> RenderProgram3D::getTexture(sl_uint32 no)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		return m_textures[no];
	} else {
		return Ref<Texture>::null();
	}
}

Ref<Texture> RenderProgram3D::getTexture()
{
	return m_textures[0];
}

void RenderProgram3D::setTexture(sl_uint32 no, const Ref<Texture>& texture)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		m_textures[no] = texture;
	}
}

void RenderProgram3D::setTexture(const Ref<Texture>& texture)
{
	m_textures[0] = texture;
}

float RenderProgram3D::getTextureAlpha(sl_uint32 no)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		return m_textureAlphas[no];
	} else {
		return 0;
	}
}

void RenderProgram3D::setTextureAlpha(sl_uint32 no, float alpha)
{
	if (no < SLIB_RENDER_MAX_TEXTURE_SAMPLERS) {
		m_textureAlphas[no] = alpha;
	}
}

/*******************************
	RenderProgram3D_PositionNormalColor_Diffuse
*******************************/
String RenderProgram3D_PositionNormalColor_Diffuse::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform;
		uniform mat4 u_MatrixModelViewIT;
		uniform vec3 u_DirectionalLight;
		uniform vec3 u_DiffuseColor;
		uniform vec3 u_AmbientColor;
		uniform float u_Alpha;
		attribute vec3 a_Position;
		attribute vec3 a_Normal;
		attribute vec4 a_Color;
		varying vec4 v_Color;
		void main() {
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			vec4 N = vec4(a_Normal, 0.0) * u_MatrixModelViewIT;
			vec3 L = u_DirectionalLight;
			float diffuse = max(dot(N.xyz, L), 0.0);
			gl_Position = P;
			v_Color = vec4(diffuse * u_DiffuseColor + u_AmbientColor, u_Alpha) * a_Color;
		}
	);
	return source;
}

String RenderProgram3D_PositionNormalColor_Diffuse::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		varying vec4 v_Color; 
		void main() { 
			gl_FragColor = v_Color; 
		}
	);
	return source;
}

sl_bool RenderProgram3D_PositionNormalColor_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrNormal, VertexData, normal);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormalColor_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrNormal);
		engine->disableVertexArrayAttribute(info->attrColor);
	}
}

/*******************************
	RenderProgram3D_PositionColor
*******************************/
String RenderProgram3D_PositionColor::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform; 
		uniform vec3 u_DiffuseColor; 
		uniform float u_Alpha; 
		attribute vec3 a_Position; 
		attribute vec4 a_Color; 
		varying vec4 v_Color; 
		void main() { 
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			vec4 C = vec4(u_DiffuseColor, u_Alpha) * a_Color; 
			gl_Position = P; 
			v_Color = C; 
		}
	);
	return source;
}

String RenderProgram3D_PositionColor::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		varying vec4 v_Color; 
		void main() { 
			gl_FragColor = v_Color; 
		}
	);
	return source;
}

sl_bool RenderProgram3D_PositionColor::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionColor::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrColor);
	}
}

/*******************************
	RenderProgram3D_PositionNormalTexture_Diffuse
*******************************/
String RenderProgram3D_PositionNormalTexture_Diffuse::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform; 
		uniform mat4 u_MatrixModelViewIT; 
		uniform vec3 u_DirectionalLight; 
		uniform vec3 u_DiffuseColor; 
		uniform vec3 u_AmbientColor; 
		attribute vec3 a_Position; 
		attribute vec3 a_Normal; 
		attribute vec2 a_TexCoord; 
		varying vec2 v_TexCoord; 
		varying vec3 v_Color; 
		void main() { 
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			vec4 N = vec4(a_Normal, 0.0) * u_MatrixModelViewIT; 
			vec3 L = u_DirectionalLight; 
			float diffuse = max(dot(N.xyz, L), 0.0); 
			gl_Position = P; 
			v_Color = diffuse * u_DiffuseColor + u_AmbientColor; 
			v_TexCoord = a_TexCoord; 
		}
	);
	return source;
}

String RenderProgram3D_PositionNormalTexture_Diffuse::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform sampler2D u_Texture; 
		uniform float u_Alpha; 
		varying vec2 v_TexCoord; 
		varying vec3 v_Color; 
		void main() { 
			vec4 colorTexture = texture2D(u_Texture, v_TexCoord); 
			vec4 C = vec4(v_Color, u_Alpha);
			gl_FragColor = C * colorTexture; 
		}
	);
	return source;
}

sl_bool RenderProgram3D_PositionNormalTexture_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrNormal, VertexData, normal);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormalTexture_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrNormal);
		engine->disableVertexArrayAttribute(info->attrTexCoord);
	}
}

/*******************************
	RenderProgram3D_PositionTexture
*******************************/
String RenderProgram3D_PositionTexture::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform; 
		attribute vec3 a_Position; 
		attribute vec2 a_TexCoord; 
		varying vec2 v_TexCoord; 
		void main() { 
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			gl_Position = P; 
			v_TexCoord = a_TexCoord; 
		}
	);
	return source;
}

String RenderProgram3D_PositionTexture::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform sampler2D u_Texture; 
		uniform float u_Alpha; 
		uniform vec3 u_DiffuseColor;
		varying vec2 v_TexCoord; 
		void main() { 
			vec4 C = vec4(u_DiffuseColor, u_Alpha);
			vec4 colorTexture = texture2D(u_Texture, v_TexCoord); 
			gl_FragColor = C * colorTexture; 
		}
	);
	return source;
}

sl_bool RenderProgram3D_PositionTexture::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionTexture::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrTexCoord);
	}
}

/*******************************
	RenderProgram3D_PositionNormal_Diffuse
*******************************/
String RenderProgram3D_PositionNormal_Diffuse::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform; 
		uniform mat4 u_MatrixModelViewIT; 
		uniform vec3 u_DirectionalLight; 
		uniform vec3 u_DiffuseColor; 
		uniform vec3 u_AmbientColor; 
		attribute vec3 a_Position; 
		attribute vec3 a_Normal; 
		varying vec3 v_Color; 
		void main() { 
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			vec4 N = vec4(a_Normal, 0.0) * u_MatrixModelViewIT; 
			vec3 L = u_DirectionalLight; 
			float diffuse = max(dot(N.xyz, L), 0.0); 
			gl_Position = P; 
			v_Color = diffuse * u_DiffuseColor + u_AmbientColor; 
		}
	);
	return source;
}

String RenderProgram3D_PositionNormal_Diffuse::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform float u_Alpha; 
		varying vec3 v_Color; 
		void main() { 
			vec4 C = vec4(v_Color, u_Alpha);
			gl_FragColor = C; 
		}
	);
	return source;
}

sl_bool RenderProgram3D_PositionNormal_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrNormal, VertexData, normal);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormal_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
		engine->disableVertexArrayAttribute(info->attrNormal);
	}
}

/*******************************
	RenderProgram3D_Position
*******************************/
String RenderProgram3D_Position::getGLSLVertexShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform mat4 u_Transform; 
		attribute vec3 a_Position; 
		void main() { 
			vec4 P = vec4(a_Position, 1.0) * u_Transform; 
			gl_Position = P; 
		}
	);
	return source;
}

String RenderProgram3D_Position::getGLSLFragmentShader(RenderEngine* engine)
{
	String source = SLIB_STRINGIFY(
		uniform float u_Alpha; 
		uniform vec3 u_DiffuseColor; 
		void main() { 
			vec4 C = vec4(u_DiffuseColor, u_Alpha);
			gl_FragColor = C;
		}
	);
	return source;
}

sl_bool RenderProgram3D_Position::onPreRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, info->attrPosition, VertexData, position);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_Position::onPostRender(RenderEngine* _engine, RenderProgramInfo* _info, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		Info_GL* info = (Info_GL*)_info;
		engine->disableVertexArrayAttribute(info->attrPosition);
	}
}

SLIB_RENDER_NAMESPACE_END

