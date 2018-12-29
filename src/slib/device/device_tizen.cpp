/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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

	String Device::getDeviceId()
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

	String Device::getDeviceName()
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

	String Device::getSystemVersion()
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

	String Device::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("Tizen %s", osVersion);
	}

	Size Device::getScreenSize()
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

	sl_uint32 Device::getDevicePPI()
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
