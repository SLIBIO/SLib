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

#ifndef CHECKHEADER_SLIB_UI_COLLECTION_VIEW
#define CHECKHEADER_SLIB_UI_COLLECTION_VIEW

#include "definition.h"

#include "scroll_view.h"
#include "adapter.h"

namespace slib
{
	
	namespace priv
	{
		namespace collection_view
		{
			class ContentView;
			class Column;
			class FreeViewSet;
		}
	}
	
	class SLIB_EXPORT CollectionView : public VerticalScrollView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CollectionView();
		
		~CollectionView();

	protected:
		void init() override;
		
	public:		
		sl_uint32 getColumnsCount();
		
		Ref<ViewAdapter> getAdapter();
		
		Ref<ViewAdapter> getAdapter(sl_uint32 columnIndex);
		
		void setAdapters(const List< Ref<ViewAdapter> >& adapters, const List<sl_uint32>& listWidth = List<sl_uint32>::null());
		
		void setAdapter(const Ref<ViewAdapter>& adapter);
		
		void setAdapter(const Ref<ViewAdapter>& adapter, sl_uint32 nColumns);
		
		void refreshItems();
		
		sl_ui_len getColumnHeight(sl_uint32 index);
		
		sl_bool isCuttingOverflowItems();
		
		void setCuttingOverflowItems(sl_bool flag);
		
	protected:
		void onScroll(sl_scroll_pos x, sl_scroll_pos y) override;
		
		void onResize(sl_ui_len x, sl_ui_len y) override;
		
		void onAttach() override;
		
	protected:
		Ref<View> _getView(ViewAdapter* adapter, sl_uint64 index, View* original);
		
		void _requestLayout();
		
		void _doLayout();
		
		void _layout();
		
		void _layoutColumn(priv::collection_view::Column* column, sl_bool flagRefresh, sl_ui_len x, sl_ui_len width);
		
		void _updateItemLayout(const Ref<View>& itemView, sl_ui_pos x, sl_ui_len widthList, sl_ui_len heightList);
		
	protected:
		AtomicList<priv::collection_view::Column> m_columns;
		List<priv::collection_view::Column> m_columnsCurrent;
		sl_bool m_flagCuttingOverflowItems;
		
		Ref<priv::collection_view::ContentView> m_contentView;
		sl_bool m_flagRefreshItems;
		
		Mutex m_lockLayout;
		sl_int32 m_idLayoutRequest;
		sl_int32 m_idLayoutComplete;
		
		sl_ui_pos m_lastScrollY;

		friend class priv::collection_view::ContentView;
		
	};

}

#endif
