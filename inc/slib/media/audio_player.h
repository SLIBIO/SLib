#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/array.h"
#include "../core/loop_queue.h"
#include "../core/string.h"
#include "../core/memory.h"
#include "../core/ptr.h"
#include "../core/function.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT AudioPlayerInfo
{
public:
	String id;
	String name;
	String description;

public:
	AudioPlayerInfo();
	
};

class AudioPlayerBuffer;

class SLIB_EXPORT IAudioPlayerBufferListener
{
public:
	virtual void onPlayAudio(AudioPlayerBuffer* player, sl_size requestedSamplesCount) = 0;
};

class SLIB_EXPORT AudioPlayerBufferParam
{
public:
	sl_uint32 samplesPerSecond;
	sl_uint32 channelsCount;
	sl_uint32 bufferLengthInMilliseconds;
	sl_uint32 frameLengthInMilliseconds;
	
	sl_bool flagAutoStart;

	Ptr<IAudioPlayerBufferListener> listener;
	Ref<Event> event;
	
public:
	AudioPlayerBufferParam();
	
};

class AudioPlayerControl;

class SLIB_EXPORT AudioPlayerOpenParam
{
public:
	Memory data;
	String url;
	
	sl_bool flagAutoStart;
	sl_bool flagKeepReference;
	
	AtomicFunction<void(AudioPlayerControl*)> onReadyToPlay;
	
public:
	AudioPlayerOpenParam();
	
};

class SLIB_EXPORT AudioPlayerParam
{
public:
	String deviceId;
	
public:
	AudioPlayerParam();
	
};

class SLIB_EXPORT AudioPlayerControl : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	AudioPlayerControl();
	
public:
	virtual void stop() = 0;
	
	virtual void resume() = 0;
	
	virtual void pause() = 0;
	
	virtual sl_bool isPlaying() = 0;
	
public:
	void _removeFromMap();
};

class SLIB_EXPORT AudioPlayerBuffer : public Object
{
	SLIB_DECLARE_OBJECT

protected:
	AudioPlayerBuffer();
	
public:
	virtual void release() = 0;
	
	virtual sl_bool isOpened() = 0;
	
	virtual void start() = 0;
	
	virtual void stop() = 0;
	
	virtual sl_bool isRunning() = 0;

public:
	void write(const AudioData& audioPlay);

protected:
	Array<sl_int16> _getProcessData(sl_size count);
	
	void _processFrame(sl_int16* s, sl_size count);
	
protected:
	LoopQueue<sl_int16> m_queue;
	sl_int16 m_lastSample;
	sl_uint32 m_nChannels;
	AtomicArray<sl_int16> m_processData;
	
	Ptr<IAudioPlayerBufferListener> m_listener;
	Ref<Event> m_event;
	
};

class SLIB_EXPORT AudioPlayer : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	static Ref<AudioPlayer> create(const AudioPlayerParam& param);
	
	static Ref<AudioPlayer> create();
	
	static List<AudioPlayerInfo> getPlayersList();
	
public:
	virtual Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) = 0;
	
	Ref<AudioPlayerControl> open(const AudioPlayerOpenParam& param);
	
	static Ref<AudioPlayerControl> playSound(const Memory& data, sl_bool flagAutoPlay = sl_true);
	
	static Ref<AudioPlayerControl> playUrl(const String& url, sl_bool flagAutoPlay = sl_true);
	
protected:
	virtual Ref<AudioPlayerControl> _openNative(const AudioPlayerOpenParam& param) = 0;
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
