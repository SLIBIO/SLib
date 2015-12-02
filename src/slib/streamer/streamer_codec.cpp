#include "../../../inc/slib/streamer/definition.h"
#include "../../../inc/slib/streamer/codec.h"
#include "../../../inc/slib/core/scoped_pointer.h"

#define STACK_BUFFER_SIZE 4096

SLIB_STREAMER_NAMESPACE_BEGIN
List<Packet> AudioOpusEncodeFilter::filter(const Packet& input)
{
	Ref<AudioOpusEncoder> encoder = m_encoder;
	if (encoder.isNull()) {
		return List<Packet>::null();
	}
	if (input.format != Packet::formatAudio_PCM_S16 
		|| input.data.getSize() % 2 != 0
		|| input.audioParam.nChannels != encoder->getChannelsCount()
		|| input.audioParam.nSamplesPerSecond != encoder->getSamplesCountPerSecond()) {
		return List<Packet>::null();
	}
	sl_uint32 nSamples = (sl_uint32)(input.data.getSize()) / 2;
	sl_uint32 nInput = nSamples;
	sl_uint32 nOutput = nInput * 2;
	SLIB_SCOPED_BUFFER(char, STACK_BUFFER_SIZE, enc, nOutput);
	if (!enc) {
		return List<Packet>::null();
	}
	if (!encoder->encode((const sl_int16*)(input.data.getBuf()), nInput, enc, nOutput)) {
		return List<Packet>::null();
	}
	if (nInput != nSamples) {
		return List<Packet>::null();
	}
	Packet output;
	output.format = Packet::formatAudio_OPUS;
	output.audioParam = input.audioParam;
	output.data = Memory::create(enc, nOutput);
	List<Packet> ret;
	ret.add(output);
	return ret;
}
SLIB_STREAMER_NAMESPACE_END


SLIB_STREAMER_NAMESPACE_BEGIN
List<Packet> AudioOpusDecodeFilter::filter(const Packet& input)
{
	Ref<AudioOpusDecoder> decoder = m_decoder;
	if (decoder.isNull()) {
		return List<Packet>::null();
	}
	if (input.format != Packet::formatAudio_OPUS && input.format != Packet::formatRaw) {
		return List<Packet>::null();
	}
	if (input.format == Packet::formatAudio_OPUS) {
		if (input.audioParam.nChannels != decoder->getChannelsCount()
			|| input.audioParam.nSamplesPerSecond != decoder->getSamplesCountPerSecond()) {
			return List<Packet>::null();
		}
	}
	sl_uint32 nInput = (sl_uint32)(input.data.getSize());
	sl_uint32 nOutput = getMaxSamplesPerFrame();
	SLIB_SCOPED_BUFFER(sl_int16, STACK_BUFFER_SIZE, enc, nOutput);
	if (!enc) {
		return List<Packet>::null();
	}
	if (!decoder->decode(input.data.getBuf(), nInput, enc, nOutput)) {
		return List<Packet>::null();
	}
	Packet output;
	output.format = Packet::formatAudio_PCM_S16;
	output.audioParam.nChannels = decoder->getChannelsCount();
	output.audioParam.nSamplesPerSecond = decoder->getSamplesCountPerSecond();
	output.data = Memory::create(enc, nOutput * 2);
	return List<Packet>::fromElement(output);
}
SLIB_STREAMER_NAMESPACE_END

