#ifndef CHECKHEADER_SLIB_UI_RADIO_BUTTON
#define CHECKHEADER_SLIB_UI_RADIO_BUTTON

#include "definition.h"

#include "check_box.h"

SLIB_UI_NAMESPACE_BEGIN

class RadioGroup;

class SLIB_EXPORT RadioButton : public CheckBox
{
	SLIB_DECLARE_OBJECT
	
public:
	RadioButton();
	
	RadioButton(sl_uint32 nCategories, ButtonCategory* categories);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	void dispatchClick(UIEvent* ev);
	
public:
	SLIB_WEAK_PROPERTY(RadioGroup, RadioGroup)
	
};

class SLIB_EXPORT RadioGroup : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	RadioGroup();
	
public:
	void add(const Ref<RadioButton>& view);
	
	void remove(const Ref<RadioButton>& view);
	
	void select(const Ref<RadioButton>& view);
	
	Ref<RadioButton> getSelected();
	
protected:
	CList< Ref<RadioButton> > m_views;
	SafeRef<RadioButton> m_viewSelected;
	
};

SLIB_UI_NAMESPACE_END

#endif
