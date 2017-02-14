#include "../../../inc/slib/media/media_player.h"

#include "../../../inc/slib/core/map.h"
#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	IMediaPlayerListener::IMediaPlayerListener()
	{
	}

	IMediaPlayerListener::~IMediaPlayerListener()
	{
	}

	void IMediaPlayerListener::onReadyToPlay(MediaPlayer* player)
	{
	}


	MediaPlayerParam::MediaPlayerParam()
	{
		flagVideo = sl_false;
		
		flagAutoStart = sl_true;
		flagAutoRepeat = sl_false;
		
		flagSelfAlive = sl_false;
	}

	MediaPlayerParam::~MediaPlayerParam()
	{
	}


	MediaPlayerRenderVideoParam::MediaPlayerRenderVideoParam()
	{
		flagUpdated = sl_false;
		
		glTextureTransformOES = Matrix3::identity();
		
		_glEngineIdLast = 0;
		_glTextureNameOES = 0;
	}

	MediaPlayerRenderVideoParam::~MediaPlayerRenderVideoParam()
	{
	}


	typedef HashMap< MediaPlayer*, Ref<MediaPlayer> > _MediaPlayersMap;
	SLIB_SAFE_STATIC_GETTER(_MediaPlayersMap, _getMediaPlayersMap)

	SLIB_DEFINE_OBJECT(MediaPlayer, Object)

	MediaPlayer::MediaPlayer()
	{
		m_flagSelfAlive = sl_false;
		m_flagAutoRepeat = sl_false;
	}

	MediaPlayer::~MediaPlayer()
	{
	}

	Ref<MediaPlayer> MediaPlayer::create(const MediaPlayerParam& param)
	{
		if (param.url.isEmpty() && param.filePath.isEmpty() && param.assetFileName.isEmpty()) {
			return sl_null;
		}
		Ref<MediaPlayer> player = _createNative(param);
		if (player.isNotNull()) {
			if (param.flagAutoStart) {
				player->resume();
			}
			return player;
		}
		return sl_null;
	}

	Ref<MediaPlayer> MediaPlayer::openUrl(const String& url, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.url = url;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return create(param);
	}

	Ref<MediaPlayer> MediaPlayer::openFile(const String& filePath, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.filePath = filePath;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return create(param);
	}

	Ref<MediaPlayer> MediaPlayer::openAsset(const String& fileName, const MediaPlayerFlags& flags)
	{
		MediaPlayerParam param;
		param.assetFileName = fileName;
		param.flagAutoStart = (flags & MediaPlayerFlags::NotStart) == 0;
		param.flagAutoRepeat = (flags & MediaPlayerFlags::Repeat) != 0;
		param.flagVideo = (flags & MediaPlayerFlags::Video) != 0;
		param.flagSelfAlive = (flags & MediaPlayerFlags::NotSelfAlive) == 0;
		return create(param);
	}

	sl_bool MediaPlayer::isAutoRepeat()
	{
		return m_flagAutoRepeat;
	}

	void MediaPlayer::setAutoRepeat(sl_bool flagRepeat)
	{
		m_flagAutoRepeat = flagRepeat;
	}

	void MediaPlayer::_onReadyToPlay()
	{
		PtrLocker<IMediaPlayerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onReadyToPlay(this);
		}
		getOnReadyToPlay()(this);
	}

	void MediaPlayer::_init(const MediaPlayerParam& param)
	{
		m_flagAutoRepeat = param.flagAutoRepeat;
		m_flagSelfAlive = param.flagSelfAlive;
		setListener(param.listener);
		setOnReadyToPlay(param.onReadyToPlay);
	}

	void MediaPlayer::_addToMap()
	{
		if (m_flagSelfAlive) {
			_MediaPlayersMap* map = _getMediaPlayersMap();
			if (map) {
				map->put(this, this);
			}
		}
	}

	void MediaPlayer::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			_MediaPlayersMap* map = _getMediaPlayersMap();
			if (map) {
				map->remove(this);
			}
		}
	}

}

