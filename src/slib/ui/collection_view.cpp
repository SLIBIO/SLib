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

#include "slib/ui/collection_view.h"

#include "slib/ui/core.h"
#include "slib/core/scoped.h"

#define MAX_ITEMS_PER_PAGE 500
#define MAX_FREE_VIEWS 50

namespace slib
{
	
	namespace priv
	{
		namespace collection_view
		{
			
			class ContentView : public ViewGroup
			{
			private:
				WeakRef<CollectionView> m_collectionView;
				
			public:
				ContentView()
				{
					setCreatingEmptyContent(sl_true);
					setSavingCanvasState(sl_false);
					setUsingChildLayouts(sl_false);
				}
				
			public:
				Ref<CollectionView> getCollectionView()
				{
					return m_collectionView;
				}
				
				void setCollectionView(CollectionView* view)
				{
					m_collectionView = view;
				}
				
			protected:
				void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override
				{
					Ref<CollectionView> view = m_collectionView;
					if (view.isNotNull()) {
						view->_requestLayout();
					}
				}
				
			};
			
			class FreeViewSet
			{
			public:
				sl_object_type type;
				CLinkedList< Ref<View> > queue;
			};
			
			class Column
			{
			public:
				Ref<ViewAdapter> adapter;
				sl_ui_len width;
				sl_scroll_pos contentHeight;
				CList<sl_ui_len> heights;
				HashMap< sl_uint32, Ref<View> > viewsVisible;
				CList<FreeViewSet> viewsFreed;
				
			public:
				Column(const Ref<ViewAdapter>& _adapter, sl_ui_len _width): adapter(_adapter), width(_width)
				{
					width = 0;
					contentHeight = 0;
				}
				
			};
			
			class ColumnAdapter : public ViewAdapter
			{
			public:
				Ref<ViewAdapter> adapterTotal;
				sl_uint32 indexColumn;
				sl_uint32 nColumns;
				
			public:
				sl_uint64 getItemsCount() override
				{
					sl_uint64 m = adapterTotal->getItemsCount() ;
					sl_uint64 n = m / nColumns;
					if (indexColumn < (m % nColumns)) {
						n++;
					}
					return n;
				}
				
				Ref<View> getView(sl_uint64 index, View* original, View* parent) override
				{
					return adapterTotal->getView(index * nColumns + indexColumn, original, parent);
				}
				
				sl_object_type getViewType(sl_uint64 index, View* parent) override
				{
					return adapterTotal->getViewType(index * nColumns + indexColumn, parent);
				}
				
				sl_ui_len getAverageItemHeight(View* parent) override
				{
					return adapterTotal->getAverageItemHeight(parent);
				}
				
				sl_ui_len getItemHeight(sl_uint64 index, View* parent) override
				{
					return adapterTotal->getItemHeight(index * nColumns + indexColumn, parent);
				}
				
				sl_uint32 getMaximumItemsCountPerPage(View* parent) override
				{
					return adapterTotal->getMaximumItemsCountPerPage(parent);
				}
				
			};
			
		}
	}
	
	using namespace priv::collection_view;

	SLIB_DEFINE_OBJECT(CollectionView, VerticalScrollView)
	
	CollectionView::CollectionView()
	{
		m_flagCuttingOverflowItems = sl_false;
		
		m_flagRefreshItems = sl_true;
		m_lastScrollY = 0;

		m_idLayoutRequest = 0;
		m_idLayoutComplete = -1;
	}

	CollectionView::~CollectionView()
	{
	}
	
	void CollectionView::init()
	{
		VerticalScrollView::init();

		m_contentView = new ContentView;
		m_contentView->setCollectionView(this);
		setContentView(m_contentView, UIUpdateMode::Init);
	}
	
	sl_uint32 CollectionView::getColumnsCount()
	{
		return (sl_uint32)(m_columns.getCount());
	}
	
	Ref<ViewAdapter> CollectionView::getAdapter()
	{
		List<Column> columns = m_columns;
		if (columns.isNotEmpty()) {
			return columns.getPointerAt(0)->adapter;
		}
		return sl_null;
	}
	
	Ref<ViewAdapter> CollectionView::getAdapter(sl_uint32 index)
	{
		List<Column> columns = m_columns;
		if (columns.isNotEmpty()) {
			Column* column = columns.getPointerAt(index);
			if (column) {
				return column->adapter;
			}
		}
		return sl_null;
	}
	
	void CollectionView::setAdapters(const List< Ref<ViewAdapter> >& _adapters, const List<sl_uint32>& _listWidth)
	{
		ListElements< Ref<ViewAdapter> > adapters(_adapters);
		ListElements<sl_uint32> listWidth(_listWidth);
		if (listWidth.count && adapters.count != listWidth.count) {
			return;
		}
		scrollTo(0, 0);
		List<Column> columns;
		for (sl_size i = 0; i < adapters.count; i++) {
			if (listWidth.count) {
				columns.add_NoLock(adapters[i], listWidth[i]);
			} else {
				columns.add_NoLock(adapters[i], 0);
			}
		}
		m_columns = columns;
		if (isDrawingThread()) {
			Base::interlockedIncrement32(&m_idLayoutRequest);
			_doLayout();
		} else {
			_requestLayout();
		}
	}
	
	void CollectionView::setAdapter(const Ref<ViewAdapter>& adapter)
	{
		if (adapter.isNull()) {
			setAdapters(sl_null, sl_null);
			return;
		}
		setAdapters(List< Ref<ViewAdapter> >::createFromElement(adapter), sl_null);
	}
	
	void CollectionView::setAdapter(const Ref<ViewAdapter>& adapterTotal, sl_uint32 nColumns)
	{
		if (adapterTotal.isNull() || !nColumns) {
			setAdapters(sl_null, sl_null);
			return;
		}
		List< Ref<ViewAdapter> > adapters;
		for (sl_uint32 i = 0; i < nColumns; i++) {
			Ref<ColumnAdapter> adapter = new ColumnAdapter;
			if (adapter.isNull()) {
				return;
			}
			adapter->adapterTotal = adapterTotal;
			adapter->nColumns = nColumns;
			adapter->indexColumn = i;
			adapters.add_NoLock(Move(adapter));
		}
		setAdapters(adapters);
	}

	void CollectionView::refreshItems()
	{
		m_flagRefreshItems = sl_true;
		_requestLayout();
	}

	sl_ui_len CollectionView::getColumnHeight(sl_uint32 index)
	{
		List<Column> columns = m_columns;
		if (columns.isNotEmpty()) {
			Column* column = columns.getPointerAt(index);
			if (column) {
				return (sl_ui_len)(column->contentHeight);
			}
		}
		return 0;
	}

	sl_bool CollectionView::isCuttingOverflowItems()
	{
		return m_flagCuttingOverflowItems;
	}

	void CollectionView::setCuttingOverflowItems(sl_bool flag)
	{
		m_flagCuttingOverflowItems = flag;
	}
	
	void CollectionView::onScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		if (Math::isAlmostZero(y - (sl_scroll_pos)m_lastScrollY)) {
			return;
		}
		_requestLayout();
	}
	
	void CollectionView::onResize(sl_ui_len x, sl_ui_len y)
	{
		_requestLayout();
	}
	
	void CollectionView::onAttach()
	{
	}
	
	Ref<View> CollectionView::_getView(ViewAdapter* adapter, sl_uint64 index, View* original)
	{
		Ref<View> view = adapter->getView(index, original, this);
		if (view.isNotNull()) {
#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_IOS) || defined(SLIB_UI_IS_ANDROID)
			if (isCreatingInstance()) {
				view->setCreatingInstance(sl_true);
			}
#endif
			View::LayoutAttributes* attrs = view->m_layoutAttrs.get();
			if (attrs) {
				attrs->topMode = PositionMode::Free;
				attrs->bottomMode = PositionMode::Free;
			}
			return view;
		}
		return sl_null;
	}

	void CollectionView::_requestLayout()
	{
		Base::interlockedIncrement32(&m_idLayoutRequest);
		dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(CollectionView, _doLayout, this));
	}

	void CollectionView::_doLayout()
	{
		MutexLocker lock(&m_lockLayout);
		while (m_idLayoutComplete != m_idLayoutRequest) {
			m_idLayoutComplete = m_idLayoutRequest;
			_layout();
		}
	}

	void CollectionView::_layout()
	{
		List<Column> columnsNew = m_columns;
		Column* columns;
		sl_uint32 nColumns;
		
		if (columnsNew != m_columnsCurrent) {
			m_columnsCurrent = columnsNew;
			columns = columnsNew.getData();
			nColumns = (sl_uint32)(columnsNew.getCount());
			m_contentView->removeAllChildren();
		} else {
			columns = m_columnsCurrent.getData();
			nColumns = (sl_uint32)(m_columnsCurrent.getCount());
		}
		
		sl_ui_pos widthCollectionView = getBounds().getWidth();
		sl_ui_pos heightCollectionView = getHeight();
		
		if (widthCollectionView <= 0 || heightCollectionView <= 0) {
			return;
		}
		
		sl_bool flagRefresh = sl_false;
		if (m_flagRefreshItems) {
			flagRefresh = sl_true;
			m_flagRefreshItems = sl_false;
		}
				
		if (!nColumns) {
			setContentHeight(0, UIUpdateMode::Redraw);
			return;
		}
		
		sl_ui_pos scrollY = (sl_ui_pos)(getScrollY());

		sl_ui_len maxHeight = 0;
		sl_ui_len minHeight = 0;
		sl_ui_pos x = 0;
		
		{
			for (sl_uint32 i = 0; i < nColumns; i++) {
				sl_ui_len width = columns[i].width;
				if (!width) {
					width = widthCollectionView / nColumns;
					x = (i * widthCollectionView) / nColumns;
				}
				_layoutColumn(columns + i, flagRefresh, x, width);
				sl_ui_len height = (sl_ui_len)(columns[i].contentHeight);
				if (height > maxHeight) {
					maxHeight = height;
				}
				if (i == 0 || height < minHeight) {
					minHeight = height;
				}
				x += width;
			}
		}
		
		if (m_flagCuttingOverflowItems) {
			View* contentView = m_contentView.get();
			for (sl_uint32 i = 0; i < nColumns; i++) {
				Column& col = columns[i];
				if (col.contentHeight > minHeight) {
					HashMapNode< sl_uint32, Ref<View> >* node = col.viewsVisible.getFirstNode();
					while (node) {
						HashMapNode< sl_uint32, Ref<View> >* next = node->next;
						View* view = node->value.get();
						if (view->getTop() + view->getLayoutHeight() > minHeight) {
							col.viewsVisible.removeAt(node);
							contentView->removeChild(view, UIUpdateMode::None);
						}
						node = next;
					}
				}
			}
			setContentSize(x, minHeight, UIUpdateMode::Redraw);
		} else {
			setContentSize(x, maxHeight, UIUpdateMode::Redraw);
		}
		
		m_lastScrollY = scrollY;
	
	}

	void CollectionView::_layoutColumn(Column* column, sl_bool flagRefresh, sl_ui_len x, sl_ui_len width)
	{
		View* contentView = m_contentView.get();
		sl_ui_pos heightCollectionView = getHeight();
		
		sl_ui_len heightMinimum = heightCollectionView / 100;
		if (heightMinimum < 1) {
			heightMinimum = 1;
		}
		
		ViewAdapter* adapter = column->adapter.get();
		if (!adapter) {
			column->contentHeight = 0;
			return;
		}
		
		sl_uint32 nTotalCount = (sl_uint32)(adapter->getItemsCount());
		sl_uint32 nOrigTotalCount = (sl_uint32)(column->heights.getCount());
		if (nTotalCount != nOrigTotalCount) {
			column->heights.setCount_NoLock(nTotalCount);
			if (nOrigTotalCount < nTotalCount) {
				sl_ui_len* heights = column->heights.getData();
				for (sl_uint32 i = nOrigTotalCount; i < nTotalCount; i++) {
					heights[i] = 0;
				}
			}
		}
		sl_ui_len* heights = column->heights.getData();
		if (!heights) {
			column->contentHeight = 0;
			return;
		}
		
		sl_ui_len heightAverage = adapter->getAverageItemHeight(this);
		sl_uint32 maxItemsCountPerPage = adapter->getMaximumItemsCountPerPage(this);
		if (maxItemsCountPerPage > MAX_ITEMS_PER_PAGE) {
			maxItemsCountPerPage = MAX_ITEMS_PER_PAGE;
		}
		
		{
			sl_uint64 heightTotal = 0;
			sl_uint32 countNotZero = 0;
			for (sl_uint32 i = 0; i < nTotalCount; i++) {
				sl_ui_len h = heights[i];
				if (h > 0) {
					heightTotal += h;
					countNotZero++;
				} else {
					h = adapter->getItemHeight(i, this);
					heights[i] = h;
					if (h > 0) {
						heightTotal += h;
						countNotZero++;
					}
				}
			}
			if (!heightAverage) {
				if (heightTotal > 0) {
					heightAverage = (sl_ui_len)(heightTotal / countNotZero);
				}
			}
		}
		
		sl_ui_pos scrollY = (sl_ui_pos)(getScrollY());
		sl_ui_len y = 0;
		HashMap< sl_uint32, Ref<View> > viewsVisible = Move(column->viewsVisible);
		
		for (sl_uint32 indexItem = 0; indexItem < nTotalCount; indexItem++) {
			sl_ui_len h = heights[indexItem];
			if (!h) {
				h = heightAverage;
			}
			if (h < heightMinimum) {
				h = heightMinimum;
			}
			if (y < scrollY + heightCollectionView && y + h > scrollY && column->viewsVisible.getCount() < maxItemsCountPerPage) {
				
				h = adapter->getItemHeight(indexItem, this);

				Ref<View> view;
				viewsVisible.remove_NoLock(indexItem, &view);
				if (view.isNull()) {
					sl_object_type type = adapter->getViewType(indexItem, this);
					ListElements<FreeViewSet> freeViewSet(column->viewsFreed);
					for (sl_size i = 0; i < freeViewSet.count; i++) {
						FreeViewSet& set = freeViewSet[i];
						if (type) {
							if (set.type == type) {
								set.queue.popFront(&view);
								break;
							}
						} else {
							set.queue.popFront(&view);
							if (view.isNotNull()) {
								break;
							}
						}
					}
					view = _getView(adapter, indexItem, view.get());
					if (view.isNotNull()) {
						_updateItemLayout(view, x, width, heightCollectionView);
					}
				} else {
					if (flagRefresh) {
						Ref<View> viewNew = _getView(adapter, indexItem, view.get());
						if (view != viewNew) {
							viewsVisible.add_NoLock(indexItem, Move(view));
							if (viewNew.isNotNull()) {
								view = Move(viewNew);
								_updateItemLayout(view, x, width, heightCollectionView);
							}
						}
					}
				}
				if (view.isNotNull()) {
					if (!h) {
						h = view->getLayoutHeight() + view->getMarginTop() + view->getMarginBottom();
					}
					heights[indexItem] = h;
					column->viewsVisible.add_NoLock(indexItem, view);
				} else {
					if (!h) {
						h = heightAverage;
					}
				}				
				if (h < heightMinimum) {
					h = heightMinimum;
				}
				
				UIRect rect = view->getLayoutFrame();
				rect.top = y + view->getMarginTop();
				y += h;
				rect.bottom = y - view->getMarginBottom();
				view->setLayoutFrame(rect);
				view->_applyLayout(UIUpdateMode::None);
				if (view->getParent() != contentView) {
					contentView->addChild(view, UIUpdateMode::None);
				}
			} else {
				y += h;
			}
		}
		// free unsed visible views
		{
			HashMapNode< sl_uint32, Ref<View> >* node = viewsVisible.getFirstNode();
			while (node) {
				Ref<View>& view = node->value;
				contentView->removeChild(view, UIUpdateMode::None);
				sl_object_type type = view->getObjectType();
				sl_bool flagFoundSet = sl_false;
				ListElements<FreeViewSet> freeViewSet(column->viewsFreed);
				for (sl_size i = 0; i < freeViewSet.count; i++) {
					FreeViewSet& set = freeViewSet[i];
					if (set.type == type) {
						flagFoundSet = sl_true;
						if (set.queue.getCount() < MAX_FREE_VIEWS) {
							set.queue.pushBack(view);
						}
						break;
					}
				}
				if (!flagFoundSet) {
					FreeViewSet set;
					set.type = type;
					set.queue.pushBack(view);
					column->viewsFreed.add_NoLock(Move(set));
				}
				node = node->next;
			}
		}
		
		column->contentHeight = y;

	}

	void CollectionView::_updateItemLayout(const Ref<View>& itemView, sl_ui_pos x, sl_ui_len widthColumn, sl_ui_len heightColumn)
	{
		itemView->setPressedState(sl_false, UIUpdateMode::None);
		itemView->setHoverState(sl_false, UIUpdateMode::None);

		Ref<LayoutAttributes>& layoutAttrs = itemView->m_layoutAttrs;
		if (layoutAttrs.isNotNull()) {
			layoutAttrs->flagInvalidLayoutInParent = sl_true;
			SizeMode mode = itemView->getHeightMode();
			if (mode == SizeMode::Filling) {
				layoutAttrs->layoutFrame.setHeight((sl_ui_len)((sl_real)heightColumn * itemView->getHeightWeight()));
			}
		}
		UpdateLayoutFrameParam param;
		param.parentContentFrame.left = x;
		param.parentContentFrame.right = x + widthColumn;
		param.parentContentFrame.top = 0;
		param.parentContentFrame.bottom = heightColumn;
		param.flagUseLayout = sl_true;
		param.flagHorizontal = sl_true;
		param.flagVertical = sl_false;
		itemView->updateLayoutFrameInParent(param);
		sl_ui_len height = itemView->getLayoutHeight() + itemView->getMarginTop() + itemView->getMarginBottom();
		sl_ui_len minItemHeight = (heightColumn / MAX_ITEMS_PER_PAGE) + 1;
		if (height < minItemHeight) {
			itemView->setLayoutHeight(minItemHeight);
		}
	}
	
}
