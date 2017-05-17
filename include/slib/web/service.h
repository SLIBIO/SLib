/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_WEB_SERVICE
#define CHECKHEADER_SLIB_WEB_SERVICE

#include "definition.h"

#include "controller.h"

#include "../core/service.h"
#include "../network/http_service.h"

namespace slib
{
	
	class SLIB_EXPORT WebService : public Service
	{
		SLIB_DECLARE_OBJECT

	public:
		WebService();

		~WebService();

	public:
		static Ref<WebService> getApp();

	public:
		HttpServiceParam& getHttpParam();

		sl_uint16 getHttpPort();

		void setHttpPort(sl_uint16 port);
	
		void useAsset(const String& prefixForAssetPath);

		const Ref<WebController>& getController();

	protected:
		sl_bool dispatchStartService() override;

		void dispatchStopService() override;

	protected:
		Ref<HttpService> m_http;
		HttpServiceParam m_httpParam;
		Ref<WebController> m_controller;

	};

}

#endif
