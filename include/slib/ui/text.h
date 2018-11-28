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

#ifndef CHECKHEADER_SLIB_UI_TEXT
#define CHECKHEADER_SLIB_UI_TEXT

#include "definition.h"

#include "../graphics/text.h"

namespace slib
{
	
	class TextInput : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextInput();
		
		~TextInput();
		
	public:
		virtual String getTextInRange(const TextRange& range) = 0;
		
		virtual sl_text_pos getPositionAtPoint(const Point& pt) = 0;
		
		virtual UIRect getFirstRectangleForRange(const TextRange& range) = 0;
		
		virtual List<UIRect> getRectanglesForRange(const TextRange& range) = 0;
		
		virtual UIRect getCaretRectangleForPosition(sl_text_pos pos) = 0;
		
		virtual sl_text_pos getClosestPositionToPoint(const Point& pt, const TextRange& range) = 0;

	public:
		TextRange getSelectedRange();
		
		void setSelectedRange(const TextRange& range);
		
		TextRange getMarkedRange();
		
		void setMarkedRange(const TextRange& range);
		
		void replaceText(const TextRange& range, const String& text);

	protected:
		virtual void onReplaceText(const TextRange& range, const String& text) = 0;
		
		virtual void onRedraw() = 0;
		
		virtual void onChangeMarkedRange();
		
		virtual void onChangeSelectedRange();
		
	protected:
		TextRange m_markedRange;
		TextRange m_selectedRange;
		
	};
	
	class UITextBox : public TextInput
	{
		SLIB_DECLARE_OBJECT
		
	public:
		UITextBox();
		
		~UITextBox();
		
	public:
		String getTextInRange(const TextRange& range) override;
		
		sl_text_pos getPositionAtPoint(const Point& pt) override;
		
		UIRect getFirstRectangleForRange(const TextRange& range) override;
		
		List<UIRect> getRectanglesForRange(const TextRange& range) override;
		
		UIRect getCaretRectangleForPosition(sl_text_pos pos) override;
		
		sl_text_pos getClosestPositionToPoint(const Point& pt, const TextRange& range) override;
		
	protected:
		void onChangeMarkedRange() override;
		
		void onChangeSelectedRange() override;
		
	public:
		CList< Ref<TextParagraph> > m_paragraphs;
		
	};

}

#endif
