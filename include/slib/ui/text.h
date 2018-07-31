/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
