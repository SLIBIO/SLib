#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "view_tizen.h"

#include "../../../inc/slib/ui/core.h"

#pragma GCC diagnostic ignored "-Wextern-c-compat"
#include <Elementary.h>

namespace slib
{

/******************************************
		Tizen_ViewInstance
******************************************/
	
	Tizen_ViewInstance::Tizen_ViewInstance()
	{
		m_type = TizenViewType::Generic;
		m_handle = sl_null;
		m_flagFreeOnRelease = sl_true;
	}
	
	Tizen_ViewInstance::~Tizen_ViewInstance()
	{
		_release();
	}
	
	sl_bool Tizen_ViewInstance::init(TizenViewType type, Evas_Object* handle)
	{
		if (handle) {
			m_type = type;
			m_handle = handle;
			UIPlatform::registerViewInstance(handle, this);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Tizen_ViewInstance::applyProperties(View* view, ViewInstance* _parent)
	{
		Tizen_ViewInstance* parent = static_cast<Tizen_ViewInstance*>(_parent);
		Evas_Object* handle = m_handle;
		if (handle) {
			UIRect frame = view->getFrame();
			if (parent && parent->m_type == TizenViewType::Grid) {
				Evas_Object* handleParent = parent->m_handle;
				if (handleParent) {
					::elm_grid_pack(handleParent, handle, frame.left, frame.top, frame.getWidth(), frame.getHeight());
				}
			} else {
				::evas_object_move(handle, frame.left, frame.top);
				::evas_object_resize(handle, frame.getWidth(), frame.getHeight());
			}
			if (m_type == TizenViewType::Grid) {
				::elm_grid_size_set(handle, frame.getWidth(), frame.getHeight());
			}
			if (view->isVisible()) {
				::evas_object_show(handle);
			}
			sl_real alpha = view->getAlpha();
			if (alpha < 0.005f) {
				::evas_object_color_set(handle, 255, 255, 255, 0);
			} else if (alpha < 0.995f) {
				::evas_object_color_set(handle, 255, 255, 255, (int)(255 * alpha));
			}
			return sl_true;
		}
		return sl_false;
	}

	void Tizen_ViewInstance::_release()
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			UIPlatform::removeViewInstance(handle);
			if (m_flagFreeOnRelease) {
				freeHandle(handle);
			}
			m_handle = sl_null;
		}
	}

	void Tizen_ViewInstance::freeHandle(Evas_Object* handle)
	{
		if (handle) {
			::evas_object_del(handle);
		}
	}

	TizenViewType Tizen_ViewInstance::getType()
	{
		return m_type;
	}

	Evas_Object* Tizen_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool Tizen_ViewInstance::isValid()
	{
		return sl_true;
	}
	
	void Tizen_ViewInstance::setFocus()
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_FUNCTION_WEAKREF(Tizen_ViewInstance, setFocus, this));
				return;
			}
			::elm_object_focus_set(handle, EINA_TRUE);
		}
	}
	
	void Tizen_ViewInstance::invalidate()
	{
	}
	
	void Tizen_ViewInstance::invalidate(const UIRect& _rect)
	{
	}
	
	UIRect Tizen_ViewInstance::getFrame()
	{
		return UIRect::zero();
	}
	
	void Tizen_ViewInstance::setFrame(const UIRect& frame)
	{
		if (m_type != TizenViewType::Window) {
			Evas_Object* handle = m_handle;
			if (handle) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), Tizen_ViewInstance, setFrame, this, frame));
					return;
				}
				::elm_grid_pack_set(handle, frame.left, frame.top, frame.getWidth(), frame.getHeight());
				if (m_type == TizenViewType::Grid) {
					::elm_grid_size_set(handle, frame.getWidth(), frame.getHeight());
				} else {
					::evas_object_move(handle, frame.left, frame.top);
					::evas_object_resize(handle, frame.getWidth(), frame.getHeight());
				}
			}
		}
	}
	
	void Tizen_ViewInstance::setTransform(const Matrix3& m)
	{
	}
	
	void Tizen_ViewInstance::setVisible(sl_bool flag)
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), Tizen_ViewInstance, setVisible, this, flag));
				return;
			}
			if (flag) {
				::evas_object_show(handle);
			} else {
				::evas_object_hide(handle);
			}
		}
	}
	
	void Tizen_ViewInstance::setEnabled(sl_bool flag)
	{
	}
	
	void Tizen_ViewInstance::setOpaque(sl_bool flag)
	{
	}
	
	void Tizen_ViewInstance::setAlpha(sl_real alpha)
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			if (!(UI::isUiThread())) {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), Tizen_ViewInstance, setAlpha, this, alpha));
				return;
			}
			if (alpha <= 0.005f) {
				::evas_object_color_set(handle, 255, 255, 255, 0);
			} else if (alpha <= 0.995) {
				::evas_object_color_set(handle, 255, 255, 255, (int)(alpha * 255));
			} else {
				::evas_object_color_set(handle, 255, 255, 255, 255);
			}
		}
	}

	void Tizen_ViewInstance::setClipping(sl_bool flag)
	{
	}

	void Tizen_ViewInstance::setDrawing(sl_bool flag)
	{
	}
	
	UIPointf Tizen_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			Evas_Coord x = 0, y = 0;
			::evas_object_geometry_get(handle, &x, &y, sl_null, sl_null);
			return UIPointf((sl_ui_posf)(ptScreen.x - x), (sl_ui_posf)(ptScreen.y - y));
		}
		return ptScreen;
	}
	
	UIPointf Tizen_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			Evas_Coord x = 0, y = 0;
			::evas_object_geometry_get(handle, &x, &y, sl_null, sl_null);
			return UIPointf((sl_ui_posf)(ptView.x + x), (sl_ui_posf)(ptView.y + y));
		}
		return ptView;
	}
	
	void Tizen_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
	{
		Tizen_ViewInstance* child = static_cast<Tizen_ViewInstance*>(_child.get());
		if (child) {
			Evas_Object* handle = m_handle;
			if (handle) {
				Evas_Object* handleChild = child->m_handle;
				if (handleChild) {
					if (m_type == TizenViewType::Grid) {
						UIRect frame;
						Ref<View> view = child->getView();
						if (view.isNotNull()) {
							frame = view->getFrame();
						} else {
							frame = UIRect::zero();
						}
						::elm_grid_pack(handle, handleChild, frame.left, frame.top, frame.getWidth(), frame.getHeight());
					}
				}
			}
		}
	}
	
	void Tizen_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
	{
		Tizen_ViewInstance* child = static_cast<Tizen_ViewInstance*>(_child.get());
		if (child) {
			Evas_Object* handle = m_handle;
			if (handle) {
				Evas_Object* handleChild = child->m_handle;
				if (handleChild) {
					if (m_type == TizenViewType::Grid) {
						::elm_grid_unpack(handle, handleChild);
					}
				}
			}
		}
	}
	
	void Tizen_ViewInstance::bringToFront()
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			::evas_object_raise(handle);
		}
	}

	Tizen_ViewInstance::TouchEventInfo::TouchEventInfo()
	{
		m_count = 0;
	}

	Tizen_ViewInstance::TouchEventInfo::~TouchEventInfo()
	{
	}

	void Tizen_ViewInstance::TouchEventInfo::setPoint(sl_uint32 index, TouchPhase phase, Evas_Coord x, Evas_Coord y)
	{
		if (index >= SLIB_TIZEN_MULTI_TOUCH_COUNT_MAX) {
			return;
		}
		if (index >= m_count) {
			for (sl_uint32 i = m_count; i < index; i++) {
				m_points[i].phase = TouchPhase::Cancel;
			}
			m_count = index + 1;
		}
		m_points[index].point.x = (sl_ui_posf)x;
		m_points[index].point.y = (sl_ui_posf)y;
		m_points[index].phase = phase;
	}

	void Tizen_ViewInstance::TouchEventInfo::clear()
	{
		m_count = 0;
	}

	sl_uint32 Tizen_ViewInstance::TouchEventInfo::getCount()
	{
		return m_count;
	}

	Array<TouchPoint> Tizen_ViewInstance::TouchEventInfo::getPoints()
	{
		sl_uint32 n = m_count;
		Array<TouchPoint> arr = Array<TouchPoint>::create(n);
		if (arr.isNotNull()) {
			TouchPoint* pts = arr.getData();
			sl_uint32 m = 0;
			for (sl_uint32 i = 0; i < n; i++) {
				if (m_points[i].phase != TouchPhase::Cancel) {
					pts[m] = m_points[i];
					m++;
				}
			}
			return arr.sub(0, m);
		}
		return sl_null;
	}

	void Tizen_ViewInstance::TouchEventInfo::finishPhases()
	{
		sl_uint32 n = m_count;
		sl_uint32 m = 0;
		for (sl_uint32 i = 0; i < n; i++) {
			TouchPoint& pt = m_points[i];
			if (pt.phase == TouchPhase::Begin) {
				pt.phase = TouchPhase::Move;
			} else if (pt.phase == TouchPhase::End) {
				pt.phase = TouchPhase::Cancel;
			}
			if (pt.phase != TouchPhase::Cancel) {
				m = i;
			}
		}
		m_count = m;
	}

	static const void* _Tizen_Event_TouchBegin = "tb";
	static const void* _Tizen_Event_TouchEnd = "te";
	static const void* _Tizen_Event_TouchMove = "tm";
	static const void* _Tizen_Event_MultiTouchBegin = "mtb";
	static const void* _Tizen_Event_MultiTouchEnd = "mte";
	static const void* _Tizen_Event_MultiTouchMove = "mtm";

	void Tizen_ViewInstance::onTouch(const void *eventType, void *event_info)
	{
		Evas_Object* handle = m_handle;
		if (!handle) {
			return;
		}

		Ref<View> view = getView();
		if (view.isNull()) {
			return;
		}

		if (m_touchEventInfo.isNull()) {
			m_touchEventInfo = new TouchEventInfo;
			if (m_touchEventInfo.isNull()) {
				return;
			}
		}

		Evas_Coord x, y;
		::evas_object_geometry_get(handle, &x, &y, sl_null, sl_null);

		UIAction action;
		unsigned int timestamp;

		if (eventType == _Tizen_Event_TouchBegin) {
			Evas_Event_Mouse_Down* ev = (Evas_Event_Mouse_Down*)event_info;
			m_touchEventInfo->setPoint(0, TouchPhase::Begin, ev->canvas.x - x, ev->canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchBegin;
		} else if (eventType == _Tizen_Event_MultiTouchBegin) {
			Evas_Event_Multi_Down* ev = (Evas_Event_Multi_Down*)event_info;
			m_touchEventInfo->setPoint(ev->device, TouchPhase::Begin, ev->canvas.x - x, ev->canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchMove;
		} else if (eventType == _Tizen_Event_TouchMove) {
			Evas_Event_Mouse_Move* ev = (Evas_Event_Mouse_Move*)event_info;
			m_touchEventInfo->setPoint(0, TouchPhase::Move, ev->cur.canvas.x - x, ev->cur.canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchMove;
		} else if (eventType == _Tizen_Event_MultiTouchMove) {
			Evas_Event_Multi_Move* ev = (Evas_Event_Multi_Move*)event_info;
			m_touchEventInfo->setPoint(ev->device, TouchPhase::Move, ev->cur.canvas.x - x, ev->cur.canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchMove;
		} else if (eventType == _Tizen_Event_TouchEnd) {
			Evas_Event_Mouse_Up* ev = (Evas_Event_Mouse_Up*)event_info;
			m_touchEventInfo->setPoint(0, TouchPhase::End, ev->canvas.x - x, ev->canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchEnd;
		} else if (eventType == _Tizen_Event_MultiTouchEnd) {
			Evas_Event_Multi_Up* ev = (Evas_Event_Multi_Up*)event_info;
			m_touchEventInfo->setPoint(ev->device, TouchPhase::End, ev->canvas.x - x, ev->canvas.y - y);
			timestamp = ev->timestamp;
			action = UIAction::TouchEnd;
		} else {
			return;
		}

		if (action == UIAction::TouchEnd && m_touchEventInfo->getCount() >= 2) {
			action = UIAction::TouchMove;
		}

		Time t;
		t.setMillisecondsCount(timestamp);
		Ref<UIEvent> ev = UIEvent::createTouchEvent(action, m_touchEventInfo->getPoints(), t);
		if (ev.isNotNull()) {
			onTouchEvent(ev.get());
		}

		m_touchEventInfo->finishPhases();
	}

	void Tizen_ViewInstance::_onTouch(void *data, Evas *evas, Evas_Object *obj, void *event_info)
	{
		Ref<Tizen_ViewInstance> instance = Ref<Tizen_ViewInstance>::from(UIPlatform::getViewInstance(obj));
		if (instance.isNotNull()) {
			instance->onTouch((const void*)data, event_info);
		}
	}
	
	void Tizen_ViewInstance::installTouchEvents()
	{
		Evas_Object* handle = m_handle;
		if (handle) {
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MOUSE_DOWN, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_TouchBegin);
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MOUSE_UP, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_TouchEnd);
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MOUSE_MOVE, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_TouchMove);
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MULTI_DOWN, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_MultiTouchBegin);
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MULTI_UP, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_MultiTouchEnd);
			::evas_object_event_callback_add(handle, EVAS_CALLBACK_MULTI_MOVE, &(Tizen_ViewInstance::_onTouch), _Tizen_Event_MultiTouchMove);
		}
	}

	
/******************************************
				View
******************************************/

	Ref<ViewInstance> View::createGenericInstance(ViewInstance* _parent)
	{
		Tizen_ViewInstance* parent = static_cast<Tizen_ViewInstance*>(_parent);
		if (parent) {
			Evas_Object* handleParent = parent->getHandle();
			if (handleParent) {
				if (m_flagCreatingChildInstances) {
					Evas_Object* handle = ::elm_grid_add(handleParent);
					if (handle) {
						Ref<Tizen_ViewInstance> ret = Tizen_ViewInstance::create<Tizen_ViewInstance>(this, parent, TizenViewType::Grid, handle, sl_true);
						if (ret.isNotNull()) {
							ret->installTouchEvents();
							return ret;
						}
					}
				}
			}
		}
		return sl_null;
	}
}

/******************************************
			UIPlatform
******************************************/

namespace slib
{

	Ref<ViewInstance> UIPlatform::createViewInstance(TizenViewType type, Evas_Object* handle, sl_bool flagFreeOnRelease)
	{
		Ref<ViewInstance> ret = UIPlatform::_getViewInstance(handle);
		if (ret.isNotNull()) {
			return ret;
		}
		return Tizen_ViewInstance::create<Tizen_ViewInstance>(type, handle, flagFreeOnRelease);
	}
	
	void UIPlatform::registerViewInstance(Evas_Object* handle, ViewInstance* instance)
	{
		UIPlatform::_registerViewInstance(handle, instance);
	}
	
	Ref<ViewInstance> UIPlatform::getViewInstance(Evas_Object* handle)
	{
		return UIPlatform::_getViewInstance(handle);
	}
	
	void UIPlatform::removeViewInstance(Evas_Object* handle)
	{
		UIPlatform::_removeViewInstance(handle);
	}
	
	Evas_Object* UIPlatform::getViewHandle(ViewInstance* _instance)
	{
		Tizen_ViewInstance* instance = static_cast<Tizen_ViewInstance*>(_instance);
		if (instance) {
			return instance->getHandle();
		}
		return sl_null;
	}
	
	Evas_Object* UIPlatform::getViewHandle(View* view)
	{
		if (view) {
			Ref<ViewInstance> _instance = view->getViewInstance();
			if (_instance.isNotNull()) {
				Tizen_ViewInstance* instance = static_cast<Tizen_ViewInstance*>(_instance.get());
				return instance->getHandle();
			}
		}
		return sl_null;
	}

}

#endif


