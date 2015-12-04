#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_GLES
#define CHECKHEADER_SLIB_RENDER_OPENGL_GLES

#include "../../../inc/slib/render/definition.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GLES)

#include "../../../inc/thirdparty/gl/GLES3/gl3.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#define _SLIB_RENDER_GLES_ENTRY(TYPE, name, ...) \
	TYPE(__stdcall* name)(__VA_ARGS__);

#define _SLIB_RENDER_GLES_ENTRIES \
	_SLIB_RENDER_GLES_ENTRY(void, glActiveTexture, GLenum texture) \
	_SLIB_RENDER_GLES_ENTRY(void, glAttachShader, GLuint program, GLuint shader) \
	_SLIB_RENDER_GLES_ENTRY(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar* name) \
	_SLIB_RENDER_GLES_ENTRY(void, glBindBuffer, GLenum target, GLuint buffer) \
	_SLIB_RENDER_GLES_ENTRY(void, glBindFramebuffer, GLenum target, GLuint framebuffer) \
	_SLIB_RENDER_GLES_ENTRY(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer) \
	_SLIB_RENDER_GLES_ENTRY(void, glBindTexture, GLenum target, GLuint texture) \
	_SLIB_RENDER_GLES_ENTRY(void, glBlendColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
	_SLIB_RENDER_GLES_ENTRY(void, glBlendEquation, GLenum mode) \
	_SLIB_RENDER_GLES_ENTRY(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
	_SLIB_RENDER_GLES_ENTRY(void, glBlendFunc, GLenum sfactor, GLenum dfactor) \
	_SLIB_RENDER_GLES_ENTRY(void, glBlendFuncSeparate, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
	_SLIB_RENDER_GLES_ENTRY(void, glBufferData, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) \
	_SLIB_RENDER_GLES_ENTRY(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data) \
	_SLIB_RENDER_GLES_ENTRY(GLenum, glCheckFramebufferStatus, GLenum target) \
	_SLIB_RENDER_GLES_ENTRY(void, glClear, GLbitfield mask) \
	_SLIB_RENDER_GLES_ENTRY(void, glClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
	_SLIB_RENDER_GLES_ENTRY(void, glClearDepthf, GLclampf depth) \
	_SLIB_RENDER_GLES_ENTRY(void, glClearStencil, GLint s) \
	_SLIB_RENDER_GLES_ENTRY(void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
	_SLIB_RENDER_GLES_ENTRY(void, glCompileShader, GLuint shader) \
	_SLIB_RENDER_GLES_ENTRY(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) \
	_SLIB_RENDER_GLES_ENTRY(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data) \
	_SLIB_RENDER_GLES_ENTRY(void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) \
	_SLIB_RENDER_GLES_ENTRY(void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	_SLIB_RENDER_GLES_ENTRY(GLuint, glCreateProgram) \
	_SLIB_RENDER_GLES_ENTRY(GLuint, glCreateShader, GLenum type) \
	_SLIB_RENDER_GLES_ENTRY(void, glCullFace, GLenum mode) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteBuffers, GLsizei n, const GLuint* buffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteFramebuffers, GLsizei n, const GLuint* framebuffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteProgram, GLuint program) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteRenderbuffers, GLsizei n, const GLuint* renderbuffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteShader, GLuint shader) \
	_SLIB_RENDER_GLES_ENTRY(void, glDeleteTextures, GLsizei n, const GLuint* textures) \
	_SLIB_RENDER_GLES_ENTRY(void, glDepthFunc, GLenum func) \
	_SLIB_RENDER_GLES_ENTRY(void, glDepthMask, GLboolean flag) \
	_SLIB_RENDER_GLES_ENTRY(void, glDepthRangef, GLclampf zNear, GLclampf zFar) \
	_SLIB_RENDER_GLES_ENTRY(void, glDetachShader, GLuint program, GLuint shader) \
	_SLIB_RENDER_GLES_ENTRY(void, glDisable, GLenum cap) \
	_SLIB_RENDER_GLES_ENTRY(void, glDisableVertexAttribArray, GLuint index) \
	_SLIB_RENDER_GLES_ENTRY(void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
	_SLIB_RENDER_GLES_ENTRY(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) \
	_SLIB_RENDER_GLES_ENTRY(void, glEnable, GLenum cap) \
	_SLIB_RENDER_GLES_ENTRY(void, glEnableVertexAttribArray, GLuint index) \
	_SLIB_RENDER_GLES_ENTRY(void, glFinish) \
	_SLIB_RENDER_GLES_ENTRY(void, glFlush) \
	_SLIB_RENDER_GLES_ENTRY(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
	_SLIB_RENDER_GLES_ENTRY(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	_SLIB_RENDER_GLES_ENTRY(void, glFrontFace, GLenum mode) \
	_SLIB_RENDER_GLES_ENTRY(void, glGenBuffers, GLsizei n, GLuint* buffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glGenerateMipmap, GLenum target) \
	_SLIB_RENDER_GLES_ENTRY(void, glGenFramebuffers, GLsizei n, GLuint* framebuffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glGenRenderbuffers, GLsizei n, GLuint* renderbuffers) \
	_SLIB_RENDER_GLES_ENTRY(void, glGenTextures, GLsizei n, GLuint* textures) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetAttachedShaders, GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders) \
	_SLIB_RENDER_GLES_ENTRY(GLint, glGetAttribLocation, GLuint program, const GLchar* name) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetBooleanv, GLenum pname, GLboolean* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(GLenum, glGetError) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetFloatv, GLenum pname, GLfloat* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetIntegerv, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetProgramiv, GLuint program, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetProgramInfoLog, GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetShaderInfoLog, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetShaderSource, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source) \
	_SLIB_RENDER_GLES_ENTRY(const GLubyte* , glGetString, GLenum name) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetTexParameteriv, GLenum target, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetUniformfv, GLuint program, GLint location, GLfloat* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetUniformiv, GLuint program, GLint location, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(GLint, glGetUniformLocation, GLuint program, const GLchar* name) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, GLvoid** pointer) \
	_SLIB_RENDER_GLES_ENTRY(void, glHint, GLenum target, GLenum mode) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsBuffer, GLuint buffer) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsEnabled, GLenum cap) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsFramebuffer, GLuint framebuffer) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsProgram, GLuint program) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsShader, GLuint shader) \
	_SLIB_RENDER_GLES_ENTRY(GLboolean, glIsTexture, GLuint texture) \
	_SLIB_RENDER_GLES_ENTRY(void, glLineWidth, GLfloat width) \
	_SLIB_RENDER_GLES_ENTRY(void, glLinkProgram, GLuint program) \
	_SLIB_RENDER_GLES_ENTRY(void, glPixelStorei, GLenum pname, GLint param) \
	_SLIB_RENDER_GLES_ENTRY(void, glPolygonOffset, GLfloat factor, GLfloat units) \
	_SLIB_RENDER_GLES_ENTRY(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) \
	_SLIB_RENDER_GLES_ENTRY(void, glReleaseShaderCompiler) \
	_SLIB_RENDER_GLES_ENTRY(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height) \
	_SLIB_RENDER_GLES_ENTRY(void, glSampleCoverage, GLclampf value, GLboolean invert) \
	_SLIB_RENDER_GLES_ENTRY(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
	_SLIB_RENDER_GLES_ENTRY(void, glShaderBinary, GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length) \
	_SLIB_RENDER_GLES_ENTRY(void, glShaderSource, GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilFunc, GLenum func, GLint ref, GLuint mask) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilMask, GLuint mask) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilMaskSeparate, GLenum face, GLuint mask) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass) \
	_SLIB_RENDER_GLES_ENTRY(void, glStencilOpSeparate, GLenum face, GLenum fail, GLenum zfail, GLenum zpass) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexParameterf, GLenum target, GLenum pname, GLfloat param) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexParameteriv, GLenum target, GLenum pname, const GLint* params) \
	_SLIB_RENDER_GLES_ENTRY(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform1f, GLint location, GLfloat x) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform1fv, GLint location, GLsizei count, const GLfloat* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform1i, GLint location, GLint x) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform1iv, GLint location, GLsizei count, const GLint* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform2f, GLint location, GLfloat x, GLfloat y) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform2fv, GLint location, GLsizei count, const GLfloat* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform2i, GLint location, GLint x, GLint y) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform2iv, GLint location, GLsizei count, const GLint* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform3f, GLint location, GLfloat x, GLfloat y, GLfloat z) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform3fv, GLint location, GLsizei count, const GLfloat* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform3i, GLint location, GLint x, GLint y, GLint z) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform3iv, GLint location, GLsizei count, const GLint* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform4f, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform4fv, GLint location, GLsizei count, const GLfloat* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform4i, GLint location, GLint x, GLint y, GLint z, GLint w) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniform4iv, GLint location, GLsizei count, const GLint* v) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	_SLIB_RENDER_GLES_ENTRY(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
	_SLIB_RENDER_GLES_ENTRY(void, glUseProgram, GLuint program) \
	_SLIB_RENDER_GLES_ENTRY(void, glValidateProgram, GLuint program) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib1f, GLuint indx, GLfloat x) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib1fv, GLuint indx, const GLfloat* values) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib2f, GLuint indx, GLfloat x, GLfloat y) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib2fv, GLuint indx, const GLfloat* values) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib3f, GLuint indx, GLfloat x, GLfloat y, GLfloat z) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib3fv, GLuint indx, const GLfloat* values) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib4f, GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttrib4fv, GLuint indx, const GLfloat* values) \
	_SLIB_RENDER_GLES_ENTRY(void, glVertexAttribPointer, GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) \
	_SLIB_RENDER_GLES_ENTRY(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height) 


SLIB_RENDER_NAMESPACE_BEGIN
struct _GLES_ENTRY_POINTS
{
	_SLIB_RENDER_GLES_ENTRIES
};
extern _GLES_ENTRY_POINTS _GLES_ENTRIES;
SLIB_RENDER_NAMESPACE_END

#define _GLES_ENTRY(name) _GLES_ENTRIES.name

#else

#define _GLES_ENTRY(name) name

#endif

#endif

#endif
