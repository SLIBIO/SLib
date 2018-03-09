/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/media_player.h"

#include "slib/core/hash_map.h"
#include "slib/core/safe_static.h"

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
	
	void IMediaPlayerListener::onComplete(MediaPlayer* player)
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


	typedef CHashMap< MediaPlayer*, Ref<MediaPlayer> > _priv_MediaPlayersMap;
	SLIB_SAFE_STATIC_GETTER(_priv_MediaPlayersMap, _getMediaPlayersMap)

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
	
	void MediaPlayer::_onComplete()
	{
		PtrLocker<IMediaPlayerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onComplete(this);
		}
		getOnComplete()(this);
	}

	void MediaPlayer::_init(const MediaPlayerParam& param)
	{
		m_flagAutoRepeat = param.flagAutoRepeat;
		m_flagSelfAlive = param.flagSelfAlive;
		setListener(param.listener);
		setOnReadyToPlay(param.onReadyToPlay);
		setOnComplete(param.onComplete);
	}

	void MediaPlayer::_addToMap()
	{
		if (m_flagSelfAlive) {
			_priv_MediaPlayersMap* map = _getMediaPlayersMap();
			if (map) {
				map->put(this, this);
			}
		}
	}

	void MediaPlayer::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			_priv_MediaPlayersMap* map = _getMediaPlayersMap();
			if (map) {
				map->remove(this);
			}
		}
	}

}

