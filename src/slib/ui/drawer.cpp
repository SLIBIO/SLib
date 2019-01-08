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

#include "slib/ui/drawer.h"

#include "slib/device/device.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(Drawer, ViewGroup)
	
	Drawer::Drawer()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setSavingCanvasState(sl_false);
		
		setWidthFilling(1, UIUpdateMode::Init);
		setHeightFilling(1, UIUpdateMode::Init);
		
		m_viewHover = new View;
		m_viewHover->setBackgroundColor(Color(0, 0, 0, 30), UIUpdateMode::Init);
		m_viewHover->setWidthFilling(1, UIUpdateMode::Init);
		m_viewHover->setHeightFilling(1, UIUpdateMode::Init);
		m_viewHover->setVisible(sl_false, UIUpdateMode::Init);
		
		m_callbackParentMouseEvent = SLIB_FUNCTION_WEAKREF(Drawer, _onParentMouseEvent, this);
	}
	
	Drawer::~Drawer()
	{
	}
	
	Ref<View> Drawer::getHover()
	{
		return m_viewHover;
	}
	
	void Drawer::onChangeParent(View* oldParent, View* newParent)
	{
		m_viewHover->removeFromParent();
		if (oldParent) {
			oldParent->removeOnMouseEvent(m_callbackParentMouseEvent);
		}
		if (newParent) {
			newParent->setCapturingChildInstanceEvents(sl_true);
			newParent->addOnMouseEvent(m_callbackParentMouseEvent);
			dispatchToUiThread(SLIB_BIND_WEAKREF(void(), View, addChild, this, m_viewHover, UIUpdateMode::UpdateLayout));
		}
	}
	
	void Drawer::_onParentMouseEvent(View* view, UIEvent* ev)
	{
		
	}
	
}
