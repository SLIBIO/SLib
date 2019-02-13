/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/ui/linear_view.h"

#include "slib/core/scoped.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(LinearView, ViewGroup)
	
	LinearView::LinearView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setAlwaysOnUpdateLayout(sl_true, UIUpdateMode::Init);
		setSavingCanvasState(sl_false);
		
		m_orientation = LayoutOrientation::Vertical;
	}
	
	LinearView::~LinearView()
	{
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
		invalidateLayout(mode);
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
	
	void LinearView::onAddChild(View* child)
	{
		if (m_orientation == LayoutOrientation::Vertical) {
			child->setTopFree(UIUpdateMode::Init);
			child->setBottomFree(UIUpdateMode::Init);
		} else {
			child->setLeftFree(UIUpdateMode::Init);
			child->setRightFree(UIUpdateMode::Init);
		}
	}
	
	void LinearView::onUpdateLayout()
	{
		sl_bool flagHorizontalLayout = m_orientation == LayoutOrientation::Horizontal;
		
		sl_ui_pos sizeSum = 0;
		sl_uint32 countFill = 0;
		sl_real sumFillWeights = 0;

		sl_ui_len widthContainer = getLayoutWidth() - getPaddingLeft() - getPaddingTop();
		sl_ui_len heightContainer = getLayoutHeight() - getPaddingTop() - getPaddingBottom();

		ListElements< Ref<View> > children(getChildren());
		
		SLIB_SCOPED_BUFFER(Size, 512, childSizes, children.count);
		
		sl_size i;
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child->getVisibility() != Visibility::Gone) {
				childSizes[i] = child->getLayoutSize();
				if (flagHorizontalLayout) {
					sizeSum += child->getMarginLeft();
					if (child->getWidthMode() != SizeMode::Filling) {
						sizeSum += child->getLayoutWidth();
					} else {
						countFill++;
						sumFillWeights += child->getWidthWeight();
					}
					sizeSum += child->getMarginRight();
				} else {
					sizeSum += child->getMarginTop();
					if (child->getHeightMode() != SizeMode::Filling) {
						sizeSum += child->getLayoutHeight();
					} else {
						countFill++;
						sumFillWeights += child->getHeightWeight();
					}
					sizeSum += child->getMarginBottom();
				}
			}
		}
		
		if (sizeSum < 0) {
			sizeSum = 0;
		}
		
		if (countFill > 0) {
			sl_ui_pos remainedSize;
			if (flagHorizontalLayout) {
				sl_ui_len n = widthContainer;
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			} else {
				sl_ui_len n = heightContainer;
				if (n > (sl_ui_len)sizeSum) {
					remainedSize = n - sizeSum;
				} else {
					remainedSize = 0;
				}
			}
			if (remainedSize < 0) {
				remainedSize = 0;
			}
			if (sumFillWeights < SLIB_EPSILON) {
				sumFillWeights = 1;
			}
			
			for (i = 0; i < children.count; i++) {
				Ref<View>& child = children[i];
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontalLayout) {
						if (child->getWidthMode() == SizeMode::Filling) {
							sl_ui_len width = (sl_ui_len)(remainedSize * child->getWidthWeight() / sumFillWeights);
							sl_ui_len height = child->getLayoutHeight();
							child->_restrictSize(width, height);
							child->setLayoutSize(width, height);
						}
					} else {
						if (child->getHeightMode() == SizeMode::Filling) {
							sl_ui_len width = getLayoutWidth();
							sl_ui_len height = (sl_ui_len)(remainedSize * child->getHeightWeight() / sumFillWeights);
							child->_restrictSize(width, height);
							child->setLayoutSize(width, height);
						}
					}
				}
			}
		}
		
		sl_ui_pos pos;
		if (flagHorizontalLayout) {
			pos = getPaddingLeft();
		} else {
			pos = getPaddingTop();
		}
		
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child->getVisibility() != Visibility::Gone) {
				if (flagHorizontalLayout) {
					UIRect frame = child->getLayoutFrame();
					sl_ui_len width = frame.getWidth();
					pos += child->getMarginLeft();
					frame.left = pos;
					frame.right = pos + width;
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
				if (!(childSizes[i].isAlmostEqual(child->getLayoutSize()))) {
					child->forceUpdateLayout();
				}
			}
		}
		
		if (flagHorizontalLayout) {
			measureLayoutWrappingSize(sl_false, isHeightWrapping());
			if (isWidthWrapping()) {
				pos += getPaddingRight();
				setLayoutWidth(pos);
			}
		} else {
			measureLayoutWrappingSize(isWidthWrapping(), sl_false);
			if (isHeightWrapping()) {
				pos += getPaddingBottom();
				setLayoutHeight(pos);
			}
		}

	}
	
	VerticalLinearView::VerticalLinearView()
	{
		setOrientation(LayoutOrientation::Vertical, UIUpdateMode::Init);
	}
	
	VerticalLinearView::~VerticalLinearView()
	{
	}

	HorizontalLinearView::HorizontalLinearView()
	{
		setOrientation(LayoutOrientation::Horizontal, UIUpdateMode::Init);
	}

	HorizontalLinearView::~HorizontalLinearView()
	{
	}

}
