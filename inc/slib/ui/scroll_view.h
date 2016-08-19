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
	
	ScrollView(sl_bool flagVertical);
	
public:
	Ref<View> getContentView();
	
	virtual void setContentView(const Ref<View>& view, sl_bool flagRedraw = sl_true);

	// override
	void setContentSize(sl_real width, sl_real height, sl_bool flagRefresh = sl_true);

	void setContentSize(const Size& size, sl_bool flagRefresh = sl_true);
	
	// override
	Point getScrollPosition();
	
	// override
	Size getScrollRange();
	
	// override
	void scrollTo(sl_real x, sl_real y, sl_bool flagRedraw = sl_true);
	
	void scrollTo(const Point& position, sl_bool flagRedraw = sl_true);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);

protected:
	// override
	void onResize(sl_real width, sl_real height);
	
	// override
	void onResizeChild(View* child, sl_real width, sl_real height);
	
	// override
	void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);

	// override
	void onMakeLayout();
	
private:
	void _init(sl_bool flagBothScroll, sl_bool flagVertical);
	
	void _scrollTo(sl_real x, sl_real y, sl_bool flagRedraw);
	
	void _refreshContentSize_NW();
	
	void _setContentView_NW(const Ref<View>& view);
	
	void _setContentView_NW_OnUiThread(const Ref<View> view);

	void _scrollTo_NW(sl_real x, sl_real y);
	
	Point _getScrollPosition_NW();
	
	Size _getScrollRange_NW();
	
	// override
	void _setBorder_NW(sl_bool flag);
	
	// override
	void _setBackgroundColor_NW(const Color& color);
	
protected:
	void _onScroll_NW(sl_real x, sl_real y);
	
protected:
	SafeRef<View> m_viewContent;
	sl_bool m_flagBothScroll;
	sl_bool m_flagVerticalScroll;
	
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
