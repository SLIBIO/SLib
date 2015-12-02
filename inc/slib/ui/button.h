#ifndef CHECKHEADER_SLIB_UI_BUTTON
#define CHECKHEADER_SLIB_UI_BUTTON

#include "definition.h"
#include "view.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT Button : public View
{
	SLIB_DECLARE_OBJECT(Button, View)
public:
	Button();
	
public:
	String getText();
	virtual void setText(const String& text);
	
	Ref<Font> getFont();
	virtual void setFont(const Ref<Font>& font);
	
	sl_bool isDefaultButton();
	virtual void setDefaultButton(sl_bool flag);

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

	Ref<Font> m_font;
	Ref<FontInstance> m_fontInstance;
	
	sl_bool m_flagDefaultButton;
};
SLIB_UI_NAMESPACE_END

#endif
