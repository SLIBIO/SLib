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

#ifndef CHECKHEADER_SLIB_UI_WINDOW
#define CHECKHEADER_SLIB_UI_WINDOW

#include "definition.h"

#include "event.h"
#include "menu.h"

#include "../core/string.h"
#include "../core/object.h"
#include "../core/function.h"
#include "../graphics/color.h"
#include "../math/rectangle.h"

namespace slib
{

	class Screen;
	class View;
	class ViewInstance;
	class Window;
	class WindowInstance;
	
	class Variant;
	
	namespace priv
	{
		namespace window
		{
			class ContentView;
		}
	}

	class SLIB_EXPORT WindowInstanceParam
	{
	public:
		Ref<WindowInstance> parent;
		
		Ref<Screen> screen;
		Ref<Menu> menu;
		sl_bool flagBorderless;
		sl_bool flagFullScreen;
		sl_bool flagCenterScreen;
		sl_bool flagDialog;
		sl_bool flagModal;
		sl_bool flagSheet; // Used in macOS
		UIPoint location;
		UISize size;

		String title;
		sl_bool flagShowTitleBar;

#if defined(SLIB_UI_IS_ANDROID)
		// jobject
		void* activity;
#endif

#if defined(SLIB_UI_IS_GTK)
		sl_bool flagClientSize;
#endif
		
	public:
		WindowInstanceParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(WindowInstanceParam)
		
	public:
		UIRect calculateRegion(const UIRect& screenFrame) const;
		
	};

	class SLIB_EXPORT Window : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Window();
		
		~Window();
		
	protected:
		void init() override;
		
	public:
		virtual void close();
		
		sl_bool isClosed();
		
		sl_bool isOpened();
		
		
		Ref<Window> getParent();
		
		virtual void setParent(const Ref<Window>& parent);
		
		
		Ref<Screen> getScreen();
		
		void setScreen(const Ref<Screen>& screen);

		
		const Ref<View>& getContentView();
		
		
		Ref<Menu> getMenu();
		
		virtual void setMenu(const Ref<Menu>& menu);
		
		
		sl_bool isActive();
		
		virtual void activate();
		

		UIRect getFrame();
		
		virtual void setFrame(const UIRect& frame);
		
		void setFrame(sl_ui_pos left, sl_ui_pos top, sl_ui_len width, sl_ui_len height);
		
		UIPoint getLocation();
		
		virtual void setLocation(const UIPoint& location);
		
		void setLocation(sl_ui_pos x, sl_ui_pos y);
		
		sl_ui_pos getLeft();
		
		void setLeft(sl_ui_pos x);
		
		sl_ui_pos getTop();
		
		void setTop(sl_ui_pos y);
		
		virtual UISize getSize();
		
		virtual void setSize(const UISize& size);
		
		void setSize(sl_ui_len width, sl_ui_len height);
		
		sl_ui_len getWidth();
		
		void setWidth(sl_ui_len width);
		
		sl_ui_len getHeight();
		
		void setHeight(sl_ui_len height);

		sl_bool isWidthWrapping();
		
		void setWidthWrapping(sl_bool flag = sl_true);
		
		sl_bool isHeightWrapping();
		
		void setHeightWrapping(sl_bool flag = sl_true);
		
		
		UIRect getClientFrame();
		
		UISize getClientSize();
		
		virtual void setClientSize(const UISize& size);

		void setClientSize(sl_ui_len width, sl_ui_len height);
		
		sl_ui_len getClientWidth();
		
		void setClientWidth(sl_ui_len width);
		
		sl_ui_len getClientHeight();
		
		void setClientHeight(sl_ui_len height);

		UIRect getClientBounds();

		
		String getTitle();
		
		virtual void setTitle(const String& title);
		
		
		Color getBackgroundColor();
		
		virtual void setBackgroundColor(const Color& color);
		
		
		sl_bool isMinimized();
		
		virtual void setMinimized(sl_bool flag);
		
		
		sl_bool isMaximized();
		
		virtual void setMaximized(sl_bool flag);
		
		
		sl_bool isVisible();
		
		virtual void setVisible(sl_bool flag);
		
		
		sl_bool isAlwaysOnTop();
		
		virtual void setAlwaysOnTop(sl_bool flag);
		
		
		sl_bool isCloseButtonEnabled();
		
		virtual void setCloseButtonEnabled(sl_bool flag);
		
		
		sl_bool isMinimizeButtonEnabled();
		
		virtual void setMinimizeButtonEnabled(sl_bool flag);
		
		
		sl_bool isMaximizeButtonEnabled();
		
		virtual void setMaximizeButtonEnabled(sl_bool flag);
		
		
		sl_bool isResizable();
		
		virtual void setResizable(sl_bool flag);
		
		
		sl_real getAlpha();
		
		virtual void setAlpha(sl_real alpha);
		
		
		sl_bool isTransparent();
		
		virtual void setTransparent(sl_bool flag);

		
		virtual UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen);
		
		UIRectf convertCoordinateFromScreenToWindow(const UIRectf& rect);
		
		virtual UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow);

		UIRectf convertCoordinateFromWindowToScreen(const UIRectf& rect);
		
		virtual UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen);
		
		UIRectf convertCoordinateFromScreenToClient(const UIRectf& rect);
		
		virtual UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient);
		
		UIRectf convertCoordinateFromClientToScreen(const UIRectf& rect);
		
		virtual UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow);
		
		UIRectf convertCoordinateFromWindowToClient(const UIRectf& rect);
		
		virtual UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient);
		
		UIRectf convertCoordinateFromClientToWindow(const UIRectf& rect);
		
		virtual UISize getWindowSizeFromClientSize(const UISize& sizeClient);
		
		virtual UISize getClientSizeFromWindowSize(const UISize& sizeWindow);

		// aspect = width / height
		virtual void setSizeRange(const UISize& sizeMinimum, const UISize& sizeMaximum, float aspectRatioMinimum = 0, float aspectRatioMaximum = 0);
		
		UISize getMinimumSize();
		
		void setMinimumSize(const UISize& sizeMinimum);
		
		void setMinimumSize(sl_ui_len width, sl_ui_len height);
		
		sl_ui_len getMinimumWidth();
		
		void setMinimumWidth(sl_ui_len width);
		
		sl_ui_len getMinimumHeight();
		
		void setMinimumHeight(sl_ui_len height);

		UISize getMaximumSize();
		
		void setMaximumSize(const UISize& sizeMaximum);
		
		void setMaximumSize(sl_ui_len width, sl_ui_len height);

		sl_ui_len getMaximumWidth();
		
		void setMaximumWidth(sl_ui_len width);
		
		sl_ui_len getMaximumHeight();
		
		void setMaximumHeight(sl_ui_len height);
		
		float getMinimumAspectRatio();
		
		void setMinimumAspectRatio(float ratio);
		
		float getMaximumAspectRatio();
		
		void setMaximumAspectRatio(float ratio);
		
		void setAspectRatio(float ratio);

		
		sl_bool isModal();
		
		void setModal(sl_bool flag);
		
		
		sl_bool isSheet();
		
		void setSheet(sl_bool flag);
		

		sl_bool isDialog();
		
		void setDialog(sl_bool flag);
		
		
		sl_bool isBorderless();
		
		void setBorderless(sl_bool flag);
		
		
		sl_bool isTitleBarVisible();
		
		void setTitleBarVisible(sl_bool flag);

		
		sl_bool isFullScreenOnCreate();
		
		void setFullScreenOnCreate(sl_bool flag);
		
		
		sl_bool isCenterScreenOnCreate();
		
		void setCenterScreenOnCreate(sl_bool flag);
		
		
		sl_bool isCloseOnOK();
		
		void setCloseOnOK(sl_bool flag = sl_true);
		
		
		Variant getResult();
		
		void setResult(const Variant& result);
		
		void close(const Variant& result);
		
		
#if defined(SLIB_UI_IS_ANDROID)
		void* getActivity();
		
		void setActivity(void* activity);
#endif
		
	public:
		Ref<WindowInstance> getWindowInstance();
		
		void attach(const Ref<WindowInstance>& instance, sl_bool flagAttachContent = sl_true);

		void detach();

		void create();
		
		void createAndKeep();
		
		void forceCreate();
		
		void forceCreateAndKeep();
		
		Variant doModal();
		
		void showModal();

		
		void addView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		void removeView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

		List< Ref<View> > getViews();
		
		void removeAllViews(UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
	public:
		static Ref<Window> getActiveWindow();

	public:
		SLIB_DECLARE_EVENT_HANDLER(Window, Create)
		SLIB_DECLARE_EVENT_HANDLER(Window, CreateFailed)
		SLIB_DECLARE_EVENT_HANDLER(Window, Close, UIEvent* ev)
		SLIB_DECLARE_EVENT_HANDLER(Window, Destroy)
		SLIB_DECLARE_EVENT_HANDLER(Window, Activate)
		SLIB_DECLARE_EVENT_HANDLER(Window, Deactivate)
		SLIB_DECLARE_EVENT_HANDLER(Window, Move)
		SLIB_DECLARE_EVENT_HANDLER(Window, Resizing, UISize& size)
		SLIB_DECLARE_EVENT_HANDLER(Window, Resize, sl_ui_len clientWidth, sl_ui_len clientHeight)
		SLIB_DECLARE_EVENT_HANDLER(Window, Minimize)
		SLIB_DECLARE_EVENT_HANDLER(Window, Deminimize)
		SLIB_DECLARE_EVENT_HANDLER(Window, Maximize)
		SLIB_DECLARE_EVENT_HANDLER(Window, Demaximize)
		SLIB_DECLARE_EVENT_HANDLER(Window, OK, UIEvent* ev)
		SLIB_DECLARE_EVENT_HANDLER(Window, Cancel, UIEvent* ev)

	public:
		void dispatchOK();
		
		void dispatchCancel();
		
	private:
		Ref<WindowInstance> createWindowInstance(const WindowInstanceParam& param);

		void _create(sl_bool flagKeepReference);
		
		void _attachContent();
		
		void _refreshSize(const UISize& size);
		
		void _constrainSize(UISize& size, sl_bool flagAdjustHeight);
		
		void _constrainSize(UIRect& frame, sl_bool flagAdjustHeight);
		
		void _applyContentWrappingSize();
		
	private:
		AtomicRef<WindowInstance> m_instance;
		AtomicWeakRef<Window> m_parent;
		Ref<priv::window::ContentView> m_viewContent;
		AtomicRef<Screen> m_screen;
		AtomicRef<Menu> m_menu;
		
		UIRect m_frame;
		AtomicString m_title;
		Color m_backgroundColor;
		
		sl_real m_alpha;
		UISize m_clientSizeRequested;
		
		UISize m_sizeMin;
		UISize m_sizeMax;
		float m_aspectRatioMinimum;
		float m_aspectRatioMaximum;
		
		sl_bool m_flagVisible : 1;
		sl_bool m_flagMinimized : 1;
		sl_bool m_flagMaximized : 1;
		
		sl_bool m_flagAlwaysOnTop : 1;
		sl_bool m_flagCloseButtonEnabled : 1;
		sl_bool m_flagMinimizeButtonEnabled : 1;
		sl_bool m_flagMaximizeButtonEnabled : 1;
		sl_bool m_flagResizable : 1;
		sl_bool m_flagTransparent : 1;
		
		sl_bool m_flagModal : 1;
		sl_bool m_flagSheet : 1;
		sl_bool m_flagDialog : 1;
		sl_bool m_flagBorderless: 1;
		sl_bool m_flagShowTitleBar : 1;
		sl_bool m_flagFullScreenOnCreate : 1;
		sl_bool m_flagCenterScreenOnCreate : 1;
		sl_bool m_flagWidthWrapping : 1;
		sl_bool m_flagHeightWrapping : 1;
		sl_bool m_flagCloseOnOK : 1;

		sl_bool m_flagUseClientSizeRequested : 1;
		sl_bool m_flagStateResizingWidth : 1;
		sl_bool m_flagStateDoModal : 1;
		sl_bool m_flagDispatchedDestroy : 1;
		
		Variant* m_result;
		SpinLock m_lockResult;

#if defined(SLIB_UI_IS_ANDROID)
		// jobject
		void* m_activity;
#endif
		friend class WindowInstance;
		
	};


	class SLIB_EXPORT WindowInstance : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		WindowInstance();

		~WindowInstance();

	public:
		Ref<Window> getWindow();
		
		void setWindow(const Ref<Window>& window);
		
		void setKeepWindow(sl_bool flag);
		
	public:
		virtual void close() = 0;
		
		virtual sl_bool isClosed() = 0;
		

		virtual sl_bool setParent(const Ref<WindowInstance>& parent) = 0;
		
		virtual Ref<ViewInstance> getContentView() = 0;
		
		virtual void setMenu(const Ref<Menu>& menu);
		
		
		virtual sl_bool isActive() = 0;

		virtual sl_bool activate() = 0;

		
		virtual UIRect getFrame() = 0;
		
		virtual sl_bool setFrame(const UIRect& frame) = 0;
		
		virtual UIRect getClientFrame() = 0;
		
		virtual UISize getClientSize() = 0;
		
		virtual sl_bool setClientSize(const UISize& size) = 0;
		

		virtual sl_bool setTitle(const String& title) = 0;
		
		virtual sl_bool setBackgroundColor(const Color& color) = 0;

		
		virtual sl_bool isMinimized() = 0;
		
		virtual sl_bool setMinimized(sl_bool flag) = 0;
		
		
		virtual sl_bool isMaximized() = 0;
		
		virtual sl_bool setMaximized(sl_bool flag) = 0;
		
		
		virtual sl_bool setVisible(sl_bool flag) = 0;

		virtual sl_bool setAlwaysOnTop(sl_bool flag) = 0;
		
		virtual sl_bool setCloseButtonEnabled(sl_bool flag) = 0;
		
		virtual sl_bool setMinimizeButtonEnabled(sl_bool flag) = 0;
		
		virtual sl_bool setMaximizeButtonEnabled(sl_bool flag) = 0;
		
		virtual sl_bool setResizable(sl_bool flag) = 0;
		
		virtual sl_bool setAlpha(sl_real alpha) = 0;
		
		virtual sl_bool setTransparent(sl_bool flag) = 0;

		
		virtual UIPointf convertCoordinateFromScreenToWindow(const UIPointf& ptScreen) = 0;
		
		virtual UIPointf convertCoordinateFromWindowToScreen(const UIPointf& ptWindow) = 0;
		
		virtual UIPointf convertCoordinateFromScreenToClient(const UIPointf& ptScreen) = 0;
		
		virtual UIPointf convertCoordinateFromClientToScreen(const UIPointf& ptClient) = 0;
		
		virtual UIPointf convertCoordinateFromWindowToClient(const UIPointf& ptWindow) = 0;
		
		virtual UIPointf convertCoordinateFromClientToWindow(const UIPointf& ptClient) = 0;
		
		virtual UISize getWindowSizeFromClientSize(const UISize& sizeClient) = 0;
		
		virtual UISize getClientSizeFromWindowSize(const UISize& sizeWindow) = 0;
		
		
		virtual void setSizeRange(const UISize& sizeMinimum, const UISize& sizeMaximum, float aspectRatioMinimum, float aspectRatioMaximum);
		
		virtual sl_bool doModal();
		
	public:
		sl_bool onClose();
		
		void onActivate();
		
		void onDeactivate();
		
		void onMove();
		
		void onResizing(UISize& size, sl_bool flagResizingWidth);
		
		void onResize(sl_ui_len clientWidth, sl_ui_len clientHeight);
		
		void onMinimize();
		
		void onDeminimize();
		
		void onMaximize();
		
		void onDemaximize();

	private:
		AtomicWeakRef<Window> m_window;
		sl_bool m_flagKeepWindow;
		
	};

}

#endif
