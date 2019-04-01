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
	
#define CHAT_MARGIN_WEIGHT 0.4f
#define CHAT_ROUND_WEIGHT 0.3f
#define CHAT_SPACE_WEIGHT 0.3f
#define USER_ICON_MARGIN_WEIGHT 0.3f
#define CHAT_DATE_HEIGHT_WEIGHT 2.0f
#define CHAT_TIME_HEIGHT_WEIGHT 1.3f
#define BOTTOM_MARGIN_WEIGHT 0.7f

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
		sl_bool flagMe;
		Ref<Drawable> userIcon;
		String userName;
		sl_bool flagShowDate;
		String message;
		Time time;
		Ref<Font> font;

		_priv_ChatView_ItemViewParams params;
		
		Ref<LabelView> label;
		
	public:
		void init() override
		{
			ViewGroup::init();
			
			setWidthFilling(1, UIUpdateMode::Init);
			
			label = new LabelView;
			label->setAlignParentTop(UIUpdateMode::Init);
			label->setWidthWrapping(UIUpdateMode::Init);
			label->setHeightWrapping(UIUpdateMode::Init);
			label->setMultiLine(MultiLineMode::WordWrap, UIUpdateMode::Init);
			label->setGravity(Alignment::TopLeft, UIUpdateMode::Init);
			addChild(label, UIUpdateMode::Init);
		}

	public:
		class Layout
		{
		public:
			sl_real fontSize;
			sl_ui_len marginChat;
			sl_ui_len marginIcon;
			sl_ui_len marginBottom;
			sl_ui_len heightDate;
			sl_ui_len heightTime;
			sl_ui_len chatSpace;
			sl_ui_len y;
			sl_ui_len xLabel;
			
			Layout(const Ref<Font>& font, sl_bool flagShowDate, sl_ui_len chatWidth, sl_ui_len userIconSize)
			{
				fontSize = font->getFontHeight();
				marginChat = (sl_ui_len)(fontSize * CHAT_MARGIN_WEIGHT);
				marginIcon = (sl_ui_len)(fontSize * USER_ICON_MARGIN_WEIGHT);
				marginBottom = (sl_ui_len)(fontSize * BOTTOM_MARGIN_WEIGHT);
				heightDate = (sl_ui_pos)(fontSize * CHAT_DATE_HEIGHT_WEIGHT);
				heightTime = (sl_ui_len)(fontSize * CHAT_TIME_HEIGHT_WEIGHT);
				chatSpace = (sl_ui_len)(fontSize * CHAT_SPACE_WEIGHT);
				y = 0;
				if (flagShowDate) {
					y += heightDate;
				}
				xLabel = marginIcon * 2 + userIconSize + chatSpace;
			}
		};
		
		void onDraw(Canvas* canvas) override
		{
			sl_ui_len width = getWidth();
			if (width < 1) {
				return;
			}
			if (getHeight() < 1) {
				return;
			}
			if (font.isNull()) {
				return;
			}

			Layout layout(font, flagShowDate, params.chatWidth, params.userIconSize);
			if (flagShowDate) {
				sl_ui_pos h = layout.heightDate;
				String strDate = getDateText();
				Size size = font->measureText(strDate);
				Ref<Pen> pen = Pen::createSolidPen(1, params.textColorDate);
				canvas->drawLine(0, (sl_real)(h/2), (sl_real)(width / 2 - size.x / 2 - layout.fontSize), (sl_real)(h/2), pen);
				canvas->drawLine((sl_real)(width / 2 + size.x / 2 + layout.fontSize), (sl_real)(h/2), (sl_real)width, (sl_real)(h/2), pen);
				canvas->drawText(strDate, (sl_real)(width / 2 - size.x / 2), (sl_real)(h / 2 - size.y / 2), font, params.textColorDate);
			}
			if (userIcon.isNotNull()) {
				sl_ui_len y = layout.y + layout.marginIcon;
				if (flagMe) {
					canvas->draw((sl_real)(width - layout.marginIcon - params.userIconSize), (sl_real)(y), (sl_real)(params.userIconSize), (sl_real)(params.userIconSize), userIcon);
				} else {
					canvas->draw((sl_real)(layout.marginIcon), (sl_real)(y), (sl_real)(params.userIconSize), (sl_real)(params.userIconSize), userIcon);
				}
			}
			{
				String strTime = getTimeText();
				Size size = font->measureText(strTime);
				sl_ui_len l = layout.marginIcon * 2 + params.userIconSize + layout.chatSpace + layout.marginChat;
				sl_ui_len m = layout.y + layout.heightTime / 2 - (sl_ui_len)(size.y / 2);
				if (flagMe) {
					canvas->drawText(strTime, (sl_real)(width - l - size.x), (sl_real)m, font, params.textColorDate);
				} else {
					canvas->drawText(strTime, (sl_real)(l), (sl_real)m, font, params.textColorDate);
				}
			}
			{
				sl_ui_len y = layout.y + layout.heightTime;
				sl_ui_len round = (sl_ui_len)(layout.fontSize * CHAT_ROUND_WEIGHT);
				Point pts[3];
				sl_ui_len o = y + round * 2 + layout.chatSpace;
				UIRect rect;
				rect.top = y;
				rect.bottom = y + label->getHeight();
				sl_ui_len w = label->getWidth();
				sl_ui_len l = layout.xLabel;
				if (flagMe) {
					rect.right = width - l;
					rect.left = rect.right - w;
					pts[0].x = (sl_real)(rect.right + layout.chatSpace); pts[0].y = (sl_real)(o);
					pts[1].x = (sl_real)(rect.right); pts[1].y = (sl_real)(o - layout.chatSpace);
					pts[2].x = (sl_real)(rect.right); pts[2].y = (sl_real)(o + layout.chatSpace);
					canvas->fillPolygon(pts, 3, params.backColorSent);
					canvas->fillRoundRect(rect, UISize(round, round), params.backColorSent);
				} else {
					rect.left = l;
					rect.right = l + w;
					pts[0].x = (sl_real)(rect.left - layout.chatSpace); pts[0].y = (sl_real)(o);
					pts[1].x = (sl_real)(rect.left); pts[1].y = (sl_real)(o - layout.chatSpace);
					pts[2].x = (sl_real)(rect.left); pts[2].y = (sl_real)(o + layout.chatSpace);
					canvas->fillPolygon(pts, 3, params.backColorReceived);
					canvas->fillRoundRect(rect, UISize(round, round), params.backColorReceived);
				}
			}
		}
		
		static sl_ui_len measureHeight(const Ref<Font>& font, const String& message, sl_bool flagShowDate, sl_ui_len chatWidth, sl_ui_len userIconSize)
		{
			SimpleTextBoxParam tp;
			tp.font = font;
			tp.text = message;
			tp.width = chatWidth;
			SimpleTextBox box;
			box.update(tp);
			Layout layout(font, flagShowDate, chatWidth, userIconSize);
			sl_ui_len bottomUserIcon = layout.y + layout.marginIcon * 2 + userIconSize + layout.marginBottom;
			sl_ui_len bottomChat = layout.y + layout.heightTime + box.getContentHeight() + 2 * layout.marginChat + layout.marginBottom;
			return Math::max(bottomUserIcon, bottomChat);
		}
		
		void setData(const ChatViewItem& item)
		{
			flagMe = item.flagMe;
			userIcon = item.userIcon;
			userName = item.userName;
			time = item.time;
			message = item.message;

			Layout layout(font, flagShowDate, params.chatWidth, params.userIconSize);

			label->setFont(font, UIUpdateMode::None);
			label->setText(message, UIUpdateMode::None);
			label->setPadding(layout.marginChat, UIUpdateMode::None);
			label->setMaximumWidth(params.chatWidth + layout.marginChat * 2, UIUpdateMode::None);
			label->setMarginTop(layout.y + layout.heightTime, UIUpdateMode::None);

			if (flagMe) {
				label->setTextColor(params.textColorSent, UIUpdateMode::None);
				label->setMarginLeft(0, UIUpdateMode::None);
				label->setMarginRight(layout.xLabel, UIUpdateMode::None);
				label->setLeftFree(UIUpdateMode::None);
				label->setAlignParentRight(UIUpdateMode::None);
			} else {
				label->setTextColor(params.textColorReceived, UIUpdateMode::None);
				label->setMarginLeft(layout.xLabel, UIUpdateMode::None);
				label->setMarginRight(0, UIUpdateMode::None);
				label->setRightFree(UIUpdateMode::None);
				label->setAlignParentLeft(UIUpdateMode::None);
			}
			UISize sizeChat = label->measureSize();
			sl_ui_len bottomUserIcon = layout.y + layout.marginIcon * 2 + params.userIconSize + layout.marginBottom;
			sl_ui_len bottomChat = layout.y + layout.heightTime + sizeChat.y + layout.marginBottom;
			setHeight(Math::max(bottomUserIcon, bottomChat), UIUpdateMode::None);
		}
		
		String getDateText()
		{
			if (params.formatDate.isNotEmpty()) {
				return time.format(params.formatDate);
			} else {
				Time today = Time::now().getDateOnly();
				Time date = time.getDateOnly();
				if (today == date) {
					return string::today::get();
				}
				today.addDays(-1);
				if (today == date) {
					return string::yesterday::get();
				}
				return time.format(TimeFormat::WeekdayDate);
			}
		}
		
		String getTimeText()
		{
			String s;
			if (params.formatTime.isNotEmpty()) {
				s = time.format(params.formatTime);
			} else {
				s = time.format(TimeFormat::HourMinute_12Hour);
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
			Ref<Font> font = parent->getFont();
			if (font.isNull()) {
				return;
			}
			sl_size index = (sl_size)_index;
			ListLocker<ChatViewItem> list(m_list);
			if (index < list.count) {
				ChatViewItem& item = list[index];
				view->font = font;
				view->params = params;
				view->flagShowDate = index <= 0 || item.time.getDateOnly() != list[index - 1].time.getDateOnly();
				view->setData(item);
			}
		}
		
		sl_ui_len getItemHeight(sl_uint64 _index, View* parent) override
		{
			Ref<Font> font = parent->getFont();
			if (font.isNull()) {
				return 0;
			}
			sl_size index = (sl_size)_index;
			ListLocker<ChatViewItem> list(m_list);
			if (index < list.count) {
				ChatViewItem& item = list[index];
				sl_bool flagShowDate = index <= 0 || item.time.getDateOnly() != list[index - 1].time.getDateOnly();
				return _priv_ChatViewItemView::measureHeight(font, item.message, flagShowDate, params.chatWidth, params.userIconSize);
			}
			return 0;
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
		_addListContent(mode);
	}
	
	void ChatView::addItems(const List<ChatViewItem>& items, UIUpdateMode mode)
	{
		m_items.addAll(items);
		_addListContent(mode);
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
	
	void ChatView::setFont(const Ref<Font>& font, UIUpdateMode mode)
	{
		View::setFont(font, UIUpdateMode::None);
		_updateListContent(UIUpdateMode::UpdateLayout);
	}
	
	void ChatView::onResize(sl_ui_len width, sl_ui_len height)
	{
		ViewGroup::onResize(width, height);
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
	}
	
	void ChatView::_updateListContent(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			return;
		}
		sl_bool flagInit = getAdapter().isNull();
		if (m_items.isEmpty()) {
			if (!flagInit) {
				setAdapter(sl_null);
			}
			return;
		}
		
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
		
		if (!flagInit) {
			return;
		}
		auto weak = ToWeakRef(this);
		dispatchToDrawingThread([weak]() {
			auto ref = ToRef(weak);
			if (ref.isNull()) {
				return;
			}
			ref->scrollToEndY();
			ref->dispatchToDrawingThread([weak]() {
				auto ref = ToRef(weak);
				if (ref.isNull()) {
					return;
				}
				ref->scrollToEndY();
				ref->dispatchToDrawingThread([weak]() {
					auto ref = ToRef(weak);
					if (ref.isNull()) {
						return;
					}
					ref->scrollToEndY();
				}, 50);
			}, 50);
		}, 50);
	}
	
	void ChatView::_addListContent(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			return;
		}
		if (m_items.isEmpty()) {
			return;
		}
		Ref<_priv_ChatViewAdapter> adapter = Ref<_priv_ChatViewAdapter>::from(getAdapter());
		if (adapter.isNotNull()) {
			adapter->setList(m_items);
			refreshItems();
			auto weak = ToWeakRef(this);
			dispatchToDrawingThread([weak]() {
				auto ref = ToRef(weak);
				if (ref.isNull()) {
					return;
				}
				ref->smoothScrollToEndY();
				ref->dispatchToDrawingThread([weak]() {
					auto ref = ToRef(weak);
					if (ref.isNull()) {
						return;
					}
					ref->smoothScrollToEndY();
					ref->dispatchToDrawingThread([weak]() {
						auto ref = ToRef(weak);
						if (ref.isNull()) {
							return;
						}
						ref->smoothScrollToEndY();
					}, 50);
				}, 50);
			}, 50);
		} else {
			_updateListContent(mode);
		}
	}
	
}
