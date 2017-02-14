#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/audio_recorder.h"

#include "../../../inc/slib/media/dsound.h"

namespace slib
{

	Ref<AudioRecorder> AudioRecorder::create(const AudioRecorderParam& param)
	{
		return DirectSound::createRecorder(param);
	}

	List<AudioRecorderInfo> AudioRecorder::getRecordersList()
	{
		return DirectSound::getRecordersList();
	}

}

#endif
