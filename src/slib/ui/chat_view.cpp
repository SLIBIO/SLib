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

#include "slib/ui/core.h"
#include "slib/ui/label_view.h"
#include "slib/ui/clipboard.h"

#include "../resources.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ChatViewItem)
	
	ChatViewItem::ChatViewItem()
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

	namespace priv
	{
		namespace chat_view
		{
			
			struct ChatItemViewParams
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
			
			class ChatItemView : public ViewGroup
			{
			public:
				ChatView* chatView;
				String itemId;
				sl_bool flagMe;
				Ref<Drawable> userIcon;
				String userName;
				sl_bool flagShowDate;
				String message;
				Time time;
				Ref<Font> font;
				
				ChatItemViewParams params;
				
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
					label->setDetectingHyperlinksInPlainText(sl_true, UIUpdateMode::Init);
					addChild(label, UIUpdateMode::Init);
					
					setOnTouchEvent(SLIB_FUNCTION_WEAKREF(ChatItemView, onTouchMessage, this));
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
							Color color = params.backColorSent;
							if (isPressedState()) {
								color.r = (sl_uint8)(color.r * 0.8);
								color.g = (sl_uint8)(color.g * 0.8);
								color.b = (sl_uint8)(color.b * 0.8);
							}
							canvas->fillPolygon(pts, 3, color);
							canvas->fillRoundRect(rect, UISize(round, round), color);
						} else {
							rect.left = l;
							rect.right = l + w;
							pts[0].x = (sl_real)(rect.left - layout.chatSpace); pts[0].y = (sl_real)(o);
							pts[1].x = (sl_real)(rect.left); pts[1].y = (sl_real)(o - layout.chatSpace);
							pts[2].x = (sl_real)(rect.left); pts[2].y = (sl_real)(o + layout.chatSpace);
							Color color = params.backColorReceived;
							if (isPressedState()) {
								color.r = (sl_uint8)(color.r * 0.8);
								color.g = (sl_uint8)(color.g * 0.8);
								color.b = (sl_uint8)(color.b * 0.8);
							}
							canvas->fillPolygon(pts, 3, color);
							canvas->fillRoundRect(rect, UISize(round, round), color);
						}
					}
				}
				
				static sl_ui_len measureHeight(const Ref<Font>& font, const String& message, sl_bool flagShowDate, sl_ui_len chatWidth, sl_ui_len userIconSize)
				{
					SimpleTextBoxParam tp;
					tp.font = font;
					tp.text = message;
					tp.width = (sl_real)chatWidth;
					SimpleTextBox box;
					box.update(tp);
					Layout layout(font, flagShowDate, chatWidth, userIconSize);
					sl_ui_len bottomUserIcon = layout.y + layout.marginIcon * 2 + userIconSize + layout.marginBottom;
					sl_ui_len bottomChat = layout.y + layout.heightTime + (sl_ui_len)(box.getContentHeight()) + 2 * layout.marginChat + layout.marginBottom;
					return Math::max(bottomUserIcon, bottomChat);
				}
				
				void setData(const ChatViewItem& item)
				{
					itemId = item.itemId;
					flagMe = item.flagMe;
					userIcon = item.userIcon;
					userName = item.userName;
					time = item.message.time;
					message = item.message.text;
					
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
							return ::slib::string::today::get();
						}
						today.addDays(-1);
						if (today == date) {
							return ::slib::string::yesterday::get();
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
				
				sl_bool m_flagTrySelect = sl_false;
				
				void onTouchMessage(View*, UIEvent* ev)
				{
					UIAction action = ev->getAction();
					if (action == UIAction::TouchBegin) {
						if (flagMe) {
							if (ev->getX() > label->getLeft()) {
								setPressedState(sl_true);
								invalidate();
							}
						} else {
							if (ev->getX() < label->getFrame().right) {
								setPressedState(sl_true);
								invalidate();
							}
						}
						m_flagTrySelect = sl_true;
						auto ref = ToRef(this);
						UIPoint pt = convertCoordinateToScreen(ev->getPoint());
						Dispatch::setTimeout([this, ref, pt]() {
							ObjectLocker lock(this);
							if (m_flagTrySelect && isPressedState()) {
								m_flagTrySelect = sl_false;
								popupMenu(pt);
							}
						}, 500);
					} else if (action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
						m_flagTrySelect = sl_false;
						setPressedState(sl_false);
						invalidate();
					}
					ev->preventDefault();
				}
				
				void popupMenu(const UIPoint& pt)
				{
					auto popup = menu::PopupMenuChatItem::get();
					auto chatView = this->chatView;
					String itemId = this->itemId;
					String text = this->message;
					popup->copy->setAction([text]() {
						Clipboard::setText(text);
					});
					popup->remove->setAction([chatView, itemId]() {
						chatView->_onRemoveItem(itemId);
					});
					popup->root->show(pt);
				}
				
			};
			
			class ChatAdapter : public ListViewAdapter<ChatViewItem, ChatItemView>
			{
			public:
				ChatView* chatView;
				ChatItemViewParams params;
				
			public:
				void onBindView(sl_uint64 _index, ChatItemView* view, View* parent) override
				{
					Ref<Font> font = parent->getFont();
					if (font.isNull()) {
						return;
					}
					sl_size index = (sl_size)_index;
					ListLocker<ChatViewItem> list(m_list);
					if (index < list.count) {
						ChatViewItem& item = list[index];
						view->chatView = chatView;
						view->font = font;
						view->params = params;
						view->flagShowDate = index <= 0 || item.message.time.getDateOnly() != list[index - 1].message.time.getDateOnly();
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
						sl_bool flagShowDate = index <= 0 || item.message.time.getDateOnly() != list[index - 1].message.time.getDateOnly();
						return ChatItemView::measureHeight(font, item.message.text, flagShowDate, params.chatWidth, params.userIconSize);
					}
					return 0;
				}
			};
			
		}
	}
	
	using namespace priv::chat_view;
	
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.formatDate = format;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.formatTime = format;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.backColorReceived = color;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorReceived = color;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.backColorSent = color;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorSent = color;
			}
			refreshItems();
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
			Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
			if (adapter.isNotNull()) {
				adapter->params.textColorDate = color;
			}
			refreshItems();
		}
	}
	
	void ChatView::setFrame(const UIRect& frame, UIUpdateMode mode)
	{
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			ListView::setFrame(frame, mode);
			return;
		}
		sl_bool flagEnd = sl_false;
		if (Math::abs(getScrollY() - getScrollRange().y) < 2) {
			flagEnd = sl_true;
		}
		ListView::setFrame(frame, mode);
		if (flagEnd) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), View, smoothScrollToEndY, this, UIUpdateMode::Redraw));
		}
	}
	
	void ChatView::setFont(const Ref<Font>& font, UIUpdateMode mode)
	{
		ListView::setFont(font, UIUpdateMode::None);
		dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ChatView, _updateListContent, this, UIUpdateMode::UpdateLayout));
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ChatView, DeleteItem, const String& itemId)
	
	void ChatView::dispatchDeleteItem(const String& itemId)
	{
		SLIB_INVOKE_EVENT_HANDLER(DeleteItem, itemId)
	}
	
	void ChatView::onResize(sl_ui_len width, sl_ui_len height)
	{
		ListView::onResize(width, height);
		sl_bool flagUpdateList = sl_false;
		{
			sl_real weight = m_chatWidthWeight;
			if (weight > 0) {
				sl_ui_len w = (sl_ui_len)(width * weight);
				if (m_chatWidth != w) {
					m_chatWidth = w;
					flagUpdateList = sl_true;
				}
			}
		}
		{
			sl_real weight = m_userIconSizeWeight;
			if (weight > 0) {
				sl_ui_len w = (sl_ui_len)(width * weight);
				if (m_userIconSize != w) {
					m_userIconSize = w;
					flagUpdateList = sl_true;
				}
			}
		}
		if (flagUpdateList) {
			dispatchToDrawingThread(SLIB_BIND_WEAKREF(void(), ChatView, _updateListContent, this, UIUpdateMode::UpdateLayout));
		}
	}
	
	void ChatView::_updateListContent(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			return;
		}
		if (getWidth() < 1 || getHeight() < 1) {
			return;
		}
		
		sl_bool flagInit = getAdapter().isNull();
		if (m_items.isEmpty()) {
			if (!flagInit) {
				setAdapter(sl_null);
			}
			return;
		}
		
		Ref<ChatAdapter> adapter = new ChatAdapter;
		adapter->chatView = this;
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
		ListViewSetAdapterParam param;
		param.adapter = adapter;
		param.flagScrollToLastItem = flagInit;
		setAdapter(param);
	}
	
	void ChatView::_addListContent(UIUpdateMode mode)
	{
		if (!SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode)) {
			return;
		}
		if (getWidth() < 1 || getHeight() < 1) {
			return;
		}
		
		if (m_items.isEmpty()) {
			return;
		}
		Ref<ChatAdapter> adapter = Ref<ChatAdapter>::from(getAdapter());
		if (adapter.isNotNull()) {
			ListViewRefreshParam param;
			param.flagScrollToLastItem = sl_true;
			refreshItems(param);
		} else {
			_updateListContent(mode);
		}
	}
	
	void ChatView::_onRemoveItem(const String& itemId)
	{
		ChatViewItem item;
		sl_size index = 0;
		sl_bool flagFound = sl_false;
		ListLocker<ChatViewItem> items(m_items);
		for (sl_size i = 0; i < items.count; i++) {
			if (items[i].itemId == itemId) {
				flagFound = sl_true;
				index = i;
				break;
			}
		}
		m_items.removeAt(index);
		dispatchDeleteItem(itemId);
		refreshItems();
	}
	
}
