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
#include "slib/ui/core.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ViewPager, ViewGroup)

	ViewPager::ViewPager()
	{
		setUsingChildLayouts(sl_false);
		setClipping(sl_true, UIUpdateMode::Init);
		
		m_indexCurrent = 0;
		
		m_flagLoop = sl_false;
		
		m_flagMouseDown = sl_false;
		m_posMouseDown.x = 0;
		m_posMouseDown.y = 0;
		m_offsetPages = 0;
		m_offsetPagesMouseDown = 0;
	}
	
	ViewPager::~ViewPager()
	{
	}
	
	void ViewPager::init()
	{
		ViewGroup::init();
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
		_selectPage(sl_false, index, mode);
	}

	void ViewPager::_selectPage(sl_bool flagEvent, sl_uint64 index, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		
		sl_uint64 n = getPagesCount();
		if (n <= 0) {
			index = 0;
		} else {
			if (m_flagLoop) {
				if ((sl_int64)index < 0) {
					index += n;
				} else if (index >= n) {
					index -= n;
				}
			}
			if ((sl_int64)index < 0) {
				index = 0;
			} else if (index >= n) {
				index = n - 1;
			}
		}
		
		sl_uint64 indexCurrent = m_indexCurrent;

		do {
			if (!SLIB_UI_UPDATE_MODE_IS_REDRAW(mode)) {
				break;
			}
			if (m_cache.isEmpty()) {
				if (indexCurrent != index) {
					m_indexCurrent = index;
					_reloadPages();
				}
				break;
			}
			if (!SLIB_UI_UPDATE_MODE_IS_ANIMATE(mode)) {
				if (indexCurrent == index) {
					if (m_offsetPages != 0) {
						_relocatePages();
					}
				} else {
					m_indexCurrent = index;
					_reloadPages();
				}
				break;
			}
			if (index != indexCurrent) {
				if (m_flagLoop) {
					sl_uint64 m = 0;
					sl_ui_len k = 0;
					if (m_offsetPages < 0) {
						if (index > indexCurrent) {
							m = index - indexCurrent;
						} else {
							m = index + n - indexCurrent;
						}
						k = getWidth();
					} else if (m_offsetPages > 0) {
						if (index < indexCurrent) {
							m = indexCurrent - index;
						} else {
							m = indexCurrent + n - index;
						}
						k = - getWidth();
					} else {
						if (index > indexCurrent) {
							if (indexCurrent == 0 && index + 1 == n) {
								m = 1;
								k = - getWidth();
							} else {
								m = index - indexCurrent;
								k = getWidth();
							}
						} else {
							if (indexCurrent + 1 == n && index == 0) {
								m = 1;
								k = getWidth();
							} else {
								m = indexCurrent - index;
								k = - getWidth();
							}
						}
					}
					if (m > 1) {
						m_indexCurrent = index;
						_reloadPages();
						break;
					}
					m_offsetPages += k;
				} else {
					if (index > indexCurrent) {
						if (index > indexCurrent + 1) {
							m_indexCurrent = index;
							_reloadPages();
							break;
						}
						m_offsetPages += getWidth();
					} else {
						if (indexCurrent > index + 1) {
							m_indexCurrent = index;
							_reloadPages();
							break;
						}
						m_offsetPages -= getWidth();
					}
				}
				_loadPage(index);
				m_indexCurrent = index;
			}
			if (m_offsetPages != 0) {
				m_timer = startTimer(SLIB_FUNCTION_WEAKREF(ViewPager, _onAnimation, this), 20);
			}
		} while (0);
		
		if (indexCurrent != index) {
			m_indexCurrent = index;
			if (flagEvent) {
				lock.unlock();
				dispatchSelectPage(index);
			}
		}
	}
	
	void ViewPager::goToPrevious(UIUpdateMode mode)
	{
		selectPage(m_indexCurrent - 1, mode);
	}
	
	void ViewPager::goToNext(UIUpdateMode mode)
	{
		selectPage(m_indexCurrent + 1, mode);
	}
	
	sl_bool ViewPager::isLoop()
	{
		return m_flagLoop;
	}
	
	void ViewPager::setLoop(sl_bool flag)
	{
		m_flagLoop = flag;
	}

	void ViewPager::setLockScroll(sl_bool flag)
	{
		ViewGroup::setLockScroll(flag);
		if (flag) {
			m_flagMouseDown = sl_false;
			_selectPage(sl_true, m_indexCurrent);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(ViewPager, SelectPage, sl_uint64 index)
	
	void ViewPager::dispatchSelectPage(sl_uint64 index)
	{
		SLIB_INVOKE_EVENT_HANDLER(SelectPage, index)
	}

	void ViewPager::onResize(sl_ui_len width, sl_ui_len height)
	{
		_resizePages();
	}
	
	void ViewPager::onChangePadding()
	{
		_resizePages();
	}
	
	void ViewPager::onMouseEvent(UIEvent* ev)
	{
		sl_uint64 countPages = getPagesCount();
		if (countPages <= 0) {
			return;
		}
		
		sl_real dimUnit = Math::ceil(UI::dpToPixel(1));
		if (dimUnit < 1) {
			dimUnit = 1;
		}
		
		UIAction action = ev->getAction();
		Point pos = ev->getPoint();
		
		if (action != UIAction::LeftButtonDrag && action != UIAction::TouchMove) {
			setCapturingEvents(sl_false);
			if (action != UIAction::LeftButtonDown || action != UIAction::TouchBegin) {
				Ref<View> parent = getParent();
				if (parent.isNotNull()) {
					parent->setLockScroll(sl_false);
				}
			}
		}
		
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			m_motionTracker.clearMovements();
			m_flagMouseDown = sl_true;
			m_posMouseDown = pos;
			m_offsetPagesMouseDown = m_offsetPages;
			m_timer.setNull();
		} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
			if (m_flagMouseDown) {
				if (isLockScroll()) {
					if (m_offsetPages && m_timer.isNull()) {
						_selectPage(sl_true, m_indexCurrent);
					}
					return;
				}
				sl_real dx = Math::abs(pos.x - m_posMouseDown.x);
				sl_real dy = Math::abs(pos.y - m_posMouseDown.y);
				if (dx > 10 * dimUnit) {
					cancelPressedStateOfChildren();
					if (dy < dx) {
						setCapturingEvents(sl_true);
						Ref<View> parent = getParent();
						if (parent.isNotNull()) {
							parent->setLockScroll(sl_true);
						}
					}
				}
				if (dy > 10 * dimUnit) {
					if (dy >= dx) {
						m_flagMouseDown = sl_false;
						m_motionTracker.clearMovements();
						_selectPage(sl_true, m_indexCurrent);
						return;
					}
				}
				m_motionTracker.addMovement(pos.x, pos.y);
				sl_uint64 indexCurrent = m_indexCurrent;
				sl_ui_len offset = m_offsetPagesMouseDown + (sl_ui_len)(pos.x - m_posMouseDown.x);
				sl_uint64 indexOther;
				if (countPages >= 2) {
					if (offset > 0) {
						if (indexCurrent == 0) {
							if (m_flagLoop) {
								indexOther = countPages - 1;
							} else {
								indexOther = indexCurrent;
								offset = 0;
							}
						} else {
							indexOther = indexCurrent - 1;
						}
					} else if (offset < 0) {
						if (indexCurrent + 1 >= countPages) {
							if (m_flagLoop) {
								indexOther = 0;
							} else {
								indexOther = indexCurrent;
								offset = 0;
							}
						} else {
							indexOther = indexCurrent + 1;
						}
					} else {
						indexOther = indexCurrent;
					}
				} else {
					indexOther = indexCurrent;
					offset = 0;
				}
				sl_ui_len width = getWidth();
				if (offset < -width) {
					offset = -width;
				}
				if (offset > width) {
					offset = width;
				}
				m_offsetPages = offset;
				_loadPage(indexCurrent);
				if (indexOther != indexCurrent) {
					_loadPage(indexOther);
				}
				for (auto& item : m_cache) {
					item.value->setTranslationX((sl_real)(_getPagePosition(item.key)));
				}
			}
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
			if (m_flagMouseDown && !(isLockScroll())) {
				sl_real v = 0;
				if (Math::abs(m_offsetPages) > dimUnit * 10) {
					m_motionTracker.getVelocity(&v, sl_null);
				}
				sl_real t = dimUnit * 10;
				if (v > t) {
					if (m_offsetPages > 0) {
						_selectPage(sl_true, m_indexCurrent - 1);
					} else {
						_selectPage(sl_true, m_indexCurrent);
					}
				} else if (v < -t) {
					if (m_offsetPages < 0) {
						_selectPage(sl_true, m_indexCurrent + 1);
					} else {
						_selectPage(sl_true, m_indexCurrent);
					}
				} else {
					sl_ui_len o = (sl_ui_len)(getWidth() * 0.4f);
					if (m_offsetPages > o) {
						_selectPage(sl_true, m_indexCurrent - 1);
					} else if (m_offsetPages < -o) {
						_selectPage(sl_true, m_indexCurrent + 1);
					} else {
						_selectPage(sl_true, m_indexCurrent);
					}
				}
			} else {
				_selectPage(sl_true, m_indexCurrent);
			}
			m_flagMouseDown = sl_false;
			m_motionTracker.clearMovements();
		}
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
				view->setTranslationX((sl_real)(_getPagePosition(index)));
				addChild(view);
				m_cache.put_NoLock(index, view);
				return view;
			}
		}
		return sl_null;
	}
	
	sl_ui_pos ViewPager::_getPagePosition(sl_uint64 index)
	{
		sl_ui_len offset = m_offsetPages;
		sl_uint64 cur = m_indexCurrent;
		if (index == cur) {
			return offset;
		} else {
			if (m_flagLoop && offset != 0) {
				sl_uint64 countPages = getPagesCount();
				if (offset < 0) {
					sl_uint64 n;
					if (index >= cur) {
						n = index - cur;
					} else {
						n = index + countPages;
						if (n > cur) {
							n -= cur;
						} else {
							n = 0;
						}
					}
					if (n > 100) {
						n = 100;
					}
					return getWidth() * (sl_ui_len)n + offset;
				} else {
					sl_uint64 n;
					if (index <= cur) {
						n = cur - index;
					} else {
						n = cur + countPages;
						if (n > index) {
							n -= index;
						} else {
							n = 0;
						}
					}
					if (n > 100) {
						n = 100;
					}
					return - (getWidth() * (sl_ui_len)n) + offset;
				}
			} else {
				if (index > cur) {
					sl_uint64 n = index - cur;
					if (n > 100) {
						n = 100;
					}
					return getWidth() * (sl_ui_len)n + offset;
				} else {
					sl_uint64 n = cur - index;
					if (n > 100) {
						n = 100;
					}
					return - (getWidth() * (sl_ui_len)n) + offset;
				}
			}
		}
	}
	
	void ViewPager::_relocatePages()
	{
		ObjectLocker lock(this);
		m_offsetPages = 0;
		for (auto& item : m_cache) {
			item.value->setTranslationX((sl_real)(_getPagePosition(item.key)));
		}
	}
	
	void ViewPager::_resizePages()
	{
		ObjectLocker lock(this);
		UIRect rect = getBoundsInnerPadding();
		m_offsetPages = 0;
		for (auto& item : m_cache) {
			item.value->setFrame(rect);
			item.value->setTranslationX((sl_real)(_getPagePosition(item.key)));
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
		sl_ui_len offset = m_offsetPages;
		sl_ui_len t = getWidth() / 10;
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
		m_offsetPages = offset;
		for (auto& item : m_cache) {
			item.value->setTranslationX((sl_real)(_getPagePosition(item.key)));
		}
	}
	
}
