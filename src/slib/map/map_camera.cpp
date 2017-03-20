/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/map/camera.h"

#include "../../../inc/slib/map/earth.h"
#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/math/transform3d.h"

namespace slib
{

	MapCameraPosition::MapCameraPosition()
	: eye(0, 0, 0), bearing(0), tilt(0)
	{
	}

	MapCameraPosition::MapCameraPosition(const MapCameraPosition& other) = default;

	MapCameraPosition::MapCameraPosition(const GeoLocation& _eye, float _bearing, float _tilt)
	: eye(_eye), bearing(_bearing), tilt(_tilt)
	{
	}

	MapCameraPosition::~MapCameraPosition()
	{
	}

	Matrix4lf MapCameraPosition::getLookDownMatrix() const
	{
		Matrix4lf matView;
		Transform3lf::setRotationY(matView, Math::getRadianFromDegrees(eye.longitude));
		Transform3lf::rotateX(matView, Math::getRadianFromDegrees(eye.latitude));
		double b = bearing;
		if (!(Math::isAlmostZero(b))) {
			Transform3lf::rotateZ(matView, Math::getRadianFromDegrees(b));
		}
		double L = RenderEarth::getRadius() + eye.altitude;
		Transform3lf::translate(matView, 0, 0, L);
		return matView;
	}

	Matrix4lf MapCameraPosition::getViewMatrix() const
	{
		Matrix4lf matView = getLookDownMatrix();
		double t = tilt;
		if (t > SLIB_EPSILON) {
			Transform3lf::rotateX(matView, Math::getRadianFromDegrees(t));
		}
		return matView;
	}

	float MapCameraPosition::getMaximumTiltForMap() const
	{
		double m1 = 350000, m2 = 10000;
		if (eye.altitude > m1) {
			return 0;
		} else {
			double R = RenderEarth::getRadius();
			double L = R + eye.altitude;
			if (L < R) {
				L = R;
			}
			double sinAlpha = (R - 1) / L;
			double alpha = Math::getDegreesFromRadian(Math::arcsin(sinAlpha));
			if (eye.altitude < m2) {
				alpha = alpha * (m1 - eye.altitude) / (m1 - m2);
			}
			if (alpha > 70) {
				alpha = 70;
			}
			return (float)alpha;
		}
	}

	float MapCameraPosition::normalizeTilt(float t)
	{
		if (t < 0) {
			t = 0;
		}
		if (t > 90) {
			t = 90;
		}
		return t;
	}

	MapCameraPosition MapCameraPosition::lerp(const MapCameraPosition& target, float factor) const
	{
		return MapCameraPosition(eye.lerp(target.eye, factor),
								SLIB_LERP(bearing, target.bearing, factor),
								SLIB_LERP(tilt, target.tilt, factor));
	}


	SLIB_DEFINE_OBJECT(MapCamera, Object);

	MapCamera::MapCamera()
	{
		m_position.eye = GeoLocation(0, 0, 8000000);
		m_position.bearing = 0;
		m_position.tilt = 0;
		
		m_flagInvalidatedViewMatrix = sl_true;
		
		m_minimumAltitudeForAnimation = 0;
	}

	MapCamera::~MapCamera()
	{
	}

	Ref<MapCamera> MapCamera::create()
	{
		return new MapCamera;
	}

	const MapCameraPosition& MapCamera::getCameraPosition()
	{
		return m_position;
	}

	void MapCamera::setCameraPosition(const MapCameraPosition& pos)
	{
		m_position = pos;
		_invalidateViewMatrix();
	}

	const GeoLocation& MapCamera::getEyeLocation()
	{
		return m_position.eye;
	}

	void MapCamera::setEyeLocation(const GeoLocation& eye)
	{
		m_position.eye = eye;
		_invalidateViewMatrix();
	}

	Vector3 MapCamera::getEyePosition()
	{
		return RenderEarth::getCartesianPosition(m_position.eye);
	}

	float MapCamera::getBearing()
	{
		return m_position.bearing;
	}

	void MapCamera::setBearing(float degree)
	{
		m_position.bearing = degree;
		_invalidateViewMatrix();
	}

	float MapCamera::getTilt()
	{
		return m_position.tilt;
	}

	void MapCamera::setTilt(float degree)
	{
		m_position.tilt = degree;
		_invalidateViewMatrix();
	}

	const Matrix4lf& MapCamera::getViewMatrix()
	{
		if (m_flagInvalidatedViewMatrix) {
			m_flagInvalidatedViewMatrix = sl_false;
			m_viewMatrix = m_position.getViewMatrix();
		}
		return m_viewMatrix;
	}

	class _MapCamera : public MapCamera
	{
		friend class _MapCameraNavigationTarget;
		friend class _MapCameraBearingAnimationTarget;
		friend class _MapCameraTiltAnimationTarget;
	};

	class _MapCameraNavigationTarget : public AnimationTargetT<MapCameraPosition>
	{
	public:
		WeakRef<MapCamera> m_camera;
		
		_MapCameraNavigationTarget(const Ref<MapCamera>& camera, const AnimationFrames<MapCameraPosition>& frames) : AnimationTargetT<MapCameraPosition>(frames), m_camera(camera)
		{
		}
		
		// override
		void update(float fraction, const MapCameraPosition& value)
		{
			Ref<MapCamera> _camera(m_camera);
			_MapCamera* camera = (_MapCamera*)(_camera.get());
			if (camera) {
				double minAlt = camera->m_minimumAltitudeForAnimation;
				camera->m_position = value;
				if (camera->m_position.eye.altitude > minAlt) {
					camera->m_position.eye.altitude = minAlt;
				}
				camera->_invalidateViewMatrix();
			}
		}
		
	};

	class _MapCameraBearingAnimationTarget : public AnimationTargetT<float>
	{
	public:
		WeakRef<MapCamera> m_camera;
		
		_MapCameraBearingAnimationTarget(const Ref<MapCamera>& camera, const AnimationFrames<float>& frames) : AnimationTargetT<float>(frames), m_camera(camera)
		{
		}
		
		// override
		void update(float fraction, const float& value)
		{
			Ref<MapCamera> _camera(m_camera);
			_MapCamera* camera = (_MapCamera*)(_camera.get());
			if (camera) {
				camera->m_position.bearing = value;
				camera->_invalidateViewMatrix();
			}
		}
		
	};

	class _MapCameraTiltAnimationTarget : public AnimationTargetT<float>
	{
	public:
		WeakRef<MapCamera> m_camera;
		
		_MapCameraTiltAnimationTarget(const Ref<MapCamera>& camera, const AnimationFrames<float>& frames) : AnimationTargetT<float>(frames), m_camera(camera)
		{
		}
		
		// override
		void update(float fraction, const float& value)
		{
			Ref<MapCamera> _camera(m_camera);
			_MapCamera* camera = (_MapCamera*)(_camera.get());
			if (camera) {
				camera->m_position.tilt = value;
				camera->_invalidateViewMatrix();
			}
		}
		
	};

	Ref<Animation> MapCamera::getNavigationAnimation()
	{
		return m_animationNavigation;
	}

	sl_bool MapCamera::isNavigating()
	{
		return m_animationNavigation.isNotNull();
	}

	Ref<Animation> MapCamera::startNavigation(const AnimationFrames<MapCameraPosition>& frames, float duration, AnimationCurve curve)
	{
		stopNavigation();
		stopTiltAnimation();
		stopBearingAnimation();
		
		Ref<Animation> animation = Animation::create(duration);
		if (animation.isNotNull()) {
			Ref<_MapCameraNavigationTarget> target = new _MapCameraNavigationTarget(this, frames);
			if (target.isNotNull()) {
				m_animationNavigation = animation;
				animation->addTarget(target);
				animation->setAnimationCurve(curve);
				animation->start();
				return animation;
			}
		}
		return sl_null;
	}

	Ref<Animation> MapCamera::startNavigation(const MapCameraPosition& newPosition, float duration, AnimationCurve curve)
	{
		stopNavigation();
		return startNavigation(AnimationFrames<MapCameraPosition>(m_position, newPosition), duration, curve);
	}

	Ref<Animation> MapCamera::startNavigation(const GeoLocation& newPosition, float duration, AnimationCurve curve)
	{
		stopNavigation();
		return startNavigation(AnimationFrames<MapCameraPosition>(m_position, MapCameraPosition(newPosition, m_position.bearing, m_position.tilt)), duration, curve);
	}

	Ref<Animation> MapCamera::startNavigation(const LatLon& newPosition, float duration, AnimationCurve curve)
	{
		stopNavigation();
		return startNavigation(AnimationFrames<MapCameraPosition>(m_position, MapCameraPosition(GeoLocation(newPosition, m_position.eye.altitude), m_position.bearing, m_position.tilt)), duration, curve);
	}

	void MapCamera::stopNavigation()
	{
		Ref<Animation> animation = m_animationNavigation;
		if (animation.isNotNull()) {
			animation->stop();
		}
		m_animationNavigation.setNull();
	}

	Ref<Animation> MapCamera::getBearingAnimation()
	{
		return m_animationBearing;
	}

	Ref<Animation> MapCamera::startBearingAnimation(float targetValue, float duration)
	{
		if (m_animationNavigation.isNull()) {
			stopBearingAnimation();
			Ref<Animation> animation = Animation::create(duration);
			if (animation.isNotNull()) {
				Ref<_MapCameraBearingAnimationTarget> target = new _MapCameraBearingAnimationTarget(this, AnimationFrames<float>(m_position.bearing, targetValue));
				if (target.isNotNull()) {
					m_animationBearing = animation;
					animation->addTarget(target);
					animation->setAnimationCurve(AnimationCurve::Linear);
					animation->start();
					return animation;
				}
			}
		}
		return sl_null;
	}

	void MapCamera::stopBearingAnimation()
	{
		Ref<Animation> animation = m_animationBearing;
		if (animation.isNotNull()) {
			animation->stop();
		}
		m_animationBearing.setNull();
	}

	Ref<Animation> MapCamera::getTiltAnimation()
	{
		return m_animationTilt;
	}

	Ref<Animation> MapCamera::startTiltAnimation(float targetValue, float duration)
	{
		if (m_animationNavigation.isNull()) {
			stopBearingAnimation();
			Ref<Animation> animation = Animation::create(duration);
			if (animation.isNotNull()) {
				Ref<_MapCameraTiltAnimationTarget> target = new _MapCameraTiltAnimationTarget(this, AnimationFrames<float>(m_position.tilt, targetValue));
				if (target.isNotNull()) {
					m_animationTilt = animation;
					animation->addTarget(target);
					animation->setAnimationCurve(AnimationCurve::Linear);
					animation->start();
					return animation;
				}
			}
		}
		return sl_null;
	}

	void MapCamera::stopTiltAnimation()
	{
		Ref<Animation> animation = m_animationTilt;
		if (animation.isNotNull()) {
			animation->stop();
		}
		m_animationTilt.setNull();
	}

	double MapCamera::getMimimumAltitudeForAnimation()
	{
		return m_minimumAltitudeForAnimation;
	}

	void MapCamera::setMinimumAltitudeForAnimation(double altitude)
	{
		m_minimumAltitudeForAnimation = altitude;
	}

	Ref<Animation> MapCamera::startNavigationToLookAt(const GeoLocation& location, float duration)
	{
		Vector3lf pos1 = RenderEarth::getCartesianPosition(m_position.eye);
		Vector3lf pos2 = RenderEarth::getCartesianPosition(location);
		double len = (pos2 - pos1).getLength();
		double a = (m_position.eye.altitude + location.altitude) / 2;
		if (len > a * 2) {
			GeoLocation locationMid;
			locationMid.longitude = (location.longitude + m_position.eye.longitude) / 2;
			locationMid.latitude = (location.latitude + m_position.eye.latitude) / 2;
			locationMid.altitude = a + len / 3;
			AnimationFrames<MapCameraPosition> frames(m_position, MapCameraPosition(location, m_position.bearing, 0));
			frames.addFrame(0.5f, MapCameraPosition(locationMid, m_position.bearing, 0));
			return startNavigation(frames, duration);
		} else {
			return startNavigation(location, duration);
		}
	}

	void MapCamera::_invalidateViewMatrix()
	{
		m_flagInvalidatedViewMatrix = sl_true;
	}

}
