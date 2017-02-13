#ifndef CHECKHEADER_SLIB_UI_SELECT_VIEW
#define CHECKHEADER_SLIB_UI_SELECT_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{
	class SelectView;
	
	class SLIB_EXPORT ISelectViewListener
	{
	public:
		virtual void onSelectItem(SelectView* view, sl_uint32 index) = 0;
		
	};
	
	class SLIB_EXPORT SelectView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		SelectView();
		
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
		
		
		virtual void selectIndex(sl_uint32 index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		virtual void selectValue(const String& value, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_uint32 getSelectedIndex();
		
		String getSelectedValue();
		
		String getSelectedTitle();
		
		
		const UISize& getIconSize();
		
		virtual void setIconSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconWidth();
		
		void setIconWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconHeight();
		
		void setIconHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getLeftIcon();
		
		virtual void setLeftIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getRightIcon();
		
		virtual void setRightIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
	public:
		SLIB_PROPERTY(AtomicPtr<ISelectViewListener>, Listener)
		
		SLIB_PROPERTY(AtomicFunction<void(SelectView*, sl_uint32)>, OnSelectItem)
		
	protected:
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onMouseEvent(UIEvent* ev);
		
		// override
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
		
		virtual void onSelectItem(sl_uint32 index);
		
	protected:
		UIRect getLeftIconRegion();
		
		UIRect getRightIconRegion();
		
	public:
		// override
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
		
		virtual void dispatchSelectItem(sl_uint32 index);
		
	private:
		void _getSelectedIndex_NW();
		
		void _select_NW(sl_uint32 index);
		
		void _refreshItemsCount_NW();
		
		void _refreshItemsContent_NW();
		
		void _setItemTitle_NW(sl_uint32 index, const String& title);
		
		// override
		void _setFont_NW(const Ref<Font>& font);
		
	protected:
		AtomicList<String> m_values;
		AtomicList<String> m_titles;
		sl_uint32 m_indexSelected;
		
		UISize m_iconSize;
		AtomicRef<Drawable> m_leftIcon;
		AtomicRef<Drawable> m_rightIcon;
		int m_clickedIconNo;
		
		Color m_textColor;
		
	};	
}

#endif
