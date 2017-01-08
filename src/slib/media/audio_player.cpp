#include "../../../inc/slib/media/audio_player.h"
#include "../../../inc/slib/media/audio_format.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_MEDIA_NAMESPACE_BEGIN

typedef HashMap< AudioPlayerControl*, Ref<AudioPlayerControl> > _AudioPlayerControlsMap;
SLIB_SAFE_STATIC_GETTER(_AudioPlayerControlsMap, _getAudioPlayerControlsMap)

AudioPlayerInfo::AudioPlayerInfo()
{
}

AudioPlayerBufferParam::AudioPlayerBufferParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
	frameLengthInMilliseconds = 50;
	bufferLengthInMilliseconds = 1000;
	
	flagAutoStart = sl_true;
}

AudioPlayerOpenParam::AudioPlayerOpenParam()
{
	flagAutoStart = sl_true;
	flagKeepReference = sl_true;
}

AudioPlayerParam::AudioPlayerParam()
{
}


SLIB_DEFINE_OBJECT(AudioPlayerControl, Object)

void AudioPlayerControl::_removeFromMap()
{
	_AudioPlayerControlsMap* map = _getAudioPlayerControlsMap();
	if (map) {
		map->remove(this);
	}
}

SLIB_DEFINE_OBJECT(AudioPlayerBuffer, Object)

AudioPlayerBuffer::AudioPlayerBuffer()
{
	m_lastSample = 0;
}

void AudioPlayerBuffer::write(const AudioData& audioIn)
{
	AudioFormat format;
	sl_uint32 nChannels = m_nChannels;
	if (nChannels == 1) {
		format = AudioFormat::Int16_Mono;
	} else {
		format = AudioFormat::Int16_Stereo;
	}
	if (audioIn.format == format && (((sl_size)(audioIn.data)) & 1) == 0) {
		m_queue.add((sl_int16*)(audioIn.data), nChannels * audioIn.count);
	} else {
		sl_int16 samples[2048];
		AudioData temp;
		temp.format = format;
		temp.data = samples;
		temp.count = 1024;
		sl_size n = audioIn.count;
		while (n > 0) {
			sl_size m = n;
			if (m > 1024) {
				m = 1024;
			}
			n -= m;
			temp.copySamplesFrom(audioIn, m);
			m_queue.add(samples, nChannels*m);
		}
	}
}

Array<sl_int16> AudioPlayerBuffer::_getProcessData(sl_size count)
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

void AudioPlayerBuffer::_processFrame(sl_int16* s, sl_size count)
{
	if (m_event.isNotNull()) {
		m_event->set();
	}
	PtrLocker<IAudioPlayerBufferListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onPlayAudio(this, count / m_nChannels);
	}
	if (!(m_queue.get(s, count))) {
		for (sl_size i = 0; i < count; i++) {
			s[i] = m_lastSample;
		}
	}
	m_lastSample = s[count - 1];
}

SLIB_DEFINE_OBJECT(AudioPlayer, Object)

Ref<AudioPlayer> AudioPlayer::create()
{
	AudioPlayerParam param;
	return create(param);
}

Ref<AudioPlayerControl> AudioPlayer::open(const slib::AudioPlayerOpenParam &param)
{
	Ref<AudioPlayerControl> control = _openNative(param);
	
	if (param.flagKeepReference) {
		_AudioPlayerControlsMap* map = _getAudioPlayerControlsMap();
		if (map) {
			map->put(control.get(), control);
		}
	}
	
	return control;
}

Ref<AudioPlayerControl> AudioPlayer::playSound(const Memory& data)
{
	Ref<AudioPlayer> player = create();
	if (player.isNotNull()) {
		AudioPlayerOpenParam param;
		param.data = data;
		return player->open(param);
	}
	return sl_null;
}

Ref<AudioPlayerControl> AudioPlayer::playUrl(const String &url)
{
	Ref<AudioPlayer> player = create();
	if (player.isNotNull()) {
		AudioPlayerOpenParam param;
		param.url = url;
		return player->open(param);
	}
	return sl_null;
}

SLIB_MEDIA_NAMESPACE_END

