#ifndef CHECKHEADER_SLIB_MAP_CAMERA
#define CHECKHEADER_SLIB_MAP_CAMERA

#include "definition.h"

#include "geo_location.h"

#include "../math/matrix4.h"
#include "../core/animation.h"

SLIB_MAP_NAMESPACE_BEGIN

class SLIB_EXPORT MapCameraPosition
{
public:
	GeoLocation eye;
	float bearing;
	float tilt;
	
public:
	MapCameraPosition();

	MapCameraPosition(const MapCameraPosition& other);

	MapCameraPosition(const GeoLocation& eye, float bearing, float tilt);
	
public:
	Matrix4lf getLookDownMatrix() const;

	Matrix4lf getViewMatrix() const;

	float getMaximumTiltForMap() const;
	
	static float normalizeTilt(float t);
	
	MapCameraPosition lerp(const MapCameraPosition& target, float factor) const;

};

class SLIB_EXPORT MapCamera : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	MapCamera();
	
public:
	static Ref<MapCamera> create();

public:
	const MapCameraPosition& getCameraPosition();
	
	void setCameraPosition(const MapCameraPosition& pos);
	
	const GeoLocation& getEyeLocation();

	void setEyeLocation(const GeoLocation& eye);
	
	Vector3 getEyePosition();
	
	float getBearing();
	
	void setBearing(float degree);
	
	float getTilt();
	
	void setTilt(float degree);

	const Matrix4lf& getViewMatrix();

	Ref<Animation> getNavigationAnimation();
	
	sl_bool isNavigating();
	
	Ref<Animation> startNavigation(const AnimationFrames<MapCameraPosition>& frames, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startNavigation(const MapCameraPosition& newPosition, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startNavigation(const GeoLocation& newPosition, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	Ref<Animation> startNavigation(const LatLon& newPosition, float duration, AnimationCurve curve = AnimationCurve::Default);
	
	void stopNavigation();
	
	Ref<Animation> getBearingAnimation();

	Ref<Animation> startBearingAnimation(float target, float duration);

	void stopBearingAnimation();
	
	Ref<Animation> getTiltAnimation();
	
	Ref<Animation> startTiltAnimation(float target, float duration);
	
	void stopTiltAnimation();
	
	double getMimimumAltitudeForAnimation();
	
	void setMinimumAltitudeForAnimation(double altitude);
	
	Ref<Animation> startNavigationToLookAt(const GeoLocation& location, float duration);

protected:
	void _invalidateViewMatrix();
	
protected:
	MapCameraPosition m_position;
	
	sl_bool m_flagInvalidatedViewMatrix;
	Matrix4lf m_viewMatrix;
	
	SafeRef<Animation> m_animationNavigation;
	SafeRef<Animation> m_animationBearing;
	SafeRef<Animation> m_animationTilt;
	
	double m_minimumAltitudeForAnimation;
	
};


template <>
SLIB_INLINE MapCameraPosition Interpolation<MapCameraPosition>::interpolate(const MapCameraPosition& a, const MapCameraPosition& b, float factor)
{
	return a.lerp(b, factor);
}

SLIB_MAP_NAMESPACE_END

#endif
