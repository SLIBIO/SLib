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

#ifndef CHECKHEADERPRIV_SLIB_RENDER_OPENGL_GLES
#define CHECKHEADERPRIV_SLIB_RENDER_OPENGL_GLES

#include "slib/render/definition.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GLES)

#include "gl/GLES3/gl3.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#define PRIV_SLIB_RENDER_GLES_ENTRY(TYPE, name, ...) \
	TYPE(__stdcall* name)(__VA_ARGS__);

#define PRIV_SLIB_RENDER_GLES_ENTRIES \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glActiveTexture, GLenum texture) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glAttachShader, GLuint program, GLuint shader) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar* name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBindBuffer, GLenum target, GLuint buffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBindFramebuffer, GLenum target, GLuint framebuffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBindTexture, GLenum target, GLuint texture) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBlendColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBlendEquation, GLenum mode) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBlendFunc, GLenum sfactor, GLenum dfactor) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBlendFuncSeparate, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBufferData, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLenum, glCheckFramebufferStatus, GLenum target) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glClear, GLbitfield mask) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glClearDepthf, GLclampf depth) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glClearStencil, GLint s) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCompileShader, GLuint shader) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLuint, glCreateProgram) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLuint, glCreateShader, GLenum type) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glCullFace, GLenum mode) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteBuffers, GLsizei n, const GLuint* buffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteFramebuffers, GLsizei n, const GLuint* framebuffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteProgram, GLuint program) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteRenderbuffers, GLsizei n, const GLuint* renderbuffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteShader, GLuint shader) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDeleteTextures, GLsizei n, const GLuint* textures) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDepthFunc, GLenum func) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDepthMask, GLboolean flag) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDepthRangef, GLclampf zNear, GLclampf zFar) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDetachShader, GLuint program, GLuint shader) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDisable, GLenum cap) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDisableVertexAttribArray, GLuint index) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glEnable, GLenum cap) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glEnableVertexAttribArray, GLuint index) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glFinish) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glFlush) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glFrontFace, GLenum mode) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGenBuffers, GLsizei n, GLuint* buffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGenerateMipmap, GLenum target) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGenFramebuffers, GLsizei n, GLuint* framebuffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGenRenderbuffers, GLsizei n, GLuint* renderbuffers) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGenTextures, GLsizei n, GLuint* textures) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetAttachedShaders, GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLint, glGetAttribLocation, GLuint program, const GLchar* name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetBooleanv, GLenum pname, GLboolean* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLenum, glGetError) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetFloatv, GLenum pname, GLfloat* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetIntegerv, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetProgramiv, GLuint program, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetProgramInfoLog, GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetShaderInfoLog, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetShaderSource, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source) \
	PRIV_SLIB_RENDER_GLES_ENTRY(const GLubyte* , glGetString, GLenum name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetTexParameteriv, GLenum target, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetUniformfv, GLuint program, GLint location, GLfloat* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetUniformiv, GLuint program, GLint location, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLint, glGetUniformLocation, GLuint program, const GLchar* name) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, GLvoid** pointer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glHint, GLenum target, GLenum mode) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsBuffer, GLuint buffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsEnabled, GLenum cap) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsFramebuffer, GLuint framebuffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsProgram, GLuint program) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsShader, GLuint shader) \
	PRIV_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsTexture, GLuint texture) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glLineWidth, GLfloat width) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glLinkProgram, GLuint program) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glPixelStorei, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glPolygonOffset, GLfloat factor, GLfloat units) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glReleaseShaderCompiler) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glSampleCoverage, GLclampf value, GLboolean invert) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glShaderBinary, GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glShaderSource, GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilFunc, GLenum func, GLint ref, GLuint mask) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilMask, GLuint mask) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilMaskSeparate, GLenum face, GLuint mask) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glStencilOpSeparate, GLenum face, GLenum fail, GLenum zfail, GLenum zpass) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexParameterf, GLenum target, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexParameteriv, GLenum target, GLenum pname, const GLint* params) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform1f, GLint location, GLfloat x) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform1fv, GLint location, GLsizei count, const GLfloat* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform1i, GLint location, GLint x) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform1iv, GLint location, GLsizei count, const GLint* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform2f, GLint location, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform2fv, GLint location, GLsizei count, const GLfloat* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform2i, GLint location, GLint x, GLint y) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform2iv, GLint location, GLsizei count, const GLint* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform3f, GLint location, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform3fv, GLint location, GLsizei count, const GLfloat* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform3i, GLint location, GLint x, GLint y, GLint z) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform3iv, GLint location, GLsizei count, const GLint* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform4f, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform4fv, GLint location, GLsizei count, const GLfloat* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform4i, GLint location, GLint x, GLint y, GLint z, GLint w) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniform4iv, GLint location, GLsizei count, const GLint* v) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glUseProgram, GLuint program) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glValidateProgram, GLuint program) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib1f, GLuint indx, GLfloat x) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib1fv, GLuint indx, const GLfloat* values) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib2f, GLuint indx, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib2fv, GLuint indx, const GLfloat* values) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib3f, GLuint indx, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib3fv, GLuint indx, const GLfloat* values) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib4f, GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib4fv, GLuint indx, const GLfloat* values) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glVertexAttribPointer, GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) \
	PRIV_SLIB_RENDER_GLES_ENTRY(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height) 


namespace slib
{
	namespace priv
	{
		namespace gles
		{
			
			struct EntryPoints
			{
				PRIV_SLIB_RENDER_GLES_ENTRIES
			};

			extern EntryPoints g_entries;

		}
	}
}

#define PRIV_GLES_ENTRY(name) slib::priv::gles::g_entries.name

#else

#define PRIV_GLES_ENTRY(name) name

#endif

#endif

#endif
