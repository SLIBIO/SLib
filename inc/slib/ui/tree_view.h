#ifndef CHECKHEADER_SLIB_UI_TREE_VIEW
#define CHECKHEADER_SLIB_UI_TREE_VIEW

#include "definition.h"

#include "scroll_view.h"

#include "../core/variant.h"

namespace slib
{

	class TreeView;
	class TreeViewItem;
	
	class SLIB_EXPORT ITreeViewListener
	{
	public:
		ITreeViewListener();

		virtual ~ITreeViewListener();

	public:
		virtual void onSelectItem(TreeView* view, TreeViewItem* item);
		
	};
	
	class _TreeContentView;
	
	class SLIB_EXPORT TreeViewItem : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TreeViewItem();
		
		~TreeViewItem();

	public:
		String getId();
		
		void setId(const String& _id);
		
		Ref<TreeView> getTreeView();
		
		Ref<TreeViewItem> getParent();
		
		sl_uint32 getLevel();
		
		Ref<TreeViewItem> getItemById(const String& _id);
		
		List< Ref<TreeViewItem> > getChildren();
		
		sl_size getChildrenCount();
		
		Ref<TreeViewItem> getChild(sl_size index);
		
		void addChild(const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> addChild(const String& text, const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> addChild(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void insertChild(sl_size index, const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> insertChild(sl_size index, const String& text, const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> insertChild(sl_size index, const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeChild(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeChild(const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeAllChildren(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isRoot();
		
		sl_bool isLeaf();
		
		sl_bool isOpened();
		
		sl_bool isVisible();
		
		void open(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void close(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		String getText();
		
		void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getIcon();
		
		void setIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getOpenedIcon();
		
		void setOpenedIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getClosedIcon();
		
		void setClosedIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<View> getCustomView();
		
		void setCustomView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getHoverTextColor();
		
		void setHoverTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getSelectedTextColor();
		
		void setSelectedTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getHeight();
		
		void setHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		SLIB_PROPERTY(AtomicFunction<void(TreeViewItem*)>, OnSelect)
		SLIB_PROPERTY(AtomicRef<Referable>, UserObject)
		SLIB_PROPERTY(AtomicVariant, UserData)
		
	private:
		void _addChild(TreeViewItem* item, UIUpdateMode mode);
		
		void _removeChild(TreeViewItem* item);
		
		void _setTreeViewHierarchy(TreeView* view, sl_uint32 level);
		
		void _relayoutTree(UIUpdateMode mode);
		
		void _redrawTree();
		
		Ref<_TreeContentView> _getContentView();
		
	private:
		AtomicString m_id;
		AtomicWeakRef<TreeView> m_tree;
		AtomicWeakRef<TreeViewItem> m_parent;
		sl_uint32 m_level;
		CList< Ref<TreeViewItem> > m_children;
		sl_bool m_flagOpened;
		AtomicRef<Drawable> m_icon;
		AtomicRef<Drawable> m_iconOpened;
		AtomicRef<Drawable> m_iconClosed;
		AtomicString m_text;
		AtomicRef<View> m_customView;
		Color m_textColor;
		Color m_hoverTextColor;
		Color m_selectedTextColor;
		sl_ui_len m_height;
		
		UIRect m_frame;
		sl_ui_pos m_bottomChildren;
		AtomicRef<Drawable> m_iconDrawing;
		
		friend class TreeView;
	};
	
	
	class SLIB_EXPORT TreeView : public ScrollView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TreeView();
		
		~TreeView();

	public:
		Ref<TreeViewItem> getRootItem();
		
		Ref<TreeViewItem> getItemById(const String& _id);
		
		List< Ref<TreeViewItem> > getItems();
		
		sl_size getItemsCount();
		
		Ref<TreeViewItem> getItem(sl_size index);
		
		void addItem(const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> addItem(const String& text, const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> addItem(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void insertItem(sl_size index, const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> insertItem(sl_size index, const String& text, const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<TreeViewItem> insertItem(sl_size index, const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeItem(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeItem(const Ref<TreeViewItem>& item, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void removeAllItems(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getItemIcon();
		
		void setItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getOpenedItemIcon();
		
		void setOpenedItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getClosedItemIcon();
		
		void setClosedItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getCollapsedIcon();
		
		void setCollapsedIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getExpandedIcon();
		
		void setExpandedIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getSelectedItemBackgroundColor();
		
		void setSelectedItemBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getItemTextColor();
		
		void setItemTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getHoverItemTextColor();
		
		void setHoverItemTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getSelectedItemTextColor();
		
		void setSelectedItemTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getItemHeight();
		
		void setItemHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getItemPadding();
		
		void setItemPadding(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getItemIndent();
		
		void setItemIndent(sl_ui_pos indent, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTextIndent();
		
		void setTextIndent(sl_ui_pos indent, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		SLIB_PROPERTY(AtomicPtr<ITreeViewListener>, Listener);
		
		SLIB_PROPERTY(AtomicFunction<void(TreeView*, TreeViewItem*)>, OnSelectItem)
		
	public:
		virtual void onSelectItem(TreeViewItem* item);
		
		// override
		virtual void onDraw(Canvas* canvas);
		
		// override
		virtual void onResize(sl_ui_len width, sl_ui_len height);
		
		// override
		virtual void onChangePadding();
		
	protected:
		virtual void dispatchSelectItem(TreeViewItem* item);
		
	private:
		void _createRootItem();
		
		void _createContentView();
		
		void _relayoutContent(UIUpdateMode mode);
		
		void _redrawContent();
		
		void _drawContent(Canvas* canvas);
		
		void _makeLayoutContent();
		
		void _makeLayoutItem(TreeViewItem* item, sl_ui_pos& top, sl_ui_pos left, sl_ui_pos right, sl_bool flagRoot);
		
		void _calcTextHeight(Canvas* canvas);
		
		void _drawItem(Canvas* canvas, TreeViewItem* item, sl_bool flagRoot);
		
		void _processMouseEvent(UIEvent* ev);
		
		void _processMouseEventItem(UIEvent* ev, sl_bool flagClick, TreeViewItem* item, sl_bool flagRoot);
		
		void _processClickItem(TreeViewItem* item);
		
	private:
		Ref<_TreeContentView> m_content;
		sl_bool m_flagInvalidLayout;
		
		Ref<TreeViewItem> m_root;
		
		AtomicRef<Drawable> m_itemIcon;
		AtomicRef<Drawable> m_itemIconOpened;
		AtomicRef<Drawable> m_itemIconClosed;
		AtomicRef<Drawable> m_iconCollapsed;
		AtomicRef<Drawable> m_iconExpanded;
		
		Color m_selectedItemBackgroundColor;
		Color m_itemTextColor;
		Color m_hoverItemTextColor;
		Color m_selectedItemTextColor;
		
		sl_ui_len m_itemHeight;
		sl_ui_pos m_itemPadding;
		sl_ui_pos m_itemIndent;
		sl_ui_pos m_textIndent;
		
		sl_ui_len m_layoutTextHeight;
		
		AtomicRef<TreeViewItem> m_itemHover;
		AtomicRef<TreeViewItem> m_itemSelected;
		
		sl_bool m_flagBeginTapping;
		UIPointf m_pointBeginTapping;
		
		friend class TreeViewItem;
		friend class _TreeContentView;
	};

}

#endif
