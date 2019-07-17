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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/edit_view.h"

#include "view_android.h"

namespace slib
{

	namespace priv
	{
		namespace edit_view
		{

			void JNICALL OnChange(JNIEnv* env, jobject _this, jlong instance);
			void JNICALL OnReturn(JNIEnv* env, jobject _this, jlong instance);

			SLIB_JNI_BEGIN_CLASS(JEditView, "slib/platform/android/ui/view/UiEditView")

				SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;I)Landroid/view/View;");

				SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
				SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setHintText, "_setHintText", "(Landroid/view/View;Ljava/lang/String;)Z");
				SLIB_JNI_STATIC_METHOD(setReadOnly, "_setReadOnly", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setMultiLine, "_setMultiLine", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setHintTextColor, "_setHintTextColor", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setReturnKeyType, "_setReturnKeyType", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setInputType, "_setInputType", "(Landroid/view/View;IIZ)Z");
				SLIB_JNI_STATIC_METHOD(measureHeight, "_measureHeight", "(Landroid/view/View;)I");
				SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");
				SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");

				SLIB_JNI_NATIVE(nativeOnChange, "nativeOnChange", "(J)V", OnChange);
				SLIB_JNI_NATIVE(nativeOnReturn, "nativeOnReturn", "(J)V", OnReturn);

			SLIB_JNI_END_CLASS

			class EditViewHelper : public EditView
			{
			public:
				void applyParameters(jobject handle)
				{
					JniLocal<jstring> jtext = Jni::getJniString(m_text);
					JEditView::setText.callBoolean(sl_null, handle, jtext.get());
					JEditView::setBorder.callBoolean(sl_null, handle, isBorder());
					JEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
					JniLocal<jstring> jhintText = Jni::getJniString(m_hintText);
					JEditView::setHintText.callBoolean(sl_null, handle, jhintText.get());
					JEditView::setReadOnly.callBoolean(sl_null, handle, m_flagReadOnly);
					JEditView::setMultiLine.callBoolean(sl_null, handle, m_multiLine != MultiLineMode::Single);
					JEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
					JEditView::setHintTextColor.callBoolean(sl_null, handle, m_hintTextColor.getARGB());
					JEditView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());
					Ref<Font> font = getFont();
					jobject jfont = GraphicsPlatform::getNativeFont(font.get());
					if (jfont) {
						JEditView::setFont.callBoolean(sl_null, handle, jfont);
					}
					JEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)m_returnKeyType);
					JEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)m_autoCapitalizationType, m_flagPassword ? 1 : 0);
				}
			};

			void JNICALL OnChange(JNIEnv* env, jobject _this, jlong _instance)
			{
				Ref<Android_ViewInstance> instance = Android_ViewInstance::findInstance(_instance);
				if (instance.isNull()) {
					return;
				}
				Ref<View> _view = instance->getView();
				if (EditView* view = CastInstance<EditView>(_view.get())) {
					String text = JEditView::getText.callString(sl_null, instance->getHandle());
					String textNew = text;
					view->dispatchChange(&textNew);
					if (text != textNew) {
						JniLocal<jstring> jstr = Jni::getJniString(textNew);
						JEditView::setText.callBoolean(sl_null, instance->getHandle(), jstr.get());
					}
				}
			}

			void JNICALL OnReturn(JNIEnv* env, jobject _this, jlong instance)
			{
				Ref<View> _view = Android_ViewInstance::findView(instance);
				if (EditView* view = CastInstance<EditView>(_view.get())) {
					view->dispatchReturnKey();
				}
			}

		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JEditView::create.callObject(sl_null, parent->getContext(), 0);
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((EditViewHelper*)this)->applyParameters(handle);
			}
		}
		return ret;
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JEditView::create.callObject(sl_null, parent->getContext(), 2);
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((EditViewHelper*)this)->applyParameters(handle);
			}
		}
		return ret;
	}

	void EditView::_getText_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_text = JEditView::getText.callString(sl_null, handle);
		}
	}

	void EditView::_setText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JEditView::setText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void EditView::_setTextAlignment_NW(Alignment align)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setAlignment.callBoolean(sl_null, handle, align.value);
		}
	}

	void EditView::_setHintText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JEditView::setHintText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setReadOnly.callBoolean(sl_null, handle, flag);
		}
	}

	void EditView::_setPassword_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)m_autoCapitalizationType, flag ? 1 : 0);
		}
	}

	void EditView::_setMultiLine_NW(MultiLineMode mode)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setMultiLine.callBoolean(sl_null, handle, mode != MultiLineMode::Single);
		}
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setHintTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setHintTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)type);
		}
	}

	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setInputType.callBoolean(sl_null, handle, (int)type, (int)m_autoCapitalizationType, m_flagPassword ? 1 : 0);
		}
	}

	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setInputType.callBoolean(sl_null, handle, (int) m_keyboardType, (int) type, m_flagPassword ? 1 : 0);
		}
	}

	sl_ui_len EditView::_measureHeight_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			return JEditView::measureHeight.callInt(sl_null, handle);
		}
		return 0;
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JEditView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setBorder.callBoolean(sl_null, handle, flag);
		}
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
	}

}

#endif
