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
