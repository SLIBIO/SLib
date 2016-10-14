#include "../../../inc/slib/core/animation.h"

#include "../../../inc/slib/core/thread.h"

SLIB_NAMESPACE_BEGIN

class _AnimationCenter
{
public:
	CList< Ref<Animation> > m_animations;
	Ref<Thread> m_thread;
	SpinLock m_lock;
	
	sl_bool m_flagUpdateList;
	sl_bool m_flagPaused;

public:
	_AnimationCenter()
	{
		m_flagUpdateList = sl_false;
		m_flagPaused = sl_false;
	}
	
	~_AnimationCenter()
	{
		if (m_thread.isNotNull()) {
			m_thread->finishAndWait();
		}
	}
	
	void addAnimation(Animation* animation)
	{
		if (!animation) {
			return;
		}
		if (m_animations.addIfNotExist(animation)) {
			m_flagUpdateList = sl_true;
			SpinLocker lock(&m_lock);
			if (m_thread.isNull()) {
				m_thread = Thread::start(SLIB_CALLBACK_CLASS(_AnimationCenter, run, this));
			} else {
				m_thread->wake();
			}
		}
	}
	
	void removeAnimation(Animation* animation)
	{
		if (!animation) {
			return;
		}
		ListLocker< Ref<Animation> > list(m_animations);
		sl_size n = list.count;
		for (sl_size i = 0; i < n;) {
			if (list[i].ptr == animation) {
				for (sl_size j = i + 1; j < n; j++) {
					list[j - 1] = list[j];
				}
				n--;
			} else {
				i++;
			}
		}
		if (n < list.count) {
			m_animations.setCount(n);
		}
		m_flagUpdateList = sl_true;
		if (m_thread.isNotNull()) {
			m_thread->wake();
		}
	}
	
	void run()
	{
		TimeCounter timer;
		
		List< Ref<Animation> > animations;
		
		while (Thread::isNotStoppingCurrent()) {
			if (m_flagPaused) {
				Thread::sleep(100000);
			} else {
				if (m_flagUpdateList) {
					m_flagUpdateList = sl_false;
					animations = m_animations.duplicate();
				}
				
				ListItems< Ref<Animation> > list(animations);
				if (list.count > 0) {
					Time now = Time::now();
					for (sl_size i = 0; i < list.count; i++) {
						Ref<Animation> animation = list[i];
						if (animation.isNotNull()) {
							if (animation->isStopped()) {
								removeAnimation(animation.ptr);
							} else {
								animation->update(now);
							}
						}
					}
				}
				if (Thread::isNotStoppingCurrent()) {
					if (list.count > 0) {
						sl_uint64 t = timer.getEllapsedMilliseconds();
						if (t < 10) {
							Thread::sleep(10 - (sl_uint32)(t));
						}
					} else {
						Thread::sleep(100000);
					}
					timer.reset();
				} else {
					break;
				}
			}
		}
	}
	
	void pause()
	{
		m_flagPaused = sl_true;
	}
	
	void resume()
	{
		m_flagPaused = sl_false;
		if (m_thread.isNotNull()) {
			m_thread->wake();
		}
	}
	
};

SLIB_SAFE_STATIC_GETTER(_AnimationCenter, _AnimationCenter_get)

void IAnimationListener::onAnimationFrame(Animation* animation, float time)
{
}

void IAnimationListener::onEndAnimation(Animation* animation, sl_int32 nRemainingRepeatCount)
{
}

void IAnimationListener::onStopAnimation(Animation* animation)
{
}


SLIB_DEFINE_OBJECT(Animation, Object)

Animation::Animation()
{
	m_duration = 1;
	m_timeDuration.setSecondsCount(1);
	m_delay = 0;
	m_timeDelay.setZero();
	m_repeatCount = 0;
	m_flagAutoReverse = sl_false;
	
	m_curve = AnimationCurve::Linear;
	m_curveEaseFactor = 1.0f;
	m_curveEaseDoubleFactor = 2.0f;
	m_curveCycles = 1.0f;
	m_curveCycles2PI = SLIB_2_PI;
	m_curveTension = 2.0f;
	m_customCurve = sl_null;
	
	m_flagStarted = sl_false;
	m_lastRepeatedCount = 0;

}

Animation::~Animation()
{
	stop();
}

Ref<Animation> Animation::create(float duration)
{
	Ref<Animation> ret = new Animation;
	if (ret.isNotNull()) {
		ret->setDuration(duration);
		return ret;
	}
	return Ref<Animation>::null();
}

void Animation::pauseAnimationCenter()
{
	_AnimationCenter* center = _AnimationCenter_get();
	if (center) {
		center->pause();
	}
}

void Animation::resumeAnimationCenter()
{
	_AnimationCenter* center = _AnimationCenter_get();
	if (center) {
		center->resume();
	}
}

float Animation::getDuration()
{
	return m_duration;
}

void Animation::setDuration(float seconds)
{
	if (seconds < SLIB_ANIMATION_DURATION_MINIMUM_SECONDS) {
		seconds = SLIB_ANIMATION_DURATION_MINIMUM_SECONDS;
	}
	m_duration = seconds;
	m_timeDuration.setSecondsCountf(seconds);
}

float Animation::getStartDelay()
{
	return m_delay;
}

void Animation::setStartDelay(float seconds)
{
	m_delay = seconds;
	m_timeDelay.setSecondsCountf(seconds);
}

sl_int32 Animation::getRepeatCount()
{
	return m_repeatCount;
}

void Animation::setRepeatCount(sl_int32 count)
{
	m_repeatCount = count;
}

sl_bool Animation::isRepeatForever()
{
	return m_repeatCount < 0;
}

void Animation::setRepeatForever(sl_bool flagRepeatForever)
{
	if (flagRepeatForever) {
		m_repeatCount = -1;
	} else {
		if (m_repeatCount < 0) {
			m_repeatCount = 0;
		}
	}
}

sl_bool Animation::isAutoReverse()
{
	return m_flagAutoReverse;
}

void Animation::setAutoReverse(sl_bool flagReverse)
{
	m_flagAutoReverse = flagReverse;
}

AnimationCurve Animation::getAnimationCurve()
{
	return m_curve;
}

void Animation::setAnimationCurve(AnimationCurve curve)
{
	m_curve = curve;
}

float Animation::getAnimationCurveEaseFactor()
{
	return m_curveEaseFactor;
}

void Animation::setAnimationCurveEaseFactor(float factor)
{
	m_curveEaseFactor = factor;
	m_curveEaseDoubleFactor = factor * 2;
}

float Animation::getAnimationCurveCycles()
{
	return m_curveCycles;
}

void Animation::setAnimationCurveCycles(float cycles)
{
	m_curveCycles = cycles;
	m_curveCycles2PI = cycles * SLIB_2_PI;
}

float Animation::getAnimationCurveTension()
{
	return m_curveTension;
}

void Animation::setAnimationCurveTension(float tension)
{
	m_curveTension = tension;
}

CustomAnimationCurve Animation::getCustomAnimationCurve()
{
	return m_customCurve;
}

Ref<Referable> Animation::getCustomAnimationCurveParam()
{
	return m_customCurveParam;
}

void Animation::setCustomAnimationCurve(CustomAnimationCurve curve)
{
	setCustomAnimationCurve(curve, Ref<Referable>::null());
}

void Animation::setCustomAnimationCurve(CustomAnimationCurve curve, const Ref<Referable>& param)
{
	m_customCurve = curve;
	m_customCurveParam = param;
}

void Animation::addTarget(const Ref<AnimationTarget>& target)
{
	if (target.isNotNull()) {
		m_targets.addIfNotExist(target);
		target->setAnimation(this);
	}
}

void Animation::removeTarget(const Ref<AnimationTarget>& target)
{
	if (target.isNotNull()) {
		m_targets.removeValue(target);
		if (m_targets.getCount() == 0) {
			stop();
		}
	}
}

void Animation::removeAllTargets()
{
	m_targets.removeAll();
}

void Animation::linkAnimation(const Ref<Animation>& animation)
{
	if (animation.isNotNull()) {
		m_linkedAnimations.add(animation);
	}
}

void Animation::unlinkAnimation(const Ref<Animation>& animation)
{
	if (animation.isNotNull()) {
		m_linkedAnimations.removeValue(animation);
	}
}

void Animation::unlinkAllAnimations()
{
	m_linkedAnimations.removeAll();
}

void Animation::start()
{
	startAndSetTime(0, Time::now());
}

void Animation::start(const Time& current)
{
	startAndSetTime(0, current);
}

void Animation::startAndSetTime(float initialTime)
{
	startAndSetTime(initialTime, Time::now());
}

void Animation::startAndSetTime(float initialTime, const Time& current)
{
	ObjectLocker lock(this);
	if (m_timer.isStarted()) {
		return;
	}
	m_flagStarted = sl_true;
	
	if (initialTime < SLIB_EPSILON) {
		dispatchAnimationFrame(0);
	}

	Time t;
	t.setSecondsCountf(initialTime);
	m_timer.startAndSetTime(t, current);
	_AnimationCenter* center = _AnimationCenter_get();
	if (center) {
		center->addAnimation(this);
	}
}

void Animation::restart()
{
	restartAndSetTime(0, Time::now());
}

void Animation::restart(const Time& current)
{
	restartAndSetTime(0, current);
}

void Animation::restartAndSetTime(float initialTime)
{
	restartAndSetTime(initialTime, Time::now());
}

void Animation::restartAndSetTime(float initialTime, const Time& current)
{
	ObjectLocker lock(this);
	if (m_timer.isStarted()) {
		Time t;
		t.setSecondsCountf(initialTime);
		m_timer.restartAndSetTime(t, current);
	} else {
		startAndSetTime(initialTime, current);
	}
}

void Animation::stop()
{
	ObjectLocker lock(this);
	sl_bool flagStarted = m_timer.isStarted();
	m_timer.stop();
	if (flagStarted) {
		dispatchStopAnimation();
		_AnimationCenter* center = _AnimationCenter_get();
		if (center) {
			center->removeAnimation(this);
		}
	}
}

void Animation::resume()
{
	resume(Time::now());
}

void Animation::resume(const Time& current)
{
	m_timer.resume(current);
}

void Animation::pause()
{
	pause(Time::now());
}

void Animation::pause(const Time& current)
{
	m_timer.pause(current);
}

float Animation::getCurrentTime(sl_uint32* outRepeatedCount)
{
	return getCurrentTime(Time::now(), outRepeatedCount);
}

float Animation::getCurrentTime(const Time& current, sl_uint32* outRepeatedCount)
{
	ObjectLocker lock(this);
	sl_uint32 iRepeat;
	sl_bool flagStop;
	float time = _getTime(current, iRepeat, flagStop);
	if (outRepeatedCount) {
		*outRepeatedCount = iRepeat;
	}
	return time;
}

float Animation::getEllapsedSeconds()
{
	return getEllapsedSeconds(Time::now());
}

float Animation::getEllapsedSeconds(const Time& current)
{
	return (float)(m_timer.getTime(current).getSecondsCountf());
}

void Animation::setEllapsedSeconds(float time)
{
	setEllapsedSeconds(time, Time::now());
}

void Animation::setEllapsedSeconds(float time, const Time& current)
{
	Time t;
	t.setSecondsCountf(time);
	m_timer.setTime(t, current);
}

float Animation::getTimeFraction()
{
	return getTimeFraction(Time::now());
}

float Animation::getTimeFraction(const Time& current)
{
	float time = getCurrentTime(current);
	return _getFraction(time);
}

sl_uint32 Animation::getTotalRepeatedCount()
{
	return m_lastRepeatedCount;
}

sl_bool Animation::isStarted()
{
	return m_timer.isStarted();
}

sl_bool Animation::isStopped()
{
	return m_timer.isStopped();
}

sl_bool Animation::isRunning()
{
	return m_timer.isRunning();
}

sl_bool Animation::isNotRunning()
{
	return m_timer.isNotRunning();
}

sl_bool Animation::isPaused()
{
	return m_timer.isPaused();
}

void Animation::update()
{
	update(Time::now());
}

void Animation::update(const Time& current)
{
	ObjectLocker lock(this);
	if (!(m_timer.isStarted())) {
		return;
	}
	
	sl_int32 repeat = m_repeatCount;
	sl_uint32 lastRepeat = m_lastRepeatedCount;
	m_lastRepeatedCount = 0;

	sl_uint32 iRepeat;
	sl_bool flagStop;
	float time = _getTime(current, iRepeat, flagStop);
	m_lastRepeatedCount = iRepeat;
	
	dispatchAnimationFrame(time);
	
	if (flagStop) {
		dispatchEndAnimation(0);
		stop();
	} else {
		if (repeat != 0 && lastRepeat < iRepeat) {
			if (repeat < 0) {
				dispatchEndAnimation(-1);
			} else {
				sl_int32 remainRepeat = repeat - iRepeat;
				if (remainRepeat < 0) {
					remainRepeat = 0;
				}
				dispatchEndAnimation(remainRepeat);
			}
		}
	}
}

void Animation::onAnimationFrame(float time)
{
}

void Animation::onEndAnimation(sl_int32 nRemainingRepeatCount)
{
}

void Animation::onStopAnimation()
{
}

void Animation::dispatchAnimationFrame(float time)
{
	onAnimationFrame(time);
	PtrLocker<IAnimationListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onAnimationFrame(this, time);
	}
	float fraction = _getFraction(time);
	ListLocker< Ref<AnimationTarget> > targets(m_targets);
	for (sl_size i = 0; i < targets.count; i++) {
		Ref<AnimationTarget>& target = targets[i];
		if (target.isNotNull()) {
			target->update(fraction);
		}
	}
}

void Animation::dispatchEndAnimation(sl_int32 nRemainingRepeatCount)
{
	onEndAnimation(nRemainingRepeatCount);
	PtrLocker<IAnimationListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onEndAnimation(this, nRemainingRepeatCount);
	}
}

void Animation::dispatchStopAnimation()
{
	onStopAnimation();
	PtrLocker<IAnimationListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onStopAnimation(this);
	}
	Time now = Time::now();
	ListLocker< Ref<Animation> > linkedAnimations(m_linkedAnimations);
	for (sl_size i = 0; i < linkedAnimations.count; i++) {
		Ref<Animation>& animation = linkedAnimations[i];
		if (animation.isNotNull()) {
			animation->start(now);
		}
	}
}

float Animation::_getTime(const Time& current, sl_uint32& iRepeat, sl_bool& flagStop)
{
	iRepeat = 0;
	flagStop = sl_false;
	
	if (m_flagStarted) {
		sl_int64 duration = m_timeDuration.toInt();
		if (duration < 1) {
			return 0;
		}
		sl_int32 repeat = m_repeatCount;
		if (m_timer.isStopped()) {
			iRepeat = m_lastRepeatedCount;
			if (repeat > 0 && m_flagAutoReverse && (repeat & 1)) {
				return 0;
			} else {
				return m_duration;
			}
		}
		
		sl_int64 delay = m_timeDelay.toInt();
		sl_int64 t = m_timer.getTime(current).toInt() - delay;
		if (t <= 0) {
			return 0;
		}
		if (t >= duration) {
			if (repeat == 0) {
				t = duration;
				flagStop = sl_true;
			} else {
				sl_uint64 ut = t;
				sl_uint64 uduration = duration;
				iRepeat = (sl_uint32)(ut / uduration);
				if (repeat < 0) {
					t = ut % uduration;
				} else {
					if (iRepeat > (sl_uint32)repeat) {
						t = duration;
						iRepeat = repeat;
						flagStop = sl_true;
					} else {
						t = ut % uduration;
					}
				}
				if (m_flagAutoReverse && (iRepeat & 1)) {
					t = duration - t;
				}
			}
		}
		return (float)(Time(t).getSecondsCountf());
	}
	return 0;
}

float Animation::_getFraction(float time)
{
	float duration = m_duration;
	if (duration < SLIB_EPSILON) {
		return 0;
	}
	time = time / duration;
	time = _applyCurve(time);
	return time;
}

SLIB_INLINE static float _Animation_bounce(float f)
{
	return f * f * 8.0f;
}

float Animation::_applyCurve(float f)
{
	switch (m_curve) {
		case AnimationCurve::Linear:
			return f;
		case AnimationCurve::EaseInOut:
			return (Math::cos((f + 1) * SLIB_PI) / 2) + 0.5f;
		case AnimationCurve::EaseIn:
			{
				float factor = m_curveEaseDoubleFactor;
				if (factor == 2.0f) {
					return f * f;
				} else {
					return Math::pow(f, factor);
				}
			}
		case AnimationCurve::EaseOut:
			{
				float factor = m_curveEaseDoubleFactor;
				f = 1 - f;
				if (factor == 2.0f) {
					return 1 - f * f;
				} else {
					return 1 - Math::pow(f, factor);
				}
			}
		case AnimationCurve::Cycle:
			{
				float cycle2PI = m_curveCycles2PI;
				return (1 - Math::cos(cycle2PI * f)) / 2;
			}
		case AnimationCurve::Bounce:
			{
				f *= 1.1226f;
				if (f < 0.3535f) {
					return _Animation_bounce(f);
				} else if (f < 0.7408f) {
					return _Animation_bounce(f - 0.54719f) + 0.7f;
				} else if (f < 0.9644f) {
					return _Animation_bounce(f - 0.8526f) + 0.9f;
				} else {
					return _Animation_bounce(f - 1.0435f) + 0.95f;
				}
			}
		case AnimationCurve::Anticipate:
			{
				float tension = m_curveTension;
				return f * f * ((tension + 1) * f - tension);
			}
		case AnimationCurve::Overshoot:
			{
				float tension = m_curveTension;
				f -= 1;
				return 1 + f * f * ((tension + 1) * f + tension);
			}
		case AnimationCurve::Custom:
			{
				CustomAnimationCurve func = m_customCurve;
				Ref<Referable> param = m_customCurveParam;
				if (func) {
					return func(f, param.ptr);
				}
				return f;
			}
	}
	return f;
}


SLIB_DEFINE_OBJECT(AnimationTarget, Object)

AnimationTarget::AnimationTarget()
{
}

Ref<Animation> AnimationTarget::getAnimation()
{
	return m_animation;
}

void AnimationTarget::setAnimation(const Ref<Animation>& animation)
{
	m_animation = animation;
}

void AnimationTarget::forceUpdate()
{
	forceUpdate(Time::now());
}

void AnimationTarget::forceUpdate(const Time& current)
{
	Ref<Animation> animation(m_animation);
	if (animation.isNotNull()) {
		float fraction = animation->getTimeFraction(current);
		update(fraction);
	}
}

SLIB_NAMESPACE_END
