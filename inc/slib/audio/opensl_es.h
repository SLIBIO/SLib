#ifndef CHECKHEADER_SLIB_AUDIO_OPENSLES
#define CHECKHEADER_SLIB_AUDIO_OPENSLES

#include "definition.h"
#include "object.h"

SLIB_AUDIO_NAMESPACE_BEGIN
class SLIB_EXPORT OpenSLES_AudioPlayer
{
public:
	static Ref<AudioPlayer> create(const AudioPlayerParam& param);
};

class SLIB_EXPORT OpenSLES_AudioRecorder
{
public:
	static Ref<AudioRecorder> create(const AudioRecorderParam& param);
};
SLIB_AUDIO_NAMESPACE_END

#endif
