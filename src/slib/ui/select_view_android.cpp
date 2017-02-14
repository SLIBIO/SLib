#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/select_view.h"

#include "view_android.h"

namespace slib
{

	void JNICALL _JAndroidSelectView_nativeOnSelect(JNIEnv* env, jobject _this, jlong instance, jint n)
	{
		Ref<View> _view = Android_ViewInstance::getAndroidView(instance);
		if (SelectView* view = CastInstance<SelectView>(_view.get())) {
			view->dispatchSelectItem(n);
		}
	}

	SLIB_JNI_BEGIN_CLASS(_JAndroidSelectView, "slib/platform/android/ui/view/UiSelectView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiSelectView;");

		SLIB_JNI_STATIC_METHOD(applyList, "_applyList", "(Landroid/view/View;[Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getSelectedIndex, "_getSelectedIndex", "(Landroid/view/View;)I");
		SLIB_JNI_STATIC_METHOD(select, "_select", "(Landroid/view/View;I)V");
		SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

		SLIB_JNI_NATIVE(nativeOnSelect, "nativeOnSelect", "(JI)V", _JAndroidSelectView_nativeOnSelect);

	SLIB_JNI_END_CLASS

	class _SelectView : public SelectView
	{
	public:
		void __copyItems(jobject jview)
		{
			ListLocker<String> titles(m_titles);
			sl_uint32 n = (sl_uint32)(titles.count);
			JniLocal<jobjectArray> arr = Jni::newStringArray(n);
			if (arr.isNotNull()) {
				for (sl_uint32 i = 0; i < titles.count; i++) {
					Jni::setStringArrayElement(arr, i, titles[i]);
				}
				titles.unlock();
				_JAndroidSelectView::applyList.call(sl_null, jview, arr.get());
				__select(jview, m_indexSelected);
			}
		}

		sl_uint32 __getSelectedIndex(jobject jview)
		{
			jint n = _JAndroidSelectView::getSelectedIndex.callInt(sl_null, jview);
			if (n < 0) {
				n = 0;
			}
			return n;
		}
		
		void __select(jobject jview, sl_uint32 n)
		{
			_JAndroidSelectView::select.call(sl_null, jview, n);
		}

	};

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = _JAndroidSelectView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();

				((_SelectView*)this)->__copyItems(handle);

				Ref<Font> font = getFont();
				jobject jfont = GraphicsPlatform::getNativeFont(font.get());
				if (jfont) {
					_JAndroidSelectView::setFont.callBoolean(sl_null, handle, jfont);
				}
			}
		}
		return ret;
	}

	void SelectView::_getSelectedIndex_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_indexSelected = ((_SelectView*)this)->__getSelectedIndex(handle);
		}
	}

	void SelectView::_select_NW(sl_uint32 index)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_SelectView*)this)->__select(handle, index);
		}
	}

	void SelectView::_refreshItemsCount_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_SelectView*)this)->__copyItems(handle);
		}
	}

	void SelectView::_refreshItemsContent_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_SelectView*)this)->__copyItems(handle);
		}
	}

	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_SelectView*)this)->__copyItems(handle);
		}
	}

	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				_JAndroidSelectView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

}

#endif
