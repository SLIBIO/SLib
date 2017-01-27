#include "../../../inc/slib/ui/video_view.h"

#include "../../../inc/slib/render/canvas.h"
#include "../../../inc/slib/graphics/image.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(VideoView, RenderView)

VideoView::VideoView()
{
	setBackgroundColor(Color::Black, UIUpdateMode::Init);
	
	setRedrawMode(RedrawMode::WhenDirty);
	setDebugTextVisible(sl_false);
	
	m_flagRepeat = sl_true;
	
	m_flagYUV = sl_false;
	
	m_programRGB = new RenderProgram2D_PositionTexture;
	m_programYUV = new RenderProgram2D_PositionTextureYUV;

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
		player->resume();
		setRedrawMode(RedrawMode::Continuously);
	} else {
		setRedrawMode(RedrawMode::WhenDirty);
	}
}

void VideoView::openUrl(const String& url)
{
	MediaPlayerFlags flags = MediaPlayerFlags::NotSelfAlive | MediaPlayerFlags::Video | MediaPlayerFlags::NotStart;
	if (m_flagRepeat) {
		flags |= MediaPlayerFlags::Repeat;
	}
	Ref<MediaPlayer> player = MediaPlayer::openUrl(url, flags);
	if (player.isNotNull()) {
		setMediaPlayer(player);
	}
}

void VideoView::openFile(const String& filePath)
{
	MediaPlayerFlags flags = MediaPlayerFlags::NotSelfAlive | MediaPlayerFlags::Video | MediaPlayerFlags::NotStart;
	if (m_flagRepeat) {
		flags |= MediaPlayerFlags::Repeat;
	}
	Ref<MediaPlayer> player = MediaPlayer::openFile(filePath, flags);
	if (player.isNotNull()) {
		setMediaPlayer(player);
	}
}

void VideoView::openAsset(const String& fileName)
{
	MediaPlayerFlags flags = MediaPlayerFlags::NotSelfAlive | MediaPlayerFlags::Video | MediaPlayerFlags::NotStart;
	if (m_flagRepeat) {
		flags |= MediaPlayerFlags::Repeat;
	}
	Ref<MediaPlayer> player = MediaPlayer::openAsset(fileName, flags);
	if (player.isNotNull()) {
		setMediaPlayer(player);
	}
}

void VideoView::setSource(const String& source)
{
	if (source.isNotEmpty()) {
		if (source.startsWith("asset://")) {
			openAsset(source.substring(8));
		} else if (source.indexOf(':') >= 0) {
			openUrl(source);
		} else {
			openFile(source);
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

void VideoView::updateCurrentFrame(const VideoFrame* frame)
{
	ColorSpace colorSpace = BitmapFormats::getColorSpace(frame->image.format);
	if (colorSpace != ColorSpace::RGB && colorSpace != ColorSpace::YUV) {
		return;
	}
	Ref<Texture> texture = m_textureFrame;
	if (texture.isNotNull()) {
		if (texture->getWidth() != frame->image.width || texture->getHeight() != frame->image.height) {
			texture.setNull();
		}
	}
	if (texture.isNull()) {
		texture = Texture::create(frame->image.width, frame->image.height);
	}
	if (texture.isNotNull()) {
		Ref<Image> image = Ref<Image>::from(texture->getSource());
		BitmapData bitmapData(image->getWidth(), image->getHeight(), image->getColors());
		if (BitmapFormats::getColorSpace(frame->image.format) == ColorSpace::YUV) {
			bitmapData.format = BitmapFormat::YUVA;
			m_flagYUV = sl_true;
		} else {
			m_flagYUV = sl_false;
		}
		bitmapData.copyPixelsFrom(frame->image);
		texture->update();
		m_textureFrame = texture;
	}
	requestRender();
}

void VideoView::onDraw(Canvas* _canvas)
{
	RenderCanvas* canvas = CastInstance<RenderCanvas>(_canvas);
	if (canvas) {
		Ref<RenderEngine> engine = canvas->getEngine();
		if (engine.isNotNull()) {
			sl_bool flagRenderTexture = sl_false;
			Ref<MediaPlayer> mediaPlayer = m_mediaPlayer;
			if (mediaPlayer.isNotNull()) {
				MediaPlayerRenderVideoParam param;
				param.onUpdateFrame = SLIB_FUNCTION_WEAKREF(VideoView, updateCurrentFrame, this);
				mediaPlayer->renderVideo(param);
			}
			Ref<Texture> texture = m_textureFrame;
			if (texture.isNotNull()) {
				Ref<RenderProgram2D_PositionTexture> program;
				if (m_flagYUV) {
					program = m_programYUV;
				} else {
					program = m_programRGB;
				}
				if (program.isNotNull()) {
					engine->drawTexture2D(program, Rectangle(-1, -1, 1, 1), m_textureFrame);
				}
			}
		}
	}
}

SLIB_UI_NAMESPACE_END
