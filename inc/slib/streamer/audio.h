#ifndef CHECKHEADER_SLIB_STREAMER_MEDIA
#define CHECKHEADER_SLIB_STREAMER_MEDIA

#include "definition.h"

#include "graph.h"
#include "../audio/object.h"

SLIB_STREAMER_NAMESPACE_BEGIN

struct SLIB_EXPORT AudioRecordSourceParam : public AudioRecorderParam
{
	sl_uint32 nSamplesPerFrame;
};

class SLIB_EXPORT AudioRecordSource : public Source
{
public:
	SLIB_INLINE AudioRecordSource()
	{
		setVolume(1.0f);
	}
	
public:
	SLIB_PROPERTY_INLINE(float, Volume)

public:
	
	static Ref<AudioRecordSource> create(const AudioRecordSourceParam& param);
};

struct SLIB_EXPORT AudioPlaySinkParam : public AudioPlayerBufferParam
{
	Ref<AudioPlayer> player;
};

class SLIB_EXPORT AudioPlaySink : public Sink
{
public:
	SLIB_INLINE AudioPlaySink() {}

	
	static Ref<AudioPlaySink> create(const AudioPlaySinkParam& param);
};
SLIB_STREAMER_NAMESPACE_END

#endif
