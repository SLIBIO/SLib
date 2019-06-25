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

#ifndef CHECKHEADER_SLIB_CORE_EXPIRE
#define CHECKHEADER_SLIB_CORE_EXPIRE

#include "definition.h"

#include "object.h"
#include "hash_map.h"
#include "dispatch_loop.h"

namespace slib
{
	
	template <class KT, class VT>
	class SLIB_EXPORT ExpiringMap : public Object
	{
	protected:
		HashMap<KT, VT> m_mapCurrent;
		HashMap<KT, VT> m_mapBackup;

		sl_uint32 m_duration;

		Ref<Timer> m_timer;
		WeakRef<DispatchLoop> m_dispatchLoop;
	
	public:
		ExpiringMap();

		~ExpiringMap();

	public:
		sl_uint32 getExpiringMilliseconds() const;
	
		void setupTimer(sl_uint32 expiring_duration_ms, const Ref<DispatchLoop>& loop);
	
		void clearTimer();

		void setupTimer(sl_uint32 expiring_duration_ms);
	
		sl_bool get(const KT& key, VT* _out = sl_null, sl_bool flagUpdateLifetime = sl_true);

		VT getValue(const KT& key, const VT& def, sl_bool flagUpdateLifetime = sl_true);

		sl_bool put(const KT& key, const VT& value);

		void remove(const KT& key);

		void removeAll();

		sl_bool contains(const KT& key);

	protected:
		void _update(Timer* timer);

		void _release();
	
	};

}

#include "detail/expire.inc"

#endif
