#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER
#define CHECKHEADER_SLIB_MEDIA_AUDIO_RECORDER

#include "definition.h"
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

class IAudioRecorderListener;
class SLIB_EXPORT AudioRecorderParam
{
public:
	String deviceId;
	sl_uint32 samplesPerSecond;
	sl_uint32 frameLengthInMilliseconds;
	sl_uint32 bufferLengthInMilliseconds;
	Ptr<IAudioRecorderListener> listener;
	sl_bool flagAutoStart;
	
public:
	AudioRecorderParam();
};

class AudioRecorder;
class SLIB_EXPORT IAudioRecorderListener
{
public:
	virtual void onRecordFrame(AudioRecorder* recorder, const float* samples, sl_uint32 count);
	
	virtual sl_bool isRecordingtUint8Frame();
	virtual void onRecordFrame(AudioRecorder* recorder, const sl_uint8* samples, sl_uint32 count);
	
	virtual sl_bool isRecordingInt16Frame();
	virtual void onRecordFrame(AudioRecorder* recorder, const sl_int16* samples, sl_uint32 count);
};

class SLIB_EXPORT AudioRecorder : public Object
{
	SLIB_DECLARE_OBJECT(AudioRecorder, Object)
protected:
	AudioRecorder();
public:
	~AudioRecorder();

public:
	virtual void release() = 0;
	virtual sl_bool isOpened() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual sl_bool isRunning() = 0;
	virtual sl_bool read(float* samples, sl_size count);

public:
	SLIB_PROPERTY_INLINE(Ref<Event>, Event)
	SLIB_PROPERTY_INLINE(Ptr<IAudioRecorderListener>, Listener)
	
protected:
	void _processFrame_S16(const sl_int16* samples, sl_uint32 count);
	Memory _getMemProcess_U8(sl_size count);
	Memory _getMemProcess_S16(sl_size count);
	Memory _getMemProcess_F(sl_size count);

protected:
	LoopQueue<float> m_queue;
	Memory m_memProcess_U8;
	Memory m_memProcess_S16;
	Memory m_memProcess_F;

public:
	static Ref<AudioRecorder> create(const AudioRecorderParam& param);
	static List<AudioRecorderInfo> getRecordersList();
};

SLIB_MEDIA_NAMESPACE_END

#endif
