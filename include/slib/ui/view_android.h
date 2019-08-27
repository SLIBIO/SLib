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

#ifndef CHECKHEADER_SLIB_UI_VIEW_ANDROID
#define CHECKHEADER_SLIB_UI_VIEW_ANDROID

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/view.h"

#include "slib/ui/platform.h"

namespace slib
{

	class Android_ViewInstance : public ViewInstance
	{
		SLIB_DECLARE_OBJECT

	public:
		Android_ViewInstance();

		~Android_ViewInstance();
		
	public:
		template <class T>
		static Ref<T> create(jobject jhandle)
		{
			if (jhandle) {
				Ref<T> ret = new T();
				if (ret.isNotNull()) {
					if (ret->initialize(jhandle)) {
						return ret;
					}
				}
			}
			return sl_null;
		}

		template <class T>
		static Ref<T> create(View* view, ViewInstance* parent, jobject jhandle)
		{
			Ref<T> ret = create<T>(jhandle);
			if (ret.isNotNull()) {
				if (ret->applyProperties(view, parent)) {
					return ret;
				}
			}
			return sl_null;
		}

		sl_bool initialize(jobject jhandle);

		sl_bool applyProperties(View* view, ViewInstance* parent);

	public:
		static Ref<Android_ViewInstance> findInstance(jlong jinstance);

		static Ref<View> findView(jlong jinstance);

	public:
		jobject getHandle();

		jobject getContext();
		
	public:
		sl_bool isValid(View* view) override;

		void setFocus(View* view, sl_bool flag) override;

		void invalidate(View* view) override;

		void invalidate(View* view, const UIRect& rect) override;

		void setFrame(View* view, const UIRect& frame) override;

		void setTransform(View* view, const Matrix3& transform) override;

		void setVisible(View* view, sl_bool flag) override;

		void setEnabled(View* view, sl_bool flag) override;

		void setOpaque(View* view, sl_bool flag) override;

		void setAlpha(View* view, sl_real alpha) override;
		
		void setClipping(View* view, sl_bool flag) override;
		
		void setDrawing(View* view, sl_bool flag) override;
		
		UIPointf convertCoordinateFromScreenToView(View* view, const UIPointf& ptScreen) override;

		UIPointf convertCoordinateFromViewToScreen(View* view, const UIPointf& ptView) override;

		void addChildInstance(View* view, const Ref<ViewInstance>& instance) override;

		void removeChildInstance(View* view, const Ref<ViewInstance>& instance) override;

		void bringToFront(View* view) override;

	protected:
		JniGlobal<jobject> m_handle;
		JniGlobal<jobject> m_context;
		
	};

}

#endif

#endif
