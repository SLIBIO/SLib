#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/audio_recorder.h"
#include "../../../inc/slib/media/dsound.h"

SLIB_MEDIA_NAMESPACE_BEGIN
Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
{
	return DirectSound::createRecorder(param);
}

List<AudioRecorderInfo> AudioRecorder::getRecordersList()
{
	return DirectSound::getRecordersList();
}
SLIB_MEDIA_NAMESPACE_END

#endif
