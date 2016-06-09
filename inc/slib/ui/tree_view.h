#ifndef CHECKHEADER_SLIB_UI_TREE_VIEW
#define CHECKHEADER_SLIB_UI_TREE_VIEW

#include "definition.h"

#include "scroll_view.h"

#include "../core/variant.h"

SLIB_UI_NAMESPACE_BEGIN

class TreeView;
class TreeViewItem;

class SLIB_EXPORT ITreeViewListener
{
public:
	virtual void onSelectItem(TreeView* view, TreeViewItem* item);
	
};

class _TreeContentView;

class SLIB_EXPORT TreeViewItem : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	TreeViewItem();
	
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
	
	void addChild(const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> addChild(const String& text, const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> addChild(const String& text, sl_bool flagRedraw = sl_true);
	
	void insertChild(sl_size index, const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> insertChild(sl_size index, const String& text, const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> insertChild(sl_size index, const String& text, sl_bool flagRedraw = sl_true);
	
	void removeChild(sl_size index, sl_bool flagRedraw = sl_true);
	
	void removeChild(const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	void removeAllChildren(sl_bool flagRedraw = sl_true);
	
	sl_bool isRoot();
	
	sl_bool isLeaf();
	
	sl_bool isOpened();
	
	sl_bool isVisible();
	
	void open(sl_bool flagRedraw = sl_true);
	
	void close(sl_bool flagRedraw = sl_true);
	
	String getText();
	
	void setText(const String& text, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getIcon();
	
	void setIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getOpenedIcon();
	
	void setOpenedIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getClosedIcon();
	
	void setClosedIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<View> getCustomView();
	
	void setCustomView(const Ref<View>& view, sl_bool flagRedraw = sl_true);
	
	Color getTextColor();
	
	void setTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getHoverTextColor();
	
	void setHoverTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getSelectedTextColor();
	
	void setSelectedTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	sl_real getHeight();
	
	void setHeight(sl_real height, sl_bool flagRedraw = sl_true);

public:
	SLIB_REF_PROPERTY(Runnable, OnSelect)
	SLIB_REF_PROPERTY(Referable, UserObject)
	SLIB_VARIANT_PROPERTY(UserData)
	
private:
	void _addChild(TreeViewItem* item, sl_bool flagRedraw);
	
	void _removeChild(TreeViewItem* item);

	void _setTreeViewHierarchy(TreeView* view, sl_uint32 level);
	
	void _relayoutTree(sl_bool flagRedraw);
	
	void _redrawTree();
	
	Ref<_TreeContentView> _getContentView();
	
private:
	SafeString m_id;
	SafeWeakRef<TreeView> m_tree;
	SafeWeakRef<TreeViewItem> m_parent;
	sl_uint32 m_level;
	CList< Ref<TreeViewItem> > m_children;
	sl_bool m_flagOpened;
	SafeRef<Drawable> m_icon;
	SafeRef<Drawable> m_iconOpened;
	SafeRef<Drawable> m_iconClosed;
	SafeString m_text;
	SafeRef<View> m_customView;
	Color m_textColor;
	Color m_hoverTextColor;
	Color m_selectedTextColor;
	sl_real m_height;
	
	Rectangle m_frame;
	sl_real m_bottomChildren;
	SafeRef<Drawable> m_iconDrawing;
	
	friend class TreeView;
};


class SLIB_EXPORT TreeView : public ScrollView
{
	SLIB_DECLARE_OBJECT
	
public:
	TreeView();
	
public:
	Ref<TreeViewItem> getRootItem();
	
	Ref<TreeViewItem> getItemById(const String& _id);
	
	List< Ref<TreeViewItem> > getItems();
	
	sl_size getItemsCount();
	
	Ref<TreeViewItem> getItem(sl_size index);
	
	void addItem(const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> addItem(const String& text, const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> addItem(const String& text, sl_bool flagRedraw = sl_true);
	
	void insertItem(sl_size index, const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> insertItem(sl_size index, const String& text, const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<TreeViewItem> insertItem(sl_size index, const String& text, sl_bool flagRedraw = sl_true);
	
	void removeItem(sl_size index, sl_bool flagRedraw = sl_true);
	
	void removeItem(const Ref<TreeViewItem>& item, sl_bool flagRedraw = sl_true);
	
	void removeAllItems(sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getItemIcon();
	
	void setItemIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getOpenedItemIcon();
	
	void setOpenedItemIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getClosedItemIcon();
	
	void setClosedItemIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getCollapsedIcon();
	
	void setCollapsedIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);

	Ref<Drawable> getExpandedIcon();
	
	void setExpandedIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getDefaultCollapsedIcon();
	
	Ref<Drawable> getDefaultExpandedIcon();
	
	void applyDefaultIcons(sl_bool flagRedraw = sl_true);
	
	Color getSelectedItemBackgroundColor();
	
	void setSelectedItemBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getItemTextColor();
	
	void setItemTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getHoverItemTextColor();
	
	void setHoverItemTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Color getSelectedItemTextColor();
	
	void setSelectedItemTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	sl_real getItemHeight();
	
	void setItemHeight(sl_real height, sl_bool flagRedraw = sl_true);
	
	sl_real getItemPadding();
	
	void setItemPadding(sl_real padding, sl_bool flagRedraw = sl_true);
	
	sl_real getItemIndent();
	
	void setItemIndent(sl_real indent, sl_bool flagRedraw = sl_true);
	
	sl_real getTextIndent();
	
	void setTextIndent(sl_real indent, sl_bool flagRedraw = sl_true);
	
public:
	SLIB_PTR_PROPERTY(ITreeViewListener, Listener);
	
public:
	virtual void onSelectItem(TreeViewItem* item);
	
	// override
	virtual void onDraw(Canvas* canvas);
	
	// override
	virtual void onResize(sl_real width, sl_real height);
	
	// override
	virtual void onChangePadding();
	
protected:
	void dispatchSelectItem(TreeViewItem* item);
	
private:
	void _createRootItem();
	
	void _createContentView();
	
	void _relayoutContent(sl_bool flagRedraw);
	
	void _redrawContent();
	
	void _drawContent(Canvas* canvas);
	
	void _makeLayoutContent();
	
	void _makeLayoutItem(TreeViewItem* item, sl_real& top, sl_real left, sl_real right, sl_bool flagRoot);
	
	void _calcTextHeight(Canvas* canvas);
	
	void _drawItem(Canvas* canvas, TreeViewItem* item, sl_bool flagRoot);
	
	void _processMouseEvent(UIEvent* ev);
	
	void _processMouseEventItem(UIEvent* ev, sl_bool flagClick, TreeViewItem* item, sl_bool flagRoot);
	
	void _processClickItem(TreeViewItem* item);
	
private:
	Ref<_TreeContentView> m_content;
	sl_bool m_flagInvalidLayout;
	
	Ref<TreeViewItem> m_root;
	
	SafeRef<Drawable> m_itemIcon;
	SafeRef<Drawable> m_itemIconOpened;
	SafeRef<Drawable> m_itemIconClosed;
	SafeRef<Drawable> m_iconCollapsed;
	SafeRef<Drawable> m_iconExpanded;
	
	Color m_selectedItemBackgroundColor;
	Color m_itemTextColor;
	Color m_hoverItemTextColor;
	Color m_selectedItemTextColor;
	
	sl_real m_itemHeight;
	sl_real m_itemPadding;
	sl_real m_itemIndent;
	sl_real m_textIndent;
	
	sl_real m_layoutTextHeight;
	
	SafeRef<TreeViewItem> m_itemHover;
	SafeRef<TreeViewItem> m_itemSelected;
	
	sl_bool m_flagBeginTapping;
	Point m_pointBeginTapping;
	
	friend class TreeViewItem;
	friend class _TreeContentView;
};

SLIB_UI_NAMESPACE_END

#endif
