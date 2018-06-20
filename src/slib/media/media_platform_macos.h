/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/definition.h"

#if defined(SLIB_PLATFORM_IS_MACOS)

#include "slib/core/list.h"
#include "slib/core/string.h"

#import <Foundation/Foundation.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioToolbox/AudioConverter.h>

namespace slib
{

	class _priv_macOS_AudioDeviceInfo
	{
	public:
		AudioDeviceID id;
		String uid;
		String name;
		String manufacturer;
		
	public:
		sl_bool getDeviceInfo(AudioDeviceID deviceID, sl_bool flagInput);
		
		sl_bool getDefaultDevice(sl_bool flagInput);
		
		sl_bool selectDevice(sl_bool flagInput, String uid);

	public:
		static List<_priv_macOS_AudioDeviceInfo> getAllDevices(sl_bool flagInput);
		
	private:
		static void logError(String text);
		
	};

}

#endif
