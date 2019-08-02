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

#include "ui_core_common.h"

#include "slib/core/safe_static.h"

namespace slib
{
	
	namespace priv
	{
		namespace ui_core
		{
			
			SLIB_SAFE_STATIC_GETTER(LinkedQueue< Function<void()> >, GetDispatchQueue)
			
			typedef CHashMap< sl_reg, Function<void()> > DelayedDispatchMap;
			SLIB_SAFE_STATIC_GETTER(DelayedDispatchMap, GetDelayedDispatchMap)
			sl_reg g_idLastDelayedDispatch = 0;
			

			sl_bool UIDispatcher::addCallback(const Function<void()>& callback)
			{
				if (callback.isNotNull()) {
					LinkedQueue< Function<void()> >* queue = GetDispatchQueue();
					if (queue) {
						return queue->push(callback);
					}
				}
				return sl_false;
			}
			
			void UIDispatcher::processCallbacks()
			{
				LinkedQueue< Function<void()> >* queue = GetDispatchQueue();
				if (queue) {
					sl_size n = queue->getCount();
					Function<void()> callback;
					while (n > 0 && queue->pop(&callback)) {
						callback();
						n--;
					}
				}
			}
			
			sl_bool UIDispatcher::addDelayedCallback(const Function<void()>& callback, sl_reg& outId)
			{
				if (callback.isNotNull()) {
					DelayedDispatchMap* map = GetDelayedDispatchMap();
					if (map) {
						sl_reg id = Base::interlockedIncrement(&g_idLastDelayedDispatch);
						if (map->put(id, callback)) {
							outId = id;
							return sl_true;
						}
					}
				}
				return sl_false;
			}
			
			void UIDispatcher::processDelayedCallback(sl_reg id)
			{
				DelayedDispatchMap* map = GetDelayedDispatchMap();
				if (map) {
					Function<void()> callback;
					if (map->get(id, &callback)) {
						callback();
					}
					map->remove(id);
				}
			}
			
			void UIDispatcher::removeAllCallbacks()
			{
				LinkedQueue< Function<void()> >* queue = GetDispatchQueue();
				if (queue) {
					queue->removeAll();
				}
				DelayedDispatchMap* map = GetDelayedDispatchMap();
				if (map) {
					map->removeAll();
				}
			}

		}
	}
	
}
