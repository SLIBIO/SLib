#include "../../../inc/slib/media/audio_data.h"
#include "../../../inc/slib/core/endian.h"

SLIB_MEDIA_NAMESPACE_BEGIN

AudioData::AudioData()
{
	count = 0;
	format = AudioFormat::null();
	
	data = sl_null;
	data1 = sl_null;
}

AudioData::AudioData(const AudioData& other)
{
	count = other.count;
	format = other.format;
	
	data = other.data;
	ref = other.ref;
	
	data1 = other.data1;
	ref1 = other.ref1;
}

AudioData& AudioData::operator=(const AudioData& other)
{
	count = other.count;
	format = other.format;
	
	data = other.data;
	ref = other.ref;
	
	data1 = other.data1;
	ref1 = other.ref1;

	return *this;
}

sl_size AudioData::getSizeForChannel() const
{
	if (format.isNull()) {
		return 0;
	}
	return ((format.getBitsPerSample() * count) + 7) >> 3;
}

sl_size AudioData::getTotalSize() const
{
	if (format.isNull()) {
		return 0;
	}
	if (format.isNonInterleaved()) {
		return getSizeForChannel() * format.getChannelsCount();
	} else {
		return ((format.getBitsPerSample() * format.getChannelsCount() * count) + 7) >> 3;
	}
}

sl_uint32 AudioData::getChannelBuffers(AudioChannelBuffer* buffers) const
{
	sl_uint32 nChannels = format.getChannelsCount();
	if (nChannels == 1) {
		if (buffers) {
			buffers[0].count = count;
			buffers[0].stride = format.getBytesPerSample();
			buffers[0].data = data;
			buffers[0].ref = ref;
		}
		return 1;
	} else if (nChannels == 2) {
		if (buffers) {
			if (format.isNonInterleaved()) {
				buffers[0].count = count;
				buffers[0].stride = format.getBytesPerSample();
				buffers[0].data = data;
				buffers[0].ref = ref;
				if (data1) {
					buffers[1].count = count;
					buffers[1].stride = format.getBytesPerSample();
					buffers[1].data = data1;
					buffers[1].ref = ref1;
				} else {
					buffers[1].count = count;
					buffers[1].stride = format.getBytesPerSample();
					buffers[1].data = (sl_uint8*)data + getSizeForChannel();
					buffers[1].ref = ref;
				}
			} else {
				buffers[0].count = count;
				buffers[0].stride = format.getBytesPerSample() * 2;
				buffers[0].data = data;
				buffers[0].ref = ref;
				
				buffers[1].count = count;
				buffers[1].stride = format.getBytesPerSample() * 2;
				buffers[1].data = (sl_uint8*)data + format.getBytesPerSample();
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
	sl_uint32 nChannels_in = format_in.getChannelsCount();
	sl_uint32 nChannels_out = format_out.getChannelsCount();
	if (nChannels_in == 1) {
		if (nChannels_out == 1) {
			for (sl_size i = 0; i < count; i++) {
				_in = IN_PROC::readSample(data_in);
				AudioFormat::convertSample(_in, _out);
				OUT_PROC::writeSample(data_out, _out);
			}
		} else if (nChannels_out == 2) {
			if (format_out.isNonInterleaved()) {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					AudioFormat::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
					OUT_PROC::writeSample(data_out1, _out);
				}
			} else {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					AudioFormat::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			}
		}
	} else if (nChannels_in == 2) {
		if (format_in.isNonInterleaved()) {
			if (nChannels_out == 1) {
				for (sl_size i = 0; i < count; i++) {
					_in = IN_PROC::readSample(data_in);
					_in1 = IN_PROC::readSample(data_in1);
					AudioFormat::mixSamples(_in, _in1, _in);
					AudioFormat::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			} else if (nChannels_out == 2) {
				if (format_out.isNonInterleaved()) {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in1);
						AudioFormat::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out1, _out1);
					}
				} else {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in1);
						AudioFormat::convertSample(_in1, _out1);
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
					AudioFormat::mixSamples(_in, _in1, _in);
					AudioFormat::convertSample(_in, _out);
					OUT_PROC::writeSample(data_out, _out);
				}
			} else if (nChannels_out == 2) {
				if (format_out.isNonInterleaved()) {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in1, _out1);
						OUT_PROC::writeSample(data_out, _out);
						OUT_PROC::writeSample(data_out1, _out1);
					}
				} else {
					for (sl_size i = 0; i < count; i++) {
						_in = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in, _out);
						_in1 = IN_PROC::readSample(data_in);
						AudioFormat::convertSample(_in1, _out1);
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
	switch (format_out.getSampleType()) {
		case audioSampleType_Int8:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT8_PROC, sl_int8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint8:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT8_PROC, sl_uint8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Int16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_Int16LE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Int16BE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_Uint16LE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint16BE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Float:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_FloatLE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_FloatBE:
			_AudioData_copySamples_Step2<IN_PROC, IN_TYPE, AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
	}
}


void _AudioData_copySamples(sl_size count, AudioFormat format_in, sl_uint8* data_in, sl_uint8* data_in1, AudioFormat format_out, sl_uint8* data_out, sl_uint8* data_out1)
{
	switch (format_in.getSampleType()) {
		case audioSampleType_Int8:
			_AudioData_copySamples_Step1<AUDIO_INT8_PROC, sl_int8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint8:
			_AudioData_copySamples_Step1<AUDIO_INT8_PROC, sl_uint8>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Int16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_Int16LE:
			_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Int16BE:
			_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_int16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint16:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_Uint16LE:
			_AudioData_copySamples_Step1<AUDIO_INT16LE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Uint16BE:
			_AudioData_copySamples_Step1<AUDIO_INT16BE_PROC, sl_uint16>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_Float:
			if (Endian::isLE()) {
				_AudioData_copySamples_Step1<AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			} else {
				_AudioData_copySamples_Step1<AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			}
			break;
		case audioSampleType_FloatLE:
			_AudioData_copySamples_Step1<AUDIO_FLOAT_LE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
		case audioSampleType_FloatBE:
			_AudioData_copySamples_Step1<AUDIO_FLOAT_BE_PROC, float>(count, format_in, data_in, data_in1, format_out, data_out, data_out1);
			break;
	}
}

void AudioData::copySamplesFrom(const AudioData& other, sl_size countSamples) const
{
	if (format.isNull()) {
		return;
	}
	if (other.format.isNull()) {
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
	if (format.isNonInterleaved() && !data_in1) {
		data_in1 = data_in + getSizeForChannel();
	}
	
	sl_uint8* data_out = (sl_uint8*)(other.data);
	sl_uint8* data_out1 = (sl_uint8*)(other.data1);
	if (other.format.isNonInterleaved() && !data_out1) {
		data_out1 = data_out + other.getSizeForChannel();
	}
	
	if (format == other.format) {
		if (format.isNonInterleaved()) {
			sl_size n = (countSamples * format.getBitsPerSample() + 7) >> 3;
			Base::copyMemory(data_out, data_in, n);
			Base::copyMemory(data_out1, data_in1, n);
		} else {
			sl_size n = (countSamples * format.getBitsPerSample() * format.getChannelsCount() + 7) >> 3;
			Base::copyMemory(data_out, data_in, n);
		}
		return;
	}
	
	_AudioData_copySamples(countSamples, format, data_in, data_in1, other.format, data_out, data_out1);
}

SLIB_MEDIA_NAMESPACE_END

