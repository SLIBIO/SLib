#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WIN32

#include "../../../inc/slib/core/preference.h"
#include "../../../inc/slib/core/json.h"

#include "../../../inc/slib/core/platform_windows.h"

SLIB_NAMESPACE_BEGIN

void Preference::setValue(const String& key, const Variant& value)
{
	if (key.isEmpty()) {
		return;
	}
	String appName = getApplicationKeyName();
	if (appName.isEmpty()) {
		return;
	}
	Windows::setRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, value.toJsonString());
}

Variant Preference::getValue(const String& key)
{
	if (key.isEmpty()) {
		return sl_null;
	}
	String appName = getApplicationKeyName();
	if (appName.isEmpty()) {
		return sl_null;
	}
	Variant v;
	if (Windows::getRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, &v)) {
		String s = v.getString();
		if (s.isNotEmpty()) {
			return Json::parseJson(s);
		}
	}
	return sl_null;
}

SLIB_NAMESPACE_END

#endif