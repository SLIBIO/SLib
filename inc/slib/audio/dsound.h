#ifndef CHECKHEADER_SLIB_AUDIO_DSOUND
#define CHECKHEADER_SLIB_AUDIO_DSOUND

#include "definition.h"
#include "object.h"

SLIB_AUDIO_NAMESPACE_BEGIN
class SLIB_EXPORT DirectSound_AudioPlayer
{
public:
	static Ref<AudioPlayer> create(const AudioPlayerParam& param);
	static List<AudioDeviceInfo> getDevicesList();
};

class SLIB_EXPORT DirectSound_AudioRecorder
{
public:
	static Ref<AudioRecorder> create(const AudioRecorderParam& param);
	static List<AudioDeviceInfo> getDevicesList();
};
SLIB_AUDIO_NAMESPACE_END

#endif
