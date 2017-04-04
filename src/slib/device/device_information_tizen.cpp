/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/device/information.h"

#include "slib/core/variant.h"
#include "slib/core/platform_tizen.h"

#include <system_info.h>
#include <stdlib.h>

namespace slib
{

	String DeviceInformation::getDeviceId()
	{
		char *value = NULL;
		int ret = ::system_info_get_platform_string("http://tizen.org/system/tizenid", &value);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			String device_id = value;
			::free(value);
			return device_id;
		}
		return sl_null;
	}

	String DeviceInformation::getDeviceName()
	{
		char* platform_name = NULL;
		int ret = ::system_info_get_platform_string("http://tizen.org/system/platform.name", &platform_name);
		if (ret == SYSTEM_INFO_ERROR_NONE) {
			char* model_name = NULL;
			ret = ::system_info_get_platform_string("http://tizen.org/system/model_name", &model_name);
			if (ret == SYSTEM_INFO_ERROR_NONE) {
				String name = String::format("%s %s", platform_name, model_name);
				::free(model_name);
				return name;
			}
			::free(platform_name);
		}
		return sl_null;
	}

	String DeviceInformation::getSystemVersion()
	{
		char *value = NULL;
		int ret = ::system_info_get_platform_string("http://tizen.org/feature/platform.version", &value);
		if (ret != SYSTEM_INFO_ERROR_NONE) {
			return sl_null;
		}
		String version = value;
		::free(value);
		return version;
	}

	String DeviceInformation::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("Tizen %s", osVersion);
	}

	Size DeviceInformation::getScreenSize()
	{
		Size size;
		int value;
		int ret = ::system_info_get_platform_int("http://tizen.org/feature/screen.height", &value);
		if (ret != SYSTEM_INFO_ERROR_NONE) {
			return size;
		}
		size.x = value;
		ret = ::system_info_get_platform_int("http://tizen.org/feature/screen.width", &value);
		if (ret != SYSTEM_INFO_ERROR_NONE) {
			return size;
		}
		size.y = value;
		return size;
	}

	sl_uint32 DeviceInformation::getDevicePPI()
	{
		int value;
		int ret = ::system_info_get_platform_int("http://tizen.org/feature/screen.dpi", &value);
		if (ret != SYSTEM_INFO_ERROR_NONE) {
			return -1;
		}
		return value;
	}

}

#endif
