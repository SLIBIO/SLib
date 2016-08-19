#include "../../../inc/slib/core/resource.h"

SLIB_NAMESPACE_BEGIN

static Locale _g_resource_current_locale = Locale::Unknown;

Locale Resources::getCurrentLocale()
{
	return _g_resource_current_locale;
}

void Resources::setCurrentLocale(const Locale& locale)
{
	_g_resource_current_locale = locale;
}

String Resources::makeResourceName(const String& path)
{
	String name = path.duplicate();
	sl_char8* szName = name.getData();
	sl_size lenName = name.getLength();
	for (sl_size i = 0; i < lenName; i++) {
		if (!(SLIB_CHAR_IS_C_NAME(szName[i]))) {
			szName[i] = '_';
		}
	}
	return name;
}

SLIB_NAMESPACE_END
