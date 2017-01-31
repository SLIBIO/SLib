#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/media/media_player.h"

#include "../../../inc/slib/core/platform_windows.h"

SLIB_MEDIA_NAMESPACE_BEGIN

Ref<MediaPlayer> MediaPlayer::_createNative(const MediaPlayerParam& param)
{
	return sl_null;
}

SLIB_MEDIA_NAMESPACE_END

#endif
