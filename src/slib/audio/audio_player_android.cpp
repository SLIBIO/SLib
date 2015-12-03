#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/audio/player.h"
#include "../../../inc/slib/audio/opensl_es.h"

SLIB_AUDIO_NAMESPACE_BEGIN
Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return OpenSL_ES::createPlayer(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	AudioPlayerInfo ret;
	SLIB_STATIC_STRING(s, "Internal Speaker");
	ret.name = s;
	return List<AudioPlayerInfo>::fromElement(ret);
}
SLIB_AUDIO_NAMESPACE_END

#endif
