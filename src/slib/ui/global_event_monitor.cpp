/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/ui/global_event_monitor.h"

#include "slib/ui/core.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_WIN32)
	
	namespace priv
	{
		namespace global_event_monitor
		{
			
			class GlobalEventMonitorHelper : public GlobalEventMonitor
			{
				friend class StaticContext;
			};
			
			class StaticContext
			{
			public:
				Function<void(UIEvent*)> m_callbackMouse;
				Function<void(UIEvent*)> m_callbackKeyboard;
				sl_uint32 m_maskCurrent;
				
			public:
				StaticContext()
				{
					m_maskCurrent = 0;
				}
			
			public:
				void add(const Function<void(UIEvent*)>& callback)
				{
					m_callbackMouse.add(callback);
					m_callbackKeyboard.add(callback);
					updateMonitor();
				}
				
				void remove(const Function<void(UIEvent*)>& callback)
				{
					m_callbackMouse.remove(callback);
					m_callbackKeyboard.remove(callback);
					updateMonitor();
				}
				
				void addMouse(const Function<void(UIEvent*)>& callback)
				{
					m_callbackMouse.add(callback);
					updateMonitor();
				}
				
				void removeMouse(const Function<void(UIEvent*)>& callback)
				{
					m_callbackMouse.remove(callback);
					updateMonitor();
				}
				
				void addKeyboard(const Function<void(UIEvent*)>& callback)
				{
					m_callbackKeyboard.add(callback);
					updateMonitor();
				}
				
				void removeKeyboard(const Function<void(UIEvent*)>& callback)
				{
					m_callbackKeyboard.remove(callback);
					updateMonitor();
				}
				
				void updateMonitor()
				{
					sl_uint32 mask = 0;
					if (m_callbackMouse.isNotNull()) {
						mask |= GlobalEventMonitorHelper::MASK_MOUSE;
					}
					if (m_callbackKeyboard.isNotNull()) {
						mask |= GlobalEventMonitorHelper::MASK_KEYBOARD;
					}
					if (mask != m_maskCurrent) {
						if (GlobalEventMonitorHelper::_updateMonitor(mask)) {
							m_maskCurrent = mask;
						} else {
							m_maskCurrent = 0;
						}
					}
				}
				
				void processEvent(UIEvent* ev)
				{
					if (ev->isMouseEvent()) {
						m_callbackMouse(ev);
					} else if (ev->isKeyEvent()) {
						m_callbackKeyboard(ev);
					}
				}

			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
			
		}
	}
	
	using namespace priv::global_event_monitor;
	
	void GlobalEventMonitor::addMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&addMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->add(callback);
		}
	}
	
	void GlobalEventMonitor::removeMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&removeMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->remove(callback);
		}
	}
	
	void GlobalEventMonitor::addMouseMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&addMouseMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->addMouse(callback);
		}
	}
	
	void GlobalEventMonitor::removeMouseMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&removeMouseMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->removeMouse(callback);
		}
	}
	
	void GlobalEventMonitor::addKeyboardMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&addKeyboardMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->addKeyboard(callback);
		}
	}
	
	void GlobalEventMonitor::removeKeyboardMonitor(const Function<void(UIEvent*)>& callback)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(Function<void()>::bind(&removeKeyboardMonitor, callback));
			return;
		}
		StaticContext* context = GetStaticContext();
		if (context) {
			context->removeKeyboard(callback);
		}
	}
	
	void GlobalEventMonitor::_onEvent(UIEvent* ev)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->processEvent(ev);
		}
	}
	
#else
	void GlobalEventMonitor::addMonitor(const Function<void(UIEvent*)>& callback)
	{
	}
	
	void GlobalEventMonitor::removeMonitor(const Function<void(UIEvent*)>& callback)
	{
	}
	
	void GlobalEventMonitor::addMouseMonitor(const Function<void(UIEvent*)>& callback)
	{
	}
	
	void GlobalEventMonitor::removeMouseMonitor(const Function<void(UIEvent*)>& callback)
	{
	}
	
	void GlobalEventMonitor::addKeyboardMonitor(const Function<void(UIEvent*)>& callback)
	{
	}
	
	void GlobalEventMonitor::removeKeyboardMonitor(const Function<void(UIEvent*)>& callback)
	{
	}

	sl_bool GlobalEventMonitor::isKeyboardAccessEnabled()
	{
		return sl_false;
	}
	
	void GlobalEventMonitor::authenticateKeyboardAccess()
	{
	}
#endif
	
}
