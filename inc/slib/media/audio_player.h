#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_PLAYER

#include "definition.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/queue.h"
#include "../core/string.h"
#include "../core/pointer.h"

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

class IAudioPlayerBufferListener;
class SLIB_EXPORT AudioPlayerBufferParam
{
public:
	sl_uint32 nSamplesPerSecond;
	sl_uint32 nChannels;
	sl_uint32 nBufferLengthInMilliseconds;
	sl_uint32 nFrameLengthInMilliseconds;
	Ptr<IAudioPlayerBufferListener> listener;
	sl_bool flagAutoStart;
	
public:
	AudioPlayerBufferParam();
};

class SLIB_EXPORT AudioPlayerParam
{
public:
	String deviceId;
	
public:
	AudioPlayerParam();
};

class AudioPlayerBuffer;
class SLIB_EXPORT IAudioPlayerBufferListener
{
public:
	virtual void onPlayFrame(AudioPlayerBuffer* player, float* samples, sl_uint32 count);
	
	virtual sl_bool isPlayingUint8Frame();
	virtual void onPlayFrame(AudioPlayerBuffer* player, sl_uint8* samples, sl_uint32 count);
	
	virtual sl_bool isPlayingInt16Frame();
	virtual void onPlayFrame(AudioPlayerBuffer* player, sl_int16* samples, sl_uint32 count);
};

class SLIB_EXPORT AudioPlayerBuffer : public Object
{
	SLIB_DECLARE_OBJECT(AudioPlayerBuffer, Object)
protected:
	AudioPlayerBuffer();
public:
	~AudioPlayerBuffer();

public:
	virtual void release() = 0;
	virtual sl_bool isOpened() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual sl_bool isRunning() = 0;
	virtual sl_bool write(float* samples, sl_size count);

public:
	SLIB_PROPERTY_INLINE(Ref<Event>, Event)
	SLIB_PROPERTY_INLINE(Ptr<IAudioPlayerBufferListener>, Listener)
	
protected:
	void _processFrame_S16(sl_int16* samples, sl_uint32 count);
	Memory _getMemProcess_U8(sl_size count);
	Memory _getMemProcess_S16(sl_size count);
	Memory _getMemProcess_F(sl_size count);

protected:
	LoopQueue<float> m_queue;
	Memory m_memProcess_U8;
	Memory m_memProcess_S16;
	Memory m_memProcess_F;
	
	sl_int16 m_lastSample_S16;
};

class SLIB_EXPORT AudioPlayer : public Object
{
	SLIB_DECLARE_OBJECT(AudioPlayer, Object)
protected:
	AudioPlayer();
public:
	~AudioPlayer();

public:
	virtual Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) = 0;

public:
	static Ref<AudioPlayer> create(const AudioPlayerParam& param);
	static List<AudioPlayerInfo> getPlayersList();
};

SLIB_MEDIA_NAMESPACE_END

#endif
