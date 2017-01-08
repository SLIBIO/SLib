#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/device/sensor.h"
#include "../../../inc/slib/core/safe_static.h"

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

#define G 9.81f

SLIB_DEVICE_NAMESPACE_BEGIN
class _Sensor;
SLIB_DEVICE_NAMESPACE_END

@interface _Slib_iOS_Sensor : NSObject <CLLocationManagerDelegate>{
	
	@public NSOperationQueue* operationQueue;
	@public CMMotionManager* m_motionManager;
	@public CLLocationManager* m_locationManager;
	@public slib::WeakRef<slib::_Sensor> m_sensor;
	
	@public sl_bool flagUseCompass;
	@public sl_bool flagUseAccelerometor;
	@public sl_bool flagUseLocation;
	
}

-(id) initWithParam:(const slib::SensorParam*) param;
-(sl_bool) start;
-(void) stop;

@end

SLIB_DEVICE_NAMESPACE_BEGIN

class _Sensor_Shared
{
public:
	CMMotionManager* defaultMotion;
	
public:
	_Sensor_Shared()
	{
		defaultMotion = [[CMMotionManager alloc] init];
	}
	
};

SLIB_SAFE_STATIC_GETTER(_Sensor_Shared, _getSensor_Shared)

class _Sensor : public Sensor
{
public:
	_Slib_iOS_Sensor* m_sensor;
    
public:
	_Sensor()
	{
	}

	~_Sensor()
	{
		stop();
	}

public:
	static Ref<_Sensor> create(const SensorParam& param)
	{
		_Slib_iOS_Sensor* sensor = [[_Slib_iOS_Sensor alloc] initWithParam:&param];
		if(sensor != nil){
			Ref<_Sensor> ret = new _Sensor;
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

	// override
	sl_bool _start()
	{
		if ([m_sensor start]) {
            return sl_true;
        }
        return sl_false;
	}

	// override
	void _stop()
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

Ref<Sensor> Sensor::create(const SensorParam& param)
{
	return _Sensor::create(param);
}

sl_bool Sensor::isAvailableLocation()
{
	return sl_true;
}

sl_bool Sensor::isAvailableCompass()
{
	_Sensor_Shared* shared = _getSensor_Shared();
	if (shared) {
		if (shared->defaultMotion.magnetometerAvailable) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool Sensor::isAvailableAccelerometer()
{
	_Sensor_Shared* shared = _getSensor_Shared();
	if (shared) {
		if (shared->defaultMotion.accelerometerAvailable) {
			return sl_true;
		}
	}
	return sl_false;
}


SLIB_DEVICE_NAMESPACE_END

@implementation _Slib_iOS_Sensor
-(id) initWithParam:(const slib::SensorParam *)param
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

-(sl_bool) start
{
	slib::Ref<slib::_Sensor> sensor(m_sensor);
	if (sensor.isNull()) {
		return sl_false;
	}
	
	if (flagUseLocation) {
		dispatch_async(dispatch_get_main_queue(), ^{
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
		});
		
		sensor->setRunningLocation(sl_true);
	}
	
	if (flagUseCompass) {
		if (m_motionManager.magnetometerAvailable) {
			sensor->setRunningCompass(sl_true);
			[m_motionManager startMagnetometerUpdatesToQueue:operationQueue withHandler:^(CMMagnetometerData * _Nullable magnetometerData, NSError * _Nullable error) {
				slib::Ref<slib::_Sensor> sensor(m_sensor);
				if (sensor.isNotNull() && magnetometerData != nil) {
					sensor->onChangeCompass(magnetometerData.magneticField.x);
				}
			}];
		} else {
			NSLog(@"magnetometer is not available!");
		}
	}
	
	if (flagUseAccelerometor) {
		if (m_motionManager.accelerometerAvailable) {
			sensor->setRunningAccelerometer(sl_true);
			sensor->setRunningAccelerometer(sl_true);
			[m_motionManager startDeviceMotionUpdatesToQueue:operationQueue withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
				slib::Ref<slib::_Sensor> sensor(m_sensor);
				if (sensor.isNotNull() && motion != nil) {
					sensor->onChangeAccelerometer((float)(motion.userAcceleration.x + motion.gravity.x) * G, (float)(motion.userAcceleration.y + motion.gravity.y) * G, (float)(motion.userAcceleration.z + motion.gravity.z) * G);
				}
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

-(void) stop
{
	slib::Ref<slib::_Sensor> sensor(m_sensor);
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
		dispatch_async(dispatch_get_main_queue(), ^{
			[m_locationManager stopUpdatingLocation];
		});
	}
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
{
	if (locations != nil) {
		CLLocation* location = [locations lastObject];
		slib::Ref<slib::_Sensor> sensor(m_sensor);
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
