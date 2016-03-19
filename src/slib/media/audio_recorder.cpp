#include "../../../inc/slib/media/audio_recorder.h"
#include "../../../inc/slib/media/audio_format.h"

SLIB_MEDIA_NAMESPACE_BEGIN

AudioRecorderInfo::AudioRecorderInfo()
{
}

AudioRecorderParam::AudioRecorderParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
	frameLengthInMilliseconds = 50;
	bufferLengthInMilliseconds = 1000;
	flagAutoStart = sl_true;
}


SLIB_DEFINE_OBJECT(AudioRecorder, Object)

sl_bool AudioRecorder::read(const AudioData& audioOut)
{
	AudioFormat format;
	sl_uint32 nChannels = m_nChannels;
	if (nChannels == 1) {
		format = AudioFormat::Int16_Mono;
	} else {
		format = AudioFormat::Int16_Stereo;
	}
	if (audioOut.format == format && (((sl_size)(audioOut.data)) & 1) == 0) {
		return m_queue.get((sl_int16*)(audioOut.data), nChannels * audioOut.count);
	} else {
		sl_int16 samples[2048];
		AudioData temp;
		temp.format = format;
		temp.data = samples;
		temp.count = 1024;
		sl_size n = audioOut.count;
		ObjectLocker lock(&m_queue);
		if (n > m_queue.getCount()) {
			while (n > 0) {
				sl_size m = n;
				if (m > 1024) {
					m = 1024;
				}
				n -= m;
				m_queue.get(samples, nChannels*m);
				audioOut.copySamplesFrom(temp, m);
			}
			return sl_true;
		}
		return sl_false;
	}
}

Array<sl_int16> AudioRecorder::_getProcessData(sl_size count)
{
	Array<sl_int16> data = m_processData;
	if (data.getCount() >= count) {
		return data;
	} else {
		data = Array<sl_int16>::create(count);
		m_processData = data;
		return data;
	}
}

void AudioRecorder::_processFrame(sl_int16* s, sl_size count)
{
	PtrLocker<IAudioRecorderListener> listener(m_listener);
	if (listener.isNotNull()) {
		AudioData audio;
		AudioFormat format;
		sl_uint32 nChannels = m_nChannels;
		if (nChannels == 1) {
			format = AudioFormat::Int16_Mono;
		} else {
			format = AudioFormat::Int16_Stereo;
		}
		audio.format = format;
		audio.count = count / nChannels;
		audio.data = s;
		listener->onRecordAudio(this, audio);
	}
	m_queue.add(s, count);
	if (m_event.isNotNull()) {
		m_event->set();
	}
}

SLIB_MEDIA_NAMESPACE_END
