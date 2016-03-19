#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_IMPL
#define CHECKHEADER_SLIB_RENDER_OPENGL_IMPL

#include "../../../inc/slib/render/definition.h"

#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/scoped_pointer.h"
#include "../../../inc/slib/graphics/image.h"

#define STACK_BUFFER_COUNT 128
#define STACK_IMAGE_SIZE 16384

SLIB_RENDER_NAMESPACE_BEGIN

void GL_BASE::setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
{
	GL_ENTRY(glViewport)(x, y, width, height);
}

void GL_BASE::clear(const RenderClearParam& param)
{
	GLbitfield mask = 0;
	if (param.flagColor) {
		GL_ENTRY(glClearColor)(param.color.getRedF(), param.color.getGreenF(), param.color.getBlueF(), param.color.getAlphaF());
		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (param.flagDepth) {
#if defined(_OPENGL_IMPL)
		GL_ENTRY(glClearDepth)(param.depth);
#else
		GL_ENTRY(glClearDepthf)(param.depth);
#endif
		mask |= GL_DEPTH_BUFFER_BIT;
	}
	if (param.flagStencil) {
		GL_ENTRY(glClearStencil)(param.stencil);
		mask |= GL_STENCIL_BUFFER_BIT;
	}
	if (mask) {
		GL_ENTRY(glClear)(mask);
	}
}

void GL_BASE::clearColor(const Color& color)
{
	RenderClearParam param;
	param.flagColor = sl_true;
	param.color = color;
	param.flagDepth = sl_false;
	clear(param);
}

void GL_BASE::clearColorDepth(const Color& color, float depth)
{
	RenderClearParam param;
	param.flagColor = sl_true;
	param.color = color;
	param.flagDepth = sl_true;
	param.depth = depth;
	clear(param);
}

void GL_BASE::clearDepth(float depth)
{
	RenderClearParam param;
	param.flagColor = sl_false;
	param.flagDepth = sl_true;
	param.depth = depth;
	clear(param);
}

void GL_BASE::setDepthTest(sl_bool flag)
{
	if (flag) {
		GL_ENTRY(glEnable)(GL_DEPTH_TEST);
	} else {
		GL_ENTRY(glDisable)(GL_DEPTH_TEST);
	}
}

void GL_BASE::setDepthWriteEnabled(sl_bool flagEnableDepthWrite)
{
	GL_ENTRY(glDepthMask)(flagEnableDepthWrite ? GL_TRUE : GL_FALSE);
}

void GL_BASE::setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW)
{
	if (flagEnableCull) {
		GL_ENTRY(glEnable)(GL_CULL_FACE);
		GL_ENTRY(glCullFace)(GL_BACK);
		if (flagCullCCW) {
			GL_ENTRY(glFrontFace)(GL_CW);
		} else {
			GL_ENTRY(glFrontFace)(GL_CCW);
		}
	} else {
		GL_ENTRY(glDisable)(GL_CULL_FACE);
	}
}

static GLenum _GL_getBlendingOp(RenderBlendingOperation op)
{
	switch (op) {
		case RenderBlendingOperation::Add:
			return GL_FUNC_ADD;
		case RenderBlendingOperation::Subtract:
			return GL_FUNC_SUBTRACT;
		case RenderBlendingOperation::ReverseSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;
	}
	return GL_FUNC_ADD;
}

static GLenum _GL_getBlendingFactor(RenderBlendingFactor factor)
{
	switch (factor) {
		case RenderBlendingFactor::One:
			return GL_ONE;
		case RenderBlendingFactor::Zero:
			return GL_ZERO;
		case RenderBlendingFactor::SrcAlpha:
			return GL_SRC_ALPHA;
		case RenderBlendingFactor::OneMinusSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case RenderBlendingFactor::DstAlpha:
			return GL_DST_ALPHA;
		case RenderBlendingFactor::OneMinusDstAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case RenderBlendingFactor::SrcColor:
			return GL_SRC_COLOR;
		case RenderBlendingFactor::OneMinusSrcColor:
			return GL_ONE_MINUS_SRC_COLOR;
		case RenderBlendingFactor::DstColor:
			return GL_DST_COLOR;
		case RenderBlendingFactor::OneMinusDstColor:
			return GL_ONE_MINUS_DST_COLOR;
		case RenderBlendingFactor::SrcAlphaSaturate:
			return GL_SRC_ALPHA_SATURATE;
		case RenderBlendingFactor::Constant:
			return GL_CONSTANT_COLOR;
		case RenderBlendingFactor::OneMinusConstant:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case RenderBlendingFactor::ConstantAlpha:
			return GL_CONSTANT_ALPHA;
		case RenderBlendingFactor::OneMinusConstantAlpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
	}
	return GL_ZERO;
}

void GL_BASE::setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param)
{
	if (flagEnableBlending) {
		GL_ENTRY(glEnable)(GL_BLEND);
		GLenum op = _GL_getBlendingOp(param.operation);
		GLenum opAlpha = _GL_getBlendingOp(param.operationAlpha);
		if (op != opAlpha) {
			GL_ENTRY(glBlendEquationSeparate)(op, opAlpha);
		} else {
			GL_ENTRY(glBlendEquation)(op);
		}
		GLenum fSrc = _GL_getBlendingFactor(param.blendSrc);
		GLenum fDst = _GL_getBlendingFactor(param.blendDst);
		GLenum fSrcAlpha = _GL_getBlendingFactor(param.blendSrcAlpha);
		GLenum fDstAlpha = _GL_getBlendingFactor(param.blendDstAlpha);
		if (fSrc == fSrcAlpha && fDst == fDstAlpha) {
			GL_ENTRY(glBlendFunc)(fSrc, fDst);
		} else {
			GL_ENTRY(glBlendFuncSeparate)(fSrc, fDst, fSrcAlpha, fDstAlpha);
		}
		GL_ENTRY(glBlendColor)(param.blendConstant.x, param.blendConstant.y, param.blendConstant.z, param.blendConstant.w);
	} else {
		GL_ENTRY(glDisable)(GL_BLEND);
	}
}

void GL_BASE::setBlending(sl_bool flagEnableBlending)
{
	RenderBlendingParam param;
	setBlending(flagEnableBlending, param);
}

static sl_uint32 _GLES_createShader(GLenum type, const String& source)
{
	GLuint shader = GL_ENTRY(glCreateShader)(type);
	if (shader) {
		if (source.isNotEmpty()) {
			const GLchar* sz = source.getData();
			GLint len = (GLint)(source.getLength());
			GL_ENTRY(glShaderSource)(shader, 1, &sz, &len);
			GL_ENTRY(glCompileShader)(shader);
			GLint status = 0;
			GL_ENTRY(glGetShaderiv)(shader, GL_COMPILE_STATUS, &status);
			if (status != GL_FALSE) {
				return shader;
			} else {
				GLchar log[1025];
				GLsizei lenLog = 0;
				GL_ENTRY(glGetShaderInfoLog)(shader, 1024, &lenLog, log);
				log[lenLog] = 0;
				SLIB_LOG("OpenGL Compile Shader", String(log));
			}
		}
		GL_ENTRY(glDeleteShader)(shader);
	}
	return 0;
}

sl_uint32 GL_BASE::createVertexShader(const String& source)
{
	return _GLES_createShader(GL_VERTEX_SHADER, source);
}

sl_uint32 GL_BASE::createFragmentShader(const String& source)
{
	return _GLES_createShader(GL_FRAGMENT_SHADER, source);
}

void GL_BASE::deleteShader(sl_uint32 shader)
{
	if (shader) {
		GL_ENTRY(glDeleteShader)(shader);
	}
}

sl_uint32 GL_BASE::createProgram()
{
	GLuint program = GL_ENTRY(glCreateProgram)();
	return program;
}

void GL_BASE::attachShader(sl_uint32 program, sl_uint32 shader)
{
	if (program && shader) {
		GL_ENTRY(glAttachShader)(program, shader);
	}
}

sl_bool GL_BASE::linkProgram(sl_uint32 program)
{
	if (program) {
		GL_ENTRY(glLinkProgram)(program);
		GLint status = GL_FALSE;
		GL_ENTRY(glGetProgramiv)(program, GL_LINK_STATUS, &status);
		if (status != GL_FALSE) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_uint32 GL_BASE::createProgram(sl_uint32 vertexShader, sl_uint32 fragmentShader)
{
	sl_uint32 program = GL_ENTRY(glCreateProgram)();
	if (program) {
		attachShader(program, vertexShader);
		attachShader(program, fragmentShader);
		if (linkProgram(program)) {
			return program;
		}
		deleteProgram(program);
	}
	return 0;
}

void GL_BASE::useProgram(sl_uint32 program)
{
	if (program) {
		GL_ENTRY(glUseProgram)(program);
	}
}

void GL_BASE::deleteProgram(sl_uint32 program)
{
	if (program) {
		GL_ENTRY(glDeleteProgram)(program);
	}
}

static sl_uint32 _GLES_createBuffer(GLenum target, sl_bool flagStatic, sl_size size, const void* data)
{
	GLuint buffer = 0;
	GL_ENTRY(glGenBuffers)(1, &buffer);
	if (buffer) {
		GL_ENTRY(glBindBuffer)(target, buffer);
		GL_ENTRY(glBufferData)(target, size, data, flagStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		return buffer;
	}
	return 0;
}

static void _GLES_updateBuffer(GLenum target, sl_uint32 buffer, sl_size offset, const void* data, sl_size size)
{
	if (buffer) {
		GL_ENTRY(glBindBuffer)(target, buffer);
		GL_ENTRY(glBufferSubData)(target, buffer, size, data);
	}
}

sl_uint32 GL_BASE::createVertexBuffer(sl_bool flagStatic, sl_size size, const void* data)
{
	return _GLES_createBuffer(GL_ARRAY_BUFFER, flagStatic, size, data);
}

void GL_BASE::updateVertexBuffer(sl_uint32 buffer, sl_size offset, const void* data, sl_size size)
{
	_GLES_updateBuffer(GL_ARRAY_BUFFER, buffer, offset, data, size);
}

void GL_BASE::bindVertexBuffer(sl_uint32 buffer)
{
	GL_ENTRY(glBindBuffer)(GL_ARRAY_BUFFER, buffer);
}

void GL_BASE::unbindVertexBuffer()
{
	GL_ENTRY(glBindBuffer)(GL_ARRAY_BUFFER, 0);
}

sl_uint32 GL_BASE::createIndexBuffer(sl_bool flagStatic, sl_size size, const void* data)
{
	return _GLES_createBuffer(GL_ELEMENT_ARRAY_BUFFER, flagStatic, size, data);
}

void GL_BASE::updateIndexBuffer(sl_uint32 buffer, sl_size offset, const void* data, sl_size size)
{
	_GLES_updateBuffer(GL_ARRAY_BUFFER, buffer, offset, data, size);
}

void GL_BASE::deleteBuffer(sl_uint32 buffer)
{
	if (buffer) {
		GLuint b = buffer;
		GL_ENTRY(glDeleteBuffers)(1, &b);
	}
}

sl_int32 GL_BASE::getAttributeLocation(sl_uint32 program, const char* name)
{
	if (program) {
		GLint ret = GL_ENTRY(glGetAttribLocation)(program, name);
		return ret;
	}
	return -1;
}

static void _GLES_setVertexArrayAttribute(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
{
	if (attributeLocation != -1) {
		GL_ENTRY(glEnableVertexAttribArray)(attributeLocation);
		GL_ENTRY(glVertexAttribPointer)(attributeLocation, countComponents, GL_FLOAT, flagDoNormalize ? GL_TRUE : GL_FALSE, strideBytes, data);
	}
}

#define _SL_SETVERTEXARRAY(t) \
	void GL_BASE::setVertex##t##ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize) \
	{ \
		_GLES_setVertexArrayAttribute(attributeLocation, data, countComponents, strideBytes, flagDoNormalize); \
	} \
	void GL_BASE::setVertex##t##ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize) \
	{ \
		_GLES_setVertexArrayAttribute(attributeLocation, (void*)offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize); \
	}

_SL_SETVERTEXARRAY(Float)
_SL_SETVERTEXARRAY(Int8)
_SL_SETVERTEXARRAY(Uint8)
_SL_SETVERTEXARRAY(Int16)
_SL_SETVERTEXARRAY(Uint16)

void GL_BASE::disableVertexArrayAttribute(sl_int32 attributeLocation)
{
	if (attributeLocation != -1) {
		GL_ENTRY(glDisableVertexAttribArray)(attributeLocation);
	}
}

sl_int32 GL_BASE::getUniformLocation(sl_uint32 program, const char* name)
{
	if (program) {
		GLint ret = GL_ENTRY(glGetUniformLocation)(program, name);
		return ret;
	}
	return -1;
}

void GL_BASE::setUniformFloatValue(sl_int32 uniformLocation, float value)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform1f)(uniformLocation, value);
	}
}

void GL_BASE::setUniformFloatArray(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform1fv)(uniformLocation, count, (float*)values);
	}
}

void GL_BASE::setUniformIntValue(sl_int32 uniformLocation, sl_int32 value)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform1i)(uniformLocation, value);
	}
}

void GL_BASE::setUniformIntArray(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform1iv)(uniformLocation, count, (GLint*)values);
	}
}

void GL_BASE::setUniformFloat2Value(sl_int32 uniformLocation, float v1, float v2)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform2f)(uniformLocation, v1, v2);
	}
}

void GL_BASE::setUniformFloat2Value(sl_int32 uniformLocation, const Vector2& v)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform2f)(uniformLocation, v.x, v.y);
	}
}

void GL_BASE::setUniformFloat2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform2fv)(uniformLocation, count, (float*)values);
	}
}

void GL_BASE::setUniformInt2Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform2i)(uniformLocation, v1, v2);
	}
}

void GL_BASE::setUniformInt2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform2iv)(uniformLocation, count, (GLint*)values);
	}
}

void GL_BASE::setUniformFloat3Value(sl_int32 uniformLocation, float v1, float v2, float v3)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform3f)(uniformLocation, v1, v2, v3);
	}
}

void GL_BASE::setUniformFloat3Value(sl_int32 uniformLocation, const Vector3& v)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform3f)(uniformLocation, v.x, v.y, v.z);
	}
}

void GL_BASE::setUniformFloat3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform3fv)(uniformLocation, count, (float*)values);
	}
}

void GL_BASE::setUniformInt3Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform3i)(uniformLocation, v1, v2, v3);
	}
}

void GL_BASE::setUniformInt3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform3iv)(uniformLocation, count, (GLint*)values);
	}
}

void GL_BASE::setUniformFloat4Value(sl_int32 uniformLocation, float v1, float v2, float v3, float v4)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform4f)(uniformLocation, v1, v2, v3, v4);
	}
}

void GL_BASE::setUniformFloat4Value(sl_int32 uniformLocation, const Vector4& v)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform4f)(uniformLocation, v.x, v.y, v.z, v.w);
	}
}

void GL_BASE::setUniformFloat4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform4fv)(uniformLocation, count, (float*)values);
	}
}

void GL_BASE::setUniformInt4Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3, sl_int32 v4)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform4i)(uniformLocation, v1, v2, v3, v4);
	}
}

void GL_BASE::setUniformInt4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform4iv)(uniformLocation, count, (GLint*)values);
	}
}

void GL_BASE::setUniformMatrix2Value(sl_int32 uniformLocation, const Matrix2& value)
{
	if (uniformLocation != -1) {
		float v[4];
		v[0] = value.m00;
		v[1] = value.m10;
		v[2] = value.m01;
		v[3] = value.m11;
		GL_ENTRY(glUniformMatrix2fv)(uniformLocation, 1, GL_FALSE, v);
	}
}

void GL_BASE::setUniformMatrix2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		SLIB_SCOPED_BUFFER(float, STACK_BUFFER_COUNT, mats, 4 * count);
		for (sl_uint32 i = 0; i < count; i++) {
			float* v = mats + i * 4;
			Matrix2& value = ((Matrix2*)(values))[i];
			v[0] = value.m00;
			v[1] = value.m10;
			v[2] = value.m01;
			v[3] = value.m11;
		}
		GL_ENTRY(glUniformMatrix2fv)(uniformLocation, count, GL_FALSE, mats);
	}
}

void GL_BASE::setUniformMatrix3Value(sl_int32 uniformLocation, const Matrix3& value)
{
	if (uniformLocation != -1) {
		float v[9];
		v[0] = value.m00;
		v[1] = value.m10;
		v[2] = value.m20;
		v[3] = value.m01;
		v[4] = value.m11;
		v[5] = value.m21;
		v[6] = value.m02;
		v[7] = value.m12;
		v[8] = value.m22;
		GL_ENTRY(glUniformMatrix3fv)(uniformLocation, 1, GL_FALSE, v);
	}
}

void GL_BASE::setUniformMatrix3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		SLIB_SCOPED_BUFFER(float, STACK_BUFFER_COUNT, mats, 9 * count);
		for (sl_uint32 i = 0; i < count; i++) {
			float* v = mats + i * 9;
			Matrix3& value = ((Matrix3*)(values))[i];
			v[0] = value.m00;
			v[1] = value.m10;
			v[2] = value.m20;
			v[3] = value.m01;
			v[4] = value.m11;
			v[5] = value.m21;
			v[6] = value.m02;
			v[7] = value.m12;
			v[8] = value.m22;
		}
		GL_ENTRY(glUniformMatrix3fv)(uniformLocation, count, GL_FALSE, mats);
	}
}

void GL_BASE::setUniformMatrix4Value(sl_int32 uniformLocation, const Matrix4& value)
{
	if (uniformLocation != -1) {
		float v[16];
		v[0] = value.m00;
		v[1] = value.m10;
		v[2] = value.m20;
		v[3] = value.m30;
		v[4] = value.m01;
		v[5] = value.m11;
		v[6] = value.m21;
		v[7] = value.m31;
		v[8] = value.m02;
		v[9] = value.m12;
		v[10] = value.m22;
		v[11] = value.m32;
		v[12] = value.m03;
		v[13] = value.m13;
		v[14] = value.m23;
		v[15] = value.m33;
		GL_ENTRY(glUniformMatrix4fv)(uniformLocation, 1, GL_FALSE, v);
	}
}

void GL_BASE::setUniformMatrix4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
{
	if (uniformLocation != -1) {
		SLIB_SCOPED_BUFFER(float, STACK_BUFFER_COUNT, mats, 16 * count);
		for (sl_uint32 i = 0; i < count; i++) {
			float* v = mats + i * 16;
			Matrix4& value = ((Matrix4*)(values))[i];
			v[0] = value.m00;
			v[1] = value.m10;
			v[2] = value.m20;
			v[3] = value.m30;
			v[4] = value.m01;
			v[5] = value.m11;
			v[6] = value.m21;
			v[7] = value.m31;
			v[8] = value.m02;
			v[9] = value.m12;
			v[10] = value.m22;
			v[11] = value.m32;
			v[12] = value.m03;
			v[13] = value.m13;
			v[14] = value.m23;
			v[15] = value.m33;
		}
		GL_ENTRY(glUniformMatrix4fv)(uniformLocation, count, GL_FALSE, mats);
	}
}

void GL_BASE::setUniformTextureSampler(sl_int32 uniformLocation, sl_uint32 textureNo)
{
	if (uniformLocation != -1) {
		GL_ENTRY(glUniform1i)(uniformLocation, textureNo);
	}
}

SLIB_INLINE static void _GLES_drawVertices(GLenum mode, sl_uint32 countVertices, sl_uint32 startIndex)
{
	GL_ENTRY(glDrawArrays)(mode, startIndex, countVertices);
}
SLIB_INLINE static void _GLES_drawIndices(GLenum mode, sl_uint32 countIndices, const sl_uint16* indices)
{
	GL_ENTRY(glBindBuffer)(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_ENTRY(glDrawElements)(mode, countIndices, GL_UNSIGNED_SHORT, indices);
}
SLIB_INLINE static void _GLES_drawIndices(GLenum mode, sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes)
{
	GL_ENTRY(glBindBuffer)(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	GL_ENTRY(glDrawElements)(mode, countIndices, GL_UNSIGNED_SHORT, (void*)offsetBytes);
	GL_ENTRY(glBindBuffer)(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#define _SL_DRAW(t1, t2) \
	void GL_BASE::draw##t1(sl_uint32 countVertices, sl_uint32 startIndex) \
	{ \
		_GLES_drawVertices(t2, countVertices, startIndex); \
	} \
	void GL_BASE::draw##t1(sl_uint32 countIndices, const sl_uint16* indices)  \
	{ \
		_GLES_drawIndices(t2, countIndices, indices); \
	} \
	void GL_BASE::draw##t1(sl_uint32 countIndices, sl_uint32 indexBuffer, sl_size offsetBytes) \
	{ \
		_GLES_drawIndices(t2, countIndices, indexBuffer, offsetBytes); \
	}

_SL_DRAW(Triangles, GL_TRIANGLES)
_SL_DRAW(TriangleStrip, GL_TRIANGLE_STRIP)
_SL_DRAW(TriangleFan, GL_TRIANGLE_FAN)
_SL_DRAW(Lines, GL_LINES)
_SL_DRAW(LineLoop, GL_LINE_LOOP)
_SL_DRAW(LineStrip, GL_LINE_STRIP)
_SL_DRAW(Points, GL_POINTS)

void GL_BASE::setLineWidth(float width)
{
	GL_ENTRY(glLineWidth)(width);
}

void GL_BASE::flush()
{
	GL_ENTRY(glFlush)();
}

sl_uint32 GL_BASE::createTexture2D(const BitmapData& bitmapData)
{
	GLuint texture = 0;
	GL_ENTRY(glGenTextures)(1, &texture);
	if (texture) {
		sl_uint32 width = bitmapData.width;
		sl_uint32 height = bitmapData.height;
		GL_ENTRY(glBindTexture)(GL_TEXTURE_2D, texture);
		if (bitmapData.format == BitmapFormat::RGBA && (bitmapData.pitch == 0 || bitmapData.pitch == width << 2)) {
			GL_ENTRY(glTexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData.data);
		} else {
			sl_uint32 size = width * height;
			SLIB_SCOPED_BUFFER(sl_uint8, STACK_IMAGE_SIZE, glImage, size << 2);
			BitmapData temp;
			temp.width = width;
			temp.height = height;
			temp.format = BitmapFormat::RGBA;
			temp.data = glImage;
			temp.pitch = width << 2;
			temp.copyPixelsFrom(bitmapData);
			GL_ENTRY(glTexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage);
		}
	}
	return texture;
}

sl_uint32 GL_BASE::createTexture2D(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	if (width > 0 && height > 0) {
		if (pixels) {
			BitmapData bitmapData(width, height, pixels, stride);
			return createTexture2D(bitmapData);
		} else {
			GLuint texture = 0;
			GL_ENTRY(glGenTextures)(1, &texture);
			if (texture) {
				GL_ENTRY(glBindTexture)(GL_TEXTURE_2D, texture);
				GL_ENTRY(glTexImage2D)(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sl_null);
			}
			return texture;
		}
	} else {
		return 0;
	}
}

sl_uint32 GL_BASE::createTexture2D(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 w, sl_uint32 h)
{
	if (bitmap.isNull()) {
		return 0;
	}
	if (w == 0 || h == 0) {
		return 0;
	}
	sl_uint32 bw = bitmap->getWidth();
	sl_uint32 bh = bitmap->getHeight();
	if (bw == 0 || bh == 0) {
		return 0;
	}
	if (x >= bw) {
		return 0;
	}
	if (y >= bh) {
		return 0;
	}
	if (x + w > bw) {
		return 0;
	}
	if (y + h > bh) {
		return 0;
	}
	if (bitmap->isImage()) {
		Ref<Image> image = Ref<Image>::from(bitmap);
		return createTexture2D(w, h, image->getColors(), image->getStride());
	} else {
		SLIB_SCOPED_BUFFER(sl_uint8, STACK_IMAGE_SIZE, glImage, (w * h) << 2);
		BitmapData temp;
		temp.width = w;
		temp.height = h;
		temp.format = BitmapFormat::RGBA;
		temp.data = glImage;
		temp.pitch = w << 2;
		if (bitmap->readPixels(x, y, temp)) {
			return createTexture2D(temp);
		}
		return 0;
	}
}

sl_uint32 GL_BASE::createTexture2D(const Ref<Bitmap>& bitmap)
{
	if (bitmap.isNull()) {
		return 0;
	}
	sl_uint32 w = bitmap->getWidth();
	sl_uint32 h = bitmap->getHeight();
	if (w == 0) {
		return 0;
	}
	if (h == 0) {
		return 0;
	}
	if (bitmap->isImage()) {
		Ref<Image> image = Ref<Image>::from(bitmap);
		return createTexture2D(w, h, image->getColors(), image->getStride());
	} else {
		SLIB_SCOPED_BUFFER(sl_uint8, STACK_IMAGE_SIZE, glImage, (w * h) << 2);
		BitmapData temp;
		temp.width = w;
		temp.height = h;
		temp.format = BitmapFormat::RGBA;
		temp.data = glImage;
		temp.pitch = w << 2;
		if (bitmap->readPixels(0, 0, temp)) {
			return createTexture2D(temp);
		}
		return 0;
	}
}

sl_uint32 GL_BASE::createTexture2DFromMemory(const void* mem, sl_size size)
{
	Ref<Image> image = Image::loadFromMemory(mem, size);
	return createTexture2D(image);
}

void GL_BASE::updateTexture2D(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData)
{
	sl_uint32 width = bitmapData.width;
	sl_uint32 height = bitmapData.height;
	if (bitmapData.format == BitmapFormat::RGBA && (bitmapData.pitch == 0 || bitmapData.pitch == width << 2)) {
		GL_ENTRY(glTexSubImage2D)(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData.data);
	} else {
		sl_uint32 size = width * height;
		SLIB_SCOPED_BUFFER(sl_uint8, STACK_IMAGE_SIZE, glImage, size << 2);
		BitmapData temp;
		temp.width = width;
		temp.height = height;
		temp.format = BitmapFormat::RGBA;
		temp.data = glImage;
		temp.pitch = width << 2;
		temp.copyPixelsFrom(bitmapData);
		GL_ENTRY(glTexSubImage2D)(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, glImage);
	}
}

void GL_BASE::updateTexture2D(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride)
{
	BitmapData bitmapData(width, height, pixels, stride);
	updateTexture2D(x, y, bitmapData);
}

void GL_BASE::updateTexture2D(sl_uint32 x, sl_uint32 y, sl_uint32 w, sl_uint32 h, const Ref<Bitmap>& bitmap, sl_uint32 bx, sl_uint32 by)
{
	if (bitmap.isNull()) {
		return;
	}
	if (w == 0 || h == 0) {
		return;
	}
	sl_uint32 bw = bitmap->getWidth();
	sl_uint32 bh = bitmap->getHeight();
	if (bw == 0 || bh == 0) {
		return;
	}
	if (bx >= bw) {
		return;
	}
	if (by >= bh) {
		return;
	}
	if (bx + w > bw) {
		return;
	}
	if (by + h > bh) {
		return;
	}
	if (bitmap->isImage()) {
		Ref<Image> image = Ref<Image>::from(bitmap);
		updateTexture2D(x, y, w, h, image->getColorsAt(bx, by), image->getStride());
	} else {
		SLIB_SCOPED_BUFFER(sl_uint8, STACK_IMAGE_SIZE, glImage, (w * h) << 2);
		BitmapData temp;
		temp.width = w;
		temp.height = h;
		temp.format = BitmapFormat::RGBA;
		temp.data = glImage;
		temp.pitch = w << 2;
		if (bitmap->readPixels(bx, by, temp)) {
			updateTexture2D(x, y, temp);
		}
	}
}

void GL_BASE::setActiveSampler(sl_uint32 textureNo)
{
	GL_ENTRY(glActiveTexture)(GL_TEXTURE0 + textureNo);
}

void GL_BASE::bindTexture2D(sl_uint32 texture)
{
	GL_ENTRY(glBindTexture)(GL_TEXTURE_2D, texture);
}

void GL_BASE::unbindTexture2D()
{
	GL_ENTRY(glBindTexture)(GL_TEXTURE_2D, 0);
}

static GLenum _GLES_getFilter(TextureFilterMode filter)
{
	switch (filter) {
		case TextureFilterMode::Linear:
			return GL_LINEAR;
		case TextureFilterMode::Point:
			return GL_NEAREST;
	}
	return GL_NONE;
}
void GL_BASE::setTexture2DFilterMode(TextureFilterMode minFilter, TextureFilterMode magFilter)
{
	GLenum f;
	f = _GLES_getFilter(minFilter);
	if (f != GL_NONE) {
		GL_ENTRY(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
	}
	f = _GLES_getFilter(magFilter);
	if (f != GL_NONE) {
		GL_ENTRY(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
	}
}

static GLenum _GLES_getWrap(TextureWrapMode wrap)
{
	switch (wrap) {
		case TextureWrapMode::Repeat:
			return GL_REPEAT;
		case TextureWrapMode::Mirror:
			return GL_MIRRORED_REPEAT;
		case TextureWrapMode::Clamp:
			return GL_CLAMP_TO_EDGE;
	}
	return GL_NONE;
}
void GL_BASE::setTexture2DWrapMode(TextureWrapMode wrapX, TextureWrapMode wrapY)
{
	GLenum f;
	f = _GLES_getWrap(wrapX);
	if (f != GL_NONE) {
		GL_ENTRY(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
	}
	f = _GLES_getWrap(wrapY);
	if (f != GL_NONE) {
		GL_ENTRY(glTexParameteri)(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
	}
}

void GL_BASE::deleteTexture(sl_uint32 texture)
{
	if (texture) {
		GLuint t = texture;
		GL_ENTRY(glDeleteTextures)(1, &t);
	}
}


/*****************************************
	OpenGL Engine
******************************************/
class GL_ENGINE : public GLRenderEngine
{
public:
	sl_uint64 m_threadUniqueId;
	
	class _BaseInstance;
	Queue< Ref<_BaseInstance> > m_listDirtyInstances;
	Queue< Ref<RenderBaseObject> > m_listUsedObjects;
	
	class _RenderProgramInstance;
	SafeRef<_RenderProgramInstance> m_currentProgram;

public:
	GL_ENGINE()
	{
		m_threadUniqueId = Thread::getCurrentThreadUniqueId();
	}

	~GL_ENGINE()
	{
		_release();
	}

public:
	void _release()
	{
		clearDirtyInstances();
	}

	sl_bool isCurrentEngine()
	{
		return m_threadUniqueId == Thread::getCurrentThreadUniqueId();
	}

	sl_bool _isValid()
	{
		return sl_true;
	}

	RenderEngineType getEngineType()
	{
#if defined(_OPENGL_ES_IMPL)
		return RenderEngineType::OpenGL_ES;
#else
		return RenderEngineType::OpenGL;
#endif
	}

	class _BaseInstance : public RenderBaseObjectInstance
	{
	public:
		sl_bool m_flagCleared;
		sl_bool m_flagUpdated;

	public:
		SLIB_INLINE _BaseInstance()
		{
			m_flagCleared = sl_false;
			m_flagUpdated = sl_false;
		}
		
	public:
		virtual void _update(RenderBaseObject* object) = 0;
		
		virtual void _free() = 0;

	public:
		void update(RenderBaseObject* object)
		{
			if (m_flagUpdated) {
				m_flagUpdated = sl_false;
				_update(object);
			}
		}
		void update()
		{
			if (m_flagUpdated) {
				m_flagUpdated = sl_false;
				Ref<RenderBaseObject> object = getObject();
				if (object.isNotNull()) {
					_update(object.ptr);
				}
			}
		}

		void clear()
		{
			m_flagCleared = sl_true;
		}

		void free()
		{
			if (m_flagCleared) {
				return;
			}
			m_flagCleared = sl_true;
			Ref<GL_ENGINE> engine = Ref<GL_ENGINE>::from(getEngine());
			if (engine.isNotNull() && engine->isCurrentEngine()) {
				_free();
			}
		}

	};

	void _onFreeInstance(RenderBaseObjectInstance* _instance)
	{
		_BaseInstance* instance = (_BaseInstance*)_instance;
		if (instance) {
			if (!isCurrentEngine()) {
				m_listDirtyInstances.push(instance);
			}
		}
	}

	Ref<_BaseInstance> getValidInstance(RenderBaseObject* object)
	{
		Ref<GL_ENGINE> engine = Ref<GL_ENGINE>::from(object->getEngine());
		if (engine.ptr == this) {
			Ref<_BaseInstance> instance = Ref<_BaseInstance>::from(object->getInstance());
			if (instance.isNotNull() && !(instance->m_flagCleared)) {
				instance->update(object);
				return instance;
			}
		}
		return Ref<_BaseInstance>::null();
	}

	void freeDirtyInstances()
	{
		m_listDirtyInstances.removeAll();
	}

	void clearDirtyInstances()
	{
		Ref<_BaseInstance> instance;
		while (m_listDirtyInstances.pop(&instance)) {
			instance->clear();
		}
		m_listDirtyInstances.removeAll();
	}

	void useObject(RenderBaseObject* object)
	{
		//m_listUsedObjects.push(object);
	}

	sl_bool _beginScene()
	{
		m_listUsedObjects.removeAll();
		freeDirtyInstances();
		return sl_true;
	}

	void _endScene()
	{
	}

	void _clear(const RenderClearParam& param)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::clear(param);
	}

	void _setViewport(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setViewport(x, y, width, height);
	}

	void _setDepthTest(sl_bool flag)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setDepthTest(flag);
	}

	void _setDepthWriteEnabled(sl_bool flagEnableDepthWrite)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setDepthWriteEnabled(flagEnableDepthWrite);
	}

	void _setCullFace(sl_bool flagEnableCull, sl_bool flagCullCCW = sl_true)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setCullFace(flagEnableCull, flagCullCCW);
	}

	void _setBlending(sl_bool flagEnableBlending, const RenderBlendingParam& param)
	{
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setBlending(flagEnableBlending, param);
	}

	class _RenderProgramInstance : public _BaseInstance
	{
	public:
		sl_uint32 vertexShader;
		sl_uint32 fragmentShader;
		sl_uint32 program;
		Ref<RenderProgramInfo> info;

	public:
		SLIB_INLINE _RenderProgramInstance()
		{
		}
		
		~_RenderProgramInstance()
		{
			free();
		}

	public:
		SLIB_INLINE Ref<RenderProgram> getProgram()
		{
			return Ref<RenderProgram>::from(getObject());
		}

		void _update(RenderBaseObject* object)
		{
		}

		void _free()
		{
			GL_BASE::deleteProgram(program);
			GL_BASE::deleteShader(vertexShader);
			GL_BASE::deleteShader(fragmentShader);
		}
	};
	
	String _convertShader(String glsl)
	{
#if defined(_OPENGL_IMPL)
#else
		if (! (glsl.startsWith("precision"))) {
			glsl = "precision highp float;" + glsl;
		}
#endif
		return glsl;
	}

	Ref<_RenderProgramInstance> _getProgram(RenderProgram* program)
	{
		Ref<_RenderProgramInstance> ret = Ref<_RenderProgramInstance>::from(getValidInstance(program));
		if (ret.isNull()) {
			String vsSource = _convertShader(program->getGLSLVertexShader(this));
			String fsSource = _convertShader(program->getGLSLFragmentShader(this));
			if (vsSource.isNotEmpty() && fsSource.isNotEmpty()) {
				sl_uint32 vs = GL_BASE::createVertexShader(vsSource);
				if (vs) {
					sl_uint32 fs = GL_BASE::createFragmentShader(fsSource);
					if (fs) {
						sl_uint32 ph = GL_BASE::createProgram(vs, fs);
						if (ph) {
							Ref<RenderProgramInfo> info = program->create(this);
							if (info.isNotNull()) {
								info->program_GL = ph;
								if (program->onInit(this, info.ptr)) {
									ret = new _RenderProgramInstance();
									if (ret.isNotNull()) {
										ret->program = ph;
										ret->vertexShader = vs;
										ret->fragmentShader = fs;
										ret->info = info;
										ret->linkObject(this, program);
										return ret;
									}
								}
							}
							GL_BASE::deleteProgram(ph);
						}
						GL_BASE::deleteShader(fs);
					}
					GL_BASE::deleteShader(vs);
				}
			}
		}
		return ret;
	}

	class _VertexBufferInstance : public _BaseInstance
	{
	public:
		sl_uint32 buffer;
		
		sl_size offsetUpdateStart;
		sl_size offsetUpdateEnd;

	public:
		SLIB_INLINE _VertexBufferInstance()
		{
		}
		
		~_VertexBufferInstance()
		{
			free();
		}

	public:
		SLIB_INLINE Ref<VertexBuffer> getVertexBuffer()
		{
			return Ref<VertexBuffer>::from(getObject());
		}

		void _update(RenderBaseObject* _object)
		{
			VertexBuffer* object = (VertexBuffer*)_object;
			if (offsetUpdateEnd > offsetUpdateStart && offsetUpdateEnd <= object->getSize())
			{
				GL_BASE::updateVertexBuffer(buffer, offsetUpdateStart, (object->getBuffer()) + offsetUpdateStart, offsetUpdateEnd - offsetUpdateStart);
			}
		}

		void _free()
		{
			GL_BASE::deleteBuffer(buffer);
		}
		
	};
	
	Ref<_VertexBufferInstance> _getVertexBuffer(VertexBuffer* buffer)
	{
		Ref<_VertexBufferInstance> ret = Ref<_VertexBufferInstance>::from(getValidInstance(buffer));
		if (ret.isNull()) {
			sl_uint32 obj = GL_BASE::createVertexBuffer(buffer->getStatic(), buffer->getSize(), buffer->getBuffer());
			if (obj) {
				ret = new _VertexBufferInstance();
				if (ret.isNotNull()) {
					ret->buffer = obj;
					ret->linkObject(this, buffer);
					return ret;
				}
				GL_BASE::deleteBuffer(obj);
			}
		}
		return ret;
	}

	void _onUpdateVertexBuffer(VertexBuffer* buffer, sl_size offset, sl_size size)
	{
		Ref<_VertexBufferInstance> _instance = Ref<_VertexBufferInstance>::from(buffer->getInstance());
		_VertexBufferInstance* instance = _instance.ptr;
		if (instance) {
			sl_size offsetEnd = offset + size;
			if (instance->m_flagUpdated) {
				if (instance->offsetUpdateStart > offset) {
					instance->offsetUpdateStart = offset;
				}
				if (instance->offsetUpdateEnd < offsetEnd) {
					instance->offsetUpdateEnd = offsetEnd;
				}
			} else {
				instance->offsetUpdateStart = offset;
				instance->offsetUpdateEnd = offsetEnd;
			}
			instance->m_flagUpdated = sl_true;
		}
	}

	sl_bool _linkVertexBuffer(VertexBuffer* buffer)
	{
		if (buffer) {
			if (isCurrentEngine()) {
				Ref<_VertexBufferInstance> t = _getVertexBuffer((VertexBuffer*)buffer);
				if (t.isNotNull()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	class _IndexBufferInstance : public _BaseInstance
	{
	public:
		sl_uint32 buffer;
		
		sl_size offsetUpdateStart;
		sl_size offsetUpdateEnd;
		
	public:
		SLIB_INLINE _IndexBufferInstance()
		{
		}
		
		~_IndexBufferInstance()
		{
			free();
		}

	public:
		SLIB_INLINE Ref<IndexBuffer> getIndexBuffer()
		{
			return Ref<IndexBuffer>::from(getObject());
		}
		
		void _update(RenderBaseObject* _object)
		{
			IndexBuffer* object = (IndexBuffer*)_object;
			if (offsetUpdateEnd > offsetUpdateStart && offsetUpdateEnd <= object->getSize())
			{
				GL_BASE::updateIndexBuffer(buffer, offsetUpdateStart, (object->getBuffer()) + offsetUpdateStart, offsetUpdateEnd - offsetUpdateStart);
			}
		}

		void _free()
		{
			GL_BASE::deleteBuffer(buffer);
		}

	};
	
	Ref<_IndexBufferInstance> _getIndexBuffer(IndexBuffer* buffer)
	{
		Ref<_IndexBufferInstance> ret = Ref<_IndexBufferInstance>::from(getValidInstance(buffer));
		if (ret.isNull()) {
			sl_uint32 obj = GL_BASE::createIndexBuffer(buffer->getStatic(), buffer->getSize(), buffer->getBuffer());
			if (obj) {
				ret = new _IndexBufferInstance();
				if (ret.isNotNull()) {
					ret->buffer = obj;
					ret->linkObject(this, buffer);
					return ret;
				}
				GL_BASE::deleteBuffer(obj);
			}
		}
		return ret;
	}

	void _onUpdateIndexBuffer(IndexBuffer* buffer, sl_size offset, sl_size size)
	{
		Ref<_IndexBufferInstance> _instance = Ref<_IndexBufferInstance>::from(buffer->getInstance());
		_IndexBufferInstance* instance = _instance.ptr;
		if (instance) {
			sl_size offsetEnd = offset + size;
			if (instance->m_flagUpdated) {
				if (instance->offsetUpdateStart > offset) {
					instance->offsetUpdateStart = offset;
				}
				if (instance->offsetUpdateEnd < offsetEnd) {
					instance->offsetUpdateEnd = offsetEnd;
				}
			} else {
				instance->offsetUpdateStart = offset;
				instance->offsetUpdateEnd = offsetEnd;
			}
			instance->m_flagUpdated = sl_true;
		}
	}

	sl_bool _linkIndexBuffer(IndexBuffer* buffer)
	{
		if (buffer) {
			if (isCurrentEngine()) {
				Ref<_IndexBufferInstance> t = _getIndexBuffer((IndexBuffer*)buffer);
				if (t.isNotNull()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	class _TextureInstance : public _BaseInstance
	{
	public:
		sl_uint32 texture;
		
		Rectanglei rectUpdate;

	public:
		SLIB_INLINE _TextureInstance()
		{
		}
		
		~_TextureInstance()
		{
			free();
		}

	public:
		SLIB_INLINE Ref<_TextureInstance> getTexture()
		{
			return Ref<_TextureInstance>::from(getObject());
		}

		void _update(RenderBaseObject* _object)
		{
			GL_BASE::bindTexture2D(texture);
			Texture* object = (Texture*)_object;
			GL_BASE::updateTexture2D(
				rectUpdate.left, rectUpdate.top, rectUpdate.getWidth(), rectUpdate.getHeight()
				, object->getSource(), rectUpdate.left, rectUpdate.top);
			if (object->isFreeSourceOnUpdate()) {
				object->freeSource();
			}
		}

		void _free()
		{
			GL_BASE::deleteTexture(texture);
		}
		
	};
	
	Ref<_TextureInstance> _getTexture(Texture* texture)
	{
		Ref<_TextureInstance> ret = Ref<_TextureInstance>::from(getValidInstance(texture));
		if (ret.isNull()) {
			sl_uint32 obj = GL_BASE::createTexture2D(texture->getSource());;
			if (texture->isFreeSourceOnUpdate()) {
				texture->freeSource();
			}
			if (obj) {
				ret = new _TextureInstance();
				if (ret.isNotNull()) {
					ret->texture = obj;
					ret->linkObject(this, texture);
					return ret;
				}
				GL_BASE::deleteTexture(obj);
			}
		}
		return ret;
	}

	void _onUpdateTexture(Texture* texture, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		if (width == 0 || height == 0) {
			return;
		}
		Ref<_TextureInstance> _instance = Ref<_TextureInstance>::from(texture->getInstance());
		_TextureInstance* instance = _instance.ptr;
		if (instance) {
			if (instance->m_flagUpdated) {
				instance->rectUpdate.mergeRectangle(Rectanglei(x, y, x + width, y + height));
			} else {
				instance->rectUpdate = Rectanglei(x, y, x + width, y + height);
			}
			instance->m_flagUpdated = sl_true;
		}
	}

	void _applyTexture(const void* _samplerNo, Texture* texture)
	{
		sl_uint32 samplerNo = (sl_uint32)(sl_reg)(_samplerNo);
		if (!texture) {
			return;
		}
		if (!isCurrentEngine()) {
			return;
		}
		GL_BASE::setActiveSampler(samplerNo);
		Ref<_TextureInstance> t = _getTexture(texture);
		if (t.isNotNull()) {
			useObject(texture);
			GL_BASE::bindTexture2D(t->texture);
			GL_BASE::setTexture2DFilterMode(texture->getMinFilter(), texture->getMagFilter());
			GL_BASE::setTexture2DWrapMode(texture->getWrapX(), texture->getWrapY());
		}
	}

	sl_bool _linkTexture(Texture* texture)
	{
		if (texture) {
			if (isCurrentEngine()) {
				Ref<_TextureInstance> t = _getTexture((Texture*)texture);
				if (t.isNotNull()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	sl_bool _beginProgram(RenderProgram* program)
	{
		if (!program) {
			return sl_false;
		}
		if (!isCurrentEngine()) {
			return sl_false;
		}
		Ref<_RenderProgramInstance> engineProgram = _getProgram(program);
		if (engineProgram.isNull()) {
			return sl_false;
		}
		useObject(program);
		GL_BASE::useProgram(engineProgram->program);
		m_currentProgram = engineProgram;
		if (program->onBeginProgram(this, engineProgram->info.ptr)) {
			return sl_true;
		}
		return sl_false;
	}

	void _endProgram()
	{
		if (!isCurrentEngine()) {
			return;
		}
		Ref<_RenderProgramInstance> engineProgram = m_currentProgram;
		if (engineProgram.isNull()) {
			return;
		}
		Ref<RenderProgram> p = engineProgram->getProgram();
		if (p.isNotNull()) {
			p->onEndProgram(this, engineProgram->info.ptr);
		}
		GL_BASE::useProgram(0);
		m_currentProgram.setNull();
	}

	void _drawPrimitive(Primitive* primitive)
	{
		if (!isCurrentEngine()) {
			return;
		}
		if (!primitive) {
			return;
		}
		if (primitive->countElements == 0) {
			return;
		}

		if (primitive->vertexBuffer.isNull()) {
			return;
		}

		Ref<_VertexBufferInstance> vb = _getVertexBuffer(primitive->vertexBuffer.ptr);
		if (vb.isNull() || vb->buffer == 0) {
			return;
		}
		useObject(primitive->vertexBuffer.ptr);

		Ref<_RenderProgramInstance> engineProgram = m_currentProgram;
		if (engineProgram.isNull()) {
			return;
		}
		Ref<RenderProgram> program = engineProgram->getProgram();
		if (program.isNull()) {
			return;
		}
		if (primitive->indexBuffer.isNotNull()) {
			Ref<_IndexBufferInstance> ib = _getIndexBuffer(primitive->indexBuffer.ptr);
			if (ib.isNull() || ib->buffer == 0) {
				return;
			}
			useObject(primitive->indexBuffer.ptr);

			GL_BASE::bindVertexBuffer(vb->buffer);
			if (program->onPreRender(this, engineProgram->info.ptr, primitive)) {
				switch (primitive->type) {
				case PrimitiveType::Triangles:
					GL_BASE::drawTriangles(primitive->countElements, ib->buffer, 0);
					break;
				case PrimitiveType::TriangleStrip:
					GL_BASE::drawTriangleStrip(primitive->countElements, ib->buffer, 0);
					break;
				case PrimitiveType::TriangleFan:
					GL_BASE::drawTriangleFan(primitive->countElements, ib->buffer, 0);
					break;
				case PrimitiveType::Lines:
					GL_BASE::drawLines(primitive->countElements, ib->buffer, 0);
					break;
				case PrimitiveType::LineStrip:
					GL_BASE::drawLineStrip(primitive->countElements, ib->buffer, 0);
					break;
				case PrimitiveType::Points:
					GL_BASE::drawPoints(primitive->countElements, ib->buffer, 0);
					break;
				}
				program->onPostRender(this, engineProgram->info.ptr, primitive);
			}
			//GL_BASE::unbindVertexBuffer();
		} else {
			GL_BASE::bindVertexBuffer(vb->buffer);
			if (program->onPreRender(this, engineProgram->info.ptr, primitive)) {
				switch (primitive->type) {
				case PrimitiveType::Triangles:
					GL_BASE::drawTriangles(primitive->countElements);
					break;
				case PrimitiveType::TriangleStrip:
					GL_BASE::drawTriangleStrip(primitive->countElements);
					break;
				case PrimitiveType::TriangleFan:
					GL_BASE::drawTriangleFan(primitive->countElements);
					break;
				case PrimitiveType::Lines:
					GL_BASE::drawLines(primitive->countElements);
					break;
				case PrimitiveType::LineStrip:
					GL_BASE::drawLineStrip(primitive->countElements);
					break;
				case PrimitiveType::Points:
					GL_BASE::drawPoints(primitive->countElements);
					break;
				}
				program->onPostRender(this, engineProgram->info.ptr, primitive);
			}
			//GL_BASE::unbindVertexBuffer();
		}
	}

	void _setLineWidth(sl_real width)
	{
		GL_BASE::setLineWidth(width);
	}

	/*************************************************
		OpenGL entry points
		**************************************************/
	sl_int32 getAttributeLocation(sl_uint32 program, const char* name)
	{
		return GL_BASE::getAttributeLocation(program, name);
	}
	
	void setVertexFloatArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexFloatArrayAttributePtr(attributeLocation, data, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexFloatArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexFloatArrayAttribute(attributeLocation, offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexInt8ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexInt8ArrayAttributePtr(attributeLocation, data, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexInt8ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexInt8ArrayAttribute(attributeLocation, offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexUint8ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexUint8ArrayAttributePtr(attributeLocation, data, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexUint8ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexUint8ArrayAttribute(attributeLocation, offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexInt16ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexInt16ArrayAttributePtr(attributeLocation, data, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexInt16ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexInt16ArrayAttribute(attributeLocation, offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexUint16ArrayAttributePtr(sl_int32 attributeLocation, const void* data, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexUint16ArrayAttributePtr(attributeLocation, data, countComponents, strideBytes, flagDoNormalize);
	}
	
	void setVertexUint16ArrayAttribute(sl_int32 attributeLocation, sl_size offsetValuesOnBuffer, sl_uint32 countComponents, sl_uint32 strideBytes, sl_bool flagDoNormalize)
	{
		GL_BASE::setVertexUint16ArrayAttribute(attributeLocation, offsetValuesOnBuffer, countComponents, strideBytes, flagDoNormalize);
	}
	
	void disableVertexArrayAttribute(sl_int32 attributeLocation)
	{
		GL_BASE::disableVertexArrayAttribute(attributeLocation);
	}

	
	sl_int32 getUniformLocation(sl_uint32 program, const char* name)
	{
		return GL_BASE::getUniformLocation(program, name);
	}
	
	void setUniformFloatValue(sl_int32 uniformLocation, float value)
	{
		GL_BASE::setUniformFloatValue(uniformLocation, value);
	}
	
	void setUniformFloatArray(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformFloatArray(uniformLocation, values, count);
	}
	
	void setUniformIntValue(sl_int32 uniformLocation, sl_int32 value)
	{
		GL_BASE::setUniformIntValue(uniformLocation, value);
	}
	
	void setUniformIntArray(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformIntArray(uniformLocation, values, count);
	}
	
	void setUniformFloat2Value(sl_int32 uniformLocation, float v1, float v2)
	{
		GL_BASE::setUniformFloat2Value(uniformLocation, v1, v2);
	}
	
	void setUniformFloat2Value(sl_int32 uniformLocation, const Vector2& v)
	{
		GL_BASE::setUniformFloat2Value(uniformLocation, v);
	}
	
	void setUniformFloat2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformFloat2Array(uniformLocation, values, count);
	}
	
	void setUniformInt2Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2)
	{
		GL_BASE::setUniformInt2Value(uniformLocation, v1, v2);
	}
	
	void setUniformInt2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformInt2Array(uniformLocation, values, count);
	}
	
	void setUniformFloat3Value(sl_int32 uniformLocation, float v1, float v2, float v3)
	{
		GL_BASE::setUniformFloat3Value(uniformLocation, v1, v2, v3);
	}
	
	void setUniformFloat3Value(sl_int32 uniformLocation, const Vector3& v)
	{
		GL_BASE::setUniformFloat3Value(uniformLocation, v);
	}
	
	void setUniformFloat3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformFloat3Array(uniformLocation, values, count);
	}
	
	void setUniformInt3Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3)
	{
		GL_BASE::setUniformInt3Value(uniformLocation, v1, v2, v3);
	}
	
	void setUniformInt3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformInt3Array(uniformLocation, values, count);
	}
	
	void setUniformFloat4Value(sl_int32 uniformLocation, float v1, float v2, float v3, float v4)
	{
		GL_BASE::setUniformFloat4Value(uniformLocation, v1, v2, v3, v4);
	}
	
	void setUniformFloat4Value(sl_int32 uniformLocation, const Vector4& v)
	{
		GL_BASE::setUniformFloat4Value(uniformLocation, v);
	}
	
	void setUniformFloat4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformFloat4Array(uniformLocation, values, count);
	}
	
	void setUniformInt4Value(sl_int32 uniformLocation, sl_int32 v1, sl_int32 v2, sl_int32 v3, sl_int32 v4)
	{
		GL_BASE::setUniformInt4Value(uniformLocation, v1, v2, v3, v4);
	}
	
	void setUniformInt4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformInt4Array(uniformLocation, values, count);
	}
	
	void setUniformMatrix2Value(sl_int32 uniformLocation, const Matrix2& value)
	{
		GL_BASE::setUniformMatrix2Value(uniformLocation, value);
	}
	
	void setUniformMatrix2Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformMatrix2Array(uniformLocation, values, count);
	}
	
	void setUniformMatrix3Value(sl_int32 uniformLocation, const Matrix3& value)
	{
		GL_BASE::setUniformMatrix3Value(uniformLocation, value);
	}
	
	void setUniformMatrix3Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformMatrix3Array(uniformLocation, values, count);
	}
	
	void setUniformMatrix4Value(sl_int32 uniformLocation, const Matrix4& value)
	{
		GL_BASE::setUniformMatrix4Value(uniformLocation, value);
	}
	
	void setUniformMatrix4Array(sl_int32 uniformLocation, const void* values, sl_uint32 count)
	{
		GL_BASE::setUniformMatrix4Array(uniformLocation, values, count);
	}
	
	void setUniformTextureSampler(sl_int32 uniformLocation, sl_uint32 samplerNo)
	{
		GL_BASE::setUniformTextureSampler(uniformLocation, samplerNo);
	}
	
};

Ref<GLRenderEngine> GL_BASE::createEngine()
{
	return new GL_ENGINE();
}

SLIB_RENDER_NAMESPACE_END

#endif
