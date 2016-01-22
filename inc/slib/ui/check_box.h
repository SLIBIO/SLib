#ifndef CHECKHEADER_SLIB_UI_CHECK_BOX
#define CHECKHEADER_SLIB_UI_CHECK_BOX

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_OSX)
#define SLIB_UI_SUPPORT_NATIVE_CHECK_BOX
#endif

#if defined(SLIB_UI_SUPPORT_NATIVE_CHECK_BOX)

#include "view.h"
#include "radio_group.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT CheckBox : public View
{
	SLIB_DECLARE_OBJECT(CheckBox, View)
public:
	CheckBox();
	
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
	sl_bool m_flagChecked;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#else

#include "s_check_box.h"

SLIB_UI_NAMESPACE_BEGIN
typedef SCheckBox CheckBox;
SLIB_UI_NAMESPACE_END

#endif

#endif
