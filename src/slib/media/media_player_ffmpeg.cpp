/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/media/ffmpeg.h"

#include "slib/media/audio_player.h"
#include "slib/core/thread.h"
#include "slib/core/asset.h"
#include "slib/core/log.h"

extern "C" {
	#include "ffmpeg/ffmpeg.h"
}

#define TAG "FFmpeg"

#define VIDEO_BACK_FRAMES_COUNT 10

namespace slib
{
	
	class _priv_FFmpegMediaPlayer : public MediaPlayer
	{
	public:
		String m_url;
		sl_bool m_flagVideo;
		
		AVFormatContext* m_format;
		AVPacket* m_packet;
		AVPacket* m_packetToSend;
		int m_streamAudio;
		int m_streamVideo;
		AVCodecContext* m_codecAudio;
		AVCodecContext* m_codecVideo;
		AVFrame* m_frameAudio;
		Queue<AVFrame*> m_queueVideoFramesRender;
		Queue<AVFrame*> m_queueVideoFramesBack;
		SwrContext* m_swrAudio;
		TimeCounter m_timeCounterVideo;

		Ref<AudioPlayerBuffer> m_audioBuffer;
		Memory m_bufResample;

		Ref<Thread> m_threadInit;
		Ref<Thread> m_threadPlay;

		sl_bool m_flagReleased;
		sl_bool m_flagOpened;
		sl_bool m_flagPlaying;
		sl_bool m_flagPlayOnOpen;
		sl_real m_volume;
		double m_timeCurrent;
		double m_timeSeek;
		double m_duration;
		
	public:
		_priv_FFmpegMediaPlayer()
		{
			m_format = sl_null;
			m_packet = sl_null;
			m_packetToSend = sl_null;
			m_streamAudio = -1;
			m_streamVideo = -1;
			m_codecAudio = sl_null;
			m_codecVideo = sl_null;
			m_frameAudio = sl_null;
			m_swrAudio = sl_null;
			
			m_flagReleased = sl_false;
			m_flagOpened = sl_false;
			m_flagPlaying = sl_false;
			m_flagPlayOnOpen = sl_false;
			m_volume = 1;
			m_timeCurrent = 0;
			m_timeSeek = -1;
			m_duration = 0;
		}
		
		~_priv_FFmpegMediaPlayer()
		{
			_release();
		}
		
	public:
		static Ref<_priv_FFmpegMediaPlayer> create(const MediaPlayerParam& param)
		{
			String url;
			if (param.url.isNotEmpty()) {
				url = param.url;
			} else if (param.filePath.isNotEmpty()) {
				url = "file:" + param.filePath;
			} else if (param.assetFileName.isNotEmpty()) {
				url = Assets::getFilePath(param.assetFileName);
			}
			Ref<_priv_FFmpegMediaPlayer> ret = new _priv_FFmpegMediaPlayer;
			if (ret.isNotNull()) {
				ret->m_url = url;
				ret->m_flagVideo = param.flagVideo;
				ret->_init(param);
				ret->m_threadInit = Thread::start(Function<void()>::bind(&onRunInit, ToWeakRef(ret)));
				return ret;
			}
			return sl_null;
		}
		
	public:
		void release() override
		{
			ObjectLocker lock(this);
			_release();
		}
		
		void resume() override
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				m_flagPlayOnOpen = sl_true;
				return;
			}
			if (m_flagPlaying) {
				return;
			}
			m_flagPlaying = sl_true;
			av_read_play(m_format);
			if (m_audioBuffer.isNotNull()) {
				m_audioBuffer->start();
			}
			m_timeCounterVideo.reset();
			m_threadPlay = Thread::start(Function<void()>::bind(&onRunPlay, ToWeakRef(this)));
			_addToMap();
		}
		
		void pause() override
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			if (!m_flagPlaying) {
				return;
			}
			m_flagPlaying = sl_false;
			av_read_pause(m_format);
			if (m_audioBuffer.isNotNull()) {
				m_audioBuffer->stop();
			}
			if (m_threadPlay.isNotNull()) {
				m_threadPlay->finishAndWait();
				m_threadPlay.setNull();
			}
			_removeFromMap();
		}
		
		sl_bool isPlaying() override
		{
			return m_flagPlaying;
		}
		
		sl_real getVolume() override
		{
			return m_volume;
		}
		
		void setVolume(sl_real volume) override
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			m_volume = volume;
		}
		
		double getDuration() override
		{
			return m_duration;
		}
		
		double getCurrentTime() override
		{
			return m_timeCurrent;
		}
		
		void seekTo(double time) override
		{
			if (time < 0) {
				time = 0;
			}
			m_timeSeek = time;
		}
		
		void renderVideo(MediaPlayerRenderVideoParam& param) override
		{
			param.flagUpdated = sl_false;
			if (param.onUpdateFrame.isNull()) {
				return;
			}
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			if (!m_codecVideo) {
				return;
			}
			AVFrame* frame;
			if (m_queueVideoFramesRender.pop(&frame)) {
				VideoFrame vf;
				_getVideoFrame(frame, vf);
				param.onUpdateFrame(vf);
				m_queueVideoFramesBack.push(frame);
			}
		}

	private:
		static void onRunInit(WeakRef<_priv_FFmpegMediaPlayer> weak)
		{
			auto player = ToRef(weak);
			if (player.isNull()) {
				return;
			}
			player->_initialize();
		}
		
		void _initialize()
		{
			if (m_flagReleased) {
				return;
			}
			
			m_packet = av_packet_alloc();
			if (!m_packet) {
				return;
			}

			if (0 < avformat_open_input(&m_format, m_url.getData(), NULL, NULL)) {
				_logError(String::format("Failed to open url: %s", m_url));
				return;
			}
			if (!m_format) {
				_logError(String::format("Failed to open url: %s", m_url));
				return;
			}
			
			if (avformat_find_stream_info(m_format, NULL) < 0) {
				_logError(String::format("Failed to open find stream info: %s", m_url));
				return;
			}
			AVCodec* codecAudio = NULL;
			AVCodec* codecVideo = NULL;
			m_streamAudio = av_find_best_stream(m_format, AVMEDIA_TYPE_AUDIO, -1, -1, &codecAudio, 0);
			m_streamVideo = av_find_best_stream(m_format, AVMEDIA_TYPE_VIDEO, -1, -1, &codecVideo, 0);
			m_codecAudio = sl_null;
			if (m_streamAudio >= 0 && codecAudio) {
				m_codecAudio = avcodec_alloc_context3(NULL);
				if (m_codecAudio) {
					avcodec_parameters_to_context(m_codecAudio, m_format->streams[m_streamAudio]->codecpar);
					if (avcodec_open2(m_codecAudio, codecAudio, NULL) < 0) {
						_logError(String::format("Cannot open audio codec: %s", codecAudio->name));
						avcodec_free_context(&m_codecAudio);
					} else {
						m_frameAudio = av_frame_alloc();
						if (!m_frameAudio) {
							avcodec_free_context(&m_codecAudio);
						}
					}
				}
			}
			m_codecVideo = sl_null;
			if (m_streamVideo >= 0 && codecVideo) {
				m_codecVideo = avcodec_alloc_context3(NULL);
				if (m_codecVideo) {
					avcodec_parameters_to_context(m_codecVideo, m_format->streams[m_streamVideo]->codecpar);
					if (avcodec_open2(m_codecVideo, codecVideo, NULL) < 0) {
						_logError(String::format("Cannot open video codec: %s", codecVideo->name));
						avcodec_free_context(&m_codecVideo);
					}
				}
			}
			if (m_codecAudio) {
				AVCodecParameters* params = m_format->streams[m_streamAudio]->codecpar;
				Ref<AudioPlayer> audioPlayer = AudioPlayer::create();
				if (audioPlayer.isNotNull()) {
					AudioPlayerBufferParam audioBufferParam;
					audioBufferParam.flagAutoStart = sl_false;
					audioBufferParam.bufferLengthInMilliseconds = 5000;
					audioBufferParam.frameLengthInMilliseconds = 50;
					audioBufferParam.samplesPerSecond = params->sample_rate;
					audioBufferParam.channelsCount = params->channels;
					if (audioBufferParam.channelsCount != 1) {
						audioBufferParam.channelsCount = 2;
					}
					audioBufferParam.onRequireAudioData = SLIB_FUNCTION_WEAKREF(_priv_FFmpegMediaPlayer, onRequireAudioData, this);
					m_audioBuffer = audioPlayer->createBuffer(audioBufferParam);
					if (m_audioBuffer.isNotNull()) {
						SwrContext* swrAudio = swr_alloc();
						if (swrAudio) {
							av_opt_set_int(swrAudio, "in_channel_count",  params->channels, 0);
							av_opt_set_int(swrAudio, "out_channel_count", audioBufferParam.channelsCount, 0);
							av_opt_set_channel_layout(swrAudio, "in_channel_layout",  params->channel_layout, 0);
							av_opt_set_channel_layout(swrAudio, "out_channel_layout", audioBufferParam.channelsCount == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO, 0);
							av_opt_set_int(swrAudio, "in_sample_rate", params->sample_rate, 0);
							av_opt_set_int(swrAudio, "out_sample_rate", audioBufferParam.samplesPerSecond, 0);
							av_opt_set_sample_fmt(swrAudio, "in_sample_fmt",  (AVSampleFormat)(params->format), 0);
							av_opt_set_sample_fmt(swrAudio, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
							swr_init(swrAudio);
							m_swrAudio = swrAudio;
						} else {
							m_audioBuffer.setNull();
						}
					}
				}
			}
			if (m_codecVideo) {
				for (int i = 0; i < VIDEO_BACK_FRAMES_COUNT; i++) {
					AVFrame* frame = av_frame_alloc();
					if (frame) {
						m_queueVideoFramesBack.push_NoLock(frame);
					}
				}
			}
			
			m_duration = (double)(m_format->duration) / (double)(AV_TIME_BASE);
			{
				ObjectLocker lock(this);
				m_flagOpened = sl_true;
				if (m_flagPlayOnOpen) {
					resume();
				}
			}
			_onReadyToPlay();
		}
		
		void _release()
		{
			m_flagReleased = sl_true;
			m_flagOpened = sl_false;
			m_flagPlaying = sl_false;
			if (m_threadInit.isNotNull()) {
				m_threadInit->finishAndWait();
				m_threadInit.setNull();
			}
			if (m_threadPlay.isNotNull()) {
				m_threadPlay->finishAndWait();
				m_threadPlay.setNull();
			}
			if (m_audioBuffer.isNotNull()) {
				m_audioBuffer->release();
				m_audioBuffer.setNull();
			}
			if (m_codecAudio) {
				avcodec_free_context(&m_codecAudio);
			}
			if (m_codecVideo) {
				avcodec_free_context(&m_codecVideo);
			}
			if (m_frameAudio) {
				av_frame_free(&m_frameAudio);
			}
			{
				AVFrame* frame;
				while (m_queueVideoFramesBack.pop(&frame)) {
					av_frame_free(&frame);
				}
			}
			{
				AVFrame* frame;
				while (m_queueVideoFramesRender.pop(&frame)) {
					av_frame_free(&frame);
				}
			}
			if (m_swrAudio) {
				swr_free(&m_swrAudio);
			}
			if (m_format) {
				avformat_close_input(&m_format);
			}
			m_packetToSend = sl_null;
			if (m_packet) {
				av_packet_free(&m_packet);
			}
		}
		
		void onRequireAudioData(AudioPlayerBuffer* buffer, sl_uint32 requestedSamplesCount)
		{
		}
		
		static void onRunPlay(WeakRef<_priv_FFmpegMediaPlayer> weak)
		{
			Ref<Thread> thread = Thread::getCurrent();
			while (thread.isNull() || thread->isNotStopping()) {
				auto player = ToRef(weak);
				if (player.isNull()) {
					return;
				}
				player->_playSeek();
				if (!(player->_playStep())) {
					return;
				}
				if (player->m_timeCurrent >= player->m_duration - 0.1) {
					player->_onComplete();
					if (player->m_flagAutoRepeat) {
						player->seekTo(0);
					} else {
						player->_release();
						return;
					}
				}
			}
		}
		
		void _playSeek()
		{
			double time = m_timeSeek;
			if (time < 0) {
				return;
			}
			m_timeSeek = -1;
			if (time > m_duration) {
				time = m_duration;
			}
			m_timeCurrent = time;
			int64_t t = (int64_t)(time * AV_TIME_BASE);
			int64_t tmin = t - AV_TIME_BASE;
			if (tmin < 0) {
				tmin = 0;
			}
			int64_t tmax = t + AV_TIME_BASE;
			if (tmax > m_format->duration) {
				tmax = m_format->duration - 1;
			}
			avformat_seek_file(m_format, -1, tmin, t, tmax, 0);
			if (m_audioBuffer.isNotNull()) {
				m_audioBuffer->flush();
			}
			{
				AVFrame* frame;
				while (m_queueVideoFramesRender.pop(&frame)) {
					m_queueVideoFramesBack.push(frame);
				}
			}
			m_packetToSend = sl_null;
		}
		
		sl_bool _playStep()
		{
			int ret;
			if (!m_flagOpened) {
				return sl_false;
			}
			if (!m_flagPlaying) {
				return sl_false;
			}
			if (m_audioBuffer.isNotNull()) {
				if (m_audioBuffer->getSamplesCountInQueue() > m_audioBuffer->getParam().samplesPerSecond * 3) {
					Thread::sleep(10);
					return sl_true;
				}
				if (m_codecAudio && m_swrAudio) {
					ret = avcodec_receive_frame(m_codecAudio, m_frameAudio);
					if (ret < 0) {
						if (ret == AVERROR_EOF) {
							Thread::sleep(10);
							return sl_true;
						} else if (ret != AVERROR(EAGAIN)) {
							return sl_false;
						}
					} else {
						m_timeCurrent = _getAudioFrameTimestamp(m_frameAudio);
						sl_size sizeBuf = m_frameAudio->nb_samples * m_audioBuffer->getParam().channelsCount * 2;
						if (m_bufResample.getSize() < sizeBuf) {
							m_bufResample = Memory::create(sizeBuf);
						}
						sl_uint8* bufs[2];
						bufs[0] = (sl_uint8*)(m_bufResample.getData());
						bufs[1] = bufs[0] + 2;
						int n = swr_convert(m_swrAudio, (uint8_t**)(bufs), m_frameAudio->nb_samples, (const uint8_t**)(m_frameAudio->data), m_frameAudio->nb_samples);
						if (n > 0) {
							AudioData data;
							if (m_audioBuffer->getParam().channelsCount == 1) {
								data.format = AudioFormat::Int16_Mono;
							} else {
								data.format = AudioFormat::Int16_Stereo;
							}
							data.data = bufs[0];
							data.count = n;
							m_audioBuffer->write(data);
						}
						return sl_true;
					}
				}
			}
			if (m_codecVideo) {
				AVFrame* frame;
				if (!(m_queueVideoFramesBack.pop(&frame))) {
					if (!(m_queueVideoFramesRender.pop(&frame))) {
						Thread::sleep(10);
						return sl_true;
					}
				}
				ret = avcodec_receive_frame(m_codecVideo, frame);
				if (ret < 0) {
					m_queueVideoFramesBack.push(frame);
					if (ret == AVERROR_EOF) {
						Thread::sleep(10);
						return sl_true;
					} else if (ret != AVERROR(EAGAIN)) {
						return sl_false;
					}
				} else {
					m_queueVideoFramesRender.push(frame);
					if (m_audioBuffer.isNull()) {
						m_timeCurrent = _getVideoFrameTimestamp(frame);
						double duration = _getVideoFrameDuration(frame);
						if (duration < 0.02) {
							duration = 0.02;
						}
						double dt = m_timeCounterVideo.getTime().getSecondsCountf();
						m_timeCounterVideo.reset();
						if (dt < duration) {
							Thread::sleep((sl_uint32)((duration - dt) * 1000));
						}
					}
					return sl_true;
				}
			}
			if (!m_packetToSend) {
				ret = av_read_frame(m_format, m_packet);
				if (ret == AVERROR_EOF) {
					Thread::sleep(10);
					return sl_true;
				} else if (ret == AVERROR(EAGAIN)) {
					Thread::sleep(10);
					return sl_true;
				} else if (ret < 0) {
					return sl_false;
				}
				m_packetToSend = m_packet;
			}
			if (m_packetToSend->stream_index == m_streamAudio) {
				if (m_codecAudio) {
					ret = avcodec_send_packet(m_codecAudio, m_packetToSend);
					if (ret < 0) {
						if (ret == AVERROR_EOF) {
							m_packetToSend = sl_null;
							Thread::sleep(10);
							return sl_true;
						} else if (ret != AVERROR(EAGAIN)) {
							return sl_false;
						}
					} else {
						m_packetToSend = sl_null;
					}
				}
			} else if (m_packetToSend->stream_index == m_streamVideo) {
				if (m_codecVideo) {
					ret = avcodec_send_packet(m_codecVideo, m_packetToSend);
					if (ret < 0) {
						if (ret == AVERROR_EOF) {
							m_packetToSend = sl_null;
							Thread::sleep(10);
							return sl_true;
						} else if (ret != AVERROR(EAGAIN)) {
							return sl_false;
						}
					} else {
						m_packetToSend = sl_null;
					}
				} else {
					m_packetToSend = sl_null;
				}
			} else {
				m_packetToSend = sl_null;
			}
			return sl_true;
		}
		
		static void _logError(const String& str)
		{
			LogError(TAG, str);
		}
		
		static void _getVideoFrame(AVFrame* af, VideoFrame& vf)
		{
			switch (af->format) {
				case AV_PIX_FMT_RGBA:
					vf.image.format = BitmapFormat::RGBA;
					break;
				case AV_PIX_FMT_BGRA:
					vf.image.format = BitmapFormat::BGRA;
					break;
				case AV_PIX_FMT_ARGB:
					vf.image.format = BitmapFormat::ARGB;
					break;
				case AV_PIX_FMT_ABGR:
					vf.image.format = BitmapFormat::ABGR;
					break;
				case AV_PIX_FMT_RGB24:
					vf.image.format = BitmapFormat::RGB;
					break;
				case AV_PIX_FMT_BGR24:
					vf.image.format = BitmapFormat::BGR;
					break;
				case AV_PIX_FMT_RGB565BE:
					vf.image.format = BitmapFormat::RGB565BE;
					break;
				case AV_PIX_FMT_RGB565LE:
					vf.image.format = BitmapFormat::RGB565LE;
					break;
				case AV_PIX_FMT_BGR565BE:
					vf.image.format = BitmapFormat::BGR565BE;
					break;
				case AV_PIX_FMT_BGR565LE:
					vf.image.format = BitmapFormat::BGR565LE;
					break;
				case AV_PIX_FMT_GRAY8:
					vf.image.format = BitmapFormat::GRAY8;
					break;
				case AV_PIX_FMT_YUVA444P:
					vf.image.format = BitmapFormat::YUVA_PLANAR;
					break;
				case AV_PIX_FMT_YUV444P:
					vf.image.format = BitmapFormat::YUV444_PLANAR;
					break;
				case AV_PIX_FMT_YUV420P:
					vf.image.format = BitmapFormat::YUV_I420;
					break;
				case AV_PIX_FMT_NV21:
					vf.image.format = BitmapFormat::YUV_NV21;
					break;
				case AV_PIX_FMT_NV12:
					vf.image.format = BitmapFormat::YUV_NV12;
					break;
			}
			if (vf.image.format != BitmapFormat::None) {
				vf.image.width = af->width;
				vf.image.height = af->height;
				vf.image.data = af->data[0];
				vf.image.pitch = af->linesize[0];
				vf.image.data1 = af->data[1];
				vf.image.pitch1 = af->linesize[1];
				vf.image.data2 = af->data[2];
				vf.image.pitch2 = af->linesize[2];
				vf.image.data3 = af->data[3];
				vf.image.pitch3 = af->linesize[3];
			}
		}
		
		double _getAudioFrameTimestamp(AVFrame* frame)
		{
			double startTime = (double)(m_format->start_time) / (double)AV_TIME_BASE;
			AVRational& timeBase = m_format->streams[m_streamAudio]->time_base;
			return (double)(frame->best_effort_timestamp) * (double)(timeBase.num) / (double)(timeBase.den) - startTime;
		}
		
		double _getVideoFrameTimestamp(AVFrame* frame)
		{
			double startTime = (double)(m_format->start_time) / (double)AV_TIME_BASE;
			AVRational& timeBase = m_format->streams[m_streamVideo]->time_base;
			return (double)(frame->best_effort_timestamp) * (double)(timeBase.num) / (double)(timeBase.den) - startTime;
		}
		
		double _getVideoFrameDuration(AVFrame* frame)
		{
			AVRational& timeBase = m_format->streams[m_streamVideo]->time_base;
			return (double)(frame->pkt_duration) * (double)(timeBase.num) / (double)(timeBase.den);
		}
	};
	
	Ref<MediaPlayer> FFmpeg::createMediaPlayer(const MediaPlayerParam& param)
	{
		if (param.url.isEmpty() && param.filePath.isEmpty() && param.assetFileName.isEmpty()) {
			return sl_null;
		}
		Ref<MediaPlayer> player = _priv_FFmpegMediaPlayer::create(param);
		if (player.isNotNull()) {
			if (param.flagAutoStart) {
				player->resume();
			}
			return player;
		}
		return sl_null;
	}
	
	Ref<MediaPlayer> FFmpeg::openUrl(const String& url, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.url = url;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return createMediaPlayer(param);
	}
	
	Ref<MediaPlayer> FFmpeg::openFile(const String& filePath, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.filePath = filePath;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return createMediaPlayer(param);
	}
	
	Ref<MediaPlayer> FFmpeg::openAsset(const String& fileName, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.assetFileName = fileName;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return createMediaPlayer(param);
	}

}

