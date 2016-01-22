#ifndef CHECKHEADER_SLIB_UI_S_CHECK_BOX
#define CHECKHEADER_SLIB_UI_S_CHECK_BOX

#include "definition.h"

#include "s_button.h"
#include "radio_group.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT SCheckBox : public SButton
{
	SLIB_DECLARE_OBJECT(SCheckBox, SButton)
public:
	SCheckBox();
	SCheckBox(sl_uint32 nCategories, SButtonCategory* categories);
	
public:
	sl_bool isChecked();
	
	virtual void setChecked(sl_bool flag);
	
public:
	// override
	void dispatchClick();
	
public:
	SLIB_WEAK_PROPERTY(RadioGroup, RadioGroup)

protected:
	sl_bool m_flagChecked;
	
};

SLIB_UI_NAMESPACE_END

#endif
