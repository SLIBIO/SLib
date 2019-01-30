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

#include "slib/ui/linear_view.h"

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
		UIUpdateMode _mode = SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None;
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
	
	void ViewListAdapter::addView(const Ref<View>& view)
	{
		m_list.add(view);
	}
	
	sl_uint64 ViewListAdapter::getItemsCount()
	{
		return m_list.getCount();
	}
	
	Ref<View> ViewListAdapter::getView(sl_uint64 index, View* original, View* parent)
	{
		return m_list.getValueAt((sl_size)index);
	}
	
	
	SLIB_DEFINE_OBJECT(ViewRowAdapter, ViewAdapter)
	
	ViewRowAdapter::ViewRowAdapter()
	 : m_nColumns(1)
	{
	}
	
	ViewRowAdapter::~ViewRowAdapter()
	{
	}
	
	Ref<ViewRowAdapter> ViewRowAdapter::create(sl_uint32 nColumns, const Ref<ViewAdapter>& itemAdapter)
	{
		if (itemAdapter.isNull()) {
			return sl_null;
		}
		if (nColumns < 1) {
			return sl_null;
		}
		Ref<ViewRowAdapter> ret = new ViewRowAdapter;
		if (ret.isNotNull()) {
			ret->m_nColumns = nColumns;
			ret->m_itemAdapter = itemAdapter;
			return ret;
		}
		return sl_null;
	}
	
	sl_uint32 ViewRowAdapter::getColumnsCount()
	{
		return m_nColumns;
	}
	
	void ViewRowAdapter::setColumnsCount(sl_uint32 nColumns)
	{
		m_nColumns = nColumns;
	}
	
	Ref<ViewAdapter> ViewRowAdapter::getItemAdapter()
	{
		return m_itemAdapter;
	}
	
	void ViewRowAdapter::setItemAdapter(const Ref<ViewAdapter>& adapter)
	{
		m_itemAdapter = adapter;
	}
	
	sl_uint64 ViewRowAdapter::getItemsCount()
	{
		Ref<ViewAdapter> adapter = m_itemAdapter;
		if (adapter.isNotNull()) {
			sl_uint32 m = m_nColumns;
			if (m >= 1) {
				sl_uint64 n = adapter->getItemsCount();
				sl_uint64 r = n / m;
				if (n % m) {
					r++;
				}
				return r;
			}
		}
		return 0;
	}
	
	Ref<View> ViewRowAdapter::getView(sl_uint64 index, View* original, View* parent)
	{
		Ref<ViewAdapter> adapter = m_itemAdapter;
		if (adapter.isNotNull()) {
			sl_uint32 m = m_nColumns;
			if (m >= 1) {
				sl_uint64 nTotal = adapter->getItemsCount();
				sl_uint64 start = index * m;
				sl_uint32 n = m;
				if (start + n > nTotal) {
					n = (sl_uint32)(nTotal - start);
				}
				Ref<LinearView> group = (LinearView*)original;
				sl_bool flagNewGroup = sl_false;
				if (group.isNull()) {
					group = new LinearView;
					if (group.isNull()) {
						return sl_null;
					}
					group->setOrientation(LayoutOrientation::Horizontal, UIUpdateMode::Init);
					group->setWidthFilling(1.0f, UIUpdateMode::Init);
					group->setHeightWrapping(UIUpdateMode::Init);
					flagNewGroup = sl_true;
				}
				sl_bool flagRebuild = sl_false;
				Queue< Ref<View> > children;
				for (sl_uint32 i = 0; i < n; i++) {
					Ref<View> child = group->getChild(i);
					Ref<View> childNew = adapter->getView(start + i, child.get(), group.get());
					if (child != childNew) {
						flagRebuild = sl_true;
					}
					if (childNew.isNotNull()) {
						children.push_NoLock(childNew);
					}
				}
				sl_size nChildren = group->getChildrenCount();
				if (flagRebuild || nChildren != children.getCount()) {
					if (nChildren) {
						group->removeAllChildren(UIUpdateMode::None);
					}
					for (auto& child : children) {
						group->addChild(child, flagNewGroup ? UIUpdateMode::Init : UIUpdateMode::None);
					}
					if (!flagNewGroup) {
						group->invalidateLayout();
					}
				}
				return group;
			}
		}
		return sl_null;
	}
	
	sl_ui_len ViewRowAdapter::getAverageItemHeight(View* parent)
	{
		Ref<ViewAdapter> adapter = m_itemAdapter;
		if (adapter.isNotNull()) {
			return adapter->getAverageItemHeight(parent);
		}
		return ViewAdapter::getAverageItemHeight(parent);
	}
	
}
