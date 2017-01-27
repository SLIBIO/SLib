#ifndef CHECKHEADER_SLIB_UI_VIDEO_VIEW
#define CHECKHEADER_SLIB_UI_VIDEO_VIEW

#include "definition.h"

#include "render_view.h"

#include "../media/video_frame.h"
#include "../media/media_player.h"

SLIB_UI_NAMESPACE_BEGIN

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
	
	void updateCurrentFrame(const VideoFrame* frame);
	
protected:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	AtomicRef<MediaPlayer> m_mediaPlayer;
	sl_bool m_flagRepeat;
	
	AtomicRef<Texture> m_textureFrame;
	sl_bool m_flagYUV;
	
	Ref<RenderProgram2D_PositionTexture> m_programRGB;
	Ref<RenderProgram2D_PositionTextureYUV> m_programYUV;
	
};

SLIB_UI_NAMESPACE_END

#endif
