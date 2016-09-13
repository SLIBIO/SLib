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
	sl_bool isHorizontalScrolling();
	
	// must be called before creating instruction or seting content view/size
	void setHorizontalScrolling(sl_bool flagHorizontal);
	
	sl_bool isVerticalScrolling();
	
	// must be called before creating instruction or seting content view/size
	void setVerticalScrolling(sl_bool flagVertical);
	
	Ref<View> getContentView();
	
	virtual void setContentView(const Ref<View>& view, sl_bool flagRedraw = sl_true);

	// override
	void setContentSize(sl_scroll_pos width, sl_scroll_pos height, sl_bool flagRefresh = sl_true);

	void setContentSize(const ScrollPoint& size, sl_bool flagRefresh = sl_true);
	
	// override
	ScrollPoint getScrollPosition();
	
	// override
	ScrollPoint getScrollRange();
	
	// override
	void scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagRedraw = sl_true);
	
	void scrollTo(const ScrollPoint& position, sl_bool flagRedraw = sl_true);

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
	void _initScrollbars();
	
	void _scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagRedraw);
	
	void _refreshContentSize_NW();
	
	void _setContentView_NW(const Ref<View>& view);
	
	void _setContentView_NW_OnUiThread(const Ref<View> view);

	void _scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y);
	
	ScrollPoint _getScrollPosition_NW();
	
	ScrollPoint _getScrollRange_NW();
	
	// override
	void _setBorder_NW(sl_bool flag);
	
	// override
	void _setBackgroundColor_NW(const Color& color);
	
protected:
	void _onScroll_NW(sl_scroll_pos x, sl_scroll_pos y);
	
protected:
	SafeRef<View> m_viewContent;
	sl_bool m_flagHorizontalScroll;
	sl_bool m_flagVerticalScroll;
	sl_bool m_flagInitedScrollbars;
	
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
