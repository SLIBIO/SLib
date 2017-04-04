/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/animation.h"

#include "slib/core/thread.h"
#include "slib/core/time.h"
#include "slib/core/scoped.h"
#include "slib/core/safe_static.h"

namespace slib
{

	IAnimationListener::IAnimationListener()
	{
	}

	IAnimationListener::~IAnimationListener()
	{
	}

	void IAnimationListener::onAnimationFrame(Animation* animation, float time)
	{
	}

	void IAnimationListener::onRepeatAnimation(Animation* animation, sl_int32 nRemainingRepeatCount)
	{
	}

	void IAnimationListener::onStopAnimation(Animation* animation)
	{
	}

	SLIB_DEFINE_OBJECT(Animation, Object)

	Animation::Animation()
	{
		static sl_reg _uniqueId = 0;
		m_id = Base::interlockedIncrement(&_uniqueId);
		m_flagSelfAlive = sl_true;
		m_flagNativeEnabled = sl_true;
		m_flagUpdateWhenStart = sl_true;

		m_time = 0;
		m_duration = 1;
		m_delay = 0;
		m_repeatCount = 0;
		m_flagAutoReverse = sl_false;
		m_flagAbsoluteTime = sl_false;

		m_curve = AnimationCurve::Linear;
		m_curveEaseFactor = 1.0f;
		m_curveEaseDoubleFactor = 2.0f;
		m_curveCycles = 1.0f;
		m_curveCycles2PI = SLIB_PI_DUAL;
		m_curveTension = 2.0f;

		m_flagStarted = sl_false;
		m_flagRunning = sl_false;
		m_lastRepeatedCount = 0;

		m_flagStartedNative = sl_false;
	}

	Animation::~Animation()
	{
		SLIB_REFERABLE_DESTRUCTOR
		stop();
	}
	
	Ref<Animation> Animation::create(float duration)
	{
		return createWithLoop(AnimationLoop::getDefault(), duration);
	}
	
	Ref<Animation> Animation::create(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createWithLoop(AnimationLoop::getDefault(), target, duration, onStop, curve, flags);
	}
	
	Ref<Animation> Animation::start(const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return createWithLoop(AnimationLoop::getDefault(), target, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}

	Ref<Animation> Animation::createWithLoop(const Ref<AnimationLoop>& loop, float duration)
	{
		Ref<Animation> ret = new Animation;
		if (ret.isNotNull()) {
			ret->setDuration(duration);
			ret->m_loop = loop;
			return ret;
		}
		return sl_null;
	}

	Ref<Animation> Animation::createWithLoop(const Ref<AnimationLoop>& loop, const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		Ref<Animation> ret = new Animation;
		if (ret.isNotNull()) {
			ret->addTarget(target);
			ret->setDuration(duration);
			ret->setAnimationCurve(curve);
			ret->m_loop = loop;
			ret->setOnStop(onStop);
			if (flags & AnimationFlags::Repeat) {
				ret->setRepeatForever(sl_true);
			}
			if (flags & AnimationFlags::AutoReverse) {
				ret->setAutoReverse(sl_true);
			}
			if (flags & AnimationFlags::NotNative) {
				ret->setNativeEnabled(sl_false);
			}
			if (flags & AnimationFlags::NotUpdateWhenStart) {
				ret->setUpdateWhenStart(sl_false);
			}
			if (flags & AnimationFlags::NotSelfAlive) {
				ret->setSelfAlive(sl_false);
			}
			if (flags & AnimationFlags::AutoStart) {
				ret->start();
			}
			return ret;
		}
		return sl_null;
	}

	Ref<Animation> Animation::startWithLoop(const Ref<AnimationLoop>& loop, const Ref<AnimationTarget>& target, float duration, const Function<void()>& onStop, AnimationCurve curve, const AnimationFlags& flags)
	{
		return Animation::createWithLoop(loop, target, duration, onStop, curve, flags | AnimationFlags::AutoStart);
	}
	
	Ref<AnimationLoop> Animation::getLoop()
	{
		return m_loop;
	}

	CList< Ref<AnimationTarget> >& Animation::getTargets()
	{
		return m_targets;
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

	sl_reg Animation::getId()
	{
		return m_id;
	}

	sl_bool Animation::isSelfAlive()
	{
		return m_flagSelfAlive;
	}

	void Animation::setSelfAlive(sl_bool flagAlive)
	{
		m_flagSelfAlive = flagAlive;
	}

	sl_bool Animation::isNativeEnabled()
	{
		return m_flagNativeEnabled;
	}

	void Animation::setNativeEnabled(sl_bool flagNative)
	{
		m_flagNativeEnabled = flagNative;
	}

	sl_bool Animation::isUpdateWhenStart()
	{
		return m_flagUpdateWhenStart;
	}

	void Animation::setUpdateWhenStart(sl_bool flagUpdate)
	{
		m_flagUpdateWhenStart = flagUpdate;
	}

	float Animation::getTime()
	{
		return m_time;
	}

	void Animation::setTime(float seconds, sl_bool flagUpdateFrame)
	{
		m_time = seconds;
		if (flagUpdateFrame) {
			update(0);
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
	}

	float Animation::getStartDelay()
	{
		return m_delay;
	}

	void Animation::setStartDelay(float seconds)
	{
		m_delay = seconds;
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

	sl_bool Animation::isAbsoluteTime()
	{
		return m_flagAbsoluteTime;
	}

	void Animation::setAbsoluteTime(sl_bool flagAbsolute)
	{
		m_flagAbsoluteTime = flagAbsolute;
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
		m_curveCycles2PI = cycles * SLIB_PI_DUAL;
	}
	
	float Animation::getAnimationCurveTension()
	{
		return m_curveTension;
	}
	
	void Animation::setAnimationCurveTension(float tension)
	{
		m_curveTension = tension;
	}

	Function<float(float)> Animation::getCustomAnimationCurve()
	{
		return m_customAnimationCurve;
	}

	void Animation::setCustomAnimationCurve(const Function<float(float)>& curve)
	{
		m_customAnimationCurve = curve;
	}
	
	float Animation::getCurrentTime(sl_uint32* outRepeatedCount)
	{
		ObjectLocker lock(this);
		sl_uint32 iRepeat;
		sl_bool flagStop;
		float time = _getTime(iRepeat, flagStop);
		if (outRepeatedCount) {
			*outRepeatedCount = iRepeat;
		}
		return time;
	}
	
	float Animation::getTimeFraction()
	{
		float time = getCurrentTime();
		return _getFraction(time);
	}

	sl_uint32 Animation::getRepeatedCount()
	{
		return m_lastRepeatedCount;
	}
	
	
	void Animation::start()
	{
		startAt(0);
	}

	void Animation::startAt(float seconds)
	{
		ObjectLocker lock(this);

		if (m_flagStarted) {
			return;
		}
		if (m_flagStartedNative) {
			return;
		}

		m_flagStarted = sl_true;
		m_flagRunning = sl_false;

		if (m_flagNativeEnabled) {
			Ref<AnimationLoop> loop(m_loop);
			if (loop.isNotNull()) {
				if (loop->startNativeAnimation(this)) {
					m_flagStartedNative = sl_true;
					return;
				}
			}
		}

		m_time = seconds;

		_resume();

		lock.unlock();

		if (m_flagUpdateWhenStart) {
			update(0);
		}

	}

	void Animation::restart()
	{
		restartAt(0);
	}

	void Animation::restartAt(float seconds)
	{
		ObjectLocker lock(this);

		if (m_flagStartedNative) {
			return;
		}

		if (!m_flagStarted) {
			m_flagStarted = sl_true;
			m_flagRunning = sl_false;
		}

		if (m_flagNativeEnabled) {
			Ref<AnimationLoop> loop(m_loop);
			if (loop.isNotNull()) {
				if (loop->startNativeAnimation(this)) {
					m_flagStartedNative = sl_true;
					return;
				}
			}
		}

		m_time = seconds;

		_resume();

		lock.unlock();

		if (m_flagUpdateWhenStart) {
			update(0);
		}

	}

	void Animation::stop()
	{
		ObjectLocker lock(this);
		sl_bool flagStarted = _stop(sl_false);
		if (flagStarted) {
			lock.unlock();
			dispatchStopAnimation();
		}
	}

	void Animation::_stopFromNative()
	{
		ObjectLocker lock(this);
		sl_bool flagStarted = _stop(sl_true);
		if (flagStarted) {
			lock.unlock();
			dispatchStopAnimation();
		}
	}

	sl_bool Animation::_stop(sl_bool flagStopFromNative)
	{
		_pause();
		sl_bool flagStarted = m_flagStarted;
		if (flagStarted) {
			if (m_flagStartedNative) {
				if (!flagStopFromNative) {
					Ref<AnimationLoop> loop(m_loop);
					if (loop.isNotNull()) {
						loop->stopNativeAnimation(this);
					}
				}
			}
		}
		m_flagStartedNative = sl_false;
		m_flagStarted = sl_false;
		return flagStarted;
	}

	void Animation::resume()
	{
		ObjectLocker lock(this);
		_resume();
	}

	void Animation::_resume()
	{
		if (!m_flagStarted) {
			return;
		}
		if (m_flagStartedNative) {
			return;
		}
		if (m_flagRunning) {
			return;
		}
		Ref<AnimationLoop> loop(m_loop);
		if (loop.isNotNull()) {
			loop->addAnimation(this);
		}
		m_flagRunning = sl_true;
	}

	void Animation::pause()
	{
		ObjectLocker lock(this);
		_pause();
	}

	void Animation::_pause()
	{
		if (!m_flagStarted) {
			return;
		}
		if (m_flagStartedNative) {
			return;
		}
		if (!m_flagRunning) {
			return;
		}
		Ref<AnimationLoop> loop(m_loop);
		if (loop.isNotNull()) {
			loop->removeAnimation(this);
		}
		m_flagRunning = sl_false;
	}

	sl_bool Animation::isStarted()
	{
		return m_flagStarted;
	}

	sl_bool Animation::isStopped()
	{
		return !m_flagStarted;
	}

	sl_bool Animation::isRunning()
	{
		return m_flagStarted && m_flagRunning;
	}
	
	sl_bool Animation::isNotRunning()
	{
		return !(m_flagStarted && m_flagRunning);
	}
	
	sl_bool Animation::isPaused()
	{
		return m_flagStarted && !m_flagRunning;
	}
	
	void Animation::update(float elapsedSeconds)
	{
		ObjectLocker lock(this);

		if (!m_flagStarted) {
			return;
		}

		m_time += elapsedSeconds;

		sl_int32 repeat = m_repeatCount;
		sl_uint32 lastRepeat = m_lastRepeatedCount;
		m_lastRepeatedCount = 0;
	
		sl_uint32 iRepeat;
		sl_bool flagStop;
		float time = _getTime(iRepeat, flagStop);
		m_lastRepeatedCount = iRepeat;

		sl_bool flagStopped = sl_false;
		if (flagStop) {
			flagStopped = _stop(sl_false);
			lock.unlock();
		} else {
			lock.unlock();
			if (repeat != 0 && lastRepeat < iRepeat) {
				if (repeat < 0) {
					dispatchRepeatAnimation(-1);
				} else {
					sl_int32 remainRepeat = repeat - iRepeat;
					if (remainRepeat < 0) {
						remainRepeat = 0;
					}
					dispatchRepeatAnimation(remainRepeat);
				}
			}
		}

		dispatchAnimationFrame(time);
		if (flagStopped) {
			dispatchStopAnimation();
		}

	}
	
	void Animation::onAnimationFrame(float time)
	{
	}
	
	void Animation::onRepeatAnimation(sl_int32 nRemainingRepeatCount)
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
		ListLocker< Ref<AnimationTarget> > _targets(m_targets);
		sl_size n = _targets.count;
		SLIB_SCOPED_BUFFER(Ref<AnimationTarget>, 8, targets, n)
		sl_size i;
		for (i = 0; i < n; i++) {
			targets[i] = _targets[i];
		}
		_targets.unlock();
		for (i = 0; i < n; i++) {
			Ref<AnimationTarget>& target = targets[i];
			if (target.isNotNull()) {
				target->update(fraction);
			}
		}
	}

	void Animation::dispatchStartFrame()
	{
		dispatchAnimationFrame(0);
	}

	void Animation::dispatchEndFrame()
	{
		dispatchAnimationFrame(m_duration);
	}
	
	void Animation::dispatchRepeatAnimation(sl_int32 nRemainingRepeatCount)
	{
		onRepeatAnimation(nRemainingRepeatCount);
		PtrLocker<IAnimationListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onRepeatAnimation(this, nRemainingRepeatCount);
		}
	}
	
	void Animation::dispatchStopAnimation()
	{
		onStopAnimation();
		Function<void()> onStop(getOnStop());
		if (onStop.isNotNull()) {
			onStop();
		}
		PtrLocker<IAnimationListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onStopAnimation(this);
		}

		ListLocker< Ref<Animation> > linkedAnimations(m_linkedAnimations);
		for (sl_size i = 0; i < linkedAnimations.count; i++) {
			Ref<Animation>& animation = linkedAnimations[i];
			if (animation.isNotNull()) {
				animation->start();
			}
		}
	}

	float Animation::_getTime(sl_uint32& iRepeat, sl_bool& flagStop)
	{
		iRepeat = 0;
		flagStop = sl_false;
		
		if (m_flagStarted) {
			sl_int64 duration = (sl_int64)(m_duration * 1000);
			if (duration < 1) {
				return 0;
			}
			sl_int32 repeat = m_repeatCount;
			if (!m_flagStarted) {
				iRepeat = m_lastRepeatedCount;
				if (repeat > 0 && m_flagAutoReverse && (repeat & 1)) {
					return 0;
				} else {
					return m_duration;
				}
			}

			sl_int64 delay = (sl_int64)(m_delay * 1000);
			sl_int64 t = (sl_int64)(m_time * 1000) - delay;
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
			return (float)t / 1000.0f;
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

	constexpr static float _Animation_bounce(float f)
	{
		return f * f * 8.0f;
	}

	float Animation::_applyCurve(float f)
	{
		switch (m_curve) {
			case AnimationCurve::Default:
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
					Function<float(float)> func = m_customAnimationCurve;
					if (func.isNotNull()) {
						return func(f);
					}
					return f;
				}
		}
		return f;
	}


	AnimationLoop::AnimationLoop()
	{
		m_flagPaused = sl_false;
		m_flagUpdateList = sl_false;
		m_lastTime = 0;
	}

	AnimationLoop::~AnimationLoop()
	{
	}

	void AnimationLoop::addAnimation(Animation* animation)
	{
		if (!animation) {
			return;
		}
		ObjectLocker lock(this);
		if (animation->isSelfAlive()) {
			m_mapAnimations.put_NoLock(animation->getId(), animation);
		} else {
			m_mapWeakAnimations.put_NoLock(animation->getId(), animation);
		}
		m_flagUpdateList = sl_true;
		if (!m_flagPaused) {
			_wake();
		}
	}

	void AnimationLoop::removeAnimation(Animation* animation)
	{
		if (!animation) {
			return;
		}
		ObjectLocker lock(this);
		sl_reg _id = animation->getId();
		m_mapAnimations.remove_NoLock(_id);
		m_mapWeakAnimations.remove_NoLock(_id);
		m_flagUpdateList = sl_true;
		if (!m_flagPaused) {
			_wake();
		}
	}

	void AnimationLoop::pause()
	{
		ObjectLocker lock(this);
		if (m_flagPaused) {
			return;
		}
		m_flagPaused = sl_true;
	}

	void AnimationLoop::resume()
	{
		ObjectLocker lock(this);
		if (!m_flagPaused) {
			return;
		}
		m_flagPaused = sl_false;
		_wake();
	}

	sl_bool AnimationLoop::isPaused()
	{
		return m_flagPaused;
	}

	void AnimationLoop::wake()
	{
		ObjectLocker lock(this);
		if (!m_flagPaused) {
			_wake();
		}
	}

	sl_bool AnimationLoop::startNativeAnimation(Animation* animation)
	{
		return sl_false;
	}

	void AnimationLoop::stopNativeAnimation(Animation* animation)
	{
	}

	void AnimationLoop::_stopAnimationFromNative(Animation* animation)
	{
		animation->_stopFromNative();
	}

	Ref<Referable> AnimationLoop::_getNativeInstance(Animation* animation)
	{
		return animation->m_nativeInstance;
	}

	void AnimationLoop::_setNativeInstance(Animation* animation, Referable* instance)
	{
		animation->m_nativeInstance = instance;
	}

	sl_int32 AnimationLoop::_runStep()
	{
		if (m_flagUpdateList) {
			m_flagUpdateList = sl_false;
			ObjectLocker lock(this);
			List< Ref<Animation> > animations;
			{
				Iterator< Ref<Animation> > iterator = m_mapAnimations.getValueIterator();
				Ref<Animation> animation;
				while (iterator.next(&animation)) {
					if (animation.isNotNull()) {
						animations.add_NoLock(animation);
					}
				}
			}
			{
				Iterator< WeakRef<Animation> > iterator = m_mapWeakAnimations.getValueIterator();
				WeakRef<Animation> _animation;
				while (iterator.next(&_animation)) {
					Ref<Animation> animation(_animation);
					if (animation.isNotNull()) {
						animations.add_NoLock(animation);
					}
				}
			}
			m_animationsRunning = animations;
		}

		if (m_flagPaused || m_animationsRunning.isEmpty()) {
			m_lastTime = 0;
			return -1;
		}

		Time tCurrent = Time::now();
		sl_int64 timeCurrent = tCurrent.getMillisecondsCount();
		if (m_lastTime == 0) {
			m_lastTime = timeCurrent;
		}
		float elapsedAbsolute = (float)(timeCurrent - m_lastTime) / 1000.0f;
		if (elapsedAbsolute < 0) {
			elapsedAbsolute = 0;
		}
		float elapsed = elapsedAbsolute;
		if (elapsed > 0.5f) {
			elapsed = 0.5f;
		}

		ListElements< Ref<Animation> > list(m_animationsRunning);
		if (list.count > 0) {
			for (sl_size i = 0; i < list.count; i++) {
				Ref<Animation> animation = list[i];
				if (animation.isNotNull()) {
					if (animation->isStopped()) {
						removeAnimation(animation.get());
					} else {
						if (animation->isAbsoluteTime()) {
							animation->update(elapsedAbsolute);
						} else {
							animation->update(elapsed);
						}
					}
				}
			}
		}

		m_lastTime = timeCurrent;
		Time tEnd = Time::now();

		sl_int64 n = (tEnd - tCurrent).getMillisecondsCount();
		if (n < 15) {
			if (n < 0) {
				n = 0;
			}
			return (sl_int32)(15 - n);
		} else {
			return 0;
		}

	}
	
	
	class _DefaultAnimationLoop : public AnimationLoop
	{
	public:
		Ref<Thread> m_thread;

	public:
		_DefaultAnimationLoop()
		{
			SLIB_REFERABLE_CONSTRUCTOR

			if (m_thread.isNull()) {
				m_thread = Thread::start(SLIB_FUNCTION_CLASS(_DefaultAnimationLoop, run, this));
			} else {
				m_thread->wake();
			}
	
		}

		~_DefaultAnimationLoop()
		{
			if (m_thread.isNotNull()) {
				m_thread->finishAndWait();
			}
		}

	public:
		// override
		void _wake()
		{
			if (m_thread.isNotNull()) {
				m_thread->wake();
			}
		}

		void run()
		{
			while (Thread::isNotStoppingCurrent()) {
				sl_int32 n = _runStep();
				if (n < 0) {
					Thread::sleep(100000);
				} else {
					if (n > 0) {
						Thread::sleep(n);
					}
				}
			}
		}

	};
	
	SLIB_SAFE_STATIC_GETTER(Ref<AnimationLoop>, _AnimationLoop_getDefault, new _DefaultAnimationLoop)
	
	Ref<AnimationLoop> AnimationLoop::getDefault()
	{
		Ref<AnimationLoop>* pLoop = _AnimationLoop_getDefault();
		if (pLoop) {
			return *pLoop;
		}
		return sl_null;
	}
	
	
	SLIB_DEFINE_OBJECT(AnimationTarget, Object)

	AnimationTarget::AnimationTarget()
	{
	}
	
	AnimationTarget::~AnimationTarget()
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
		Ref<Animation> animation(m_animation);
		if (animation.isNotNull()) {
			float fraction = animation->getTimeFraction();
			update(fraction);
		}
	}

}
