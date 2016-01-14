#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/drop_down_list.h"

#include "view_android.h"

SLIB_UI_NAMESPACE_BEGIN

void JNICALL _JAndroidDropDownList_nativeOnSelect(JNIEnv* env, jobject _this, jlong instance, jint n)
{
	Ref<View> _view = Android_ViewInstance::getAndroidView(instance);
	if (DropDownList::checkInstance(_view)) {
		DropDownList* view = (DropDownList*)(_view.get());
		view->onSelectItem(n);
	}
}

SLIB_JNI_BEGIN_CLASS(_JAndroidDropDownList, "slib/platform/android/ui/view/UiDropDownList")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiDropDownList;");

	SLIB_JNI_STATIC_METHOD(applyList, "_applyList", "(Landroid/view/View;[Ljava/lang/String;)V");
	SLIB_JNI_STATIC_METHOD(getSelectedIndex, "_getSelectedIndex", "(Landroid/view/View;)I");
	SLIB_JNI_STATIC_METHOD(select, "_select", "(Landroid/view/View;I)V");
	SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

	SLIB_JNI_NATIVE(nativeOnSelect, "nativeOnSelect", "(JI)V", _JAndroidDropDownList_nativeOnSelect);

SLIB_JNI_END_CLASS

class _DropDownList : public DropDownList
{
public:
	void __copyItems(jobject jview)
	{
		ListLocker<String> titles(m_titles);
		sl_uint32 n = (sl_uint32)(titles.count());
		JniLocal<jobjectArray> arr = Jni::newStringArray(n);
		if (arr.isNotNull()) {
			for (sl_uint32 i = 0; i < titles.count(); i++) {
				Jni::setStringArrayElement(arr, i, titles[i]);
			}
			titles.unlock();
			_JAndroidDropDownList::applyList.call(sl_null, jview, arr.get());
			__select(jview, m_indexSelected);
		}
	}

	sl_uint32 __getSelectedIndex(jobject jview)
	{
		jint n = _JAndroidDropDownList::getSelectedIndex.callInt(sl_null, jview);
		if (n < 0) {
			n = 0;
		}
		return n;
	}
	
	void __select(jobject jview, sl_uint32 n)
	{
		_JAndroidDropDownList::select.call(sl_null, jview, n);
	}

};

Ref<ViewInstance> DropDownList::createInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidDropDownList::create.callObject(sl_null, parent->getContext());
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();

			((_DropDownList*)this)->__copyItems(handle);

			Ref<Font> font = m_font;
			Ref<FontInstance> fontInstance;
			jobject jfont = UIPlatform::getNativeFont(font.get(), fontInstance);
			if (jfont) {
				_JAndroidDropDownList::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}
	return ret;
}

sl_uint32 DropDownList::getSelectedIndex()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_indexSelected = ((_DropDownList*)this)->__getSelectedIndex(handle);
	}
	return m_indexSelected;
}

void DropDownList::_select(sl_uint32 index)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__select(handle, index);
	}
	m_indexSelected = index;
}

void DropDownList::_refreshItemsCount()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__copyItems(handle);
	}
}

void DropDownList::_refreshItemsContent()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__copyItems(handle);
	}
}

void DropDownList::_setItemTitle(sl_uint32 index, const String& title)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__copyItems(handle);
	}
}

void DropDownList::setFont(const Ref<Font>& font)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Ref<FontInstance> fontInstance;
		jobject jfont = UIPlatform::getNativeFont(font.get(), fontInstance);
		if (jfont) {
			_JAndroidDropDownList::setFont.callBoolean(sl_null, handle, jfont);
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

#endif
