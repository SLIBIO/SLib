#ifndef CHECKHEADER_SLIB_UI_TAB_VIEW
#define CHECKHEADER_SLIB_UI_TAB_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class TabView;

class SLIB_EXPORT ITabViewListener
{
public:
	virtual void onSelectTab(TabView* view, sl_uint32 index) = 0;
};

class TabViewItem
{
public:
	SafeString label;
	
	SafeRef<View> contentView;
	
};

class SLIB_EXPORT TabView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	TabView();
	
public:
	sl_uint32 getTabsCount();
	
	virtual void setTabsCount(sl_uint32 count, sl_bool flagRedraw = sl_true);
	
	String getTabLabel(sl_uint32 index);
	
	virtual void setTabLabel(sl_uint32 index, const String& text, sl_bool flagRedraw = sl_true);
	
	Ref<View> getTabContentView(sl_uint32 index);
	
	virtual void setTabContentView(sl_uint32 index, const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	sl_uint32 getSelectedTabIndex();
	
	virtual void selectTab(sl_uint32 index, sl_bool flagRedraw = sl_true);
	
	Size getContentViewSize();
	
	sl_bool isVerticalTabBar();
	
	void setVerticalTabBar(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	sl_real getTabWidth();
	
	virtual void setTabWidth(sl_real width, sl_bool flagRedraw = sl_true);
	
	sl_real getTabHeight();
	
	virtual void setTabHeight(sl_real height, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getTabBarBackground();
	
	void setTabBarBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setTabBarBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getSelectedTabBackground();
	
	void setSelectedTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setSelectedTabBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverTabBackground();
	
	void setHoverTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setHoverTabBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getTabLabelColor();
	
	void setTabLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getSelectedTabLabelColor();
	
	void setSelectedTabLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getHoverTabLabelColor();
	
	void setHoverTabLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	virtual Rectangle getTabBarRegion();
	
	virtual Rectangle getTabRegion(sl_uint32 index);
	
	virtual Rectangle getTabContentRegion();
	
public:
	SLIB_PTR_PROPERTY(ITabViewListener, Listener)
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	void dispatchSelectTab(sl_uint32 index);
	
protected:
	virtual void onSelectTab(sl_uint32 index);
	
	// override
	void onClickView(UIEvent* ev);
	
	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onResize(sl_real width, sl_real height);
	
	virtual void onDrawTabBarBackground(Canvas* canvas);
	
	virtual void onDrawTab(Canvas* canvas, const Rectangle& rect, sl_uint32 index, const String& label);
	
protected:
	void _invalidateTabBar();
	
	void _relayout(sl_bool flagRedraw);
	
private:
	void _refreshTabsCount_NW();
	
	void _refreshSize_NW();
	
	void _setTabLabel_NW(sl_uint32 index, const String& text);
	
	void _setTabContentView_NW(sl_uint32 index, const Ref<View>& view);
	
	void _setTabContentView_NW_OnUiThread(sl_uint32 index, Ref<View> view);
	
	void _getSelectedTabIndex_NW();
	
	Size _getContentViewSize_NW();
	
	void _selectTab_NW(sl_uint32 index);
	
	// override
	void _setFont_NW(const Ref<Font>& font);
	
protected:
	CList<TabViewItem> m_items;
	sl_uint32 m_indexSelected;
	sl_int32 m_indexHover;
	
	sl_bool m_flagVertical;
	sl_real m_tabWidth;
	sl_real m_tabHeight;
	
	SafeRef<Drawable> m_tabBarBackground;
	SafeRef<Drawable> m_selectedTabBackground;
	SafeRef<Drawable> m_hoverTabBackground;
	Color m_tabLabelColor;
	Color m_selectedTabLabelColor;
	Color m_hoverTabLabelColor;
	
};

SLIB_UI_NAMESPACE_END

#endif
