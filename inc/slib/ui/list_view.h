#ifndef CHECKHEADER_SLIB_UI_LIST_VIEW
#define CHECKHEADER_SLIB_UI_LIST_VIEW

#include "definition.h"

#include "scroll_view.h"

SLIB_UI_NAMESPACE_BEGIN

class ListView;

class SLIB_EXPORT IListViewAdapter
{
public:
	virtual sl_size getItemsCount(ListView* lv) = 0;
	
	virtual Ref<View> getView(ListView* lv, sl_size index, View* original) = 0;
	
	virtual sl_real getAverageItemHeight(ListView* lv);
	
};


class _ListContentView;

class SLIB_EXPORT ListView : public VerticalScrollView
{
	SLIB_DECLARE_OBJECT
	
public:
	ListView();
	
	~ListView();
	
public:
	void setAdapter(const Ptr<IListViewAdapter>& adapter);
	
	void refreshItems();
	
protected:
	// override
	void onScroll(sl_real x, sl_real y);
	
	// override
	void onResize(sl_real x, sl_real y);

protected:
	void _checkUpdateContent(sl_bool fromDraw);
	
	void _initStatus();
	
	void _resetAdapter();
	
	void _layoutItemViews(sl_bool flagFromDraw, sl_bool flagFromScroll, sl_bool flagRefresh);
	
	sl_real _measureItemHeight(const Ref<View>& itemView, sl_real heightList);
		
protected:
	SafePtr<IListViewAdapter> m_adapter;
	
	Ref<_ListContentView> m_contentView;
	sl_bool m_flagResetAdapter;
	sl_bool m_flagRefreshItems;

	sl_size m_countTotalItems;
	sl_size m_indexFirstItem;
	sl_size m_countVisibleItems;
	sl_real m_yFirstItem;
	Ref<View>* m_viewsVisibleItems;
	Ref<View>* m_viewsGoDownItems;
	Ref<View>* m_viewsGoUpItems;
	Ref<View>* m_viewsFreeItems;
	sl_real* m_heightsVisibleItems;
	sl_real* m_heightsTopItems;
	sl_real* m_heightsBottomItems;
	sl_real* m_heightsGoDownItems;
	sl_real* m_heightsGoUpItems;
	sl_real m_averageItemHeight;
	double m_averageMidItemHeight;
	sl_real m_heightTotalItems;
	sl_real m_lastScrollY;
		
	friend class _ListContentView;
	
};

SLIB_UI_NAMESPACE_END

#endif
