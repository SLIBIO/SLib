#include "../../../inc/slib/ui/linear_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LinearView, View)

LinearView::LinearView(LayoutOrientation orientation) : m_orientation(orientation)
{
	setOnMakeLayoutEnabled(sl_true);
	setLayoutEnabled(sl_true);
}

LayoutOrientation LinearView::getOrientation()
{
	return m_orientation;
}

void LinearView::setOrientation(LayoutOrientation orientation)
{
	m_orientation = orientation;
	requestLayout();
	invalidate();
}

sl_bool LinearView::isHorizontal()
{
	return m_orientation == LayoutOrientation::Horizontal;
}

void LinearView::setHorizontal()
{
	setOrientation(LayoutOrientation::Horizontal);
}

sl_bool LinearView::isVertical()
{
	return m_orientation == LayoutOrientation::Vertical;
}

void LinearView::setVertical()
{
	setOrientation(LayoutOrientation::Vertical);
}

void LinearView::onResizeChild(View* child, sl_real width, sl_real height)
{
	if (!(child->isLayoutEnabled())) {
		requestLayout();
	}
}

void LinearView::onChangeVisibilityOfChild(View* child, Visibility oldVisibility, Visibility newVisibility)
{
	if (!(child->isLayoutEnabled())) {
		requestLayout();
	}
}

void LinearView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (m_orientation == LayoutOrientation::Horizontal) {
		flagVertical = sl_false;
	} else {
		flagHorizontal = sl_false;
	}
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	sl_real size = 0;
	ListLocker< Ref<View> > children(_getChildren());
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				child->measureLayout();
				if (flagHorizontal) {
					size += child->getMeasuredWidth();
				} else {
					size += child->getMeasuredHeight();
				}
			}
		}
	}
	if (flagHorizontal) {
		setMeasuredWidth(size + getPaddingLeft() + getPaddingRight());
	} else {
		setMeasuredHeight(size + getPaddingTop() + getPaddingBottom());
	}
}

void LinearView::onMakeLayout()
{
	sl_bool flagHorizontal = m_orientation == LayoutOrientation::Horizontal;
	sl_real pos;
	if (flagHorizontal) {
		pos = getPaddingLeft();
	} else {
		pos = getPaddingTop();
	}
	ListLocker< Ref<View> > children(_getChildren());
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				Rectangle frame = child->getLayoutFrame();
				if (flagHorizontal) {
					sl_real width = frame.getWidth();
					frame.left = pos;
					frame.right = pos + width;
					child->setLayoutFrame(frame);
					pos += width;
				} else {
					sl_real height = frame.getHeight();
					frame.top = pos;
					frame.bottom = pos + height;
					child->setLayoutFrame(frame);
					pos += height;
				}
			}
		}
	}
}

VerticalLinearView::VerticalLinearView() : LinearView(LayoutOrientation::Vertical)
{
}

HorizontalLinearView::HorizontalLinearView() : LinearView(LayoutOrientation::Horizontal)
{
}

SLIB_UI_NAMESPACE_END
