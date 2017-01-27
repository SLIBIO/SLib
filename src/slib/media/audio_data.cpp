#include "../../../inc/slib/media/audio_data.h"

#include "../../../inc/slib/media/audio_util.h"
#include "../../../inc/slib/core/endian.h"

SLIB_MEDIA_NAMESPACE_BEGIN

AudioData::AudioData()
{
	count = 0;
	format = AudioFormat::None;
	
	data = sl_null;
	data1 = sl_null;
}

AudioData::AudioData(const AudioData& other) = default;

AudioData::~AudioData()
{
}

AudioData& AudioData::operator=(const AudioData& other) = default;

sl_size AudioData::getSizeForChannel() const
{
	if (format == AudioFormat::None) {
		return 0;
	}
	return ((AudioFormats::getBitsPerSample(format) * count) + 7) >> 3;
}

sl_size AudioData::getTotalSize() const
{
	if (format == AudioFormat::None) {
		return 0;
	}
	if (AudioFormats::isNonInterleaved(format)) {
		return getSizeForChannel() * AudioFormats::getChannelsCount(format);
	} else {
		return ((AudioFormats::getBitsPerSample(format) * AudioFormats::getChannelsCount(format) * count) + 7) >> 3;
	}
}

sl_uint32 AudioData::getChannelBuffers(AudioChannelBuffer* buffers) const
{
	sl_uint32 nChannels = AudioFormats::getChannelsCount(format);
	if (nChannels == 1) {
		if (buffers) {
			buffers[0].count = count;
			buffers[0].stride = AudioFormats::getBytesPerSample(format);
			buffers[0].data = data;
			buffers[0].ref = ref;
		}
		return 1;
	} else if (nChannels == 2) {
		if (buffers) {
			if (AudioFormats::isNonInterleaved(format)) {
				buffers[0].count = count;
				buffers[0].stride = AudioFormats::getBytesPerSample(format);
				buffers[0].data = data;
				buffers[0].ref = ref;
				if (data1) {
					buffers[1].count = count;
					buffers[1].stride = AudioFormats::getBytesPerSample(format);
					buffers[1].data = data1;
					buffers[1].ref = ref1;
				} else {
					buffers[1].count = count;
					buffers[1].stride = AudioFormats::getBytesPerSample(format);
					buffers[1].data = (sl_uint8*)data + getSizeForChannel();
					buffers[1].ref = ref;
				}
			} else {
				buffers[0].count = count;
				buffers[0].stride = AudioFormats::getBytesPerSample(format) * 2;
				buffers[0].data = data;
				buffers[0].ref = ref;
				
				buffers[1].count = count;
				buffers[1].stride = AudioFormats::getBytesPerSample(format) * 2;
				buffers[1].data = (sl_uint8*)data + AudioFormats::getBytesPerSample(format);
				buffers[1].ref = ref;
			}
		}
		return 2;
	}
	return 0;
}

class AUDIO_INT8_PROC
{
public:
	SLIB_INLINE static sl_uint8 readSample(sl_uint8*& p)
	{
		sl_uint8 v = *p;
		p++;
		return v;
	}
	
	SLIB_INLINE static void writeSample(sl_uint8*& p, sl_uint8 v)
	{
		*p = v;
		p++;
	}
};

class AUDIO_INT16LE_PROC
{
public:
	SLIB_INLINE static sl_uint16 readSample(sl_uint8*& p)
	{
		sl_uint16 v = p[0] | (((sl_uint16)(p[1])) << 8);
		p += 2;
		return v;
	}
	
	SLIB_INLINE static void writeSample(sl_uint8*& p, sl_uint16 v)
	{
		p[0] = (sl_uint8)(v);
		p[1] = (sl_uint8)(v >> 8);
		p += 2;
	}
};

class AUDIO_INT16BE_PROC
{
public:
	SLIB_INLINE static sl_uint16 readSample(sl_uint8*& p)
	{
		sl_uint16 v = p[1] | (((sl_uint16)(p[0])) << 8);
		p += 2;
		return v;
	}
	
	SLIB_INLINE static void writeSample(sl_uint8*& p, sl_uint16 v)
	{
		p[1] = (sl_uint8)(v);
		p[0] = (sl_uint8)(v >> 8);
		p += 2;
	}
};

class AUDIO_FLOAT_LE_PROC
{
public:
	SLIB_INLINE static float readSample(sl_uint8*& p)
	{
		sl_uint32 n = p[0] | (((sl_uint32)(p[1])) << 8) | (((sl_uint32)(p[2])) << 16) | (((sl_uint32)(p[3])) << 24);
		p += 4;
		return *((float*)((void*)(&n)));
	}
	
	SLIB_INLINE static void writeSample(sl_uint8*& p, float v)
	{
		sl_uint32& n = *((sl_uint32*)((void*)(&v)));
		p[0] = (sl_uint8)(n);
		p[1] = (sl_uint8)(n >> 8);
		p[2] = (sl_uint8)(n >> 16);
		p[3] = (sl_uint8)(n >> 24);
		p += 4;
	}
};

class AUDIO_FLOAT_BE_PROC
{
public:
	SLIB_INLINE static float readSample(sl_uint8*& p)
	{
		sl_uint32 n = p[3] | (((sl_uint32)(p[2])) << 8) | (((sl_uint32)(p[1])) << 16) | (((sl_uint32)(p[0])) << 24);
		p += 4;
		return *((float*)((void*)(&n)));
	}
	
	SLIB_INLINE static void writeSample(sl_uint8*& p, float v)
	{
		sl_uint32& n = *((sl_uint32*)((void*)(&v)));
		p[3] = (sl_uint8)(n);
		p[2] = (sl_uint8)(n >> 8);
		p[1] = (sl_uint8)(n >> 16);
		p[0] = (sl_uint8)(n >> 24);
		p += 4;
	}
};

template <class IN_PROC, class IN_TYPE, class OUT_PROC, class OUT_TYPE>
void _AudioData_copySamples_Step2(sl_size count, AudioFormat format_in, sl_uint8* data_in, sl_uint8* data_in1, AudioFormat format_out, sl_uint8* data_out, sl_uint8* data_out1)
{
	IN_TYPE _in;
	IN_TYPE _in1;
	OUT_TYPE _out;
	OUT_TYPE _out1;
	sl_uint32 nChannels_in = AudioFormats::getChannelsCount(format_in);
	sl_uint32 nChannels_out = AudioFormats::getChannelsCount(format_out);
	if (nChannels_in == 1) {
		if (nChannels_out == 1) {
			for (sl_size i = 0; i < count; i++) {
				_in = IN_PROC::readSample(data_in);
				AudioUtil::convertSample(_in, _out);
				OUT_PROC::writeSample(data_out, _out);
			}
		} else if (nChannels_out == 2) {
			if (AudioFormats::isNonInterleaved(format_out)) {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					AudioUtil::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
					OUT_PROC::writeSample(data_out1, _out);
				}
			} else {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					AudioUtil::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			}
		}
	} else if (nChannels_in == 2) {
		if (AudioFormats::isNonInterleaved(format_in)) {
			if (nChannels_out == 1) {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					_in1 = IN_PROC::readSample(data_in1);
					AudioUtil::mixSamples(_in, _in1, _in);
					AudioUtil::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			} else if (nChannels_out == 2) {
				if (AudioFormats::isNonInterleaved(format_out)) {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in1);
						AudioUtil::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out1, _out1);
					}
				} else {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in1);
						AudioUtil::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out, _out1);
					}
				}
			}
		} else {
			if (nChannels_out == 1) {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					_in1 = IN_PROC::readSample(data_in);
					AudioUtil::mixSamples(_in, _in1, _in);
					AudioUtil::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			} else if (nChannels_out == 2) {
				if (AudioFormats::isNonInterleaved(format_out)) {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out1, _out1);
					}
				} else {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in);
						AudioUtil::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out, _out1);
					}
				}
			}
		}
	}
}


template<class IN_PROC, class IN_TYPE>
void _AudioData_copySamples_Step1(sl_size count, AudioFormat format_in, sl_uint8* data_in, sl_uint8* data_in1, AudioFormat format_out, sl_uint8* data_out, sl_uint8* data_out1)
{
	switch (AudioFormats::getSampleType(format_out)) {
		case AudioSampleType::Int8:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT8_PROC, sl_int8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint8:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT8_PROC, sl_uint8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Int16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::Int16LE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Int16BE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::Uint16LE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint16BE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Float:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::FloatLE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::FloatBE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
	}
}


void _AudioData_copySamples(sl_size count, AudioFormat format_in, sl_uint8* data_in, sl_uint8* data_in1, AudioFormat format_out, sl_uint8* data_out, sl_uint8* data_out1)
{
	switch (AudioFormats::getSampleType(format_in)) {
		case AudioSampleType::Int8:
			_AudioData_copySamples_Step1<AUDIO_INT8_PROC, sl_int8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint8:
			_AudioData_copySamples_Step1<AUDIO_INT8_PROC, sl_uint8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Int16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::Int16LE:
			_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Int16BE:
			_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::Uint16LE:
			_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Uint16BE:
			_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::Float:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case AudioSampleType::FloatLE:
			_AudioData_copySamples_Step1<AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case AudioSampleType::FloatBE:
			_AudioData_copySamples_Step1<AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
	}
}

void AudioData::copySamplesFrom(const AudioData& other, sl_size countSamples) const
{
	if (format == AudioFormat::None) {
		return;
	}
	if (other.format == AudioFormat::None) {
		return;
	}
	if (countSamples > count) {
		countSamples = count;
	}
	if (countSamples > other.count) {
		countSamples = other.count;
	}
	if (countSamples == 0) {
		return;
	}
	
	sl_uint8* data_in = (sl_uint8*)data;
	sl_uint8* data_in1 = (sl_uint8*)data1;
	if (AudioFormats::isNonInterleaved(format) && !data_in1) {
		data_in1 = data_in + getSizeForChannel();
	}
	
	sl_uint8* data_out = (sl_uint8*)(other.data);
	sl_uint8* data_out1 = (sl_uint8*)(other.data1);
	if (AudioFormats::isNonInterleaved(other.format) && !data_out1) {
		data_out1 = data_out + other.getSizeForChannel();
	}
	
	if (format == other.format) {
		if (AudioFormats::isNonInterleaved(format)) {
			sl_size n = (countSamples * AudioFormats::getBitsPerSample(format) + 7) >> 3;
			Base::copyMemory(data_out, data_in, n);
			Base::copyMemory(data_out1, data_in1, n);
		} else {
			sl_size n = (countSamples * AudioFormats::getBitsPerSample(format) * AudioFormats::getChannelsCount(format) + 7) >> 3;
			Base::copyMemory(data_out, data_in, n);
		}
		return;
	}
	
	_AudioData_copySamples(countSamples, format, data_in, data_in1, other.format, data_out, data_out1);
}

void AudioData::copySamplesFrom(const AudioData& other) const
{
	copySamplesFrom(other, count);
}

SLIB_MEDIA_NAMESPACE_END

