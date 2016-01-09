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
		ObjectLocker lock(this);
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
		ObjectLocker lock(this);
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
		ObjectLocker lock(this);
		Ref<AsyncTimer> timer = m_timer;
		if (timer.isNotNull()) {
			Ref<AsyncLoop> loop = m_loop;
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
		ObjectLocker lock(this);
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
		ObjectLocker lock(this);
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
		ObjectLocker lock(this);
		m_mapBackup.remove(key);
		return m_mapCurrent.put(key, value);
	}

	void remove(const KT& key)
	{
		ObjectLocker lock(this);
		m_mapCurrent.remove(key);
		m_mapBackup.remove(key);
	}

	void removeAll()
	{
		ObjectLocker lock(this);
		m_mapCurrent.clear();
		m_mapBackup.clear();
	}

	sl_bool containsKey(const KT& key)
	{
		ObjectLocker lock(this);
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
		ObjectLocker lock(this);
		m_mapBackup = m_mapCurrent;
		m_mapCurrent.setNull();
	}
};
SLIB_NAMESPACE_END

#endif
