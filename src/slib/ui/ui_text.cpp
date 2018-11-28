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

#include "slib/ui/text.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(TextInput, Object)
	
	TextInput::TextInput()
	{
		m_selectedRange.location = 0;
	}
	
	TextInput::~TextInput()
	{
	}
	
	TextRange TextInput::getSelectedRange()
	{
		return m_selectedRange;
	}
	
	void TextInput::setSelectedRange(const TextRange& range)
	{
		if (m_selectedRange != range) {
			m_selectedRange = range;
			onChangeSelectedRange();
		}
	}
	
	TextRange TextInput::getMarkedRange()
	{
		return m_markedRange;
	}
	
	void TextInput::setMarkedRange(const TextRange& range)
	{
		if (m_markedRange != range) {
			m_markedRange = range;
			onChangeMarkedRange();
			onRedraw();
		}
	}
	
	void TextInput::replaceText(const TextRange& range, const String& text)
	{
		
	}
	
	void TextInput::onChangeMarkedRange()
	{
	}
	
	void TextInput::onChangeSelectedRange()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(UITextBox, TextInput)
	
	UITextBox::UITextBox()
	{
	}
	
	UITextBox::~UITextBox()
	{
	}

}
