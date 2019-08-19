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

#include "slib/ui/toast.h"

#include "slib/ui/core.h"
#include "slib/ui/mobile_app.h"
#include "slib/ui/label_view.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace toast
		{
			
			class ToastManager
			{
			public:
				Mutex lock;
				Ref<LabelView> currentToast;
				Ref<Animation> animation;
				
			public:
				void show(Toast* toast)
				{
					MutexLocker locker(&lock);
					if (currentToast.isNotNull()) {
						currentToast->removeFromParent();
						currentToast.setNull();
						animation.setNull();
					}
					Ref<Font> font = toast->font;
					if (font.isNull()) {
						return;
					}
					Ref<View> parent = toast->parent;
					if (parent.isNull()) {
						Ref<MobileApp> app = MobileApp::getApp();
						if (app.isNotNull()) {
							parent = app->getContentView();
						} else {
							Ref<UIApp> ui = UIApp::getApp();
							if (ui.isNotNull()) {
								Ref<Window> window = ui->getMainWindow();
								if (window.isNotNull()) {
									parent = window->getContentView();
								}
							}
						}
					}
					if (parent.isNull()) {
						return;
					}
					Ref<LabelView> refView = new LabelView;
					LabelView* view = refView.get();
					if (!view) {
						return;
					}
					view->setCreatingInstance(sl_true);
					view->setText(toast->text, UIUpdateMode::Init);
					view->setMultiLine(MultiLineMode::WordWrap, UIUpdateMode::Init);
					view->setWidthWrapping(UIUpdateMode::Init);
					view->setHeightWrapping(UIUpdateMode::Init);
					view->setMaximumWidth((sl_ui_len)(parent->getWidth() * 0.9f), UIUpdateMode::Init);
					view->setFont(font, UIUpdateMode::Init);
					view->setTextColor(Color::White, UIUpdateMode::Init);
					view->setBackgroundColor(Color(0, 0, 0, 160), UIUpdateMode::Init);
					view->setBoundRadius(font->getFontHeight() / 3, UIUpdateMode::Init);
					view->setPadding((sl_ui_pos)(font->getFontHeight() / 3), UIUpdateMode::Init);
					view->setCenterInParent(UIUpdateMode::Init);
					view->setClipping(sl_true, UIUpdateMode::Init);
					currentToast = view;
					parent->addChild(view);
					animation = view->startAlphaAnimation(0, 1, 0.3f, sl_null, AnimationCurve::Linear, AnimationFlags::NotSelfAlive);
					
					UI::dispatchToUiThread(Function<void()>::with(ToWeakRef(view), [this, view]() {
						MutexLocker locker(&lock);
						if (currentToast.isNotNull()) {
							animation = currentToast->startAlphaAnimation(1, 0, 0.3f, Function<void()>::with(ToWeakRef(view), [this, view]() {
								MutexLocker locker(&lock);
								if (currentToast.isNotNull()) {
									currentToast->removeFromParent();
									currentToast.setNull();
									animation.setNull();
								}
							}), AnimationCurve::Linear, AnimationFlags::NotSelfAlive);
						}
					}), (sl_uint32)(toast->duration * 1000));
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(ToastManager, GetToastManager)
			
			
			float g_defaultDuration = 2.0f;
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Font>, g_defaultFont)
			
		}
	}
	
	using namespace priv::toast;
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Toast)

	Toast::Toast()
	{
		duration = getDefaultDuration();
		font = getDefaultFont();
	}
	
	void Toast::show()
	{
		ToastManager* manager = GetToastManager();
		if (manager) {
			manager->show(this);
		}
	}
	
	void Toast::show(const String& text)
	{
		Toast toast;
		toast.text = text;
		toast.show();
	}
	
	float Toast::getDefaultDuration()
	{
		return g_defaultDuration;
	}
	
	void Toast::setDefaultDuration(float duration)
	{
		g_defaultDuration = duration;
	}
	
	Ref<Font> Toast::getDefaultFont()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_defaultFont)) {
			return sl_null;
		}
		if (g_defaultFont.isNull()) {
			g_defaultFont = Font::create("Arial", UI::dpToPixel(20));
		}
		return g_defaultFont;
	}
	
	void Toast::setDefaultFont(const Ref<Font>& font)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_defaultFont)) {
			return;
		}
		g_defaultFont = font;
	}
	
}
