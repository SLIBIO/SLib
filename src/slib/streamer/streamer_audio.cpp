#include "../../../inc/slib/streamer/definition.h"
#include "../../../inc/slib/streamer/audio.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_STREAMER_NAMESPACE_BEGIN

#define STACK_BUFFER_SIZE 4096

class _AudioRecordSourceImpl : public AudioRecordSource
{
private:
	_AudioRecordSourceImpl()
	{
	}

public:
	Ref<AudioRecorder> m_recorder;
	sl_uint32 m_nSamplesPerFrame;
	sl_uint32 m_nSamplesPerSecond;

	~_AudioRecordSourceImpl()
	{
		m_recorder->release();
	}

	static Ref<_AudioRecordSourceImpl> create(const AudioRecordSourceParam& param)
	{
		Ref<AudioRecorder> recorder = AudioRecorder::create(param);
		if (recorder.isNull()) {
			return Ref<_AudioRecordSourceImpl>::null();
		}
		Ref<_AudioRecordSourceImpl> ret = new _AudioRecordSourceImpl();
		if (ret.isNotNull()) {
			ret->m_recorder = recorder;
			ret->m_nSamplesPerFrame = param.nSamplesPerFrame;
			ret->m_nSamplesPerSecond = param.nSamplesPerSecond;
		} else {
			recorder->release();
		}
		return ret;
	}

	sl_bool receivePacket(Packet* out)
	{
		sl_uint32 n = m_nSamplesPerFrame;
		SLIB_SCOPED_BUFFER(float, STACK_BUFFER_SIZE, buf, n);
		if (!buf) {
			return sl_false;
		}
		if (m_recorder->read(buf, n)) {
			Memory mem = Memory::create(n * 2);
			if (mem.isNotEmpty()) {
				float volume = getVolume();
				sl_int16* s = (sl_int16*)(mem.getBuf());
				for (sl_uint32 i = 0; i < n; i++) {
					float f = buf[i] * 32768.0f * volume;
					if (f > 32766.0f) {
						f = 32766.0f;
					}
					if (f < -32767.0f) {
						f = -32767.0f;
					}
					s[i] = (sl_int16)(f);
				}
				out->data = mem;
				out->format = Packet::formatAudio_PCM_S16;
				out->audioParam.nChannels = 1;
				out->audioParam.nSamplesPerSecond = m_nSamplesPerSecond;
				return sl_true;
			}
		}
		return sl_false;
	}

	void setEvent(const Ref<Event>& ev)
	{
		m_recorder->setEvent(ev);
		AudioRecordSource::setEvent(ev);
	}
};

Ref<AudioRecordSource> AudioRecordSource::create(const AudioRecordSourceParam& param)
{
	return Ref<AudioRecordSource>::from(_AudioRecordSourceImpl::create(param));
}

SLIB_STREAMER_NAMESPACE_END



SLIB_STREAMER_NAMESPACE_BEGIN

class _AudioPlaySinkImpl : public AudioPlaySink
{
private:
	_AudioPlaySinkImpl()
	{
	}

public:
	Ref<AudioPlayerBuffer> m_playerBuffer;
	sl_uint32 m_nSamplesPerSecond;
	sl_uint32 m_nChannels;

	~_AudioPlaySinkImpl()
	{
		m_playerBuffer->release();
	}

	static Ref<_AudioPlaySinkImpl> create(const AudioPlaySinkParam& param)
	{
		Ref<AudioPlayer> player = param.player;
		if (player.isNull()) {
			return Ref<_AudioPlaySinkImpl>::null();
		}
		Ref<AudioPlayerBuffer> playerBuffer = player->createBuffer(param);
		if (playerBuffer.isNull()) {
			return Ref<_AudioPlaySinkImpl>::null();
		}
		Ref<_AudioPlaySinkImpl> ret = new _AudioPlaySinkImpl();
		if (ret.isNotNull()) {
			ret->m_playerBuffer = playerBuffer;
			ret->m_nSamplesPerSecond = param.nSamplesPerSecond;
			ret->m_nChannels = param.nChannels;
		} else {
			playerBuffer->release();
		}
		return ret;
	}

	sl_bool sendPacket(const Packet& input)
	{
		if (input.format != Packet::formatAudio_PCM_S16) {
			return sl_false;
		}
		if (input.audioParam.nSamplesPerSecond != m_nSamplesPerSecond) {
			return sl_false;
		}
		if (input.audioParam.nChannels != m_nChannels) {
			return sl_false;
		}
		sl_uint32 n = (sl_uint32)(input.data.getSize()) / 2;
		if (n > 0) {
			sl_int16* s = (sl_int16*)(input.data.getBuf());
			SLIB_SCOPED_BUFFER(float, STACK_BUFFER_SIZE, frame, n);
			if (!frame) {
				return sl_false;
			}
			for (sl_uint32 i = 0; i < n; i++) {
				sl_int16 v16 = ((sl_int16*)(s))[i];
				frame[i] = (float)(v16) / 32768.0f;
			}
			m_playerBuffer->write(frame, n);
			return sl_true;
		}
		return sl_false;
	}

	void setEvent(const Ref<Event>& ev)
	{
		m_playerBuffer->setEvent(ev);
		AudioPlaySink::setEvent(ev);
	}
};

Ref<AudioPlaySink> AudioPlaySink::create(const AudioPlaySinkParam& param)
{
	return Ref<AudioPlaySink>::from(_AudioPlaySinkImpl::create(param));
}

SLIB_STREAMER_NAMESPACE_END

