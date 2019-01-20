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

#include "slib/ui/view_pager.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPager, View)

	ViewPager::ViewPager()
	{
		m_indexCurrent = 0;
	}
	
	ViewPager::~ViewPager()
	{
	}
	
	void ViewPager::setAdapter(const Ref<ViewAdapter>& adapter, UIUpdateMode mode)
	{
		m_adapter = adapter;
	}
	
	void ViewPager::addPage(const Ref<View>& view, UIUpdateMode mode)
	{
		Ref<ViewAdapter> adapter = m_adapter;
		Ref<ViewListAdapter> listAdapter;
		if (adapter.isNotNull()) {
			listAdapter = CastInstance<ViewListAdapter>(adapter.get());
		} else {
			listAdapter = new ViewListAdapter;
		}
		if (listAdapter.isNull()) {
			return;
		}
		listAdapter->addView(view);
		setAdapter(listAdapter, mode);
	}
	
	sl_uint64 ViewPager::getCurrentIndex()
	{
		return m_indexCurrent;
	}
	
	void ViewPager::selectPage(sl_uint64 index, UIUpdateMode mode)
	{
		m_indexCurrent = index;
	}
	
	void ViewPager::onResize(sl_ui_len width, sl_ui_len height)
	{
	}
	
	void ViewPager::onChangePadding()
	{
	}
	
}
