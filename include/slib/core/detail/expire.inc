/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

	template <class KT, class VT>
	ExpiringMap<KT, VT>::ExpiringMap()
	{
		m_mapCurrent.init();
		m_mapBackup.init();

		m_duration = 0;
	}
	
	template <class KT, class VT>
	ExpiringMap<KT, VT>::~ExpiringMap()
	{
		_release();
	}
	
	template <class KT, class VT>
	sl_uint32 ExpiringMap<KT, VT>::getExpiringMilliseconds() const
	{
		return m_duration;
	}
	
	template <class KT, class VT>
	void ExpiringMap<KT, VT>::setupTimer(sl_uint32 expiring_duration_ms, const Ref<DispatchLoop>& _loop)
	{
		ObjectLocker lock(this);
		clearTimer();
		Ref<DispatchLoop> loop = _loop;
		if (loop.isNull()) {
			loop = DispatchLoop::getDefault();
			if (loop.isNull()) {
				return;
			}
		}
		if (expiring_duration_ms > 0) {
			m_duration = expiring_duration_ms;
			m_dispatchLoop = loop;
			typedef ExpiringMap<KT, VT> EXPIRING_MAP;
			m_timer = Timer::startWithLoop(loop, SLIB_FUNCTION_CLASS(EXPIRING_MAP, _update, this), expiring_duration_ms);
		}
	}
	
	template <class KT, class VT>
	void ExpiringMap<KT, VT>::clearTimer()
	{
		ObjectLocker lock(this);
		Ref<Timer> timer = m_timer;
		if (timer.isNotNull()) {
			Ref<DispatchLoop> loop = m_dispatchLoop;
			if (loop.isNotNull()) {
				loop->removeTimer(timer);
			}
		}
		m_timer.setNull();
		m_dispatchLoop.setNull();
	}
	
	template <class KT, class VT>
	void ExpiringMap<KT, VT>::setupTimer(sl_uint32 period_ms)
	{
		setupTimer(period_ms, Ref<DispatchLoop>::null());
	}
	
	template <class KT, class VT>
	sl_bool ExpiringMap<KT, VT>::get(const KT& key, VT* _out, sl_bool flagUpdateLifetime)
	{
		ObjectLocker lock(this);
		VT* p = m_mapCurrent.getItemPointer(key);
		if (p) {
			if (_out) {
				*_out = *p;
			}
			return sl_true;
		}
		p = m_mapBackup.getItemPointer(key);
		if (p) {
			if (_out) {
				*_out = *p;
			}
			if (flagUpdateLifetime) {
				m_mapCurrent.put_NoLock(key, *p);
				m_mapBackup.remove_NoLock(key);
			}
			return sl_true;
		}
		return sl_false;
	}
	
	template <class KT, class VT>
	VT ExpiringMap<KT, VT>::getValue(const KT& key, const VT& def, sl_bool flagUpdateLifetime)
	{
		ObjectLocker lock(this);
		VT* p = m_mapCurrent.getItemPointer(key);
		if (p) {
			return *p;
		}
		p = m_mapBackup.getItemPointer(key);
		if (p) {
			if (flagUpdateLifetime) {
				m_mapCurrent.put_NoLock(key, *p);
				m_mapBackup.remove_NoLock(key);
			}
			return *p;
		}
		return def;
	}

	template <class KT, class VT>
	sl_bool ExpiringMap<KT, VT>::put(const KT& key, const VT& value)
	{
		ObjectLocker lock(this);
		m_mapBackup.remove_NoLock(key);
		return m_mapCurrent.put_NoLock(key, value);
	}

	template <class KT, class VT>
	void ExpiringMap<KT, VT>::remove(const KT& key)
	{
		ObjectLocker lock(this);
		m_mapCurrent.remove_NoLock(key);
		m_mapBackup.remove_NoLock(key);
	}

	template <class KT, class VT>
	void ExpiringMap<KT, VT>::removeAll()
	{
		ObjectLocker lock(this);
		m_mapCurrent.removeAll_NoLock();
		m_mapBackup.removeAll_NoLock();
	}

	template <class KT, class VT>
	sl_bool ExpiringMap<KT, VT>::contains(const KT& key)
	{
		ObjectLocker lock(this);
		if (m_mapCurrent.find_NoLock(key)) {
			return sl_true;
		}
		if (m_mapBackup.find_NoLock(key)) {
			return sl_true;
		}
		return sl_false;
	}

	template <class KT, class VT>
	void ExpiringMap<KT, VT>::_update(Timer* timer)
	{
		ObjectLocker lock(this);
		m_mapBackup = m_mapCurrent;
		m_mapCurrent.init();
	}

	template <class KT, class VT>
	void ExpiringMap<KT, VT>::_release()
	{
		ObjectLocker lock(this);
		if (m_timer.isNotNull()) {
			m_timer->stopAndWait();
		}
		clearTimer();
	}

}
