/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_MEDIA_MEDIA_PLAYER
#define CHECKHEADER_SLIB_MEDIA_MEDIA_PLAYER

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"
#include "../core/function.h"

#include "../media/video_frame.h"
#include "../render/texture.h"
#include "../render/opengl_engine.h"

namespace slib
{
	class MediaPlayer;
	
	class SLIB_EXPORT MediaPlayerParam
	{
	public:
		String url;
		String filePath;
		String assetFileName;
		
		sl_bool flagVideo;
		
		sl_bool flagAutoStart;
		sl_bool flagAutoRepeat;
		
		sl_bool flagSelfAlive;
		
		Function<void(MediaPlayer*)> onReadyToPlay;
		Function<void(MediaPlayer*)> onComplete;
		
	public:
		MediaPlayerParam();
		
		~MediaPlayerParam();
		
	};
	
	class MediaPlayerFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(MediaPlayerFlags, value)
		
	public:
		enum {
			Default = 0,
			NotStart = 1,
			Repeat = 2,
			Video = 16,
			NotSelfAlive = 32
		};
	};
	
	// save this structure after rendering and reuse for next frame
	class MediaPlayerRenderVideoParam
	{
	public:
		// in
		Function<void(VideoFrame*)> onUpdateFrame;
		
		// in
		Ref<GLRenderEngine> glEngine;
		
		// out
		Ref<Texture> glTextureOES;
		// out
		Matrix3 glTextureTransformOES;
		
		// out
		sl_bool flagUpdated;
		
	public:
		// used interally by MediaPlayer
		sl_uint64 _glEngineIdLast;
		sl_uint32 _glTextureNameOES;
		
	public:
		MediaPlayerRenderVideoParam();
		
		~MediaPlayerRenderVideoParam();
		
	};
	
	class SLIB_EXPORT MediaPlayer : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		MediaPlayer();
		
		~MediaPlayer();
		
	public:
		static Ref<MediaPlayer> create(const MediaPlayerParam& param);
		
		static Ref<MediaPlayer> openUrl(const String& url, const MediaPlayerFlags& flags = MediaPlayerFlags::Default);
		
		static Ref<MediaPlayer> openFile(const String& filePath, const MediaPlayerFlags& flags = MediaPlayerFlags::Default);
		
		static Ref<MediaPlayer> openAsset(const String& fileName, const MediaPlayerFlags& flags = MediaPlayerFlags::Default);
		
	public:
		virtual void release() = 0;
		
		virtual void resume() = 0;
		
		virtual void pause() = 0;
		
		virtual sl_bool isPlaying() = 0;
		
		virtual sl_real getVolume() = 0;
		
		virtual void setVolume(sl_real) = 0;
		
		// Seconds, negative means the infinite duration
		virtual double getDuration() = 0;
		
		// Seconds
		virtual double getCurrentTime() = 0;
		
		// Seconds
		virtual void seekTo(double seconds) = 0;

		virtual void renderVideo(MediaPlayerRenderVideoParam& param) = 0;
		
	public:
		sl_bool isAutoRepeat();
		
		virtual void setAutoRepeat(sl_bool flagRepeat);
				
	public:
		SLIB_PROPERTY_FUNCTION(void(MediaPlayer*), OnReadyToPlay)
		
		SLIB_PROPERTY_FUNCTION(void(MediaPlayer*), OnComplete)
		
	protected:
		void _onReadyToPlay();
		
		void _onComplete();
		
	protected:
		static Ref<MediaPlayer> _createNative(const MediaPlayerParam& param);
		
		void _init(const MediaPlayerParam& param);
		
		void _addToMap();
		
		void _removeFromMap();
		
	protected:
		sl_bool m_flagSelfAlive;
		sl_bool m_flagAutoRepeat;

	};

}

#endif
