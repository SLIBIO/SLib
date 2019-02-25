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

#ifndef CHECKHEADER_SLIB_UI_VIDEO_VIEW
#define CHECKHEADER_SLIB_UI_VIDEO_VIEW

#include "definition.h"

#include "render_view.h"

#include "slider.h"

#include "../media/video_frame.h"
#include "../media/media_player.h"

namespace slib
{

	class SLIB_EXPORT VideoView : public RenderView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		VideoView();
		
		~VideoView();
		
	public:
		Ref<MediaPlayer> getMediaPlayer();
		
		void setMediaPlayer(const Ref<MediaPlayer>& player);
		
		
		void openUrl(const String& url);
		
		void openFile(const String& filePath);
		
		void openAsset(const String& fileName);
		
		void setSource(const String& source);
		
		
		sl_bool isRepeat();
		
		void setRepeat(sl_bool flagRepeat);
		
		
		RotationMode getRotation();
		
		void setRotation(const RotationMode& rotation);
		
		
		FlipMode getFlip();
		
		void setFlip(const FlipMode& flip);

		
		ScaleMode getScaleMode();
		
		void setScaleMode(ScaleMode scaleMode);
		
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align);
		
		
		sl_bool isControlsVisible();
		
		void setControlsVisible(sl_bool flag);
		
		
		void updateCurrentFrame(VideoFrame& frame);
		
	protected:
		void onDraw(Canvas* canvas) override;
		
	protected:
		Ref<VertexBuffer> _applyFrameRotationAndFlip(FlipMode frameFlip, RotationMode frameRotation, FlipMode userFlip, RotationMode userRotation);
		
		void _updateControls();
		
		void _onSeek(Slider* slider, float value);
				
	protected:
		AtomicRef<MediaPlayer> m_mediaPlayer;
		sl_bool m_flagRepeat;
		RotationMode m_rotation;
		FlipMode m_flip;
		RotationMode m_rotationFrame;
		FlipMode m_flipFrame;

		AtomicRef<Texture> m_textureFrame;
		sl_bool m_flagYUV;
		
		Ref<RenderProgram2D_PositionTexture> m_programRGB;
		Ref<RenderProgram2D_PositionTextureYUV> m_programYUV;
		Ref<RenderProgram2D_PositionTextureOES> m_programOES;
		
		AtomicRef<VertexBuffer> m_vbFrame;
		FlipMode m_flipFrameApplied;
		RotationMode m_rotationFrameApplied;
		FlipMode m_flipApplied;
		RotationMode m_rotationApplied;

		MediaPlayerRenderVideoParam m_renderVideoParam;
		
		ScaleMode m_scaleMode;
		Alignment m_gravity;
		
		sl_bool m_flagControlsVisible;
		Ref<Slider> m_sliderSeek;
		
	};

}

#endif
