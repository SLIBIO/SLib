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

#include "slib/ui/video_view.h"

#include "slib/render/canvas.h"
#include "slib/graphics/image.h"
#include "slib/graphics/util.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(VideoView, RenderView)
	
	VideoView::VideoView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setSavingCanvasState(sl_false);
		
		setBackgroundColor(Color::Black, UIUpdateMode::Init);
		
		setRedrawMode(RedrawMode::WhenDirty);
		setDebugTextVisible(sl_false);
		
		m_flagRepeat = sl_true;
		m_rotation = RotationMode::Rotate0;
		m_flip = FlipMode::None;
		
		m_flagYUV = sl_false;
		m_rotationFrame = RotationMode::Rotate0;
		m_flipFrame = FlipMode::None;

		m_programRGB = new RenderProgram2D_PositionTexture;
		m_programYUV = new RenderProgram2D_PositionTextureYUV;
		m_programOES = new RenderProgram2D_PositionTextureOES;
		
		m_renderVideoParam.onUpdateFrame = SLIB_FUNCTION_WEAKREF(VideoView, updateCurrentFrame, this);
		
		m_flipFrameApplied = FlipMode::None;
		m_rotationFrameApplied = RotationMode::Rotate0;
		m_flipApplied = FlipMode::None;
		m_rotationApplied = RotationMode::Rotate0;

		m_scaleMode = ScaleMode::Stretch;
		m_gravity = Alignment::MiddleCenter;
		
		m_flagControlsVisible = sl_false;
	}
	
	VideoView::~VideoView()
	{
	}
	
	Ref<MediaPlayer> VideoView::getMediaPlayer()
	{
		return m_mediaPlayer;
	}
	
	void VideoView::setMediaPlayer(const Ref<MediaPlayer>& player)
	{
		m_mediaPlayer = player;
		if (player.isNotNull()) {
			setRedrawMode(RedrawMode::Continuously);
		} else {
			setRedrawMode(RedrawMode::WhenDirty);
		}
	}
	
	void VideoView::openUrl(const String& url, const MediaPlayerFlags& _flags)
	{
		MediaPlayerFlags flags = _flags | MediaPlayerFlags::NotSelfAlive;
		if (m_flagRepeat) {
			flags |= MediaPlayerFlags::Repeat;
		}
		Ref<MediaPlayer> player = MediaPlayer::openUrl(url, flags);
		if (player.isNotNull()) {
			setMediaPlayer(player);
		}
	}
	
	void VideoView::openFile(const String& filePath, const MediaPlayerFlags& _flags)
	{
		MediaPlayerFlags flags = _flags | MediaPlayerFlags::NotSelfAlive;
		if (m_flagRepeat) {
			flags |= MediaPlayerFlags::Repeat;
		}
		Ref<MediaPlayer> player = MediaPlayer::openFile(filePath, flags);
		if (player.isNotNull()) {
			setMediaPlayer(player);
		}
	}
	
	void VideoView::openAsset(const String& fileName, const MediaPlayerFlags& _flags)
	{
		MediaPlayerFlags flags = _flags | MediaPlayerFlags::NotSelfAlive;
		if (m_flagRepeat) {
			flags |= MediaPlayerFlags::Repeat;
		}
		Ref<MediaPlayer> player = MediaPlayer::openAsset(fileName, flags);
		if (player.isNotNull()) {
			setMediaPlayer(player);
		}
	}
	
	void VideoView::setSource(const String& source, const MediaPlayerFlags& flags)
	{
		if (source.isNotEmpty()) {
			if (source.startsWith("asset://")) {
				openAsset(source.substring(8), flags);
			} else if (source.indexOf(':') >= 0) {
				openUrl(source, flags);
			} else {
				openFile(source, flags);
			}
		}
	}
	
	sl_bool VideoView::isRepeat()
	{
		return m_flagRepeat;
	}
	
	void VideoView::setRepeat(sl_bool flagRepeat)
	{
		m_flagRepeat = flagRepeat;
		Ref<MediaPlayer> player = m_mediaPlayer;
		if (player.isNotNull()) {
			player->setAutoRepeat(flagRepeat);
		}
	}
	
	RotationMode VideoView::getRotation()
	{
		return m_rotation;
	}
	
	void VideoView::setRotation(const RotationMode& rotation)
	{
		m_rotation = rotation;
	}
	
	FlipMode VideoView::getFlip()
	{
		return m_flip;
	}
	
	void VideoView::setFlip(const FlipMode& flip)
	{
		m_flip = flip;
	}
	
	ScaleMode VideoView::getScaleMode()
	{
		return m_scaleMode;
	}
	
	void VideoView::setScaleMode(ScaleMode scaleMode)
	{
		m_scaleMode = scaleMode;
		requestRender();
	}
	
	Alignment VideoView::getGravity()
	{
		return m_gravity;
	}
	
	void VideoView::setGravity(Alignment align)
	{
		m_gravity = align;
		requestRender();
	}
	
	sl_bool VideoView::isControlsVisible()
	{
		return m_gravity;
	}
	
	void VideoView::setControlsVisible(sl_bool flag)
	{
		ObjectLocker lock(this);
		m_flagControlsVisible = flag;
		if (flag) {
			if (m_sliderSeek.isNull()) {
				m_sliderSeek = new Slider;
				if (m_sliderSeek.isNotNull()) {
					m_sliderSeek->setWidthFilling(1.0f, UIUpdateMode::Init);
					m_sliderSeek->setHeightWeight(0.05f, UIUpdateMode::Init);
					m_sliderSeek->setAlignParentBottom(UIUpdateMode::Init);
					addChild(m_sliderSeek);
					m_sliderSeek->setOnChange(SLIB_FUNCTION_WEAKREF(VideoView, _onSeek, this));
				}
			}
			_updateControls();
		}
		if (m_sliderSeek.isNotNull()) {
			m_sliderSeek->setVisible(sl_true);
		}
	}
	
	void VideoView::updateCurrentFrame(VideoFrame& frame)
	{
		ColorSpace colorSpace = BitmapFormats::getColorSpace(frame.image.format);
		if (colorSpace != ColorSpace::RGB && colorSpace != ColorSpace::YUV) {
			return;
		}
		Ref<Texture> texture = m_textureFrame;
		if (texture.isNotNull()) {
			if (texture->getWidth() != frame.image.width || texture->getHeight() != frame.image.height) {
				texture.setNull();
			}
		}
		if (texture.isNull()) {
			texture = Texture::create(frame.image.width, frame.image.height);
		}
		if (texture.isNotNull()) {
			Ref<Image> image = Ref<Image>::from(texture->getSource());
			BitmapData bitmapData(image->getWidth(), image->getHeight(), image->getColors());
			if (BitmapFormats::getColorSpace(frame.image.format) == ColorSpace::YUV) {
				bitmapData.format = BitmapFormat::YUVA;
				m_flagYUV = sl_true;
			} else {
				m_flagYUV = sl_false;
			}
			m_rotationFrame = frame.rotation;
			m_flipFrame = frame.flip;
			bitmapData.copyPixelsFrom(frame.image);
			texture->update();
			m_textureFrame = texture;
		}
		requestRender();
	}
	
	void VideoView::onDraw(Canvas* _canvas)
	{
		_updateControls();
		Rectangle rectBounds = getBounds();
		if (rectBounds.getWidth() < SLIB_EPSILON || rectBounds.getHeight() < SLIB_EPSILON) {
			return;
		}
		RenderCanvas* canvas = CastInstance<RenderCanvas>(_canvas);
		if (canvas) {
			Ref<RenderEngine> engine = canvas->getEngine();
			if (engine.isNotNull()) {
				Ref<MediaPlayer> mediaPlayer = m_mediaPlayer;
				if (mediaPlayer.isNotNull()) {
					m_renderVideoParam.glEngine = CastRef<GLRenderEngine>(engine);
					mediaPlayer->renderVideo(m_renderVideoParam);
				}
				Ref<Texture> texture;
				Matrix3 textureMatrix;
				Ref<RenderProgram2D_PositionTexture> program;
				if (m_renderVideoParam.glTextureOES.isNotNull()) {
					texture = m_renderVideoParam.glTextureOES;
					textureMatrix = m_renderVideoParam.glTextureTransformOES;
					program = m_programOES;
				} else {
					texture = m_textureFrame;
					textureMatrix = Matrix3::identity();
					if (m_flagYUV) {
						program = m_programYUV;
					} else {
						program = m_programRGB;
					}
				}
				Ref<VertexBuffer> vb = _applyFrameRotationAndFlip(m_flipFrame, m_rotationFrame, m_flip, m_rotation);
				if (vb.isNotNull() && texture.isNotNull() && program.isNotNull()) {
					sl_real sw = (sl_real)(texture->getWidth());
					sl_real sh = (sl_real)(texture->getHeight());
					if (m_rotationFrameApplied == RotationMode::Rotate90 || m_rotationFrameApplied == RotationMode::Rotate270) {
						Swap(sw, sh);
					}
					if (m_rotationApplied == RotationMode::Rotate90 || m_rotationApplied == RotationMode::Rotate270) {
						Swap(sw, sh);
					}
					Rectangle rectDraw;
					if (!(GraphicsUtil::calculateAlignRectangle(rectDraw, rectBounds, sw, sh, m_scaleMode, m_gravity))) {
						return;
					}
					Matrix3 mat = canvas->getTransformMatrixForRectangle(rectDraw);
					RenderProgramScope<RenderProgramState2D_PositionTexture> scope;
					if (scope.begin(engine.get(), program)) {
						scope->setTransform(mat);
						scope->setTexture(texture);
						scope->setTextureTransform(textureMatrix);
						scope->setColor(Color4f(1, 1, 1, canvas->getAlpha()));
						engine->drawPrimitive(4, vb, PrimitiveType::TriangleStrip);
					}
				}
			}
		}
	}
	
	Ref<VertexBuffer> VideoView::_applyFrameRotationAndFlip(FlipMode frameFlip, RotationMode frameRotation, FlipMode userFlip, RotationMode userRotation)
	{
		if (m_vbFrame.isNotNull() && m_flipFrameApplied == frameFlip && m_rotationFrameApplied == frameRotation && m_flipApplied == userFlip && m_rotationApplied == userRotation) {
			return m_vbFrame;
		}
		RenderVertex2D_PositionTexture v[] = {
			{ { 0, 0 }, { 0, 0 } }
			, { { 1, 0 }, { 1, 0 } }
			, { { 0, 1 }, { 0, 1 } }
			, { { 1, 1 }, { 1, 1 } }
		};
		Vector2 t;
		switch (frameFlip) {
			case FlipMode::Horizontal:
				Swap(v[0].texCoord, v[1].texCoord);
				Swap(v[2].texCoord, v[3].texCoord);
				break;
			case FlipMode::Vertical:
				Swap(v[0].texCoord, v[2].texCoord);
				Swap(v[1].texCoord, v[3].texCoord);
				break;
			case FlipMode::Both:
				Swap(v[0].texCoord, v[3].texCoord);
				Swap(v[1].texCoord, v[2].texCoord);
				break;
			default:
				break;
		}
		switch (frameRotation) {
			case RotationMode::Rotate90:
				t = v[0].texCoord;
				v[0].texCoord = v[2].texCoord;
				v[2].texCoord = v[3].texCoord;
				v[3].texCoord = v[1].texCoord;
				v[1].texCoord = t;
				break;
			case RotationMode::Rotate180:
				Swap(v[0].texCoord, v[3].texCoord);
				Swap(v[1].texCoord, v[2].texCoord);
				break;
			case RotationMode::Rotate270:
				t = v[0].texCoord;
				v[0].texCoord = v[1].texCoord;
				v[1].texCoord = v[3].texCoord;
				v[3].texCoord = v[2].texCoord;
				v[2].texCoord = t;
				break;
			default:
				break;
		}
		switch (userFlip) {
			case FlipMode::Horizontal:
				Swap(v[0].texCoord, v[1].texCoord);
				Swap(v[2].texCoord, v[3].texCoord);
				break;
			case FlipMode::Vertical:
				Swap(v[0].texCoord, v[2].texCoord);
				Swap(v[1].texCoord, v[3].texCoord);
				break;
			case FlipMode::Both:
				Swap(v[0].texCoord, v[3].texCoord);
				Swap(v[1].texCoord, v[2].texCoord);
				break;
			default:
				break;
		}
		switch (userRotation) {
			case RotationMode::Rotate90:
				t = v[0].texCoord;
				v[0].texCoord = v[2].texCoord;
				v[2].texCoord = v[3].texCoord;
				v[3].texCoord = v[1].texCoord;
				v[1].texCoord = t;
				break;
			case RotationMode::Rotate180:
				Swap(v[0].texCoord, v[3].texCoord);
				Swap(v[1].texCoord, v[2].texCoord);
				break;
			case RotationMode::Rotate270:
				t = v[0].texCoord;
				v[0].texCoord = v[1].texCoord;
				v[1].texCoord = v[3].texCoord;
				v[3].texCoord = v[2].texCoord;
				v[2].texCoord = t;
				break;
			default:
				break;
		}
		Ref<VertexBuffer> vb = VertexBuffer::create(v, sizeof(v));
		m_vbFrame = vb;
		m_flipFrameApplied = frameFlip;
		m_rotationFrameApplied = frameRotation;
		m_flipApplied = userFlip;
		m_rotationApplied = userRotation;
		return vb;
	}
	
	void VideoView::_updateControls()
	{
		Ref<MediaPlayer> player = m_mediaPlayer;
		if (player.isNull()) {
			return;
		}
		if (m_sliderSeek.isNotNull()) {
			double duration = player->getDuration();
			if (duration > 0) {
				m_sliderSeek->setMaximumValue((float)duration, UIUpdateMode::None);
				m_sliderSeek->setValue((float)(player->getCurrentTime()));
			} else {
				m_sliderSeek->setMaximumValue(1, UIUpdateMode::None);
				m_sliderSeek->setValue(0);
			}
		}
	}
	
	void VideoView::_onSeek(Slider* slider, float value)
	{
		Ref<MediaPlayer> player = m_mediaPlayer;
		if (player.isNotNull()) {
			player->seekTo(value);
		}
	}

}
