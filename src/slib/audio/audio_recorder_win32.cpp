#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/audio/recorder.h"
#include "../../../inc/slib/audio/dsound.h"

SLIB_AUDIO_NAMESPACE_BEGIN
Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return DirectSound::createRecorder(param);
}

List<AudioRecorderInfo> AudioRecorder::getRecordersList()
{
	return DirectSound::getRecordersList();
}
SLIB_AUDIO_NAMESPACE_END

#endif
