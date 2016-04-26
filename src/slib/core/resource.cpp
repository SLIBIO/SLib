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

SLIB_NAMESPACE_END
