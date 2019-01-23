/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_UI_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_LABEL_VIEW

#include "definition.h"

#include "view.h"

#include "../graphics/text.h"

namespace slib
{

	struct TextLine {
		String16 text;
		sl_ui_len width;
		sl_ui_len height;
	};
	
	class SLIB_EXPORT LabelView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		LabelView();
		
		~LabelView();
		
	public:
		String getText();
		
		virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		MultiLineMode getMultiLine();
		
		virtual void setMultiLine(MultiLineMode multiLineMode, UIUpdateMode updateMode = UIUpdateMode::UpdateLayout);
		
		EllipsizeMode getEllipsize();
		
		virtual void setEllipsize(EllipsizeMode ellipsizeMode, UIUpdateMode updateMode = UIUpdateMode::UpdateLayout);
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onUpdateLayout() override;
		
	private:
		void _setText_NW(const String& text);
		
		void _setTextColor_NW(const Color& color);
		
		void _setTextAlignment_NW(Alignment align);
		
		void _setFont_NW(const Ref<Font>& font) override;
		
		void _setBorder_NW(sl_bool flag) override;
		
		void _setBackgroundColor_NW(const Color& color) override;
		
	protected:
		AtomicString m_text;
		Color m_textColor;
		Alignment m_textAlignment;
		MultiLineMode m_multiLineMode;
		EllipsizeMode m_ellipsizeMode;
		
		SimpleTextBox m_textBox;
		
	};

}

#endif
