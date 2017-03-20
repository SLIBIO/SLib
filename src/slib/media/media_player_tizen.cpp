/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/media/media_player.h"
#include "../../../inc/slib/core/platform_tizen.h"

#include <player.h>

namespace slib
{
	class _MediaPlayer : public MediaPlayer
	{
	public:
		player_h m_player;

		sl_bool m_flagInited;
		sl_bool m_flagPlaying;
		sl_bool m_flagPrepared;

		sl_real m_volume;

	public:
		_MediaPlayer()
		{
			m_player = sl_null;
			m_flagInited = sl_false;
			m_flagPlaying = sl_false;
			m_flagPrepared = sl_false;
			m_volume = 1;
		}

		~_MediaPlayer()
		{
			SLIB_REFERABLE_DESTRUCTOR
			_release(sl_true);
		}

	public:
		static Ref<_MediaPlayer> create(const MediaPlayerParam& param)
		{
			player_h player;
			int errorCode = ::player_create(&player);

			if (PLAYER_ERROR_NONE != errorCode) {
				return sl_null;
			}

			if (param.url.isNotEmpty()) {
				errorCode = ::player_set_uri(player, param.url.getData());
			} else if (param.filePath.isNotEmpty()) {
				errorCode = ::player_set_uri(player, param.filePath.getData());
			} else if (param.assetFileName.isNotEmpty()) {
				errorCode = ::player_set_uri(player, Tizen::getAssetFilePath(param.assetFileName).getData());
			}

			if (PLAYER_ERROR_NONE == errorCode) {

				Ref<_MediaPlayer> ret = new _MediaPlayer;

				if (ret.isNotNull()) {

					ret->m_player = player;
					ret->m_flagInited = sl_true;
					ret->_init(param);

					errorCode = ::player_prepare_async(player, _MediaPlayer::__onPrepared, ret.get());

					if (PLAYER_ERROR_NONE == errorCode) {

						::player_set_completed_cb(player, _MediaPlayer::__onReachEnd, ret.get());

						if (param.flagAutoStart) {
							ret->resume();
						}

						return ret;
					}

					return sl_null;
				}

				::player_destroy(player);
			}

			return sl_null;

		}

	public:
		static void __onPrepared(void* userData)
		{
			Ref<_MediaPlayer> player = (_MediaPlayer*)userData;
			player->_onPrepared();

		}

		static void __onReachEnd(void* userData)
		{
			Ref<_MediaPlayer> player = (_MediaPlayer*)userData;
			player->_onReachEnd();
		}

		void _onPrepared()
		{
			ObjectLocker lock(this);
			if (!m_flagInited) {
				return;
			}
			player_state_e state = PLAYER_STATE_NONE;
			::player_get_state(m_player, &state);
			if (state == PLAYER_STATE_READY) {
				m_flagPrepared = sl_true;
				if (m_flagPlaying) {
					::player_start(m_player);
				}
				::player_set_volume(m_player, m_volume, m_volume);
			} else {
				_release(sl_false);
			}
		}

		void _onReachEnd()
		{
			ObjectLocker lock(this);
			if (!m_flagInited) {
				return;
			}
			if (m_flagAutoRepeat) {
				::player_set_play_position(m_player, 0, sl_true, NULL, NULL);
			} else {
				_release(sl_false);
			}
		}

		void _release(sl_bool flagFromDestructor)
		{
			ObjectLocker lock(this);
			if (!m_flagInited) {
				return;
			}

			m_flagInited = sl_false;
			m_flagPlaying = sl_false;
			lock.unlock();

			if (!flagFromDestructor) {
				_removeFromMap();
			}

			::player_stop(m_player);
			::player_unprepare(m_player);
			::player_destroy(m_player);

			m_player = sl_null;

		}

		void release()
		{
			_release(sl_false);
		}

		void resume()
		{
			ObjectLocker lock(this);

			if (!m_flagInited) {
				return;
			}
			if (m_flagPlaying) {
				return;
			}

			if (m_flagPrepared) {
				int errorCode = ::player_start(m_player);
				if (PLAYER_ERROR_NONE != errorCode) {
					return;
				}
			}

			m_flagPlaying = sl_true;

			_addToMap();

		}

		void pause()
		{
			ObjectLocker lock(this);

			if (!m_flagInited) {
				return;
			}
			if (!m_flagPlaying) {
				return;
			}

			if (m_flagPrepared) {
				int errorCode = ::player_pause(m_player);
				if (PLAYER_ERROR_NONE != errorCode) {
					return;
				}
			}

			m_flagPlaying = sl_false;

			_removeFromMap();

		}

		sl_bool isPlaying()
		{
			return m_flagPlaying;
		}

		sl_real getVolume()
		{
			return m_volume;
		}

		void setVolume(sl_real volume)
		{
			ObjectLocker lock(this);

			if (!m_flagInited) {
				return;
			}

			if (m_flagPrepared) {
				::player_set_volume(m_player, volume, volume);
			}

			m_volume = volume;
		}

		void renderVideo(MediaPlayerRenderVideoParam& param)
		{
		}

	};

	Ref<MediaPlayer> MediaPlayer::_createNative(const MediaPlayerParam& param)
	{
		return _MediaPlayer::create(param);
	}
}

#endif
