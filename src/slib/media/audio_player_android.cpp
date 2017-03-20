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

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/media/audio_player.h"

#include "../../../inc/slib/media/opensl_es.h"

namespace slib
{

	Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
	{
		return OpenSL_ES::createPlayer(param);
	}

	List<AudioPlayerInfo> AudioPlayer::getPlayersList()
	{
		AudioPlayerInfo ret;
		SLIB_STATIC_STRING(s, "Internal Speaker");
		ret.name = s;
		return List<AudioPlayerInfo>::createFromElement(ret);
	}

}

#endif
