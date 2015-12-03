#ifndef CHECKHEADER_SLIB_AUDIO_OPENSL_ES
#define CHECKHEADER_SLIB_AUDIO_OPENSL_ES

#include "definition.h"
#include "player.h"
#include "recorder.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#define SLIB_AUDIO_SUPPORT_OPENSL_ES
#endif

SLIB_AUDIO_NAMESPACE_BEGIN
class SLIB_EXPORT OpenSL_ES
{
public:
	static Ref<AudioPlayer> createPlayer(const AudioPlayerParam& param);
	static Ref<AudioRecorder> createRecorder(const AudioRecorderParam& param);
};
SLIB_AUDIO_NAMESPACE_END

#endif
