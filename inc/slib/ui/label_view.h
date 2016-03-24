#ifndef CHECKHEADER_SLIB_UI_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_LABEL_VIEW

#include "definition.h"

#include "view.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT LabelView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	LabelView();
	
public:
	String getText();
	
	virtual void setText(const String& text);
	
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color);
	
	
	Alignments getTextAlignment();
	
	virtual void setTextAlignment(Alignments align);
	
	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);

public:
	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color);
	
	
	sl_bool isBorder();
	
	virtual void setBorder(sl_bool flag);
	
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
	sl_bool m_flagBorder;
	Alignments m_textAlignment;
	Color m_textColor;
	Color m_backgroundColor;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

SLIB_UI_NAMESPACE_END

#endif
