#ifndef CHECKHEADER_SLIB_UI_RADIO_GROUP
#define CHECKHEADER_SLIB_UI_RADIO_GROUP

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT RadioGroup : public Object
{
	SLIB_DECLARE_OBJECT(RadioGroup, Object)
public:
	RadioGroup();
	
public:
	void addView(const Ref<View>& view);
	
	void removeView(const Ref<View>& view);

	
	void select(const Ref<View>& view);
	
	Ref<View> getSelectedView();

protected:
	void _setChecked(View* view, sl_bool flag);
	
protected:
	CList< Ref<View> > m_views;
	SafeRef<View> m_viewSelected;
	
};

SLIB_UI_NAMESPACE_END

#endif
