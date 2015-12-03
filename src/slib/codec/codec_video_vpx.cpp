#include "../../../inc/slib/codec/video_vpx.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/scoped_pointer.h"

#include "../../../inc/thirdparty/libvpx/vpx_config.h"
#include "../../../inc/thirdparty/libvpx/vpx/vp8cx.h"
#include "../../../inc/thirdparty/libvpx/vpx/vp8dx.h"
#include "../../../inc/thirdparty/libvpx/vpx/vpx_encoder.h"
#include "../../../inc/thirdparty/libvpx/vpx/vpx_decoder.h"

typedef vpx_codec_iface_t *(*vpx_codec_interface)(void);
SLIB_CODEC_NAMESPACE_BEGIN
class _VpxVideoEncoderImpl : public VideoVpxEncoder
{
public:
	_VpxVideoEncoderImpl()
	{
		m_codec = sl_null;
		m_codec_image = sl_null;
		m_codec_interface = sl_null;
		m_nProcessFrameCount = 0;
	}

public:
	sl_uint32 m_nProcessFrameCount;
	vpx_codec_enc_cfg_t m_codec_config;
	vpx_codec_ctx_t* m_codec;
	vpx_image_t* m_codec_image;
	vpx_codec_interface m_codec_interface;

	~_VpxVideoEncoderImpl()
	{
		if (m_codec_image != sl_null) {
			delete m_codec_image;
			m_codec_image = sl_null;
		}

		if (m_codec != sl_null) {
			vpx_codec_destroy(m_codec);
			delete m_codec;
			m_codec = sl_null;
		}
	}

	static void logError(String str)
	{
		SLIB_LOG_ERROR("VideoVpxEncoder", str);
	}

	static Ref<_VpxVideoEncoderImpl> create(const VideoVpxEncoderParam& param)
	{
		vpx_codec_enc_cfg_t codec_config;
		vpx_codec_interface codec_interface = sl_null;
		vpx_codec_ctx_t* codec = new vpx_codec_ctx_t;
		vpx_image_t* codec_image = new vpx_image_t;

		if (param.type == VideoVpxEncoderParam::VP8) {
#if CONFIG_VP8_ENCODER
			codec_interface = &vpx_codec_vp8_cx;
#endif
		} else if (param.type == VideoVpxEncoderParam::VP9) {
#if CONFIG_VP9_ENCODER
			codec_interface = &vpx_codec_vp9_cx;
#endif
		}
		
		if (codec_interface != sl_null) {
			if (vpx_img_alloc(codec_image, VPX_IMG_FMT_I420, param.nWidth, param.nHeight, 1)) {
				vpx_codec_err_t res = vpx_codec_enc_config_default(codec_interface(), &codec_config, 0);
				if (!res) {
					codec_config.g_w = param.nWidth;
					codec_config.g_h = param.nHeight;
					codec_config.rc_end_usage = (vpx_rc_mode)param.mode;
					codec_config.rc_target_bitrate = param.nBitrate;
					codec_config.g_threads = param.nThreadCount;
					codec_config.g_usage = param.nCpuUsage;
					codec_config.g_timebase.den = param.nFramePerSecond;
					codec_config.g_timebase.num = 1;

					if (!vpx_codec_enc_init(codec, codec_interface(), &codec_config, 0)) {
						Ref<_VpxVideoEncoderImpl> ret = new _VpxVideoEncoderImpl();
						if (ret.isNotNull()) {
							ret->m_codec = codec;
							ret->m_codec_config = codec_config;
							ret->m_codec_image = codec_image;
							ret->m_codec_interface = codec_interface;
							ret->m_nWidth = param.nWidth;
							ret->m_nHeight = param.nHeight;
							ret->m_nKeyFrameInterval = param.nKeyFrameInterval;
							ret->setBitrate(param.nBitrate);
							return ret;
						} else {
							vpx_codec_destroy(codec);
						}
					} else {
						logError("Failed to initialize video encoder codec.");
					}
				} else {
					logError("Failed to set default video encoder codec configuration.");
				}
			} else {
				logError("Failed to allocate video codec image");
			}
		} else {
			logError("Video codec is not supported");
		}

		delete codec;
		delete codec_image;
		return Ref<_VpxVideoEncoderImpl>::null();
	}

	void initializeCodecImage(const sl_uint8* i420, sl_int32 width, sl_int32 height)
	{
		sl_int32 yPlaneSize = width * height;
		Base::copyMemory(m_codec_image->planes[0], i420, (yPlaneSize));
		Base::copyMemory(m_codec_image->planes[1], i420 + yPlaneSize, (yPlaneSize >> 2));
		Base::copyMemory(m_codec_image->planes[2], i420 + yPlaneSize + (yPlaneSize >> 2), (yPlaneSize >> 2));
	}

	sl_bool encode(const BitmapDesc& input, void* output, sl_uint32& sizeOutput, sl_uint32& outTime)
	{
		if (m_nWidth == input.width && m_nHeight == input.height) {
			if (input.colorModel != Color::YUV_I420) {
				SLIB_SCOPED_ARRAY(sl_uint8, i420, input.width * input.height * 2);
				Color::convert(input.width, input.height, input.colorModel, input.data, input.pitch, Color::YUV_I420, i420, 0);
				initializeCodecImage(i420, input.width, input.height);

			} else {
				initializeCodecImage((sl_uint8*)(input.data), input.width, input.height);
			}

			sl_int32 flags = 0;
			if (m_nProcessFrameCount > 0 && m_nProcessFrameCount % m_nKeyFrameInterval == 0) {
				flags |= VPX_EFLAG_FORCE_KF;
			}
			vpx_codec_err_t res = vpx_codec_encode(m_codec, m_codec_image, m_nProcessFrameCount++, 1, flags, VPX_DL_REALTIME);
			outTime = 0;
			if (res == VPX_CODEC_OK) {
				vpx_codec_iter_t iter = sl_null;
				const vpx_codec_cx_pkt_t *pkt = sl_null;
				MemoryWriter encodeWriter;

				while ((pkt = vpx_codec_get_cx_data(m_codec, &iter)) != sl_null) {
					if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
						//const int keyframe = (pkt->data.frame.flags & VPX_FRAME_IS_KEY) != 0;
						encodeWriter.writeInt64(pkt->data.frame.pts);
						encodeWriter.writeInt64(pkt->data.frame.sz);
						encodeWriter.write(pkt->data.frame.buf, pkt->data.frame.sz);
					}
				}

				Memory encodedData = encodeWriter.getData();
				if (encodedData.getSize() < sizeOutput) {
					sizeOutput = (sl_uint32)encodedData.getSize();
					Base::copyMemory(output, encodedData.getBuf(), sizeOutput);
					return sl_true;
				} else {
					logError("Size of output buffer is small.");
					sizeOutput = 0;
				}
			} else {
				logError("Failed to encode bitmap data.");
			}
		} else {
			logError("BitmapDesc size is wrong.");
		}
		return sl_false;
	}

	void setBitrate(const sl_uint32& _bitrate)
	{
		sl_uint32 bitrate = _bitrate;
		if (bitrate < 50) {
			bitrate = 50;
		}
		if (bitrate > 1500) {
			bitrate = 1500;
		}
		//opus_encoder_ctl(m_encoder, OPUS_SET_BITRATE(bitrate));
		VideoEncoder::setBitrate(bitrate);
	}
};

class _VpxVideoDecoderImpl : public VideoVpxDecoder
{
private:
	_VpxVideoDecoderImpl()
	{
		m_codec = sl_null;
		m_codec_image = sl_null;
		m_codec_interface = sl_null;
	}

public:
	vpx_codec_ctx_t* m_codec;
	vpx_image_t* m_codec_image;
	vpx_codec_interface m_codec_interface;

	~_VpxVideoDecoderImpl()
	{

		if (m_codec_image != sl_null) {
			delete m_codec_image;
			m_codec_image = sl_null;
		}

		if (m_codec != sl_null) {
			vpx_codec_destroy(m_codec);
			delete m_codec;
			m_codec = sl_null;
		}
	}

	static void logError(String str)
	{
		SLIB_LOG_ERROR("VideoVpxDecoder", str);
	}

	static Ref<_VpxVideoDecoderImpl> create(const VideoVpxDecoderParam& param)
	{
		vpx_codec_interface codec_interface = sl_null;
		vpx_codec_ctx_t* codec = new vpx_codec_ctx_t;
		vpx_image_t* codec_image = new vpx_image_t;

		if (param.type == VideoVpxDecoderParam::VP8) {
#if CONFIG_VP8_ENCODER
			codec_interface = &vpx_codec_vp8_dx;
#endif
		} else if (param.type == VideoVpxDecoderParam::VP9) {
#if CONFIG_VP9_ENCODER
			codec_interface = &vpx_codec_vp9_dx;
#endif
		}

		if (codec_interface != sl_null) {
			if (!vpx_codec_dec_init(codec, codec_interface(), NULL, 0)) {
				Ref<_VpxVideoDecoderImpl> ret = new _VpxVideoDecoderImpl;
				if (ret.isNotNull()) {
					ret->m_nWidth = param.nWidth;
					ret->m_nHeight = param.nHeight;
					ret->m_codec = codec;
					ret->m_codec_image = codec_image;
					ret->m_codec_interface = codec_interface;
					return ret;
				} else {
					vpx_codec_destroy(codec);
					logError("Failed allocate _VpxVideoDecoderImpl.");
				}
			} else{
				logError("Failed initialize video decoder codec.");
			}
		} else {
			logError("Video codec is not supported");
		}

		delete codec;
		delete codec_image;
		return Ref<_VpxVideoDecoderImpl>::null();
	}

	SLIB_INLINE sl_int32 vpx_img_plane_width(const vpx_image_t *img, sl_int32 plane) {
		if (plane > 0 && img->x_chroma_shift > 0)
			return (img->d_w + 1) >> img->x_chroma_shift;
		else
			return img->d_w;
	}

	SLIB_INLINE sl_int32 vpx_img_plane_height(const vpx_image_t *img, sl_int32 plane) {
		if (plane > 0 && img->y_chroma_shift > 0)
			return (img->d_h + 1) >> img->y_chroma_shift;
		else
			return img->d_h;
	}

	sl_bool decode(const void* input, const sl_uint32& inputSize , BitmapDesc& output)
	{
		MemoryReader reader(input, inputSize);
		sl_int64 pts = reader.readInt64();
		SLIB_UNUSED(pts);
		sl_int64 size = reader.readInt64();

		if (!vpx_codec_decode(m_codec, (sl_uint8*)input + 16, (unsigned int)size, NULL, 0)) {
			vpx_codec_iter_t iter = NULL;
			while ((m_codec_image = vpx_codec_get_frame(m_codec, &iter)) != NULL) {
				sl_int32 yPlaneSize = m_nWidth * m_nHeight;
				SLIB_SCOPED_ARRAY(sl_uint8, i420, yPlaneSize * 2);

				sl_uint32 _indexI420 = 0;
				for (sl_uint32 plane = 0; plane < 3; ++plane) {
					const unsigned char *buf = m_codec_image->planes[plane];
					const int stride = m_codec_image->stride[plane];
					const int w = vpx_img_plane_width(m_codec_image, plane) * ((m_codec_image->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
					const int h = vpx_img_plane_height(m_codec_image, plane);
					int y;

					for (y = 0; y < h; ++y) {
						Base::copyMemory(i420 + _indexI420, buf, w);
						_indexI420 += w;
						buf += stride;
					}
				}
				Color::convert(m_nWidth, m_nHeight, Color::YUV_I420, i420, m_nWidth, output.colorModel, output.data, output.pitch);
			}
		}
		return sl_false;
	}
};

Ref<VideoVpxEncoder> VideoVpxEncoder::create(const VideoVpxEncoderParam& param)
{
	return Ref<VideoVpxEncoder>::from(_VpxVideoEncoderImpl::create(param));
}

Ref<VideoVpxDecoder> VideoVpxDecoder::create(const VideoVpxDecoderParam& param)
{
	return Ref<VideoVpxDecoder>::from(_VpxVideoDecoderImpl::create(param));
}
SLIB_CODEC_NAMESPACE_END

