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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/system_tray_icon.h"
#include "slib/ui/platform.h"

namespace slib
{
	namespace priv
	{
		namespace system_tray_icon
		{
			class SystemTrayIconImpl;
		}
	}
}

@interface SLIBSystemTrayIconListener : NSObject
{
	@public slib::WeakRef<slib::priv::system_tray_icon::SystemTrayIconImpl> m_object;
}

- (void)onAction;

@end

namespace slib
{
	
	namespace priv
	{
		namespace system_tray_icon
		{
			
			class SystemTrayIconImpl : public SystemTrayIcon
			{
			public:
				NSStatusItem* m_item;
				SLIBSystemTrayIconListener* m_listener;
				
			public:
				SystemTrayIconImpl()
				{
					m_item = nil;
				}
				
				~SystemTrayIconImpl()
				{
					if (m_item != nil) {
						NSStatusBar* bar = [NSStatusBar systemStatusBar];
						if (bar != nil) {
							[bar removeStatusItem:m_item];
						}
					}
				}
				
			public:
				static Ref<SystemTrayIconImpl> create(const SystemTrayIconParam& param)
				{
					NSStatusBar* bar = [NSStatusBar systemStatusBar];
					if (bar != nil) {
						NSStatusItem* item = [bar statusItemWithLength:NSSquareStatusItemLength];
						if (item != nil) {
							Ref<SystemTrayIconImpl> ret = new SystemTrayIconImpl;
							if (ret.isNotNull()) {
								ret->m_item = item;
								ret->_init(param);
								return ret;
							}
						}
					}
					return sl_null;
				}

			public:
				void _init(const SystemTrayIconParam& param)
				{
					SystemTrayIcon::_init(param);
					if (param.icon.isNotNull() || param.iconName.isNotEmpty()) {
						setIcon_NI(param.icon, param.iconName);
					}
					if (param.toolTip.isNotEmpty()) {
						setToolTip_NI(param.toolTip);
					}
					setVisible_NI(param.flagVisible);
					if (param.menu.isNotNull()) {
						setMenu_NI(param.menu);
					}
					if (param.flagHighlight) {
						m_item.highlightMode = YES;
					}
					
					if (@available(macos 10.10, *)) {
						NSStatusBarButton* button = m_item.button;
						SLIBSystemTrayIconListener* listener = [SLIBSystemTrayIconListener new];
						listener->m_object = this;
						button.target = listener;
						button.action = @selector(onAction);
						m_listener = listener;
					} else {
						SLIBSystemTrayIconListener* listener = [SLIBSystemTrayIconListener new];
						listener->m_object = this;
						m_item.target = listener;
						m_item.action = @selector(onAction);
					}
				}
				
				void setIcon_NI(const Ref<Bitmap>& icon, const String& name) override
				{
					NSImage* image;
					if (name.isNotEmpty()) {
						image = [NSImage imageNamed:Apple::getNSStringFromString(name)];
					} else {
						image = GraphicsPlatform::createNSImageFromBitmap(icon);
						if (image != nil) {
							CGFloat height = [[NSStatusBar systemStatusBar] thickness];
							if (image.size.width > height + 0.5 && image.size.height > height + 0.5) {
								[image setSize:NSMakeSize(height, height)];
							}
						}
					}
					if (@available(macos 10.10, *)) {
						m_item.button.image = image;
						m_item.button.alternateImage = image;
					} else {
						m_item.image = image;
						m_item.alternateImage = image;
					}
				}
				
				void setToolTip_NI(const String& toolTip) override
				{
					NSString* str = Apple::getNSStringFromString(toolTip);
					if (@available(macos 10.10, *)) {
						m_item.button.toolTip = str;
					} else {
						m_item.toolTip = str;
					}
				}
				
				void setVisible_NI(sl_bool flag) override
				{
					if (@available(macos 10.12, *)) {
						m_item.visible = flag;
					}
				}
				
				void setMenu_NI(const Ref<Menu>& menu) override
				{
					m_item.menu = UIPlatform::getMenuHandle(menu);
				}
				
				void showMessage_NI(const String& title, const String& message, const Ref<Bitmap>& icon, sl_uint32 milliseconsTimeout) override
				{
				}
				
				UIRect getFrame() override
				{
					if (@available(macos 10.10, *)) {
						NSStatusBarButton* button = m_item.button;
						NSRect bounds = button.bounds;
						NSWindow* window = [button window];
						if (window != nil) {
							NSScreen* screen = [window screen];
							NSPoint pw = [button convertPoint:NSMakePoint(0, 0) toView:nil];
							NSRect rect;
							rect.origin.x = pw.x;
							rect.origin.y = pw.y;
							rect.size.width = 0;
							rect.size.height = 0;
							rect = [window convertRectToScreen:rect];
							UIRect ret;
							ret.left = (sl_ui_pos)(rect.origin.x);
							ret.top = (sl_ui_pos)([screen frame].size.height - 1 - rect.origin.y);
							ret.right = ret.left + (sl_ui_pos)(bounds.size.width);
							ret.bottom = ret.top + (sl_ui_pos)(bounds.size.height);
							return ret;
						}
					}
					return UIRect::zero();
				}
				
				void _onMouseEvent(UIAction action, NSEvent* event)
				{
					if (@available(macos 10.10, *)) {
						NSStatusBarButton* handle = m_item.button;
						NSWindow* window = [handle window];
						if (window != nil) {
							NSPoint pw = [event locationInWindow];
							NSPoint pt = [handle convertPoint:pw fromView:nil];
							sl_ui_posf x = (sl_ui_posf)(pt.x);
							sl_ui_posf y = (sl_ui_posf)(pt.y);
							Time t;
							t.setSecondsCountf([event timestamp]);
							Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
							if (ev.isNotNull()) {
								UIPlatform::applyEventModifiers(ev.get(), event);
								dispatchMouseEvent(ev.get());
							}
						}
					}
				}
				
			};
			
		}
	}
	
	using namespace priv::system_tray_icon;

	Ref<SystemTrayIcon> SystemTrayIcon::create(const SystemTrayIconParam& param)
	{
		return Ref<SystemTrayIcon>::from(SystemTrayIconImpl::create(param));
	}

}

using namespace slib;
using namespace slib::priv::system_tray_icon;

@implementation SLIBSystemTrayIconListener

- (void)onAction
{
	Ref<SystemTrayIconImpl> object(m_object);
	if (object.isNotNull()) {
		object->dispatchAction();
	}
}

@end

#endif
