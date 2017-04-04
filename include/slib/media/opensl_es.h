/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_OPENSL_ES
#define CHECKHEADER_SLIB_MEDIA_OPENSL_ES

#include "definition.h"

#include "audio_player.h"
#include "audio_recorder.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#define SLIB_AUDIO_SUPPORT_OPENSL_ES
#endif

namespace slib
{
	class SLIB_EXPORT OpenSL_ES
	{
	public:
		static Ref<AudioPlayer> createPlayer(const AudioPlayerParam& param);
		
		static Ref<AudioRecorder> createRecorder(const AudioRecorderParam& param);
		
	};
}

#endif
