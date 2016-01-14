#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPointF, "android/graphics/PointF")
	SLIB_JNI_FLOAT_FIELD(x);
	SLIB_JNI_FLOAT_FIELD(y);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidRectF, "android/graphics/RectF")
	SLIB_JNI_FLOAT_FIELD(left);
	SLIB_JNI_FLOAT_FIELD(top);
	SLIB_JNI_FLOAT_FIELD(right);
	SLIB_JNI_FLOAT_FIELD(bottom);
SLIB_JNI_END_CLASS

void JNICALL _AndroidWindow_nativeOnResize(JNIEnv* env, jobject _this, jlong instance, float w, float h);
jboolean JNICALL _AndroidWindow_nativeOnClose(JNIEnv* env, jobject _this, jlong instance);

SLIB_JNI_BEGIN_CLASS(_JAndroidWindow, "slib/platform/android/ui/window/UiWindow")

	SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZZFFFF)Lslib/platform/android/ui/window/UiWindow;");

	SLIB_JNI_LONG_FIELD(instance);

	SLIB_JNI_METHOD(getContentView, "getContentView", "()Landroid/view/View;");
	SLIB_JNI_METHOD(close, "close", "()V");
	SLIB_JNI_METHOD(getBackgroundColor, "getWindowBackgroundColor", "()I");
	SLIB_JNI_METHOD(setBackgroundColor, "setWindowBackgroundColor", "(I)V");
	SLIB_JNI_METHOD(getFrame, "getFrame", "()Landroid/graphics/RectF;");
	SLIB_JNI_METHOD(setFrame, "setFrame", "(FFFF)V");
	SLIB_JNI_METHOD(getSize, "getSize", "()Landroid/graphics/PointF;");
	SLIB_JNI_METHOD(setSize, "setSize", "(FF)V");
	SLIB_JNI_METHOD(isVisible, "isVisible", "()Z");
	SLIB_JNI_METHOD(setVisible, "setVisible", "(Z)V");
	SLIB_JNI_METHOD(isAlwaysOnTop, "isAlwaysOnTop", "()Z");
	SLIB_JNI_METHOD(setAlwaysOnTop, "setAlwaysOnTop", "(Z)V");
	SLIB_JNI_METHOD(getAlpha, "getWindowAlpha", "()F");
	SLIB_JNI_METHOD(setAlpha, "setWindowAlpha", "(F)V");
	SLIB_JNI_METHOD(focus, "focus", "()V");
	SLIB_JNI_METHOD(convertCoordinateFromScreenToWindow, "convertCoordinateFromScreenToWindow", "(FF)Landroid/graphics/PointF;");
	SLIB_JNI_METHOD(convertCoordinateFromWindowToScreen, "convertCoordinateFromWindowToScreen", "(FF)Landroid/graphics/PointF;");

	SLIB_JNI_NATIVE(onResize, "nativeOnResize", "(JFF)V", _AndroidWindow_nativeOnResize);
	SLIB_JNI_NATIVE(onClose, "nativeOnClose", "(J)Z", _AndroidWindow_nativeOnClose);

SLIB_JNI_END_CLASS
	
class _Android_Window : public WindowInstance
{
public:
	JniSafeGlobal<jobject> m_window;
	Ref<ViewInstance> m_viewContent;

	_Android_Window()
	{
	}

	~_Android_Window()
	{
		close();
	}

	static Ref<_Android_Window> create(jobject jwindow)
	{
		if (!jwindow) {
			return Ref<_Android_Window>::null();
		}
		JniLocal<jobject> jcontent = _JAndroidWindow::getContentView.callObject(jwindow);
		Ref<_Android_Window> ret = new _Android_Window();
		if (ret.isNotNull()) {
			JniGlobal<jobject> window = jwindow;
			Ref<ViewInstance> content = UIPlatform::createViewInstance(jcontent);
			if (window.isNotNull() && content.isNotNull()) {
				ret->m_window = window;
				ret->m_viewContent = content;
				jlong instance = (jlong)(window.get());
				_JAndroidWindow::instance.set(jwindow, instance);
				return ret;
			}
		}
		return Ref<_Android_Window>::null();
	}

	static jobject createHandle(const WindowInstanceParam& param)
	{
		jobject jactivity = (jobject)(param.activity);
		if (!jactivity) {
			jactivity = Android::getCurrentActivity();
			if (!jactivity) {
				return 0;
			}
		}
		jobject jwindow = _JAndroidWindow::create.callObject(sl_null, jactivity
			, param.flagFullScreen, param.flagCenterScreen
			, param.location.x, param.location.y, param.size.x, param.size.y);
		return jwindow;
	}

	const Ref<ViewInstance>& getContentView()
	{
		return m_viewContent;
	}

	void close()
	{
		ObjectLocker lock(this);
		m_viewContent.setNull();
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			UIPlatform::removeWindowInstance(jwindow);
			_JAndroidWindow::close.call(jwindow);
			m_window.setNull();
		}
	}

	sl_bool isClosed()
	{
		return m_window.isNull();
	}

	Ref<WindowInstance> getParent()
	{
		return Ref<WindowInstance>::null();
	}

	sl_bool setParent(const Ref<WindowInstance>& window)
	{
		return sl_false;
	}
	
	sl_bool setFocus()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::focus.call(jwindow);
			return sl_true;
		}
		return sl_false;
	}

	String getTitle()
	{
		return String::null();
	}

	sl_bool setTitle(const String& title)
	{
		return sl_false;
	}

	Color getBackgroundColor()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			int color = _JAndroidWindow::getBackgroundColor.callInt(jwindow);
			Color ret;
			ret.setARGB(color);
			return ret;
		}
		return Color::zero();
	}

	sl_bool setBackgroundColor(const Color& _color)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			Color color = _color;
			if (color.isNotZero()) {
				_JAndroidWindow::setBackgroundColor.call(jwindow, color.getARGB());
				return sl_true;
			}
		}
		return sl_false;
	}

	Rectangle getFrame()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			JniLocal<jobject> rect = _JAndroidWindow::getFrame.callObject(jwindow);
			if (rect.isNotNull()) {
				Rectangle ret;
				ret.left = _JAndroidRectF::left.get(rect);
				ret.top = _JAndroidRectF::top.get(rect);
				ret.right = _JAndroidRectF::right.get(rect);
				ret.bottom = _JAndroidRectF::bottom.get(rect);
				return ret;
			}
		}
		return Rectangle::zero();
	}

	sl_bool setFrame(const Rectangle& frame)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::setFrame.call(jwindow, frame.left, frame.top, frame.right, frame.bottom);
			return sl_true;
		}
		return sl_false;
	}

	Rectangle getClientFrame()
	{
		return getFrame();
	}

	Size getClientSize()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			JniLocal<jobject> size = _JAndroidWindow::getSize.callObject(jwindow);
			if (size.isNotNull()) {
				Size ret;
				ret.x = _JAndroidPointF::x.get(size);
				ret.y = _JAndroidPointF::y.get(size);
				return ret;
			}
		}
		return Size::zero();
	}

	sl_bool setClientSize(const Size& size)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::setSize.call(jwindow, size.x, size.y);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool isMinimized()
	{
		return sl_false;
	}

	sl_bool setMinimized(sl_bool flag)
	{
		return sl_false;
	}

	sl_bool isMaximized()
	{
		return sl_false;
	}

	sl_bool setMaximized(sl_bool flag)
	{
		return sl_false;
	}

	sl_bool isVisible()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			return _JAndroidWindow::isVisible.callBoolean(jwindow) != 0;
		}
		return sl_false;
	}

	sl_bool setVisible(sl_bool flag)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::setVisible.call(jwindow, flag);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool isAlwaysOnTop()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			return _JAndroidWindow::isAlwaysOnTop.callBoolean(jwindow) != 0;
		}
		return sl_false;
	}

	sl_bool setAlwaysOnTop(sl_bool flag)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::setAlwaysOnTop.call(jwindow, flag);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool isCloseButtonEnabled()
	{
		return sl_false;
	}

	sl_bool setCloseButtonEnabled(sl_bool flag)
	{
		return sl_false;
	}

	sl_bool isMinimizeButtonEnabled()
	{
		return sl_false;
	}

	sl_bool setMinimizeButtonEnabled(sl_bool flag)
	{
		return sl_false;
	}

	sl_bool isMaximizeButtonEnabled()
	{
		return sl_false;
	}

	sl_bool setMaximizeButtonEnabled(sl_bool flag)
	{
		return sl_false;
	}

	sl_bool isResizable()
	{
		return sl_false;
	}

	sl_bool setResizable(sl_bool flag)
	{
		return sl_false;
	}

	sl_real getAlpha()
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			return (sl_real)(_JAndroidWindow::getAlpha.callFloat(jwindow));
		}
		return sl_false;
	}

	sl_bool setAlpha(sl_real alpha)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			_JAndroidWindow::setAlpha.call(jwindow, (jfloat)alpha);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool isTransparent()
	{
		return sl_false;
	}

	sl_bool setTransparent(sl_bool flag)
	{
		return sl_false;
	}

	Point convertCoordinateFromScreenToWindow(const Point& ptScreen)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			JniLocal<jobject> jpt = _JAndroidWindow::convertCoordinateFromScreenToWindow.callObject(jwindow, ptScreen.x, ptScreen.y);
			if (jpt.isNotNull()) {
				Point ret;
				ret.x = _JAndroidPointF::x.get(jpt);
				ret.y = _JAndroidPointF::y.get(jpt);
				return ret;
			}
		}
		return ptScreen;
	}

	Point convertCoordinateFromWindowToScreen(const Point& ptWindow)
	{
		JniGlobal<jobject> _jwindow(m_window);
		jobject jwindow = _jwindow;
		if (jwindow) {
			JniLocal<jobject> jpt = _JAndroidWindow::convertCoordinateFromWindowToScreen.callObject(jwindow, ptWindow.x, ptWindow.y);
			if (jpt.isNotNull()) {
				Point ret;
				ret.x = _JAndroidPointF::x.get(jpt);
				ret.y = _JAndroidPointF::y.get(jpt);
				return ret;
			}
		}
		return ptWindow;
	}

	Point convertCoordinateFromScreenToClient(const Point& ptScreen)
	{
		return convertCoordinateFromScreenToWindow(ptScreen);
	}

	Point convertCoordinateFromClientToScreen(const Point& ptClient)
	{
		return convertCoordinateFromWindowToScreen(ptClient);
	}

	Point convertCoordinateFromWindowToClient(const Point& ptWindow)
	{
		return ptWindow;
	}

	Point convertCoordinateFromClientToWindow(const Point& ptClient)
	{
		return ptClient;
	}

	Size getWindowSizeFromClientSize(const Size& size)
	{
		return size;
	}

	Size getClientSizeFromWindowSize(const Size& size)
	{
		return size;
	}

	void runModal()
	{
	}
};

Ref<_Android_Window> _AndroidUi_getWindow(jlong instance)
{
	return Ref<_Android_Window>::from(UIPlatform::getWindowInstance((jobject)instance));
}

void JNICALL _AndroidWindow_nativeOnResize(JNIEnv* env, jobject _this, jlong instance, float w, float h)
{
	Ref<_Android_Window> window = _AndroidUi_getWindow(instance);
	if (window.isNotNull()) {
		Size size(w, h);
		window->onResize(size);
	}
}

jboolean JNICALL _AndroidWindow_nativeOnClose(JNIEnv* env, jobject _this, jlong instance)
{
	Ref<_Android_Window> window = _AndroidUi_getWindow(instance);
	if (window.isNotNull()) {
		return window->onClose();
	}
	return 1;
}

Ref<WindowInstance> Window::createWindowInstance(const WindowInstanceParam& param)
{
	JniLocal<jobject> jwindow = _Android_Window::createHandle(param);
	if (jwindow.isNotNull()) {
		return UIPlatform::createWindowInstance(jwindow);
	}
	return Ref<WindowInstance>::null();
}


Ref<WindowInstance> UIPlatform::createWindowInstance(jobject jwindow)
{
	Ref<WindowInstance> window = UIPlatform::_getWindowInstance((void*)jwindow);
	if (window.isNotNull()) {
		return window;
	}
	Ref<_Android_Window> ret = _Android_Window::create(jwindow);
	if (ret.isNotNull()) {
		UIPlatform::_registerWindowInstance((void*)(ret->m_window.get()), ret.get());
	}
	return ret;
}

Ref<WindowInstance> UIPlatform::getWindowInstance(jobject jwindow)
{
	return UIPlatform::_getWindowInstance((void*)jwindow);
}

void UIPlatform::removeWindowInstance(jobject jwindow)
{
	UIPlatform::_removeWindowInstance((void*)jwindow);
}

jobject UIPlatform::getWindowHandle(WindowInstance* instance)
{
	_Android_Window* window = (_Android_Window*)instance;
	if (window) {
		return window->m_window.get();
	} else {
		return 0;
	}
}

SLIB_UI_NAMESPACE_END

#endif
