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

#ifndef CHECKHEADER_SLIB_UI_MOTION_TRACKER
#define CHECKHEADER_SLIB_UI_MOTION_TRACKER

/******************************************************************
 
Motion tracking algorithm based on least-squares linear regression.
 
 https://en.wikipedia.org/wiki/Least_squares
 https://en.wikipedia.org/wiki/Linear_regression
 

 MotionTracker is not thread-safe!
 
******************************************************************/

#include "definition.h"

#include "../core/time.h"

#define SLIB_MOTION_TRACKER_HISTORY_SIZE 20

namespace slib
{
	
	class UIEvent;
	
	class MotionTracker : public Referable
	{
	public:
		MotionTracker();
		
		MotionTracker(sl_uint32 degree);
		
		~MotionTracker();
		
	public:
		void addMovement(sl_real x, sl_real y, const Time& time);
		
		void addMovement(sl_real x, sl_real y);
		
		void addMovement(const Point& pt, const Time& time);
		
		void addMovement(const Point& pt);
		
		void addMovement(UIEvent* ev);
		
		sl_bool getLastPosition(Point* _out);
		
		void clearMovements();
		
		sl_bool getVelocity(sl_real* outX, sl_real* outY);
		
		sl_bool getVelocity(Point* _out);
		
	protected:
		void _computeVelocity();
		
	protected:
		sl_int32 m_degree;
		Time m_lastTime;
		
		struct Movement
		{
			sl_real x;
			sl_real y;
			Time time;
		};
		Movement m_history[SLIB_MOTION_TRACKER_HISTORY_SIZE];
		sl_uint32 m_nHistory;
		sl_uint32 m_topHistory;
		
		Point m_currentVelocity;
		sl_real m_currentConfidence;
		
		sl_bool m_flagRefreshTrack;
		sl_bool m_flagValidTrack;
		
	};

}

#endif
