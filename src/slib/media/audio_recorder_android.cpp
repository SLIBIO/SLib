#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/media/audio_recorder.h"
#include "../../../inc/slib/media/opensl_es.h"

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return OpenSL_ES::createRecorder(param);
}

List<AudioRecorderInfo> AudioRecorder::getRecordersList()
{
	AudioRecorderInfo ret;
	SLIB_STATIC_STRING(s, "Internal Microphone");
	ret.name = s;
	return List<AudioRecorderInfo>::createFromElement(ret);
}
SLIB_MEDIA_NAMESPACE_END

#endif
