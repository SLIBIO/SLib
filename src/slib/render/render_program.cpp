
#include "../../../inc/slib/render/program.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"

SLIB_RENDER_NAMESPACE_BEGIN

/*******************************
	RenderProgram
*******************************/

SLIB_DEFINE_OBJECT(RenderProgramInstance, RenderBaseObjectInstance)

SLIB_DEFINE_OBJECT(RenderProgram, RenderBaseObject)

sl_bool RenderProgram::onInit(RenderEngine* engine, RenderProgramState* state)
{
	return sl_true;
}

sl_bool RenderProgram::onUpdate(RenderEngine* engine, RenderProgramState* state)
{
	return sl_true;
}

sl_bool RenderProgram::onPreRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive)
{
	return sl_true;
}

void RenderProgram::onPostRender(RenderEngine* engine, RenderProgramState* state, Primitive* primitive)
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

Ref<RenderProgramInstance> RenderProgram::getInstance(RenderEngine* engine)
{
	return Ref<RenderProgramInstance>::from(RenderBaseObject::getInstance(engine));
}

/*******************************
	RenderProgram2D
*******************************/

RenderProgramState2D::RenderProgramState2D()
: transform(Matrix3::identity()), textureTransform(Matrix3::identity()), color(1, 1, 1), alpha(1)
{
}

RenderProgram2D::RenderProgram2D()
{
}

sl_bool RenderProgram2D::onInit(RenderEngine* _engine, RenderProgramState* _state)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		
		sl_uint32 program = state->gl_program;
		state->gl_attrPosition = engine->getAttributeLocation(program, "a_Position");
		state->gl_attrColor = engine->getAttributeLocation(program, "a_Color");
		state->gl_attrTexCoord = engine->getAttributeLocation(program, "a_TexCoord");
		
		state->gl_uniformTransform = engine->getUniformLocation(program, "u_Transform");
		state->gl_uniformTexture = engine->getUniformLocation(program, "u_Texture");
		state->gl_uniformTextureTransform = engine->getUniformLocation(program, "u_TextureTransform");
		state->gl_uniformColor = engine->getUniformLocation(program, "u_Color");
		state->gl_uniformAlpha = engine->getUniformLocation(program, "u_Alpha");
		
		return sl_true;
		
	}
	
	return sl_false;
	
}

sl_bool RenderProgram2D::onUpdate(RenderEngine* _engine, RenderProgramState* _state)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		
		engine->setUniformMatrix3Value(state->gl_uniformTransform, state->transform);
		engine->setUniformTextureSampler(state->gl_uniformTexture, 0);
		engine->applyTexture(0, state->texture);
		engine->setUniformMatrix3Value(state->gl_uniformTextureTransform, state->textureTransform);
		engine->setUniformFloat3Value(state->gl_uniformColor, state->color);
		engine->setUniformFloatValue(state->gl_uniformAlpha, state->alpha);
		
		return sl_true;
	}
	
	return sl_false;
}

/*******************************
	RenderProgram2D_PositionTexture
*******************************/
String RenderProgram2D_PositionTexture::getGLSLVertexShader(RenderEngine* engine)
{
	String source;
	source = SLIB_STRINGIFY(
		uniform mat3 u_Transform;
		uniform mat3 u_TextureTransform;
		attribute vec2 a_Position;
		attribute vec2 a_TexCoord;
		varying vec2 v_TexCoord;
		void main() {
			vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
			gl_Position = vec4(P.x, P.y, 0.0, 1.0);
			vec3 t = vec3(a_TexCoord, 1.0) * u_TextureTransform;
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

sl_bool RenderProgram2D_PositionTexture::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_PositionTexture::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrTexCoord);
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

sl_bool RenderProgram2D_PositionColor::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_PositionColor::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrColor);
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

sl_bool RenderProgram2D_Position::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram2D_Position::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState2D* state = (RenderProgramState2D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
	}
}

/*******************************
	RenderProgram3D
*******************************/

RenderProgramState3D::RenderProgramState3D()
: transform(Matrix4::identity()), matrixModelViewIT(Matrix4::identity()),
directionalLight(Vector3(0.0f, -1.0f, 1.0f)), diffuseColor(1, 1, 1), ambientColor(0, 0, 0), alpha(1)
{
}

RenderProgram3D::RenderProgram3D()
{
}

sl_bool RenderProgram3D::onInit(RenderEngine* _engine, RenderProgramState* _state)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		
		sl_uint32 program = state->gl_program;
		
		state->gl_attrPosition = engine->getAttributeLocation(program, "a_Position");
		state->gl_attrNormal = engine->getAttributeLocation(program, "a_Normal");
		state->gl_attrColor = engine->getAttributeLocation(program, "a_Color");
		state->gl_attrTexCoord = engine->getAttributeLocation(program, "a_TexCoord");
		
		state->gl_uniformTransform = engine->getUniformLocation(program, "u_Transform");
		state->gl_uniformMatrixModelViewIT = engine->getUniformLocation(program, "u_MatrixModelViewIT");
		state->gl_uniformTexture = engine->getUniformLocation(program, "u_Texture");
		state->gl_uniformDirectionalLight = engine->getUniformLocation(program, "u_DirectionalLight");
		state->gl_uniformDiffuseColor = engine->getUniformLocation(program, "u_DiffuseColor");
		state->gl_uniformAmbientColor = engine->getUniformLocation(program, "u_AmbientColor");
		state->gl_uniformAlpha = engine->getUniformLocation(program, "u_Alpha");
		
		return sl_true;
		
	}
	
	return sl_false;
	
}

sl_bool RenderProgram3D::onUpdate(RenderEngine* _engine, RenderProgramState* _state)
{
	RenderEngineType type = _engine->getEngineType();
	
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		
		engine->setUniformMatrix4Value(state->gl_uniformTransform, state->transform);
		engine->setUniformMatrix4Value(state->gl_uniformMatrixModelViewIT, state->matrixModelViewIT);
		engine->setUniformTextureSampler(state->gl_uniformTexture, 0);
		engine->applyTexture(0, state->texture);
		engine->setUniformFloat3Value(state->gl_uniformDirectionalLight, state->directionalLight);
		engine->setUniformFloat3Value(state->gl_uniformDiffuseColor, state->diffuseColor);
		engine->setUniformFloat3Value(state->gl_uniformAmbientColor, state->ambientColor);
		engine->setUniformFloatValue(state->gl_uniformAlpha, state->alpha);
		
		return sl_true;
	}
	
	return sl_false;
	
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

sl_bool RenderProgram3D_PositionNormalColor_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrNormal, VertexData, normal);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormalColor_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrNormal);
		engine->disableVertexArrayAttribute(state->gl_attrColor);
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

sl_bool RenderProgram3D_PositionColor::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrColor, VertexData, color);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionColor::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrColor);
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

sl_bool RenderProgram3D_PositionNormalTexture_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrNormal, VertexData, normal);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormalTexture_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrNormal);
		engine->disableVertexArrayAttribute(state->gl_attrTexCoord);
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

sl_bool RenderProgram3D_PositionTexture::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrTexCoord, VertexData, texCoord);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionTexture::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrTexCoord);
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

sl_bool RenderProgram3D_PositionNormal_Diffuse::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrNormal, VertexData, normal);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_PositionNormal_Diffuse::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
		engine->disableVertexArrayAttribute(state->gl_attrNormal);
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

sl_bool RenderProgram3D_Position::onPreRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, state->gl_attrPosition, VertexData, position);
		return sl_true;
	}
	return sl_false;
}

void RenderProgram3D_Position::onPostRender(RenderEngine* _engine, RenderProgramState* _state, Primitive* primitive)
{
	RenderEngineType type = _engine->getEngineType();
	if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
		GLRenderEngine* engine = (GLRenderEngine*)_engine;
		RenderProgramState3D* state = (RenderProgramState3D*)_state;
		engine->disableVertexArrayAttribute(state->gl_attrPosition);
	}
}

SLIB_RENDER_NAMESPACE_END

