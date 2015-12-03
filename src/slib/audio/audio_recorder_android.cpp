#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/audio/recorder.h"
#include "../../../inc/slib/audio/opensl_es.h"

SLIB_AUDIO_NAMESPACE_BEGIN
Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return OpenSL_ES::createRecorder(param);
}

List<AudioRecorderInfo> AudioRecorder::getRecordersList()
{
	AudioRecorderInfo ret;
	SLIB_STATIC_STRING(s, "Internal Microphone");
	ret.name = s;
	return List<AudioRecorderInfo>::fromElement(ret);
}
SLIB_AUDIO_NAMESPACE_END

#endif
