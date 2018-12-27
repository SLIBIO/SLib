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

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/device/device.h"

#import <AVFoundation/AVFoundation.h>

namespace slib
{

	void Device::setAudioCategory(const DeviceAudioCategory& _category)
	{
		AVAudioSession* session = [AVAudioSession sharedInstance];
		if (session == nil) {
			return;
		}
		NSString* category;
		NSString* mode;
		switch (_category) {
			case DeviceAudioCategory::Playback:
				category = AVAudioSessionCategoryPlayback;
				mode = AVAudioSessionModeDefault;
				break;
			case DeviceAudioCategory::Record:
				category = AVAudioSessionCategoryRecord;
				mode = AVAudioSessionModeDefault;
				break;
			case DeviceAudioCategory::PlayAndRecord:
				category = AVAudioSessionCategoryPlayAndRecord;
				mode = AVAudioSessionModeVoiceChat;
				break;
			case DeviceAudioCategory::PlayVideo:
				category = AVAudioSessionCategoryPlayback;
				mode = AVAudioSessionModeMoviePlayback;
				break;
			case DeviceAudioCategory::RecordVideo:
				category = AVAudioSessionCategoryRecord;
				mode = AVAudioSessionModeVideoRecording;
				break;
			case DeviceAudioCategory::VideoChat:
				category = AVAudioSessionCategoryPlayAndRecord;
				mode = AVAudioSessionModeVideoChat;
				break;
			default:
				category = AVAudioSessionCategorySoloAmbient;
				mode = AVAudioSessionModeDefault;
				break;
		}
		@try {
			[session setCategory:category mode:mode options:0 error:nil];
		} @catch(NSError* e) {
			NSLog(@"[Exception][Device][setAudioCategory] %@", e.localizedDescription);
		}
	}

}

#endif
