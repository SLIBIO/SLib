/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
