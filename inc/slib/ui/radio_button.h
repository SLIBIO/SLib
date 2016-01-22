#ifndef CHECKHEADER_SLIB_UI_RADIO_BUTTON
#define CHECKHEADER_SLIB_UI_RADIO_BUTTON

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_OSX)
#define SLIB_UI_SUPPORT_NATIVE_RADIO_BUTTON
#endif

#if defined(SLIB_UI_SUPPORT_NATIVE_RADIO_BUTTON)

#include "view.h"
#include "radio_group.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT RadioButton : public View
{
	SLIB_DECLARE_OBJECT(RadioButton, View)
public:
	RadioButton();
	
public:
	sl_bool isChecked();
	
	virtual void setChecked(sl_bool flag);
	
public:
	String getText();
	
	virtual void setText(const String& text);
	
	
    Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);

public:
	SLIB_REF_PROPERTY(Runnable, OnClick)
	
	SLIB_WEAK_PROPERTY(RadioGroup, RadioGroup)

public:
	// override
	void dispatchClick();
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	SafeString m_text;
	sl_bool m_flagSelected;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#else

#include "s_radio_button.h"

SLIB_UI_NAMESPACE_BEGIN
typedef SRadioButton RadioButton;
SLIB_UI_NAMESPACE_END

#endif

#endif
