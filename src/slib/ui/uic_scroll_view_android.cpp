#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/scroll_view.h"

#include "ui_view_android.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidScrollView, "slib/platform/android/ui/view/UiScrollView")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiScrollView;");

	SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)V");
	SLIB_JNI_STATIC_METHOD(scrollTo, "_scrollTo", "(Landroid/view/View;FF)V");
	SLIB_JNI_STATIC_METHOD(getScrollX, "_getScrollX", "(Landroid/view/View;)F");
	SLIB_JNI_STATIC_METHOD(getScrollY, "_getScrollY", "(Landroid/view/View;)F");

SLIB_JNI_END_CLASS

class _ScrollView : public ScrollView
{
public:
	void __applyContent(jobject sv, ViewInstance* scrollViewInstance)
	{
		Ref<View> viewContent = m_viewContent;
		if (viewContent.isNotNull()) {
			viewContent->attachToNewInstance(scrollViewInstance);
			viewContent->setParent(this);
		}
	}
	
	void __applyProperties(jobject handle, ViewInstance* scrollViewInstance)
	{
		_JAndroidScrollView::setBackgroundColor.call(sl_null, handle, m_backgroundColor.getARGB());
		__applyContent(handle, scrollViewInstance);
	}
};

Ref<ViewInstance> ScrollView::createInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidScrollView::create.callObject(sl_null, parent->getContext());
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			((_ScrollView*)this)->__applyProperties(handle, ret.get());
		}
	}
	return Ref<ViewInstance>::from(ret);
}

void ScrollView::onResize()
{
	View::onResize();
}

void ScrollView::onResizeChild(View* child)
{
}

void ScrollView::_setBorder(sl_bool flag)
{
}

void ScrollView::_setBackgroundColor(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidScrollView::setBackgroundColor.call(sl_null, handle, color.getARGB());
	}
}

void ScrollView::_setContentView(const Ref<View>& view)
{
	Ref<ViewInstance> instance = getViewInstance();
	if (instance.isNotNull()) {
		jobject handle = UIPlatform::getViewHandle(instance.get());
		if (handle) {
			((_ScrollView*)this)->__applyContent(handle, instance.get());
		}
	}
}

void ScrollView::scrollTo(sl_real x, sl_real y)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidScrollView::scrollTo.call(sl_null, handle, x, y);
	}
}

Point ScrollView::getScrollPosition()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Point ret;
		ret.x = _JAndroidScrollView::getScrollX.callFloat(sl_null, handle);
		ret.y = _JAndroidScrollView::getScrollY.callFloat(sl_null, handle);
		return ret;
	}
	return Point::zero();
}

Size ScrollView::getScrollRange()
{
	Ref<View> content = m_viewContent;
	if (content.isNotNull()) {
		Size ret;
		ret.width = content->getWidth() - getWidth();
		if (ret.width < 0) {
			ret.width = 0;
		}
		ret.height = content->getHeight() - getHeight();
		if (ret.height < 0) {
			ret.height = 0;
		}
		return ret;
	}
	return Size::zero();
}
SLIB_UI_NAMESPACE_END

#endif
