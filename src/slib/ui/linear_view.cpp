/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/linear_view.h"

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
	
	void LinearView::onUpdateLayout()
	{
		sl_bool flagHorizontalLayout = m_orientation == LayoutOrientation::Horizontal;
		
		sl_ui_pos sizeSum = 0;
		sl_uint32 countFullFill = 0;
		sl_uint32 countPartFill = 0;
		
		ListElements< Ref<View> > children(getChildren());
		sl_size i;
		for (i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child->getVisibility() != Visibility::Gone) {
				if (flagHorizontalLayout) {
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
		
		if (sizeSum < 0) {
			sizeSum = 0;
		}
		
		if (countPartFill > 0) {
			sl_ui_pos remainedSize;
			if (flagHorizontalLayout) {
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
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontalLayout) {
						if (child->getWidthMode() == SizeMode::Filling) {
							sl_real weight = child->getWidthWeight();
							if (!(Math::isAlmostZero(weight - 1))) {
								sl_ui_pos width = (sl_ui_pos)((sl_real)(remainedSize) * weight);
								sizeSum += width;
								child->setLayoutWidth(width);
							}
						}
					} else {
						if (child->getHeightMode() == SizeMode::Filling) {
							sl_real weight = child->getHeightWeight();
							if (!(Math::isAlmostZero(weight - 1))) {
								sl_ui_pos height = (sl_ui_pos)((sl_real)(remainedSize) * weight);
								sizeSum += height;
								child->setLayoutHeight(height);
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
			if (flagHorizontalLayout) {
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
				if (child->getVisibility() != Visibility::Gone) {
					if (flagHorizontalLayout) {
						if (child->getWidthMode() == SizeMode::Filling) {
							sl_real weight = child->getWidthWeight();
							if (Math::isAlmostZero(weight - 1)) {
								child->setLayoutWidth(sizeAvg);
							}
						}
					} else {
						if (child->getHeightMode() == SizeMode::Filling) {
							sl_real weight = child->getHeightWeight();
							if (Math::isAlmostZero(weight - 1)) {
								child->setLayoutHeight(sizeAvg);
							}
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
			}
		}
		
		View::onUpdateLayout();
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
