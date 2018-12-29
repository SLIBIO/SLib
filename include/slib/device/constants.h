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

#ifndef CHECKHEADER_SLIB_DEVICE_CONSTANTS
#define CHECKHEADER_SLIB_DEVICE_CONSTANTS

#include "definition.h"

#include "../core/macro.h"

namespace slib
{

	enum class DeviceAudioCategory
	{
		Default = 0,
		Playback = 1,
		Record = 2,
		PlayAndRecord = 3,
		PlayVideo = 4,
		RecordVideo = 5,
		VideoChat = 6
	};
	
	class DevicePermissions
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(DevicePermissions, value)
		
		enum {
			Camera = 1,
			
			RecordAudio = (1<<1),
			
			WriteExternalStorage = (1<<2),
			ReadExternalStorage = (1<<3),
			
			ReadPhoneState = (1<<4),
			ReadPhoneNumbers = (1<<5),
			CallPhone = (1<<6),
			AnswerPhoneCalls = (1<<7),
			AddVoiceMail = (1<<8),
			UseSip = (1<<9),
			
			SendSMS = (1<<10),
			ReceiveSMS = (1<<11),
			ReadSMS = (1<<12),
			ReceiveWapPush = (1<<13),
			ReceiveMMS = (1<<14),
			
			ReadContacts = (1<<15),
			WriteContacts = (1<<16),
			GetAccounts = (1<<17),

			AccessFineLocation = (1<<18),
			AccessCoarseLocation = (1<<19),

			ReadCalendar = (1<<20),
			WriteCalendar = (1<<21),
			
			ReadCallLog = (1<<22),
			WriteCallLog = (1<<23),
			ProcessOutgoingCalls = (1<<24),
			
			BodySensors = (1<<25)
		};
	};

}

#endif
