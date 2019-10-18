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
						font = Toast::getDefaultFont();
						if (font.isNull()) {
							return;
						}
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
					Alignment halign = toast->gravity & Alignment::HorizontalMask;
					Alignment valign = toast->gravity & Alignment::VerticalMask;
					if (halign == Alignment::Left) {
						view->setAlignParentLeft(UIUpdateMode::Init);
					} else if (halign == Alignment::Right) {
						view->setAlignParentRight(UIUpdateMode::Init);
					} else {
						view->setCenterHorizontal(UIUpdateMode::Init);
					}
					if (valign == Alignment::Top) {
						view->setAlignParentTop(UIUpdateMode::Init);
					} else if (valign == Alignment::Bottom) {
						view->setAlignParentBottom(UIUpdateMode::Init);
					} else {
						view->setCenterVertical(UIUpdateMode::Init);
					}
					view->setMargin(toast->margin, UIUpdateMode::Init);
					view->setClipping(sl_true, UIUpdateMode::Init);
					currentToast = view;
					parent->addChild(view);
					animation = view->startAlphaAnimation(0, 1, 0.3f, sl_null, AnimationCurve::Linear, AnimationFlags::NotSelfAlive);
					
					sl_uint32 duration = toast->duration;
					if (!duration) {
						duration = Toast::getDefaultDuration();
					}
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
					}), duration);
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(ToastManager, GetToastManager)
			
			// milliseconds
			sl_uint32 g_defaultDuration = 2000;
			
			SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Font>, g_defaultFont)
			
		}
	}
	
	using namespace priv::toast;
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Toast)

	Toast::Toast()
	{
		duration = 0; // default duration will be used
		gravity = Alignment::MiddleCenter;
		margin.left = margin.top = margin.right = margin.bottom = UI::dpToPixel(50);
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
	
	void Toast::show(const String& text, sl_uint32 duration)
	{
		Toast toast;
		toast.text = text;
		toast.duration = duration;
		toast.show();
	}
	
	void Toast::show(const String& text, sl_uint32 duration, const Ref<Font>& font)
	{
		Toast toast;
		toast.text = text;
		toast.duration = duration;
		toast.font = font;
		toast.show();
	}
	
	void Toast::show(const String& text, sl_uint32 duration, const Ref<Font>& font, const Alignment& gravity)
	{
		Toast toast;
		toast.text = text;
		toast.duration = duration;
		toast.font = font;
		toast.gravity = gravity;
		toast.show();
	}
	
	void Toast::show(const String& text, sl_uint32 duration, const Ref<Font>& font, const Alignment& gravity, const UIEdgeInsets& margin)
	{
		Toast toast;
		toast.text = text;
		toast.duration = duration;
		toast.font = font;
		toast.gravity = gravity;
		toast.margin = margin;
		toast.show();
	}
	
	sl_uint32 Toast::getDefaultDuration()
	{
		return g_defaultDuration;
	}
	
	void Toast::setDefaultDuration(sl_uint32 duration)
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
