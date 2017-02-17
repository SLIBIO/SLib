#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_TIZEN

#include "../../../inc/slib/core/preference.h"

#include "../../../inc/slib/core/json.h"

#include <app_preference.h>
#include <stdlib.h>

namespace slib
{

	void Preference::setValue(const String& key, const Variant& value)
	{
		if (key.isEmpty()) {
			return;
		}
		String json = value.toJsonString();
		::preference_set_string(key.getData(), json.getData());
	}

	Variant Preference::getValue(const String& key)
	{
		if (key.isEmpty()) {
			return sl_null;
		}
		char* buf = sl_null;
		::preference_get_string(key.getData(), &buf);
		if (buf) {
			String s = buf;
			Variant ret;
			if (s.isNotEmpty()) {
				ret = Json::parseJson(s);
			}
			::free(buf);
			return ret;
		}
		return sl_null;
	}

}

#endif
