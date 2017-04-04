/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/window.h"

#include "slib/ui/view.h"
#include "slib/ui/core.h"
#include "slib/ui/screen.h"

namespace slib
{
	
	WindowInstanceParam::WindowInstanceParam()
	{
		flagBorderless = sl_false;
		flagShowTitleBar = sl_false;
		flagFullScreen = sl_true;
		flagCenterScreen = sl_true;
		flagDialog = sl_false;
		flagModal = sl_false;
		
		activity = sl_null;
	}

	WindowInstanceParam::~WindowInstanceParam()
	{
	}

	UIRect WindowInstanceParam::calculateRegion(const UIRect& screenFrame) const
	{
		UIRect frame;
		if (flagFullScreen) {
			frame.setLeftTop(0, 0);
			frame.setSize(screenFrame.getSize());
		} else {
			if (flagCenterScreen) {
				frame.setLeftTop(screenFrame.getWidth() / 2 - size.x / 2, screenFrame.getHeight() / 2 - size.y / 2);
			} else {
				frame.setLeftTop(location);
			}
			frame.setSize(size);
		}
		frame.fixSizeError();
		return frame;
	}

#define CHECK_INSTANCE(instance) (instance.isNotNull() && !(instance->isClosed()))

	SLIB_DEFINE_OBJECT(Window, Object)

	Window::Window()
	{
		SLIB_REFERABLE_CONSTRUCTOR;
		
		Ref<View> view = new ViewGroup;
		if (view.isNotNull()) {
			view->setWindow(this);
			m_viewContent = view;
		}
		
		m_frame.left = 100;
		m_frame.top = 100;
		m_frame.right = 500;
		m_frame.bottom = 400;
		
		m_backgroundColor = Color::zero();
		
		m_flagVisible = sl_true;
		m_flagMinimized = sl_false;
		m_flagMaximized = sl_false;
		
		m_flagAlwaysOnTop = sl_false;
		m_flagCloseButtonEnabled = sl_true;
		m_flagMinimizeButtonEnabled = sl_false;
		m_flagMaximizeButtonEnabled = sl_false;
		m_flagResizable = sl_false;
		
		m_alpha = 1.0;
		m_flagTransparent = sl_false;
		
		m_flagModal = sl_false;
		m_flagDialog = sl_false;
		m_flagBorderless = sl_false;
		m_flagShowTitleBar = sl_true;
		m_flagFullScreenOnCreate = SLIB_IF_PLATFORM_IS_MOBILE(sl_true, sl_false);
		m_flagCenterScreenOnCreate = sl_false;
		
		m_flagUseClientSizeRequested = sl_false;
		
		m_activity = sl_null;
		
	}

	Window::~Window()
	{
	}

	void Window::close()
	{
		ObjectLocker lock(this);
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->close();
			detach();
			lock.unlock();
			dispatchDestroy();
		}
	}

	sl_bool Window::isClosed()
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->isClosed();
		}
		return sl_true;
	}

	sl_bool Window::isOpened()
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return !(instance->isClosed());
		}
		return sl_false;
	}

	Ref<Window> Window::getParent()
	{
		return m_parent;
	}

	void Window::setParent(const Ref<Window>& parent)
	{
		m_parent = parent;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			if (parent.isNotNull()) {
				instance->setParent(parent->m_instance);
			} else {
				instance->setParent(Ref<WindowInstance>::null());
			}
		}
	}

	Ref<Screen> Window::getScreen()
	{
		return m_screen;
	}

	void Window::setScreen(const Ref<Screen>& screen)
	{
		m_screen = screen;
	}

	Ref<View> Window::getContentView()
	{
		return m_viewContent;
	}

	void Window::setContentView(const Ref<View>& view)
	{
		ObjectLocker lock(this);
		
		Ref<View> orig = m_viewContent;
		Ref<ViewInstance> instance;
		if (orig.isNotNull()) {
			instance = orig->getViewInstance();
			orig->detach();
		}
		if (view.isNotNull()) {
			if (instance.isNotNull()) {
				view->attach(instance);
			}
			view->setWindow(this);
		}
		m_viewContent = view;
	}


	Ref<Menu> Window::getMenu()
	{
		return m_menu;
	}

	void Window::setMenu(const Ref<Menu>& menu)
	{
		m_menu = menu;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setMenu(menu);
		}
	}


	void Window::setFocus()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setFocus();
		}
	}

	UIRect Window::getFrame()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_frame = instance->getFrame();
		}
		return m_frame;
	}

	void Window::setFrame(const UIRect& frame)
	{
		m_frame = frame;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setFrame(frame);
		}
		m_flagUseClientSizeRequested = sl_false;
	}

	void Window::setFrame(sl_ui_pos left, sl_ui_pos top, sl_ui_len width, sl_ui_len height)
	{
		UIRect rect;
		rect.left = left;
		rect.top = top;
		rect.setSize(width, height);
		setFrame(rect);
	}

	UIPoint Window::getLocation()
	{
		return getFrame().getLocation();
	}

	void Window::setLocation(const UIPoint& location)
	{
		UIRect frame = getFrame();
		frame.setLocation(location);
		setFrame(frame);
	}

	void Window::setLocation(sl_ui_pos x, sl_ui_pos y)
	{
		setLocation(UIPoint(x, y));
	}

	sl_ui_pos Window::getLeft()
	{
		return getFrame().left;
	}

	void Window::setLeft(sl_ui_pos x)
	{
		UIRect frame = getFrame();
		frame.left = x;
		setFrame(frame);
	}

	sl_ui_pos Window::getTop()
	{
		return getFrame().top;
	}

	void Window::setTop(sl_ui_pos y)
	{
		UIRect frame = getFrame();
		frame.top = y;
		setFrame(frame);
	}

	UISize Window::getSize()
	{
		return getFrame().getSize();
	}

	void Window::setSize(const UISize& size)
	{
		UIRect frame = getFrame();
		frame.setSize(size);
		setFrame(frame);
	}

	void Window::setSize(sl_ui_len width, sl_ui_len height)
	{
		setSize(UISize(width, height));
	}

	sl_ui_len Window::getWidth()
	{
		return getFrame().getWidth();
	}

	void Window::setWidth(sl_ui_len width)
	{
		UIRect frame = getFrame();
		frame.setWidth(width);
		setFrame(frame);
	}

	sl_ui_len Window::getHeight()
	{
		return getFrame().getHeight();
	}

	void Window::setHeight(sl_ui_len height)
	{
		UIRect frame = getFrame();
		frame.setHeight(height);
		setFrame(frame);
	}

	UIRect Window::getClientFrame()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			return instance->getClientFrame();
		}
		return UIRect::zero();
	}

	UISize Window::getClientSize()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			return instance->getClientSize();
		} else {
			if (m_flagUseClientSizeRequested) {
				return m_clientSizeRequested;
			} else {
				return UISize::zero();
			}
		}
	}

	void Window::setClientSize(const UISize& size)
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_flagUseClientSizeRequested = sl_false;
			instance->setClientSize(size);
		} else {
			m_flagUseClientSizeRequested = sl_true;
			m_clientSizeRequested = size;
			m_frame.setSize(size);
		}
	}

	void Window::setClientSize(sl_ui_len width, sl_ui_len height)
	{
		setClientSize(UISize(width, height));
	}

	UIRect Window::getClientBounds()
	{
		UISize size = getClientSize();
		return UIRect(0, 0, size.x, size.y);
	}

	String Window::getTitle()
	{
		return m_title;
	}

	void Window::setTitle(const String& title)
	{
		m_title = title;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setTitle(title);
		}
	}

	Color Window::getBackgroundColor()
	{
		return m_backgroundColor;
	}

	void Window::setBackgroundColor(const Color& color)
	{
		m_backgroundColor = color;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setBackgroundColor(color);
		}
	}

	sl_bool Window::isMinimized()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_flagMinimized = instance->isMinimized();
		}
		return m_flagMinimized;
	}

	void Window::setMinimized(sl_bool flag)
	{
		m_flagMinimized = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setMinimized(flag);
		}
	}

	sl_bool Window::isMaximized()
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_flagMaximized = instance->isMaximized();
		}
		return m_flagMaximized;
	}

	void Window::setMaximized(sl_bool flag)
	{
		m_flagMaximized = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setMaximized(flag);
		}
	}

	sl_bool Window::isVisible()
	{
		return m_flagVisible;
	}

	void Window::setVisible(sl_bool flag)
	{
		m_flagVisible = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setVisible(flag);
		}
	}

	sl_bool Window::isAlwaysOnTop()
	{
		return m_flagAlwaysOnTop;
	}

	void Window::setAlwaysOnTop(sl_bool flag)
	{
		m_flagAlwaysOnTop = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setAlwaysOnTop(flag);
		}
	}

	sl_bool Window::isCloseButtonEnabled()
	{
		return m_flagCloseButtonEnabled;
	}

	void Window::setCloseButtonEnabled(sl_bool flag)
	{
		m_flagCloseButtonEnabled = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setCloseButtonEnabled(flag);
		}
	}

	sl_bool Window::isMinimizeButtonEnabled()
	{
		return m_flagMinimizeButtonEnabled;
	}

	void Window::setMinimizeButtonEnabled(sl_bool flag)
	{
		m_flagMinimizeButtonEnabled = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setMinimizeButtonEnabled(flag);
		}
	}

	sl_bool Window::isMaximizeButtonEnabled()
	{
		return m_flagMaximizeButtonEnabled;
	}

	void Window::setMaximizeButtonEnabled(sl_bool flag)
	{
		m_flagMaximizeButtonEnabled = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setMaximizeButtonEnabled(flag);
		}
	}

	sl_bool Window::isResizable()
	{
		return m_flagResizable;
	}

	void Window::setResizable(sl_bool flag)
	{
		m_flagResizable = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setResizable(flag);
		}
	}

	sl_real Window::getAlpha()
	{
		return m_alpha;
	}

	void Window::setAlpha(sl_real alpha)
	{
		m_alpha = alpha;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setAlpha(alpha);
		}
	}

	sl_bool Window::isTransparent()
	{
		return m_flagTransparent;
	}

	void Window::setTransparent(sl_bool flag)
	{
		m_flagTransparent = flag;
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			instance->setTransparent(flag);
		}
	}

	UIPointf Window::convertCoordinateFromScreenToWindow(const UIPointf& ptScreen)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromScreenToWindow(ptScreen);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromScreenToWindow(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromScreenToWindow(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromScreenToWindow(rect.getRightBottom()));
		return ret;
	}

	UIPointf Window::convertCoordinateFromWindowToScreen(const UIPointf& ptWindow)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromWindowToScreen(ptWindow);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromWindowToScreen(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromWindowToScreen(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromWindowToScreen(rect.getRightBottom()));
		return ret;
	}

	UIPointf Window::convertCoordinateFromScreenToClient(const UIPointf& ptScreen)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromScreenToClient(ptScreen);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromScreenToClient(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromScreenToClient(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromScreenToClient(rect.getRightBottom()));
		return ret;
	}

	UIPointf Window::convertCoordinateFromClientToScreen(const UIPointf& ptClient)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromClientToScreen(ptClient);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromClientToScreen(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromClientToScreen(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromClientToScreen(rect.getRightBottom()));
		return ret;
	}

	UIPointf Window::convertCoordinateFromWindowToClient(const UIPointf& ptWindow)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromWindowToClient(ptWindow);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromWindowToClient(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromWindowToClient(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromWindowToClient(rect.getRightBottom()));
		return ret;
	}

	UIPointf Window::convertCoordinateFromClientToWindow(const UIPointf& ptClient)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->convertCoordinateFromClientToWindow(ptClient);
		} else {
			return UIPointf::zero();
		}
	}

	UIRectf Window::convertCoordinateFromClientToWindow(const UIRectf& rect)
	{
		UIRectf ret;
		ret.setLeftTop(convertCoordinateFromClientToWindow(rect.getLeftTop()));
		ret.setRightBottom(convertCoordinateFromClientToWindow(rect.getRightBottom()));
		return ret;
	}

	UISize Window::getWindowSizeFromClientSize(const UISize& sizeClient)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->getWindowSizeFromClientSize(sizeClient);
		} else {
			return UISize::zero();
		}
	}

	UISize Window::getClientSizeFromWindowSize(const UISize& sizeWindow)
	{
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			return instance->getClientSizeFromWindowSize(sizeWindow);
		} else {
			return UISize::zero();
		}
	}

	sl_bool Window::isModal()
	{
		return m_flagModal;
	}

	void Window::setModal(sl_bool flag)
	{
		m_flagModal = flag;
	}

	sl_bool Window::isDialog()
	{
		return m_flagDialog;
	}

	void Window::setDialog(sl_bool flag)
	{
		m_flagDialog = flag;
	}

	sl_bool Window::isBorderless()
	{
		return m_flagBorderless;
	}

	void Window::setBorderless(sl_bool flag)
	{
		m_flagBorderless = flag;
	}

	sl_bool Window::isTitleBarVisible()
	{
		return m_flagShowTitleBar;
	}

	void Window::setTitleBarVisible(sl_bool flag)
	{
		m_flagShowTitleBar = flag;
	}

	sl_bool Window::isFullScreenOnCreate()
	{
		return m_flagFullScreenOnCreate;
	}

	void Window::setFullScreenOnCreate(sl_bool flag)
	{
		m_flagFullScreenOnCreate = flag;
	}

	sl_bool Window::isCenterScreenOnCreate()
	{
		return m_flagCenterScreenOnCreate;
	}

	void Window::setCenterScreenOnCreate(sl_bool flag)
	{
		m_flagCenterScreenOnCreate = flag;
	}

	void* Window::getActivity()
	{
		return m_activity;
	}

	void Window::setActivity(void* activity)
	{
		m_activity = activity;
	}

	Ref<WindowInstance> Window::getWindowInstance()
	{
		return m_instance;
	}

	void Window::attach(const Ref<WindowInstance>& instance, sl_bool flagAttachContent)
	{
		ObjectLocker lock(this);

		detach();
		
		if (instance.isNotNull()) {
			instance->setWindow(this);
			m_instance = instance;
			if (flagAttachContent) {
				lock.unlock();
				_attachContent();
			}
		}
	}
	
	void Window::detach()
	{
		ObjectLocker lock(this);
		
		// save current window's properties
		getFrame();
		isMaximized();
		isMinimized();

		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			view->removeParent();
			view->detach();
		}
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setWindow(Ref<Window>::null());
		}
		m_instance.setNull();
	}

	void Window::create()
	{
		if (UI::isUiThread()) {
			_create();
		} else {
			UI::dispatchToUiThread(SLIB_FUNCTION_REF(Window, _create, this));
		}
	}

	void Window::forceCreate()
	{
		detach();
		create();
	}

	void Window::_create()
	{
		if (m_instance.isNotNull()) {
			return;
		}
		
		WindowInstanceParam param;

		Ref<Window> parent = m_parent;
		if (parent.isNotNull()) {
			param.parent = parent->m_instance;
		}
		
		param.screen = m_screen;
		param.menu = m_menu;
		param.flagBorderless = m_flagBorderless;
		param.flagFullScreen = m_flagFullScreenOnCreate;
		param.flagCenterScreen = m_flagCenterScreenOnCreate;
		param.flagDialog = m_flagDialog;
		param.flagModal = m_flagModal;
		param.location = m_frame.getLocation();
		param.size = m_frame.getSize();
		param.title = m_title;
		param.flagShowTitleBar = m_flagShowTitleBar;
		param.activity = m_activity;
		
		Ref<WindowInstance> window = createWindowInstance(param);
		
		if (window.isNotNull()) {
			
			window->setBackgroundColor(m_backgroundColor);
			
			window->setVisible(m_flagVisible);
			window->setMinimized(m_flagMinimized);
			window->setMaximized(m_flagMaximized);
			
			window->setAlwaysOnTop(m_flagAlwaysOnTop);
			window->setCloseButtonEnabled(m_flagCloseButtonEnabled);
			window->setMinimizeButtonEnabled(m_flagMinimizeButtonEnabled);
			window->setMaximizeButtonEnabled(m_flagMaximizeButtonEnabled);
			window->setResizable(m_flagResizable);
			window->setAlpha(m_alpha);
			window->setTransparent(m_flagTransparent);
			
			if (m_flagUseClientSizeRequested) {
				UISize size = window->getWindowSizeFromClientSize(m_clientSizeRequested);
				m_frame = window->getFrame();
				m_frame.setSize(size);
				window->setFrame(m_frame);
			}
			
			window->setVisible(sl_true);

			attach(window, sl_false);
			
#if defined(SLIB_PLATFORM_IS_OSX) || defined(SLIB_PLATFORM_IS_IOS) || defined(SLIB_PLATFORM_IS_TIZEN)
			UISize sizeClient = getClientSize();
			dispatchResize(sizeClient.x, sizeClient.y);
#endif

			dispatchCreate();
			
		} else {
			dispatchCreateFailed();
		}
		
	}
	
	void Window::_attachContent()
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_FUNCTION_REF(Window, _attachContent, this));
			return;
		}
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			Ref<ViewInstance> contentViewInstance = instance->getContentView();
			if (contentViewInstance.isNotNull()) {
				Ref<View> view = m_viewContent;
				if (view.isNotNull()) {
					view->removeParent();
					view->detach();
					view->attach(contentViewInstance);
				}
			}
		}
	}

	void Window::addView(const Ref<View>& child)
	{
		if (child.isNotNull()) {
			Ref<View> view = m_viewContent;
			if (view.isNotNull()) {
				view->addChild(child);
			}
		}
	}

	void Window::removeView(const Ref<View>& child)
	{
		if (child.isNotNull()) {
			Ref<View> view = m_viewContent;
			if (view.isNotNull()) {
				view->removeChild(child);
			}
		}
	}

	List< Ref<View> > Window::getViews()
	{
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			return view->getChildren();
		}
		return sl_null;
	}

	void Window::removeAllViews()
	{
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			view->removeAllChildren();
		}
	}

	void Window::onCreate()
	{
	}

	void Window::onCreateFailed()
	{
	}

	void Window::onClose(UIEvent* ev)
	{
	}

	void Window::onDestroy()
	{
	}

	void Window::onActivate()
	{
	}

	void Window::onDeactivate()
	{
	}

	void Window::onMove()
	{
	}

	void Window::onResizing(UISize& size)
	{
	}

	void Window::onResize(sl_ui_len width, sl_ui_len height)
	{
	}

	void Window::onMinimize()
	{
	}

	void Window::onDeminimize()
	{
	}

	void Window::onMaximize()
	{
	}

	void Window::onDemaximize()
	{
	}

	void Window::onOK(UIEvent* ev)
	{
	}

	void Window::onCancel(UIEvent* ev)
	{
	}

	void Window::dispatchCreate()
	{
		onCreate();
		getOnCreate()(this);
	}

	void Window::dispatchCreateFailed()
	{
		onCreateFailed();
		getOnCreateFailed()(this);
	}

	void Window::dispatchClose(UIEvent* ev)
	{
		onClose(ev);
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onClose(this, ev);
		}
		getOnClose()(this,ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		detach();
		dispatchDestroy();
	}

	void Window::dispatchDestroy()
	{
		onDestroy();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onCancel(this);
		}
		getOnDestroy()(this);
	}

	void Window::dispatchActivate()
	{
		onActivate();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onActivate(this);
		}
		getOnActivate()(this);
	}

	void Window::dispatchDeactivate()
	{
		onDeactivate();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onDeactivate(this);
		}
		getOnDeactivate()(this);
	}

	void Window::dispatchMove()
	{
		onMove();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onMove(this);
		}
		getOnMove()(this);
	}

	void Window::dispatchResizing(UISize& size)
	{
		onResizing(size);
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onResizing(this, size);
		}
		getOnResizing()(this, size);
	}

	void Window::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_refreshSize();
		if (width > 0 && height > 0) {
			Ref<View> viewContent = m_viewContent;
			if (viewContent.isNotNull()) {
				if (!(viewContent->isInstance())) {
					_attachContent();
				}
			}
		}
		onResize(width, height);
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onResize(this, width, height);
		}
		getOnResize()(this, width, height);
	}

	void Window::dispatchMinimize()
	{
		onMinimize();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onMinimize(this);
		}
		getOnMinimize()(this);
	}

	void Window::dispatchDeminimize()
	{
		onDeminimize();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onDeminimize(this);
		}
		getOnDeminimize()(this);
	}

	void Window::dispatchMaximize()
	{
		_refreshSize();
		onMaximize();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onMaximize(this);
		}
		getOnMaximize()(this);
	}

	void Window::dispatchDemaximize()
	{
		_refreshSize();
		onDemaximize();
		PtrLocker<IWindowListener> listener(getEventListener());
		if (listener.isNotNull()) {
			listener->onDemaximize(this);
		}
		getOnDemaximize()(this);
	}

	void Window::dispatchOK(UIEvent* ev)
	{
		onOK(ev);
		getOnOK()(this, ev);
	}

	void Window::dispatchOK()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchOK(ev.get());
		}
	}

	void Window::dispatchCancel(UIEvent* ev)
	{
		onCancel(ev);
		getOnCancel()(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		close();
	}

	void Window::dispatchCancel()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchCancel(ev.get());
		}
	}

	void Window::_refreshSize()
	{
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			UIRect rect;
			rect.setLeftTop(UIPoint::zero());
			rect.setSize(getClientSize());
			view->setFrame(rect);
		}
	}


	SLIB_DEFINE_OBJECT(WindowInstance, Object)

	WindowInstance::WindowInstance()
	{
	}

	WindowInstance::~WindowInstance()
	{
	}

	Ref<Window> WindowInstance::getWindow()
	{
		return m_window;
	}

	void WindowInstance::setWindow(const Ref<Window>& window)
	{
		m_window = window;
	}

	void WindowInstance::setMenu(const Ref<Menu>& menu)
	{
	}

	sl_bool WindowInstance::onClose()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
			if (ev.isNotNull()) {
				window->dispatchClose(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_false;
				}
			}
		}
		return sl_true;
	}

	void WindowInstance::onActivate()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchActivate();
		}
	}

	void WindowInstance::onDeactivate()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchDeactivate();
		}
	}

	void WindowInstance::onMove()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchMove();
		}
	}

	void WindowInstance::onResizing(UISize& size)
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchResizing(size);
		}
	}

	void WindowInstance::onResize(sl_ui_len width, sl_ui_len height)
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchResize(width, height);
		}
	}

	void WindowInstance::onMinimize()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchMinimize();
		}
	}

	void WindowInstance::onDeminimize()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchDeminimize();
		}
	}

	void WindowInstance::onMaximize()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchMaximize();
		}
	}

	void WindowInstance::onDemaximize()
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchDemaximize();
		}
	}

}
