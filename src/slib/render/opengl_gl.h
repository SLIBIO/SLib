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

#ifndef CHECKHEADER_SLIB_RENDER_OPENGL_GL
#define CHECKHEADER_SLIB_RENDER_OPENGL_GL

#include "slib/render/definition.h"

#if defined(SLIB_RENDER_SUPPORT_OPENGL_GL)

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <Windows.h>
#include <gl/gl.h>

#include "gl/GL/glext.h"

#define PRIV_SLIB_RENDER_GL_ENTRY(TYPE, name, ...) \
	TYPE(__stdcall* name)(__VA_ARGS__);

#define PRIV_SLIB_RENDER_GL_SUPPORT(name) \
	sl_bool flagSupports##name; \
	SLIB_INLINE sl_bool isSupporting##name() \
	{ \
		return flagSupports##name; \
	}

#define PRIV_SLIB_RENDER_GL_ENTRIES \
	/* 1.1 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glAccum, GLenum op, GLfloat value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glAlphaFunc, GLenum func, GLclampf ref) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glAreTexturesResident, GLsizei n, const GLuint *textures, GLboolean *residences) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glArrayElement, GLint i) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBegin, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindTexture, GLenum target, GLuint texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBitmap, GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendFunc, GLenum sfactor, GLenum dfactor) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCallList, GLuint list) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCallLists, GLsizei n, GLenum type, const void *lists) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClear, GLbitfield mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearAccum, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearDepth, GLclampd depth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearIndex, GLfloat c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearStencil, GLint s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClipPlane, GLenum plane, const GLdouble *equation) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3b, GLbyte red, GLbyte green, GLbyte blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3bv, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3d, GLdouble red, GLdouble green, GLdouble blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3f, GLfloat red, GLfloat green, GLfloat blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3i, GLint red, GLint green, GLint blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3s, GLshort red, GLshort green, GLshort blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3ub, GLubyte red, GLubyte green, GLubyte blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3ubv, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3ui, GLuint red, GLuint green, GLuint blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3uiv, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3us, GLushort red, GLushort green, GLushort blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor3usv, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4b, GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4bv, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4d, GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4f, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4i, GLint red, GLint green, GLint blue, GLint alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4s, GLshort red, GLshort green, GLshort blue, GLshort alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4ub, GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4ubv, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4ui, GLuint red, GLuint green, GLuint blue, GLuint alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4uiv, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4us, GLushort red, GLushort green, GLushort blue, GLushort alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColor4usv, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorMaterial, GLenum face, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorPointer, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum type) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTexImage1D, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCullFace, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteLists, GLuint list, GLsizei range) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteTextures, GLsizei n, const GLuint *textures) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthFunc, GLenum func) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthMask, GLboolean flag) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthRange, GLclampd zNear, GLclampd zFar) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDisable, GLenum cap) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDisableClientState, GLenum array) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawBuffer, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const void *indices) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawPixels, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEdgeFlag, GLboolean flag) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEdgeFlagPointer, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEdgeFlagv, const GLboolean *flag) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnable, GLenum cap) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnableClientState, GLenum array) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnd) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEndList) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord1d, GLdouble u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord1dv, const GLdouble *u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord1f, GLfloat u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord1fv, const GLfloat *u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord2d, GLdouble u, GLdouble v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord2dv, const GLdouble *u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord2f, GLfloat u, GLfloat v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalCoord2fv, const GLfloat *u) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalMesh1, GLenum mode, GLint i1, GLint i2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalMesh2, GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalPoint1, GLint i) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEvalPoint2, GLint i, GLint j) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFeedbackBuffer, GLsizei size, GLenum type, GLfloat *buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFinish) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFlush) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogf, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogfv, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogi, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogiv, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFrontFace, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFrustum, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glGenLists, GLsizei range) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenTextures, GLsizei n, GLuint *textures) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBooleanv, GLenum pname, GLboolean *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetClipPlane, GLenum plane, GLdouble *equation) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetDoublev, GLenum pname, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLenum, glGetError) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetFloatv, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetIntegerv, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetLightfv, GLenum light, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetLightiv, GLenum light, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMapdv, GLenum target, GLenum query, GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMapfv, GLenum target, GLenum query, GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMapiv, GLenum target, GLenum query, GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMaterialfv, GLenum face, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMaterialiv, GLenum face, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetPixelMapfv, GLenum map, GLfloat *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetPixelMapuiv, GLenum map, GLuint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetPixelMapusv, GLenum map, GLushort *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetPointerv, GLenum pname, void* *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetPolygonStipple, GLubyte *mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(const GLubyte *, glGetString, GLenum name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexEnvfv, GLenum target, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexEnviv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexGendv, GLenum coord, GLenum pname, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexGenfv, GLenum coord, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexGeniv, GLenum coord, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexImage, GLenum target, GLint level, GLenum format, GLenum type, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexLevelParameterfv, GLenum target, GLint level, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexLevelParameteriv, GLenum target, GLint level, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexParameteriv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glHint, GLenum target, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexMask, GLuint mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexPointer, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexd, GLdouble c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexdv, const GLdouble *c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexf, GLfloat c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexfv, const GLfloat *c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexi, GLint c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexiv, const GLint *c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexs, GLshort c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexsv, const GLshort *c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexub, GLubyte c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glIndexubv, const GLubyte *c) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInitNames) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInterleavedArrays, GLenum format, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsEnabled, GLenum cap) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsList, GLuint list) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsTexture, GLuint texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightModelf, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightModelfv, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightModeli, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightModeliv, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightf, GLenum light, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightfv, GLenum light, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLighti, GLenum light, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLightiv, GLenum light, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLineStipple, GLint factor, GLushort pattern) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLineWidth, GLfloat width) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glListBase, GLuint base) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadIdentity) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadMatrixd, const GLdouble *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadMatrixf, const GLfloat *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadName, GLuint name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLogicOp, GLenum opcode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMap1d, GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMap1f, GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMap2d, GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMap2f, GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMapGrid1d, GLint un, GLdouble u1, GLdouble u2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMapGrid1f, GLint un, GLfloat u1, GLfloat u2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMapGrid2d, GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMapGrid2f, GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMaterialf, GLenum face, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMaterialfv, GLenum face, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMateriali, GLenum face, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMaterialiv, GLenum face, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMatrixMode, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultMatrixd, const GLdouble *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultMatrixf, const GLfloat *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNewList, GLuint list, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3b, GLbyte nx, GLbyte ny, GLbyte nz) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3bv, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3d, GLdouble nx, GLdouble ny, GLdouble nz) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3f, GLfloat nx, GLfloat ny, GLfloat nz) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3i, GLint nx, GLint ny, GLint nz) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3s, GLshort nx, GLshort ny, GLshort nz) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormal3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormalPointer, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glOrtho, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPassThrough, GLfloat token) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelMapfv, GLenum map, GLsizei mapsize, const GLfloat *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelMapuiv, GLenum map, GLsizei mapsize, const GLuint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelMapusv, GLenum map, GLsizei mapsize, const GLushort *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelStoref, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelStorei, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelTransferf, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelTransferi, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPixelZoom, GLfloat xfactor, GLfloat yfactor) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPointSize, GLfloat size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPolygonMode, GLenum face, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPolygonOffset, GLfloat factor, GLfloat units) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPolygonStipple, const GLubyte *mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPopAttrib) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPopClientAttrib) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPopMatrix) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPopName) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPrioritizeTextures, GLsizei n, const GLuint *textures, const GLclampf *priorities) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPushAttrib, GLbitfield mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPushClientAttrib, GLbitfield mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPushMatrix) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPushName, GLuint name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2d, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2f, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2i, GLint x, GLint y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2s, GLshort x, GLshort y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos2sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3d, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3f, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3i, GLint x, GLint y, GLint z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3s, GLshort x, GLshort y, GLshort z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4d, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4f, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4i, GLint x, GLint y, GLint z, GLint w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4s, GLshort x, GLshort y, GLshort z, GLshort w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRasterPos4sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glReadBuffer, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectd, GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectdv, const GLdouble *v1, const GLdouble *v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectf, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectfv, const GLfloat *v1, const GLfloat *v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRecti, GLint x1, GLint y1, GLint x2, GLint y2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectiv, const GLint *v1, const GLint *v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRects, GLshort x1, GLshort y1, GLshort x2, GLshort y2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRectsv, const GLshort *v1, const GLshort *v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glRenderMode, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRotated, GLdouble angle, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRotatef, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScaled, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScalef, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSelectBuffer, GLsizei size, GLuint *buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glShadeModel, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilFunc, GLenum func, GLint ref, GLuint mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilMask, GLuint mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1d, GLdouble s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1f, GLfloat s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1i, GLint s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1s, GLshort s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord1sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2d, GLdouble s, GLdouble t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2f, GLfloat s, GLfloat t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2i, GLint s, GLint t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2s, GLshort s, GLshort t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord2sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3d, GLdouble s, GLdouble t, GLdouble r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3f, GLfloat s, GLfloat t, GLfloat r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3i, GLint s, GLint t, GLint r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3s, GLshort s, GLshort t, GLshort r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4d, GLdouble s, GLdouble t, GLdouble r, GLdouble q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4f, GLfloat s, GLfloat t, GLfloat r, GLfloat q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4i, GLint s, GLint t, GLint r, GLint q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4s, GLshort s, GLshort t, GLshort r, GLshort q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoord4sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordPointer, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexEnvf, GLenum target, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexEnvfv, GLenum target, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexEnvi, GLenum target, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexEnviv, GLenum target, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGend, GLenum coord, GLenum pname, GLdouble param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGendv, GLenum coord, GLenum pname, const GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGenf, GLenum coord, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGenfv, GLenum coord, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGeni, GLenum coord, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexGeniv, GLenum coord, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexImage1D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameterf, GLenum target, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameteriv, GLenum target, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTranslated, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTranslatef, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2d, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2f, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2i, GLint x, GLint y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2s, GLshort x, GLshort y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex2sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3d, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3f, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3i, GLint x, GLint y, GLint z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3s, GLshort x, GLshort y, GLshort z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4d, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4f, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4i, GLint x, GLint y, GLint z, GLint w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4s, GLshort x, GLshort y, GLshort z, GLshort w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertex4sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexPointer, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_1_1) \
	/* 1.2 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_1_2) \
	/* 1.3 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glActiveTexture, GLenum texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSampleCoverage, GLfloat value, GLboolean invert) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexImage1D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetCompressedTexImage, GLenum target, GLint level, void *img) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClientActiveTexture, GLenum texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1d, GLenum target, GLdouble s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1dv, GLenum target, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1f, GLenum target, GLfloat s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1fv, GLenum target, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1i, GLenum target, GLint s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1iv, GLenum target, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1s, GLenum target, GLshort s) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord1sv, GLenum target, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2d, GLenum target, GLdouble s, GLdouble t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2dv, GLenum target, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2f, GLenum target, GLfloat s, GLfloat t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2fv, GLenum target, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2i, GLenum target, GLint s, GLint t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2iv, GLenum target, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2s, GLenum target, GLshort s, GLshort t) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord2sv, GLenum target, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3d, GLenum target, GLdouble s, GLdouble t, GLdouble r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3dv, GLenum target, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3f, GLenum target, GLfloat s, GLfloat t, GLfloat r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3fv, GLenum target, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3i, GLenum target, GLint s, GLint t, GLint r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3iv, GLenum target, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3s, GLenum target, GLshort s, GLshort t, GLshort r) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord3sv, GLenum target, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4d, GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4dv, GLenum target, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4f, GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4fv, GLenum target, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4i, GLenum target, GLint s, GLint t, GLint r, GLint q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4iv, GLenum target, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4s, GLenum target, GLshort s, GLshort t, GLshort r, GLshort q) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoord4sv, GLenum target, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadTransposeMatrixf, const GLfloat *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLoadTransposeMatrixd, const GLdouble *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultTransposeMatrixf, const GLfloat *m) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultTransposeMatrixd, const GLdouble *m) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_1_3) \
	/* 1.4 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiDrawArrays, GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiDrawElements, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPointParameterf, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPointParameterfv, GLenum pname, const GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPointParameteri, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPointParameteriv, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogCoordf, GLfloat coord) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogCoordfv, const GLfloat *coord) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogCoordd, GLdouble coord) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogCoorddv, const GLdouble *coord) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFogCoordPointer, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3b, GLbyte red, GLbyte green, GLbyte blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3bv, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3d, GLdouble red, GLdouble green, GLdouble blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3f, GLfloat red, GLfloat green, GLfloat blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3i, GLint red, GLint green, GLint blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3s, GLshort red, GLshort green, GLshort blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3ub, GLubyte red, GLubyte green, GLubyte blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3ubv, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3ui, GLuint red, GLuint green, GLuint blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3uiv, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3us, GLushort red, GLushort green, GLushort blue) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColor3usv, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColorPointer, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2d, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2f, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2i, GLint x, GLint y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2s, GLshort x, GLshort y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos2sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3d, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3dv, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3f, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3fv, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3i, GLint x, GLint y, GLint z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3iv, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3s, GLshort x, GLshort y, GLshort z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWindowPos3sv, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendEquation, GLenum mode) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_1_4) \
	/* 1.5 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenQueries, GLsizei n, GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteQueries, GLsizei n, const GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsQuery, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBeginQuery, GLenum target, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEndQuery, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindBuffer, GLenum target, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteBuffers, GLsizei n, const GLuint *buffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenBuffers, GLsizei n, GLuint *buffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsBuffer, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void*, glMapBuffer, GLenum target, GLenum access) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glUnmapBuffer, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBufferPointerv, GLenum target, GLenum pname, void **params) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_1_5) \
	/* 2.0 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawBuffers, GLsizei n, const GLenum *bufs) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glStencilMaskSeparate, GLenum face, GLuint mask) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glAttachShader, GLuint program, GLuint shader) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompileShader, GLuint shader) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glCreateProgram) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glCreateShader, GLenum type) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteProgram, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteShader, GLuint shader) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDetachShader, GLuint program, GLuint shader) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDisableVertexAttribArray, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnableVertexAttribArray, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetAttribLocation, GLuint program, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetUniformLocation, GLuint program, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformfv, GLuint program, GLint location, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformiv, GLuint program, GLint location, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, void **pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsProgram, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsShader, GLuint shader) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glLinkProgram, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUseProgram, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1f, GLint location, GLfloat v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1i, GLint location, GLint v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2i, GLint location, GLint v0, GLint v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1iv, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2iv, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3iv, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4iv, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glValidateProgram, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1d, GLuint index, GLdouble x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1f, GLuint index, GLfloat x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1fv, GLuint index, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1s, GLuint index, GLshort x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib1sv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2fv, GLuint index, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib2sv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3fv, GLuint index, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib3sv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nbv, GLuint index, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Niv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nsv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nubv, GLuint index, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nuiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4Nusv, GLuint index, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4bv, GLuint index, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4fv, GLuint index, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4iv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4sv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4ubv, GLuint index, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4uiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttrib4usv, GLuint index, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_2_0) \
	/* 2.1 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_2_1) \
	/* 3.0 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorMaski, GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBooleani_v, GLenum target, GLuint index, GLboolean *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetIntegeri_v, GLenum target, GLuint index, GLint *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnablei, GLenum target, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDisablei, GLenum target, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsEnabledi, GLenum target, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBeginTransformFeedback, GLenum primitiveMode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEndTransformFeedback) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClampColor, GLenum target, GLenum clamp) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBeginConditionalRender, GLuint id, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEndConditionalRender) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI1i, GLuint index, GLint x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI2i, GLuint index, GLint x, GLint y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI1ui, GLuint index, GLuint x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI1iv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI2iv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI3iv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4iv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI1uiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI2uiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI3uiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4uiv, GLuint index, const GLuint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4bv, GLuint index, const GLbyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4sv, GLuint index, const GLshort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribI4usv, GLuint index, const GLushort *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindFragDataLocation, GLuint program, GLuint color, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetFragDataLocation, GLuint program, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1ui, GLint location, GLuint v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2ui, GLint location, GLuint v0, GLuint v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1uiv, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2uiv, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3uiv, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4uiv, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameterIiv, GLenum target, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexParameterIuiv, GLenum target, GLenum pname, const GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) \
	PRIV_SLIB_RENDER_GL_ENTRY(const GLubyte *, glGetStringi, GLenum name, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteRenderbuffers, GLsizei n, const GLuint *renderbuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenRenderbuffers, GLsizei n, GLuint *renderbuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsFramebuffer, GLuint framebuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindFramebuffer, GLenum target, GLuint framebuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteFramebuffers, GLsizei n, const GLuint *framebuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenFramebuffers, GLsizei n, GLuint *framebuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLenum, glCheckFramebufferStatus, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferTexture1D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferTexture3D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenerateMipmap, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferTextureLayer, GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void *, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFlushMappedBufferRange, GLenum target, GLintptr offset, GLsizeiptr length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindVertexArray, GLuint array) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenVertexArrays, GLsizei n, GLuint *arrays) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsVertexArray, GLuint array) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_3_0) \
	/* 3.1 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPrimitiveRestartIndex, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyBufferSubData, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveUniformName, GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar *uniformBlockName) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_3_1) \
	/* 3.2 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawRangeElementsBaseVertex, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsInstancedBaseVertex, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiDrawElementsBaseVertex, GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProvokingVertex, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLsync, glFenceSync, GLenum condition, GLbitfield flags) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsSync, GLsync sync) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteSync, GLsync sync) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetInteger64v, GLenum pname, GLint64 *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetInteger64i_v, GLenum target, GLuint index, GLint64 *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64 *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexImage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexImage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetMultisamplefv, GLenum pname, GLuint index, GLfloat *val) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSampleMaski, GLuint maskNumber, GLbitfield mask) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_3_2) \
	/* 3.3 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindFragDataLocationIndexed, GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetFragDataIndex, GLuint program, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenSamplers, GLsizei count, GLuint *samplers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteSamplers, GLsizei count, const GLuint *samplers) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsSampler, GLuint sampler) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindSampler, GLuint unit, GLuint sampler) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const GLuint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetSamplerParameteriv, GLuint sampler, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetSamplerParameterIiv, GLuint sampler, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetSamplerParameterfv, GLuint sampler, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetSamplerParameterIuiv, GLuint sampler, GLenum pname, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glQueryCounter, GLuint id, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryObjecti64v, GLuint id, GLenum pname, GLint64 *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryObjectui64v, GLuint id, GLenum pname, GLuint64 *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribDivisor, GLuint index, GLuint divisor) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP1ui, GLuint index, GLenum type, GLboolean normalized, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP1uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP2ui, GLuint index, GLenum type, GLboolean normalized, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP2uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP3ui, GLuint index, GLenum type, GLboolean normalized, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP3uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP4ui, GLuint index, GLenum type, GLboolean normalized, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribP4uiv, GLuint index, GLenum type, GLboolean normalized, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP2ui, GLenum type, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP2uiv, GLenum type, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP3ui, GLenum type, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP3uiv, GLenum type, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP4ui, GLenum type, GLuint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexP4uiv, GLenum type, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP1ui, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP1uiv, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP2ui, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP2uiv, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP3ui, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP3uiv, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP4ui, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexCoordP4uiv, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP1ui, GLenum texture, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP1uiv, GLenum texture, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP2ui, GLenum texture, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP2uiv, GLenum texture, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP3ui, GLenum texture, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP3uiv, GLenum texture, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP4ui, GLenum texture, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiTexCoordP4uiv, GLenum texture, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormalP3ui, GLenum type, GLuint coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNormalP3uiv, GLenum type, const GLuint *coords) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorP3ui, GLenum type, GLuint color) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorP3uiv, GLenum type, const GLuint *color) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorP4ui, GLenum type, GLuint color) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glColorP4uiv, GLenum type, const GLuint *color) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColorP3ui, GLenum type, GLuint color) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glSecondaryColorP3uiv, GLenum type, const GLuint *color) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_3_3) \
	/* 4.0 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMinSampleShading, GLfloat value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendEquationi, GLuint buf, GLenum mode) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendEquationSeparatei, GLuint buf, GLenum modeRGB, GLenum modeAlpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendFunci, GLuint buf, GLenum src, GLenum dst) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlendFuncSeparatei, GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawArraysIndirect, GLenum mode, const void *indirect) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1d, GLint location, GLdouble x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2d, GLint location, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3d, GLint location, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4d, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform1dv, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform2dv, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform3dv, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniform4dv, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix2x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix3x4dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4x2dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformMatrix4x3dv, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformdv, GLuint program, GLint location, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetSubroutineUniformLocation, GLuint program, GLenum shadertype, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glGetSubroutineIndex, GLuint program, GLenum shadertype, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveSubroutineUniformiv, GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveSubroutineUniformName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveSubroutineName, GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUniformSubroutinesuiv, GLenum shadertype, GLsizei count, const GLuint *indices) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetUniformSubroutineuiv, GLenum shadertype, GLint location, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramStageiv, GLuint program, GLenum shadertype, GLenum pname, GLint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPatchParameteri, GLenum pname, GLint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPatchParameterfv, GLenum pname, const GLfloat *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindTransformFeedback, GLenum target, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenTransformFeedbacks, GLsizei n, GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsTransformFeedback, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPauseTransformFeedback) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glResumeTransformFeedback) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawTransformFeedback, GLenum mode, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawTransformFeedbackStream, GLenum mode, GLuint id, GLuint stream) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBeginQueryIndexed, GLenum target, GLuint index, GLuint id) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEndQueryIndexed, GLenum target, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryIndexediv, GLenum target, GLuint index, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_0) \
	/* 4.1 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glReleaseShaderCompiler) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glShaderBinary, GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthRangef, GLfloat n, GLfloat f) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearDepthf, GLfloat d) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramParameteri, GLuint program, GLenum pname, GLint value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glUseProgramStages, GLuint pipeline, GLbitfield stages, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glActiveShaderProgram, GLuint pipeline, GLuint program) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glCreateShaderProgramv, GLenum type, GLsizei count, const GLchar *const*strings) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindProgramPipeline, GLuint pipeline) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDeleteProgramPipelines, GLsizei n, const GLuint *pipelines) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenProgramPipelines, GLsizei n, GLuint *pipelines) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glIsProgramPipeline, GLuint pipeline) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramPipelineiv, GLuint pipeline, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1i, GLuint program, GLint location, GLint v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1iv, GLuint program, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1f, GLuint program, GLint location, GLfloat v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1fv, GLuint program, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1d, GLuint program, GLint location, GLdouble v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1dv, GLuint program, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1ui, GLuint program, GLint location, GLuint v0) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform1uiv, GLuint program, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2i, GLuint program, GLint location, GLint v0, GLint v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2iv, GLuint program, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2f, GLuint program, GLint location, GLfloat v0, GLfloat v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2fv, GLuint program, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2d, GLuint program, GLint location, GLdouble v0, GLdouble v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2dv, GLuint program, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2ui, GLuint program, GLint location, GLuint v0, GLuint v1) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform2uiv, GLuint program, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3iv, GLuint program, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3fv, GLuint program, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3dv, GLuint program, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform3uiv, GLuint program, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4i, GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4iv, GLuint program, GLint location, GLsizei count, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4f, GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4fv, GLuint program, GLint location, GLsizei count, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4d, GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4dv, GLuint program, GLint location, GLsizei count, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4ui, GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniform4uiv, GLuint program, GLint location, GLsizei count, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4x2fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3x4fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4x3fv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix2x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4x2dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix3x4dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glProgramUniformMatrix4x3dv, GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glValidateProgramPipeline, GLuint pipeline) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramPipelineInfoLog, GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL1d, GLuint index, GLdouble x) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL2d, GLuint index, GLdouble x, GLdouble y) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL3d, GLuint index, GLdouble x, GLdouble y, GLdouble z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL1dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL2dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL3dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribL4dv, GLuint index, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribLPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexAttribLdv, GLuint index, GLenum pname, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glViewportArrayv, GLuint first, GLsizei count, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glViewportIndexedf, GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glViewportIndexedfv, GLuint index, const GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScissorArrayv, GLuint first, GLsizei count, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScissorIndexed, GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glScissorIndexedv, GLuint index, const GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthRangeArrayv, GLuint first, GLsizei count, const GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDepthRangeIndexed, GLuint index, GLdouble n, GLdouble f) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetFloati_v, GLenum target, GLuint index, GLfloat *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetDoublei_v, GLenum target, GLuint index, GLdouble *data) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_1) \
	/* 4.2 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawArraysInstancedBaseInstance, GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsInstancedBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawElementsInstancedBaseVertexBaseInstance, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetInternalformativ, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetActiveAtomicCounterBufferiv, GLuint program, GLuint bufferIndex, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindImageTexture, GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMemoryBarrier, GLbitfield barriers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexStorage1D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawTransformFeedbackInstanced, GLenum mode, GLuint id, GLsizei instancecount) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDrawTransformFeedbackStreamInstanced, GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_2) \
	/* 4.3 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferData, GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearBufferSubData, GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDispatchCompute, GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDispatchComputeIndirect, GLintptr indirect) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyImageSubData, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFramebufferParameteri, GLenum target, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetFramebufferParameteriv, GLenum target, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetInternalformati64v, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateTexImage, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateBufferData, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateSubFramebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiDrawArraysIndirect, GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMultiDrawElementsIndirect, GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramInterfaceiv, GLuint program, GLenum programInterface, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glGetProgramResourceIndex, GLuint program, GLenum programInterface, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramResourceName, GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetProgramResourceiv, GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetProgramResourceLocation, GLuint program, GLenum programInterface, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLint, glGetProgramResourceLocationIndex, GLuint program, GLenum programInterface, const GLchar *name) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glShaderStorageBlockBinding, GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexBufferRange, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexStorage2DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTexStorage3DMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureView, GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindVertexBuffer, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribFormat, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribIFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribLFormat, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexAttribBinding, GLuint attribindex, GLuint bindingindex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexBindingDivisor, GLuint bindingindex, GLuint divisor) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDebugMessageControl, GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDebugMessageInsert, GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDebugMessageCallback, GLDEBUGPROC callback, const void *userParam) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLuint, glGetDebugMessageLog, GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPushDebugGroup, GLenum source, GLuint id, GLsizei length, const GLchar *message) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glPopDebugGroup) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glObjectLabel, GLenum identifier, GLuint name, GLsizei length, const GLchar *label) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetObjectLabel, GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glObjectPtrLabel, const void *ptr, GLsizei length, const GLchar *label) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetObjectPtrLabel, const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_3) \
	/* 4.4 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBufferStorage, GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearTexImage, GLuint texture, GLint level, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearTexSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindBuffersBase, GLenum target, GLuint first, GLsizei count, const GLuint *buffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindBuffersRange, GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindTextures, GLuint first, GLsizei count, const GLuint *textures) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindSamplers, GLuint first, GLsizei count, const GLuint *samplers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindImageTextures, GLuint first, GLsizei count, const GLuint *textures) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindVertexBuffers, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_4) \
	/* 4.5 */ \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClipControl, GLenum origin, GLenum depth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateTransformFeedbacks, GLsizei n, GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTransformFeedbackBufferBase, GLuint xfb, GLuint index, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTransformFeedbackBufferRange, GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTransformFeedbackiv, GLuint xfb, GLenum pname, GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTransformFeedbacki_v, GLuint xfb, GLenum pname, GLuint index, GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTransformFeedbacki64_v, GLuint xfb, GLenum pname, GLuint index, GLint64 *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateBuffers, GLsizei n, GLuint *buffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedBufferStorage, GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedBufferData, GLuint buffer, GLsizeiptr size, const void *data, GLenum usage) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyNamedBufferSubData, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedBufferData, GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedBufferSubData, GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void *, glMapNamedBuffer, GLuint buffer, GLenum access) \
	PRIV_SLIB_RENDER_GL_ENTRY(void *, glMapNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLboolean, glUnmapNamedBuffer, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glFlushMappedNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedBufferParameteriv, GLuint buffer, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedBufferParameteri64v, GLuint buffer, GLenum pname, GLint64 *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedBufferPointerv, GLuint buffer, GLenum pname, void **params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedBufferSubData, GLuint buffer, GLintptr offset, GLsizeiptr size, void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateFramebuffers, GLsizei n, GLuint *framebuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferRenderbuffer, GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferParameteri, GLuint framebuffer, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferTexture, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferTextureLayer, GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferDrawBuffer, GLuint framebuffer, GLenum buf) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferDrawBuffers, GLuint framebuffer, GLsizei n, const GLenum *bufs) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedFramebufferReadBuffer, GLuint framebuffer, GLenum src) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateNamedFramebufferData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glInvalidateNamedFramebufferSubData, GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedFramebufferiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedFramebufferuiv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedFramebufferfv, GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glClearNamedFramebufferfi, GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBlitNamedFramebuffer, GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLenum, glCheckNamedFramebufferStatus, GLuint framebuffer, GLenum target) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedFramebufferParameteriv, GLuint framebuffer, GLenum pname, GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedFramebufferAttachmentParameteriv, GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateRenderbuffers, GLsizei n, GLuint *renderbuffers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedRenderbufferStorage, GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glNamedRenderbufferStorageMultisample, GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetNamedRenderbufferParameteriv, GLuint renderbuffer, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateTextures, GLenum target, GLsizei n, GLuint *textures) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureBuffer, GLuint texture, GLenum internalformat, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureBufferRange, GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureStorage1D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureStorage2D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureStorage3D, GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureStorage2DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureStorage3DMultisample, GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCompressedTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTextureSubImage1D, GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCopyTextureSubImage3D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameterf, GLuint texture, GLenum pname, GLfloat param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameterfv, GLuint texture, GLenum pname, const GLfloat *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameteri, GLuint texture, GLenum pname, GLint param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameterIiv, GLuint texture, GLenum pname, const GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameterIuiv, GLuint texture, GLenum pname, const GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureParameteriv, GLuint texture, GLenum pname, const GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGenerateTextureMipmap, GLuint texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glBindTextureUnit, GLuint unit, GLuint texture) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureImage, GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetCompressedTextureImage, GLuint texture, GLint level, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureLevelParameterfv, GLuint texture, GLint level, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureLevelParameteriv, GLuint texture, GLint level, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureParameterfv, GLuint texture, GLenum pname, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureParameterIiv, GLuint texture, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureParameterIuiv, GLuint texture, GLenum pname, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureParameteriv, GLuint texture, GLenum pname, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateVertexArrays, GLsizei n, GLuint *arrays) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glDisableVertexArrayAttrib, GLuint vaobj, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glEnableVertexArrayAttrib, GLuint vaobj, GLuint index) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayElementBuffer, GLuint vaobj, GLuint buffer) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayVertexBuffer, GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayVertexBuffers, GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayAttribBinding, GLuint vaobj, GLuint attribindex, GLuint bindingindex) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayAttribFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayAttribIFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayAttribLFormat, GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glVertexArrayBindingDivisor, GLuint vaobj, GLuint bindingindex, GLuint divisor) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexArrayiv, GLuint vaobj, GLenum pname, GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexArrayIndexediv, GLuint vaobj, GLuint index, GLenum pname, GLint *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetVertexArrayIndexed64iv, GLuint vaobj, GLuint index, GLenum pname, GLint64 *param) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateSamplers, GLsizei n, GLuint *samplers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateProgramPipelines, GLsizei n, GLuint *pipelines) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glCreateQueries, GLenum target, GLsizei n, GLuint *ids) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryBufferObjecti64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryBufferObjectiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryBufferObjectui64v, GLuint id, GLuint buffer, GLenum pname, GLintptr offset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetQueryBufferObjectuiv, GLuint id, GLuint buffer, GLenum pname, GLintptr offset) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glMemoryBarrierByRegion, GLbitfield barriers) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetCompressedTextureSubImage, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(GLenum, glGetGraphicsResetStatus) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnCompressedTexImage, GLenum target, GLint lod, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnUniformdv, GLuint program, GLint location, GLsizei bufSize, GLdouble *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnUniformfv, GLuint program, GLint location, GLsizei bufSize, GLfloat *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnUniformiv, GLuint program, GLint location, GLsizei bufSize, GLint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnUniformuiv, GLuint program, GLint location, GLsizei bufSize, GLuint *params) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glReadnPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnMapdv, GLenum target, GLenum query, GLsizei bufSize, GLdouble *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnMapfv, GLenum target, GLenum query, GLsizei bufSize, GLfloat *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnMapiv, GLenum target, GLenum query, GLsizei bufSize, GLint *v) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnPixelMapfv, GLenum map, GLsizei bufSize, GLfloat *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnPixelMapuiv, GLenum map, GLsizei bufSize, GLuint *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnPixelMapusv, GLenum map, GLsizei bufSize, GLushort *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnPolygonStipple, GLsizei bufSize, GLubyte *pattern) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnColorTable, GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnConvolutionFilter, GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnSeparableFilter, GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnHistogram, GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glGetnMinmax, GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) \
	PRIV_SLIB_RENDER_GL_ENTRY(void, glTextureBarrier) \
	PRIV_SLIB_RENDER_GL_SUPPORT(Version_4_5)


namespace slib
{
	namespace priv
	{
		namespace gl
		{

			struct EntryPoints
			{
				PRIV_SLIB_RENDER_GL_ENTRIES
			};

			extern EntryPoints g_entries;

		}
	}
}

#define PRIV_GL_ENTRY(name) slib::priv::gl::g_entries.name

#elif defined(SLIB_PLATFORM_IS_MACOS)

#include <OpenGL/gl3.h>

#define PRIV_GL_ENTRY(name) name

#else

#define GL3_PROTOTYPES
#include "gl/GL/gl3.h"

#define PRIV_GL_ENTRY(name) name

#endif

#endif

#endif
