#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/loop_queue.h"
#include "../core/string.h"
#include "../core/ptr.h"
#include "../core/function.h"

namespace slib
{
	class SLIB_EXPORT AudioRecorderInfo
	{
	public:
		String id;
		String name;
		String description;
		
	public:
		AudioRecorderInfo();
		
		~AudioRecorderInfo();
		
	};
	
	class AudioRecorder;
	
	class SLIB_EXPORT IAudioRecorderListener
	{
	public:
		IAudioRecorderListener();

		virtual ~IAudioRecorderListener();

	public:
		virtual void onRecordAudio(AudioRecorder* recorder, const AudioData& audio) = 0;
	};
	
	class SLIB_EXPORT AudioRecorderParam
	{
	public:
		String deviceId;
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		sl_uint32 frameLengthInMilliseconds;
		sl_uint32 bufferLengthInMilliseconds;
		
		sl_bool flagAutoStart;
		
		Ptr<IAudioRecorderListener> listener;
		Function<void(AudioRecorder*, AudioData const&)> onRecordAudio;
		Ref<Event> event;
		
	public:
		AudioRecorderParam();
		
		~AudioRecorderParam();
		
	};
	
	class SLIB_EXPORT AudioRecorder : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AudioRecorder();
		
		~AudioRecorder();
		
	public:
		static Ref<AudioRecorder> create(const AudioRecorderParam& param);
		
		static List<AudioRecorderInfo> getRecordersList();
		
	public:
		virtual void release() = 0;
		
		virtual sl_bool isOpened() = 0;
		
		virtual void start() = 0;
		
		virtual void stop() = 0;
		
		virtual sl_bool isRunning() = 0;
		
	public:
		sl_bool read(const AudioData& audio);
		
	protected:
		void _init(const AudioRecorderParam& param);
		
		Array<sl_int16> _getProcessData(sl_uint32 count);
		
		void _processFrame(sl_int16* s, sl_uint32 count);
		
	protected:
		LoopQueue<sl_int16> m_queue;
		sl_uint32 m_nChannels;
		AtomicArray<sl_int16> m_processData;
		
		Ptr<IAudioRecorderListener> m_listener;
		Function<void(AudioRecorder*, AudioData const&)> m_onRecordAudio;
		Ref<Event> m_event;
		
	};	
}

#endif
