#ifndef CHECKHEADER_SLIB_MEDIA_DSOUND
#define CHECKHEADER_SLIB_MEDIA_DSOUND

#include "definition.h"

#include "audio_player.h"
#include "audio_recorder.h"

SLIB_MEDIA_NAMESPACE_BEGIN
class SLIB_EXPORT DirectSound
{
public:
	static Ref<AudioPlayer> createPlayer(const AudioPlayerParam& param);
	static List<AudioPlayerInfo> getPlayersList();
	
	static Ref<AudioRecorder> createRecorder(const AudioRecorderParam& param);
	static List<AudioRecorderInfo> getRecordersList();
};
SLIB_MEDIA_NAMESPACE_END

#endif
