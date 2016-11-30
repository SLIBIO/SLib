package slib.platform.android.device;

import slib.platform.android.Logger;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.view.WindowManager;

public class Sensor {

	private long mInstance = 0;
	
	private Activity mContext = null;
	boolean mFlagUseLocation;
	int mLocationProviderType;
	boolean mFlagUseCompass;
	boolean mFlagUseAccelerometer;
	
	private SensorManager mSensorManager = null;
	private LocationManager mLocationManager = null;

	private LocationTracker locationTracker;
	private Compass compass;
	private Accelerometer acceleromter;

	private static final float ALPHA = 0.15f;

	private Sensor(Activity context) {
		
		mContext = context;
		
		mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		mLocationManager = (LocationManager) context.getApplicationContext().getSystemService(Context.LOCATION_SERVICE);

		locationTracker = new LocationTracker();
		compass = new Compass();
		acceleromter = new Accelerometer();
	}

	public static boolean isAvailableLocation(Activity context) {
		LocationManager locationManager = (LocationManager) context.getApplicationContext().getSystemService(Context.LOCATION_SERVICE);
		if (locationManager != null || locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
			return true;
		}
		return false;
	}

	public static boolean isAvailableCompass(Activity context) {
		SensorManager sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		if (sensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ACCELEROMETER) != null) {
			return true;
		}
		return false;
	}

	public static boolean isAvailableAccelerometer(Activity context) {
		SensorManager sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		if (sensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_MAGNETIC_FIELD) != null) {
			return true;
		}
		return false;
	}

	public static Sensor create(Activity context, boolean flagUseLocation, int locationProviderType, boolean flagUseCompass, boolean flagUseAccelerometer) {
		try {
			Logger.info("Sensor Info");
			Sensor ret = new Sensor(context);
			ret.mFlagUseLocation = flagUseLocation;
			ret.mLocationProviderType = locationProviderType;
			ret.mFlagUseCompass = flagUseCompass;
			ret.mFlagUseAccelerometer = flagUseAccelerometer;
			return ret;
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}

	public void setInstance(long instance) {
		mInstance = instance;
	}

	public boolean start() {
		
		if (mContext == null) {
			return false;
		}
		
		stop();

		try {
			if (mFlagUseLocation || mFlagUseCompass) {
				if (mLocationManager != null) {
					if (!locationTracker.start(mLocationProviderType)) {
						return false;
					}
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		try {
			if (mFlagUseCompass) {
				if (mSensorManager != null) {
					acceleromter.start();
					compass.start();
				}
			} else if (mFlagUseAccelerometer) {
				if (mSensorManager != null) {
					acceleromter.start();
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}

		return false;
	}

	public void stop() {
		locationTracker.stop();
		compass.stop();
		acceleromter.stop();
	}

	private float lowPass(float input, float output) {
		if (Math.abs(180 - input) > 170) {
			return input;
		}
		return output + ALPHA * (input - output);
	}

	private static native void nativeOnChangeLocation(long instance, double latitude, double longitude, double altitude);
	private static native void nativeOnChangeCompass(long instance, float declination);
	private static native void nativeOnChangeAccelerometer(long instance, float xAccel, float yAccel, float zAccel);

	private int getDeviceOrientation() {
		if (mContext != null) {
			return ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getRotation();
		}
		return 1;
	}
	
	class Accelerometer {
		boolean flagStarted = false;
		SensorEventListener listener;
		android.hardware.Sensor mSensorAccelerometer = null;
		public Accelerometer()
		{
			mSensorAccelerometer = mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ACCELEROMETER);
			listener = new SensorEventListener() {
				@Override
				public void onSensorChanged(SensorEvent event) {
					switch (event.sensor.getType()) {
					case android.hardware.Sensor.TYPE_ACCELEROMETER:
						final int axisSwap[][] = {
								{ 1,  -1,  0,  1  },     // ROTATION_0 
								{-1,  -1,  1,  0  },     // ROTATION_90 
								{-1,   1,  0,  1  },     // ROTATION_180 
								{ 1,   1,  1,  0  }  	 // ROTATION_270 
						}; 
						final int[] as = axisSwap[getDeviceOrientation()]; 
						float[] accelValue = new float[3];
						accelValue[0]  =  (float)as[0] * event.values[ as[2] ]; 
						accelValue[1]  =  (float)as[1] * event.values[ as[3] ]; 
						accelValue[2]  =  event.values[2];

						nativeOnChangeAccelerometer(mInstance, accelValue[0], accelValue[1], accelValue[2]);
						break;
					}
				}
				public void onAccuracyChanged(android.hardware.Sensor sensor, int accuracy) {}
			};
		}

		public boolean start()
		{
			if (flagStarted) {
				return true;
			}
			try {
				if (mSensorManager.registerListener(listener, mSensorAccelerometer, SensorManager.SENSOR_DELAY_GAME)) {
					flagStarted = true;
					return true;				
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
			return false;
		}

		public void stop()
		{
			if (flagStarted) {
				try {
					mSensorManager.unregisterListener(listener, mSensorAccelerometer);					
				} catch (Exception e) {
					Logger.exception(e);
				}
				flagStarted = false;
			}
		}
	}

	class LocationTracker {

		private static final int LOCATION_REFRESH_INTERVAL = 1000;

		String providerName;

		public Location lastLocation;

		public boolean flagStarted = false;
		public long lastLocationCommit = 0;
		LocationListener listener;

		public LocationTracker()
		{
			listener = new LocationListener() {
				public void onStatusChanged(String provider, int status, Bundle extras) {}
				public void onProviderEnabled(String provider) {}
				public void onProviderDisabled(String provider) {}
				public void onLocationChanged(Location location) {
					try {
						lastLocation = location;
						long curTime = System.currentTimeMillis();
						if (curTime - lastLocationCommit > 10 * 1000) {
							nativeOnChangeLocation(
									mInstance
									, (double)(location.getLatitude())
									, (double)(location.getLongitude())
									, (double)(location.getAltitude()));
							lastLocationCommit = curTime;
						}
					} catch (Throwable e) {
						Logger.exception(e);
					}
				}
			};
		}

		public boolean start(int type)
		{
			if (type == 0) {
				providerName = LocationManager.GPS_PROVIDER;
			} else if (type == 1) {
				providerName = LocationManager.NETWORK_PROVIDER;
			} else if (type == 2) {
				providerName = LocationManager.PASSIVE_PROVIDER;
			} else {
				return false;
			}
			if (!isLocationEnabled()) {
				//openSettings(mContext);
				return false;
		}
			lastLocation = mLocationManager.getLastKnownLocation(providerName);

			if (lastLocation != null) {
				nativeOnChangeLocation(
						mInstance
						, (lastLocation.getLatitude())
						, (lastLocation.getLongitude())
						, (lastLocation.getAltitude()));
			}
			mContext.runOnUiThread(new Runnable() {
				public void run() {
					try {
						mLocationManager.requestLocationUpdates(providerName, LOCATION_REFRESH_INTERVAL, 0, listener);
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			});
			flagStarted = true;

			return true;
		}

		public void openSettings(Context context) {
			Intent settingsActivity = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
			settingsActivity.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_NEW_TASK);
			context.startActivity(settingsActivity);
		}

		public boolean isLocationEnabled() {
			return mLocationManager.isProviderEnabled(providerName);
		}

		public void stop()
		{
			if (flagStarted) {
				try {
					mLocationManager.removeUpdates(listener);
				} catch (Exception e) {
					Logger.exception(e);
				}
				flagStarted = false;
			}
		}
	}

	class Compass {

		private android.hardware.Sensor mSensorOrientation = null;
		public boolean flagStarted = false;
		SensorEventListener listener;
		float  azimuth;

		@SuppressWarnings("deprecation")
		public Compass() {
			mSensorOrientation = mSensorManager.getDefaultSensor(android.hardware.Sensor.TYPE_ORIENTATION);
			listener = new SensorEventListener() {
				public void onAccuracyChanged(android.hardware.Sensor sensor, int accuracy) {}

				public void onSensorChanged(SensorEvent event) {
					try {
						switch (event.sensor.getType()) {
						case android.hardware.Sensor.TYPE_ORIENTATION:
							azimuth = lowPass(event.values[0], azimuth);
							nativeOnChangeCompass(mInstance, azimuth);
							break;
						}

					} catch (Throwable e) {
						Logger.exception(e);
					}
				}
			};
		}

		public boolean start() {
			if (flagStarted) {
				return true;
			}
			try {
				if (mSensorManager.registerListener(listener, mSensorOrientation, SensorManager.SENSOR_DELAY_GAME)) {
					flagStarted = true;
					return true;
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
			return false;
		}

		public void stop() {
			if (flagStarted) {
				try {
					mSensorManager.unregisterListener(listener, mSensorOrientation);
				} catch (Exception e) {
					Logger.exception(e);
				}
				flagStarted = false;
			}
		}
	}
}
