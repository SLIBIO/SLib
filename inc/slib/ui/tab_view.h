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
	
	UISize getContentViewSize();
	
	LayoutOrientation getOrientation();
	
	void setOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getTabWidth();
	
	virtual void setTabWidth(sl_ui_len width, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getTabHeight();
	
	virtual void setTabHeight(sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getBarBackground();
	
	void setBarBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setBarBackground(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getContentBackground();
	
	void setContentBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setContentBackground(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getTabBackground();
	
	void setTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setTabBackground(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getSelectedTabBackground();
	
	void setSelectedTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setSelectedTabBackground(const Color& color, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getHoverTabBackground();
	
	void setHoverTabBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw = sl_true);
	
	void setHoverTabBackground(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getLabelColor();
	
	void setLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getSelectedLabelColor();
	
	void setSelectedLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getHoverLabelColor();
	
	void setHoverLabelColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Alignment getLabelAlignment();
	
	virtual void setLabelAlignment(Alignment align, sl_bool flagRedraw = sl_true);
	
	virtual void setLabelMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw = sl_true);
	
	void setLabelMargin(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getLabelMarginLeft();
	
	void setLabelMarginLeft(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getLabelMarginTop();
	
	void setLabelMarginTop(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getLabelMarginRight();
	
	void setLabelMarginRight(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getLabelMarginBottom();
	
	void setLabelMarginBottom(sl_ui_pos margin, sl_bool flagRedraw = sl_true);

	virtual UIRect getTabBarRegion();
	
	virtual UIRect getTabRegion(sl_uint32 index);
	
	virtual UIRect getWholeContentRegion();

	virtual UIRect getTabContentRegion();
	
public:
	SLIB_PTR_PROPERTY(ITabViewListener, Listener)
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	void dispatchSelectTab(sl_uint32 index);
	
protected:
	virtual void onSelectTab(sl_uint32 index);
	
	// override
	void onClick(UIEvent* ev);
	
	// override
	void onMouseEvent(UIEvent* ev);
	
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onResize(sl_ui_len width, sl_ui_len height);
	
	virtual void onDrawTab(Canvas* canvas, const UIRect& rect, sl_uint32 index, const String& label);
	
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
	
	UISize _getContentViewSize_NW();
	
	void _selectTab_NW(sl_uint32 index);
	
	// override
	void _setFont_NW(const Ref<Font>& font);
	
protected:
	CList<TabViewItem> m_items;
	sl_uint32 m_indexSelected;
	sl_int32 m_indexHover;
	
	LayoutOrientation m_orientation;
	sl_ui_len m_tabWidth;
	sl_ui_len m_tabHeight;
	
	SafeRef<Drawable> m_barBackground;
	SafeRef<Drawable> m_contentBackground;
	SafeRef<Drawable> m_tabBackground;
	SafeRef<Drawable> m_selectedTabBackground;
	SafeRef<Drawable> m_hoverTabBackground;
	Color m_labelColor;
	Color m_selectedLabelColor;
	Color m_hoverLabelColor;
	Alignment m_labelAlignment;
	sl_ui_pos m_labelMarginLeft;
	sl_ui_pos m_labelMarginTop;
	sl_ui_pos m_labelMarginRight;
	sl_ui_pos m_labelMarginBottom;
	
};

SLIB_UI_NAMESPACE_END

#endif
