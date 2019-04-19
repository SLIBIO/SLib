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

#include "slib/ui/refresh_view.h"

#include "slib/core/safe_static.h"

#if defined(SLIB_UI_IS_ANDROID)
#	define HAS_NATIVE_WIDGET_IMPL
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(RefreshView, ViewGroup)
	
	RefreshView::RefreshView()
	{
#ifdef HAS_NATIVE_WIDGET_IMPL
		setCreatingNativeWidget(sl_true);
#endif
		m_flagRefreshing = sl_false;
	}
	
	RefreshView::~RefreshView()
	{
	}
	
	sl_bool RefreshView::isRefreshing()
	{
		return m_flagRefreshing;
	}
	
	void RefreshView::setRefreshing(sl_bool flag)
	{
		m_flagRefreshing = flag;
		_setRefreshing_NW(flag);
	}
	
	SLIB_DEFINE_EVENT_HANDLER(RefreshView, Refresh)

	void RefreshView::dispatchRefresh()
	{
		SLIB_INVOKE_EVENT_HANDLER(Refresh)
	}
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
	Ref<ViewInstance> RefreshView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
#endif
	
#if !defined(SLIB_UI_IS_IOS)
	void RefreshView::onAttachChild(View* child)
	{
	}
#endif

#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void RefreshView::_setRefreshing_NW(sl_bool flag)
	{
	}
#endif

	void RefreshView::_onRefresh_NW()
	{
		m_flagRefreshing = sl_true;
		dispatchRefresh();
	}
	
}
