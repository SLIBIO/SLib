#ifndef CHECKHEADER_SLIB_CORE_EXPIRE
#define CHECKHEADER_SLIB_CORE_EXPIRE

#include "definition.h"

#include "object.h"
#include "map.h"
#include "async.h"

SLIB_NAMESPACE_BEGIN
template <class KT, class VT>
class SLIB_EXPORT ExpiringMap : public Object
{
protected:
	typedef ExpiringMap<KT, VT> _ExpiringMap;

	Map<KT, VT> m_mapCurrent;
	Map<KT, VT> m_mapBackup;
	
	sl_uint32 m_duration;
	Ref<AsyncTimer> m_timer;
	WeakRef<AsyncLoop> m_loop;

public:
	SLIB_INLINE ExpiringMap()
	{
		m_duration = 0;
	}

	SLIB_INLINE ~ExpiringMap()
	{
		MutexLocker lock(getLocker());
		Ref<AsyncTimer> timer = m_timer;
		if (timer.isNotNull()) {
			timer->stopAndWait();
		}
		clearTimer();
	}

	SLIB_INLINE sl_uint32 getExpiringMilliseconds() const
	{
		return m_duration;
	}

	void setupTimer(sl_uint32 expiring_duration_ms, const Ref<AsyncLoop>& _loop)
	{
		MutexLocker lock(getLocker());
		clearTimer();
		Ref<AsyncLoop> loop = _loop;
		if (loop.isNull()) {
			loop = AsyncLoop::getDefault();
			if (loop.isNull()) {
				return;
			}
		}
		if (expiring_duration_ms > 0) {
			m_duration = expiring_duration_ms;
			m_loop = loop;
			m_timer = loop->addTimer(SLIB_CALLBACK_CLASS(_ExpiringMap, _update, this), expiring_duration_ms);
		}
	}

	void clearTimer()
	{
		MutexLocker lock(getLocker());
		Ref<AsyncTimer> timer = m_timer;
		if (timer.isNotNull()) {
			Ref<AsyncLoop> loop = m_loop.lock();
			if (loop.isNotNull()) {
				loop->removeTimer(timer);
			}
		}
		m_timer.setNull();
		m_loop.setNull();
	}

	SLIB_INLINE void setupTimer(sl_uint32 period_ms)
	{
		setupTimer(period_ms, Ref<AsyncLoop>::null());
	}

	sl_bool get(const KT& key, VT* out = sl_null, sl_bool flagUpdateLifetime = sl_true)
	{
		MutexLocker lock(getLocker());
		VT* p = m_mapCurrent.getItemPtr(key);
		if (p) {
			if (out) {
				*out = *p;
			}
			return sl_true;
		}
		p = m_mapBackup.getItemPtr(key);
		if (p) {
			if (out) {
				*out = *p;
			}
			if (flagUpdateLifetime) {
				m_mapCurrent.put(key, *p);
				m_mapBackup.remove(key);
			}
			return sl_true;
		}
		return sl_false;
	}

	VT getValue(const KT& key, const VT& def, sl_bool flagUpdateLifetime = sl_true)
	{
		MutexLocker lock(getLocker());
		VT* p = m_mapCurrent.getItemPtr(key);
		if (p) {
			return *p;
		}
		p = m_mapBackup.getItemPtr(key);
		if (p) {
			if (flagUpdateLifetime) {
				m_mapCurrent.put(key, *p);
				m_mapBackup.remove(key);
			}
			return *p;
		}
		return def;
	}

	sl_bool put(const KT& key, const VT& value)
	{
		MutexLocker lock(getLocker());
		m_mapBackup.remove(key);
		return m_mapCurrent.put(key, value);
	}

	void remove(const KT& key)
	{
		MutexLocker lock(getLocker());
		m_mapCurrent.remove(key);
		m_mapBackup.remove(key);
	}

	void clear()
	{
		MutexLocker lock(getLocker());
		m_mapCurrent.clear();
		m_mapBackup.clear();
	}

	SLIB_INLINE void removeAll()
	{
		clear();
	}

	sl_bool containsKey(const KT& key)
	{
		MutexLocker lock(getLocker());
		if (m_mapCurrent.containsKey(key)) {
			return sl_true;
		}
		if (m_mapBackup.containsKey(key)) {
			return sl_true;
		}
		return sl_false;
	}

protected:
	void _update()
	{
		MutexLocker lock(getLocker());
		m_mapBackup = m_mapCurrent;
		m_mapCurrent.setNull();
	}
};
SLIB_NAMESPACE_END

#endif
