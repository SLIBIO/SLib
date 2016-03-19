#include "../../../inc/slib/media/audio_util.h"

SLIB_MEDIA_NAMESPACE_BEGIN

#define _DEFINE_CONVERT_SAMPLES(TYPE_IN, TYPE_OUT) \
void AudioUtil::convertSamples(sl_size count, const TYPE_IN* in, TYPE_OUT* out) \
{ \
	for (sl_size i = 0; i < count; i++) { \
		convertSample(in[i], out[i]); \
	} \
}

_DEFINE_CONVERT_SAMPLES(sl_int8, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_int8, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_int8, float)

_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_uint8, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_uint8, float)

_DEFINE_CONVERT_SAMPLES(sl_int16, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_int16, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_int16, float)

_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_int8)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_uint8)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_int16)
_DEFINE_CONVERT_SAMPLES(sl_uint16, sl_uint16)
_DEFINE_CONVERT_SAMPLES(sl_uint16, float)

_DEFINE_CONVERT_SAMPLES(float, sl_int8)
_DEFINE_CONVERT_SAMPLES(float, sl_uint8)
_DEFINE_CONVERT_SAMPLES(float, sl_int16)
_DEFINE_CONVERT_SAMPLES(float, sl_uint16)
_DEFINE_CONVERT_SAMPLES(float, float)

SLIB_MEDIA_NAMESPACE_END

