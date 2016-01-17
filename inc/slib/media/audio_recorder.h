#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/event.h"
#include "../core/queue.h"
#include "../core/string.h"
#include "../core/pointer.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT AudioRecorderInfo
{
public:
	String id;
	String name;
	String description;

public:
	AudioRecorderInfo();
};

class AudioRecorder;

class SLIB_EXPORT IAudioRecorderListener
{
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
	Ref<Event> event;
	
public:
	AudioRecorderParam();
};

class SLIB_EXPORT AudioRecorder : public Object
{
	SLIB_DECLARE_OBJECT(AudioRecorder, Object)
	
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
	Array<sl_int16> _getProcessData(sl_size count);
	
	void _processFrame(sl_int16* s, sl_size count);
	
protected:
	LoopQueue<sl_int16> m_queue;
	sl_uint32 m_nChannels;
	SafeArray<sl_int16> m_processData;
	
	Ptr<IAudioRecorderListener> m_listener;
	Ref<Event> m_event;
};

SLIB_MEDIA_NAMESPACE_END

#endif
