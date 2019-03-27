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

#include "slib/ui/chat_view.h"

#include "slib/ui/label_view.h"

#include "../resources.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatViewItem)
	
	ChatViewItem::ChatViewItem()
	 : flagMe(sl_false), time(Time::zero())
	{
	}
	
	SLIB_DEFINE_OBJECT(ChatView, ListView)
	
	ChatView::ChatView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		m_chatWidth = 0;
		m_chatWidthWeight = 0.6f;
		m_userIconSize = 0;
		m_userIconSizeWeight = 0.1f;
		
		m_backColorReceived = Color(220, 230, 240);
		m_textColorReceived = Color::Black;
		m_backColorSent = Color(200, 220, 250);
		m_textColorSent = Color::Black;
		m_textColorDate = Color(120, 120, 120);
	}
	
	ChatView::~ChatView()
	{
	}
	
#define CHAT_BODY_MARGIN_WEIGHT 0.4f
#define CHAT_MARGIN_WEIGHT 0.4f
#define CHAT_ROUND_WEIGHT 0.3f
#define CHAT_SPACE_WEIGHT 0.4f
#define USER_ICON_MARGIN_WEIGHT 0.3f
#define CHAT_DATE_HEIGHT_WEIGHT 1.5f
#define CHAT_TIME_HEIGHT_WEIGHT 1.5f

	struct _priv_ChatView_ItemViewParams
	{
		sl_ui_len userIconSize;
		sl_ui_len chatWidth;

		String formatDate;
		String formatTime;
		Color backColorReceived;
		Color textColorReceived;
		Color backColorSent;
		Color textColorSent;
		Color textColorDate;
	};
	
	class _priv_ChatViewItemView : public ViewGroup
	{
	public:
		_priv_ChatViewItemView()
		{
			SLIB_REFERABLE_CONSTRUCTOR
			
			setWidthFilling(1, UIUpdateMode::Init);
			setHeightWrapping(UIUpdateMode::Init);
			
			lblMessage = new LabelView;
			lblMessage->setAlignParentTop(UIUpdateMode::Init);
			lblMessage->setWidthWrapping(UIUpdateMode::Init);
			lblMessage->setHeightWrapping(UIUpdateMode::Init);
			lblMessage->setMultiLine(MultiLineMode::WordWrap, UIUpdateMode::Init);
			lblMessage->setGravity(Alignment::TopLeft, UIUpdateMode::Init);
			addChild(lblMessage, UIUpdateMode::Init);
		}
		
	public:
		sl_bool flagMe;
		Ref<Drawable> userIcon;
		String userName;
		sl_bool flagShowDate;
		Time m_time;
		
		_priv_ChatView_ItemViewParams params;

		Ref<LabelView> lblMessage;
		
	public:
		void onDraw(Canvas* canvas) override
		{
			Ref<Font> font = getFont();
			if (font.isNull()) {
				return;
			}
			sl_real fontSize = font->getSize();
			sl_ui_len width = getWidth();
			sl_ui_pos y = 0;
			if (flagShowDate) {
				sl_ui_pos h = (sl_ui_pos)(fontSize * CHAT_DATE_HEIGHT_WEIGHT);
				String strDate = getDateText();
				Size size = font->measureText(strDate);
				Ref<Pen> pen = Pen::createSolidPen(1, params.textColorDate);
				canvas->drawLine(0, (sl_real)(h/2), (sl_real)(width / 2 - size.x / 2 - fontSize), (sl_real)(h/2), pen);
				canvas->drawLine((sl_real)(width / 2 + size.x / 2 + fontSize), (sl_real)(h/2), (sl_real)width, (sl_real)(h/2), pen);
				canvas->drawText(strDate, (sl_real)(width / 2 - size.x / 2), (sl_real)(h / 2 - size.y / 2), font, params.textColorDate);
				y += h;
			}
			sl_ui_len marginBody = (sl_ui_len)(fontSize * CHAT_BODY_MARGIN_WEIGHT);
			sl_ui_len marginIcon = (sl_ui_len)(fontSize * USER_ICON_MARGIN_WEIGHT);
			sl_ui_len marginChat = (sl_ui_len)(fontSize * CHAT_MARGIN_WEIGHT);
			sl_ui_len chatSpace = (sl_ui_len)(fontSize * CHAT_SPACE_WEIGHT);
			if (userIcon.isNotNull()) {
				if (flagMe) {
					canvas->draw((sl_real)(width - marginIcon - params.userIconSize), (sl_real)(y + marginBody + marginIcon), (sl_real)(params.userIconSize), (sl_real)(params.userIconSize), userIcon);
				} else {
					canvas->draw((sl_real)marginIcon, (sl_real)(y + marginBody + marginIcon), (sl_real)(params.userIconSize), (sl_real)(params.userIconSize), userIcon);
				}
			}
			sl_ui_len heightTime = (sl_ui_len)(fontSize * CHAT_TIME_HEIGHT_WEIGHT);
			{
				String strTime = getTimeText();
				Size size = font->measureText(strTime);
				sl_ui_len l = marginIcon * 2 + params.userIconSize + chatSpace + marginChat;
				sl_ui_len m = y + heightTime / 2 - (sl_ui_len)(size.y / 2);
				if (flagMe) {
					canvas->drawText(strTime, (sl_real)(width - l - size.x), (sl_real)m, font, params.textColorDate);
				} else {
					canvas->drawText(strTime, (sl_real)(l), (sl_real)m, font, params.textColorDate);
				}
			}
			{
				sl_ui_len round = (sl_ui_len)(fontSize * CHAT_ROUND_WEIGHT);
				UIRect rect = lblMessage->getFrame();
				Point pts[3];
				sl_ui_len o = rect.top + round * 2 + chatSpace;
				if (flagMe) {
					canvas->fillRoundRect(rect, UISize(round, round), params.backColorSent);
					pts[0].x = (sl_real)(rect.right + chatSpace); pts[0].y = (sl_real)(o);
					pts[1].x = (sl_real)(rect.right); pts[1].y = (sl_real)(o - chatSpace);
					pts[2].x = (sl_real)(rect.right); pts[2].y = (sl_real)(o + chatSpace);
					canvas->fillPolygon(pts, 3, params.backColorSent);
				} else {
					canvas->fillRoundRect(rect, UISize(round, round), params.backColorReceived);
					pts[0].x = (sl_real)(rect.left - chatSpace); pts[0].y = (sl_real)(o);
					pts[1].x = (sl_real)(rect.left); pts[1].y = (sl_real)(o - chatSpace);
					pts[2].x = (sl_real)(rect.left); pts[2].y = (sl_real)(o + chatSpace);
					canvas->fillPolygon(pts, 3, params.backColorReceived);
				}
			}
		}
		
		void setData(const ChatViewItem& item)
		{
			flagMe = item.flagMe;
			userIcon = item.userIcon;
			userName = item.userName;
			m_time = item.time;
			lblMessage->setText(item.message, UIUpdateMode::None);
			lblMessage->setMaximumWidth(params.chatWidth, UIUpdateMode::None);
			sl_real fontSize = getFontSize();
			sl_ui_len marginChat = (sl_ui_len)(fontSize * CHAT_MARGIN_WEIGHT);
			sl_ui_len marginIcon = (sl_ui_len)(fontSize * USER_ICON_MARGIN_WEIGHT);
			sl_ui_len marginBody = (sl_ui_len)(fontSize * CHAT_BODY_MARGIN_WEIGHT);
			lblMessage->setPadding(marginChat, UIUpdateMode::None);
			lblMessage->setMarginBottom(marginBody, UIUpdateMode::None);
			sl_ui_len marginX = (sl_ui_len)(fontSize * CHAT_SPACE_WEIGHT) + 2 * marginIcon + params.userIconSize;
			sl_ui_len marginY = marginBody;
			marginBody *= 2;
			if (flagShowDate) {
				sl_ui_len m = (sl_ui_len)(fontSize * CHAT_DATE_HEIGHT_WEIGHT);
				marginY += m;
				marginBody += m;
			}
			setMinimumHeight(marginBody + marginIcon * 2 + params.userIconSize, UIUpdateMode::None);
			marginY += (sl_ui_len)(fontSize * CHAT_TIME_HEIGHT_WEIGHT);
			lblMessage->setMarginTop(marginY, UIUpdateMode::None);
			if (flagMe) {
				lblMessage->setTextColor(params.textColorSent, UIUpdateMode::Init);
				lblMessage->setMarginLeft(0, UIUpdateMode::None);
				lblMessage->setMarginRight(marginX, UIUpdateMode::None);
				lblMessage->setLeftFree(UIUpdateMode::None);
				lblMessage->setAlignParentRight();
			} else {
				lblMessage->setTextColor(params.textColorReceived, UIUpdateMode::Init);
				lblMessage->setMarginLeft(marginX, UIUpdateMode::None);
				lblMessage->setMarginRight(0, UIUpdateMode::None);
				lblMessage->setRightFree(UIUpdateMode::None);
				lblMessage->setAlignParentLeft();
			}
		}
		
		String getDateText()
		{
			if (params.formatDate.isNotEmpty()) {
				return m_time.format(params.formatDate);
			} else {
				Time today = Time::now().getDateOnly();
				Time date = m_time.getDateOnly();
				if (today == date) {
					return string::today::get();
				}
				today.addDays(-1);
				if (today == date) {
					return string::yesterday::get();
				}
				return m_time.format(TimeFormat::WeekdayDate);
			}
		}
		
		String getTimeText()
		{
			String s;
			if (params.formatTime.isNotEmpty()) {
				s = m_time.format(params.formatTime);
			} else {
				s = m_time.format(TimeFormat::HourMinute_12Hour);
			}
			if (userName.isNotEmpty()) {
				s = userName + ", " + s;
			}
			return s;
		}
		
	};
	
	class _priv_ChatViewAdapter : public ListViewAdapter<ChatViewItem, _priv_ChatViewItemView>
	{
	public:
		_priv_ChatView_ItemViewParams params;
		
	public:
		void onBindView(sl_uint64 _index, _priv_ChatViewItemView* view, View* parent) override
		{
			sl_size index = (sl_size)_index;
			ListLocker<ChatViewItem> list(m_list);
			if (index < list.count) {
				ChatViewItem& item = list[index];
				view->params = params;
				view->flagShowDate = index <= 0 || item.time.getDateOnly() != list[index - 1].time.getDateOnly();
				view->setData(item);
			}
		}
	};
	
	void ChatView::setItems(const List<ChatViewItem>& items, UIUpdateMode mode)
	{
		m_items = items;
		_updateListContent(mode);
	}
	
	void ChatView::addItem(const ChatViewItem& item, UIUpdateMode mode)
	{
		m_items.add(item);
		_updateListContent(mode);
	}
	
	void ChatView::addItems(const List<ChatViewItem>& items, UIUpdateMode mode)
	{
		m_items.addAll(items);
		_updateListContent(mode);
	}

	sl_ui_len ChatView::getChatWidth()
	{
		return m_chatWidth;
	}
	
	void ChatView::setChatWidth(sl_ui_len width, UIUpdateMode mode)
	{
		m_chatWidth = width;
		m_chatWidthWeight = 0;
		_updateListContent(mode);
	}
	
	sl_real ChatView::getChatWidthWeight()
	{
		return m_chatWidthWeight;
	}
	
	void ChatView::setChatWidthWeight(sl_real weight, UIUpdateMode mode)
	{
		m_chatWidthWeight = weight;
		if (weight > 0) {
			m_chatWidth = (sl_ui_len)(getWidth() * weight);
		}
		_updateListContent(mode);
	}
	
	sl_ui_len ChatView::getUserIconSize()
	{
		return m_userIconSize;
	}
	
	void ChatView::setUserIconSize(sl_ui_len width, UIUpdateMode mode)
	{
		m_userIconSize = width;
		m_userIconSizeWeight = 0;
		_updateListContent(mode);
	}
	
	sl_real ChatView::getUserIconSizeWeight()
	{
		return m_userIconSizeWeight;
	}
	
	void ChatView::setUserIconSizeWeight(sl_real weight, UIUpdateMode mode)
	{
		m_userIconSizeWeight = weight;
		if (weight > 0) {
			m_userIconSize = (sl_ui_len)(getWidth() * weight);
		}
		_updateListContent(mode);
	}
	
	String ChatView::getDateFormat()
	{
		return m_formatDate;
	}
	
	void ChatView::setDateFormat(const String& format, UIUpdateMode mode)
	{
		m_formatDate = format;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.formatDate = format;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.formatDate = format;
			}
		}
	}
	
	String ChatView::getTimeFormat()
	{
		return m_formatTime;
	}
	
	void ChatView::setTimeFormat(const String& format, UIUpdateMode mode)
	{
		m_formatTime = format;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.formatTime = format;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.formatTime = format;
			}
		}
	}
	
	Color ChatView::getReceivedChatBackColor()
	{
		return m_backColorReceived;
	}
	
	void ChatView::setReceivedChatBackColor(const Color& color, UIUpdateMode mode)
	{
		m_backColorReceived = color;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.backColorReceived = color;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.backColorReceived = color;
			}
		}
	}
	
	Color ChatView::getReceivedChatTextColor()
	{
		return m_textColorReceived;
	}
	
	void ChatView::setReceivedChatTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColorReceived = color;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.textColorReceived = color;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorReceived = color;
			}
		}
	}
	
	Color ChatView::getSentChatBackColor()
	{
		return m_backColorSent;
	}
	
	void ChatView::setSentChatBackColor(const Color& color, UIUpdateMode mode)
	{
		m_backColorSent = color;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.backColorSent = color;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.backColorSent = color;
			}
		}
	}
	
	Color ChatView::getSentChatTextColor()
	{
		return m_textColorSent;
	}
	
	void ChatView::setSentChatTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColorSent = color;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.textColorSent = color;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorSent = color;
			}
		}
	}
	
	Color ChatView::getDateTextColor()
	{
		return m_textColorDate;
	}
	
	void ChatView::setDateTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColorDate = color;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			ListElements< Ref<View> > views(getVisibleItemViews());
			for (sl_size i = 0; i < views.count; i++) {
				((_priv_ChatViewItemView*)(views[i].get()))->params.textColorDate = color;
				views[i]->invalidate();
			}
			Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorDate = color;
			}
		}
	}
	
	void ChatView::onResize(sl_ui_len width, sl_ui_len height)
	{
		sl_bool flagUpdateList = sl_false;
		{
			sl_real weight = m_chatWidthWeight;
			if (weight > 0) {
				m_chatWidth = (sl_ui_len)(width * weight);
				flagUpdateList = sl_true;
			}
		}
		{
			sl_real weight = m_userIconSizeWeight;
			if (weight > 0) {
				m_userIconSize = (sl_ui_len)(width * weight);
				flagUpdateList = sl_true;
			}
		}
		if (flagUpdateList) {
			_updateListContent(UIUpdateMode::UpdateLayout);
		}
		ViewGroup::onResize(width, height);
	}
	
	void ChatView::_updateListContent(UIUpdateMode mode)
	{
		Ref<_priv_ChatViewAdapter> adapter = new _priv_ChatViewAdapter;
		adapter->params.chatWidth = m_chatWidth;
		adapter->params.userIconSize = m_userIconSize;
		adapter->params.formatDate = m_formatDate;
		adapter->params.formatTime = m_formatTime;
		adapter->params.backColorReceived = m_backColorReceived;
		adapter->params.textColorReceived = m_textColorReceived;
		adapter->params.backColorSent = m_backColorSent;
		adapter->params.textColorSent = m_textColorSent;
		adapter->params.textColorDate = m_textColorDate;
		adapter->setList(m_items);
		setAdapter(adapter);
	}
	
}
