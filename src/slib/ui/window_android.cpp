/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/ui/window.h"
#include "slib/ui/view.h"
#include "slib/ui/platform.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidPoint, "android/graphics/Point")
		SLIB_JNI_INT_FIELD(x);
		SLIB_JNI_INT_FIELD(y);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(_JAndroidRect, "android/graphics/Rect")
		SLIB_JNI_INT_FIELD(left);
		SLIB_JNI_INT_FIELD(top);
		SLIB_JNI_INT_FIELD(right);
		SLIB_JNI_INT_FIELD(bottom);
	SLIB_JNI_END_CLASS

	void JNICALL _AndroidWindow_nativeOnResize(JNIEnv* env, jobject _this, jlong instance, int w, int h);
	jboolean JNICALL _AndroidWindow_nativeOnClose(JNIEnv* env, jobject _this, jlong instance);

	SLIB_JNI_BEGIN_CLASS(_JAndroidWindow, "slib/platform/android/ui/window/UiWindow")

		SLIB_JNI_STATIC_METHOD(create, "create", "(Landroid/app/Activity;ZZIIII)Lslib/platform/android/ui/window/UiWindow;");

		SLIB_JNI_LONG_FIELD(instance);

		SLIB_JNI_METHOD(getContentView, "getContentView", "()Landroid/view/View;");
		SLIB_JNI_METHOD(close, "close", "()V");
		SLIB_JNI_METHOD(getBackgroundColor, "getWindowBackgroundColor", "()I");
		SLIB_JNI_METHOD(setBackgroundColor, "setWindowBackgroundColor", "(I)V");
		SLIB_JNI_METHOD(getFrame, "getFrame", "()Landroid/graphics/Rect;");
		SLIB_JNI_METHOD(setFrame, "setFrame", "(IIII)V");
		SLIB_JNI_METHOD(getSize, "getSize", "()Landroid/graphics/Point;");
		SLIB_JNI_METHOD(setSize, "setSize", "(II)V");
		SLIB_JNI_METHOD(isVisible, "isVisible", "()Z");
		SLIB_JNI_METHOD(setVisible, "setVisible", "(Z)V");
		SLIB_JNI_METHOD(isAlwaysOnTop, "isAlwaysOnTop", "()Z");
		SLIB_JNI_METHOD(setAlwaysOnTop, "setAlwaysOnTop", "(Z)V");
		SLIB_JNI_METHOD(getAlpha, "getWindowAlpha", "()F");
		SLIB_JNI_METHOD(setAlpha, "setWindowAlpha", "(F)V");
		SLIB_JNI_METHOD(focus, "focus", "()V");
		SLIB_JNI_METHOD(convertCoordinateFromScreenToWindow, "convertCoordinateFromScreenToWindow", "(II)Landroid/graphics/Point;");
		SLIB_JNI_METHOD(convertCoordinateFromWindowToScreen, "convertCoordinateFromWindowToScreen", "(II)Landroid/graphics/Point;");

		SLIB_JNI_NATIVE(onResize, "nativeOnResize", "(JII)V", _AndroidWindow_nativeOnResize);
		SLIB_JNI_NATIVE(onClose, "nativeOnClose", "(J)Z", _AndroidWindow_nativeOnClose);

	SLIB_JNI_END_CLASS
		
	class _Android_Window : public WindowInstance
	{
	public:
		AtomicJniGlobal<jobject> m_window;
		AtomicRef<ViewInstance> m_viewContent;

	public:
		_Android_Window()
		{
		}

		~_Android_Window()
		{
			close();
		}

	public:
		static Ref<_Android_Window> create(jobject jwindow)
		{
			if (!jwindow) {
				return sl_null;
			}
			JniLocal<jobject> jcontent = _JAndroidWindow::getContentView.callObject(jwindow);
			Ref<_Android_Window> ret = new _Android_Window();
			if (ret.isNotNull()) {
				JniGlobal<jobject> window = jwindow;
				Ref<ViewInstance> content = UIPlatform::createViewInstance(jcontent);
				if (window.isNotNull() && content.isNotNull()) {
					content->setWindowContent(sl_true);
					ret->m_window = window;
					ret->m_viewContent = content;
					jlong instance = (jlong)(window.get());
					_JAndroidWindow::instance.set(jwindow, instance);
					return ret;
				}
			}
			return sl_null;
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
				, (int)(param.location.x), (int)(param.location.y), (int)(param.size.x), (int)(param.size.y));
			return jwindow;
		}

		Ref<ViewInstance> getContentView()
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
			return sl_null;
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

		UIRect getFrame()
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				JniLocal<jobject> rect = _JAndroidWindow::getFrame.callObject(jwindow);
				if (rect.isNotNull()) {
					UIRect ret;
					ret.left = (sl_ui_pos)(_JAndroidRect::left.get(rect));
					ret.top = (sl_ui_pos)(_JAndroidRect::top.get(rect));
					ret.right = (sl_ui_pos)(_JAndroidRect::right.get(rect));
					ret.bottom = (sl_ui_pos)(_JAndroidRect::bottom.get(rect));
					ret.fixSizeError();
					return ret;
				}
			}
			return UIRect::zero();
		}

		sl_bool setFrame(const UIRect& frame)
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				_JAndroidWindow::setFrame.call(jwindow, (int)(frame.left), (int)(frame.top), (int)(frame.right), (int)(frame.bottom));
				return sl_true;
			}
			return sl_false;
		}

		UIRect getClientFrame()
		{
			return getFrame();
		}

		UISize getClientSize()
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				JniLocal<jobject> size = _JAndroidWindow::getSize.callObject(jwindow);
				if (size.isNotNull()) {
					UISize ret;
					ret.x = (sl_ui_pos)(_JAndroidPoint::x.get(size));
					ret.y = (sl_ui_pos)(_JAndroidPoint::y.get(size));
					return ret;
				}
			}
			return UISize::zero();
		}

		sl_bool setClientSize(const UISize& size)
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				_JAndroidWindow::setSize.call(jwindow, (int)(size.x), (int)(size.y));
				return sl_true;
			}
			return sl_false;
		}

		String getTitle()
		{
			return sl_null;
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

		UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen)
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				JniLocal<jobject> jpt = _JAndroidWindow::convertCoordinateFromScreenToWindow.callObject(jwindow, 0, 0);
				if (jpt.isNotNull()) {
					UIPointf ret;
					ret.x = ptScreen.x + (sl_ui_posf)(_JAndroidPoint::x.get(jpt));
					ret.y = ptScreen.y + (sl_ui_posf)(_JAndroidPoint::y.get(jpt));
					return ret;
				}
			}
			return ptScreen;
		}

		UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow)
		{
			JniGlobal<jobject> _jwindow(m_window);
			jobject jwindow = _jwindow;
			if (jwindow) {
				JniLocal<jobject> jpt = _JAndroidWindow::convertCoordinateFromWindowToScreen.callObject(jwindow, 0, 0);
				if (jpt.isNotNull()) {
					UIPointf ret;
					ret.x = ptWindow.x + (sl_ui_posf)(_JAndroidPoint::x.get(jpt));
					ret.y = ptWindow.y + (sl_ui_posf)(_JAndroidPoint::y.get(jpt));
					return ret;
				}
			}
			return ptWindow;
		}

		UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen)
		{
			return convertCoordinateFromScreenToWindow(ptScreen);
		}

		UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient)
		{
			return convertCoordinateFromWindowToScreen(ptClient);
		}

		UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow)
		{
			return ptWindow;
		}

		UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient)
		{
			return ptClient;
		}

		UISize getWindowSizeFromClientSize(const UISize& size)
		{
			return size;
		}

		UISize getClientSizeFromWindowSize(const UISize& size)
		{
			return size;
		}

	};

	Ref<_Android_Window> _AndroidUi_getWindow(jlong instance)
	{
		return Ref<_Android_Window>::from(UIPlatform::getWindowInstance((jobject)instance));
	}

	void JNICALL _AndroidWindow_nativeOnResize(JNIEnv* env, jobject _this, jlong instance, int w, int h)
	{
		Ref<_Android_Window> window = _AndroidUi_getWindow(instance);
		if (window.isNotNull()) {
			window->onResize((sl_ui_pos)w, (sl_ui_pos)h);
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
		return sl_null;
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

}

#endif
