#ifndef CHECKHEADER_SLIB_UI_RADIO_BUTTON
#define CHECKHEADER_SLIB_UI_RADIO_BUTTON

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_OSX)
#define SLIB_UI_SUPPORT_NATIVE_RADIO_BUTTON
#endif

#if defined(SLIB_UI_SUPPORT_NATIVE_RADIO_BUTTON)

#include "view.h"

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
	SLIB_PROPERTY_INLINE(Ref<Runnable>, OnClick)
	
public:
	// override
	void onClick();
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	String m_text;
	sl_bool m_flagSelected;
	Ref<Font> m_font;
	Ref<FontInstance> m_fontInstance;
};
SLIB_UI_NAMESPACE_END
#else

#include "s_radio_button.h"

SLIB_UI_NAMESPACE_BEGIN
typedef SRadioButton RadioButton;
SLIB_UI_NAMESPACE_END

#endif

#endif
