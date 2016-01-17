#ifndef CHECKHEADER_SLIB_MEDIA_OPENSL_ES
#define CHECKHEADER_SLIB_MEDIA_OPENSL_ES

#include "definition.h"

#include "audio_player.h"
#include "audio_recorder.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)
#define SLIB_AUDIO_SUPPORT_OPENSL_ES
#endif

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT OpenSL_ES
{
public:
	static Ref<AudioPlayer> createPlayer(const AudioPlayerParam& param);
	
	static Ref<AudioRecorder> createRecorder(const AudioRecorderParam& param);
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
