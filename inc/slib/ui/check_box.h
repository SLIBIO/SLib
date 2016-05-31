#ifndef CHECKHEADER_SLIB_UI_CHECK_BOX
#define CHECKHEADER_SLIB_UI_CHECK_BOX

#include "definition.h"

#include "button.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT CheckBox : public Button
{
	SLIB_DECLARE_OBJECT
	
public:
	CheckBox();
	
	CheckBox(sl_uint32 nCategories, ButtonCategory* categories);
	
public:
	sl_bool isChecked();
	
	virtual void setChecked(sl_bool flag, sl_bool flagRedraw = sl_true);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	void dispatchClick(UIEvent* ev);
	
public:
	void _getChecked_NW();
	
	void _setChecked_NW(sl_bool flag);

protected:
	sl_bool m_flagChecked;
	
};

SLIB_UI_NAMESPACE_END

#endif
