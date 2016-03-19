#ifndef CHECKHEADER_SLIB_UI_S_TAB_VIEW
#define CHECKHEADER_SLIB_UI_S_TAB_VIEW

#include "definition.h"

#include "s_view.h"

SLIB_UI_NAMESPACE_BEGIN

class STabView;

class SLIB_EXPORT STabViewListener
{
public:
	virtual void onSelectTab(STabView* view, sl_uint32 index) = 0;
	
};

class STabViewItem
{
public:
	SafeString label;
	
	SafeRef<View> contentView;
	
};

class SLIB_EXPORT STabView : public SView
{
	SLIB_DECLARE_OBJECT
	
public:
	STabView();
	
public:
	sl_uint32 getTabsCount();
	
	virtual void setTabsCount(sl_uint32 count);
	
	
	String getTabLabel(sl_uint32 index);
	
	virtual void setTabLabel(sl_uint32 index, const String& text);
	
	
	Ref<View> getTabContentView(sl_uint32 index);
	
	virtual void setTabContentView(sl_uint32 index, const Ref<View>& view);
	
	
	sl_uint32 getSelectedTabIndex();
	
	virtual void selectTab(sl_uint32 index);
	
	
	Size getContentViewSize();
	
	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);
	
public:
	sl_bool isVerticalTabBar();
	
	void setVerticalTabBar(sl_bool flag);
	
	
	sl_real getTabWidth();
	
	virtual void setTabWidth(sl_real width);
	
	
	sl_real getTabHeight();
	
	virtual void setTabHeight(sl_real height);
	
	
	Ref<Drawable> getTabBarBackground();
	
	void setTabBarBackground(const Ref<Drawable>& drawable);
	
	void setTabBarBackgroundColor(const Color& color);
	
	
	Ref<Drawable> getSelectedTabBackground();

	void setSelectedTabBackground(const Ref<Drawable>& drawable);
	
	void setSelectedTabBackgroundColor(const Color& color);

	
	Ref<Drawable> getHoverTabBackground();
	
	void setHoverTabBackground(const Ref<Drawable>& drawable);
	
	void setHoverTabBackgroundColor(const Color& color);

	
	Color getTabLabelColor();
	
	void setTabLabelColor(const Color& color);
	
	
	Color getSelectedTabLabelColor();
	
	void setSelectedTabLabelColor(const Color& color);

	
	Color getHoverTabLabelColor();
	
	void setHoverTabLabelColor(const Color& color);

	
	virtual Rectangle getTabBarRegion();
	
	virtual Rectangle getTabRegion(sl_uint32 index);
	
	// override
	Rectangle getContentBounds();
	
public:
	SLIB_PTR_PROPERTY(STabViewListener, Listener)
	
protected:
	virtual void onSelectTab(sl_uint32 index);
	
	virtual void onDrawTabBarBackground(Canvas* canvas);
	
	virtual void onDrawTab(Canvas* canvas, const Rectangle& rect, sl_uint32 index, const String& label);
	
protected:
	void invalidateTabBar();
	
	void relayout();
	
protected:
	// override
	void onClickView(UIEvent* ev);

	// override
	void onMouseEvent(UIEvent* ev);

	// override
	void onDraw(Canvas* canvas);

private:
	Ref<Font> _getFont();
	
private:
	CList<STabViewItem> m_items;
	sl_uint32 m_indexSelected;
	sl_int32 m_indexHover;

	sl_bool m_flagVertical;
	sl_real m_tabWidth;
	sl_real m_tabHeight;
	
	SafeRef<Font> m_font;

	SafeRef<Drawable> m_tabBarBackground;
	SafeRef<Drawable> m_selectedTabBackground;
	SafeRef<Drawable> m_hoverTabBackground;
	Color m_tabLabelColor;
	Color m_selectedTabLabelColor;
	Color m_hoverTabLabelColor;
	
};

SLIB_UI_NAMESPACE_END

#endif
