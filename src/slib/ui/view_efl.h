/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_UI_VIEW_EFL
#define CHECKHEADER_SLIB_UI_VIEW_EFL

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_EFL)

#include "slib/ui/view.h"
#include "slib/ui/platform.h"

namespace slib
{

	class EFL_ViewInstance : public ViewInstance
	{
		SLIB_DECLARE_OBJECT

	public:
		EFL_ViewInstance();

		~EFL_ViewInstance();
		
	public:
		template <class T>
		static Ref<T> create(EFL_ViewType type, Evas_Object* handle, sl_bool flagFreeOnRelease)
		{
			if (handle) {
				Ref<T> ret = new T();
				if (ret.isNotNull()) {
					if (ret->initialize(type, handle)) {
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
		static Ref<T> create(View* view, ViewInstance* parent, EFL_ViewType type, Evas_Object* handle, sl_bool flagFreeOnRelease)
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
		sl_bool initialize(EFL_ViewType type, Evas_Object* handle);

		sl_bool applyProperties(View* view, ViewInstance* parent);

		static void freeHandle(Evas_Object* handle);

		EFL_ViewType getType();

		Evas_Object* getHandle();

		void installTouchEvents();

	public:
		sl_bool isValid() override;

		void setFocus() override;

		void invalidate() override;

		void invalidate(const UIRect& rect) override;

		UIRect getFrame() override;

		void setFrame(const UIRect& frame) override;

		void setTransform(const Matrix3& transform) override;

		void setVisible(sl_bool flag) override;

		void setEnabled(sl_bool flag) override;

		void setOpaque(sl_bool flag) override;

		void setAlpha(sl_real alpha) override;

		void setClipping(sl_bool flag) override;

		void setDrawing(sl_bool flag) override;
		
		UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen) override;

		UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView) override;

		void addChildInstance(const Ref<ViewInstance>& instance) override;

		void removeChildInstance(const Ref<ViewInstance>& instance) override;

		void bringToFront() override;

	protected:
		void onTouch(const void* eventType, void *event_info);

	private:
		void _release();

		static void _onTouch(void *data, Evas *evas, Evas_Object *obj, void *event_info);

	protected:
		EFL_ViewType m_type;
		Evas_Object* m_handle;
		sl_bool m_flagFreeOnRelease;
		
#define SLIB_EFL_MULTI_TOUCH_COUNT_MAX 10

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
			TouchPoint m_points[SLIB_EFL_MULTI_TOUCH_COUNT_MAX];

		};

		Ref<TouchEventInfo> m_touchEventInfo;

	};

}

#endif

#endif
