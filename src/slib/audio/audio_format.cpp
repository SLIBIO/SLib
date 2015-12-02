#include "../../../inc/slib/audio/format.h"

SLIB_AUDIO_NAMESPACE_BEGIN

#define _DEFINE_CONVERT_SAMPLES(TYPE_IN, TYPE_OUT) \
void AudioFormat::convertSamples(sl_size count, const TYPE_IN* in, TYPE_OUT* out) \
{ \
	for (sl_size i = 0; i < count; i++) { \
		AudioFormat::convertSample(in[i], out[i]); \
	} \
}

_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_uint8, float)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_int16, float)
_DEFINE_CONVERT_SAMPLES(float, sl_uint8)
_DEFINE_CONVERT_SAMPLES(float, sl_int16)

SLIB_AUDIO_NAMESPACE_END

