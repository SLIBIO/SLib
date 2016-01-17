#include "../../../inc/slib/ui/video_view.h"

#include "../../../inc/slib/graphics/image.h"

SLIB_UI_NAMESPACE_BEGIN
VideoView::VideoView()
{
	setRedrawMode(redrawMode_WhenDirty);
	m_flagYUV = sl_false;
	
	m_programRGB = new RenderProgram2D_PositionTexture;
	m_programYUV = new RenderProgram2D_PositionTextureYUV;
}

void VideoView::updateCurrentFrame(const VideoFrame* frame)
{
	ColorSpace colorSpace = frame->image.format.getColorSpace();
	if (colorSpace != colorSpace_RGB && colorSpace != colorSpace_YUV) {
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
		if (frame->image.format.getColorSpace() == colorSpace_YUV) {
			bitmapData.format = bitmapFormat_YUVA;
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
	engine->clearColor(Color::black());
	if (m_flagYUV) {
		engine->drawTexture2D(-1, -1, 2, 2, m_textureFrame, m_programYUV);
	} else {
		engine->drawTexture2D(-1, -1, 2, 2, m_textureFrame, m_programRGB);
	}
}
SLIB_UI_NAMESPACE_END
