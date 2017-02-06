#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/media/media_player.h"

#include "../../../inc/slib/render/opengl.h"
#include "../../../inc/slib/core/safe_static.h"

#include "../../../inc/slib/core/platform_android.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

SLIB_MEDIA_NAMESPACE_BEGIN

class _Android_MediaPlayer;

typedef HashMap<jlong, WeakRef<_Android_MediaPlayer> > _AndroidMediaPlayerMap;
SLIB_SAFE_STATIC_GETTER(_AndroidMediaPlayerMap, _AndroidMediaPlayerMap_get)

void _MediaPlayer_onCompleted(JNIEnv* env, jobject _this, jlong instance);
void _MediaPlayer_onPrepared(JNIEnv* env, jobject _this, jlong instance);

SLIB_JNI_BEGIN_CLASS(_JMediaPlayer, "slib/platform/android/media/SMediaPlayer")
	SLIB_JNI_STATIC_METHOD(openUrl, "openUrl", "(Ljava/lang/String;)Lslib/platform/android/media/SMediaPlayer;");
	SLIB_JNI_STATIC_METHOD(openAsset, "openAsset", "(Landroid/content/Context;Ljava/lang/String;)Lslib/platform/android/media/SMediaPlayer;");
	SLIB_JNI_METHOD(setInstance, "setInstance", "(J)V");
	SLIB_JNI_METHOD(start, "start", "()V");
	SLIB_JNI_METHOD(pause, "pause", "()V");
	SLIB_JNI_METHOD(stop, "stop", "()V");
	SLIB_JNI_METHOD(isPlaying, "isPlaying", "()Z");
	SLIB_JNI_METHOD(setVolume, "setVolume", "(F)V");
	SLIB_JNI_METHOD(getVolume, "getVolume", "()F");
	SLIB_JNI_METHOD(setLooping, "setLooping", "(Z)V");
	SLIB_JNI_METHOD(renderVideo, "renderVideo", "(IZ)Z");
	SLIB_JNI_NATIVE(onCompleted, "nativeOnCompleted", "(J)V", _MediaPlayer_onCompleted);
	SLIB_JNI_NATIVE(onPrepared, "nativeOnPrepared", "(J)V", _MediaPlayer_onPrepared);
	SLIB_JNI_OBJECT_FIELD(mTextureMatrix, "[F");
SLIB_JNI_END_CLASS

class _Android_MediaPlayer : public MediaPlayer
{
public:
	JniGlobal<jobject> m_player;
	sl_bool m_flagInited;
	sl_bool m_flagPlaying;
	sl_bool m_flagPrepared;
	sl_bool m_flagVideo;

public:
	_Android_MediaPlayer()
	{
		m_flagInited = sl_false;
		m_flagPlaying = sl_false;
		m_flagPrepared = sl_false;
		m_flagVideo = sl_false;
	}

	~_Android_MediaPlayer()
	{
		SLIB_REFERABLE_DESTRUCTOR
		release();
		_AndroidMediaPlayerMap* map = _AndroidMediaPlayerMap_get();
		if (map) {
			map->remove((jlong) this);
		}
	}

public:
	static Ref<_Android_MediaPlayer> create(const MediaPlayerParam& param)
	{		
		JniLocal<jobject> player;
		if (param.url.isNotEmpty()) {
			JniLocal<jstring> url = Jni::getJniString(param.url);
			player = _JMediaPlayer::openUrl.callObject(sl_null, url.get());
		} else if (param.filePath.isNotEmpty()) {
			JniLocal<jstring> filePath = Jni::getJniString(param.filePath);
			player = _JMediaPlayer::openUrl.callObject(sl_null, filePath.get());
		} else if (param.assetFileName.isNotEmpty()) {
			JniLocal<jstring> assetFileName = Jni::getJniString(param.assetFileName);
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				player = _JMediaPlayer::openAsset.callObject(sl_null, jactivity, assetFileName.get());
			}
		}
		if (player.isNull()) {
			return sl_null;
		}

		Ref<_Android_MediaPlayer> ret = new _Android_MediaPlayer;
		if (ret.isNotNull()) {
			ret->m_player = player;
			ret->_init(param);
			_AndroidMediaPlayerMap* map = _AndroidMediaPlayerMap_get();
			if (map) {
				jlong instance = (jlong)(ret.get());
				map->put(instance, ret);
				_JMediaPlayer::setInstance.call(player.get(), instance);
				_JMediaPlayer::setLooping.call(player.get(), param.flagAutoRepeat);
				ret->m_flagVideo = param.flagVideo;
				ret->m_flagInited = sl_true;
				return ret;
			}
		}
		return sl_null;
	}

	// override
	void release()
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return;
		}
		m_flagInited = sl_false;
		if (m_flagPrepared) {
			JniGlobal<jobject> player = m_player;
			if (player.isNotNull()) {
				_JMediaPlayer::stop.call(player.get());
			}
		}
		_removeFromMap();
	}

	// override
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
			_JMediaPlayer::start.call(m_player.get());
		}
		m_flagPlaying = sl_true;
		_addToMap();
	}

	// override
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
			_JMediaPlayer::pause.call(m_player.get());
		}
		m_flagPlaying = sl_false;
		_removeFromMap();
	}

	// override
	sl_bool isPlaying()
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return sl_false;
		}
		return m_flagPlaying;
	}

	// override
	void setVolume(sl_real volume)
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return;
		}

		_JMediaPlayer::setVolume.call(m_player.get(), volume);
	}

	//override
	sl_real getVolume()
	{
		return (sl_real) _JMediaPlayer::getVolume.callFloat(m_player.get());
	}

	// override
	void setAutoRepeat(sl_bool flagRepeat)
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return;
		}
		_JMediaPlayer::setLooping.call(m_player.get(), flagRepeat);
	}

	// override
	void renderVideo(MediaPlayerRenderVideoParam& param)
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return;
		}
		if (!m_flagVideo) {
			return;
		}
		if (param.glEngine.isNull()) {
			return;
		}
		sl_uint64 engineId = param.glEngine->getUniqueId();
		if (param._glEngineIdLast != engineId) {
			param.glTextureOES.setNull();
		}
		param._glEngineIdLast = engineId;
		GLuint textureName = 0;
		sl_bool flagResetTexture = sl_false;
		if (param.glTextureOES.isNull()) {
			glGenTextures(1, &textureName);
			if (textureName == 0) {
				return;
			}
			param.glTextureOES = param.glEngine->createTextureFromName(GL_TEXTURE_EXTERNAL_OES, textureName);
			if (param.glTextureOES.isNull()) {
				return;
			}
			param._glTextureNameOES = textureName;
			flagResetTexture = sl_true;
		} else {
			textureName = param._glTextureNameOES;
			if (textureName == 0) {
				return;
			}
		}
		param.flagUpdated = _JMediaPlayer::renderVideo.callBoolean(m_player.get(), textureName, flagResetTexture) != 0;
		if (param.flagUpdated) {
			JniLocal<jfloatArray> arr = (jfloatArray)(_JMediaPlayer::mTextureMatrix.get(m_player.get()));
			if (arr.isNotNull()) {
				float t[16];
				Jni::getFloatArrayRegion(arr.get(), 0, 16, t);
				Matrix3 mat;
				mat.m00 = t[0];
				mat.m01 = t[1];
				mat.m02 = 0;
				mat.m10 = -t[4];
				mat.m11 = -t[5];
				mat.m12 = 0;
				mat.m20 = t[12];
				mat.m21 = 1-t[13];
				mat.m22 = 1;
				param.glTextureTransformOES = mat;
			}
		}
	}

	void __onPrepared()
	{
		ObjectLocker lock(this);
		if (!m_flagInited) {
			return;
		}
		m_flagPrepared = sl_true;
		if (m_flagPlaying) {
			_JMediaPlayer::start.call(m_player.get());
		}
		lock.unlock();
		_onReadyToPlay();
	}

	void __onCompleted()
	{
	}

};

void _MediaPlayer_onCompleted(JNIEnv* env, jobject _this, jlong instance)
{
	_AndroidMediaPlayerMap* map = _AndroidMediaPlayerMap_get();
	if (map) {
		WeakRef<_Android_MediaPlayer> _player;
		map->get(instance, &_player);
		Ref<_Android_MediaPlayer> player = _player;
		if (player.isNotNull()) {
			player->__onCompleted();
		}
	}
}

void _MediaPlayer_onPrepared(JNIEnv* env, jobject _this, jlong instance)
{
	_AndroidMediaPlayerMap* map = _AndroidMediaPlayerMap_get();
	if (map) {
		WeakRef<_Android_MediaPlayer> _player;
		map->get(instance, &_player);
		Ref<_Android_MediaPlayer> player = _player;
		if (player.isNotNull()) {
			player->__onPrepared();
		}
	}
}

Ref<MediaPlayer> MediaPlayer::_createNative(const MediaPlayerParam& param)
{
	return _Android_MediaPlayer::create(param);
}

SLIB_MEDIA_NAMESPACE_END

#endif
