#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/media/audio_player.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class _Android_AudioPlayerControl;

typedef HashMap<jlong, WeakRef<_Android_AudioPlayerControl> > _AndroidAudioPlayerControlMap;
SLIB_SAFE_STATIC_GETTER(_AndroidAudioPlayerControlMap, _AndroidAudioPlayerControls_get)

void _AudioPlayer_onCompleted(JNIEnv* env, jobject _this, jlong instance);
void _AudioPlayer_onPrepared(JNIEnv* env, jobject _this, jlong instance);

SLIB_JNI_BEGIN_CLASS(_JAndroidMedia, "slib/platform/android/media/AudioPlayer")
	SLIB_JNI_STATIC_METHOD(createMedia, "createMedia", "(Lslib/platform/android/SlibActivity;Ljava/lang/String;J)Landroid/media/MediaPlayer;");
	SLIB_JNI_STATIC_METHOD(start, "start", "(Lslib/platform/android/SlibActivity;Landroid/media/MediaPlayer;)V");
	SLIB_JNI_STATIC_METHOD(pause, "pause", "(Lslib/platform/android/SlibActivity;Landroid/media/MediaPlayer;)V");
	SLIB_JNI_STATIC_METHOD(stop, "stop", "(Lslib/platform/android/SlibActivity;Landroid/media/MediaPlayer;)V");
	SLIB_JNI_STATIC_METHOD(isPlaying, "isPlaying", "(Lslib/platform/android/SlibActivity;Landroid/media/MediaPlayer;)Z");
	SLIB_JNI_NATIVE(onCompleted, "nativeOnCompleted", "(J)V", _AudioPlayer_onCompleted);
	SLIB_JNI_NATIVE(onPrepared, "nativeOnPrepared", "(J)V", _AudioPlayer_onPrepared);
SLIB_JNI_END_CLASS

class _Android_AudioPlayerControl : public AudioPlayerControl
{
public:
	_Android_AudioPlayerControl()
	{
		isPrepared = sl_false;
		isStartedBeforePrepared = sl_false;
	}

	void init(const String& url)
	{
		JniLocal<jstring> _url = Jni::getJniString(url);
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			m_player = _JAndroidMedia::createMedia.callObject(sl_null, jactivity, _url.get(), (jlong)this);
			_AndroidAudioPlayerControlMap* map = _AndroidAudioPlayerControls_get();
			if (map) {
				jlong instance = (jlong) this;
				map->put(instance, this);
			}
		}
	}

	void init(const Memory& data)
	{
	}

	~_Android_AudioPlayerControl()
	{
		_AndroidAudioPlayerControlMap* map = _AndroidAudioPlayerControls_get();
		if (map) {
			map->remove((jlong) this);
		}
	}

public:
	void start()
	{
		if (!isPrepared) {
			isStartedBeforePrepared = sl_true;
		} else {
			JniGlobal<jobject> player = m_player;
			jobject jactivity = Android::getCurrentActivity();
			if (jactivity) {
				if (player.isNotNull()) {
					ObjectLocker lock(this);
					_JAndroidMedia::start.call(sl_null, jactivity, player.get());
				}
			}
		}
	}

	void pause()
	{
		ObjectLocker lock(this);
		JniGlobal<jobject> player = m_player;
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			if (player.isNotNull()) {
				ObjectLocker lock(this);
				_JAndroidMedia::pause.call(sl_null, jactivity, player.get());
			}
		}
	}

	void stop()
	{
		ObjectLocker lock(this);
		JniGlobal<jobject> player = m_player;
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			if (player.isNotNull()) {
				ObjectLocker lock(this);
				_JAndroidMedia::stop.call(sl_null, jactivity, player.get());
				_removeFromMap();
			}
		}
	}

	sl_bool isRunning()
	{
		ObjectLocker lock(this);
		JniGlobal<jobject> player = m_player;
		if (player.isNotNull()) {
			ObjectLocker lock(this);
			return _JAndroidMedia::isPlaying.callBoolean(sl_null, player.get());
		}
		return sl_false;
	}

	void onPrepared()
	{
		isPrepared = sl_true;
		if (isStartedBeforePrepared) {
			start();
		}
	}

	void onCompleted()
	{
		stop();
	}

private:
	JniSafeGlobal<jobject> m_player;
	sl_bool isPrepared, isStartedBeforePrepared;
};

class _Android_AudioPlayer : public AudioPlayer {
public:
	_Android_AudioPlayer()
	{
	}

	~_Android_AudioPlayer()
	{
	}

public:
	static void logError(String text)
	{

	}

	static Ref<_Android_AudioPlayer> create(const AudioPlayerParam& param)
	{
		Ref<_Android_AudioPlayer> ret = new _Android_AudioPlayer();
		return ret;
	}

	Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param)
	{
		return sl_null;
	}

	Ref<AudioPlayerControl> _openNative(const AudioPlayerOpenParam& param);
};

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return _Android_AudioPlayer::create(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	AudioPlayerInfo ret;
	SLIB_STATIC_STRING(s, "Internal Speaker");
	ret.name = s;
	return List<AudioPlayerInfo>::createFromElement(ret);
}

Ref<AudioPlayerControl> _Android_AudioPlayer::_openNative(const AudioPlayerOpenParam &param)
{
	Ref<_Android_AudioPlayerControl> ret = new _Android_AudioPlayerControl;
	if (ret.isNotNull()) {
		if (param.data.isNotNull() && param.data.getSize() > 0) {
			ret->init(param.data);
		} else if (param.url.isNotNull() && param.url.getLength() > 0) {
			ret->init(param.url);
		}
		if (param.flagAutoStart) {
			ret->start();
		}
		return ret;
	}
	return sl_null;
}

void _AudioPlayer_onCompleted(JNIEnv* env, jobject _this, jlong instance)
{
	_AndroidAudioPlayerControlMap* map = _AndroidAudioPlayerControls_get();
	if (map) {
		WeakRef<_Android_AudioPlayerControl> control;
		map->get(instance, &control);
		Ref<_Android_AudioPlayerControl> _control = control;
		if (_control.isNotNull()) {
			_control->onCompleted();
		}
	}
}


void _AudioPlayer_onPrepared(JNIEnv* env, jobject _this, jlong instance)
{
	_AndroidAudioPlayerControlMap* map = _AndroidAudioPlayerControls_get();
	if (map) {
		WeakRef<_Android_AudioPlayerControl> control;
		map->get(instance, &control);
		Ref<_Android_AudioPlayerControl> _control = control;
		if (_control.isNotNull()) {
			_control->onPrepared();
		}
	}
}

SLIB_MEDIA_NAMESPACE_END

#endif
