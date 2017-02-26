#ifndef CHECKHEADER_SLIB_UI_VIEW_ANDROID
#define CHECKHEADER_SLIB_UI_VIEW_ANDROID

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/platform.h"

namespace slib
{

	class Tizen_ViewInstance : public ViewInstance
	{
	public:
		Tizen_ViewInstance();

		~Tizen_ViewInstance();
		
	public:
		template <class T>
		static Ref<T> create(TizenViewType type, Evas_Object* handle, sl_bool flagFreeOnRelease)
		{
			if (handle) {
				Ref<T> ret = new T();
				if (ret.isNotNull()) {
					if (ret->init(type, handle)) {
						ret->m_flagFreeOnRelease = flagFreeOnRelease;
						return ret;
					}
				}
				if (flagFreeOnRelease) {
					::evas_object_del(handle);
				}
			}
			return sl_null;
		}

		template <class T>
		static Ref<T> create(View* view, ViewInstance* parent, TizenViewType type, Evas_Object* handle, sl_bool flagFreeOnRelease)
		{
			Ref<T> ret = create<T>(type, handle, flagFreeOnRelease);
			if (ret.isNotNull()) {
				if (ret->applyProperties(view, parent)) {
					return ret;
				}
			}
			return sl_null;
		}

	public:
		sl_bool init(TizenViewType type, Evas_Object* handle);

		sl_bool applyProperties(View* view, ViewInstance* parent);

		static void freeHandle(Evas_Object* handle);

		TizenViewType getType();

		Evas_Object* getHandle();

		void installTouchEvents();

	public:
		// override
		sl_bool isValid();

		// override
		void setFocus();

		// override
		void invalidate();

		// override
		void invalidate(const UIRect& rect);

		// override
		UIRect getFrame();

		// override
		void setFrame(const UIRect& frame);

		// override
		void setTransform(const Matrix3& transform);

		// override
		void setVisible(sl_bool flag);

		// override
		void setEnabled(sl_bool flag);

		// override
		void setOpaque(sl_bool flag);

		// override
		void setAlpha(sl_real alpha);

		// override
		void setClipping(sl_bool flag);

		// override
		void setDrawing(sl_bool flag);
		
		// override
		UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen);

		// override
		UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView);

		// override
		void addChildInstance(const Ref<ViewInstance>& instance);

		// override
		void removeChildInstance(const Ref<ViewInstance>& instance);

		// override
		void bringToFront();

	protected:
		void onTouch(const void* eventType, void *event_info);

	private:
		void _release();

		static void _onTouch(void *data, Evas *evas, Evas_Object *obj, void *event_info);

	protected:
		TizenViewType m_type;
		Evas_Object* m_handle;
		sl_bool m_flagFreeOnRelease;
		
#define SLIB_TIZEN_MULTI_TOUCH_COUNT_MAX 10

		class TouchEventInfo : public Referable
		{
		public:
			TouchEventInfo();

			~TouchEventInfo();

		public:
			void setPoint(sl_uint32 index, TouchPhase phase, Evas_Coord x, Evas_Coord y);

			void clear();

			sl_uint32 getCount();

			Array<TouchPoint> getPoints();

			void finishPhases();

		private:
			sl_uint32 m_count;
			TouchPoint m_points[SLIB_TIZEN_MULTI_TOUCH_COUNT_MAX];

		};

		Ref<TouchEventInfo> m_touchEventInfo;

	};

}

#endif

#endif
