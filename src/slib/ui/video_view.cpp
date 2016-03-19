#include "../../../inc/slib/ui/video_view.h"

#include "../../../inc/slib/graphics/image.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(VideoView, RenderView)

VideoView::VideoView()
{
	setRedrawMode(RedrawMode::WhenDirty);
	m_flagYUV = sl_false;
	
	m_programRGB = new RenderProgram2D_PositionTexture;
	m_programYUV = new RenderProgram2D_PositionTextureYUV;
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

void VideoView::onFrame(RenderEngine* engine)
{
	engine->clearColor(Color::Black);
	if (m_flagYUV) {
		engine->drawTexture2D(-1, -1, 2, 2, m_textureFrame, m_programYUV);
	} else {
		engine->drawTexture2D(-1, -1, 2, 2, m_textureFrame, m_programRGB);
	}
}

SLIB_UI_NAMESPACE_END
