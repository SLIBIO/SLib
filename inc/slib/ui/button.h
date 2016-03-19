#ifndef CHECKHEADER_SLIB_UI_BUTTON
#define CHECKHEADER_SLIB_UI_BUTTON

#include "definition.h"

#include "view.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT Button : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	Button();
	
public:
	String getText();
	
	virtual void setText(const String& text);
	
	
	sl_bool isDefaultButton();
	
	virtual void setDefaultButton(sl_bool flag);

	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);
	
public:
	SLIB_REF_PROPERTY(Runnable, OnClick)
	
public:
	// override
	void dispatchClick();
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	SafeString m_text;
	sl_bool m_flagDefaultButton;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#endif
