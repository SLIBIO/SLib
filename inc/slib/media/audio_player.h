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

namespace slib
{
	class SLIB_EXPORT AudioPlayerInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		AudioPlayerInfo();
		
		~AudioPlayerInfo();
		
	};
	
	class AudioPlayerBuffer;
	
	class SLIB_EXPORT IAudioPlayerBufferListener
	{
	public:
		IAudioPlayerBufferListener();

		virtual ~IAudioPlayerBufferListener();

	public:
		virtual void onRequireAudioData(AudioPlayerBuffer* player, sl_size requestedSamplesCount) = 0;
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
		Function<void(AudioPlayerBuffer*, sl_size)> onRequireAudioData;
		Ref<Event> event;
		
	public:
		AudioPlayerBufferParam();
		
		~AudioPlayerBufferParam();
		
	};
	
	class SLIB_EXPORT AudioPlayerBuffer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AudioPlayerBuffer();
		
		~AudioPlayerBuffer();
		
	public:
		virtual void release() = 0;
		
		virtual sl_bool isOpened() = 0;
		
		virtual void start() = 0;
		
		virtual void stop() = 0;
		
		virtual sl_bool isRunning() = 0;
		
	public:
		void write(const AudioData& audioPlay);
		
	protected:
		void _init(const AudioPlayerBufferParam& param);
		
		Array<sl_int16> _getProcessData(sl_size count);
		
		void _processFrame(sl_int16* s, sl_size count);
		
	protected:
		LoopQueue<sl_int16> m_queue;
		sl_int16 m_lastSample;
		sl_uint32 m_nChannels;
		AtomicArray<sl_int16> m_processData;
		
		Ptr<IAudioPlayerBufferListener> m_listener;
		Function<void(AudioPlayerBuffer*, sl_size)> m_onRequireAudioData;
		Ref<Event> m_event;
		
	};
	
	class SLIB_EXPORT AudioPlayerParam
	{
	public:
		String deviceId;
		
	public:
		AudioPlayerParam();
		
		~AudioPlayerParam();
		
	};
	
	class SLIB_EXPORT AudioPlayer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AudioPlayer();
		
		~AudioPlayer();
		
	public:
		static Ref<AudioPlayer> create(const AudioPlayerParam& param);
		
		static Ref<AudioPlayer> create();
		
		static List<AudioPlayerInfo> getPlayersList();
		
	public:
		virtual Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) = 0;
		
	};	
}

#endif
