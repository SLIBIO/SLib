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

#include "slib/ui/view_pager.h"

#include "slib/core/timer.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPager, View)

	ViewPager::ViewPager()
	{
		m_indexCurrent = 0;
		setClipping(sl_true, UIUpdateMode::Init);
		
		m_flagMouseDown = sl_false;
		m_posMouseDown = 0;
		m_offsetPages = 0;
		m_offsetPagesMouseDown = 0;
	}
	
	ViewPager::~ViewPager()
	{
	}
	
	void ViewPager::setAdapter(const Ref<ViewAdapter>& adapter, UIUpdateMode mode)
	{
		m_adapter = adapter;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			_reloadPages();
		}
	}
	
	void ViewPager::addPage(const Ref<View>& view, UIUpdateMode mode)
	{
		Ref<ViewAdapter> adapter = m_adapter;
		Ref<ViewListAdapter> listAdapter;
		if (adapter.isNotNull()) {
			listAdapter = CastInstance<ViewListAdapter>(adapter.get());
		} else {
			listAdapter = new ViewListAdapter;
		}
		if (listAdapter.isNull()) {
			return;
		}
		listAdapter->addView(view);
		setAdapter(listAdapter, mode);
	}
	
	sl_uint64 ViewPager::getPagesCount()
	{
		Ref<ViewAdapter> adapter = m_adapter;
		if (adapter.isNotNull()) {
			return adapter->getItemsCount();
		}
		return 0;
	}
	
	sl_uint64 ViewPager::getCurrentIndex()
	{
		return m_indexCurrent;
	}
	
	void ViewPager::selectPage(sl_uint64 index, UIUpdateMode mode)
	{
		sl_uint64 n = getPagesCount();
		if (n <= 0) {
			index = 0;
		} else {
			if ((sl_int64)index < 0) {
				index = 0;
			} else if (index + 1 >= n) {
				index = n - 1;
			}
		}
		if (!SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
			m_indexCurrent = index;
			return;
		}
		if (m_cache.isEmpty()) {
			m_indexCurrent = index;
			_reloadPages();
			return;
		}
		if (!SLIB_UI_UPDATE_MODE_IS_ANIMATE(mode)) {
			if (m_indexCurrent == index) {
				if (m_offsetPages != 0) {
					_relocatePages();
				}
				return;
			}
			m_indexCurrent = index;
			_reloadPages();
			return;
		}
		sl_uint64 indexCurrent = m_indexCurrent;
		if (index > indexCurrent) {
			if (index > indexCurrent + 1) {
				m_indexCurrent = index;
				_reloadPages();
				return;
			}
			m_offsetPages += (sl_real)(index - indexCurrent) * (sl_real)(getWidth());
		} else if (index < indexCurrent) {
			if (indexCurrent > index + 1) {
				m_indexCurrent = index;
				_reloadPages();
				return;
			}
			m_offsetPages -= (sl_real)(indexCurrent - index) * (sl_real)(getWidth());
		}
		m_indexCurrent = index;
		m_timer = startTimer(SLIB_FUNCTION_WEAKREF(ViewPager, _onAnimation, this), 20);
	}
	
	void ViewPager::goToPrevious(UIUpdateMode mode)
	{
		selectPage(m_indexCurrent - 1, mode);
	}
	
	void ViewPager::goToNext(UIUpdateMode mode)
	{
		selectPage(m_indexCurrent + 1, mode);
	}

	void ViewPager::onMouseEvent(UIEvent* ev)
	{
		sl_uint64 countPages = getPagesCount();
		if (countPages <= 0) {
			return;
		}
		UIAction action = ev->getAction();
		sl_real pos = ev->getX();
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			m_motionTracker.clearMovements();
			m_flagMouseDown = sl_true;
			m_posMouseDown = pos;
			m_offsetPagesMouseDown = m_offsetPages;
			m_timer.setNull();
		} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
			if (m_flagMouseDown) {
				m_motionTracker.addMovement(pos, 0);
				sl_uint64 indexCurrent = m_indexCurrent;
				sl_real offset = m_offsetPagesMouseDown + (pos - m_posMouseDown);
				sl_uint64 indexOther = indexCurrent;
				if (indexCurrent <= 0) {
					if (offset > 0) {
						offset = 0;
					}
				} else {
					if (offset > 0) {
						indexOther--;
					}
				}
				if (indexCurrent + 1 >= countPages) {
					if (offset < 0) {
						offset = 0;
					}
				} else {
					if (offset < 0) {
						indexOther++;
					}
				}
				sl_real width = (sl_real)(getWidth());
				if (offset < -width) {
					offset = -width;
				}
				if (offset > width) {
					offset = width;
				}
				_loadPage(indexCurrent);
				if (indexOther != indexCurrent) {
					_loadPage(indexOther);
				}
				for (auto& item : m_cache) {
					item.value->setTranslationX(_getPagePosition(item.key) + offset);
				}
				m_offsetPages = offset;
			}
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
			if (m_flagMouseDown) {
				m_flagMouseDown = sl_false;
				sl_real v = 0;
				m_motionTracker.getVelocity(&v, sl_null);
				sl_real t = (sl_real)(getWidth()) * 0.05f;
				if (v > t) {
					if (m_offsetPages > 0) {
						goToPrevious();
					} else {
						selectPage(m_indexCurrent);
					}
				} else if (v < -t) {
					if (m_offsetPages < 0) {
						goToNext();
					} else {
						selectPage(m_indexCurrent);
					}
				} else {
					t = (sl_real)(getWidth()) * 0.4f;
					if (m_offsetPages > t) {
						goToPrevious();
					} else if (m_offsetPages < -t) {
						goToNext();
					} else {
						selectPage(m_indexCurrent);
					}
				}
				m_motionTracker.clearMovements();
			}
		}
	}
	
	void ViewPager::onResize(sl_ui_len width, sl_ui_len height)
	{
		_resizePages();
	}
	
	void ViewPager::onChangePadding()
	{
		_resizePages();
	}
	
	Ref<View> ViewPager::_loadPage(sl_uint64 index)
	{
		ObjectLocker lock(this);
		Ref<View> view;
		if (m_cache.get_NoLock(index, &view)) {
			return view;
		}
		Ref<ViewAdapter> adapter = m_adapter;
		if (adapter.isNotNull()) {
			view = adapter->getView(index, sl_null, this);
			if (view.isNotNull()) {
				UIRect rect = getBoundsInnerPadding();
				view->setFrame(rect);
				view->setTranslationX(_getPagePosition(index));
				addChild(view);
				m_cache.put_NoLock(index, view);
				return view;
			}
		}
		return sl_null;
	}
	
	sl_real ViewPager::_getPagePosition(sl_uint64 index)
	{
		sl_uint64 cur = m_indexCurrent;
		if (index == cur) {
			return 0;
		} else if (index > cur) {
			sl_uint64 n = index - cur;
			if (n > 100) {
				n = 100;
			}
			return (sl_real)(getWidth()) * (sl_real)n;
		} else {
			sl_uint64 n = cur - index;
			if (n > 100) {
				n = 100;
			}
			return - ((sl_real)(getWidth()) * (sl_real)n);
		}
	}
	
	void ViewPager::_relocatePages()
	{
		ObjectLocker lock(this);
		m_offsetPages = 0;
		for (auto& item : m_cache) {
			item.value->setTranslationX(_getPagePosition(item.key));
		}
	}
	
	void ViewPager::_resizePages()
	{
		ObjectLocker lock(this);
		UIRect rect = getBoundsInnerPadding();
		m_offsetPages = 0;
		for (auto& item : m_cache) {
			item.value->setFrame(rect);
			item.value->setTranslationX(_getPagePosition(item.key));
		}
	}
	
	void ViewPager::_reloadPages()
	{
		ObjectLocker lock(this);
		for (auto& item : m_cache) {
			removeChild(item.value, UIUpdateMode::None);
		}
		m_cache.removeAll_NoLock();
		m_offsetPages = 0;
		_loadPage(m_indexCurrent);
	}
	
	void ViewPager::_cleanCache()
	{
		ObjectLocker lock(this);
		sl_uint64 indexCurrent = m_indexCurrent;
		Ref<View> current;
		for (auto& item : m_cache) {
			if (item.key == indexCurrent) {
				current = item.value;
			} else {
				removeChild(item.value);
			}
		}
		m_cache.removeAll_NoLock();
		if (current.isNotNull()) {
			m_cache.put_NoLock(indexCurrent, current);
		}
	}
	
	void ViewPager::_onAnimation(Timer* timer)
	{
		ObjectLocker lock(this);
		sl_real offset = m_offsetPages;
		sl_real t = (sl_real)(getWidth()) * 0.1f;
		if (t < 1) {
			t = 1;
		}
		if (offset < -t) {
			offset += t;
		} else if (offset > t) {
			offset -= t;
		} else {
			offset = 0;
			m_timer.setNull();
			_cleanCache();
		}
		for (auto& item : m_cache) {
			item.value->setTranslationX(_getPagePosition(item.key) + offset);
		}
		m_offsetPages = offset;
	}
	
}
