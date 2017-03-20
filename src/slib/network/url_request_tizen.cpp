#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/network/url_request.h"

namespace slib
{

	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		return sl_null;
	}

}

#endif
