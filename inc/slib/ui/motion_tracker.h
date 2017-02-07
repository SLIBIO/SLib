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

SLIB_UI_NAMESPACE_BEGIN

#define SLIB_MOTION_TRACKER_HISTORY_SIZE 20

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

SLIB_UI_NAMESPACE_END

#endif
