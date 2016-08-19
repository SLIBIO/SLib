#include "../../../inc/slib/ui/list_view.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/core/log.h"

#define MAX_ITEMS_VISIBLE 500
#define MAX_MID_HEIGHT 1000000

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_IOS)
#else
#	define USE_CONTENT_VIEW
#endif

SLIB_UI_NAMESPACE_BEGIN

class _ListContentView : public ViewGroup
{
public:
	WeakRef<ListView> m_lv;
	
public:
	// override
	void dispatchDraw(Canvas* canvas);
	
	// override
	void onResizeChild(View* child, sl_real width, sl_real height);
	
};

sl_real IListViewAdapter::getAverageItemHeight(ListView* lv)
{
	return 0;
}

SLIB_DEFINE_OBJECT(ListView, VerticalScrollView)

ListView::ListView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	ScrollView::setOccurringClick(sl_false);
	
	m_flagResetAdapter = sl_true;
	m_flagRefreshItems = sl_true;
	
	m_viewsVisibleItems = (Ref<View>*)(Base::createZeroMemory(sizeof(Ref<View>)*MAX_ITEMS_VISIBLE*4));
	m_viewsGoDownItems = m_viewsVisibleItems + MAX_ITEMS_VISIBLE;
	m_viewsGoUpItems = m_viewsGoDownItems + MAX_ITEMS_VISIBLE;
	m_viewsFreeItems = m_viewsGoUpItems + MAX_ITEMS_VISIBLE;
	
	m_heightsVisibleItems = (sl_real*)(Base::createMemory(sizeof(sl_real)*MAX_ITEMS_VISIBLE*5));
	m_heightsTopItems = m_heightsVisibleItems + MAX_ITEMS_VISIBLE;
	m_heightsBottomItems = m_heightsTopItems + MAX_ITEMS_VISIBLE;
	m_heightsGoDownItems = m_heightsBottomItems + MAX_ITEMS_VISIBLE;
	m_heightsGoUpItems = m_heightsGoDownItems + MAX_ITEMS_VISIBLE;
	
	_initStatus();
	
	m_contentView = new _ListContentView;
	m_contentView->setLayoutEnabled(sl_false);
	m_contentView->m_lv = this;
#ifdef USE_CONTENT_VIEW
	ScrollView::setContentView(m_contentView);
#else
	addChild(m_contentView, sl_false);
	setCreatingChildInstances(sl_true);
#endif
	
#if defined(SLIB_PLATFORM_IS_WIN32)
	setOpaque(sl_true, sl_false);
#endif

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

void ListView::setAdapter(const Ptr<IListViewAdapter>& adapter)
{
	m_adapter = adapter;
	m_flagResetAdapter = sl_true;
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ListView, _checkUpdateContent, this, sl_false));
}

void ListView::refreshItems()
{
	m_flagRefreshItems = sl_true;
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ListView, _checkUpdateContent, this, sl_false));
}

void ListView::onScroll(sl_real x, sl_real y)
{
	if (Math::isAlmostZero(y - m_lastScrollY)) {
		return;
	}
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ListView, _layoutItemViews, this, sl_false, sl_true, sl_false));
}

void ListView::onResize(sl_real x, sl_real y)
{
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ListView, _layoutItemViews, this, sl_false, sl_false, sl_false));
}

void ListView::_checkUpdateContent(sl_bool fromDraw)
{
	if (!(UI::isUiThread())) {
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
	for (sl_uint32 i = 0; i < MAX_ITEMS_VISIBLE; i++) {
		m_heightsTopItems[i] = 0;
		m_heightsBottomItems[i] = 0;
	}
}

void ListView::_resetAdapter()
{
	m_contentView->removeAllChildren();
	setContentHeight(0);
	for (sl_size i = 0; i < m_countVisibleItems; i++) {
		m_viewsVisibleItems[i].setNull();
	}
	_initStatus();
	scrollTo(0, 0);
}

static sl_real _ListView_getTotalHeights(sl_size count, sl_real averageHeight, sl_real* topHeights, sl_real* bottomHeights, double& averageMidHeight)
{
	averageMidHeight = averageHeight;
	sl_size i;
	sl_real s = 0;
	for (i = 0; i < MAX_ITEMS_VISIBLE; i++) {
		if (i >= count) {
			return s;
		}
		sl_real h = topHeights[i];
		if (h <= 0) {
			h = averageHeight;
		}
		s += h;
	}
	count -= MAX_ITEMS_VISIBLE;
	if (count == 0) {
		return s;
	}
	for (i = 0; i < MAX_ITEMS_VISIBLE; i++) {
		if (i >= count) {
			return s;
		}
		sl_real h = bottomHeights[i];
		if (h <= 0) {
			h = averageHeight;
		}
		s += h;
	}
	count -= MAX_ITEMS_VISIBLE;
	if (count == 0) {
		return s;
	}
	sl_real mid = count * averageHeight;
	if (mid > MAX_MID_HEIGHT) {
		mid = MAX_MID_HEIGHT;
		averageMidHeight = (double)mid / (double)count;
	}
	return s + mid;
}

static sl_real _ListView_getAverageHeight(sl_size count, sl_real* topHeights, sl_real* bottomHeights)
{
	sl_size i;
	sl_real s = 0;
	sl_size n = 0;
	for (i = 0; i < MAX_ITEMS_VISIBLE; i++) {
		if (i >= count) {
			break;
		}
		sl_real h = topHeights[i];
		if (h > 0) {
			s += h;
			n++;
		}
	}
	if (count > MAX_ITEMS_VISIBLE) {
		count -= MAX_ITEMS_VISIBLE;
		if (count > 0) {
			for (i = 0; i < MAX_ITEMS_VISIBLE; i++) {
				if (i >= count) {
					break;
				}
				sl_real h = bottomHeights[i];
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
	return s / (sl_real)n;
}

static sl_real _ListView_getYPositionOfItem(sl_size index, sl_size count, sl_real averageHeight, double averageMidHeight, sl_real totalHeight, sl_real* topHeights, sl_real* bottomHeights)
{
	if (index > count) {
		index = count;
	}
	if (index <= MAX_ITEMS_VISIBLE) {
		sl_real y = 0;
		for (sl_size i = 0; i < index; i++) {
			sl_real h = topHeights[i];
			if (h <= 0) {
				h = averageHeight;
			}
			y += h;
		}
		return y;
	}
	if (index + MAX_ITEMS_VISIBLE >= count) {
		sl_real y = 0;
		sl_size n = count - index;
		for (sl_size i = 0; i < n; i++) {
			sl_real h = bottomHeights[i];
			if (h <= 0) {
				h = averageHeight;
			}
			y += h;
		}
		y = totalHeight - y;
		if (y < 0) {
			y = 0;
		}
		return y;
	}
	sl_real y = 0;
	for (sl_size i = 0; i < MAX_ITEMS_VISIBLE; i++) {
		sl_real h = topHeights[i];
		if (h <= 0) {
			h = averageHeight;
		}
		y += h;
	}
	return y + (sl_real)((double)(index - MAX_ITEMS_VISIBLE) * averageMidHeight);
}

static void _ListView_pushRealArrayLeft(sl_real* arr, sl_size sizeArr, sl_size capacityArr, sl_real value, sl_size count)
{
	if (count >= capacityArr) {
		for (sl_size i = 0; i < capacityArr; i++) {
			arr[i] = 0;
		}
	} else {
		sl_size n = sizeArr + count;
		if (n > capacityArr) {
			n = capacityArr;
		}
		n -= count;
		sl_size p = n - 1;
		sl_size i;
		for (i = 0; i < n; i++) {
			arr[p+count] = arr[p];
			p--;
		}
		for (i = 0; i < count; i++) {
			arr[i] = 0;
		}
	}
}

static void _ListView_pushRealArrayRight(sl_real* arr, sl_size sizeArr, sl_size capacityArr, sl_real value, sl_size count)
{
	if (count >= capacityArr) {
		for (sl_size i = 0; i < capacityArr; i++) {
			arr[i] = 0;
		}
	} else {
		sl_size i;
		sl_size n = sizeArr;
		if (n > capacityArr) {
			n = capacityArr;
		}
		n -= count;
		for (i = 0; i < n; i++) {
			arr[i] = arr[i + count];
		}
		n += count;
		for (; i < n; i++) {
			arr[i] = 0;
		}
	}
}

void ListView::_layoutItemViews(sl_bool fromDraw, sl_bool fromScroll, sl_bool flagRefresh)
{
	if (!(UI::isUiThread())) {
		return;
	}
	
	sl_real widthListView = getWidth();
	sl_real heightListView = getHeight();
	
	if (Math::isAlmostZero(widthListView) || Math::isAlmostZero(heightListView)) {
		return;
	}
	
	PtrLocker<IListViewAdapter> adapter(m_adapter);
	
	if (adapter.isNotNull()) {
		
		Rectangle frame;
		frame.left = 0;
		frame.right = widthListView;
		
		View* contentView = m_contentView.ptr;

		Ref<View>* viewsVisibleItems = m_viewsVisibleItems;
		sl_real* heightsVisibleItems = m_heightsVisibleItems;
		sl_real* heightsTopItems = m_heightsTopItems;
		sl_real* heightsBottomItems = m_heightsBottomItems;
		Ref<View>* viewsGoDownItems = m_viewsGoDownItems;
		sl_real* heightsGoDownItems = m_heightsGoDownItems;
		Ref<View>* viewsGoUpItems = m_viewsGoUpItems;
		sl_real* heightsGoUpItems = m_heightsGoUpItems;
		Ref<View>* viewsFreeItems = m_viewsFreeItems;
		
		sl_size lastCountTotalItems = m_countTotalItems;
		sl_real lastHeightTotalItems = m_heightTotalItems;
		sl_size lastIndexFirstItem = m_indexFirstItem;
		sl_real lastAverageItemHeight = m_averageItemHeight;
		double lastAverageMidItemHeight = m_averageMidItemHeight;
		sl_size lastCountVisibleItems = m_countVisibleItems;
		
		sl_size countFreeViews = 0;
		sl_size countGoDownViews = 0;
		sl_size countGoUpViews = 0;
		
		sl_size countTotalItems = lastCountTotalItems;
		sl_real heightTotalItems = lastHeightTotalItems;
		sl_bool flagClearAll = sl_false;
		
		sl_real yGoDown = 0;
		sl_size indexGoDown = 0;
		sl_real yGoUp = 0;
		sl_size indexGoUp = 0;
		
		sl_real scrollY = getScrollY();
		if (fromScroll) {
			if (Math::isAlmostZero(scrollY - m_lastScrollY)) {
				return;
			}
		}
		sl_real originalScrollY = scrollY;
		
		if (flagRefresh) {
			
			countTotalItems = adapter->getItemsCount(this);
			
			flagClearAll = sl_true;
			
			// shift bottom heights when count is changed
			if (lastCountTotalItems != countTotalItems) {
				if (countTotalItems > lastCountTotalItems) {
					sl_size off = countTotalItems - lastCountTotalItems;
					_ListView_pushRealArrayLeft(heightsBottomItems, lastCountTotalItems, MAX_ITEMS_VISIBLE, 0, off);
				} else {
					sl_size off = lastCountTotalItems - countTotalItems;
					_ListView_pushRealArrayRight(heightsBottomItems, lastCountTotalItems, MAX_ITEMS_VISIBLE, 0, off);
				}
			}
			
			heightTotalItems = _ListView_getTotalHeights(countTotalItems, lastAverageItemHeight, heightsTopItems, heightsBottomItems, lastAverageMidItemHeight);
			
			// free visible views
			for (sl_size iItem = 0; iItem < lastCountVisibleItems; iItem++) {
				Ref<View> view = viewsVisibleItems[iItem];
				if (view.isNotNull()) {
					viewsFreeItems[countFreeViews] = view;
					countFreeViews++;
					viewsVisibleItems[iItem].setNull();
				}
			}
			
		} else {
			
			// removes scrolled-over items
			sl_real yItem = m_yFirstItem;
			sl_size iItem;
			for (iItem = 0; iItem < lastCountVisibleItems; iItem++) {
				sl_real h = heightsVisibleItems[iItem];
				if (yItem + h > scrollY) {
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
			sl_size iStart = iItem;
			
			// reuse visible items
			for (; iItem < lastCountVisibleItems && yItem < scrollY + heightListView; iItem++) {
				Ref<View> view = viewsVisibleItems[iItem];
				sl_real h = _measureItemHeight(view, heightListView);
				yItem += h;
				viewsGoDownItems[countGoDownViews] = view;
				heightsGoDownItems[countGoDownViews] = h;
				countGoDownViews++;
				viewsVisibleItems[iItem].setNull();
			}
			

			yGoDown = yItem;
			indexGoDown = lastIndexFirstItem + iItem;
			
			if (iStart >= lastCountVisibleItems || yGoUp >= scrollY + heightListView) {
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
		
		if (scrollY > heightTotalItems) {
			scrollY = heightTotalItems;
		}
		
		// all of former visible items is scrolled over, so have to find index and y position
		if (flagClearAll) {
			
			sl_real yTop = 0;
			sl_real yBottom = heightTotalItems;
			sl_size n = countTotalItems;
			
			sl_real y = 0;
			sl_size index = 0;
			sl_bool flagFound = sl_false;
			
			if (!(Math::isAlmostZero(scrollY)) && !(Math::isAlmostZero(lastAverageItemHeight))) {
				
				sl_size i;
				for (i = 0; i < MAX_ITEMS_VISIBLE; i++) {
					if (i >= n) {
						break;
					}
					sl_real h = heightsTopItems[i];
					if (h <= 0) {
						h = lastAverageItemHeight;
					}
					if (yTop + h > scrollY) {
						y = yTop;
						index = i;
						flagFound = sl_true;
						break;
					}
					h = heightsBottomItems[i];
					if (h <= 0) {
						h = lastAverageItemHeight;
					}
					if (yBottom - h < scrollY) {
						y = yBottom;
						index = countTotalItems - i;
						flagFound = sl_true;
						break;
					}
					yTop += h;
					yBottom -= h;
				}
				
				if (!flagFound) {
					y = scrollY;
					if (i < MAX_ITEMS_VISIBLE || Math::isAlmostZero(lastAverageMidItemHeight)) {
						index = 0;
					} else {
						index = MAX_ITEMS_VISIBLE + (sl_size)((double)(y - yTop) / lastAverageMidItemHeight);
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
			while (yGoUp > scrollY && indexGoUp > 0 && countGoUpViews < MAX_ITEMS_VISIBLE) {
				Ref<View> viewFree;
				if (countFreeViews > 0) {
					viewFree = viewsFreeItems[countFreeViews-1];
					if (viewFree.isNull()) {
						countFreeViews--;
					}
				}
				Ref<View> view = adapter->getView(this, indexGoUp - 1, viewFree.ptr);
				sl_real h = _measureItemHeight(view, heightListView);
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
		
		sl_size indexStart = indexGoUp;
		sl_real yStart = yGoUp;

		// Go Down
		{
			while (yGoDown < scrollY + heightListView && indexGoDown < countTotalItems && countGoDownViews < MAX_ITEMS_VISIBLE) {
				Ref<View> viewFree;
				if (countFreeViews > 0) {
					viewFree = viewsFreeItems[countFreeViews-1];
					if (viewFree.isNull()) {
						countFreeViews--;
					}
				}
				Ref<View> view = adapter->getView(this, indexGoDown, viewFree.ptr);
				sl_real h = _measureItemHeight(view, heightListView);
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
		sl_size countVisibleItems = 0;
		{
			sl_size i;
			sl_size k = countGoUpViews - 1;
			for (i = 0; i < countGoUpViews && countVisibleItems < MAX_ITEMS_VISIBLE; i++) {
				viewsVisibleItems[countVisibleItems] = viewsGoUpItems[k];
				heightsVisibleItems[countVisibleItems] = heightsGoUpItems[k];
				countVisibleItems++;
				viewsGoUpItems[k].setNull();
				k--;
			}
			for (i = 0; i < countGoDownViews && countVisibleItems < MAX_ITEMS_VISIBLE; i++) {
				viewsVisibleItems[countVisibleItems] = viewsGoDownItems[i];
				heightsVisibleItems[countVisibleItems] = heightsGoDownItems[i];
				countVisibleItems++;
				viewsGoDownItems[i].setNull();
			}
		}
		
		// Save top&bottom heights
		{
			for (sl_size i = 0; i < countVisibleItems; i++) {
				sl_size index = indexStart + i;
				if (index < MAX_ITEMS_VISIBLE) {
					heightsTopItems[index] = heightsVisibleItems[i];
				}
				if (index + MAX_ITEMS_VISIBLE >= countTotalItems && index < countTotalItems) {
					heightsBottomItems[countTotalItems - 1 - index] = heightsVisibleItems[i];
				}
			}
		}
		sl_real averageItemHeight = adapter->getAverageItemHeight(this);
		if (averageItemHeight == 0) {
			averageItemHeight = _ListView_getAverageHeight(countTotalItems, heightsTopItems, heightsBottomItems);
		}
		double averageMidItemHeight = averageItemHeight;
		heightTotalItems = _ListView_getTotalHeights(countTotalItems, averageItemHeight, heightsTopItems, heightsBottomItems, averageMidItemHeight);

		// readjust scroll position
		if (!flagClearAll) {
			if (scrollY < heightListView || scrollY + 2 * heightListView >= heightTotalItems) {
				sl_real scrollOffset = scrollY - yStart;
				sl_real yStartNew = _ListView_getYPositionOfItem(indexStart, countTotalItems, averageItemHeight, averageMidItemHeight, heightTotalItems, heightsTopItems, heightsBottomItems);
				if (Math::abs(yStartNew - yStart) > averageItemHeight/10) {
					yStart = yStartNew;
					scrollY = yStartNew + scrollOffset;
				}
			}
		}

		// remove free views
		{
			for (sl_size iItem = 0; iItem < countFreeViews; iItem++) {
				contentView->removeChild(viewsFreeItems[iItem]);
				viewsFreeItems[iItem].setNull();
			}
		}
		
		// layout views
		{
			sl_real y;
#if defined(USE_CONTENT_VIEW)
			y = yStart;
#else
			y = yStart - scrollY;
#endif
			for (sl_size i = 0; i < countVisibleItems; i++) {
				Ref<View> view = viewsVisibleItems[i];
				sl_real b = y + heightsVisibleItems[i];
				if (view.isNotNull()) {
					Rectangle rect;
					rect.left = 0;
					rect.right = widthListView;
					rect.top = y;
					rect.bottom = b;
					if (view->getParent() != contentView) {
						view->setFrame(rect, sl_false);
						contentView->addChild(view, !fromDraw);
					} else {
						view->setFrame(rect, !fromDraw);
					}
				}
				y = b;
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

		if (!(Math::isAlmostZero(getContentHeight() - heightTotalItems)) || !(Math::isAlmostZero(getContentWidth() - widthListView))) {
			setContentSize(widthListView, heightTotalItems);
		}
		if (!(Math::isAlmostZero(scrollY - originalScrollY))) {
			scrollTo(0, scrollY);
		}
		
#if !defined(USE_CONTENT_VIEW)
#if defined(SLIB_PLATFORM_IS_IOS)
		Rectangle rcContent = getBounds();
		rcContent.top += scrollY;
		rcContent.bottom += scrollY;
		contentView->setFrame(rcContent, sl_false);
#else
		contentView->setFrame(getBounds(), sl_false);
#endif
#endif
		if (!fromDraw) {
			contentView->invalidate();
		}
	}
}

sl_real ListView::_measureItemHeight(const Ref<View>& itemView, sl_real heightList)
{
	sl_real ret = 0;
	if (itemView.isNotNull()) {
		SizeMode mode = itemView->getHeightMode();
		switch (mode) {
			case SizeMode::Filling:
				ret = heightList;
				break;
			case SizeMode::Weight:
				ret = heightList * itemView->getHeightWeight();
				break;
			case SizeMode::Wrapping:
				itemView->measureLayout();
				ret = itemView->getMeasuredHeight();
				break;
			default:
				ret = itemView->getHeight();
				break;
		}
	}
	sl_real minItemHeight = heightList / (MAX_ITEMS_VISIBLE + 1);
	if (ret < minItemHeight) {
		ret = minItemHeight;
	}
	return ret;
}

void _ListContentView::dispatchDraw(Canvas* canvas)
{
	Ref<ListView> lv = m_lv;
	if (lv.isNotNull()) {
		lv->_layoutItemViews(sl_true, sl_false, sl_false);
	}
	ViewGroup::dispatchDraw(canvas);
}

void _ListContentView::onResizeChild(View* child, sl_real width, sl_real height)
{
	Ref<ListView> lv = m_lv;
	if (lv.isNotNull()) {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ListView, _layoutItemViews, lv.ptr, sl_false, sl_false, sl_false));
	}
}

SLIB_UI_NAMESPACE_END
