#include "../../../inc/slib/core/preference.h"

SLIB_NAMESPACE_BEGIN

SLIB_STATIC_ATOMIC_STRING16_NULL(_g_preference_app_key)

String Preference::getApplicationKeyName()
{
	return _g_preference_app_key;
}

void Preference::setApplicationKeyName(const String& name)
{
	_g_preference_app_key = name;
}

SLIB_NAMESPACE_END