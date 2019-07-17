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

#include "slib/media/media_player.h"

#include "slib/core/hash_map.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace media_player
		{

			typedef CHashMap< MediaPlayer*, Ref<MediaPlayer> > MediaPlayersMap;

			SLIB_SAFE_STATIC_GETTER(MediaPlayersMap, GetMediaPlayersMap)

		}
	}

	using namespace priv::media_player;

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(MediaPlayerParam)

	MediaPlayerParam::MediaPlayerParam()
	{
		flagVideo = sl_false;
		
		flagAutoStart = sl_true;
		flagAutoRepeat = sl_false;
		
		flagSelfAlive = sl_false;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(MediaPlayerRenderVideoParam)
	
	MediaPlayerRenderVideoParam::MediaPlayerRenderVideoParam()
	{
		flagUpdated = sl_false;
		
		glTextureTransformOES = Matrix3::identity();
		
		_glEngineIdLast = 0;
		_glTextureNameOES = 0;
	}

	
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
		getOnReadyToPlay()(this);
	}
	
	void MediaPlayer::_onComplete()
	{
		getOnComplete()(this);
	}

	void MediaPlayer::_init(const MediaPlayerParam& param)
	{
		m_flagAutoRepeat = param.flagAutoRepeat;
		m_flagSelfAlive = param.flagSelfAlive;
		setOnReadyToPlay(param.onReadyToPlay);
		setOnComplete(param.onComplete);
	}

	void MediaPlayer::_addToMap()
	{
		if (m_flagSelfAlive) {
			MediaPlayersMap* map = GetMediaPlayersMap();
			if (map) {
				map->put(this, this);
			}
		}
	}

	void MediaPlayer::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			MediaPlayersMap* map = GetMediaPlayersMap();
			if (map) {
				map->remove(this);
			}
		}
	}

}

