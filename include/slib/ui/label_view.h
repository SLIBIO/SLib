/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		MultiLineMode getMultiLineMode();
		
		virtual void setMultiLineMode(MultiLineMode multiLineMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical, const UIRect& currentFrame) override;
		
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
		
		SimpleTextBox m_textBox;
		
	};

}

#endif
