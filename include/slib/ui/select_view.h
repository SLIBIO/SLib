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

#ifndef CHECKHEADER_SLIB_UI_SELECT_VIEW
#define CHECKHEADER_SLIB_UI_SELECT_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{
	
	class ISelectViewInstance;

	class SLIB_EXPORT SelectView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		SelectView();
		
		~SelectView();

	public:
		sl_uint32 getItemsCount();
		
		virtual void setItemsCount(sl_uint32 n, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeAllItems(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		String getItemValue(sl_uint32 index);
		
		virtual void setItemValue(sl_uint32 index, const String& value);
		
		List<String> getValues();
		
		virtual void setValues(const List<String>& values);
		
		
		String getItemTitle(sl_uint32 index);
		
		virtual void setItemTitle(sl_uint32 index, const String& title, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		List<String> getTitles();
		
		virtual void setTitles(const List<String>& values, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		void addItem(const String& value, const String& title, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		virtual void selectIndex(sl_uint32 index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void selectValue(const String& value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_uint32 getSelectedIndex();
		
		String getSelectedValue();
		
		String getSelectedTitle();
		
		
		const UISize& getIconSize();
		
		virtual void setIconSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		void setIconSize(sl_ui_len size, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getIconWidth();
		
		void setIconWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		sl_ui_len getIconHeight();
		
		void setIconHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::UpdateLayout);
		
		
		Ref<Drawable> getLeftIcon();
		
		virtual void setLeftIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getRightIcon();
		
		virtual void setRightIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Alignment getGravity();
		
		virtual void setGravity(const Alignment& align, UIUpdateMode mode = UIUpdateMode::Redraw);

		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(SelectView, SelectItem, sl_uint32 index)		

	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onUpdateLayout() override;
		
	protected:
		UIRect getLeftIconRegion();
		
		UIRect getRightIconRegion();
		
	protected:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		virtual Ptr<ISelectViewInstance> getSelectViewInstance();
		
	protected:
		AtomicList<String> m_values;
		AtomicList<String> m_titles;
		sl_uint32 m_indexSelected;
		
		UISize m_iconSize;
		AtomicRef<Drawable> m_leftIcon;
		AtomicRef<Drawable> m_rightIcon;
		int m_clickedIconNo;
		
		Alignment m_gravity;
		Color m_textColor;
		
	};
	
	class SLIB_EXPORT ISelectViewInstance
	{
	public:
		virtual void select(SelectView* view, sl_uint32 index) = 0;
		
		virtual void refreshItemsCount(SelectView* view) = 0;
		
		virtual void refreshItemsContent(SelectView* view) = 0;
		
		virtual void setItemTitle(SelectView* view, sl_uint32 index, const String& title) = 0;
		
		virtual void setGravity(SelectView* view, const Alignment& gravity);
		
		virtual void setTextColor(SelectView* view, const Color& color);
		
		virtual sl_bool measureSize(SelectView* view, UISize& _out);
		
	};

}

#endif
