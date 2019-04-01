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

#include "slib/ui/list_view.h"

#include "slib/ui/core.h"

#define MAX_ITEMS_VISIBLE 500
#define MAX_ITEMS_SAVE_HEIGHTS 10000
#define MAX_MID_HEIGHT 1000000

namespace slib
{
	class _priv_ListContentView : public ViewGroup
	{
	public:
		WeakRef<ListView> m_lv;
		
	public:
		_priv_ListContentView()
		{			
			setSavingCanvasState(sl_false);
			setUsingChildLayouts(sl_false);
		}
		
	public:
		void dispatchDraw(Canvas* canvas) override;
		
		void onResizeChild(View* child, sl_ui_len width, sl_ui_len height) override;
		
	};
	
	SLIB_DEFINE_OBJECT(ListView, VerticalScrollView)
	
	ListView::ListView()
	{
		m_lockCountLayouting = 0;
		
		m_flagResetAdapter = sl_true;
		m_flagRefreshItems = sl_true;
		
		m_viewsVisibleItems = (Ref<View>*)(Base::createZeroMemory(sizeof(Ref<View>)*MAX_ITEMS_VISIBLE*4));
		m_viewsGoDownItems = m_viewsVisibleItems + MAX_ITEMS_VISIBLE;
		m_viewsGoUpItems = m_viewsGoDownItems + MAX_ITEMS_VISIBLE;
		m_viewsFreeItems = m_viewsGoUpItems + MAX_ITEMS_VISIBLE;
		
		m_heightsVisibleItems = (sl_ui_len*)(Base::createMemory(sizeof(sl_ui_len)*(MAX_ITEMS_VISIBLE*3+MAX_ITEMS_SAVE_HEIGHTS*2)));
		m_heightsTopItems = m_heightsVisibleItems + MAX_ITEMS_VISIBLE;
		m_heightsBottomItems = m_heightsTopItems + MAX_ITEMS_SAVE_HEIGHTS;
		m_heightsGoDownItems = m_heightsBottomItems + MAX_ITEMS_SAVE_HEIGHTS;
		m_heightsGoUpItems = m_heightsGoDownItems + MAX_ITEMS_VISIBLE;
		
		_initStatus();
	}

	ListView::~ListView()
	{
		for (sl_size i = 0; i < MAX_ITEMS_VISIBLE*3; i++) {
			if (m_viewsVisibleItems[i].isNotNull()) {
				m_viewsVisibleItems[i].setNull();
			}
		}
		Base::freeMemory(m_viewsVisibleItems);
		Base::freeMemory(m_heightsVisibleItems);
	}
	
	void ListView::init()
	{
		VerticalScrollView::init();

		m_contentView = new _priv_ListContentView;
		m_contentView->m_lv = this;
		ScrollView::setContentView(m_contentView);		
	}
	
	Ref<ViewAdapter> ListView::getAdapter()
	{
		return m_adapter;
	}
	
	void ListView::setAdapter(const Ref<ViewAdapter>& adapter)
	{
		m_adapter = adapter;
		m_flagResetAdapter = sl_true;
		runOnDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _checkUpdateContent, this, sl_false));
	}
	
	void ListView::refreshItems()
	{
		m_flagRefreshItems = sl_true;
		runOnDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _checkUpdateContent, this, sl_false));
	}
	
	List< Ref<View> > ListView::getVisibleItemViews()
	{
		List< Ref<View> > ret;
		MutexLocker lock(&m_lockVisibleItems);
		for (sl_size i = 0; i < m_countVisibleItems; i++) {
			if (m_viewsVisibleItems[i].isNotNull()) {
				ret.add_NoLock(m_viewsVisibleItems[i]);
			}
		}
		return ret;
	}
	
	void ListView::onScroll(sl_scroll_pos _x, sl_scroll_pos _y)
	{
		sl_ui_pos y = (sl_ui_pos)(_y);
		if (Math::isAlmostZero(y - m_lastScrollY)) {
			return;
		}
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _layoutItemViews, this, sl_false, sl_true, sl_false));
	}
	
	void ListView::onResize(sl_ui_len x, sl_ui_len y)
	{
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _layoutItemViews, this, sl_false, sl_false, sl_true));
	}
	
	void ListView::_checkUpdateContent(sl_bool fromDraw)
	{
		if (!(isDrawingThread())) {
			return;
		}
		if (m_flagResetAdapter) {
			m_flagResetAdapter = sl_false;
			m_flagRefreshItems = sl_false;
			_resetAdapter();
			_layoutItemViews(fromDraw, sl_false, sl_true);
			return;
		}
		if (m_flagRefreshItems) {
			m_flagRefreshItems = sl_false;
			_layoutItemViews(fromDraw, sl_false, sl_true);
			return;
		}
	}
	
	void ListView::_initStatus()
	{
		m_countTotalItems = 0;
		m_indexFirstItem = 0;
		m_yFirstItem = 0;
		m_countVisibleItems = 0;
		m_averageItemHeight = 0;
		m_averageMidItemHeight = 0;
		m_heightTotalItems = 0;
		m_lastScrollY = 0;
		if (m_adapter.isNotNull()) {
			Ref<ViewAdapter> adapter = m_adapter;
			if (adapter.isNotNull()) {
				View* parent = this;
				sl_uint64 nTotal = adapter->getItemsCount();
				sl_uint32 i;
				if (nTotal <= MAX_ITEMS_SAVE_HEIGHTS) {
					for (i = 0; i < nTotal; i++) {
						sl_ui_len height = adapter->getItemHeight(i, parent);
						m_heightsTopItems[i] = height;
						sl_uint64 k = nTotal - 1 - i;
						m_heightsBottomItems[(sl_uint32)k] = height;
					}
					for (i = (sl_uint32)nTotal; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
						m_heightsTopItems[i] = 0;
						m_heightsBottomItems[i] = 0;
					}
				} else if (nTotal < 2 * MAX_ITEMS_SAVE_HEIGHTS) {
					for (i = 0; i < nTotal; i++) {
						sl_ui_len height = adapter->getItemHeight(i, parent);
						if (i < MAX_ITEMS_SAVE_HEIGHTS) {
							m_heightsTopItems[i] = height;
						}
						sl_uint64 k = nTotal - 1 - i;
						if (k < MAX_ITEMS_SAVE_HEIGHTS) {
							m_heightsBottomItems[(sl_uint32)k] = height;
						}
					}
				} else {
					for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
						m_heightsTopItems[i] = adapter->getItemHeight(i, parent);
						sl_uint64 k = nTotal - 1 - i;
						m_heightsBottomItems[i] = adapter->getItemHeight(k, parent);
					}
				}
				m_countTotalItems = nTotal;
			}
		} else {
			for (sl_uint32 i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
				m_heightsTopItems[i] = 0;
				m_heightsBottomItems[i] = 0;
			}
		}
	}
	
	void ListView::_resetAdapter()
	{
		_initStatus();
		m_contentView->removeAllChildren();
		{
			MutexLocker lock(&m_lockVisibleItems);
			for (sl_size i = 0; i < m_countVisibleItems; i++) {
				m_viewsVisibleItems[i].setNull();
			}
			m_countVisibleItems = 0;
		}
	}
	
	static sl_ui_len _priv_ListView_getTotalHeights(sl_uint64 count, sl_ui_len averageHeight, sl_ui_len* topHeights, sl_ui_len* bottomHeights, double& averageMidHeight)
	{
		averageMidHeight = (double)(averageHeight);
		sl_uint32 i;
		sl_ui_len s = 0;
		for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
			if (i >= count) {
				return s;
			}
			sl_ui_len h = topHeights[i];
			if (h <= 0) {
				h = averageHeight;
			}
			s += h;
		}
		count -= MAX_ITEMS_SAVE_HEIGHTS;
		if (count == 0) {
			return s;
		}
		for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
			if (i >= count) {
				return s;
			}
			sl_ui_len h = bottomHeights[i];
			if (h <= 0) {
				h = averageHeight;
			}
			s += h;
		}
		count -= MAX_ITEMS_SAVE_HEIGHTS;
		if (count == 0 || averageHeight == 0) {
			return s;
		}
		double fCount = (double)count;
		double mid = fCount * (double)(averageHeight);
		sl_ui_len _mid;
		if (mid > MAX_MID_HEIGHT) {
			_mid = MAX_MID_HEIGHT;
			averageMidHeight = MAX_MID_HEIGHT / fCount;
		} else {
			_mid = (sl_ui_len)(count) * averageHeight;
		}
		s += _mid;
		return s;
	}
	
	static sl_ui_len _priv_ListView_getAverageHeight(sl_uint64 count, sl_ui_len* topHeights, sl_ui_len* bottomHeights)
	{
		sl_uint32 i;
		sl_ui_len s = 0;
		sl_uint32 n = 0;
		for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
			if (i >= count) {
				break;
			}
			sl_ui_len h = topHeights[i];
			if (h > 0) {
				s += h;
				n++;
			}
		}
		if (count > MAX_ITEMS_SAVE_HEIGHTS) {
			count -= MAX_ITEMS_SAVE_HEIGHTS;
			if (count > 0) {
				for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
					if (i >= count) {
						break;
					}
					sl_ui_len h = bottomHeights[i];
					if (h > 0) {
						s += h;
						n++;
					}
				}
			}
		}
		if (n == 0) {
			return 0;
		}
		return s / n;
	}
	
	static sl_ui_pos _priv_ListView_getYPositionOfItem(sl_uint64 index, sl_uint64 count, sl_ui_len averageHeight, double averageMidHeight, sl_ui_len totalHeight, sl_ui_len* topHeights, sl_ui_len* bottomHeights)
	{
		if (index > count) {
			index = count;
		}
		if (index <= MAX_ITEMS_SAVE_HEIGHTS) {
			sl_ui_len y = 0;
			sl_uint32 n = (sl_uint32)index;
			for (sl_uint32 i = 0; i < n; i++) {
				sl_ui_len h = topHeights[i];
				if (h <= 0) {
					h = averageHeight;
				}
				y += h;
			}
			return y;
		}
		if (count - index <= MAX_ITEMS_SAVE_HEIGHTS) {
			sl_ui_len y = 0;
			sl_uint32 n = (sl_uint32)(count - index);
			for (sl_uint32 i = 0; i < n; i++) {
				sl_ui_len h = bottomHeights[i];
				if (h <= 0) {
					h = averageHeight;
				}
				y += h;
			}
			if (totalHeight > y) {
				y = totalHeight - y;
			} else {
				y = 0;
			}
			return y;
		}
		sl_ui_len y = 0;
		for (sl_uint32 i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
			sl_ui_len h = topHeights[i];
			if (h <= 0) {
				h = averageHeight;
			}
			y += h;
		}
		return y + (sl_ui_len)((double)(index - MAX_ITEMS_SAVE_HEIGHTS) * averageMidHeight);
	}
	
	template <class T>
	static void _priv_ListView_pushArrayLeft(T* arr, sl_uint64 sizeArr, sl_uint32 capacityArr, T value, sl_uint64 _count)
	{
		if (_count >= capacityArr) {
			for (sl_uint32 i = 0; i < capacityArr; i++) {
				arr[i] = value;
			}
		} else {
			sl_uint32 count = (sl_uint32)_count;
			sl_uint64 _n = sizeArr + count;
			if (_n > capacityArr) {
				_n = capacityArr;
			}
			sl_uint32 n = (sl_uint32)_n;
			n -= count;
			sl_uint32 p = n - 1;
			sl_uint32 i;
			for (i = 0; i < n; i++) {
				arr[p+count] = arr[p];
				p--;
			}
			for (i = 0; i < count; i++) {
				arr[i] = value;
			}
		}
	}
	
	template <class T>
	static void _priv_ListView_pushArrayRight(T* arr, sl_uint64 sizeArr, sl_uint32 capacityArr, T value, sl_uint64 _count)
	{
		if (_count >= capacityArr) {
			for (sl_uint32 i = 0; i < capacityArr; i++) {
				arr[i] = value;
			}
		} else {
			sl_uint32 count = (sl_uint32)_count;
			sl_uint32 i;
			sl_uint32 n;
			if (sizeArr > capacityArr) {
				n = capacityArr;
			} else {
				n = (sl_uint32)sizeArr;
			}
			if (n > count) {
				n -= count;
				for (i = 0; i < n; i++) {
					arr[i] = arr[i + count];
				}
				n += count;
				for (; i < n; i++) {
					arr[i] = value;
				}
			} else {
				for (i = 0; i < n; i++) {
					arr[i] = value;
				}
			}
		}
	}
	
	Ref<View> ListView::_getView(ViewAdapter* adapter, sl_uint64 index, View* original)
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
	
	void ListView::_layoutItemViews(sl_bool fromDraw, sl_bool fromScroll, sl_bool flagRefresh)
	{
		if (!(isDrawingThread())) {
			return;
		}
		
		if (m_lockCountLayouting != 0) {
			return;
		}
		
		sl_ui_pos widthListView = getWidth();
		sl_ui_pos heightListView = getHeight();
		
		if (widthListView <= 0 || heightListView <= 0) {
			return;
		}
		
		Ref<ViewAdapter> adapter(m_adapter);
		
		if (adapter.isNotNull()) {
			
			Base::interlockedIncrement32(&m_lockCountLayouting);
			
			do {
				
				View* contentView = m_contentView.get();
				
				MutexLocker lock(&m_lockVisibleItems);
				Ref<View>* viewsVisibleItems = m_viewsVisibleItems;
				sl_ui_len* heightsVisibleItems = m_heightsVisibleItems;
				sl_ui_len* heightsTopItems = m_heightsTopItems;
				sl_ui_len* heightsBottomItems = m_heightsBottomItems;
				Ref<View>* viewsGoDownItems = m_viewsGoDownItems;
				sl_ui_len* heightsGoDownItems = m_heightsGoDownItems;
				Ref<View>* viewsGoUpItems = m_viewsGoUpItems;
				sl_ui_len* heightsGoUpItems = m_heightsGoUpItems;
				Ref<View>* viewsFreeItems = m_viewsFreeItems;
				
				sl_uint64 lastCountTotalItems = m_countTotalItems;
				sl_ui_len lastHeightTotalItems = m_heightTotalItems;
				sl_uint64 lastIndexFirstItem = m_indexFirstItem;
				sl_ui_len lastAverageItemHeight = m_averageItemHeight;
				double lastAverageMidItemHeight = m_averageMidItemHeight;
				sl_uint32 lastCountVisibleItems = m_countVisibleItems;
				
				sl_uint32 countFreeViews = 0;
				sl_uint32 countGoDownViews = 0;
				sl_uint32 countGoUpViews = 0;
				
				sl_uint64 countTotalItems = lastCountTotalItems;
				sl_ui_len heightTotalItems = lastHeightTotalItems;
				sl_bool flagClearAll = sl_false;
				
				sl_ui_pos yGoDown = 0;
				sl_uint64 indexGoDown = 0;
				sl_ui_pos yGoUp = 0;
				sl_uint64 indexGoUp = 0;
				
				sl_ui_pos scrollY = (sl_ui_pos)(getScrollY());
				if (fromScroll) {
					if (Math::isAlmostZero(scrollY - m_lastScrollY)) {
						break;
					}
				}

				sl_ui_pos windowStart = scrollY - heightListView / 2;
				if (windowStart < 0) {
					windowStart = 0;
				}
				sl_ui_pos windowEnd = scrollY + heightListView + heightListView / 2;

				if (flagRefresh) {
					
					countTotalItems = adapter->getItemsCount();
					
					flagClearAll = sl_true;
					
					// shift bottom heights when count is changed
					if (lastCountTotalItems != countTotalItems) {
						if (countTotalItems > lastCountTotalItems) {
							sl_uint64 off = countTotalItems - lastCountTotalItems;
							_priv_ListView_pushArrayLeft<sl_ui_len>(heightsBottomItems, lastCountTotalItems, MAX_ITEMS_VISIBLE, 0, off);
						} else {
							sl_uint64 off = lastCountTotalItems - countTotalItems;
							_priv_ListView_pushArrayRight<sl_ui_len>(heightsBottomItems, lastCountTotalItems, MAX_ITEMS_VISIBLE, 0, off);
						}
					}
					
					heightTotalItems = _priv_ListView_getTotalHeights(countTotalItems, lastAverageItemHeight, heightsTopItems, heightsBottomItems, lastAverageMidItemHeight);
					
					// free visible views
					for (sl_uint32 iItem = 0; iItem < lastCountVisibleItems; iItem++) {
						Ref<View> view = viewsVisibleItems[iItem];
						if (view.isNotNull()) {
							viewsFreeItems[countFreeViews] = view;
							countFreeViews++;
							viewsVisibleItems[iItem].setNull();
						}
					}
					
				} else {
					
					// removes scrolled-over items
					sl_ui_pos yItem = m_yFirstItem;
					sl_uint32 iItem;
					for (iItem = 0; iItem < lastCountVisibleItems; iItem++) {
						sl_ui_pos h = heightsVisibleItems[iItem];
						if (yItem + h > windowStart) {
							break;
						}
						yItem += h;
						Ref<View> view = viewsVisibleItems[iItem];
						if (view.isNotNull()) {
							viewsFreeItems[countFreeViews] = view;
							countFreeViews++;
							viewsVisibleItems[iItem].setNull();
						}
					}
					
					yGoUp = yItem;
					indexGoUp = lastIndexFirstItem + iItem;
					sl_uint32 iStart = iItem;
					
					// reuse visible items
					for (; iItem < lastCountVisibleItems && yItem < windowEnd; iItem++) {
						Ref<View> view = viewsVisibleItems[iItem];
						sl_ui_len h = _updateItemLayout(view, widthListView, heightListView);
						yItem += h;
						viewsGoDownItems[countGoDownViews] = view;
						heightsGoDownItems[countGoDownViews] = h;
						countGoDownViews++;
						viewsVisibleItems[iItem].setNull();
					}
					
					
					yGoDown = yItem;
					indexGoDown = lastIndexFirstItem + iItem;
					
					if (iStart >= lastCountVisibleItems || yGoUp >= windowEnd) {
						flagClearAll = sl_true;
					}
					
					// free remaining visible views
					for (; iItem < lastCountVisibleItems; iItem++) {
						Ref<View> view = viewsVisibleItems[iItem];
						if (view.isNotNull()) {
							viewsFreeItems[countFreeViews] = view;
							countFreeViews++;
							viewsVisibleItems[iItem].setNull();
						}
					}
				}
				
				if (scrollY > (sl_ui_pos)heightTotalItems) {
					scrollY = heightTotalItems;
				}
				
				// all of former visible items is scrolled over, so have to find index and y position
				if (flagClearAll) {
					
					sl_ui_pos yTop = 0;
					sl_ui_pos yBottom = heightTotalItems;
					
					sl_ui_pos y = 0;
					sl_uint64 index = 0;
					sl_bool flagFound = sl_false;
					
					if (scrollY != 0 && lastAverageItemHeight > 0) {
						
						sl_uint32 i;
						for (i = 0; i < MAX_ITEMS_SAVE_HEIGHTS; i++) {
							if (i >= countTotalItems) {
								break;
							}
							sl_ui_pos h = heightsTopItems[i];
							if (h <= 0) {
								h = lastAverageItemHeight;
							}
							if (yTop + h > windowStart) {
								y = yTop;
								index = i;
								flagFound = sl_true;
								break;
							}
							h = heightsBottomItems[i];
							if (h <= 0) {
								h = lastAverageItemHeight;
							}
							if (yBottom - h < windowStart) {
								y = yBottom;
								index = countTotalItems - i;
								flagFound = sl_true;
								break;
							}
							yTop += h;
							yBottom -= h;
						}
						
						if (!flagFound) {
							y = windowStart;
							if (i < MAX_ITEMS_VISIBLE || Math::isAlmostZero(lastAverageMidItemHeight)) {
								index = 0;
							} else {
								double no = (double)(y - yTop) / lastAverageMidItemHeight;
								if (no < 0) {
									no = 0;
								}
								index = MAX_ITEMS_VISIBLE + (sl_uint64)(no);
								if (index > countTotalItems) {
									index = countTotalItems;
								}
							}
						}
					}
					
					yGoDown = y;
					indexGoDown = index;
					yGoUp = y;
					indexGoUp = index;
					
				}
				
				// Go Up
				{
					while (yGoUp > windowStart && indexGoUp > 0 && countGoUpViews < MAX_ITEMS_VISIBLE) {
						Ref<View> viewFree;
						if (countFreeViews > 0) {
							viewFree = viewsFreeItems[countFreeViews - 1];
							if (viewFree.isNull()) {
								countFreeViews--;
							}
						}
						Ref<View> view = _getView(adapter.get(), indexGoUp - 1, viewFree.get());
						sl_ui_len h = _updateItemLayout(view, widthListView, heightListView);
						viewsGoUpItems[countGoUpViews] = view;
						heightsGoUpItems[countGoUpViews] = h;
						countGoUpViews++;
						yGoUp -= h;
						indexGoUp--;
						if (view == viewFree && viewFree.isNotNull()) {
							countFreeViews--;
							viewsFreeItems[countFreeViews].setNull();
						}
					}
				}
				
				sl_uint64 indexStart = indexGoUp;
				sl_ui_pos yStart = yGoUp;
				
				// Go Down
				{
					while (yGoDown < windowEnd && indexGoDown < countTotalItems && countGoDownViews < MAX_ITEMS_VISIBLE) {
						Ref<View> viewFree;
						if (countFreeViews > 0) {
							viewFree = viewsFreeItems[countFreeViews - 1];
							if (viewFree.isNull()) {
								countFreeViews--;
							}
						}
						Ref<View> view = _getView(adapter.get(), indexGoDown, viewFree.get());
						sl_ui_len h = _updateItemLayout(view, widthListView, heightListView);
						viewsGoDownItems[countGoDownViews] = view;
						heightsGoDownItems[countGoDownViews] = h;
						countGoDownViews++;
						yGoDown += h;
						indexGoDown++;
						if (view == viewFree && viewFree.isNotNull()) {
							countFreeViews--;
							viewsFreeItems[countFreeViews].setNull();
						}
					}
				}
				
				// Merge to visible items
				sl_uint32 countVisibleItems = 0;
				{
					sl_uint32 i;
					sl_uint32 k = countGoUpViews - 1;
					for (i = 0; i < countGoUpViews; i++) {
						if (countVisibleItems < MAX_ITEMS_VISIBLE) {
							viewsVisibleItems[countVisibleItems] = viewsGoUpItems[k];
							heightsVisibleItems[countVisibleItems] = heightsGoUpItems[k];
							countVisibleItems++;
						} else {
							viewsFreeItems[countFreeViews] = viewsGoUpItems[k];
							countFreeViews++;
						}
						viewsGoUpItems[k].setNull();
						k--;
					}
					for (i = 0; i < countGoDownViews; i++) {
						if (countVisibleItems < MAX_ITEMS_VISIBLE) {
							viewsVisibleItems[countVisibleItems] = viewsGoDownItems[i];
							heightsVisibleItems[countVisibleItems] = heightsGoDownItems[i];
							countVisibleItems++;
						} else {
							viewsFreeItems[countFreeViews] = viewsGoDownItems[i];
							countFreeViews++;
						}
						viewsGoDownItems[i].setNull();
					}
				}
				
				// Save top&bottom heights
				{
					for (sl_uint32 i = 0; i < countVisibleItems; i++) {
						sl_uint64 index = indexStart + i;
						if (index < MAX_ITEMS_SAVE_HEIGHTS) {
							heightsTopItems[(sl_uint32)index] = heightsVisibleItems[i];
						}
						if (index < countTotalItems) {
							if (countTotalItems - index <= MAX_ITEMS_SAVE_HEIGHTS) {
								heightsBottomItems[(sl_uint32)(countTotalItems - 1 - index)] = heightsVisibleItems[i];
							}
						}
					}
				}
				sl_ui_len averageItemHeight = adapter->getAverageItemHeight(this);
				if (averageItemHeight <= 0) {
					averageItemHeight = _priv_ListView_getAverageHeight(countTotalItems, heightsTopItems, heightsBottomItems);
				}
				double averageMidItemHeight = averageItemHeight;
				heightTotalItems = _priv_ListView_getTotalHeights(countTotalItems, averageItemHeight, heightsTopItems, heightsBottomItems, averageMidItemHeight);
				
				// readjust scroll position
				if (!flagClearAll) {
					if (scrollY < heightListView || scrollY + 2 * heightListView >= (sl_ui_pos)heightTotalItems) {
						sl_ui_pos scrollOffset = scrollY - yStart;
						sl_ui_pos yStartNew = _priv_ListView_getYPositionOfItem(indexStart, countTotalItems, averageItemHeight, averageMidItemHeight, heightTotalItems, heightsTopItems, heightsBottomItems);
						if (Math::abs(yStartNew - yStart) > (sl_ui_pos)averageItemHeight / 10) {
							yStart = yStartNew;
							scrollY = yStartNew + scrollOffset;
						}
					}
				}
				
				// remove free views
				{
					for (sl_size iItem = 0; iItem < countFreeViews; iItem++) {
						contentView->removeChild(viewsFreeItems[iItem], UIUpdateMode::None);
						viewsFreeItems[iItem].setNull();
					}
				}
				
				// layout views
				{
					sl_ui_pos y = yStart;
					for (sl_size i = 0; i < countVisibleItems; i++) {
						Ref<View> view = viewsVisibleItems[i];
						if (view.isNotNull()) {
							UIRect rect = view->getLayoutFrame();
							rect.top = y + view->getMarginTop();
							y += heightsVisibleItems[i];
							rect.bottom = y - view->getMarginBottom();
							view->setLayoutFrame(rect);
							view->_applyLayout(UIUpdateMode::None);
							if (view->getParent() != contentView) {
								contentView->addChild(view, UIUpdateMode::None);
							}
						}
					}
				}
				
				m_countTotalItems = countTotalItems;
				m_indexFirstItem = indexStart;
				m_yFirstItem = yStart;
				m_countVisibleItems = countVisibleItems;
				m_averageItemHeight = averageItemHeight;
				m_averageMidItemHeight = averageMidItemHeight;
				m_heightTotalItems = heightTotalItems;
				m_lastScrollY = scrollY;
				
				if (!(Math::isAlmostZero(getContentHeight() - (sl_scroll_pos)heightTotalItems)) || !(Math::isAlmostZero(getContentWidth() - (sl_scroll_pos)widthListView))) {
					setContentSize((sl_scroll_pos)widthListView, (sl_scroll_pos)heightTotalItems, UIUpdateMode::None);
				}
				
				if (Math::isAlmostZero(getScrollY() - scrollY)) {
					if (!fromDraw) {
						contentView->invalidate();
					}
				} else {
					dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _layoutItemViews, this, sl_false, sl_true, sl_false));
				}
				
			} while (0);
			
			Base::interlockedDecrement32(&m_lockCountLayouting);
			
		} else {
			setContentHeight(0);
		}
	}
	
	sl_ui_len ListView::_updateItemLayout(const Ref<View>& itemView, sl_ui_len widthList, sl_ui_len heightList)
	{
		if (itemView.isNotNull()) {
			Ref<LayoutAttributes>& layoutAttrs = itemView->m_layoutAttrs;
			if (layoutAttrs.isNotNull()) {
				layoutAttrs->flagInvalidLayoutInParent = sl_true;
				SizeMode mode = itemView->getHeightMode();
				if (mode == SizeMode::Filling) {
					layoutAttrs->layoutFrame.setHeight((sl_ui_len)((sl_real)heightList * itemView->getHeightWeight()));
				}
			}
			UpdateLayoutFrameParam param;
			param.parentContentFrame.left = 0;
			param.parentContentFrame.right = widthList;
			param.parentContentFrame.top = 0;
			param.parentContentFrame.bottom = heightList;
			param.flagUseLayout = sl_true;
			param.flagHorizontal = sl_true;
			param.flagVertical = sl_false;
			itemView->updateLayoutFrameInParent(param);
			sl_ui_len height = itemView->getLayoutHeight() + itemView->getMarginTop() + itemView->getMarginBottom();
			sl_ui_len minItemHeight = (heightList / MAX_ITEMS_VISIBLE) + 1;
			if (height < minItemHeight) {
				itemView->setLayoutHeight(minItemHeight);
				height = minItemHeight;
			}
			return height;
		}
		return 0;
	}
	
	void _priv_ListContentView::dispatchDraw(Canvas* canvas)
	{
		Ref<ListView> lv = m_lv;
		if (lv.isNotNull()) {
			lv->_layoutItemViews(sl_true, sl_false, sl_false);
		}
		ViewGroup::dispatchDraw(canvas);
	}
	
	void _priv_ListContentView::onResizeChild(View* child, sl_ui_len width, sl_ui_len height)
	{
		Ref<ListView> lv = m_lv;
		if (lv.isNotNull()) {
			if (lv->m_lockCountLayouting == 0) {
				dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ListView, _layoutItemViews, lv.get(), sl_false, sl_false, sl_false));
			}
		}
	}
}
