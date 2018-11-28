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

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_ANDROID)

#include "slib/graphics/path.h"

#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidRectF, "android/graphics/RectF")
		SLIB_JNI_FLOAT_FIELD(left);
		SLIB_JNI_FLOAT_FIELD(top);
		SLIB_JNI_FLOAT_FIELD(right);
		SLIB_JNI_FLOAT_FIELD(bottom);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidPath, "slib/platform/android/ui/UiPath")
		SLIB_JNI_NEW(init, "()V");
		SLIB_JNI_METHOD(setFillMode, "setFillMode", "(I)V");
		SLIB_JNI_METHOD(moveTo, "moveTo", "(FF)V");
		SLIB_JNI_METHOD(lineTo, "lineTo", "(FF)V");
		SLIB_JNI_METHOD(cubicTo, "cubicTo", "(FFFFFF)V");
		SLIB_JNI_METHOD(closeSubpath, "closeSubpath", "()V");
		SLIB_JNI_METHOD(getBounds, "getBounds", "()Landroid/graphics/RectF;");
		SLIB_JNI_METHOD(containsPoint, "containsPoint", "(FF)Z");
	SLIB_JNI_END_CLASS

	class _priv_Android_GraphicsPathObject : public Referable
	{
	public:
		JniGlobal<jobject> m_gpath;
		jobject path;

	public:
		_priv_Android_GraphicsPathObject()
		{
			path = 0;
			JniLocal<jobject> jpath = JAndroidPath::init.newObject(sl_null);
			if (jpath.isNotNull()) {
				JniGlobal<jobject> gpath = jpath;
				if (gpath.isNotNull()) {
					m_gpath = gpath;
					path = gpath.get();
				}
			}
		}
	};

	void GraphicsPath::_initialize_PO()
	{
		Ref<_priv_Android_GraphicsPathObject> po = new _priv_Android_GraphicsPathObject;
		if (po.isNotNull() && po->path) {
			m_platformObject = po;
		}
	}

	void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JAndroidPath::moveTo.call(po->path, (float)(x), (float)(y));
		}
	}

	void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JAndroidPath::lineTo.call(po->path, (float)(x), (float)(y));
		}
	}

	void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JAndroidPath::cubicTo.call(po->path, (float)(xc1), (float)(yc1) , (float)(xc2), (float)(yc2) , (float)(xe), (float)(ye));
		}
	}

	void GraphicsPath::_closeSubpath_PO()
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JAndroidPath::closeSubpath.call(po->path);
		}
	}

	void GraphicsPath::_setFillMode_PO(FillMode mode)
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JAndroidPath::setFillMode.call(po->path, (int)mode);
		}
	}

	Rectangle GraphicsPath::_getBounds_PO()
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			JniLocal<jobject> jrect = JAndroidPath::getBounds.callObject(po->path);
			if (jrect.isNotNull()) {
				Rectangle ret;
				ret.left = JAndroidRectF::left.get(jrect);
				ret.top = JAndroidRectF::top.get(jrect);
				ret.right = JAndroidRectF::right.get(jrect);
				ret.bottom = JAndroidRectF::bottom.get(jrect);
				return ret;
			}
		}
		return Rectangle::zero();
	}

	sl_bool GraphicsPath::_containsPoint_PO(sl_real x, sl_real y)
	{
		_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			return (JAndroidPath::containsPoint.callBoolean(po->path, x, y)) != 0;
		}
		return sl_false;
	}

	class _priv_GraphicsPath : public GraphicsPath
	{
	public:
		jobject getPlatformPath()
		{
			_priv_Android_GraphicsPathObject* po = (_priv_Android_GraphicsPathObject*)(m_platformObject.get());
			if (po) {
				return po->path;
			}
			return 0;
		}
	};

	jobject GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
	{
		if (path) {
			return ((_priv_GraphicsPath*)path)->getPlatformPath();
		}
		return 0;
	}

}

#endif
