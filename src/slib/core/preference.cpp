#include "../../../inc/slib/core/preference.h"

#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	SLIB_STATIC_ZERO_INITIALIZED(AtomicString, _g_preference_app_key)

	String Preference::getApplicationKeyName()
	{
		return _g_preference_app_key;
	}

	void Preference::setApplicationKeyName(const String& name)
	{
		_g_preference_app_key = name;
	}

}
