#include "../../../inc/slib/ui/video_view.h"

#include "../../../inc/slib/render/canvas.h"
#include "../../../inc/slib/graphics/image.h"

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
		
		m_flagYUV = sl_false;
		
		m_programRGB = new RenderProgram2D_PositionTexture;
		m_programYUV = new RenderProgram2D_PositionTextureYUV;
		m_programOES = new RenderProgram2D_PositionTextureOES;
		
		m_renderVideoParam.onUpdateFrame = SLIB_FUNCTION_WEAKREF(VideoView, updateCurrentFrame, this);
		
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
				if (texture.isNotNull() && program.isNotNull()) {
					Matrix3 mat = canvas->getTransformMatrixForRectangle(getBounds());
					RenderProgramScope<RenderProgramState2D_PositionTexture> scope;
					if (scope.begin(engine.get(), program)) {
						scope->setTransform(mat);
						scope->setTexture(texture);
						scope->setTextureTransform(textureMatrix);
						scope->setColor(Color4f(1, 1, 1, canvas->getAlpha()));
						Ref<VertexBuffer> vb = engine->getDefaultVertexBufferForDrawTexture2D();
						engine->drawPrimitive(4, vb, PrimitiveType::TriangleStrip);
					}
				}
			}
		}
	}

}
