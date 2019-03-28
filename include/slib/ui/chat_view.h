/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_UI_CHAT_VIEW
#define CHECKHEADER_SLIB_UI_CHAT_VIEW

#include "definition.h"

#include "list_view.h"

namespace slib
{
	
	class SLIB_EXPORT ChatViewItem
	{
	public:
		Ref<Drawable> userIcon;
		String userName;
		sl_bool flagMe;
		String message;
		Time time;
	
	public:
		ChatViewItem();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChatViewItem)
		
	};
	
	class SLIB_EXPORT ChatView : public ListView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ChatView();
		
		~ChatView();
		
	public:
		void setItems(const List<ChatViewItem>& items, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void addItem(const ChatViewItem& item, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void addItems(const List<ChatViewItem>& items, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getChatWidth();
		
		void setChatWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getChatWidthWeight();
		
		void setChatWidthWeight(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getUserIconSize();
		
		void setUserIconSize(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_real getUserIconSizeWeight();
		
		void setUserIconSizeWeight(sl_real weight, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		String getDateFormat();
		
		void setDateFormat(const String& format, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getTimeFormat();
		
		void setTimeFormat(const String& format, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getReceivedChatBackColor();
		
		void setReceivedChatBackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getReceivedChatTextColor();
		
		void setReceivedChatTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Color getSentChatBackColor();
		
		void setSentChatBackColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Color getSentChatTextColor();
		
		void setSentChatTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);

		Color getDateTextColor();
		
		void setDateTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setFont(const Ref<Font>& font, UIUpdateMode mode = UIUpdateMode::UpdateLayout) override;

	protected:
		void onResize(sl_ui_len width, sl_ui_len height) override;
		
	protected:
		void _updateListContent(UIUpdateMode mode);
		
	protected:
		AtomicList<ChatViewItem> m_items;
		sl_ui_len m_chatWidth;
		sl_real m_chatWidthWeight;
		sl_ui_len m_userIconSize;
		sl_real m_userIconSizeWeight;
		
		String m_formatDate;
		String m_formatTime;
		Color m_backColorReceived;
		Color m_textColorReceived;
		Color m_backColorSent;
		Color m_textColorSent;
		Color m_textColorDate;

	};

}

#endif
