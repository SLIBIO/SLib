#ifndef CHECKHEADER_SLIB_UI_VIEW_ANDROID
#define CHECKHEADER_SLIB_UI_VIEW_ANDROID

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/platform.h"

namespace slib
{

	class Android_ViewInstance : public ViewInstance
	{
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
					if (ret->init(jhandle)) {
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

		sl_bool init(jobject jhandle);

		sl_bool applyProperties(View* view, ViewInstance* parent);

	public:
		static Ref<Android_ViewInstance> findInstance(jlong jinstance);

		static Ref<View> findView(jlong jinstance);

	public:
		jobject getHandle();

		jobject getContext();
		
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
		JniGlobal<jobject> m_handle;
		JniGlobal<jobject> m_context;
		
	};

}

#endif

#endif
