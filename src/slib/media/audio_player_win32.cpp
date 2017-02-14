#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/audio_player.h"

#include "../../../inc/slib/media/dsound.h"

namespace slib
{

	Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
	{
		return DirectSound::createPlayer(param);
	}

	List<AudioPlayerInfo> AudioPlayer::getPlayersList()
	{
		return DirectSound::getPlayersList();
	}

}

#endif
