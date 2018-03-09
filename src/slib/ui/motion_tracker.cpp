/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/motion_tracker.h"

#include "slib/ui/event.h"
#include "slib/core/math.h"

#define MOVEMENT_ZERO_MOVEMENT 5
	
#define MAX_TRACK_HORIZON 0.2f
#define MOVEMENT_STOP_MILLIS 500
	
	
#define MAX_DEGREE 4
#define HISTORY_SIZE SLIB_MOTION_TRACKER_HISTORY_SIZE
	
namespace slib
{

	MotionTracker::MotionTracker() : MotionTracker(1)
	{
	}
	
	MotionTracker::MotionTracker(sl_uint32 degree)
	{
		if (degree < 1) {
			degree = 1;
		}
		if (degree > MAX_DEGREE) {
			degree = MAX_DEGREE;
		}
		m_degree = degree;
		m_lastTime.setZero();
		m_topHistory = 0;
		m_nHistory = 0;
		m_flagRefreshTrack = sl_true;
		m_flagValidTrack = sl_false;
		m_currentConfidence = 0;
	}
	
	MotionTracker::~MotionTracker()
	{
	}
	
	void MotionTracker::addMovement(sl_real x, sl_real y, const Time& time)
	{
		if (m_lastTime.isNotZero() && (time - m_lastTime).getMillisecondsCount() > MOVEMENT_STOP_MILLIS) {
			clearMovements();
		}
		sl_uint32 top = m_topHistory;
		m_history[top].x = x;
		m_history[top].y = y;
		m_history[top].time = time;
		top = (top + 1) % HISTORY_SIZE;
		m_topHistory = top;
		sl_uint32 nHistory = m_nHistory;
		if (nHistory < HISTORY_SIZE) {
			nHistory++;
			m_nHistory = nHistory;
		}
		m_lastTime = time;
		m_flagRefreshTrack = sl_true;
	}
	
	void MotionTracker::addMovement(sl_real x, sl_real y)
	{
		addMovement(x, y, Time::now());
	}
	
	void MotionTracker::addMovement(const Point& pt, const Time& time)
	{
		addMovement(pt.x, pt.y, time);
	}
	
	void MotionTracker::addMovement(const Point& pt)
	{
		addMovement(pt.x, pt.y, Time::now());
	}
	
	void MotionTracker::addMovement(UIEvent* ev)
	{
		addMovement(ev->getX(), ev->getY(), ev->getTime());
	}
	
	sl_bool MotionTracker::getLastPosition(Point* _out)
	{
		if (m_nHistory > 0) {
			if (_out) {
				if (m_topHistory == 0) {
					_out->x = m_history[HISTORY_SIZE - 1].x;
					_out->y = m_history[HISTORY_SIZE - 1].y;
				} else {
					_out->x = m_history[m_topHistory - 1].x;
					_out->y = m_history[m_topHistory - 1].y;
				}
			}
			return sl_true;
		}
		return sl_false;
	}
	
	void MotionTracker::clearMovements()
	{
		m_nHistory = 0;
		m_flagRefreshTrack = sl_true;
	}
	
	sl_bool MotionTracker::getVelocity(sl_real* outX, sl_real* outY)
	{
		_computeVelocity();
		if (m_flagRefreshTrack) {
			return sl_false;
		}
		if (m_flagValidTrack) {
			if (outX) {
				*outX = m_currentVelocity.x;
			}
			if (outY) {
				*outY = m_currentVelocity.y;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool MotionTracker::getVelocity(Point* _out)
	{
		_computeVelocity();
		if (m_flagRefreshTrack) {
			return sl_false;
		}
		if (m_flagValidTrack) {
			if (_out) {
				*_out = m_currentVelocity;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	SLIB_INLINE static sl_real _priv_MotionTracker_getVectorDotProduct(const sl_real* v1, const sl_real* v2, sl_uint32 N)
	{
		sl_real dot = 0;
		for (sl_uint32 i = 0; i < N; i++) {
			dot += v1[i] * v2[i];
		}
		return dot;
	}
	
	SLIB_INLINE static sl_real _priv_MotionTracker_getVectorLength(const sl_real* v, sl_uint32 N)
	{
		sl_real len = 0;
		for (sl_uint32 i = 0; i < N; i++) {
			len += v[i] * v[i];
		}
		return Math::sqrt(len);
	}
	
	static sl_bool _priv_MotionTracker_solveLeastSquares(const sl_real* x, const sl_real* y, const sl_real* w, sl_uint32 m, sl_uint32 n, sl_real* outB, sl_real* outDet) {
		// Expand the X vector to a matrix A, pre-multiplied by the weights.
		sl_real a[MAX_DEGREE + 1][HISTORY_SIZE]; // column-major order
		for (sl_uint32 h = 0; h < m; h++) {
			a[0][h] = w[h];
			for (sl_uint32 i = 1; i < n; i++) {
				a[i][h] = a[i - 1][h] * x[h];
			}
		}
		// Apply the Gram-Schmidt process to A to obtain its QR decomposition.
		sl_real q[MAX_DEGREE + 1][HISTORY_SIZE]; // orthonormal basis, column-major order
		sl_real r[MAX_DEGREE + 1][MAX_DEGREE + 1]; // upper triangular matrix, row-major order
		for (sl_uint32 j = 0; j < n; j++) {
			for (sl_uint32 h = 0; h < m; h++) {
				q[j][h] = a[j][h];
			}
			for (sl_uint32 i = 0; i < j; i++) {
				sl_real dot = _priv_MotionTracker_getVectorDotProduct(&q[j][0], &q[i][0], m);
				for (sl_uint32 h = 0; h < m; h++) {
					q[j][h] -= dot * q[i][h];
				}
			}
			//sl_real f = q[j][0];
			sl_real norm = _priv_MotionTracker_getVectorLength(&q[j][0], m);
			if (norm < 0.000001f) {
				// vectors are linearly dependent or zero so no solution
				return sl_false;
			}
			sl_real invNorm = 1.0f / norm;
			for (sl_uint32 h = 0; h < m; h++) {
				q[j][h] *= invNorm;
			}
			for (sl_uint32 i = 0; i < n; i++) {
				r[j][i] = i < j ? 0 : _priv_MotionTracker_getVectorDotProduct(&q[j][0], &a[i][0], m);
			}
		}
		
		// Solve R B = Qt W Y to find B.  This is easy because R is upper triangular.
		// We just work from bottom-right to top-left calculating B's coefficients.
		sl_real wy[HISTORY_SIZE];
		for (sl_uint32 h = 0; h < m; h++) {
			wy[h] = y[h] * w[h];
		}
		for (sl_uint32 i = n; i != 0; ) {
			i--;
			outB[i] = _priv_MotionTracker_getVectorDotProduct(&q[i][0], wy, m);
			for (sl_uint32 j = n - 1; j > i; j--) {
				outB[i] -= r[i][j] * outB[j];
			}
			outB[i] /= r[i][i];
		}
		
		// Calculate the coefficient of determination as 1 - (SSerr / SStot) where
		// SSerr is the residual sum of squares (variance of the error),
		// and SStot is the total sum of squares (variance of the data) where each
		// has been weighted.
		sl_real ymean = 0;
		for (sl_uint32 h = 0; h < m; h++) {
			ymean += y[h];
		}
		ymean /= m;
		sl_real sserr = 0;
		sl_real sstot = 0;
		for (sl_uint32 h = 0; h < m; h++) {
			sl_real err = y[h] - outB[0];
			sl_real term = 1;
			for (sl_uint32 i = 1; i < n; i++) {
				term *= x[h];
				err -= term * outB[i];
			}
			sserr += w[h] * w[h] * err * err;
			sl_real var = y[h] - ymean;
			sstot += w[h] * w[h] * var * var;
		}
		*outDet = sstot > 0.000001f ? 1.0f - (sserr / sstot) : 1;
		
		return sl_true;
	}
	
	void MotionTracker::_computeVelocity()
	{
		if (!m_flagRefreshTrack) {
			return;
		}
		
		sl_uint32 nHistory = m_nHistory;
		if (nHistory < 1) {
			return;
		}
		
		Time lastTime = m_lastTime;
		
		sl_real xCoeff[MAX_DEGREE + 1], yCoeff[MAX_DEGREE + 1];
		sl_real x[HISTORY_SIZE];
		sl_real y[HISTORY_SIZE];
		sl_real w[HISTORY_SIZE];
		sl_real time[HISTORY_SIZE];
		
		sl_uint32 index = m_topHistory;
		for (sl_uint32 i = 0; i < nHistory; i++) {
			if (index == 0) {
				index = HISTORY_SIZE - 1;
			} else {
				index--;
			}
			x[i] = m_history[index].x;
			y[i] = m_history[index].y;
			w[i] = 1; // coefficient weight (currently no-weight is applied)
			time[i] = (sl_real)((lastTime - m_history[index].time).getSecondsCountf());
			if (time[i] > MAX_TRACK_HORIZON) {
				nHistory = i;
				break;
			}
			time[i] = -time[i];
		}
		if (nHistory >= 2) {
			sl_uint32 degree = m_degree;
			if (degree > nHistory - 1) {
				degree = nHistory - 1;
			}
			if (degree >= 1) {
				sl_real xdet, ydet;
				if (_priv_MotionTracker_solveLeastSquares(time, x, w, nHistory, degree + 1, xCoeff, &xdet) && _priv_MotionTracker_solveLeastSquares(time, y, w, nHistory, degree + 1, yCoeff, &ydet)) {
					m_currentConfidence = xdet * ydet;
					m_currentVelocity.x = xCoeff[1];
					m_currentVelocity.y = yCoeff[1];
					m_flagValidTrack = sl_true;
					m_flagRefreshTrack = sl_false;
					return;
				}
			}
		}
		m_flagValidTrack = sl_false;
		m_flagRefreshTrack = sl_false;
		
	}

}
