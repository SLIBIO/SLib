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

#ifndef CHECKHEADER_SLIB_UI_ADAPTER
#define CHECKHEADER_SLIB_UI_ADAPTER

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT ViewAdapter : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewAdapter();
		
		~ViewAdapter();
		
	public:
		virtual sl_uint64 getItemsCount() = 0;
		
		virtual Ref<View> getView(sl_uint64 index, View* original, View* parent) = 0;
		
		virtual sl_ui_len getAverageItemHeight(View* parent);
		
		void populateInto(View* parent, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void populateInto(const Ref<View>& parent, UIUpdateMode mode = UIUpdateMode::UpdateLayout);

	};
	
	template <class ViewType>
	class SLIB_EXPORT ViewAdapterT : public ViewAdapter
	{
	public:
		virtual void onBindView(sl_uint64 index, ViewType* view, View* parent) = 0;
		
	public:
		Ref<View> getView(sl_uint64 index, View* original, View* parent) override
		{
			Ref<ViewType> view;
			if (original) {
				view = (ViewType*)original;
			} else {
				view = new ViewType;
			}
			onBindView(index, view.get(), parent);
			return view;
		}
		
	};
	
	template <class DataType, class ViewType>
	class SLIB_EXPORT ListViewAdapter : public ViewAdapterT<ViewType>
	{
	public:
		ListViewAdapter() {}
		
		ListViewAdapter(const List<DataType>& list): m_list(list) {}
		
		ListViewAdapter(const List<DataType>& list, const Function<void(DataType& data, ViewType* view, View* parent)>& onBindView): m_list(list), m_onBindView(onBindView) {}

	public:
		static Ref<ListViewAdapter> create(const List<DataType>& list, const Function<void(DataType& data, ViewType* view, View* parent)>& onBindView)
		{
			return new ListViewAdapter(list, onBindView);
		}
		
		List<DataType> getList()
		{
			return m_list;
		}
		
		void setList(const List<DataType>& list)
		{
			m_list = list;
		}
		
		Function<void(DataType& data, ViewType* view, View* parent)> getOnBindView()
		{
			return m_onBindView;
		}
		
		void setOnBindView(const Function<void(DataType& data, ViewType* view, View* parent)>& onBindView)
		{
			m_onBindView = onBindView;
		}
		
		sl_uint64 getItemsCount() override
		{
			return m_list.getCount();
		}
		
	public:
		virtual void onBindView(DataType& data, ViewType* view, View* parent)
		{
			m_onBindView(data, view, parent);
		}

		void onBindView(sl_uint64 index, ViewType* view, View* parent) override
		{
			ListLocker<DataType> list(m_list);
			if (index < list.count) {
				onBindView(list[index], view, parent);
			}
		}
		
	protected:
		AtomicList<DataType> m_list;
		AtomicFunction<void(DataType& data, ViewType* view, View* parent)> m_onBindView;
		
	};
	
	class ViewListAdapter : public ViewAdapter
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewListAdapter();
		
		~ViewListAdapter();
		
		ViewListAdapter(const List< Ref<View> >& list);
		
	public:
		static Ref<ViewListAdapter> create(const List< Ref<View> >& list);
		
		List< Ref<View> > getList();
		
		void setList(const List< Ref<View> >& list);
		
		void addView(const Ref<View>& view);
		
	public:
		sl_uint64 getItemsCount() override;
		
		Ref<View> getView(sl_uint64 index, View* original, View* parent) override;
		
	protected:
		AtomicList< Ref<View> > m_list;

	};
	
	
	class ViewRowAdapter : public ViewAdapter
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ViewRowAdapter();
		
		~ViewRowAdapter();
		
	public:
		static Ref<ViewRowAdapter> create(sl_uint32 nColumns, const Ref<ViewAdapter>& itemAdapter);
		
	public:
		sl_uint32 getColumnsCount();
		
		void setColumnsCount(sl_uint32 nColumns);
		
		Ref<ViewAdapter> getItemAdapter();
		
		void setItemAdapter(const Ref<ViewAdapter>& adapter);
		
	public:
		sl_uint64 getItemsCount() override;
		
		Ref<View> getView(sl_uint64 index, View* original, View* parent) override;
		
		sl_ui_len getAverageItemHeight(View* parent) override;
		
	protected:
		sl_uint32 m_nColumns;
		AtomicRef<ViewAdapter> m_itemAdapter;
		
	};
	
}

#endif
