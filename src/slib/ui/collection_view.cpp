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
						if (view->m_lockCountLayouting == 0) {
							dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, view.get(), sl_null, sl_false));
						}
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
				CList<sl_ui_len> heights;
				HashMap< sl_uint32, Ref<View> > viewsVisible;
				CList<FreeViewSet> viewsFreed;
				
			public:
				Column(const Ref<ViewAdapter>& _adapter, sl_ui_len _width): adapter(_adapter), width(_width)
				{
					width = 0;
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
				
			};
			
		}
	}
	
	using namespace priv::collection_view;

	SLIB_DEFINE_OBJECT(CollectionView, VerticalScrollView)
	
	CollectionView::CollectionView()
	{
		m_flagRefreshItems = sl_true;
		m_lockCountLayouting = 0;
		m_lastScrollY = 0;
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
	
	Ref<ViewAdapter> CollectionView::getAdapter()
	{
		List<Column> columns = m_columns;
		if (columns.isNotEmpty()) {
			return columns.getPointerAt(0)->adapter;
		}
		return sl_null;
	}
	
	sl_uint32 CollectionView::getColumnsCount()
	{
		return (sl_uint32)(m_columns.getCount());
	}
	
	Ref<ViewAdapter> CollectionView::getAdapterForColumn(sl_uint32 index)
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
		List<Column> columns;
		for (sl_size i = 0; i < adapters.count; i++) {
			if (listWidth.count) {
				columns.add_NoLock(adapters[i], listWidth[i]);
			} else {
				columns.add_NoLock(adapters[i], 0);
			}
		}
		runOnDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, this, columns, sl_false));
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
		runOnDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, this, sl_null, sl_false));
	}
	
	void CollectionView::onScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		if (Math::isAlmostZero(y - (sl_scroll_pos)m_lastScrollY)) {
			return;
		}
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, this, sl_null, sl_true));
	}
	
	void CollectionView::onResize(sl_ui_len x, sl_ui_len y)
	{
		runOnDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, this, sl_null, sl_false));
	}
	
	void CollectionView::onAttach()
	{
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), CollectionView, _layout, this, sl_null, sl_false));
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
	
	sl_ui_len CollectionView::_layoutColumn(Column* column, sl_bool flagRefresh, sl_ui_len x, sl_ui_len width)
	{
		View* contentView = m_contentView.get();
		sl_ui_pos heightCollectionView = getHeight();
		
		sl_ui_len heightMinimum = heightCollectionView / 100;
		if (heightMinimum < 1) {
			heightMinimum = 1;
		}
		
		ViewAdapter* adapter = column->adapter.get();
		if (!adapter) {
			return 0;
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
			return 0;
		}
		
		sl_ui_len heightAverage = adapter->getAverageItemHeight(this);
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
			if (y < scrollY + heightCollectionView && y + h > scrollY) {
				
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
		return y;
	}

	void CollectionView::_layout(const List<Column>& columnsNew, sl_bool fromScroll)
	{
		if (!(isDrawingThread())) {
			return;
		}

		MutexLocker lock(&m_lockLayout);
		
		Column* columns;
		sl_uint32 nColumns;
		if (columnsNew.isNotNull()) {
			m_columns = columnsNew;
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
		
		Base::interlockedIncrement32(&m_lockCountLayouting);
		
		do {
			sl_ui_pos scrollY = (sl_ui_pos)(getScrollY());
			if (fromScroll) {
				if (Math::isAlmostZero(scrollY - m_lastScrollY)) {
					break;
				}
			}

			if (!nColumns) {
				setContentHeight(0, UIUpdateMode::Redraw);
				break;
			}
			
			sl_ui_len heightTotal = 0;
			sl_ui_pos x = 0;
			for (sl_uint32 i = 0; i < nColumns; i++) {
				sl_ui_len width = columns[i].width;
				if (!width) {
					width = widthCollectionView / nColumns;
					x = (i * widthCollectionView) / nColumns;
				}
				sl_ui_len height = _layoutColumn(columns + i, flagRefresh, x, width);
				if (height > heightTotal) {
					heightTotal = height;
				}
				x += width;
			}
			
			setContentSize(x, heightTotal, UIUpdateMode::Redraw);
			
			m_lastScrollY = scrollY;
			
		} while (0);
		
		Base::interlockedDecrement32(&m_lockCountLayouting);
		
	}
	
	void CollectionView::_updateItemLayout(const Ref<View>& itemView, sl_ui_pos x, sl_ui_len widthColumn, sl_ui_len heightColumn)
	{
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
	}
	
}
