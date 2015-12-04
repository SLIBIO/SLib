#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/canvas.h"

#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/core/endian.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPointF, "android/graphics/PointF")
	SLIB_JNI_FLOAT_FIELD(x);
	SLIB_JNI_FLOAT_FIELD(y);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidPen, "slib/platform/android/ui/UiPen")
	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_METHOD(setStyle, "setStyle", "(I)V");
	SLIB_JNI_INT_FIELD(cap);
	SLIB_JNI_INT_FIELD(join);
	SLIB_JNI_FLOAT_FIELD(width);
	SLIB_JNI_INT_FIELD(color);
	SLIB_JNI_FLOAT_FIELD(miterLimit);
SLIB_JNI_END_CLASS


class _Android_PenInstance : public PenInstance
{
	SLIB_DECLARE_OBJECT(_Android_PenInstance, PenInstance)
public:
	JniGlobal<jobject> m_pen;

	_Android_PenInstance()
	{
	}

	static Ref<_Android_PenInstance> _create(const PenDesc& desc)
	{
		Ref<_Android_PenInstance> ret;
		JniLocal<jobject> jpen = _JAndroidPen::init.newObject(sl_null);
		if (jpen.isNotNull()) {
			JniGlobal<jobject> gpen = jpen;
			if (gpen.isNotNull()) {
				_JAndroidPen::cap.set(jpen, desc.cap);
				_JAndroidPen::join.set(jpen, desc.join);
				_JAndroidPen::color.set(jpen, desc.color.getARGB());
				_JAndroidPen::width.set(jpen, desc.width);
				_JAndroidPen::miterLimit.set(jpen, desc.miterLimit);
				_JAndroidPen::setStyle.call(jpen, desc.style);
				ret = new _Android_PenInstance();
				if (ret.isNotNull()) {
					ret->m_pen = gpen;
					return ret;
				}
			}
		}
		return ret;
	}
};


SLIB_JNI_BEGIN_CLASS(_JAndroidBrush, "slib/platform/android/ui/UiBrush")
	SLIB_JNI_NEW(init, "()V");
	SLIB_JNI_INT_FIELD(style);
	SLIB_JNI_INT_FIELD(color);
SLIB_JNI_END_CLASS


class _Android_BrushInstance : public BrushInstance
{
	SLIB_DECLARE_OBJECT(_Android_BrushInstance, BrushInstance)
public:
	JniGlobal<jobject> m_brush;

	_Android_BrushInstance()
	{
	}

	static Ref<_Android_BrushInstance> _create(const BrushDesc& desc)
	{
		Ref<_Android_BrushInstance> ret;
		JniLocal<jobject> jbrush = _JAndroidBrush::init.newObject(sl_null);
		if (jbrush.isNotNull()) {
			JniGlobal<jobject> gbrush = jbrush;
			if (gbrush.isNotNull()) {
				_JAndroidBrush::color.set(jbrush, desc.color.getARGB());
				_JAndroidBrush::style.set(jbrush, desc.style);
				ret = new _Android_BrushInstance();
				if (ret.isNotNull()) {
					ret->m_brush = gbrush;
					return ret;
				}
			}
		}
		return ret;
	}
};

SLIB_JNI_BEGIN_CLASS(_JAndroidGraphics, "slib/platform/android/ui/Graphics")
	SLIB_JNI_BOOLEAN_FIELD(flagAntiAlias);
	SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
	SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
	SLIB_JNI_METHOD(save, "save", "()V");
	SLIB_JNI_METHOD(restore, "restore", "()V");
	SLIB_JNI_METHOD(clipToRectangle, "clipToRectangle", "(FFFF)V");
	SLIB_JNI_METHOD(clipToPath, "clipToPath", "(Lslib/platform/android/ui/UiPath;)V");
	SLIB_JNI_METHOD(concatMatrix, "concatMatrix", "(FFFFFFFFF)V");
	SLIB_JNI_METHOD(drawText, "drawText", "(Ljava/lang/String;FFLslib/platform/android/ui/UiFont;I)V");
	SLIB_JNI_METHOD(drawLine, "drawLine", "(FFFFLslib/platform/android/ui/UiPen;)V");
	SLIB_JNI_METHOD(drawLines, "drawLines", "([FLslib/platform/android/ui/UiPen;)V");
	SLIB_JNI_METHOD(drawArc, "drawArc", "(FFFFFFLslib/platform/android/ui/UiPen;)V");
	SLIB_JNI_METHOD(drawRectangle, "drawRectangle", "(FFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
	SLIB_JNI_METHOD(drawRoundRectangle, "drawRoundRectangle", "(FFFFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
	SLIB_JNI_METHOD(drawEllipse, "drawEllipse", "(FFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
	SLIB_JNI_METHOD(drawPolygon, "drawPolygon", "([FLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;I)V");
	SLIB_JNI_METHOD(drawPie, "drawPie", "(FFFFFFLslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
	SLIB_JNI_METHOD(drawPath, "drawPath", "(Lslib/platform/android/ui/UiPath;Lslib/platform/android/ui/UiPen;Lslib/platform/android/ui/UiBrush;)V");
SLIB_JNI_END_CLASS

class _Android_Canvas : public Canvas
{
	SLIB_DECLARE_OBJECT(_Android_Canvas, Canvas)
public:
	JniGlobal<jobject> m_canvas;
	int m_width;
	int m_height;

	_Android_Canvas()
	{
	}

	static Ref<_Android_Canvas> create(jobject jcanvas) {
		Ref<_Android_Canvas> ret;
		JniGlobal<jobject> canvas = jcanvas;
		if (canvas.isNotNull()) {
			int width = _JAndroidGraphics::getWidth.callInt(jcanvas);
			int height = _JAndroidGraphics::getHeight.callInt(jcanvas);
			ret = new _Android_Canvas();
			if (ret.isNotNull()) {
                ret->setGraphicsContext(UI::getGraphicsContext());
				ret->m_canvas = canvas;
				ret->m_width = width;
				ret->m_height = height;
			}
		}
		return ret;
	}

	Size getSize()
	{
		return Size((sl_real)(m_width), (sl_real)(m_height));
	}

	Rectangle getInvalidatedRect()
	{
		return Rectangle(0, 0, (sl_real)(m_width), (sl_real)(m_height));
	}

	void save()
	{
		_JAndroidGraphics::save.call(m_canvas);
	}
	void restore()
	{
		_JAndroidGraphics::restore.call(m_canvas);
	}

	sl_bool isAntiAlias()
	{
		return _JAndroidGraphics::flagAntiAlias.get(m_canvas);
	}

	void setAntiAlias(sl_bool flag)
	{
		_JAndroidGraphics::flagAntiAlias.set(m_canvas, flag);
	}

	void clipToRectangle(const Rectangle& _rect)
	{
		_JAndroidGraphics::clipToRectangle.call(m_canvas, (float)(_rect.left), (float)(_rect.top), (float)(_rect.right), (float)(_rect.bottom));
	}

	void clipToPath(const Ref<GraphicsPath>& _path)
	{
		Ref<GraphicsPath> path = _path;
		Ref<GraphicsPathInstance> instance;
		jobject handle = UIPlatform::getGraphicsPath(path.get(), instance);
		if (handle) {
			_JAndroidGraphics::clipToPath.call(m_canvas, handle);
		}
	}

	void concatMatrix(const Matrix3& matrix)
	{
		_JAndroidGraphics::concatMatrix.call(m_canvas
				, (float)(matrix.m00), (float)(matrix.m10), (float)(matrix.m20)
				, (float)(matrix.m01), (float)(matrix.m11), (float)(matrix.m21)
				, (float)(matrix.m02), (float)(matrix.m12), (float)(matrix.m22));
	}

	void drawText(const String& text, sl_real x, sl_real y, const Ref<Font>& _font, const Color& color)
	{
		if (text.isNotEmpty()) {
			Ref<Font> font = _font;
			if (font.isNull()) {
				font = Font::getDefault();
			}
			Ref<FontInstance> fontInstance;
			jobject hFont = UIPlatform::getNativeFont(font.get(), fontInstance);
			if (hFont) {
				JniLocal<jstring> jtext = Jni::getJniString(text);
				_JAndroidGraphics::drawText.call(m_canvas.get(), jtext.value, (float)x, (float)y, hFont, color.getARGB());
			}
		}
	}

	void drawLine(const Point& pt1, const Point& pt2, const Ref<Pen>& _pen)
	{
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		if (hPen) {
			_JAndroidGraphics::drawLine.call(m_canvas.get()
					, (float)(pt1.x), (float)(pt1.y), (float)(pt2.x), (float)(pt2.y)
					, hPen);
		}
	}

	void drawLines(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen)
	{
		if (countPoints < 2) {
			return;
		}
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		if (hPen) {
			JniLocal<jfloatArray> jarr = Jni::newFloatArray(countPoints*2);
			if (jarr.isNotNull()) {
				Jni::setFloatArrayRegion(jarr, 0, countPoints*2, (jfloat*)(points));
				_JAndroidGraphics::drawLines.call(m_canvas.get(), jarr.value, hPen);
			}
		}
	}

	void drawArc(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen)
	{
		Ref<Pen> pen = _pen;
		if (pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		if (hPen) {
			_JAndroidGraphics::drawArc.call(m_canvas.get()
					, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
					, (float)(startDegrees), (float)(endDegrees)
					, hPen);
		}
	}

	void drawRectangle(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& _brush)
	{
		Ref<Brush> brush = _brush;
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		Ref<BrushInstance> brushInstance;
		jobject hBrush = _getBrush(brush.get(), brushInstance);
		if (hPen || hBrush) {
			_JAndroidGraphics::drawRectangle.call(m_canvas
					, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
					, hPen, hBrush);
		}
	}

	void drawRoundRect(const Rectangle& rect, const Size& radius, const Ref<Pen>& _pen, const Ref<Brush>& _brush)
	{
		Ref<Brush> brush = _brush;
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		Ref<BrushInstance> brushInstance;
		jobject hBrush = _getBrush(brush.get(), brushInstance);
		if (hPen || hBrush) {
			_JAndroidGraphics::drawRoundRectangle.call(m_canvas
					, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
					, (float)(radius.width), (float)(radius.height), hPen, hBrush);
		}
	}

	void drawEllipse(const Rectangle& rect, const Ref<Pen>& _pen, const Ref<Brush>& _brush)
	{
		Ref<Brush> brush = _brush;
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		Ref<BrushInstance> brushInstance;
		jobject hBrush = _getBrush(brush.get(), brushInstance);
		if (hPen || hBrush) {
			_JAndroidGraphics::drawEllipse.call(m_canvas
					, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
					, hPen, hBrush);
		}
	}

	void drawPolygon(const Point* points, sl_uint32 countPoints, const Ref<Pen>& _pen, const Ref<Brush>& _brush, FillMode fillMode)
	{
		if (countPoints <= 2) {
			return;
		}
		Ref<Brush> brush = _brush;
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		Ref<BrushInstance> brushInstance;
		jobject hBrush = _getBrush(brush.get(), brushInstance);
		if (hPen || hBrush) {
			JniLocal<jfloatArray> jarr = Jni::newFloatArray(countPoints*2);
			if (jarr.isNotNull()) {
				Jni::setFloatArrayRegion(jarr, 0, countPoints*2, (jfloat*)(points));
				_JAndroidGraphics::drawPolygon.call(m_canvas, jarr.value, hPen, hBrush, fillMode);
			}
		}
	}

	void drawPie(const Rectangle& rect, sl_real startDegrees, sl_real endDegrees, const Ref<Pen>& _pen, const Ref<Brush>& _brush)
	{
		Ref<Brush> brush = _brush;
		Ref<Pen> pen = _pen;
		if (brush.isNull() && pen.isNull()) {
			pen = Pen::getDefault();
		}
		Ref<PenInstance> penInstance;
		jobject hPen = _getPen(pen.get(), penInstance);
		Ref<BrushInstance> brushInstance;
		jobject hBrush = _getBrush(brush.get(), brushInstance);
		if (hPen || hBrush) {
			_JAndroidGraphics::drawPie.call(m_canvas
					, (float)(rect.left), (float)(rect.top), (float)(rect.right), (float)(rect.bottom)
					, (float)(startDegrees), (float)(endDegrees)
					, hPen, hBrush);
		}
	}

	void drawPath(const Ref<GraphicsPath>& _path, const Ref<Pen>& _pen, const Ref<Brush>& _brush)
	{
		Ref<GraphicsPath> path = _path;
		Ref<GraphicsPathInstance> pathInstance;
		jobject hPath = UIPlatform::getGraphicsPath(path.get(), pathInstance);
		if (hPath) {
			Ref<Brush> brush = _brush;
			Ref<Pen> pen = _pen;
			if (brush.isNull() && pen.isNull()) {
				pen = Pen::getDefault();
			}
			Ref<PenInstance> penInstance;
			jobject hPen = _getPen(pen.get(), penInstance);
			Ref<BrushInstance> brushInstance;
			jobject hBrush = _getBrush(brush.get(), brushInstance);
			if (hPen || hBrush) {
				_JAndroidGraphics::drawPath.call(m_canvas.get()
						, hPath
						, hPen, hBrush);
			}
		}
	}

	jobject _getPen(Pen* pen, Ref<PenInstance>& instanceOut)
	{
		if (pen) {
			Ref<PenInstance> _instance = pen->getInstance();
			Ref<_Android_PenInstance> instance;
			if (_Android_PenInstance::checkInstance(_instance)) {
				instance = Ref<_Android_PenInstance>::from(_instance);
			}
			if (instance.isNull()) {
				PenDesc desc;
				pen->getDesc(desc);
				instance = _Android_PenInstance::_create(desc);
				if (instance.isNotNull()) {
					pen->setInstance(instance);
				}
			}
			instanceOut = instance;
			if (instance.isNotNull()) {
				return instance->m_pen;
			}
		}
		return sl_null;
	}

	jobject _getBrush(Brush* brush, Ref<BrushInstance>& instanceOut)
	{
		if (brush) {
			Ref<BrushInstance> _instance = brush->getInstance();
			Ref<_Android_BrushInstance> instance;
			if (_Android_BrushInstance::checkInstance(_instance)) {
				instance = Ref<_Android_BrushInstance>::from(_instance);
			}
			if (instance.isNull()) {
				BrushDesc desc;
				brush->getDesc(desc);
				instance = _Android_BrushInstance::_create(desc);
				if (instance.isNotNull()) {
					brush->setInstance(instance);
				}
			}
			instanceOut = instance;
			if (instance.isNotNull()) {
				return instance->m_brush;
			}
		}
		return sl_null;
	}
};

Ref<Canvas> UIPlatform::createCanvas(jobject jcanvas)
{
	if (!jcanvas) {
		return Ref<Canvas>::null();
	}
	Ref<_Android_Canvas> canvas = _Android_Canvas::create(jcanvas);
	return Ref<Canvas>::from(canvas);
}

jobject UIPlatform::getCanvasHandle(Canvas* _canvas)
{
	if (_Android_Canvas::checkInstance(_canvas)) {
		_Android_Canvas* canvas = (_Android_Canvas*)_canvas;
		return canvas->m_canvas;
	} else {
		return 0;
	}
}

SLIB_UI_NAMESPACE_END
#endif
