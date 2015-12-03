#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/audio/player.h"
#include "../../../inc/slib/audio/dsound.h"

SLIB_AUDIO_NAMESPACE_BEGIN
Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return DirectSound::createPlayer(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	return DirectSound::getPlayersList();
}
SLIB_AUDIO_NAMESPACE_END

#endif
