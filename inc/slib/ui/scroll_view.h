#ifndef CHECKHEADER_SLIB_UI_SCROLL_VIEW
#define CHECKHEADER_SLIB_UI_SCROLL_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ScrollView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ScrollView();
	
public:
	Ref<View> getContentView();
	
	virtual void setContentView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);

	// override
	void setContentSize(sl_scroll_pos width, sl_scroll_pos height, UIUpdateMode mode = UIUpdateMode::Redraw);

	void setContentSize(const ScrollPoint& size, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	// override
	ScrollPoint getScrollPosition();
	
	// override
	ScrollPoint getScrollRange();
	
	// override
	void scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void scrollTo(const ScrollPoint& position, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isPaging();
	
	void setPaging(sl_bool flagPaging);
	
	sl_ui_len getPageWidth();
	
	void setPageWidth(sl_ui_len width);
	
	sl_ui_len getPageHeight();
	
	void setPageHeight(sl_ui_len height);
	
	// override
	void setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode = UIUpdateMode::Redraw);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);

protected:
	// override
	void onResize(sl_ui_len width, sl_ui_len height);
	
	// override
	void onResizeChild(View* child, sl_ui_len width, sl_ui_len height);
	
	// override
	void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);

	// override
	void onMakeLayout();
	
private:
	void _scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode);
	
	void _updatePaging();
	
	void _refreshContentSize_NW();
	
	void _setContentView_NW(const Ref<View>& view);
	
	void _scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y);
	
	ScrollPoint _getScrollPosition_NW();
	
	ScrollPoint _getScrollRange_NW();
	
	// override
	void _setBorder_NW(sl_bool flag);
	
	// override
	void _setBackgroundColor_NW(const Color& color);
	
	void _setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight);
	
	void _setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical);
	
protected:
	void _onScroll_NW(sl_scroll_pos x, sl_scroll_pos y);
	
protected:
	AtomicRef<View> m_viewContent;
	sl_bool m_flagPaging;
	sl_ui_pos m_pageWidth;
	sl_ui_pos m_pageHeight;
	
};

class HorizontalScrollView : public ScrollView
{
public:
	HorizontalScrollView();
	
};

class VerticalScrollView : public ScrollView
{
public:
	VerticalScrollView();
	
};

SLIB_UI_NAMESPACE_END

#endif
