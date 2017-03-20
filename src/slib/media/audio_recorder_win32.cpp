/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/audio_recorder.h"

#include "../../../inc/slib/media/dsound.h"

namespace slib
{

	Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
	{
		return DirectSound::createRecorder(param);
	}

	List<AudioRecorderInfo> AudioRecorder::getRecordersList()
	{
		return DirectSound::getRecordersList();
	}

}

#endif
