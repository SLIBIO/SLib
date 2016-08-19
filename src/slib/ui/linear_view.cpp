#include "../../../inc/slib/ui/linear_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LinearView, View)

LinearView::LinearView(LayoutOrientation orientation) : m_orientation(orientation)
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setCreatingChildInstances(sl_true);
	setLayoutEnabled(sl_true, sl_false);
 	setOnMakeLayoutEnabled(sl_true, sl_false);
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
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	sl_real measuredWidth = 0;
	sl_real measuredHeight = 0;
	sl_real paddingWidth = getPaddingLeft() + getPaddingRight();
	sl_real paddingHeight = getPaddingTop() + getPaddingBottom();
	if (m_orientation == LayoutOrientation::Horizontal) {
		measuredWidth = paddingWidth;
	} else {
		measuredHeight = paddingHeight;
	}

	ListLocker< Ref<View> > children(_getChildren());
	for (sl_size i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				child->measureLayout();
				if (m_orientation == LayoutOrientation::Horizontal) {
					if (flagHorizontal) {
						if (child->isLayoutLeftFixed() && child->isLayoutRightFixed()) {
							measuredWidth += child->getMeasuredWidth();
							if (!(child->isRelativeMarginLeft())) {
								measuredWidth += child->getMarginLeft();
							}
							if (!(child->isRelativeMarginRight())) {
								measuredWidth += child->getMarginRight();
							}
						}
					}
					if (flagVertical) {
						sl_real h = child->getMeasuredOuterBoundHeight();
						if (!(child->isLayoutTopFixed() && child->isLayoutBottomFixed())) {
							h += paddingHeight;
						}
						if (h > measuredHeight) {
							measuredHeight = h;
						}
					}
				} else {
					if (flagVertical) {
						if (child->isLayoutTopFixed() && child->isLayoutBottomFixed()) {
							measuredHeight += child->getMeasuredHeight();
							if (!(child->isRelativeMarginTop())) {
								measuredHeight += child->getMarginTop();
							}
							if (!(child->isRelativeMarginBottom())) {
								measuredHeight += child->getMarginBottom();
							}
						}
					}
					if (flagHorizontal) {
						sl_real w = child->getMeasuredOuterBoundWidth();
						if (!(child->isLayoutLeftFixed() && child->isLayoutRightFixed())) {
							w += paddingWidth;
						}
						if (w > measuredWidth) {
							measuredWidth = w;
						}
					}
				}
			}
		}
	}
	if (flagHorizontal) {
		setMeasuredWidth(measuredWidth);
	}
	if (flagVertical) {
		setMeasuredHeight(measuredHeight);
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
				if (flagHorizontal) {
					if (child->isLayoutLeftFixed() && child->isLayoutRightFixed()) {
						Rectangle frame = child->getLayoutFrame();
						sl_real width = frame.getWidth();
						pos += child->getMarginLeft();
						frame.left = pos;
						frame.right = pos + width;
						child->setLayoutFrame(frame);
						pos += width;
						pos += child->getMarginRight();
					}
				} else {
					if (child->isLayoutTopFixed() && child->isLayoutBottomFixed()) {
						Rectangle frame = child->getLayoutFrame();
						sl_real height = frame.getHeight();
						pos += child->getMarginTop();
						frame.top = pos;
						frame.bottom = pos + height;
						child->setLayoutFrame(frame);
						pos += height;
						pos += child->getMarginBottom();
					}
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
