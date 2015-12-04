#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/path.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

class _Quartz_GraphicsPathInstance : public GraphicsPathInstance
{
	SLIB_DECLARE_OBJECT(_Quartz_GraphicsPathInstance, GraphicsPathInstance)
public:
	CGMutablePathRef m_path;
	sl_bool m_flagMoved;
	sl_real m_moveX;
	sl_real m_moveY;
	
	_Quartz_GraphicsPathInstance()
	{
		m_flagMoved = sl_false;
		m_moveX = 0;
		m_moveY = 0;
	}
	
	~_Quartz_GraphicsPathInstance()
	{
		CGPathRelease(m_path);
	}
	
	static Ref<_Quartz_GraphicsPathInstance> _create()
	{
		Ref<_Quartz_GraphicsPathInstance> ret;
		CGMutablePathRef path = CGPathCreateMutable();
		if (path) {
			ret = new _Quartz_GraphicsPathInstance();
			if (ret.isNotNull()) {
				ret->m_path = path;
				return ret;
			}
			CGPathRelease(path);
		}
		return ret;
	}
	
	void moveTo(const Point& pt)
	{
		CGPathMoveToPoint(m_path, NULL, pt.x, pt.y);
		m_flagMoved = sl_true;
		m_moveX = pt.x;
		m_moveY = pt.y;
	}
	
	void lineTo(const Point& pt)
	{
		if (!m_flagMoved) {
			CGPathMoveToPoint(m_path, NULL, m_moveX, m_moveY);
		}
		CGPathAddLineToPoint(m_path, NULL, pt.x, pt.y);
		m_flagMoved = sl_true;
		m_moveX = pt.x;
		m_moveY = pt.y;
	}
	
	void cubicTo(const Point& ptControl1, const Point& ptControl2, const Point& ptEnd)
	{
		if (!m_flagMoved) {
			CGPathMoveToPoint(m_path, NULL, m_moveX, m_moveY);
		}
		CGPathAddCurveToPoint(m_path, NULL, ptControl1.x, ptControl1.y, ptControl2.x, ptControl2.y, ptEnd.x, ptEnd.y);
		m_flagMoved = sl_true;
		m_moveX = ptEnd.x;
		m_moveY = ptEnd.y;
	}
	
	void closeSubpath()
	{
		CGPathCloseSubpath(m_path);
		m_flagMoved = sl_false;
	}
	
};

CGPathRef UIPlatform::getGraphicsPath(GraphicsPath* path, Ref<GraphicsPathInstance>& instanceOut)
{
	if (!path) {
		return NULL;
	}
	Ref<GraphicsPathInstance> _instance = path->getInstance();
	Ref<_Quartz_GraphicsPathInstance> instance;
	if (_Quartz_GraphicsPathInstance::checkInstance(_instance)) {
		instance = Ref<_Quartz_GraphicsPathInstance>::from(_instance);
	} else {
		instance = _Quartz_GraphicsPathInstance::_create();
		if (instance.isNotNull()) {
			instance->buildFrom(path);
			path->setInstance(instance);
		} else {
			return NULL;
		}
	}
	instanceOut = instance;
	return instance->m_path;
}

Rectangle UI::getPathBounds(const Ref<GraphicsPath>& _path)
{
	Ref<GraphicsPath> path = _path;
	if (path.isNull()) {
		return Rectangle::zero();
	}
	Ref<GraphicsPathInstance> instance;
	CGPathRef handle = UIPlatform::getGraphicsPath(path.get(), instance);
	if (handle) {
		Rectangle ret;
		CGRect rc = CGPathGetPathBoundingBox(handle);
		ret.left = (sl_real)(rc.origin.x);
		ret.top = (sl_real)(rc.origin.y);
		ret.right = ret.left + (sl_real)(rc.size.width);
		ret.bottom = ret.top + (sl_real)(rc.size.height);
		return ret;
	}
	return Rectangle::zero();
}

sl_bool UI::checkPointInPath(const Ref<GraphicsPath>& _path, const Point& _pt)
{
	Ref<GraphicsPath> path = _path;
	if (path.isNull()) {
		return sl_false;
	}
	Ref<GraphicsPathInstance> instance;
	CGPathRef handle = UIPlatform::getGraphicsPath(path.get(), instance);
	if (handle) {
		CGPoint pt;
		pt.x = _pt.x;
		pt.y = _pt.y;
		return CGPathContainsPoint(handle, NULL, pt, path->getFillMode() != fillModeWinding);
	}
	return sl_false;
}
SLIB_UI_NAMESPACE_END

#endif
