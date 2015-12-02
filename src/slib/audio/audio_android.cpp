#include "../../../inc/slib/core/definition.h"

#define USE_OPENSL_ES

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/audio/object.h"
#include "../../../inc/slib/audio/opensl_es.h"

SLIB_AUDIO_NAMESPACE_BEGIN

#ifdef USE_OPENSL_ES
Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return OpenSLES_AudioRecorder::create(param);
}

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return OpenSLES_AudioPlayer::create(param);
}
#endif

List<AudioDeviceInfo> AudioRecorder::getDevicesList()
{
	AudioDeviceInfo ret;
	SLIB_STATIC_STRING(s, "Internal Microphone");
	ret.name = s;
	return List<AudioDeviceInfo>::fromElement(ret);
}

List<AudioDeviceInfo> AudioPlayer::getDevicesList()
{
	AudioDeviceInfo ret;
	SLIB_STATIC_STRING(s, "Internal Speaker");
	ret.name = s;
	return List<AudioDeviceInfo>::fromElement(ret);
}

SLIB_AUDIO_NAMESPACE_END

#endif
