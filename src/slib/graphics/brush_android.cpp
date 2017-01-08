#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/brush.h"
#include "../../../inc/slib/graphics/platform.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidBrush, "slib/platform/android/ui/UiBrush")
	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_INT_FIELD(style);
	SLIB_JNI_INT_FIELD(color);
SLIB_JNI_END_CLASS

class _Android_BrushObject : public Referable
{
public:
	JniGlobal<jobject> m_brush;

public:
	_Android_BrushObject(const BrushDesc& desc)
	{
		JniLocal<jobject> jbrush = _JAndroidBrush::init.newObject(sl_null);
		if (jbrush.isNotNull()) {
			JniGlobal<jobject> gbrush = jbrush;
			if (gbrush.isNotNull()) {
				_JAndroidBrush::color.set(jbrush, desc.color.getARGB());
				_JAndroidBrush::style.set(jbrush, (int)(desc.style));
				m_brush = gbrush;
			}
		}
	}
};

class _Brush : public Brush
{
public:
	_Android_BrushObject* getPlatformObject()
	{
		if (m_platformObject.isNull()) {
			SpinLocker lock(&m_lock);
			if (m_platformObject.isNull()) {
				m_platformObject = new _Android_BrushObject(m_desc);
			}
		}
		return (_Android_BrushObject*)(m_platformObject.get());;
	}

	jobject getPlatformHandle()
	{
		_Android_BrushObject* po = getPlatformObject();
		if (po) {
			return po->m_brush.get();
		}
		return 0;
	}
};

jobject GraphicsPlatform::getBrushHandle(Brush* brush)
{
	if (brush) {
		return ((_Brush*)brush)->getPlatformHandle();
	}
	return 0;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
