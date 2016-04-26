#include "../../../inc/slib/ui/view_group.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ViewGroup, GenericViewWithDrawing)

ViewGroup::ViewGroup()
{
	setGroup(sl_true);
}

void ViewGroup::removeAllChildren()
{
	if (!(UI::isUiThread())) {
		UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ViewGroup, removeAllChildren, this));
		return;
	}
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		const Ref<View>& child = children[i];
		if (child.isNotNull()) {
			_removeChild(child);
		}
	}
	m_children.removeAll_NoLock();
}

void ViewGroup::removeChild(const Ref<View>& view)
{
	if (view.isNotNull()) {
		_removeChild(view);
		m_children.removeValue(view);
	}
}

void ViewGroup::addChild(const Ref<View>& view)
{
	if (view.isNull()) {
		return;
	}
	attachChild(view);
	view->setParent(this);
	m_children.addIfNotExist(view);
}

void ViewGroup::onAttach()
{
	ListLocker< Ref<View> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View> child = children[i];
		if (child.isNotNull()) {
			attachChild(child);
		}
	}
}

List< Ref<View> > ViewGroup::getChildren()
{
	return m_children.duplicate();
}

void ViewGroup::_removeChild(const Ref<View>& child)
{
	ObjectLocker lock(child.ptr);
	Ref<ViewInstance> instanceChild = child->getViewInstance();
	if (instanceChild.isNotNull()) {
		removeChildInstance(instanceChild.ptr);
	}
	child->detach();
	child->removeParent(this);
}

SLIB_UI_NAMESPACE_END
