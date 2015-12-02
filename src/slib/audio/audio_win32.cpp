#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/audio/object.h"
#include "../../../inc/slib/audio/dsound.h"

#define USE_DSOUND

SLIB_AUDIO_NAMESPACE_BEGIN

#ifdef USE_DSOUND
Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return DirectSound_AudioRecorder::create(param);
}

List<AudioDeviceInfo> AudioRecorder::getDevicesList()
{
	return DirectSound_AudioRecorder::getDevicesList();
}

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return DirectSound_AudioPlayer::create(param);
}

List<AudioDeviceInfo> AudioPlayer::getDevicesList()
{
	return DirectSound_AudioPlayer::getDevicesList();
}
#endif

SLIB_AUDIO_NAMESPACE_END

#endif
