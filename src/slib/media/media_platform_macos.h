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
