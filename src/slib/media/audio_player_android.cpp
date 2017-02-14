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
