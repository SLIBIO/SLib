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
	
	virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Alignment getGravity();
	
	virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	String getHintText();
	
	virtual void setHintText(const String& str, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	sl_bool isReadOnly();
	
	virtual void setReadOnly(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	virtual sl_bool isMultiLine();

	virtual void setMultiLine(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	UIReturnKeyType getReturnKeyType();
	
	void setReturnKeyType(UIReturnKeyType type);
	
	UIKeyboardType getKeyboardType();
	
	void setKeyboardType(UIKeyboardType type);
	
	void setAutoCapitalizationType(UIAutoCapitalizationType type);
	
	UIAutoCapitalizationType getAutoCaptializationType();
	
	sl_bool isAutoDismissKeyboard();
	
	void setAutoDismissKeyboard(sl_bool flag);
	
protected:
	// override
	void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
	
public:
	SLIB_PROPERTY(AtomicPtr<IEditViewListener>, Listener)
	
	SLIB_PROPERTY(AtomicFunction<void()>, EnterAction)
	
protected:
	virtual String onChange(const String& newValue);

	virtual void onEnterAction();
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	void dispatchKeyEvent(UIEvent* ev);
	
	virtual String dispatchChange(const String& newValue);
	
	virtual void dispatchEnterAction();
	
private:
	void _getText_NW();
	
	void _setText_NW(const String& text);
	
	void _setTextAlignment_NW(Alignment align);
	
	void _setHintText_NW(const String& str);
	
	void _setReadOnly_NW(sl_bool flag);
	
	void _setMultiLine_NW(sl_bool flag);
	
	void _setTextColor_NW(const Color& color);
	
	void _setReturnKeyType_NW(UIReturnKeyType type);
	
	void _setKeyboardType_NW(UIKeyboardType type);
	
	void _setAutoCapitalizationType_NW(UIAutoCapitalizationType type);

	// override
	void _setFont_NW(const Ref<Font>& font);
	
	// override
	void _setBorder_NW(sl_bool flag);
	
	// override
	void _setBackgroundColor_NW(const Color& color);
	
protected:
	AtomicString m_text;
	Alignment m_textAlignment;
	AtomicString m_hintText;
	sl_bool m_flagReadOnly;
	sl_bool m_flagMultiLine;
	Color m_textColor;
	UIReturnKeyType m_returnKeyType;
	UIKeyboardType m_keyboardType;
	UIAutoCapitalizationType m_autoCapitalizationType;
	sl_bool m_flagAutoDismissKeyboard;
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
	virtual void setMultiLine(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);

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
	void setMultiLine(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);

public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
};

SLIB_UI_NAMESPACE_END

#endif
