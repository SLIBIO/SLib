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

#include "slib/ui/window.h"

#include "slib/ui/view.h"
#include "slib/ui/core.h"
#include "slib/ui/screen.h"

#include "slib/core/variant.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(WindowInstanceParam)
	
	WindowInstanceParam::WindowInstanceParam()
	{
		flagBorderless = sl_false;
		flagShowTitleBar = sl_false;
		flagFullScreen = sl_true;
		flagCenterScreen = sl_true;
		flagDialog = sl_false;
		flagModal = sl_false;
		flagSheet = sl_false;
		
#if defined(SLIB_UI_IS_ANDROID)
		activity = sl_null;
#endif
#if defined(SLIB_UI_IS_GTK)
		flagClientSize = sl_false;
#endif
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
	
	namespace priv
	{
		namespace window
		{
			class ContentView : public ViewGroup
			{
			protected:
				void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override
				{
					applyWrappingContentSize();
				}
				
			public:
				void applyWrappingContentSize()
				{
					Ref<Window> window = getWindow();
					if (window.isNull()) {
						return;
					}
					sl_bool flagHorz = window->isWidthWrapping();
					sl_bool flagVert = window->isHeightWrapping();
					if (!flagHorz && !flagVert) {
						return;
					}
					UISize sizeOld = window->getClientSize();
					UISize sizeNew = sizeOld;
					UISize sizeMeasured = measureLayoutWrappingSize(flagHorz, flagVert);
					if (flagHorz) {
						sizeNew.x = sizeMeasured.x;
					}
					if (flagVert) {
						sizeNew.y = sizeMeasured.y;
					}
					if (sizeNew.isAlmostEqual(sizeOld)) {
						return;
					}
					if (window->getWindowInstance().isNotNull()) {
						if (window->isCenterScreenOnCreate()) {
							UIPoint posOld = window->getLocation();
							window->setClientSize(sizeNew);
							window->setLocation(posOld + (sizeOld - sizeNew) / 2);
							return;
						}
					}
					window->setClientSize(sizeNew);
				}
				
			};
		}
	}
	
#define CHECK_INSTANCE(instance) (instance.isNotNull() && !(instance->isClosed()))

	SLIB_DEFINE_OBJECT(Window, Object)

	Window::Window()
	{
		m_frame.left = 100;
		m_frame.top = 100;
		m_frame.right = 500;
		m_frame.bottom = 400;
		
		m_backgroundColor = Color::zero();
		
		m_alpha = 1.0;
		
		m_sizeMin.x = 0;
		m_sizeMin.y = 0;
		m_sizeMax.x = 0;
		m_sizeMax.y = 0;
		m_aspectRatioMinimum = 0;
		m_aspectRatioMaximum = 0;

		m_flagVisible = sl_true;
		m_flagMinimized = sl_false;
		m_flagMaximized = sl_false;
		
		m_flagAlwaysOnTop = sl_false;
		m_flagCloseButtonEnabled = sl_true;
		m_flagMinimizeButtonEnabled = sl_false;
		m_flagMaximizeButtonEnabled = sl_false;
		m_flagResizable = sl_false;
		m_flagTransparent = sl_false;
		
		m_flagModal = sl_false;
		m_flagSheet = sl_false;
		m_flagDialog = sl_false;
		m_flagBorderless = sl_false;
		m_flagShowTitleBar = sl_true;
		m_flagFullScreenOnCreate = 
#if defined(SLIB_PLATFORM_IS_MOBILE)
			sl_true;
#else
			sl_false;
#endif
		m_flagCenterScreenOnCreate = sl_false;
		m_flagWidthWrapping = sl_false;
		m_flagHeightWrapping = sl_false;
		m_flagCloseOnOK = sl_false;

		m_flagUseClientSizeRequested = sl_false;
		m_flagStateResizingWidth = sl_false;
		m_flagStateDoModal = sl_false;
		m_flagDispatchedDestroy = sl_false;
		
		m_result = sl_null;

#if defined(SLIB_UI_IS_ANDROID)
		m_activity = sl_null;
#endif
	}

	Window::~Window()
	{
		if (m_result) {
			delete m_result;
		}
	}
	
	void Window::init()
	{
		Object::init();
		
		m_viewContent = new priv::window::ContentView;
		m_viewContent->setWindow(this);
	}

	void Window::close()
	{
		Ref<Window> window = this;
		{
			ObjectLocker lock(this);
			Ref<WindowInstance> instance = m_instance;
			if (instance.isNotNull()) {
				instance->close();
				detach();
				lock.unlock();
				dispatchDestroy();
			}
			if (m_flagStateDoModal) {
				m_flagStateDoModal = sl_false;
				UI::quitLoop();
			}
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

	const Ref<View>& Window::getContentView()
	{
		return Ref<View>::from(m_viewContent);
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

	void Window::setFrame(const UIRect& _frame)
	{
		UIRect frame = _frame;
		_constrainSize(frame, frame.getWidth() > 0);
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
	
	sl_bool Window::isWidthWrapping()
	{
		return m_flagWidthWrapping;
	}
	
	void Window::setWidthWrapping(sl_bool flag)
	{
		m_flagWidthWrapping = flag;
		if (flag) {
			_applyContentWrappingSize();
		}
	}
	
	sl_bool Window::isHeightWrapping()
	{
		return m_flagHeightWrapping;
	}
	
	void Window::setHeightWrapping(sl_bool flag)
	{
		m_flagHeightWrapping = flag;
		if (flag) {
			_applyContentWrappingSize();
		}
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
			if (!(instance->setClientSize(size))) {
				setSize(size);
			}
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

	sl_ui_len Window::getClientWidth()
	{
		return getClientSize().x;
	}
	
	void Window::setClientWidth(sl_ui_len width)
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_flagUseClientSizeRequested = sl_false;
			if (!(instance->setClientSize(UISize(width, m_clientSizeRequested.y)))) {
				setWidth(width);
			}
		} else {
			m_flagUseClientSizeRequested = sl_true;
			m_clientSizeRequested.x = width;
			m_frame.setWidth(width);
		}
	}
	
	sl_ui_len Window::getClientHeight()
	{
		return getClientSize().y;
	}
	
	void Window::setClientHeight(sl_ui_len height)
	{
		Ref<WindowInstance> instance = m_instance;
		if (CHECK_INSTANCE(instance)) {
			m_flagUseClientSizeRequested = sl_false;
			if (!(instance->setClientSize(UISize(m_clientSizeRequested.x, height)))) {
				setHeight(height);
			}
		} else {
			m_flagUseClientSizeRequested = sl_true;
			m_clientSizeRequested.y = height;
			m_frame.setHeight(height);
		}
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
	
	void Window::setSizeRange(const UISize& _sizeMinimum, const UISize& _sizeMaximum, float aspectRatioMinimum, float aspectRatioMaximum)
	{
		UISize sizeMinimum = _sizeMinimum;
		if (sizeMinimum.x < 0) {
			sizeMinimum.x = 0;
		}
		if (sizeMinimum.y < 0) {
			sizeMinimum.y = 0;
		}
		m_sizeMin = sizeMinimum;
		UISize sizeMaximum = _sizeMaximum;
		if (sizeMaximum.x < 0) {
			sizeMaximum.x = 0;
		}
		if (sizeMaximum.y < 0) {
			sizeMaximum.y = 0;
		}
		m_sizeMax = sizeMaximum;
		if (aspectRatioMinimum < 0) {
			aspectRatioMinimum = 0;
		}
		m_aspectRatioMinimum = aspectRatioMinimum;
		if (aspectRatioMaximum < 0) {
			aspectRatioMaximum = 0;
		}
		m_aspectRatioMaximum = aspectRatioMaximum;
		
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			instance->setSizeRange(sizeMinimum, sizeMaximum, aspectRatioMinimum, m_aspectRatioMaximum);
		}
		
		UIRect frame = m_frame;
		UIRect frameOld = frame;
		_constrainSize(frame, frame.getWidth() > 0);
		if (!(frame.isAlmostEqual(frameOld))) {
			setFrame(frame);
		}
	}
	
	UISize Window::getMinimumSize()
	{
		return m_sizeMin;
	}
	
	void Window::setMinimumSize(const UISize& sizeMinimum)
	{
		setSizeRange(sizeMinimum, m_sizeMax, m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	void Window::setMinimumSize(sl_ui_len width, sl_ui_len height)
	{
		setSizeRange(UISize(width, height), m_sizeMax, m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	sl_ui_len Window::getMinimumWidth()
	{
		return m_sizeMin.x;
	}
	
	void Window::setMinimumWidth(sl_ui_len width)
	{
		setSizeRange(UISize(width, m_sizeMin.y), m_sizeMax, m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	sl_ui_len Window::getMinimumHeight()
	{
		return m_sizeMin.y;
	}
	
	void Window::setMinimumHeight(sl_ui_len height)
	{
		setSizeRange(UISize(m_sizeMin.x, height), m_sizeMax, m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	UISize Window::getMaximumSize()
	{
		return m_sizeMax;
	}
	
	void Window::setMaximumSize(const UISize& sizeMaximum)
	{
		setSizeRange(m_sizeMin, sizeMaximum, m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	void Window::setMaximumSize(sl_ui_len width, sl_ui_len height)
	{
		setSizeRange(m_sizeMin, UISize(width, height), m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	sl_ui_len Window::getMaximumWidth()
	{
		return m_sizeMax.x;
	}
	
	void Window::setMaximumWidth(sl_ui_len width)
	{
		setSizeRange(m_sizeMin, UISize(width, m_sizeMax.y), m_aspectRatioMinimum, m_aspectRatioMaximum);
	}
	
	sl_ui_len Window::getMaximumHeight()
	{
		return m_sizeMax.y;
	}
	
	void Window::setMaximumHeight(sl_ui_len height)
	{
		setSizeRange(m_sizeMin, UISize(m_sizeMax.x, height), m_aspectRatioMinimum, m_aspectRatioMaximum);
	}

	float Window::getMinimumAspectRatio()
	{
		return m_aspectRatioMinimum;
	}
	
	void Window::setMinimumAspectRatio(float ratio)
	{
		setSizeRange(m_sizeMin, m_sizeMax, ratio, m_aspectRatioMaximum);
	}
	
	float Window::getMaximumAspectRatio()
	{
		return m_aspectRatioMaximum;
	}
	
	void Window::setMaximumAspectRatio(float ratio)
	{
		setSizeRange(m_sizeMin, m_sizeMax, m_aspectRatioMinimum, ratio);
	}
	
	void Window::setAspectRatio(float ratio)
	{
		setSizeRange(m_sizeMin, m_sizeMax, ratio, ratio);
	}
	
	sl_bool Window::isModal()
	{
		return m_flagModal;
	}

	void Window::setModal(sl_bool flag)
	{
		m_flagModal = flag;
	}

	sl_bool Window::isSheet()
	{
		return m_flagSheet;
	}
	
	void Window::setSheet(sl_bool flag)
	{
		m_flagSheet = flag;
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
	
	sl_bool Window::isCloseOnOK()
	{
		return m_flagCloseOnOK;
	}
	
	void Window::setCloseOnOK(sl_bool flag)
	{
		m_flagCloseOnOK = flag;
	}
	
	Variant Window::getResult()
	{
		SpinLocker lock(&m_lockResult);
		if (m_result) {
			return *m_result;
		} else {
			return sl_null;
		}
	}
	
	void Window::setResult(const Variant& result)
	{
		SpinLocker lock(&m_lockResult);
		if (m_result) {
			*m_result = result;
		} else {
			m_result = new Variant(result);
		}
	}
	
	void Window::close(const Variant& result)
	{
		setResult(result);
		close();
	}

#if defined(SLIB_UI_IS_ANDROID)
	void* Window::getActivity()
	{
		return m_activity;
	}

	void Window::setActivity(void* activity)
	{
		m_activity = activity;
	}
#endif

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
			view->detachAll();
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
			_create(sl_false);
		} else {
			UI::dispatchToUiThread(SLIB_BIND_REF(void(), Window, _create, this, sl_false));
		}
	}

	void Window::createAndKeep()
	{
		if (UI::isUiThread()) {
			_create(sl_true);
		} else {
			UI::dispatchToUiThread(SLIB_BIND_REF(void(), Window, _create, this, sl_true));
		}
	}

	void Window::forceCreate()
	{
		detach();
		create();
	}

	void Window::forceCreateAndKeep()
	{
		detach();
		createAndKeep();
	}

	void Window::_create(sl_bool flagKeepReference)
	{
		if (m_instance.isNotNull()) {
			return;
		}
		
		WindowInstanceParam param;

		Ref<Window> parent = m_parent;
		if (parent.isNotNull()) {
			param.parent = parent->m_instance;
		}
		
		if (m_flagWidthWrapping || m_flagHeightWrapping) {
			UISize sizeMeasured = m_viewContent->measureLayoutWrappingSize(m_flagWidthWrapping, m_flagHeightWrapping);
			if (m_flagWidthWrapping) {
				if (sizeMeasured.x < 100) {
					sizeMeasured.x = 100;
				}
				m_frame.setWidth(sizeMeasured.x);
			}
			if (m_flagHeightWrapping) {
				if (sizeMeasured.y < 100) {
					sizeMeasured.y = 100;
				}
				m_frame.setHeight(sizeMeasured.y);
			}
		}
		
		param.screen = m_screen;
		param.menu = m_menu;
		param.flagBorderless = m_flagBorderless;
		param.flagFullScreen = m_flagFullScreenOnCreate;
		param.flagCenterScreen = m_flagCenterScreenOnCreate;
		param.flagDialog = m_flagDialog;
		param.flagModal = m_flagModal;
		param.flagSheet = m_flagSheet;
		param.location = m_frame.getLocation();
		param.size = m_frame.getSize();
		param.title = m_title;
		param.flagShowTitleBar = m_flagShowTitleBar;
#if defined(SLIB_UI_IS_ANDROID)
		param.activity = m_activity;
#endif
#if defined(SLIB_UI_IS_GTK)
		param.flagClientSize = m_flagUseClientSizeRequested;
		if (m_flagUseClientSizeRequested) {
			param.size = m_clientSizeRequested;
		}
#endif

		Ref<WindowInstance> window = createWindowInstance(param);
		
		if (window.isNotNull()) {
			
			if (flagKeepReference) {
				increaseReference();
				window->setKeepWindow(sl_true);
			}
			
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
			
			window->setSizeRange(m_sizeMin, m_sizeMax, m_aspectRatioMinimum, m_aspectRatioMaximum);
			
#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_WIN32)
			if (m_flagUseClientSizeRequested) {
				UISize size = window->getWindowSizeFromClientSize(m_clientSizeRequested);
				m_frame = window->getFrame();
				m_frame.setSize(size);
				window->setFrame(m_frame);
			}
#endif

			window->setVisible(sl_true);

			attach(window, sl_false);
			
#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_EFL)
			UISize sizeClient = getClientSize();
			dispatchResize(sizeClient.x, sizeClient.y);
#endif
#if defined(SLIB_UI_IS_WIN32)
			if (m_flagDialog) {
				UISize sizeClient = getClientSize();
				dispatchResize(sizeClient.x, sizeClient.y);
			}
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
					view->removeAllViewInstances();
					contentViewInstance->setEnabled(view->isEnabled());
					contentViewInstance->setOpaque(view->isOpaque());
					contentViewInstance->setDrawing(view->isDrawing());
					view->attach(contentViewInstance);
				}
			}
		}
	}
	
	Variant Window::doModal()
	{
		if (!(UI::isUiThread())) {
			return sl_null;
		}
		setModal(sl_true);
		forceCreate();
		Ref<WindowInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (instance->doModal()) {
				return getResult();
			}
			m_flagStateDoModal = sl_true;
			UI::runLoop();
			m_flagStateDoModal = sl_false;
			return getResult();
		}
		return sl_null;
	}

	void Window::showModal()
	{
		UI::dispatchToUiThread(SLIB_BIND_REF(void(), Window, doModal, this));
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

	SLIB_DEFINE_EVENT_HANDLER(Window, Create)
	
	void Window::dispatchCreate()
	{
		SLIB_INVOKE_EVENT_HANDLER(Create)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, CreateFailed)

	void Window::dispatchCreateFailed()
	{
		SLIB_INVOKE_EVENT_HANDLER(CreateFailed)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Close, UIEvent* ev)

	void Window::dispatchClose(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Close, ev)

		if (ev->isPreventedDefault()) {
			return;
		}
		detach();
		dispatchDestroy();
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Destroy)

	void Window::dispatchDestroy()
	{
		if (m_flagDispatchedDestroy) {
			return;
		}
		m_flagDispatchedDestroy = sl_true;
		
		SLIB_INVOKE_EVENT_HANDLER(Destroy)
		
		if (m_flagStateDoModal) {
			m_flagStateDoModal = sl_false;
			UI::quitLoop();
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(Window, Activate)

	void Window::dispatchActivate()
	{
		SLIB_INVOKE_EVENT_HANDLER(Activate)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Deactivate)

	void Window::dispatchDeactivate()
	{
		SLIB_INVOKE_EVENT_HANDLER(Deactivate)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Move)

	void Window::dispatchMove()
	{
		SLIB_INVOKE_EVENT_HANDLER(Move)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(Window, Resizing, UISize& size)

	void Window::dispatchResizing(UISize& size)
	{
		_constrainSize(size, m_flagStateResizingWidth);
		
		SLIB_INVOKE_EVENT_HANDLER(Resizing, size)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(Window, Resize, sl_ui_len clientWidth, sl_ui_len clientHeight)

	void Window::dispatchResize(sl_ui_len clientWidth, sl_ui_len clientHeight)
	{
		_refreshSize();
		if (clientWidth > 0 && clientHeight > 0) {
			Ref<View> viewContent = m_viewContent;
			if (viewContent.isNotNull()) {
				if (!(viewContent->isInstance())) {
					_attachContent();
				}
			}
		}
		
		SLIB_INVOKE_EVENT_HANDLER(Resize, clientWidth, clientHeight)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Minimize)

	void Window::dispatchMinimize()
	{
		SLIB_INVOKE_EVENT_HANDLER(Minimize)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Deminimize)

	void Window::dispatchDeminimize()
	{
		SLIB_INVOKE_EVENT_HANDLER(Deminimize)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Maximize)

	void Window::dispatchMaximize()
	{
		_refreshSize();
		
		SLIB_INVOKE_EVENT_HANDLER(Maximize)
	}
	
	SLIB_DEFINE_EVENT_HANDLER(Window, Demaximize)

	void Window::dispatchDemaximize()
	{
		_refreshSize();
		
		SLIB_INVOKE_EVENT_HANDLER(Demaximize)
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, OK, UIEvent* ev)

	void Window::dispatchOK(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(OK, ev)
		if (m_flagCloseOnOK) {
			close(DialogResult::Ok);
		}
	}

	void Window::dispatchOK()
	{
		Ref<UIEvent> ev = UIEvent::create(UIAction::Unknown);
		if (ev.isNotNull()) {
			dispatchOK(ev.get());
		}
	}

	SLIB_DEFINE_EVENT_HANDLER(Window, Cancel, UIEvent* ev)

	void Window::dispatchCancel(UIEvent* ev)
	{
		SLIB_INVOKE_EVENT_HANDLER(Cancel, ev)
		if (ev->isPreventedDefault()) {
			return;
		}
		SLIB_INVOKE_EVENT_HANDLER(Close, ev)
		if (ev->isPreventedDefault()) {
			return;
		}
		close(DialogResult::Cancel);
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

	void Window::_constrainSize(UISize& size, sl_bool flagAdjustHeight)
	{
		sl_ui_len minX = m_sizeMin.x;
		sl_ui_len minY = m_sizeMin.y;
		sl_ui_len maxX = m_sizeMax.x;
		if (maxX <= 0) {
			maxX = 1000000;
		}
		sl_ui_len maxY = m_sizeMax.y;
		if (maxY <= 0) {
			maxY = 1000000;
		}

		size.x = Math::clamp(size.x, minX, maxX);
		size.y = Math::clamp(size.y, minY, maxY);

		float minAspect = m_aspectRatioMinimum;
		float maxAspect = m_aspectRatioMaximum;
		if (minAspect > 0 || maxAspect > 0) {
			if (minAspect > 0) {
				if (flagAdjustHeight) {
					sl_ui_len ay = (sl_ui_len)(Math::min(size.x / minAspect, 1000000.0f));
					if (size.y > ay) {
						if (ay > minY) {
							size.y = ay;
						} else {
							size.y = minY;
							size.x = (sl_ui_len)(Math::min(minY * minAspect, 1000000.0f));
						}
					}
				} else {
					sl_ui_len ax = (sl_ui_len)(Math::min(size.y * minAspect, 1000000.0f));
					if (size.x < ax) {
						if (ax < maxX) {
							size.x = ax;
						} else {
							size.x = maxX;
							size.y = (sl_ui_len)(Math::min(maxX / minAspect, 1000000.0f));
						}
					}
				}
			}
			if (maxAspect > 0) {
				if (flagAdjustHeight) {
					sl_ui_len ay = (sl_ui_len)(Math::min(size.x / maxAspect, 1000000.0f));
					if (size.y < ay) {
						if (ay < maxY) {
							size.y = ay;
						} else {
							size.y = maxY;
							size.x = (sl_ui_len)(Math::min(maxY * maxAspect, 1000000.0f));
						}
					}
				} else {
					sl_ui_len ax = (sl_ui_len)(size.y * maxAspect);
					if (size.x > ax) {
						if (ax > minX) {
							size.x = ax;
						} else {
							size.x = minX;
							size.y = (sl_ui_len)(Math::min(minX / maxAspect, 1000000.0f));
						}
					}
				}
			}
		}
	}
	
	void Window::_constrainSize(UIRect& frame, sl_bool flagAdjustHeight)
	{
		UISize size = frame.getSize();
		_constrainSize(size, flagAdjustHeight);
		frame.setSize(size);
	}
	
	void Window::_applyContentWrappingSize()
	{
		m_viewContent->applyWrappingContentSize();
	}
	

	SLIB_DEFINE_OBJECT(WindowInstance, Object)

	WindowInstance::WindowInstance(): m_flagKeepWindow(sl_false)
	{
	}

	WindowInstance::~WindowInstance()
	{
		if (m_flagKeepWindow) {
			Ref<Window> window = m_window;
			if (window.isNotNull()) {
				window->decreaseReference();
			}
		}
	}

	Ref<Window> WindowInstance::getWindow()
	{
		return m_window;
	}

	void WindowInstance::setWindow(const Ref<Window>& window)
	{
		m_window = window;
	}
	
	void WindowInstance::setKeepWindow(sl_bool flag)
	{
		m_flagKeepWindow = flag;
	}

	void WindowInstance::setMenu(const Ref<Menu>& menu)
	{
	}
	
	void WindowInstance::setSizeRange(const UISize& sizeMinimum, const UISize& sizeMaximum, float aspectRatioMinimum, float aspectRatioMaximum)
	{
	}

	sl_bool WindowInstance::doModal()
	{
		return sl_false;
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

	void WindowInstance::onResizing(UISize& size, sl_bool flagResizingWidth)
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->m_flagStateResizingWidth = flagResizingWidth;
			window->dispatchResizing(size);
		}
	}

	void WindowInstance::onResize(sl_ui_len clientWidth, sl_ui_len clientHeight)
	{
		Ref<Window> window = getWindow();
		if (window.isNotNull()) {
			window->dispatchResize(clientWidth, clientHeight);
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
