#ifndef CHECKHEADER_SLIB_UI_EDIT_VIEW
#define CHECKHEADER_SLIB_UI_EDIT_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class EditView;

class SLIB_EXPORT IEditViewListener
{
public:
	virtual String onChange(EditView* edit, const String& newValue) = 0;
	
};

class SLIB_EXPORT EditView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	EditView();
	
public:
	String getText();
	
	virtual void setText(const String& text, sl_bool flagRedraw = sl_true);
	
	Alignment getTextAlignment();
	
	virtual void setTextAlignment(Alignment align, sl_bool flagRedraw = sl_true);
	
	String getHintText();
	
	virtual void setHintText(const String& str, sl_bool flagRedraw = sl_true);
	
	sl_bool isReadOnly();
	
	virtual void setReadOnly(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	virtual sl_bool isMultiLine();

	virtual void setMultiLine(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
public:
	SLIB_PTR_PROPERTY(IEditViewListener, Listener)
	
	SLIB_REF_PROPERTY(Runnable, EnterAction)
	
protected:
	virtual String onChange(const String& newValue);

	virtual void onEnterAction();
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	void dispatchKeyEvent(UIEvent* ev);
	
	String dispatchChange(const String& newValue);
	
	void dispatchEnterAction();
	
private:
	void _getText_NW();
	
	void _setText_NW(const String& text);
	
	void _getTextAlignment_NW();
	
	void _setTextAlignment_NW(Alignment align);
	
	void _getHintText_NW();
	
	void _setHintText_NW(const String& str);
	
	void _isReadOnly_NW();
	
	void _setReadOnly_NW(sl_bool flag);
	
	void _isMultiLine_NW();
	
	void _setMultiLine_NW(sl_bool flag);
	
	void _setTextColor_NW(const Color& color);

	// override
	void _setFont_NW(const Ref<Font>& font);
	
	// override
	void _setBorder_NW(sl_bool flag);
	
	// override
	void _setBackgroundColor_NW(const Color& color);
	
protected:
	SafeString m_text;
	Alignment m_textAlignment;
	SafeString m_hintText;
	sl_bool m_flagReadOnly;
	sl_bool m_flagMultiLine;
	Color m_textColor;

};

class PasswordView : public EditView
{
	SLIB_DECLARE_OBJECT
	
public:
	PasswordView();
	
public:
	// override
	sl_bool isMultiLine();
	
	// override
	virtual void setMultiLine(sl_bool flag, sl_bool flagRedraw = sl_true);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
};

class TextArea : public EditView
{
	SLIB_DECLARE_OBJECT
	
public:
	TextArea();
	
public:
	// override
	sl_bool isMultiLine();
	
	// override
	void setMultiLine(sl_bool flag, sl_bool flagRedraw = sl_true);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
};

SLIB_UI_NAMESPACE_END

#endif
