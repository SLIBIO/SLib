#include "../../../inc/slib/ui/linear_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LinearView, View)

LinearView::LinearView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setCreatingChildInstances(sl_true);
	setLayoutEnabled(sl_true, sl_false);
 	setOnMakeLayoutEnabled(sl_true, sl_false);
	
	m_orientation = LayoutOrientation::Vertical;
	
}

LayoutOrientation LinearView::getOrientation()
{
	return m_orientation;
}

void LinearView::setOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	if (m_orientation == orientation) {
		return;
	}
	m_orientation = orientation;
	requestLayout(sl_false);
	if (flagRedraw) {
		invalidate();
	}
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
	
	sl_real sizeSum = 0;
	sl_size countFullFill = 0;
	sl_size countPartFill = 0;
	
	ListLocker< Ref<View> > children(_getChildren());
	sl_size i;
	for (i = 0; i < children.count; i++) {
		Ref<View>& child = children[i];
		if (child.isNotNull()) {
			if (child->getVisibility() != Visibility::Gone) {
				if (flagHorizontal) {
					if (child->isLayoutLeftFixed() && child->isLayoutRightFixed()) {
						sizeSum += child->getMarginLeft();
						if (child->getWidthMode() != SizeMode::Filling) {
							Rectangle frame = child->getLayoutFrame();
							sizeSum += frame.getWidth();
						} else {
							if (Math::isAlmostZero(child->getWidthWeight() - 1)) {
								countFullFill++;
							} else {
								countPartFill++;
							}
						}
						sizeSum += child->getMarginRight();
					}
				} else {
					if (child->isLayoutTopFixed() && child->isLayoutBottomFixed()) {
						sizeSum += child->getMarginTop();
						if (child->getHeightMode() != SizeMode::Filling) {
							Rectangle frame = child->getLayoutFrame();
							sizeSum += frame.getHeight();
						} else {
							if (Math::isAlmostZero(child->getWidthWeight() - 1)) {
								countFullFill++;
							} else {
								countPartFill++;
							}
						}
						sizeSum += child->getMarginBottom();
					}
				}
			}
		}
	}
	
	if (countPartFill > 0) {
		sl_real remainedSize;
		if (flagHorizontal) {
			remainedSize = getWidth() - sizeSum;
		} else {
			remainedSize = getHeight() - sizeSum;
		}
		if (remainedSize < 0) {
			remainedSize = 0;
		}
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontal) {
						if (child->isLayoutLeftFixed() && child->isLayoutRightFixed()) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getWidthWeight();
								if (!(Math::isAlmostZero(weight - 1))) {
									sl_real width = remainedSize * weight;
									sizeSum += width;
									Rectangle frame = child->getLayoutFrame();
									frame.setWidth(width);
									child->setLayoutFrame(frame);
								}
							}
						}
					} else {
						if (child->isLayoutTopFixed() && child->isLayoutBottomFixed()) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getHeightWeight();
								if (!(Math::isAlmostZero(weight - 1))) {
									sl_real height = remainedSize * weight;
									sizeSum += height;
									Rectangle frame = child->getLayoutFrame();
									frame.setHeight(height);
									child->setLayoutFrame(frame);
								}
							}
						}
					}
				}
			}
		}
	}
	
	if (countFullFill > 0) {
		sl_real remainedSize;
		if (flagHorizontal) {
			remainedSize = getWidth() - sizeSum;
		} else {
			remainedSize = getHeight() - sizeSum;
		}
		if (remainedSize < 0) {
			remainedSize = 0;
		}
		sl_real sizeAvg = remainedSize / (sl_real)countFullFill;
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontal) {
						if (child->isLayoutLeftFixed() && child->isLayoutRightFixed()) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getWidthWeight();
								if (Math::isAlmostZero(weight - 1)) {
									Rectangle frame = child->getLayoutFrame();
									frame.setWidth(sizeAvg);
									child->setLayoutFrame(frame);
								}
							}
						}
					} else {
						if (child->isLayoutTopFixed() && child->isLayoutBottomFixed()) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getHeightWeight();
								if (Math::isAlmostZero(weight - 1)) {
									Rectangle frame = child->getLayoutFrame();
									frame.setHeight(sizeAvg);
									child->setLayoutFrame(frame);
								}
							}
						}
					}
				}
			}
		}
	}
	
	sl_real pos;
	if (flagHorizontal) {
		pos = getPaddingLeft();
	} else {
		pos = getPaddingTop();
	}
	for (i = 0; i < children.count; i++) {
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

VerticalLinearView::VerticalLinearView()
{
	setOrientation(LayoutOrientation::Vertical);
}

HorizontalLinearView::HorizontalLinearView()
{
	setOrientation(LayoutOrientation::Horizontal);
}

SLIB_UI_NAMESPACE_END
