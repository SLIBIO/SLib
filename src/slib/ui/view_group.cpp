#include "../../../inc/slib/ui/view_group.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN
ViewGroup::ViewGroup()
{
	setGroup(sl_true);
}

void ViewGroup::removeAllChildren()
{
	if (!(UI::isUIThread())) {
		UI::runOnUIThread(SLIB_CALLBACK_WEAKREF(ViewGroup, removeAllChildren, this));
		return;
	}
	ListLocker< Ref<View> > children(m_children.duplicate());
	for (sl_size i = 0; i < children.getCount(); i++) {
		Ref<View> child = children[i];
		if (child.isNotNull()) {
			removeChild(child.get());
		}
	}
}

void ViewGroup::removeChild(const Ref<View>& _view)
{
	Ref<View> view = _view;
	if (view.isNotNull()) {
		MutexLocker lock(view->getLocker());
		Ref<ViewInstance> instanceChild = view->getViewInstance();
		if (instanceChild.isNotNull()) {
			removeChildInstance(instanceChild.get());
		}
		m_children.removeValue(view);
		view->detach();
		view->removeParent(this);
	}
}

void ViewGroup::addChild(const Ref<View>& _view)
{
	Ref<View> view = _view;
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
	for (sl_size i = 0; i < children.getCount(); i++) {
		Ref<View> child = children[i];
		if (child.isNotNull()) {
			attachChild(child);
		}
	}
}

const List< Ref<View> >& ViewGroup::getChildren()
{
	return m_children;
}
SLIB_UI_NAMESPACE_END
