#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/path.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_UI_NAMESPACE_BEGIN

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

class _Android_GraphicsPathInstance : public GraphicsPathInstance
{
	SLIB_DECLARE_OBJECT
public:
	JniGlobal<jobject> m_path;

public:
    static Ref<_Android_GraphicsPathInstance> _create()
	{
		Ref<_Android_GraphicsPathInstance> ret;
		JniLocal<jobject> jpath = _JAndroidPath::init.newObject(sl_null);
		if (jpath.isNotNull()) {
			JniGlobal<jobject> path = jpath;
			if (path.isNotNull()) {
				ret = new _Android_GraphicsPathInstance();
				if (ret.isNotNull()) {
					ret->m_path = path;
					return ret;
				}
			}
		}
		return ret;
	}

    // override
	void moveTo(const Point& pt)
	{
		_JAndroidPath::moveTo.call(m_path, (float)(pt.x), (float)(pt.y));
	}

    // override
	void lineTo(const Point& pt)
	{
		_JAndroidPath::lineTo.call(m_path, (float)(pt.x), (float)(pt.y));
	}

    // override
	void cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd)
	{
		_JAndroidPath::cubicTo.call(m_path, (float)(ptControl1.x), (float)(ptControl1.y)
				, (float)(ptControl2.x), (float)(ptControl2.y)
				, (float)(ptEnd.x), (float)(ptEnd.y));
	}

    // override
	void closeSubpath()
	{
		_JAndroidPath::closeSubpath.call(m_path);
	}
    
    void setFillMode(FillMode mode)
	{
		_JAndroidPath::setFillMode.call(m_path, mode);
	}
    
};

SLIB_DEFINE_OBJECT(_Android_GraphicsPathInstance, GraphicsPathInstance)

jobject UIPlatform::getGraphicsPath(GraphicsPath* path, Ref<GraphicsPathInstance>& instanceOut)
{
	if (!path) {
		return 0;
	}
	Ref<GraphicsPathInstance> _instance = path->getInstance();
	Ref<_Android_GraphicsPathInstance> instance;
	if (_Android_GraphicsPathInstance::checkInstance(_instance.ptr)) {
		instance = Ref<_Android_GraphicsPathInstance>::from(_instance);
	} else {
		instance = _Android_GraphicsPathInstance::_create();
		if (instance.isNotNull()) {
			instance->buildFrom(path);
			path->setInstance(instance);
		} else {
			return 0;
		}
	}
	instanceOut = instance;
	if (instance.isNotNull()) {
		instance->setFillMode(path->getFillMode());
	}
	return instance->m_path;
}

Rectangle UI::getPathBounds(const Ref<GraphicsPath>& path)
{
	if (path.isNull()) {
		return Rectangle::zero();
	}
	Ref<GraphicsPathInstance> instance;
	jobject handle = UIPlatform::getGraphicsPath(path.ptr, instance);
	if (handle) {
		JniLocal<jobject> jrect = _JAndroidPath::getBounds.callObject(handle);
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

sl_bool UI::checkPointInPath(const Ref<GraphicsPath>& path, const Point& pt)
{
	if (path.isNull()) {
		return sl_false;
	}
	Ref<GraphicsPathInstance> instance;
	jobject handle = UIPlatform::getGraphicsPath(path.ptr, instance);
	if (handle) {
		return (_JAndroidPath::containsPoint.callBoolean(handle, pt.x, pt.y)) != 0;
	}
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
