#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/path.h"
#include "../../../inc/slib/graphics/platform.h"

#include "../../../inc/slib/core/scoped.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidRectF, "android/graphics/RectF")
	SLIB_JNI_FLOAT_FIELD(left);
	SLIB_JNI_FLOAT_FIELD(top);
	SLIB_JNI_FLOAT_FIELD(right);
	SLIB_JNI_FLOAT_FIELD(bottom);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidPath, "slib/platform/android/ui/UiPath")
	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_METHOD(setFillMode, "setFillMode", "(I)V");
	SLIB_JNI_METHOD(moveTo, "moveTo", "(FF)V");
	SLIB_JNI_METHOD(lineTo, "lineTo", "(FF)V");
	SLIB_JNI_METHOD(cubicTo, "cubicTo", "(FFFFFF)V");
	SLIB_JNI_METHOD(closeSubpath, "closeSubpath", "()V");
	SLIB_JNI_METHOD(getBounds, "getBounds", "()Landroid/graphics/RectF;");
	SLIB_JNI_METHOD(containsPoint, "containsPoint", "(FF)Z");
SLIB_JNI_END_CLASS

class _Android_GraphicsPathObject : public Referable
{
public:
	JniGlobal<jobject> m_gpath;
	jobject path;

public:
	_Android_GraphicsPathObject()
	{
		path = 0;
		JniLocal<jobject> jpath = _JAndroidPath::init.newObject(sl_null);
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
	Ref<_Android_GraphicsPathObject> po = new _Android_GraphicsPathObject;
	if (po.isNotNull() && po->path) {
		m_platformObject = po;
	}
}

void GraphicsPath::_moveTo_PO(sl_real x, sl_real y)
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		_JAndroidPath::moveTo.call(po->path, (float)(x), (float)(y));
	}
}

void GraphicsPath::_lineTo_PO(sl_real x, sl_real y)
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		_JAndroidPath::lineTo.call(po->path, (float)(x), (float)(y));
	}
}

void GraphicsPath::_cubicTo_PO(sl_real xc1, sl_real yc1, sl_real xc2, sl_real yc2, sl_real xe, sl_real ye)
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		_JAndroidPath::cubicTo.call(po->path, (float)(xc1), (float)(yc1) , (float)(xc2), (float)(yc2) , (float)(xe), (float)(ye));
	}
}

void GraphicsPath::_closeSubpath_PO()
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		_JAndroidPath::closeSubpath.call(po->path);
	}
}

void GraphicsPath::_setFillMode_PO(FillMode mode)
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		_JAndroidPath::setFillMode.call(po->path, (int)mode);
	}
}

Rectangle GraphicsPath::_getBounds_PO()
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		JniLocal<jobject> jrect = _JAndroidPath::getBounds.callObject(po->path);
		if (jrect.isNotNull()) {
			Rectangle ret;
			ret.left = _JAndroidRectF::left.get(jrect);
			ret.top = _JAndroidRectF::top.get(jrect);
			ret.right = _JAndroidRectF::right.get(jrect);
			ret.bottom = _JAndroidRectF::bottom.get(jrect);
			return ret;
		}
	}
	return Rectangle::zero();
}

sl_bool GraphicsPath::_containsPoint_PO(sl_real x, sl_real y)
{
	_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
	if (po) {
		return (_JAndroidPath::containsPoint.callBoolean(po->path, x, y)) != 0;
	}
	return sl_false;
}

class _GraphicsPath : public GraphicsPath
{
public:
	jobject getPlatformPath()
	{
		_Android_GraphicsPathObject* po = (_Android_GraphicsPathObject*)(m_platformObject.get());
		if (po) {
			return po->path;
		}
		return 0;
	}
};

jobject GraphicsPlatform::getGraphicsPath(GraphicsPath* path)
{
	if (path) {
		return ((_GraphicsPath*)path)->getPlatformPath();
	}
	return 0;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
