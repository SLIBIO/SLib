#ifndef CHECKHEADER_SLIB_UI_TAB_VIEW
#define CHECKHEADER_SLIB_UI_TAB_VIEW

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_OSX)
#define SLIB_UI_SUPPORT_NATIVE_TAB_VIEW
#endif

#if defined(SLIB_UI_SUPPORT_NATIVE_TAB_VIEW)

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
	
	virtual void setTabsCount(sl_uint32 nCount);
	
	
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
	SLIB_PTR_PROPERTY(ITabViewListener, Listener)
	
protected:
	void onSelectTab(sl_uint32 index);
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
	// override
	void onResize();

protected:
	void _refreshTabsCount();

	void _setTabLabel(sl_uint32 index, const String& text);
	
	void _setTabContentView(sl_uint32 index, const Ref<View>& view);
	
	void _setTabContentView_(sl_uint32 index, Ref<View> view);
	
	void _selectTab(sl_uint32 index);

protected:
	CList<TabViewItem> m_items;
	sl_uint32 m_indexSelected;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#else

#include "s_tab_view.h"

SLIB_UI_NAMESPACE_BEGIN
typedef STabView TabView;
typedef STabViewListener ITabViewListener;
SLIB_UI_NAMESPACE_END

#endif

#endif
