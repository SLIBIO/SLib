package slib.platform.android.fcm;

import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import androidx.core.app.NotificationCompat;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.ui.Util;

public class FCM {

	private static boolean mFlagInitialized = false;

	public static void initialize(final Activity activity) {

		if (mFlagInitialized) {
			return;
		}

		if (activity == null) {
			return;
		}
		mActivity = activity;

		mFlagInitialized = true;

		SlibActivity.addIntentListener(new SlibActivity.IntentListener() {
			@Override
			public void onIntent(Intent intent) {
				Serializable o = intent.getSerializableExtra(NOTIFICATION_INTENT_EXTRA_DATA);
				if (o == null) {
					return;
				}
				if (o instanceof Map) {
					Map<String, String> data = (Map<String, String>)o;
					String title = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_TITLE);
					String content = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_CONTENT);
					dispatchMessage(title, content, data, true, false);
				}
			}
		});

		try {
			NotificationService.initialize(new NotificationService.NotificationListener() {

				@Override
				public void onToken(String token) {
					nativeOnToken(token);
				}

				@Override
				public void onMessageReceived(String title, String content, Map<String, String> data) {
					if (data == null) {
						return;
					}
					boolean flagBackground = !(SlibActivity.isRunningGlobal());
					dispatchMessage(title, content, data, false, flagBackground);
					if (flagBackground) {
						if (title == null) {
							title = data.get("title");
						}
						if (content == null) {
							content = data.get("content");
						}
						if (title != null && content != null) {
							showNotification(title, content, data);
						}
						String badge = data.get("badge");
						if (badge != null) {
							try {
								int nBadge = Integer.parseInt(badge);
								if (nBadge >= 0) {
									Util.setBadgeNumber(activity, nBadge);
								}
							} catch (Exception e) {
								Logger.exception(e);
							}
						}
					}
				}

				@Override
				public void onException(Exception e) {
					//Logger.exception(e);
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static native void nativeOnToken(String token);

	private static native void nativeOnMessageReceived(String title, String content, String[] data, boolean flagClicked, boolean flagBackground);


	private static Activity mActivity;

	private final static int NOTIFICATION_ID = 0x7fffffff;
	private final static String NOTIFICATION_CHANNEL_ID = "slib_fcm";
	private final static String NOTIFICATION_CHANNEL_NAME = "UserNotification";
	private final static String NOTIFICATION_INTENT_EXTRA_TITLE = "fcm_title";
	private final static String NOTIFICATION_INTENT_EXTRA_CONTENT = "fcm_content";
	private final static String NOTIFICATION_INTENT_EXTRA_DATA = "fcm_data";

	private static void dispatchMessage(String title, String content, Map<String, String> mapData, boolean flagClicked, boolean flagBackground) {
		if (title == null) {
			title = mapData.get("title");
		}
		if (content == null) {
			content = mapData.get("content");
		}
		ArrayList<String> data = new ArrayList<String>();
		if (mapData != null) {
			for (Map.Entry<String, String> item : mapData.entrySet()) {
				data.add(item.getKey());
				data.add(item.getValue());
			}
		}
		nativeOnMessageReceived(title, content, data.toArray(new String[]{}), flagClicked, flagBackground);
	}

	private static void showNotification(String title, String content, Map<String, String> data) {

		if (title == null || content == null) {
			return;
		}

		Activity activity = mActivity;
		if (activity == null) {
			return;
		}

		try {
			NotificationManager notificationManager = (NotificationManager)(activity.getSystemService(Context.NOTIFICATION_SERVICE));
			if (notificationManager == null) {
				return;
			}
			Intent intent = new Intent(activity, activity.getClass());
			intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
			intent.putExtra(NOTIFICATION_INTENT_EXTRA_TITLE, title);
			intent.putExtra(NOTIFICATION_INTENT_EXTRA_CONTENT, content);
			intent.putExtra(NOTIFICATION_INTENT_EXTRA_DATA, new HashMap<String, String>(data));
			PendingIntent pendingIntent = PendingIntent.getActivity(activity, SlibActivity.REQUEST_INTENT_REMOTE_NOTIFICATION, intent, PendingIntent.FLAG_ONE_SHOT);

			Uri defaultSoundUri= RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
			NotificationCompat.Builder builder = new NotificationCompat.Builder(activity, NOTIFICATION_CHANNEL_ID);
			builder.setContentTitle(title);
			builder.setContentText(content);
			builder.setAutoCancel(true);
			builder.setSound(defaultSoundUri);
			builder.setSmallIcon(activity.getApplication().getApplicationInfo().icon);
			builder.setContentIntent(pendingIntent);
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				NotificationChannel channel = new NotificationChannel(NOTIFICATION_CHANNEL_ID, NOTIFICATION_CHANNEL_NAME, NotificationManager.IMPORTANCE_DEFAULT);
				notificationManager.createNotificationChannel(channel);
			}
			notificationManager.notify(NOTIFICATION_ID, builder.build());
		} catch (Exception e) {
			Logger.exception(e);
		}

	}

}
