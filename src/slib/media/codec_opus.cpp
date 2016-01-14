#include "../../../inc/slib/media/codec_opus.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/time.h"

#include "../../../inc/thirdparty/opus/opus.h"

//#define OPUS_RESET_INTERVAL 10000

SLIB_MEDIA_NAMESPACE_BEGIN

AudioOpusEncoderParam::AudioOpusEncoderParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
	bitsPerSecond = 8000;
	type = typeVoice;
}

AudioOpusEncoder::AudioOpusEncoder()
{
}

sl_bool AudioOpusEncoder::isValidSamplingRate(sl_uint32 nSamplesPerSecond)
{
	if (nSamplesPerSecond == 8000
		||nSamplesPerSecond == 12000
		|| nSamplesPerSecond == 16000
		|| nSamplesPerSecond == 24000
		|| nSamplesPerSecond == 48000) {
		return sl_true;
	} else {
		return sl_false;
	}
}

class _OpusAudioEncoderImpl : public AudioOpusEncoder
{
public:
	_OpusAudioEncoderImpl()
	{
	}

public:
	sl_size m_sizeEncoder;
	OpusEncoder* m_encoder;
	
#ifdef OPUS_RESET_INTERVAL
	OpusEncoder* m_encoderBackup;
	TimeCounter m_timeStartReset;
#endif

	~_OpusAudioEncoderImpl()
	{
		Base::freeMemory(m_encoder);
#ifdef OPUS_RESET_INTERVAL
		Base::freeMemory(m_encoderBackup);
#endif
	}

	static void logError(String str)
	{
		SLIB_LOG_ERROR("AudioOpusEncoder", str);
	}

	static Ref<_OpusAudioEncoderImpl> create(const AudioOpusEncoderParam& param)
	{
		if (!isValidSamplingRate(param.samplesPerSecond)) {
			logError("Encoding sampling rate must be one of 8000, 12000, 16000, 24000, 48000");
			return Ref<_OpusAudioEncoderImpl>::null();
		}
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			logError("Encoding channel must be 1 or 2");
			return Ref<_OpusAudioEncoderImpl>::null();
		}
		
		int sizeEncoder = opus_encoder_get_size((opus_int32)(param.channelsCount));
		if (sizeEncoder <= 0) {
			return Ref<_OpusAudioEncoderImpl>::null();
		}
		
		OpusEncoder* encoder = (OpusEncoder*)(Base::createMemory(sizeEncoder));
		if (encoder) {
			OpusEncoder* encoderBackup = (OpusEncoder*)(Base::createMemory(sizeEncoder));
			if (encoderBackup) {
				int app = OPUS_APPLICATION_VOIP;
				if (param.type == param.typeMusic) {
					app = OPUS_APPLICATION_AUDIO;
				}
				int error = opus_encoder_init(encoder, (opus_int32)(param.samplesPerSecond), (opus_int32)(param.channelsCount), app);
				if (error == OPUS_OK) {
					
					if (app == OPUS_SIGNAL_VOICE) {
						opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
					}
					
					Ref<_OpusAudioEncoderImpl> ret = new _OpusAudioEncoderImpl();

					if (ret.isNotNull()) {
						
						ret->m_sizeEncoder = sizeEncoder;
						ret->m_encoder = encoder;
						
#ifdef OPUS_RESET_INTERVAL
						ret->m_encoderBackup = encoderBackup;
						Base::copyMemory(encoderBackup, encoder, sizeEncoder);
						ret->m_timeStartReset.reset();
#endif
						
						ret->m_nSamplesPerSecond = param.samplesPerSecond;
						ret->m_nChannels = param.channelsCount;
						
						ret->setBitrate(param.bitsPerSecond);
						
						return ret;
					}
				}
				Base::freeMemory(encoderBackup);
			}
			Base::freeMemory(encoder);
		}
		return Ref<_OpusAudioEncoderImpl>::null();
	}
	
	sl_bool encode(const sl_int16* input, sl_uint32& countInput, void* output, sl_uint32& sizeOutput)
	{
		sl_uint32 u = m_nSamplesPerSecond / 100;
		sl_uint32 n = countInput / u;
		if (n == 0) {
			return sl_false;
		}
		n *= u;
		int ret = opus_encode(m_encoder, input, (int)(n), (unsigned char*)output, (opus_int32)sizeOutput);
		if (ret > 1) {
			sizeOutput = ret;
			countInput = n;
		}
#ifdef OPUS_RESET_INTERVAL
		if (m_timeStartReset.getEllapsedMilliseconds() > OPUS_RESET_INTERVAL || ret <= 0) {
			Base::copyMemory(m_encoder, m_encoderBackup, m_sizeEncoder);
			m_timeStartReset.reset();
		}
#endif
		return ret > 1 ? sl_true : sl_false;
	}

	void setBitrate(const sl_uint32& _bitrate)
	{
		sl_uint32 bitrate = _bitrate;
		if (bitrate < 500) {
			bitrate = 500;
		}
		if (bitrate > 512000) {
			bitrate = 512000;
		}
		opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(bitrate));
		AudioEncoder::setBitrate(bitrate);
	}
	
};

Ref<AudioOpusEncoder> AudioOpusEncoder::create(const AudioOpusEncoderParam& param)
{
	return _OpusAudioEncoderImpl::create(param);
}


AudioOpusDecoderParam::AudioOpusDecoderParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
}

AudioOpusDecoder::AudioOpusDecoder()
{
}

sl_bool AudioOpusDecoder::isValidSamplingRate(sl_uint32 nSamplesPerSecond)
{
	return AudioOpusEncoder::isValidSamplingRate(nSamplesPerSecond);
}

class _OpusAudioDecoderImpl : public AudioOpusDecoder
{
private:
	_OpusAudioDecoderImpl()
	{
	}

public:
	OpusDecoder* m_decoder; 
	
	~_OpusAudioDecoderImpl()
	{
		opus_decoder_destroy(m_decoder);
	}

	static void logError(String str)
	{
		SLIB_LOG_ERROR("AudioOpusDecoder", str);
	}

	static Ref<_OpusAudioDecoderImpl> create(const AudioOpusDecoderParam& param)
	{
		if (!isValidSamplingRate(param.samplesPerSecond)) {
			logError("Decoding sampling rate must be one of 8000, 12000, 16000, 24000, 48000");
			return Ref<_OpusAudioDecoderImpl>::null();
		}
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			logError("Decoding channel must be 1 or 2");
			return Ref<_OpusAudioDecoderImpl>::null();
		}
		int error;
		OpusDecoder* decoder = opus_decoder_create((opus_int32)(param.samplesPerSecond), (opus_int32)(param.channelsCount), &error);
		if (! decoder) {
			return Ref<_OpusAudioDecoderImpl>::null();
		}
		Ref<_OpusAudioDecoderImpl> ret = new _OpusAudioDecoderImpl();
		if (ret.isNotNull()) {
			ret->m_decoder = decoder;
			ret->m_nSamplesPerSecond = param.samplesPerSecond;
			ret->m_nChannels = param.channelsCount;
		} else {
			opus_decoder_destroy(decoder);
		}
		return ret;
	}

	sl_bool decode(const void* input, sl_uint32& sizeInput, sl_int16* output, sl_uint32& countOutput)
	{
		int ret = opus_decode(m_decoder, (unsigned char *)input, (int)sizeInput, output, (opus_int32)countOutput, 0);
		if (ret > 0) {
			countOutput = ret;
			return sl_true;
		} else {
			return sl_false;
		}
	}
};

Ref<AudioOpusDecoder> AudioOpusDecoder::create(const AudioOpusDecoderParam& param)
{
	return _OpusAudioDecoderImpl::create(param);
}

SLIB_MEDIA_NAMESPACE_END

