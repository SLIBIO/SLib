#include "../../../inc/slib/ui/tree_view.h"

SLIB_UI_NAMESPACE_BEGIN

void ITreeViewListener::onSelectItem(TreeView* view, TreeViewItem* item)
{
}

class _TreeContentView : public View
{
public:
	SafeWeakRef<TreeView> m_tree;
	
public:
	_TreeContentView()
	{
		setFocusable(sl_true);
	}
	
	// override
	void onDraw(Canvas* canvas)
	{
		Ref<TreeView> tree = m_tree;
		if (tree.isNotNull()) {
			tree->_drawContent(canvas);
		}
	}
	
	// override
	void onMouseEvent(UIEvent* ev)
	{
		Ref<TreeView> tree = m_tree;
		if (tree.isNotNull()) {
			tree->_processMouseEvent(ev);
		}
	}
};

SLIB_DEFINE_OBJECT(TreeViewItem, Object)

TreeViewItem::TreeViewItem()
{
	m_level = 0;
	m_flagOpened = sl_false;
	m_textColor = Color::zero();
	m_hoverTextColor = Color::zero();
	m_selectedTextColor = Color::zero();
	m_height = 0;
	
	m_frame.setZero();
}

String TreeViewItem::getId()
{
	return m_id;
}

void TreeViewItem::setId(const String& _id)
{
	m_id = _id;
}

Ref<TreeView> TreeViewItem::getTreeView()
{
	return m_tree;
}

Ref<TreeViewItem> TreeViewItem::getParent()
{
	return m_parent;
}

sl_uint32 TreeViewItem::getLevel()
{
	return m_level;
}

Ref<TreeViewItem> TreeViewItem::getItemById(const String& _id)
{
	if (m_id == _id) {
		return this;
	}
	ListLocker< Ref<TreeViewItem> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		Ref<TreeViewItem> item = children[i];
		if (item.isNotNull()) {
			item = item->getItemById(_id);
			if (item.isNotNull()) {
				return item;
			}
		}
	}
	return Ref<TreeViewItem>::null();
}

List< Ref<TreeViewItem> > TreeViewItem::getChildren()
{
	return m_children.duplicate();
}

sl_size TreeViewItem::getChildrenCount()
{
	return m_children.getCount();
}

Ref<TreeViewItem> TreeViewItem::getChild(sl_size index)
{
	return m_children.getItemValue(index, Ref<TreeViewItem>::null());
}

void TreeViewItem::addChild(const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	if (item.isNull()) {
		return;
	}
	if (m_children.add(item)) {
		_addChild(item.ptr, mode);
	}
}

Ref<TreeViewItem> TreeViewItem::addChild(const String& text, const Ref<Drawable>& icon, UIUpdateMode mode)
{
	Ref<TreeViewItem> item = new TreeViewItem;
	if (item.isNotNull()) {
		item->setText(text);
		item->setIcon(icon);
		addChild(item, mode);
		return item;
	}
	return Ref<TreeViewItem>::null();
}

Ref<TreeViewItem> TreeViewItem::addChild(const String& text, UIUpdateMode mode)
{
	return addChild(text, Ref<Drawable>::null(), mode);
}

void TreeViewItem::insertChild(sl_size index, const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	if (item.isNull()) {
		return;
	}
	if (m_children.insert(index, item)) {
		_addChild(item.ptr, mode);
	}
}

Ref<TreeViewItem> TreeViewItem::insertChild(sl_size index, const String& text, const Ref<Drawable>& icon, UIUpdateMode mode)
{
	Ref<TreeViewItem> item = new TreeViewItem;
	if (item.isNotNull()) {
		item->setText(text);
		item->setIcon(icon);
		insertChild(index, item, mode);
		return item;
	}
	return Ref<TreeViewItem>::null();
}

Ref<TreeViewItem> TreeViewItem::insertChild(sl_size index, const String& text, UIUpdateMode mode)
{
	return insertChild(index, text, Ref<Drawable>::null(), mode);
}

void TreeViewItem::removeChild(sl_size index, UIUpdateMode mode)
{
	Ref<TreeViewItem> item = m_children.getItemValue(index, Ref<TreeViewItem>::null());
	if (item.isNull()) {
		return;
	}
	_removeChild(item.ptr);
	m_children.remove(index);
	if (isVisible()) {
		_relayoutTree(mode);
	}
}

void TreeViewItem::removeChild(const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	if (item.isNull()) {
		return;
	}
	_removeChild(item.ptr);
	m_children.removeValue(item);
	if (isVisible()) {
		_relayoutTree(mode);
	}
}

void TreeViewItem::removeAllChildren(UIUpdateMode mode)
{
	ListLocker< Ref<TreeViewItem> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		_removeChild(children[i].ptr);
	}
	m_children.removeAll();
	if (isVisible()) {
		_relayoutTree(mode);
	}
}

sl_bool TreeViewItem::isRoot()
{
	return m_parent.isNull();
}

sl_bool TreeViewItem::isLeaf()
{
	return m_children.getCount() == 0;
}

sl_bool TreeViewItem::isOpened()
{
	return m_flagOpened;
}

sl_bool TreeViewItem::isVisible()
{
	Ref<TreeView> tree = m_tree;
	if (tree.isNull()) {
		return sl_false;
	}
	Ref<TreeViewItem> parent = m_parent;
	if (parent.isNull()) {
		return sl_true;
	}
	if (parent->m_flagOpened) {
		return parent->isVisible();
	} else {
		return sl_false;
	}
}

void TreeViewItem::open(UIUpdateMode mode)
{
	m_flagOpened = sl_true;
	if (m_children.getCount() != 0) {
		_relayoutTree(mode);
	}
}

void TreeViewItem::close(UIUpdateMode mode)
{
	m_flagOpened = sl_false;
	if (m_children.getCount() != 0) {
		_relayoutTree(mode);
	}
}

String TreeViewItem::getText()
{
	return m_text;
}

void TreeViewItem::setText(const String& text, UIUpdateMode mode)
{
	m_text = text;
	if (mode == UIUpdateMode::Redraw) {
		_redrawTree();
	}
}

Ref<Drawable> TreeViewItem::getIcon()
{
	return m_icon;
}

void TreeViewItem::setIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_icon = icon;
	_relayoutTree(mode);
}

Ref<Drawable> TreeViewItem::getOpenedIcon()
{
	return m_iconOpened;
}

void TreeViewItem::setOpenedIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_iconOpened = icon;
	_relayoutTree(mode);
}

Ref<Drawable> TreeViewItem::getClosedIcon()
{
	return m_iconClosed;
}

void TreeViewItem::setClosedIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_iconClosed = icon;
	_relayoutTree(mode);
}

Ref<View> TreeViewItem::getCustomView()
{
	return m_customView;
}

void TreeViewItem::setCustomView(const Ref<View>& view, UIUpdateMode mode)
{
	Ref<View> old = m_customView;
	m_customView = view;
	Ref<_TreeContentView> content = _getContentView();
	if (content.isNotNull()) {
		if (old.isNotNull()) {
			content->removeChild(old);
		}
		if (view.isNotNull()) {
			view->setParent(content);
		}
	}
	_relayoutTree(mode);
}

Color TreeViewItem::getTextColor()
{
	return m_textColor;
}

void TreeViewItem::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawTree();
	}
}

Color TreeViewItem::getHoverTextColor()
{
	return m_hoverTextColor;
}

void TreeViewItem::setHoverTextColor(const Color& color, UIUpdateMode mode)
{
	m_hoverTextColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawTree();
	}
}

Color TreeViewItem::getSelectedTextColor()
{
	return m_selectedTextColor;
}

void TreeViewItem::setSelectedTextColor(const Color& color, UIUpdateMode mode)
{
	m_selectedTextColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawTree();
	}
}

sl_ui_len TreeViewItem::getHeight()
{
	return m_height;
}

void TreeViewItem::setHeight(sl_ui_len height, UIUpdateMode mode)
{
	m_height = height;
	_relayoutTree(mode);
}

void TreeViewItem::_addChild(TreeViewItem* item, UIUpdateMode mode)
{
	item->m_parent = this;
	Ref<TreeView> tree = m_tree;
	if (tree.isNotNull()) {
		item->_setTreeViewHierarchy(tree.ptr, m_level + 1);
	}
	if (isVisible()) {
		_relayoutTree(mode);
	}
}

void TreeViewItem::_removeChild(TreeViewItem* item)
{
	item->m_parent.setNull();
	item->m_tree.setNull();
}

void TreeViewItem::_setTreeViewHierarchy(TreeView* view, sl_uint32 level)
{
	m_level = level;
	m_tree = view;
	ListLocker< Ref<TreeViewItem> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		const Ref<TreeViewItem>& child = children[i];
		if (child.isNotNull()) {
			child->_setTreeViewHierarchy(view, level + 1);
		}
	}
}

void TreeViewItem::_relayoutTree(UIUpdateMode mode)
{
	Ref<TreeView> tree = m_tree;
	if (tree.isNotNull()) {
		tree->_relayoutContent(mode);
	}
}

void TreeViewItem::_redrawTree()
{
	Ref<TreeView> tree = m_tree;
	if (tree.isNotNull()) {
		tree->_redrawContent();
	}
}

Ref<_TreeContentView> TreeViewItem::_getContentView()
{
	Ref<TreeView> tree = getTreeView();
	if (tree.isNotNull()) {
		return tree->m_content;
	}
	return Ref<_TreeContentView>::null();
}

class _TreeView_DefaultIdentIcon : public Drawable
{
public:
	Ref<Brush> m_brush;
	Point m_pts[3];
	
public:
	_TreeView_DefaultIdentIcon(sl_bool flagCollapse)
	{
		m_brush = Brush::createSolidBrush(Color(50, 50, 50));
		if (flagCollapse) {
			m_pts[0] = Point(0.33f, 0.34f);
			m_pts[1] = Point(0.67f, 0.51f);
			m_pts[2] = Point(0.33f, 0.68f);
		} else {
			m_pts[0] = Point(0.3f, 0.35f);
			m_pts[1] = Point(0.5f, 0.65f);
			m_pts[2] = Point(0.7f, 0.35f);
		}
	}
	
public:
	// override
	sl_real getDrawableWidth()
	{
		return 16;
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return 16;
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		if (m_brush.isNotNull()) {
			Point pts[3];
			for (int i = 0; i < 3; i++) {
				pts[i].x = rectDst.left + rectDst.getWidth() * m_pts[i].x;
				pts[i].y = rectDst.top + rectDst.getHeight() * m_pts[i].y;
			}
			canvas->fillPolygon(pts, 3, m_brush);
		}
	}
	
};

SLIB_DEFINE_OBJECT(TreeView, ScrollView)

TreeView::TreeView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	ScrollView::setCreatingNativeWidget(sl_false);
	setUsingFont(sl_true);
	ScrollView::setOccurringClick(sl_false);
	
	_createRootItem();
	_createContentView();
	
	m_flagInvalidLayout = sl_true;
	
	m_selectedItemBackgroundColor = Color(0, 0, 0, 50);
	m_itemTextColor = Color::Black;
	m_hoverItemTextColor = Color(0, 0, 200);
	m_selectedItemTextColor = Color(0, 0, 200);
	
	m_itemHeight = 0;
	
	ScrollView::setPadding(6, 6, 6, 6, UIUpdateMode::Init);
	m_itemPadding = 8;
	m_itemIndent = 16;
	m_textIndent = 4;
	
	m_iconCollapsed = new _TreeView_DefaultIdentIcon(sl_true);
	m_iconExpanded = new _TreeView_DefaultIdentIcon(sl_false);
}

Ref<TreeViewItem> TreeView::getRootItem()
{
	return m_root;
}

Ref<TreeViewItem> TreeView::getItemById(const String& _id)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->getItemById(_id);
	}
	return Ref<TreeViewItem>::null();
}

List< Ref<TreeViewItem> > TreeView::getItems()
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->getChildren();
	}
	return List< Ref<TreeViewItem> >::null();
}

sl_size TreeView::getItemsCount()
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->getChildrenCount();
	}
	return 0;
}

Ref<TreeViewItem> TreeView::getItem(sl_size index)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->getChild(index);
	}
	return Ref<TreeViewItem>::null();
}

void TreeView::addItem(const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		root->addChild(item, mode);
	}
}

Ref<TreeViewItem> TreeView::addItem(const String& text, const Ref<Drawable>& icon, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->addChild(text, icon, mode);
	}
	return Ref<TreeViewItem>::null();
}

Ref<TreeViewItem> TreeView::addItem(const String& text, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->addChild(text, mode);
	}
	return Ref<TreeViewItem>::null();
}

void TreeView::insertItem(sl_size index, const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		root->insertChild(index, item, mode);
	}
}

Ref<TreeViewItem> TreeView::insertItem(sl_size index, const String& text, const Ref<Drawable>& icon, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->insertChild(index, text, icon, mode);
	}
	return Ref<TreeViewItem>::null();
}

Ref<TreeViewItem> TreeView::insertItem(sl_size index, const String& text, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		return root->insertChild(index, text, mode);
	}
	return Ref<TreeViewItem>::null();
}

void TreeView::removeItem(sl_size index, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		root->removeChild(index, mode);
	}
}

void TreeView::removeItem(const Ref<TreeViewItem>& item, UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		root->removeChild(item, mode);
	}
}

void TreeView::removeAllItems(UIUpdateMode mode)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		root->removeAllChildren(mode);
	}
}

Ref<Drawable> TreeView::getItemIcon()
{
	return m_itemIcon;
}

void TreeView::setItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_itemIcon = icon;
	_relayoutContent(mode);
}

Ref<Drawable> TreeView::getOpenedItemIcon()
{
	return m_itemIconOpened;
}

void TreeView::setOpenedItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_itemIconOpened = icon;
	_relayoutContent(mode);
}

Ref<Drawable> TreeView::getClosedItemIcon()
{
	return m_itemIconClosed;
}

void TreeView::setClosedItemIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_itemIconClosed = icon;
	_relayoutContent(mode);
}

Ref<Drawable> TreeView::getCollapsedIcon()
{
	return m_iconCollapsed;
}

void TreeView::setCollapsedIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_iconCollapsed = icon;
	_relayoutContent(mode);
}

Ref<Drawable> TreeView::getExpandedIcon()
{
	return m_iconExpanded;
}

void TreeView::setExpandedIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_iconExpanded = icon;
	_relayoutContent(mode);
}

Color TreeView::getSelectedItemBackgroundColor()
{
	return m_selectedItemBackgroundColor;
}

void TreeView::setSelectedItemBackgroundColor(const Color& color, UIUpdateMode mode)
{
	m_selectedItemBackgroundColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawContent();
	}
}

Color TreeView::getItemTextColor()
{
	return m_itemTextColor;
}

void TreeView::setItemTextColor(const Color& color, UIUpdateMode mode)
{
	m_itemTextColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawContent();
	}
}

Color TreeView::getHoverItemTextColor()
{
	return m_hoverItemTextColor;
}

void TreeView::setHoverItemTextColor(const Color& color, UIUpdateMode mode)
{
	m_hoverItemTextColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawContent();
	}
}

Color TreeView::getSelectedItemTextColor()
{
	return m_selectedItemTextColor;
}

void TreeView::setSelectedItemTextColor(const Color& color, UIUpdateMode mode)
{
	m_selectedItemTextColor = color;
	if (mode == UIUpdateMode::Redraw) {
		_redrawContent();
	}
}

sl_ui_len TreeView::getItemHeight()
{
	return m_itemHeight;
}

void TreeView::setItemHeight(sl_ui_len height, UIUpdateMode mode)
{
	m_itemHeight = height;
	_relayoutContent(mode);
}

sl_ui_pos TreeView::getItemPadding()
{
	return m_itemPadding;
}

void TreeView::setItemPadding(sl_ui_pos padding, UIUpdateMode mode)
{
	m_itemPadding = padding;
	_relayoutContent(mode);
}

sl_ui_pos TreeView::getItemIndent()
{
	return m_itemIndent;
}

void TreeView::setItemIndent(sl_ui_pos indent, UIUpdateMode mode)
{
	m_itemIndent = indent;
	_relayoutContent(mode);
}

sl_ui_pos TreeView::getTextIndent()
{
	return m_textIndent;
}

void TreeView::setTextIndent(sl_ui_pos indent, UIUpdateMode mode)
{
	m_textIndent = indent;
	if (mode == UIUpdateMode::Redraw) {
		_redrawContent();
	}
}

void TreeView::onSelectItem(TreeViewItem* item)
{
}

void TreeView::onDraw(Canvas* canvas)
{
	_calcTextHeight(canvas);
	_makeLayoutContent();
}

void TreeView::onResize(sl_ui_len width, sl_ui_len height)
{
	ScrollView::onResize(width, height);
	Ref<_TreeContentView> content = m_content;
	if (content.isNotNull()) {
		content->setWidth(width, UIUpdateMode::NoRedraw);
		_relayoutContent(UIUpdateMode::NoRedraw);
	}
}

void TreeView::onChangePadding()
{
	_relayoutContent(UIUpdateMode::NoRedraw);
}

void TreeView::dispatchSelectItem(TreeViewItem* item)
{
	onSelectItem(item);
	PtrLocker<ITreeViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectItem(this, item);
	}
	Ref<Runnable> onSelect = item->getOnSelect();
	if (onSelect.isNotNull()) {
		onSelect->run();
	}
}

void TreeView::_createRootItem()
{
	Ref<TreeViewItem> item = new TreeViewItem;
	if (item.isNotNull()) {
		item->m_tree = this;
		item->m_flagOpened = sl_true;
		m_root = item;
	}
}

void TreeView::_createContentView()
{
	Ref<_TreeContentView> view = new _TreeContentView;
	if (view.isNotNull()) {
		view->m_tree = this;
		m_content = view;
		setContentView(view);
	}
}

void TreeView::_relayoutContent(UIUpdateMode mode)
{
	Ref<_TreeContentView> content = m_content;
	if (content.isNotNull()) {
		m_flagInvalidLayout = sl_true;
		if (mode == UIUpdateMode::Redraw) {
			content->invalidate();
		}
	}
}

void TreeView::_redrawContent()
{
	Ref<_TreeContentView> view = m_content;
	if (view.isNotNull()) {
		view->invalidate();
	}
}

void TreeView::_drawContent(Canvas* canvas)
{
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		_drawItem(canvas, root.ptr, sl_true);
	}
}

void TreeView::_makeLayoutContent()
{
	if (!m_flagInvalidLayout) {
		return;
	}
	m_flagInvalidLayout = sl_false;
	
	Ref<TreeViewItem> root = m_root;
	if (root.isNotNull()) {
		sl_ui_pos top = getPaddingTop();
		sl_ui_pos left = getPaddingLeft();
		sl_ui_pos right = getWidth() - getPaddingRight();
		if (m_iconCollapsed.isNotNull() || m_iconExpanded.isNotNull()) {
			left += m_itemIndent;
		}
		_makeLayoutItem(root.ptr, top, left, right, sl_true);
		top += getPaddingBottom();
		if (top < 0) {
			top = 0;
		}
		Ref<_TreeContentView> content = m_content;
		if (content.isNotNull()) {
			content->setHeight(top, UIUpdateMode::NoRedraw);
		}
	}
	
}

void TreeView::_makeLayoutItem(TreeViewItem* item, sl_ui_pos& top, sl_ui_pos left, sl_ui_pos right, sl_bool flagRoot)
{
	if (!flagRoot) {
		Ref<Drawable> iconDraw;
		if (item->m_children.getCount() == 0) {
			iconDraw = item->m_icon;
			if (iconDraw.isNull()) {
				iconDraw = m_itemIcon;
			}
		} else {
			if (item->m_flagOpened) {
				iconDraw = item->m_iconOpened;
				if (iconDraw.isNull()) {
					iconDraw = item->m_icon;
					if (iconDraw.isNull()) {
						iconDraw = m_itemIconOpened;
						if (iconDraw.isNull()) {
							iconDraw = m_itemIcon;
						}
					}
				}
			} else {
				iconDraw = item->m_iconClosed;
				if (iconDraw.isNull()) {
					iconDraw = item->m_icon;
					if (iconDraw.isNull()) {
						iconDraw = m_itemIconOpened;
						if (iconDraw.isNull()) {
							iconDraw = m_itemIcon;
						}
					}
				}
			}
		}
		sl_ui_pos height = item->m_height;
		if (height == 0) {
			height = m_itemHeight;
			if (height == 0) {
				if (iconDraw.isNotNull()) {
					height = (sl_ui_pos)(iconDraw->getDrawableHeight());
				}
				if (height < (sl_ui_pos)m_layoutTextHeight) {
					height = m_layoutTextHeight;
				}
				height += m_itemPadding;
			}
		}
		item->m_iconDrawing = iconDraw;
		item->m_frame.left = left;
		item->m_frame.right = right;
		item->m_frame.top = top;
		item->m_frame.bottom = top + height;
		top = item->m_frame.bottom;
		left += m_itemIndent;
	}
	if (item->m_flagOpened) {
		ListLocker< Ref<TreeViewItem> > children(item->m_children);
		for (sl_size i = 0; i < children.count; i++) {
			const Ref<TreeViewItem>& child = children[i];
			if (child.isNotNull()) {
				_makeLayoutItem(child.ptr, top, left, right, sl_false);
			}
		}
	}
	item->m_bottomChildren = top;
}

void TreeView::_calcTextHeight(Canvas* canvas)
{
	sl_ui_pos height = (sl_ui_pos)(canvas->getTextSize(getFont(), "|").y);
	if (height < 0) {
		height = 0;
	}
	m_layoutTextHeight = height;
}

void TreeView::_drawItem(Canvas* canvas, TreeViewItem* item, sl_bool flagRoot)
{
	if (!flagRoot) {
		sl_ui_pos left = item->m_frame.left;
		sl_ui_pos right = item->m_frame.right;
		sl_ui_pos top = item->m_frame.top;
		sl_ui_pos bottom = item->m_frame.bottom;
		if (item == m_itemSelected) {
			Color backColor = m_selectedItemBackgroundColor;
			if (backColor.a != 0) {
				canvas->fillRectangle(UIRect(0, top, getWidth(), bottom), Brush::createSolidBrush(backColor));
			}
		}
		if (item->m_children.getCount() > 0) {
			if (item->m_flagOpened) {
				Ref<Drawable> icon = m_iconExpanded;
				if (icon.isNotNull()) {
					canvas->draw(UIRect(left - m_itemIndent, top, left, bottom), icon, ScaleMode::None, Alignment::MiddleCenter);
				}
			} else {
				Ref<Drawable> icon = m_iconCollapsed;
				if (icon.isNotNull()) {
					canvas->draw(UIRect(left - m_itemIndent, top, left, bottom), icon, ScaleMode::None, Alignment::MiddleCenter);
				}
			}
		}
		Ref<Drawable> icon = item->m_iconDrawing;
		if (icon.isNotNull()) {
			sl_ui_pos iconWidth = (sl_ui_pos)(icon->getDrawableWidth());
			canvas->draw(UIRect(left, top, left + iconWidth, bottom), icon, ScaleMode::None, Alignment::MiddleCenter);
			left += iconWidth;
			left += m_textIndent;
		}
		String text = item->m_text;
		if (text.isNotEmpty()) {
			Color colorText;
			if (item == m_itemHover) {
				colorText = item->m_hoverTextColor;
				if (colorText.isZero()) {
					colorText = item->m_textColor;
					if (colorText.isZero()) {
						colorText = m_hoverItemTextColor;
					}
				}
			} else if (item == m_itemSelected) {
				colorText = item->m_selectedTextColor;
				if (colorText.isZero()) {
					colorText = item->m_textColor;
					if (colorText.isZero()) {
						colorText = m_hoverItemTextColor;
					}
				}
			} else {
				colorText = item->m_textColor;
				if (colorText.isZero()) {
					colorText = m_itemTextColor;
				}
			}
			canvas->drawText(text, UIRect(left, top, right, bottom), getFont(), colorText, Alignment::MiddleLeft);
		}
	}
	if (item->m_flagOpened) {
		ListLocker< Ref<TreeViewItem> > children(item->m_children);
		for (sl_size i = 0; i < children.count; i++) {
			const Ref<TreeViewItem>& child = children[i];
			if (child.isNotNull()) {
				_drawItem(canvas, child.ptr, sl_false);
			}
		}
	}
}

void TreeView::_processMouseEvent(UIEvent* ev)
{
	UIAction action = ev->getAction();
	if (action == UIAction::MouseLeave) {
		m_itemHover.setNull();
		return;
	}
	if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
		Ref<_TreeContentView> content = m_content;
		if (content.isNotNull()) {
			m_pointBeginTapping = content->convertCoordinateToParent(ev->getPoint());
			m_flagBeginTapping = sl_true;
		}
	} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd) {
		if (m_flagBeginTapping) {
			Ref<_TreeContentView> content = m_content;
			if (content.isNotNull()) {
				if (content->convertCoordinateToParent(ev->getPoint()).getLength2p(m_pointBeginTapping) < 25) {
					Ref<TreeViewItem> root = m_root;
					if (root.isNotNull()) {
						_processMouseEventItem(ev, sl_true, root.ptr, sl_true);
					}
				}
			}
		}
	} else if (action == UIAction::MouseMove) {
		if (m_flagBeginTapping) {
			Ref<_TreeContentView> content = m_content;
			if (content.isNotNull()) {
				if (content->convertCoordinateToParent(ev->getPoint()).getLength2p(m_pointBeginTapping) > 25) {
					m_flagBeginTapping = sl_false;
				}
			}
		}
		Ref<TreeViewItem> root = m_root;
		if (root.isNotNull()) {
			_processMouseEventItem(ev, sl_false, root.ptr, sl_true);
		}
	}
}

void TreeView::_processMouseEventItem(UIEvent* ev, sl_bool flagClick, TreeViewItem* item, sl_bool flagRoot)
{
	sl_ui_pos y = (sl_ui_pos)(ev->getY());
	UIAction action = ev->getAction();
	if (!flagRoot) {
		if (item->m_frame.top <= y && y < item->m_frame.bottom) {
			if (flagClick) {
				m_itemSelected = item;
				_processClickItem(item);
			} else {
				if (action == UIAction::MouseMove) {
					if (m_itemHover != item) {
						m_itemHover = item;
						_redrawContent();
					}
				}
			}
			return;
		}
	}
	if (item->m_flagOpened) {
		ListLocker< Ref<TreeViewItem> > children(item->m_children);
		for (sl_size i = 0; i < children.count; i++) {
			const Ref<TreeViewItem>& child = children[i];
			if (child.isNotNull()) {
				if (child->m_frame.top <= y && y < child->m_bottomChildren) {
					_processMouseEventItem(ev, flagClick, child.ptr, sl_false);
					return;
				}
			}
		}
	}
}

void TreeView::_processClickItem(TreeViewItem *item)
{
	if (item->isOpened()) {
		item->close();
	} else {
		item->open();
	}
	dispatchSelectItem(item);
}

SLIB_UI_NAMESPACE_END
