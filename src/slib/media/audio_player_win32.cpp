#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/audio_player.h"
#include "../../../inc/slib/media/dsound.h"

SLIB_MEDIA_NAMESPACE_BEGIN
Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return DirectSound::createPlayer(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	return DirectSound::getPlayersList();
}
SLIB_MEDIA_NAMESPACE_END

#endif
