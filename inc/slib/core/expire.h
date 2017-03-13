#ifndef CHECKHEADER_SLIB_CORE_EXPIRE
#define CHECKHEADER_SLIB_CORE_EXPIRE

#include "definition.h"

#include "object.h"
#include "map.h"
#include "dispatch_loop.h"

namespace slib
{
	
	template <class KT, class VT>
	class SLIB_EXPORT ExpiringMap : public Object
	{
	protected:
		Map<KT, VT> m_mapCurrent;
		Map<KT, VT> m_mapBackup;

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

		void setupTimer(sl_uint32 period_ms);
	
		sl_bool get(const KT& key, VT* _out = sl_null, sl_bool flagUpdateLifetime = sl_true);

		VT getValue(const KT& key, const VT& def, sl_bool flagUpdateLifetime = sl_true);

		sl_bool put(const KT& key, const VT& value);

		void remove(const KT& key);

		void removeAll();

		sl_bool containsKey(const KT& key);

	protected:
		void _update(Timer* timer);

		void _release();
	
	};

}

#include "detail/expire.h"

#endif
