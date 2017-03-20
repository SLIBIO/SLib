/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#include "../../../inc/slib/render/program.h"

#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/opengl.h"

namespace slib
{

/*******************************
		RenderProgram
*******************************/
	
	SLIB_DEFINE_OBJECT(RenderProgramInstance, RenderBaseObjectInstance)
	
	RenderProgramInstance::RenderProgramInstance()
	{
	}
	
	RenderProgramInstance::~RenderProgramInstance()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(RenderProgram, RenderBaseObject)
	
	RenderProgram::RenderProgram()
	{
	}
	
	RenderProgram::~RenderProgram()
	{
	}
	
	sl_bool RenderProgram::onInit(RenderEngine* engine, RenderProgramState* state)
	{
		return sl_true;
	}
	
	sl_bool RenderProgram::onPreRender(RenderEngine* engine, RenderProgramState* state)
	{
		return sl_true;
	}
	
	void RenderProgram::onPostRender(RenderEngine* engine, RenderProgramState* state)
	{
	}
	
	String RenderProgram::getGLSLVertexShader(RenderEngine* engine)
	{
		return sl_null;
	}
	
	String RenderProgram::getGLSLFragmentShader(RenderEngine* engine)
	{
		return sl_null;
	}
	
	Ref<RenderProgramInstance> RenderProgram::getInstance(RenderEngine* engine)
	{
		return Ref<RenderProgramInstance>::from(RenderBaseObject::getInstance(engine));
	}
	
	
/*******************************
 		RenderProgramState
*******************************/
	
	RenderProgramState::RenderProgramState()
	{
	}
	
	RenderProgramState::~RenderProgramState()
	{
	}
	
	void RenderProgramState::setUniformFloatValue(sl_int32 uniformLocation, float value)
	{
		gl_engine->setUniformFloatValue(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformFloatArray(sl_int32 uniformLocation, const float *arr, sl_uint32 n)
	{
		gl_engine->setUniformFloatArray(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformIntValue(sl_int32 uniformLocation, sl_int32 value)
	{
		gl_engine->setUniformIntValue(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformIntArray(sl_int32 uniformLocation, const sl_int32 *arr, sl_uint32 n)
	{
		gl_engine->setUniformIntArray(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformFloat2Value(sl_int32 uniformLocation, const Vector2& value)
	{
		gl_engine->setUniformFloat2Value(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformFloat2Array(sl_int32 uniformLocation, const Vector2* arr, sl_uint32 n)
	{
		gl_engine->setUniformFloat2Array(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformFloat3Value(sl_int32 uniformLocation, const Vector3& value)
	{
		gl_engine->setUniformFloat3Value(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformFloat3Array(sl_int32 uniformLocation, const Vector3* arr, sl_uint32 n)
	{
		gl_engine->setUniformFloat3Array(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformFloat4Value(sl_int32 uniformLocation, const Vector4& value)
	{
		gl_engine->setUniformFloat4Value(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformFloat4Array(sl_int32 uniformLocation, const Vector4* arr, sl_uint32 n)
	{
		gl_engine->setUniformFloat4Array(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformMatrix3Value(sl_int32 uniformLocation, const Matrix3& value)
	{
		gl_engine->setUniformMatrix3Value(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformMatrix3Array(sl_int32 uniformLocation, const Matrix3* arr, sl_uint32 n)
	{
		gl_engine->setUniformMatrix3Array(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformMatrix4Value(sl_int32 uniformLocation, const Matrix4& value)
	{
		gl_engine->setUniformMatrix4Value(uniformLocation, value);
	}
	
	void RenderProgramState::setUniformMatrix4Array(sl_int32 uniformLocation, const Matrix4* arr, sl_uint32 n)
	{
		gl_engine->setUniformMatrix4Array(uniformLocation, arr, n);
	}
	
	void RenderProgramState::setUniformTexture(sl_int32 uniformLocation, const Ref<Texture>& texture, sl_reg sampler)
	{
		gl_engine->applyTexture(texture, sampler);
		gl_engine->setUniformTextureSampler(uniformLocation, (sl_uint32)sampler);
	}
	
	void RenderProgramState::setUniformTextureArray(sl_int32 uniformLocation, const Ref<Texture>* textures, const sl_reg* samplers, sl_uint32 n)
	{
		for (sl_uint32 i = 0; i < n; i++) {
			gl_engine->applyTexture(textures[i], samplers[i]);
		}
		gl_engine->setUniformTextureSamplerArray(uniformLocation, samplers, n);
	}
	
/*******************************
	_RenderProgramTemplate
*******************************/
	
	sl_bool _RenderProgramTemplate::onInit(RenderEngine* _engine, RenderProgramState* _state)
	{
		RenderEngineType type = _engine->getEngineType();
		
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			_RenderProgramStateTemplate* state = (_RenderProgramStateTemplate*)_state;
			
			sl_uint32 program = state->gl_program;
			_RenderProgramStateItem* item = state->items;
			sl_int32 i = 0;
			while (item->gl_name) {
				if (item->type == 1) {
					item->gl_location = engine->getUniformLocation(program, item->gl_name);
				} else if (item->type == 2) {
					item->gl_location = engine->getAttributeLocation(program, item->gl_name);
					if (state->_indexFirstAttribute < 0) {
						state->_indexFirstAttribute = i;
					}
					state->_indexLastAttribute = i;
				}
				item++;
				i++;
			}
			return sl_true;
		}
		
		return sl_false;
	}
	
	sl_bool _RenderProgramTemplate::onPreRender(RenderEngine* _engine, RenderProgramState* _state)
	{
		RenderEngineType type = _engine->getEngineType();
		
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			_RenderProgramStateTemplate* state = (_RenderProgramStateTemplate*)_state;
			
			sl_int32 i = state->_indexFirstAttribute;
			
			if (i >= 0) {
				sl_int32 n = state->_indexLastAttribute;
				_RenderProgramStateItem* items = state->items;
				for (; i <= n; i++) {
					if (items[i].type == 2 && items[i].gl_location >= 0) {
						switch (items[i].attrType) {
							case 0:
								engine->setVertexFloatArrayAttribute(items[i].gl_location, items[i].attrOffset, items[i].attrCount, state->_sizeVertexData);
								break;
							case 1:
								engine->setVertexInt8ArrayAttribute(items[i].gl_location, items[i].attrOffset, items[i].attrCount, state->_sizeVertexData);
								break;
							case 2:
								engine->setVertexUint8ArrayAttribute(items[i].gl_location, items[i].attrOffset, items[i].attrCount, state->_sizeVertexData);
								break;
							case 3:
								engine->setVertexInt16ArrayAttribute(items[i].gl_location, items[i].attrOffset, items[i].attrCount, state->_sizeVertexData);
								break;
							case 4:
								engine->setVertexUint16ArrayAttribute(items[i].gl_location, items[i].attrOffset, items[i].attrCount, state->_sizeVertexData);
								break;
						}
					}
				}
			}
			return sl_true;
		}
		
		return sl_false;
	}
	
	void _RenderProgramTemplate::onPostRender(RenderEngine* _engine, RenderProgramState* _state)
	{
		RenderEngineType type = _engine->getEngineType();
		
		if (type == RenderEngineType::OpenGL_ES || type == RenderEngineType::OpenGL) {
			
			GLRenderEngine* engine = (GLRenderEngine*)_engine;
			_RenderProgramStateTemplate* state = (_RenderProgramStateTemplate*)_state;
			
			sl_int32 i = state->_indexFirstAttribute;
			
			if (i >= 0) {
				sl_int32 n = state->_indexLastAttribute;
				_RenderProgramStateItem* items = state->items;
				for (; i <= n; i++) {
					if (items[i].type == 2 && items[i].gl_location >= 0) {
						engine->disableVertexArrayAttribute(items[i].gl_location);
					}
				}
			}
		}
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
									   uniform vec4 u_Color;
									   uniform sampler2D u_Texture;
									   varying vec2 v_TexCoord;
									   void main() {
										   vec4 colorTexture = texture2D(u_Texture, v_TexCoord);
										   gl_FragColor = colorTexture * u_Color;
									   }
									   );
		return source;
	}
	
/*******************************
 RenderProgram2D_PositionTextureYUV
*******************************/
	String RenderProgram2D_PositionTextureYUV::getGLSLFragmentShader(RenderEngine* engine)
	{
		String source = SLIB_STRINGIFY(
									   uniform vec4 u_Color;
									   uniform sampler2D u_Texture;
									   varying vec2 v_TexCoord;
									   void main() {
										   vec4 YUV = texture2D(u_Texture, v_TexCoord);
										   float R = YUV.r + 1.370705*(YUV.b-0.5);
										   float G = YUV.r - 0.698001*(YUV.g-0.5) - 0.337633*(YUV.b-0.5);
										   float B = YUV.r + 1.732446*(YUV.g-0.5);
										   gl_FragColor = vec4(R, G, B, YUV.a) * u_Color;
									   }
									   );
		return source;
	}
	
/*******************************
 RenderProgram2D_PositionTextureOES
*******************************/
	String RenderProgram2D_PositionTextureOES::getGLSLFragmentShader(RenderEngine* engine)
	{
		String source = "#extension GL_OES_EGL_image_external : require\n";
		source += SLIB_STRINGIFY(
								 precision mediump float;
								 uniform vec4 u_Color;
								 uniform samplerExternalOES u_Texture;
								 varying vec2 v_TexCoord;
								 void main() {
									 vec4 colorTexture = texture2D(u_Texture, v_TexCoord);
									 gl_FragColor = colorTexture * u_Color;
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
									   uniform vec4 u_Color;
									   attribute vec2 a_Position;
									   attribute vec4 a_Color;
									   varying vec4 v_Color;
									   void main() {
										   vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
										   gl_Position = vec4(P.x, P.y, 0.0, 1.0);
										   v_Color = a_Color * u_Color;
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
									   uniform vec4 u_Color;
									   void main() {
										   gl_FragColor = u_Color;
									   }
									   );
		return source;
	}
	
	
/*******************************
 RenderProgram3D_PositionNormalColor
*******************************/
	String RenderProgram3D_PositionNormalColor::getGLSLVertexShader(RenderEngine* engine)
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
	
	String RenderProgram3D_PositionNormalColor::getGLSLFragmentShader(RenderEngine* engine)
	{
		String source = SLIB_STRINGIFY(
									   varying vec4 v_Color;
									   void main() {
										   gl_FragColor = v_Color;
									   }
									   );
		return source;
	}
	
/*******************************
 RenderProgram3D_PositionColor
*******************************/
	String RenderProgram3D_PositionColor::getGLSLVertexShader(RenderEngine* engine)
	{
		String source = SLIB_STRINGIFY(
									   uniform mat4 u_Transform;
									   uniform vec4 u_Color;
									   attribute vec3 a_Position;
									   attribute vec4 a_Color;
									   varying vec4 v_Color;
									   void main() {
										   vec4 P = vec4(a_Position, 1.0) * u_Transform;
										   vec4 C = u_Color * a_Color;
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
	
/*******************************
 RenderProgram3D_PositionNormalTexture
*******************************/
	String RenderProgram3D_PositionNormalTexture::getGLSLVertexShader(RenderEngine* engine)
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
	
	String RenderProgram3D_PositionNormalTexture::getGLSLFragmentShader(RenderEngine* engine)
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
									   uniform vec4 u_Color;
									   varying vec2 v_TexCoord; 
									   void main() { 
										   vec4 colorTexture = texture2D(u_Texture, v_TexCoord);
										   gl_FragColor = u_Color * colorTexture;
									   }
									   );
		return source;
	}
	
/*******************************
 RenderProgram3D_PositionNormal
*******************************/
	String RenderProgram3D_PositionNormal::getGLSLVertexShader(RenderEngine* engine)
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
	
	String RenderProgram3D_PositionNormal::getGLSLFragmentShader(RenderEngine* engine)
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
									   uniform vec4 u_Color;
									   void main() { 
										   gl_FragColor = u_Color;
									   }
									   );
		return source;
	}
	
}
