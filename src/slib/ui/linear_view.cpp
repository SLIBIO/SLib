#include "../../../inc/slib/ui/linear_view.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(LinearView, ViewGroup)
	
	LinearView::LinearView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setOnMakeLayoutEnabled(sl_true, UIUpdateMode::Init);
		
		m_orientation = LayoutOrientation::Vertical;
		
	}
	
	LayoutOrientation LinearView::getOrientation()
	{
		return m_orientation;
	}
	
	void LinearView::setOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		if (m_orientation == orientation) {
			return;
		}
		m_orientation = orientation;
		invalidateLayoutFromResizeContent(mode);
	}
	
	sl_bool LinearView::isHorizontal()
	{
		return m_orientation == LayoutOrientation::Horizontal;
	}
	
	void LinearView::setHorizontal(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Horizontal, mode);
	}
	
	sl_bool LinearView::isVertical()
	{
		return m_orientation == LayoutOrientation::Vertical;
	}
	
	void LinearView::setVertical(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Vertical, mode);
	}
	
	void LinearView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		sl_ui_pos measuredWidth = 0;
		sl_ui_pos measuredHeight = 0;
		sl_ui_pos paddingWidth = getPaddingLeft() + getPaddingRight();
		sl_ui_pos paddingHeight = getPaddingTop() + getPaddingBottom();
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
							measuredWidth += child->getMeasuredWidth();
							if (!(child->isRelativeMarginLeft())) {
								measuredWidth += child->getMarginLeft();
							}
							if (!(child->isRelativeMarginRight())) {
								measuredWidth += child->getMarginRight();
							}
						}
					} else {
						if (flagVertical) {
							measuredHeight += child->getMeasuredHeight();
							if (!(child->isRelativeMarginTop())) {
								measuredHeight += child->getMarginTop();
							}
							if (!(child->isRelativeMarginBottom())) {
								measuredHeight += child->getMarginBottom();
							}
						}
					}
				}
			}
		}
		
		if (m_orientation == LayoutOrientation::Horizontal) {
			if (flagHorizontal) {
				if (measuredWidth < 0) {
					measuredWidth = 0;
				}
				setMeasuredWidth(measuredWidth);
			}
			if (flagVertical) {
				measureRelativeLayout(sl_false, sl_true);
			}
		} else {
			if (flagHorizontal) {
				measureRelativeLayout(sl_true, sl_false);
			}
			if (flagVertical) {
				if (measuredHeight < 0) {
					measuredHeight = 0;
				}
				setMeasuredHeight(measuredHeight);
			}
		}
	}
	
	void LinearView::onMakeLayout()
	{
		sl_bool flagHorizontal = m_orientation == LayoutOrientation::Horizontal;
		
		sl_ui_pos sizeSum = 0;
		sl_uint32 countFullFill = 0;
		sl_uint32 countPartFill = 0;
		
		ListLocker< Ref<View> > children(_getChildren());
		sl_size i;
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontal) {
						sizeSum += child->getMarginLeft();
						if (child->getWidthMode() != SizeMode::Filling) {
							UIRect frame = child->getLayoutFrame();
							sizeSum += frame.getWidth();
						} else {
							if (Math::isAlmostZero(child->getWidthWeight() - 1)) {
								countFullFill++;
							} else {
								countPartFill++;
							}
						}
						sizeSum += child->getMarginRight();
					} else {
						sizeSum += child->getMarginTop();
						if (child->getHeightMode() != SizeMode::Filling) {
							UIRect frame = child->getLayoutFrame();
							sizeSum += frame.getHeight();
						} else {
							if (Math::isAlmostZero(child->getHeightWeight() - 1)) {
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
		
		if (sizeSum < 0) {
			sizeSum = 0;
		}
		
		if (countPartFill > 0) {
			sl_ui_pos remainedSize;
			if (flagHorizontal) {
				sl_ui_len n = getWidth();
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			} else {
				sl_ui_len n = getHeight();
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			}
			if (remainedSize < 0) {
				remainedSize = 0;
			}
			for (i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				if (child.isNotNull()) {
					if (child->getVisibility() != Visibility::Gone) {
						if (flagHorizontal) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getWidthWeight();
								if (!(Math::isAlmostZero(weight - 1))) {
									sl_ui_pos width = (sl_ui_pos)((sl_real)(remainedSize) * weight);
									sizeSum += width;
									UIRect frame = child->getLayoutFrame();
									frame.setWidth(width);
									child->setLayoutFrame(frame);
								}
							}
						} else {
							if (child->getHeightMode() == SizeMode::Filling) {
								sl_real weight = child->getHeightWeight();
								if (!(Math::isAlmostZero(weight - 1))) {
									sl_ui_pos height = (sl_ui_pos)((sl_real)(remainedSize) * weight);
									sizeSum += height;
									UIRect frame = child->getLayoutFrame();
									frame.setHeight(height);
									child->setLayoutFrame(frame);
								}
							}
						}
					}
				}
			}
		}
		
		if (sizeSum < 0) {
			sizeSum = 0;
		}
		
		if (countFullFill > 0) {
			sl_ui_pos remainedSize;
			if (flagHorizontal) {
				sl_ui_len n = getWidth();
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			} else {
				sl_ui_len n = getHeight();
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			}
			if (remainedSize < 0) {
				remainedSize = 0;
			}
			
			sl_ui_pos sizeAvg = remainedSize / countFullFill;
			for (i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				if (child.isNotNull()) {
					if (child->getVisibility() != Visibility::Gone) {
						if (flagHorizontal) {
							if (child->getWidthMode() == SizeMode::Filling) {
								sl_real weight = child->getWidthWeight();
								if (Math::isAlmostZero(weight - 1)) {
									UIRect frame = child->getLayoutFrame();
									frame.setWidth(sizeAvg);
									child->setLayoutFrame(frame);
								}
							}
						} else {
							if (child->getHeightMode() == SizeMode::Filling) {
								sl_real weight = child->getHeightWeight();
								if (Math::isAlmostZero(weight - 1)) {
									UIRect frame = child->getLayoutFrame();
									frame.setHeight(sizeAvg);
									child->setLayoutFrame(frame);
								}
							}
						}
					}
				}
			}
		}
		
		sl_ui_pos pos;
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
						UIRect frame = child->getLayoutFrame();
						sl_ui_len width = frame.getWidth();
						pos += child->getMarginLeft();
						frame.left = pos;
						frame.right = pos + width;
						frame.fixSizeError();
						child->setLayoutFrame(frame);
						pos += width;
						pos += child->getMarginRight();
					} else {
						UIRect frame = child->getLayoutFrame();
						sl_ui_len height = frame.getHeight();
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
	
	VerticalLinearView::VerticalLinearView()
	{
		setOrientation(LayoutOrientation::Vertical, UIUpdateMode::Init);
	}
	
	HorizontalLinearView::HorizontalLinearView()
	{
		setOrientation(LayoutOrientation::Horizontal, UIUpdateMode::Init);
	}
}
