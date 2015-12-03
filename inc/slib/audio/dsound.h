#ifndef CHECKHEADER_SLIB_AUDIO_DSOUND
#define CHECKHEADER_SLIB_AUDIO_DSOUND

#include "definition.h"
#include "player.h"
#include "recorder.h"

SLIB_AUDIO_NAMESPACE_BEGIN
class SLIB_EXPORT DirectSound
{
public:
	static Ref<AudioPlayer> createPlayer(const AudioPlayerParam& param);
	static List<AudioPlayerInfo> getPlayersList();
	
	static Ref<AudioRecorder> createRecorder(const AudioRecorderParam& param);
	static List<AudioRecorderInfo> getRecordersList();
};
SLIB_AUDIO_NAMESPACE_END

#endif
