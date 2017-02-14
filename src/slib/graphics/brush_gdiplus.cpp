#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/brush.h"

#include "../../../inc/slib/graphics/platform.h"

#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	class _Gdiplus_BrushObject : public Referable
	{
	public:
		Gdiplus::Brush* m_brush;

	public:
		_Gdiplus_BrushObject(const BrushDesc& desc)
		{
			m_brush = NULL;
			const Color& _color = desc.color;
			Gdiplus::Color color(_color.a, _color.r, _color.g, _color.b);
			if (desc.style == BrushStyle::Solid) {
				Gdiplus::Brush* brush = new Gdiplus::SolidBrush(color);
				if (brush) {
					m_brush = brush;
				}
			}
		}

		~_Gdiplus_BrushObject()
		{
			if (m_brush) {
				delete m_brush;
			}
		}

	};

	class _Brush : public Brush
	{
	public:
		_Gdiplus_BrushObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new _Gdiplus_BrushObject(m_desc);
				}
			}
			return (_Gdiplus_BrushObject*)(m_platformObject.get());;
		}

		Gdiplus::Brush* getPlatformHandle()
		{
			_Gdiplus_BrushObject* po = getPlatformObject();
			if (po) {
				return po->m_brush;
			}
			return NULL;
		}
	};

	Gdiplus::Brush* GraphicsPlatform::getBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((_Brush*)brush)->getPlatformHandle();
		}
		return NULL;
	}

}

#endif
