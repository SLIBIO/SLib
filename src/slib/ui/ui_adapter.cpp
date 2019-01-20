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

#include "slib/ui/adapter.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(ViewAdapter, Object)
	
	ViewAdapter::ViewAdapter()
	{
	}
	
	ViewAdapter::~ViewAdapter()
	{
	}
	
	sl_ui_len ViewAdapter::getAverageItemHeight(View* parent)
	{
		return 0;
	}
	
	void ViewAdapter::populateInto(View* parent, UIUpdateMode mode)
	{
		if (!parent) {
			return;
		}
		UIUpdateMode _mode = mode == UIUpdateMode::Init ? UIUpdateMode::Init : UIUpdateMode::None;
		sl_uint64 n = getItemsCount();
		for (sl_uint64 i = 0; i < n; i++) {
			Ref<View> view = getView(i, sl_null, parent);
			if (view.isNotNull()) {
				parent->addChild(view, _mode);
			}
		}
		parent->invalidateLayout(mode);
	}
	
	void ViewAdapter::populateInto(const Ref<View>& parent, UIUpdateMode mode)
	{
		populateInto(parent.get(), mode);
	}

	
	SLIB_DEFINE_OBJECT(ViewListAdapter, ViewAdapter)
	
	ViewListAdapter::ViewListAdapter()
	{
	}
	
	ViewListAdapter::~ViewListAdapter()
	{
	}
	
	ViewListAdapter::ViewListAdapter(const List< Ref<View> >& list): m_list(list)
	{
	}

	Ref<ViewListAdapter> ViewListAdapter::create(const List< Ref<View> >& list)
	{
		return new ViewListAdapter(list);
	}
	
	List< Ref<View> > ViewListAdapter::getList()
	{
		return m_list;
	}
	
	void ViewListAdapter::setList(const List< Ref<View> >& list)
	{
		m_list = list;
	}
	
	sl_uint64 ViewListAdapter::getItemsCount()
	{
		return m_list.getCount();
	}
	
	Ref<View> ViewListAdapter::getView(sl_uint64 index, View* original, View* parent)
	{
		return m_list.getValueAt((sl_size)index);
	}
	
}
