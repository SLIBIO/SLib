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
				SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setHintText, "_setHintText", "(Landroid/view/View;Ljava/lang/String;)Z");
				SLIB_JNI_STATIC_METHOD(setHintAlignment, "_setHintAlignment", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setHintTextColor, "_setHintTextColor", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setHintFont, "_setHintFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");
				SLIB_JNI_STATIC_METHOD(setReadOnly, "_setReadOnly", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setMultiLine, "_setMultiLine", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setReturnKeyType, "_setReturnKeyType", "(Landroid/view/View;I)Z");
				SLIB_JNI_STATIC_METHOD(setInputType, "_setInputType", "(Landroid/view/View;IIZ)Z");
				SLIB_JNI_STATIC_METHOD(measureHeight, "_measureHeight", "(Landroid/view/View;)I");
				SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");
				SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
				SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");

				SLIB_JNI_NATIVE(nativeOnChange, "nativeOnChange", "(J)V", OnChange);
				SLIB_JNI_NATIVE(nativeOnReturn, "nativeOnReturn", "(J)V", OnReturn);

			SLIB_JNI_END_CLASS

			class EditViewInstance;

			class EditViewHelper : public TextArea
			{
			public:
				void apply(jobject handle, EditViewInstance* instance);

			};

			class EditViewInstance : public Android_ViewInstance, public IEditViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				Ref<EditViewHelper> getHelper()
				{
					return CastRef<EditViewHelper>(getView());
				}

				sl_bool getText(EditView* view, String& _out) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						_out = JEditView::getText.callString(sl_null, handle);
						return sl_true;
					}
					return sl_false;
				}

				void setText(EditView* view, const String& text) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JniLocal<jstring> jstr = Jni::getJniString(text);
						JEditView::setText.callBoolean(sl_null, handle, jstr.get());
					}
				}

				void setGravity(EditView* view, const Alignment& align) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setAlignment.callBoolean(sl_null, handle, align.value);
					}
				}

				void setTextColor(EditView* view, const Color& color) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
					}
				}

				void setHintText(EditView* view, const String& text) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JniLocal<jstring> jstr = Jni::getJniString(text);
						JEditView::setHintText.callBoolean(sl_null, handle, jstr.get());
					}
				}

				void setHintGravity(EditView* view, const Alignment& align) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setHintAlignment.callBoolean(sl_null, handle, align.value);
					}
				}

				void setHintTextColor(EditView* view, const Color& color) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setHintTextColor.callBoolean(sl_null, handle, color.getARGB());
					}
				}

				void setHintFont(EditView* view, const Ref<Font>& font) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						jobject jfont = GraphicsPlatform::getNativeFont(font.get());
						if (jfont) {
							JEditView::setHintFont.callBoolean(sl_null, handle, jfont);
						}
					}
				}

				void setReadOnly(EditView* view, sl_bool flag) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setReadOnly.callBoolean(sl_null, handle, flag);
					}
				}

				void setPassword(EditView* view, sl_bool flag) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setInputType.callBoolean(sl_null, handle, (int)(view->getKeyboardType()), (int)(view->getAutoCaptializationType()), flag ? 1 : 0);
					}
				}

				void setMultiLine(EditView* view, MultiLineMode mode) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setMultiLine.callBoolean(sl_null, handle, mode != MultiLineMode::Single);
					}
				}

				void setReturnKeyType(EditView* view, UIReturnKeyType type) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)type);
					}
				}

				void setKeyboardType(EditView* view, UIKeyboardType type) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setInputType.callBoolean(sl_null, handle, (int)type, (int)(view->getAutoCaptializationType()), view->isPassword() ? 1 : 0);
					}
				}

				void setAutoCapitalizationType(EditView* view, UIAutoCapitalizationType type) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setInputType.callBoolean(sl_null, handle, (int)(view->getKeyboardType()), (int) type, view->isPassword() ? 1 : 0);
					}
				}

				sl_ui_len measureHeight(EditView* view) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						return JEditView::measureHeight.callInt(sl_null, handle);
					}
					return 0;
				}

				void setFont(View* view, const Ref<Font>& font) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						jobject jfont = GraphicsPlatform::getNativeFont(font.get());
						if (jfont) {
							JEditView::setFont.callBoolean(sl_null, handle, jfont);
						}
					}
				}

				void setBorder(View* view, sl_bool flag) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setBorder.callBoolean(sl_null, handle, flag);
					}
				}

				void setBackgroundColor(View* view, const Color& color) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
					}
				}

				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
				}

				void onChange()
				{
					jobject handle = m_handle.get();
					if (handle) {
						Ref<EditViewHelper> helper = getHelper();
						if (helper.isNotNull()) {
							String text = JEditView::getText.callString(sl_null, handle);
							String textNew = text;
							helper->dispatchChange(&textNew);
							if (text != textNew) {
								JniLocal<jstring> jstr = Jni::getJniString(textNew);
								JEditView::setText.callBoolean(sl_null, handle, jstr.get());
							}
						}
					}
				}

				void onReturn()
				{
					Ref<EditViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						helper->dispatchReturnKey();
					}
				}

			};

			SLIB_DEFINE_OBJECT(EditViewInstance, Android_ViewInstance)

			void EditViewHelper::apply(jobject handle, EditViewInstance* instance)
			{
				JniLocal<jstring> jtext = Jni::getJniString(m_text);
				JEditView::setText.callBoolean(sl_null, handle, jtext.get());
				JEditView::setBorder.callBoolean(sl_null, handle, isBorder());
				JEditView::setAlignment.callBoolean(sl_null, handle, m_gravity.value);
				JEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
				JniLocal<jstring> jhintText = Jni::getJniString(m_hintText);
				JEditView::setHintText.callBoolean(sl_null, handle, jhintText.get());
				JEditView::setHintAlignment.callBoolean(sl_null, handle, m_hintGravity.value);
				JEditView::setHintTextColor.callBoolean(sl_null, handle, m_hintTextColor.getARGB());
				instance->setHintFont(this, getHintFont());
				JEditView::setReadOnly.callBoolean(sl_null, handle, m_flagReadOnly);
				JEditView::setMultiLine.callBoolean(sl_null, handle, m_multiLine != MultiLineMode::Single);
				JEditView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());
				instance->setFont(this, getFont());
				JEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)m_returnKeyType);
				JEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)m_autoCapitalizationType, m_flagPassword ? 1 : 0);
			}

			void JNICALL OnChange(JNIEnv* env, jobject _this, jlong _instance)
			{
				Ref<EditViewInstance> instance = CastRef<EditViewInstance>(Android_ViewInstance::findInstance(_instance));
				if (instance.isNotNull()) {
					instance->onChange();
				}
			}

			void JNICALL OnReturn(JNIEnv* env, jobject _this, jlong _instance)
			{
				Ref<EditViewInstance> instance = CastRef<EditViewInstance>(Android_ViewInstance::findInstance(_instance));
				if (instance.isNotNull()) {
					instance->onReturn();
				}
			}

		}
	}

	using namespace priv::edit_view;

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JEditView::create.callObject(sl_null, parent->getContext(), 0);
			Ref<EditViewInstance> ret = Android_ViewInstance::create<EditViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject jhandle = ret->getHandle();
				(static_cast<EditViewHelper*>(this))->apply(jhandle, ret.get());
				return ret;
			}
		}
		return sl_null;
	}

	Ptr<IEditViewInstance> EditView::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JEditView::create.callObject(sl_null, parent->getContext(), 2);
			Ref<EditViewInstance> ret = Android_ViewInstance::create<EditViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject jhandle = ret->getHandle();
				(static_cast<EditViewHelper*>(this))->apply(jhandle, ret.get());
				return ret;
			}
		}
		return sl_null;
	}

	Ptr<IEditViewInstance> TextArea::getEditViewInstance()
	{
		return CastRef<EditViewInstance>(getViewInstance());
	}

}

#endif
