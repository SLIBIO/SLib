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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/device/sensor.h"

#include "slib/core/safe_static.h"

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

#define G 9.81f

namespace slib
{
	namespace priv
	{
		namespace sensor
		{
			class SensorImpl;
		}
	}
}

@interface SLIBSensor : NSObject <CLLocationManagerDelegate>
{
	
	@public NSOperationQueue* operationQueue;
	@public CMMotionManager* m_motionManager;
	@public CLLocationManager* m_locationManager;
	@public slib::WeakRef<slib::priv::sensor::SensorImpl> m_sensor;
	
	@public sl_bool flagUseCompass;
	@public sl_bool flagUseAccelerometor;
	@public sl_bool flagUseLocation;
	
}

-(id) initWithParam:(const slib::SensorParam*) param;
-(sl_bool) start;
-(void) stop;

@end

namespace slib
{

	namespace priv
	{
		namespace sensor
		{

			class SharedContext
			{
			public:
				CMMotionManager* defaultMotion;
				
			public:
				SharedContext()
				{
					defaultMotion = [[CMMotionManager alloc] init];
				}
				
			};

			SLIB_SAFE_STATIC_GETTER(SharedContext, GetSharedContext)

			class SensorImpl : public Sensor
			{
			public:
				SLIBSensor* m_sensor;
				
			public:
				SensorImpl()
				{
				}

				~SensorImpl()
				{
					stop();
				}

			public:
				static Ref<SensorImpl> create(const SensorParam& param)
				{
					SLIBSensor* sensor = [[SLIBSensor alloc] initWithParam:&param];
					if(sensor != nil){
						Ref<SensorImpl> ret = new SensorImpl;
						if(ret.isNotNull()) {
							ret->_init(param);
							ret->m_sensor = sensor;
							sensor->m_sensor = ret;
							if(param.flagAutoStart) {
								ret->start();
							}
							return ret;
						}
					}
					return sl_null;
				}

				sl_bool _start() override
				{
					if ([m_sensor start]) {
						return sl_true;
					}
					return sl_false;
				}

				void _stop() override
				{
					[m_sensor stop];
				}
				
				void onChangeLocation(double latitude, double longitude, double altitude)
				{
					_onLocationChanged(GeoLocation(latitude, longitude, altitude));
				}

				void onChangeCompass(float declination)
				{
					_onCompassChanged(declination);
				}

				void onChangeAccelerometer(float xAccel, float yAccel, float zAccel)
				{
					_onAccelerometerChanged(xAccel, yAccel, zAccel);
				}
				
				void setRunningLocation(sl_bool flag)
				{
					m_flagRunningLocation = flag;
				}
				
				void setRunningCompass(sl_bool flag)
				{
					m_flagRunningCompass = flag;
				}
				
				void setRunningAccelerometer(sl_bool flag)
				{
					m_flagRunningAccelerometer = flag;
				}
			};

		}
	}

	using namespace priv::sensor;

	Ref<Sensor> Sensor::create(const SensorParam& param)
	{
		return SensorImpl::create(param);
	}

	sl_bool Sensor::isAvailableLocation()
	{
		return sl_true;
	}

	sl_bool Sensor::isAvailableCompass()
	{
		SharedContext* shared = GetSharedContext();
		if (shared) {
			if (shared->defaultMotion.magnetometerAvailable) {
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool Sensor::isAvailableAccelerometer()
	{
		SharedContext* shared = GetSharedContext();
		if (shared) {
			if (shared->defaultMotion.accelerometerAvailable) {
				return sl_true;
			}
		}
		return sl_false;
	}

}

using namespace slib;
using namespace slib::priv::sensor;

@implementation SLIBSensor

-(id) initWithParam:(const SensorParam *)param
{
	self = [super init];
	if(self != nil){
		operationQueue = [[NSOperationQueue alloc] init];
		m_motionManager = [[CMMotionManager alloc] init];
		flagUseCompass = param->flagUseCompass;
		flagUseAccelerometor = param->flagUseAccelerometor;
		flagUseLocation = param->flagUseLocation;
	}
	return self;
}

-(void) onChangeCompass: (CMMagnetometerData * _Nullable) magnetometerData error: (NSError * _Nullable) error
{
	Ref<SensorImpl> sensor(m_sensor);
	if (sensor.isNotNull() && magnetometerData != nil) {
		sensor->onChangeCompass(magnetometerData.magneticField.x);
	}
}

-(void) onChangeAccelerometer: (CMDeviceMotion * _Nullable) motion error: (NSError * _Nullable) error
{
	Ref<SensorImpl> sensor(m_sensor);
	if (sensor.isNotNull() && motion != nil) {
		sensor->onChangeAccelerometer((float)(motion.userAcceleration.x + motion.gravity.x) * G, (float)(motion.userAcceleration.y + motion.gravity.y) * G, (float)(motion.userAcceleration.z + motion.gravity.z) * G);
	}
}

-(sl_bool) start
{
	Ref<SensorImpl> sensor(m_sensor);
	if (sensor.isNull()) {
		return sl_false;
	}
	
	if (flagUseLocation) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self startLocationManager];
		});
		sensor->setRunningLocation(sl_true);
	}
	
	if (flagUseCompass) {
		if (m_motionManager.magnetometerAvailable) {
			sensor->setRunningCompass(sl_true);
			[m_motionManager startMagnetometerUpdatesToQueue:operationQueue withHandler:^(CMMagnetometerData * _Nullable magnetometerData, NSError * _Nullable error) {
				[self onChangeCompass:magnetometerData error:error];
			}];
		} else {
			NSLog(@"magnetometer is not available!");
		}
	}
	
	if (flagUseAccelerometor) {
		if (m_motionManager.accelerometerAvailable) {
			sensor->setRunningAccelerometer(sl_true);
			[m_motionManager startDeviceMotionUpdatesToQueue:operationQueue withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
				[self onChangeAccelerometer:motion error:error];
			}];
		} else {
			NSLog(@"accelerometer is not available!");
		}
	}
	
	if (sensor->isRunningLocation()) {
		return sl_true;
	}
	
	if (sensor->isRunningAccelerometer()) {
		return sl_true;
	}
	
	if (sensor->isRunningCompass()) {
		return sl_true;
	}
	
	if (sensor->isRunningLocation()) {
		return sl_true;
	}
	
	return sl_false;
}

-(void) startLocationManager
{
	if (m_locationManager == nil) {
		m_locationManager = [[CLLocationManager alloc] init];
		m_locationManager.delegate = self;
		
		if (__IPHONE_OS_VERSION_MAX_ALLOWED >= 80000) {
			[m_locationManager requestAlwaysAuthorization];
		} else {
			[m_locationManager requestWhenInUseAuthorization];
		}
	}
	[m_locationManager startUpdatingLocation];
}

-(void) stop
{
	Ref<SensorImpl> sensor(m_sensor);
	if (sensor.isNull()) {
		return;
	}
	
	if (sensor->isRunningCompass()) {
		[m_motionManager stopMagnetometerUpdates];
	}
	
	if (sensor->isRunningAccelerometer()) {
		[m_motionManager stopAccelerometerUpdates];
	}
	
	if (sensor->isRunningLocation()) {
		CLLocationManager* manager = m_locationManager;
		if (manager != nil) {
			dispatch_async(dispatch_get_main_queue(), ^{
				[manager stopUpdatingLocation];
			});
		}
	}
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
{
	if (locations != nil) {
		CLLocation* location = [locations lastObject];
		Ref<SensorImpl> sensor(m_sensor);
		if (sensor.isNotNull() && location != nil) {
			sensor->onChangeLocation(location.coordinate.latitude, location.coordinate.longitude, location.altitude);
		}
	}
}

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error
{
}

@end

#endif
