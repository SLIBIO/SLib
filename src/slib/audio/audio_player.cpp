#include "../../../inc/slib/audio/player.h"
#include "../../../inc/slib/audio/format.h"

SLIB_AUDIO_NAMESPACE_BEGIN
AudioPlayerInfo::AudioPlayerInfo()
{
}

AudioPlayerBufferParam::AudioPlayerBufferParam()
{
	nSamplesPerSecond = 16000;
	nChannels = 1;
	nFrameLengthInMilliseconds = 50;
	nBufferLengthInMilliseconds = 1000;
	flagAutoStart = sl_true;
}

AudioPlayerParam::AudioPlayerParam()
{
}

void IAudioPlayerBufferListener::onPlayFrame(AudioPlayerBuffer *player, float *samples, sl_uint32 count)
{
}

sl_bool IAudioPlayerBufferListener::isPlayingUint8Frame()
{
	return sl_false;
}

void IAudioPlayerBufferListener::onPlayFrame(AudioPlayerBuffer *player, sl_uint8 *samples, sl_uint32 count)
{
}

sl_bool IAudioPlayerBufferListener::isPlayingInt16Frame()
{
	return sl_false;
}

void IAudioPlayerBufferListener::onPlayFrame(AudioPlayerBuffer *player, sl_int16 *samples, sl_uint32 count)
{
}

AudioPlayerBuffer::AudioPlayerBuffer()
{
	m_lastSample_S16 = 0;
}

AudioPlayerBuffer::~AudioPlayerBuffer()
{
}

sl_bool AudioPlayerBuffer::write(float* samples, sl_size count)
{
	return m_queue.add(samples, count, sl_false);
}

void AudioPlayerBuffer::_processFrame_S16(sl_int16 *samples, sl_uint32 count)
{
	Ref<Event> ev = getEvent();
	if (ev.isNotNull()) {
		ev->set();
	}
	if (count > 0) {
		PtrLocker<IAudioPlayerBufferListener> listener(getListener());
		if (listener.isNotNull()) {
			if (listener->isPlayingInt16Frame()) {
				listener->onPlayFrame(this, samples, count);
			} else if (listener->isPlayingUint8Frame()) {
				Memory mem = _getMemProcess_U8(count);
				if (mem.isNotEmpty()) {
					sl_uint8* buf = (sl_uint8*)(mem.getBuf());
					listener->onPlayFrame(this, buf, count);
					AudioFormat::convertSamples(count, buf, samples);
				}
			} else {
				Memory mem = _getMemProcess_F(count);
				if (mem.isNotEmpty()) {
					float* buf = (float*)(mem.getBuf());
					listener->onPlayFrame(this, buf, count);
					AudioFormat::convertSamples(count, buf, samples);
				}
			}
		} else {
			Memory mem = _getMemProcess_F(count);
			if (mem.isNotEmpty()) {
				float* buf = (float*)(mem.getBuf());
				if (m_queue.get(buf, count)) {
					AudioFormat::convertSamples(count, buf, samples);
				} else {
					for (sl_size i = 0; i < count; i++) {
						samples[i] = m_lastSample_S16;
					}
				}
			}
		}
		m_lastSample_S16 = samples[count - 1];
	}
}

Memory AudioPlayerBuffer::_getMemProcess_U8(sl_size count)
{
	sl_size size = count;
	Memory mem = m_memProcess_U8;
	if (mem.getSize() >= size) {
		return mem;
	} else {
		mem = Memory::create(size);
		m_memProcess_U8 = mem;
		return mem;
	}
}

Memory AudioPlayerBuffer::_getMemProcess_S16(sl_size count)
{
	sl_size size = count * 2;
	Memory mem = m_memProcess_S16;
	if (mem.getSize() >= size) {
		return mem;
	} else {
		mem = Memory::create(size);
		m_memProcess_S16 = mem;
		return mem;
	}
}

Memory AudioPlayerBuffer::_getMemProcess_F(sl_size count)
{
	sl_size size = count * 4;
	Memory mem = m_memProcess_F;
	if (mem.getSize() >= size) {
		return mem;
	} else {
		mem = Memory::create(size);
		m_memProcess_F = mem;
		return mem;
	}
}

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
}
SLIB_AUDIO_NAMESPACE_END

