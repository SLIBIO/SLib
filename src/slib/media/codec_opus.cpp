#include "../../../inc/slib/media/codec_opus.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/time.h"
#include "../../../inc/slib/core/scoped.h"

#include "../../../inc/thirdparty/opus/opus.h"

//#define OPUS_RESET_INTERVAL 10000

SLIB_MEDIA_NAMESPACE_BEGIN

OpusEncoderParam::OpusEncoderParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
	bitsPerSecond = 8000;
	type = OpusEncoderType::Voice;
}

sl_bool OpusEncoder::isValidSamplingRate(sl_uint32 nSamplesPerSecond)
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

class _OpusEncoderImpl : public OpusEncoder
{
public:
	sl_size m_sizeEncoder;
	::OpusEncoder* m_encoder;
	
#ifdef OPUS_RESET_INTERVAL
	OpusEncoder* m_encoderBackup;
	TimeCounter m_timeStartReset;
#endif
	
	sl_bool m_flagResetBitrate;
	
public:
	_OpusEncoderImpl()
	{
		m_flagResetBitrate = sl_false;
	}

	~_OpusEncoderImpl()
	{
		Base::freeMemory(m_encoder);
#ifdef OPUS_RESET_INTERVAL
		Base::freeMemory(m_encoderBackup);
#endif
	}

public:
	static void logError(String str)
	{
		SLIB_LOG_ERROR("AudioOpusEncoder", str);
	}

	static Ref<_OpusEncoderImpl> create(const OpusEncoderParam& param)
	{
		if (!isValidSamplingRate(param.samplesPerSecond)) {
			logError("Encoding sampling rate must be one of 8000, 12000, 16000, 24000, 48000");
			return Ref<_OpusEncoderImpl>::null();
		}
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			logError("Encoding channel must be 1 or 2");
			return Ref<_OpusEncoderImpl>::null();
		}
		
		int sizeEncoder = opus_encoder_get_size((opus_int32)(param.channelsCount));
		if (sizeEncoder <= 0) {
			return Ref<_OpusEncoderImpl>::null();
		}
		
		::OpusEncoder* encoder = (::OpusEncoder*)(Base::createMemory(sizeEncoder));
		if (encoder) {
			::OpusEncoder* encoderBackup = (::OpusEncoder*)(Base::createMemory(sizeEncoder));
			if (encoderBackup) {
				int app = OPUS_APPLICATION_VOIP;
				if (param.type == OpusEncoderType::Music) {
					app = OPUS_APPLICATION_AUDIO;
				}
				int error = ::opus_encoder_init(encoder, (opus_int32)(param.samplesPerSecond), (opus_int32)(param.channelsCount), app);
				if (error == OPUS_OK) {
					
					if (app == OPUS_SIGNAL_VOICE) {
						::opus_encoder_ctl(encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
					}
					
					Ref<_OpusEncoderImpl> ret = new _OpusEncoderImpl();

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
		return Ref<_OpusEncoderImpl>::null();
	}
	
	Memory encode(const AudioData& input)
	{
		sl_uint32 lenMinFrame = m_nSamplesPerSecond / 400; // 2.5 ms
		if (input.count % lenMinFrame == 0) {
			sl_uint32 nMinFrame = (sl_uint32)(input.count / lenMinFrame);
			// one frame is (2.5, 5, 10, 20, 40 or 60 ms) of audio data
			if (nMinFrame == 1 || nMinFrame == 2 || nMinFrame == 4 || nMinFrame == 8 || nMinFrame == 16 || nMinFrame == 24) {
				
				AudioData audio;
				audio.count = input.count;
				
				sl_bool flagFloat = AudioFormats::isFloat(input.format);
				
				if (flagFloat) {
					if (m_nChannels == 2) {
						audio.format = AudioFormat::Float_Stereo;
					} else {
						audio.format = AudioFormat::Float_Mono;
					}
				} else {
					if (m_nChannels == 2) {
						audio.format = AudioFormat::Int16_Stereo;
					} else {
						audio.format = AudioFormat::Int16_Mono;
					}
				}
				
				if (audio.format == input.format) {
					if (flagFloat) {
						if (((sl_size)(input.data) & 3) == 0) {
							audio.data = input.data;
						}
					} else {
						if (((sl_size)(input.data) & 1) == 0) {
							audio.data = input.data;
						}
					}
				}
				
				/*
					maximum sampling rate is 48000
					maximum frame length is 48000/400*24=2880 bytes
					maximum frame size is 2880*2*4=23040 bytes (5760 dwords)
				 */
				sl_uint32 _samples[5760];
				if (!(audio.data)) {
					audio.data = _samples;
					audio.copySamplesFrom(input);
				}
				
				ObjectLocker lock(this);
				if (m_flagResetBitrate) {
					sl_uint32 bitrate = getBitrate();
					::opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(bitrate));
					m_flagResetBitrate = sl_false;
				}
#ifdef OPUS_RESET_INTERVAL
				if (m_timeStartReset.getEllapsedMilliseconds() > OPUS_RESET_INTERVAL || ret <= 0) {
					Base::copyMemory(m_encoder, m_encoderBackup, m_sizeEncoder);
					m_timeStartReset.reset();
				}
#endif

				sl_uint8 output[4000]; // opus recommends 4000 bytes for output buffer
				int ret;
				if (flagFloat) {
					ret = ::opus_encode_float(m_encoder, (float*)(audio.data), (int)(audio.count), (unsigned char*)output, sizeof(output));
				} else {
					ret = ::opus_encode(m_encoder, (opus_int16*)(audio.data), (int)(audio.count), (unsigned char*)output, sizeof(output));
				}
				if (ret > 0) {
					return Memory::create(output, ret);
				}
			}
		}
		return Memory::null();
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
		m_flagResetBitrate = sl_true;
		AudioEncoder::setBitrate(bitrate);
	}
	
};

Ref<OpusEncoder> OpusEncoder::create(const OpusEncoderParam& param)
{
	return _OpusEncoderImpl::create(param);
}


OpusDecoderParam::OpusDecoderParam()
{
	samplesPerSecond = 16000;
	channelsCount = 1;
}

sl_bool OpusDecoder::isValidSamplingRate(sl_uint32 nSamplesPerSecond)
{
	return OpusEncoder::isValidSamplingRate(nSamplesPerSecond);
}

class _OpusDecoderImpl : public OpusDecoder
{
public:
	::OpusDecoder* m_decoder;

public:
	_OpusDecoderImpl()
	{
	}
	
	~_OpusDecoderImpl()
	{
		::opus_decoder_destroy(m_decoder);
	}

public:
	static void logError(String str)
	{
		SLIB_LOG_ERROR("AudioOpusDecoder", str);
	}

	static Ref<_OpusDecoderImpl> create(const OpusDecoderParam& param)
	{
		if (!isValidSamplingRate(param.samplesPerSecond)) {
			logError("Decoding sampling rate must be one of 8000, 12000, 16000, 24000, 48000");
			return Ref<_OpusDecoderImpl>::null();
		}
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			logError("Decoding channel must be 1 or 2");
			return Ref<_OpusDecoderImpl>::null();
		}
		int error;
		::OpusDecoder* decoder = ::opus_decoder_create((opus_int32)(param.samplesPerSecond), (opus_int32)(param.channelsCount), &error);
		if (! decoder) {
			return Ref<_OpusDecoderImpl>::null();
		}
		Ref<_OpusDecoderImpl> ret = new _OpusDecoderImpl();
		if (ret.isNotNull()) {
			ret->m_decoder = decoder;
			ret->m_nSamplesPerSecond = param.samplesPerSecond;
			ret->m_nChannels = param.channelsCount;
		} else {
			::opus_decoder_destroy(decoder);
		}
		return ret;
	}

	sl_uint32 decode(const void* input, sl_uint32 sizeInput, const AudioData& output)
	{
		AudioData audio;
		audio.count = output.count;
		
		sl_bool flagFloat = AudioFormats::isFloat(output.format);
		
		if (flagFloat) {
			if (m_nChannels == 2) {
				audio.format = AudioFormat::Float_Stereo;
			} else {
				audio.format = AudioFormat::Float_Mono;
			}
		} else {
			if (m_nChannels == 2) {
				audio.format = AudioFormat::Int16_Stereo;
			} else {
				audio.format = AudioFormat::Int16_Mono;
			}
		}
		
		if (audio.format == output.format) {
			if (flagFloat) {
				if (((sl_size)(output.data) & 3) == 0) {
					audio.data = output.data;
				}
			} else {
				if (((sl_size)(output.data) & 1) == 0) {
					audio.data = output.data;
				}
			}
		}
		
		SLIB_SCOPED_BUFFER(sl_uint8, 23040, _samples, audio.data ? 0 : audio.count);
		if (!(audio.data)) {
			audio.data = _samples;
		}
		int ret = 0;
		{
			ObjectLocker lock(this);
			if (flagFloat) {
				ret = ::opus_decode_float(m_decoder, (unsigned char*)input, (int)sizeInput, (float*)(audio.data), (int)(audio.count), 0);
			} else {
				ret = ::opus_decode(m_decoder, (unsigned char*)input, (int)sizeInput, (opus_int16*)(audio.data), (int)(audio.count), 0);
			}
		}
		if (ret > 0) {
			if (audio.data != output.data) {
				output.copySamplesFrom(audio, ret);
			}
			return ret;
		}
		return 0;
	}
};

Ref<OpusDecoder> OpusDecoder::create(const OpusDecoderParam& param)
{
	return _OpusDecoderImpl::create(param);
}

SLIB_MEDIA_NAMESPACE_END

