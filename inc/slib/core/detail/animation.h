/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_DETAIL_ANIMATION
#define CHECKHEADER_SLIB_CORE_DETAIL_ANIMATION

#include "../animation.h"

namespace slib
{
/**************************************************
		AnimationFrames Implementations
**************************************************/

	template <class T, class INTERPOLATION>
	void AnimationFrames<T, INTERPOLATION>::addFrame(float fraction, const T& value)
	{
		frames.add(AnimationFrame<T>(fraction, value));
	}
	
	template <class T, class INTERPOLATION>
	T AnimationFrames<T, INTERPOLATION>::getValue(float fraction)
	{
		T* sv = &startValue;
		float st = 0;
		ListLocker< AnimationFrame<T> > f(frames);
		for (sl_size i = 0; i <= f.count; i++) {
			float et;
			if (i == f.count) {
				et = 1;
			} else {
				et = f[i].fraction;
			}
			if (et > st + SLIB_EPSILON) {
				T* ev;
				if (i == f.count) {
					ev = &endValue;
				} else {
					ev = &(f[i].value);
				}
				if (Math::isAlmostZero(fraction - et)) {
					return f[i].value;
				}
				if (fraction <= et) {
					float w = (fraction - st) / (et - st);
					return INTERPOLATION::interpolate(*sv, *ev, w);
				}
				sv = ev;
				st = et;
			}
		}
		return endValue;
	}


/**************************************************
	AnimationFramesSeeker Implementations
**************************************************/

	template <class T, class INTERPOLATION>
	AnimationFramesSeeker<T, INTERPOLATION>::AnimationFramesSeeker(const AnimationFrames<T, INTERPOLATION>& frames)
	{
		startValue = frames.startValue;
		endValue = frames.endValue;
		m_arrFrames = frames.frames.toArray();
		m_frames = m_arrFrames.getData();
		m_countFrames = m_arrFrames.getCount();
		m_currentIndex = 0;
		m_currentStartFraction = 0;
		if (m_countFrames > 0) {
			m_currentEndFraction = m_frames[0].fraction;
		} else {
			m_currentEndFraction = 1;
		}
		m_currentStartValue = &startValue;
		if (m_countFrames > 0) {
			m_currentEndValue = &(m_frames[0].value);
		} else {
			m_currentEndValue = &endValue;
		}
	}
	
	template <class T, class INTERPOLATION>
	sl_size AnimationFramesSeeker<T, INTERPOLATION>::getFramesCount()
	{
		return m_countFrames;
	}
	
	template <class T, class INTERPOLATION>
	AnimationFrame<T>& AnimationFramesSeeker<T, INTERPOLATION>::getFrame(sl_size index)
	{
		return m_frames[index];
	}
	
	template <class T, class INTERPOLATION>
	T AnimationFramesSeeker<T, INTERPOLATION>::seek(float fraction)
	{
		if (m_countFrames == 0) {
			return INTERPOLATION::interpolate(startValue, endValue, fraction);
		}
		if (Math::isAlmostZero(fraction - m_currentStartFraction)) {
			return *m_currentStartValue;
		}
		if (Math::isAlmostZero(fraction - m_currentEndFraction)) {
			return *m_currentEndValue;
		}
		ObjectLocker lock(this);
		sl_bool flagSeek;
		if (fraction < m_currentStartFraction) {
			m_currentIndex = 0;
			flagSeek = sl_true;
		} else if (fraction > m_currentEndFraction) {
			if (m_currentIndex >= m_countFrames) {
				return endValue;
			}
			m_currentIndex ++;
			flagSeek = sl_true;
		} else {
			flagSeek = sl_false;
		}
		if (flagSeek) {
			for (; m_currentIndex < m_countFrames; m_currentIndex++) {
				if (fraction < m_frames[m_currentIndex].fraction) {
					break;
				}
			}
			if (m_currentIndex > 0) {
				m_currentStartFraction = m_frames[m_currentIndex - 1].fraction;
				m_currentStartValue = &(m_frames[m_currentIndex - 1].value);
			} else {
				m_currentStartFraction = 0;
				m_currentStartValue = &(startValue);
			}
			if (m_currentIndex < m_countFrames) {
				m_currentEndFraction = m_frames[m_currentIndex].fraction;
				m_currentEndValue = &(m_frames[m_currentIndex].value);
			} else {
				m_currentEndFraction = 1;
				m_currentEndValue = &(endValue);
			}
		}
		float w = fraction - m_currentStartFraction;
		float t = m_currentEndFraction - m_currentStartFraction;
		if (t < SLIB_EPSILON) {
			return *m_currentEndValue;
		}
		w /= t;
		return INTERPOLATION::interpolate(*m_currentStartValue, *m_currentEndValue, w);
	}


/**************************************************
		AnimationTargetT Implementations
**************************************************/

	template <class T>
	T& AnimationTargetT<T>::getStartValue()
	{
		return m_seeker.startValue;
	}

	template <class T>
	T& AnimationTargetT<T>::getEndValue()
	{
		return m_seeker.endValue;
	}

	template <class T>
	sl_size AnimationTargetT<T>::getFramesCount()
	{
		return m_seeker.getFramesCount();
	}

	template <class T>
	T& AnimationTargetT<T>::getFrame(sl_size index)
	{
		return m_seeker.getFrame(index);
	}

	template <class T>
	void AnimationTargetT<T>::update(float fraction)
	{
		update(fraction, m_seeker.seek(fraction));
	}

}

#endif
