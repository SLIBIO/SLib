#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "view_android.h"

SLIB_UI_NAMESPACE_BEGIN

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

SLIB_JNI_BEGIN_CLASS(_JAndroidTouchPoint, "slib/platform/android/ui/view/UiTouchPoint")
	SLIB_JNI_FLOAT_FIELD(x);
	SLIB_JNI_FLOAT_FIELD(y);
	SLIB_JNI_FLOAT_FIELD(pressure);
	SLIB_JNI_INT_FIELD(phase);
SLIB_JNI_END_CLASS

void JNICALL _AndroidView_nativeOnDraw(JNIEnv* env, jobject _this, jlong jinstance, jobject jcanvas)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, jcanvas);
		if (canvas.isNotNull()) {
			instance->onDraw(canvas.get());
		}
	}
}

jboolean JNICALL _AndroidView_nativeOnKeyEvent(JNIEnv* env, jobject _this, jlong jinstance, jboolean flagDown, int keycode
	, jboolean flagControl, jboolean flagShift, jboolean flagAlt, jboolean flagWin, jlong time)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		UIAction action = flagDown ? UIAction::KeyDown : UIAction::KeyUp;
		sl_uint32 vkey = keycode;
		Keycode key = UIEvent::getKeycodeFromSystemKeycode(keycode);
		Time t;
		t.setMillisecondsCount(time);
		Ref<UIEvent> ev = UIEvent::createKeyEvent(action, key, vkey, t);
		if (ev.isNotNull()) {
			if (flagControl) {
				ev->setControlKey();
			}
			if (flagShift) {
				ev->setShiftKey();
			}
			if (flagAlt) {
				ev->setAltKey();
			}
			if (flagWin) {
				ev->setWindowsKey();
			}
			instance->onKeyEvent(ev.get());
			if (ev->isStoppedPropagation()) {
				return 1;
			}
		}
	}
	return 0;
}

jboolean JNICALL _AndroidView_nativeOnTouchEvent(JNIEnv* env, jobject _this, jlong jinstance, int _action, jobjectArray jpoints, jlong time)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		UIAction action = (UIAction)_action;
		sl_uint32 nPts = Jni::getArrayLength(jpoints);
		if (nPts > 0) {
			Array<TouchPoint> points = Array<TouchPoint>::create(nPts);
			if (points.isNotNull()) {
				TouchPoint* pts = points.getData();
				for (sl_uint32 i = 0; i < nPts; i++) {
					JniLocal<jobject> jpt = Jni::getObjectArrayElement(jpoints, i);
					if (jpt.isNotNull()) {
						pts[i].point.x = (sl_ui_posf)(_JAndroidTouchPoint::x.get(jpt));
						pts[i].point.y = (sl_ui_posf)(_JAndroidTouchPoint::y.get(jpt));
						pts[i].pressure = _JAndroidTouchPoint::pressure.get(jpt);
						pts[i].phase = (TouchPhase)(_JAndroidTouchPoint::phase.get(jpt));
					}
				}
				Time t;
				t.setMillisecondsCount(time);
				Ref<UIEvent> ev = UIEvent::createTouchEvent(action, points, t);
				if (ev.isNotNull()) {
					instance->onTouchEvent(ev.get());
					if (ev->isStoppedPropagation()) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void JNICALL _AndroidView_nativeOnClick(JNIEnv* env, jobject _this, jlong jinstance)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		instance->onClick();
	}
}

jboolean JNICALL _AndroidView_nativeHitTestTouchEvent(JNIEnv* env, jobject _this, jlong jinstance, int x, int y)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (view.isNotNull()) {
			if (!(view->isEnabled())) {
				return 1;
			}
			if (view->isCapturingChildInstanceEvents()) {
				if (view->hitTestForCapturingChildInstanceEvents(UIPoint((sl_ui_pos)x, (sl_ui_pos)y))) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void JNICALL _AndroidView_nativeOnSwipe(JNIEnv* env, jobject _this, jlong jinstance, int type)
{
	Ref<Android_ViewInstance> instance = Android_ViewInstance::getAndroidInstance(jinstance);
	if (instance.isNotNull()) {
		instance->onSwipe((GestureType)type);
	}
}

SLIB_JNI_BEGIN_CLASS(_JAndroidView, "slib/platform/android/ui/view/UiView")

	SLIB_JNI_STATIC_METHOD(getContext, "getContext", "(Landroid/view/View;)Landroid/content/Context;");
	SLIB_JNI_STATIC_METHOD(setInstance, "setInstance", "(Landroid/view/View;J)V");
	SLIB_JNI_STATIC_METHOD(freeView, "freeView", "(Landroid/view/View;)V");

	SLIB_JNI_STATIC_METHOD(createGeneric, "createGeneric", "(Landroid/content/Context;)Landroid/view/View;");
	SLIB_JNI_STATIC_METHOD(createGroup, "createGroup", "(Landroid/content/Context;)Landroid/view/View;");

	SLIB_JNI_STATIC_METHOD(setFocus, "setFocus", "(Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(invalidate, "invalidate", "(Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(invalidateRect, "invalidateRect", "(Landroid/view/View;IIII)V");
	SLIB_JNI_STATIC_METHOD(getFrame, "getFrame", "(Landroid/view/View;)Landroid/graphics/Rect;");
	SLIB_JNI_STATIC_METHOD(setFrame, "setFrame", "(Landroid/view/View;IIII)Z");
	SLIB_JNI_STATIC_METHOD(setTransform, "setTransform", "(Landroid/view/View;FFFFFFF)V");
	SLIB_JNI_STATIC_METHOD(isVisible, "isVisible", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setVisible, "setVisible", "(Landroid/view/View;Z)V");
	SLIB_JNI_STATIC_METHOD(isEnabled, "isEnabled", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setEnabled, "setEnabled", "(Landroid/view/View;Z)V");
	SLIB_JNI_STATIC_METHOD(setAlpha, "setAlpha", "(Landroid/view/View;F)V");
	SLIB_JNI_STATIC_METHOD(setLayered, "setLayered", "(Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(convertCoordinateFromScreenToView, "convertCoordinateFromScreenToView", "(Landroid/view/View;II)Landroid/graphics/Point;");
	SLIB_JNI_STATIC_METHOD(convertCoordinateFromViewToScreen, "convertCoordinateFromViewToScreen", "(Landroid/view/View;II)Landroid/graphics/Point;");

	SLIB_JNI_STATIC_METHOD(addChild, "addChild", "(Landroid/view/View;Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(removeChild, "removeChild", "(Landroid/view/View;Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(bringToFront, "bringToFront", "(Landroid/view/View;)V");
	SLIB_JNI_STATIC_METHOD(enableGesture, "enableGesture", "(Landroid/view/View;)V");

	SLIB_JNI_NATIVE(nativeOnDraw, "nativeOnDraw", "(JLslib/platform/android/ui/Graphics;)V", _AndroidView_nativeOnDraw);
	SLIB_JNI_NATIVE(nativeOnKeyEvent, "nativeOnKeyEvent", "(JZIZZZZJ)Z", _AndroidView_nativeOnKeyEvent);
	SLIB_JNI_NATIVE(nativeOnTouchEvent, "nativeOnTouchEvent", "(JI[Lslib/platform/android/ui/view/UiTouchPoint;J)Z", _AndroidView_nativeOnTouchEvent);
	SLIB_JNI_NATIVE(nativeOnClick, "nativeOnClick", "(J)V", _AndroidView_nativeOnClick);
	SLIB_JNI_NATIVE(nativeHitTestTouchEvent, "nativeHitTestTouchEvent", "(JII)Z", _AndroidView_nativeHitTestTouchEvent);
	SLIB_JNI_NATIVE(nativeOnSwipe, "nativeOnSwipe", "(JI)V", _AndroidView_nativeOnSwipe);

SLIB_JNI_END_CLASS

class _View : public View
{
	friend class Android_ViewInstance;
};

/******************************************
			Android_ViewInstance
******************************************/
Android_ViewInstance::Android_ViewInstance()
{
}

Android_ViewInstance::~Android_ViewInstance()
{
	jobject jhandle = m_handle.get();
	if (jhandle) {
		UIPlatform::removeViewInstance(jhandle);
		_JAndroidView::freeView.call(sl_null, jhandle);
	}
}

sl_bool Android_ViewInstance::init(jobject jhandle)
{
	if (jhandle) {
		JniLocal<jobject> jcontext = _JAndroidView::getContext.callObject(sl_null, jhandle);
		JniGlobal<jobject> context = jcontext;
		JniGlobal<jobject> handle = jhandle;
		if (context.isNotNull() && handle.isNotNull()) {
			m_context = context;
			m_handle = handle;
			jhandle = handle.get();
			jlong instance = (jlong)(jhandle);
			_JAndroidView::setInstance.call(sl_null, jhandle, instance);
			UIPlatform::registerViewInstance(jhandle, this);
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool Android_ViewInstance::applyProperties(View* _view, ViewInstance* parent)
{
	_View* view = (_View*)_view;
	jobject jhandle = m_handle.get();
	if (jhandle) {
		UIRect frame = view->getFrame();
		_JAndroidView::setFrame.callBoolean(sl_null, jhandle, (int)(frame.left), (int)(frame.top), (int)(frame.right), (int)(frame.bottom));
		_JAndroidView::setVisible.call(sl_null, jhandle, view->isVisible());
		_JAndroidView::setEnabled.call(sl_null, jhandle, view->isEnabled());
		sl_real alpha = view->getAlpha();
		if (alpha < 0.995f) {
			_JAndroidView::setAlpha.call(sl_null, jhandle, alpha);
		}
		if (view->isInstanceLayer()) {
			_JAndroidView::setLayered.call(sl_null, jhandle);
		}
		Vector2 t;
		sl_real r;
		Vector2 s;
		Vector2 anchor;
		if (view->getFinalTranslationRotationScale(&t, &r, &s, &anchor)) {
			_JAndroidView::setTransform.call(sl_null, jhandle, t.x, t.y, r, s.x, s.y, anchor.x, anchor.y);
		}

        if (parent) {
            jobject jparent = UIPlatform::getViewHandle(parent);
            if (jparent) {
                _JAndroidView::addChild.call(sl_null, jparent, jhandle);            
            }            
        }
		return sl_true;
	}
	return sl_false;
}

Ref<Android_ViewInstance> Android_ViewInstance::getAndroidInstance(jlong jinstance)
{
	return Ref<Android_ViewInstance>::from(UIPlatform::getViewInstance((jobject)jinstance));
}

Ref<View> Android_ViewInstance::getAndroidView(jlong jinstance)
{
	Ref<ViewInstance> instance = UIPlatform::getViewInstance((jobject)jinstance);
	if (instance.isNotNull()) {
		return instance->getView();
	}
	return sl_null;
}

jobject Android_ViewInstance::getHandle()
{
	return m_handle.get();
}

jobject Android_ViewInstance::getContext()
{
	return m_context.get();
}

sl_bool Android_ViewInstance::isValid()
{
	return sl_true;
}

void Android_ViewInstance::setFocus()
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::setFocus.call(sl_null, handle);
	}
}

void Android_ViewInstance::invalidate()
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::invalidate.call(sl_null, handle);
	}
}

void Android_ViewInstance::invalidate(const UIRect& rect)
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::invalidateRect.call(sl_null, handle, (int)(rect.left), (int)(rect.top), (int)(rect.right), (int)(rect.bottom));
	}
}

UIRect Android_ViewInstance::getFrame()
{
	jobject handle = m_handle.get();
	if (handle) {
		JniLocal<jobject> jrect = _JAndroidView::getFrame.callObject(sl_null, handle);
		if (jrect.isNotNull()) {
            UIRect ret;
			ret.left = (sl_ui_pos)(_JAndroidRect::left.get(jrect));
			ret.top = (sl_ui_pos)(_JAndroidRect::top.get(jrect));
			ret.right = (sl_ui_pos)(_JAndroidRect::right.get(jrect));
			ret.bottom = (sl_ui_pos)(_JAndroidRect::bottom.get(jrect));
			ret.fixSizeError();
            return ret;
		}
	}
	return UIRect::zero();
}

void Android_ViewInstance::setFrame(const UIRect& frame)
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::setFrame.callBoolean(sl_null, handle, (int)(frame.left), (int)(frame.top), (int)(frame.right), (int)(frame.bottom));
	}
}

void Android_ViewInstance::setTransform(const Matrix3& transform)
{
}

void Android_ViewInstance::setVisible(sl_bool flag)
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::setVisible.call(sl_null, handle, flag);
	}
}

void Android_ViewInstance::setEnabled(sl_bool flag)
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::setEnabled.call(sl_null, handle, flag);
	}
}

void Android_ViewInstance::setOpaque(sl_bool flag)
{
}

void Android_ViewInstance::setAlpha(sl_real alpha)
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::setAlpha.call(sl_null, handle, (float)alpha);
	}
}

UIPointf Android_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
{
	jobject handle = m_handle.get();
	if (handle) {
		JniLocal<jobject> jpt = _JAndroidView::convertCoordinateFromScreenToView.callObject(sl_null, handle, 0, 0);
		if (jpt.isNotNull()) {
			UIPointf ret;
			ret.x = ptScreen.x + (sl_ui_pos)(_JAndroidPoint::x.get(jpt));
			ret.y = ptScreen.y + (sl_ui_pos)(_JAndroidPoint::y.get(jpt));
			return ret;
		}
	}
	return ptScreen;
}

UIPointf Android_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
{
	jobject handle = m_handle.get();
	if (handle) {
		JniLocal<jobject> jpt = _JAndroidView::convertCoordinateFromViewToScreen.callObject(sl_null, handle, 0, 0);
		if (jpt.isNotNull()) {
			UIPointf ret;
			ret.x = ptView.x + (sl_ui_pos)(_JAndroidPoint::x.get(jpt));
			ret.y = ptView.y + (sl_ui_pos)(_JAndroidPoint::y.get(jpt));
			return ret;
		}
	}
	return ptView;
}

void Android_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
{
	jobject handle = m_handle.get();
	jobject child = UIPlatform::getViewHandle(_child.get());
	if (handle && child) {
		_JAndroidView::addChild.call(sl_null, handle, child);
	}
}

void Android_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
{
	jobject handle = m_handle.get();
	jobject child = UIPlatform::getViewHandle(_child.get());
	if (handle && child) {
		_JAndroidView::removeChild.call(sl_null, handle, child);
	}
}

void Android_ViewInstance::bringToFront()
{
	jobject handle = m_handle.get();
	if (handle) {
		_JAndroidView::bringToFront.call(sl_null, handle);
	}
}

/******************************************
				View
 ******************************************/
Ref<ViewInstance> View::createGenericInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle;
		if (m_flagCreatingChildInstances) {
			handle = _JAndroidView::createGroup.callObject(sl_null, parent->getContext());
		} else {
			handle = _JAndroidView::createGeneric.callObject(sl_null, parent->getContext());
		}
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
	}
	return ret;
}

void View::_setTransform_NI(const Matrix3& matrix)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Vector2 t;
		sl_real r;
		Vector2 s;
		Vector2 anchor;
		if (getFinalTranslationRotationScale(&t, &r, &s, &anchor)) {
			_JAndroidView::setTransform.call(sl_null, handle, t.x, t.y, r, s.x, s.y, anchor.x, anchor.y);
		}
	}
}

/******************************************
				UIPlatform
 ******************************************/
Ref<ViewInstance> UIPlatform::createViewInstance(jobject jhandle)
{
	Ref<ViewInstance> ret = UIPlatform::_getViewInstance((void*)jhandle);
	if (ret.isNotNull()) {
		return ret;
	}
	return Android_ViewInstance::create<Android_ViewInstance>(jhandle);
}

void UIPlatform::registerViewInstance(jobject jhandle, ViewInstance* instance)
{
	UIPlatform::_registerViewInstance((void*)(jhandle), instance);
}

Ref<ViewInstance> UIPlatform::getViewInstance(jobject jhandle)
{
	return UIPlatform::_getViewInstance((void*)jhandle);
}

void UIPlatform::removeViewInstance(jobject jhandle)
{
	UIPlatform::_removeViewInstance((void*)jhandle);
}

jobject UIPlatform::getViewHandle(ViewInstance* _instance)
{
	Android_ViewInstance* instance = (Android_ViewInstance*)_instance;
	if (instance) {
		return instance->getHandle();
	}
	return 0;
}

jobject UIPlatform::getViewHandle(View* view)
{
	if (view) {
		Ref<Android_ViewInstance> instance = Ref<Android_ViewInstance>::from(view->getViewInstance());
		if (instance.isNotNull()) {
			return instance->getHandle();
		}
	}
	return 0;
}

sl_bool GestureDetector::_enableNative(const Ref<View>& view, GestureType type)
{
	Ref<ViewInstance> _instance = view->getViewInstance();
	Android_ViewInstance* instance = static_cast<Android_ViewInstance*>(_instance.get());
	if (instance) {
		jobject handle = instance->getHandle();
		if (handle) {
			switch (type) {
				case GestureType::SwipeLeft:
				case GestureType::SwipeRight:
				case GestureType::SwipeUp:
				case GestureType::SwipeDown:
					_JAndroidView::enableGesture.call(sl_null, handle);
					return sl_true;
				default:
					break;
			}
		}
	}
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
