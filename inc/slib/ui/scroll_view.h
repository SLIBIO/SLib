#ifndef CHECKHEADER_SLIB_UI_SCROLL_VIEW
#define CHECKHEADER_SLIB_UI_SCROLL_VIEW

#include "definition.h"
#include "view.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT ScrollView : public View
{
	SLIB_DECLARE_OBJECT(ScrollView, View)
public:
	ScrollView();
	
public:
	Ref<View> getContentView();
	virtual void setContentView(const Ref<View>& view);
	
	virtual void scrollTo(sl_real x, sl_real y);
	void scrollTo(const Point& pt);
	
	Size getScrollRange();
	Point getScrollPosition();
	
public:
	sl_bool isBorder();
	virtual void setBorder(sl_bool flag);
	
	Color getBackgroundColor();
	virtual void setBackgroundColor(const Color& color);

protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
	// override
	void onResize();

	// override
	void onResizeChild(View* child);
	
protected:
	void _setContentView(const Ref<View>& view);
	void _setContentView_Safe(const Ref<View> view);
	
	void _setBorder(sl_bool flag);
	
	void _setBackgroundColor(const Color& color);
	
protected:
	Ref<View> m_viewContent;
	sl_bool m_flagBorder;
	Color m_backgroundColor;
};
SLIB_UI_NAMESPACE_END

#endif
