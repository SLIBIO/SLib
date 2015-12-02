package slib.platform.android.mobile;

import slib.platform.android.Logger;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.view.WindowManager;

public class Sensors {

	private long mInstance = 0;
	private Activity mContext = null;
	private SensorManager mSensorManager = null;
	private LocationManager mLocationManager = null;

	private LocationTracker locationTracker;
	private Compass compass;
	private Accelerometer acceleromter;

	private static final float ALPHA = 0.15f;

	private Sensors(Activity context) {
		mContext = context;
		mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
		mLocationManager = (LocationManager) context.getApplicationContext().getSystemService(Context.LOCATION_SERVICE);
		locationTracker = new LocationTracker();
		compass = new Compass();
		acceleromter = new Accelerometer();

	}

	private int getDeviceOrientation()
	{
		if (mContext != null) {
			return ((WindowManager)mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getRotation();
		}
		return 1;
	}
	public static Sensors create(Activity context) {
		try {
			Logger.info("Sensor Info");
			Sensors ret = new Sensors(context);
			return ret;
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}

	public void setInstance(long instance) {
		mInstance = instance;
	}

	public boolean start(boolean flagUseLocation, int type, boolean flagUseCompass, boolean flagUseAccelerometer) {
		if (mContext == null) {
			return false;
		}
		stop();

		try {
			if (flagUseLocation || flagUseCompass) {
				if (mLocationManager != null) {
					if (!locationTracker.start(type)) {
						return false;
					}
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		try {
			if (flagUseCompass) {
				if (mSensorManager != null) {
					acceleromter.start();
					compass.start();
				}
			} else if (flagUseAccelerometer) {
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
		try {
			locationTracker.stop();
			compass.stop();
			acceleromter.stop();
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}

	private float lowPass(float input, float output) {
		if (Math.abs(180 - input) > 170) {
			return input;
		}
		return output + ALPHA * (input - output);
	}

	private static native void nativeOnChangeLocation(long instance, float latitude, float longitude, float altitude);
	private static native void nativeOnChangeCompass(long instance, float declination);
	private static native void nativeOnChangeAccelerometer(long instance, float xAccel, float yAccel, float zAccel);

	class Accelerometer {
		boolean flagStarted = false;
		SensorEventListener listener;
		Sensor mSensorAccelerometer = null;
		public Accelerometer()
		{
			mSensorAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			listener = new SensorEventListener() {
				@Override
				public void onSensorChanged(SensorEvent event) {
					switch (event.sensor.getType()) {
					case Sensor.TYPE_ACCELEROMETER:
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
				public void onAccuracyChanged(Sensor sensor, int accuracy) {}
			};
		}

		public boolean start()
		{
			mSensorManager.registerListener(listener, mSensorAccelerometer, SensorManager.SENSOR_DELAY_GAME);
			flagStarted = true;
			return true;
		}

		public void stop()
		{
			if (flagStarted) {
				mSensorManager.unregisterListener(listener, mSensorAccelerometer);
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
									, (float)(location.getLatitude())
									, (float)(location.getLongitude())
									, (float)(location.getAltitude()));
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
						, (float)(lastLocation.getLatitude())
						, (float)(lastLocation.getLongitude())
						, (float)(lastLocation.getAltitude()));
			}
			mContext.runOnUiThread(new Runnable() {
				public void run() {
					try {
						mLocationManager.requestLocationUpdates(providerName, LOCATION_REFRESH_INTERVAL, 0, listener);					
					} catch (Throwable e) {
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
				flagStarted = false;
				mLocationManager.removeUpdates(listener);					
			}
		}
	}

	class Compass {

		private Sensor mSensorOrientation = null;
		public boolean flagStarted = false;
		SensorEventListener listener;
		float  azimuth;

		@SuppressWarnings("deprecation")
		public Compass() {
			mSensorOrientation = mSensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION);
			listener = new SensorEventListener() {
				public void onAccuracyChanged(Sensor sensor, int accuracy) {}

				public void onSensorChanged(SensorEvent event) {
					try {
						switch (event.sensor.getType()) {
						case Sensor.TYPE_ORIENTATION:
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

		public void start() {
			mSensorManager.registerListener(listener, mSensorOrientation, SensorManager.SENSOR_DELAY_GAME);
			flagStarted = true;
		}

		public void stop() {
			if (flagStarted) {
				flagStarted = false;
				mSensorManager.unregisterListener(listener, mSensorOrientation);
			}
		}
	}
}
