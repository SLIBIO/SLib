package slib.platform.android.ui.notification;

import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

import androidx.core.app.NotificationCompat;
import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class UserNotification {

	private final static String NOTIFICATION_INTENT_EXTRA_ID = "slib_ntf_id";
	private final static String NOTIFICATION_INTENT_EXTRA_TITLE = "slib_ntf_title";
	private final static String NOTIFICATION_INTENT_EXTRA_CONTENT = "slib_ntf_content";
	private final static String NOTIFICATION_INTENT_EXTRA_DATA = "slib_ntf_data";
	private final static String NOTIFICATION_INTENT_EXTRA_ACTION = "slib_ntf_action";
	private final static String NOTIFICATION_INTENT_EXTRA_TIME = "slib_ntf_time";

	private final static String NOTIFICATION_DEFAULT_CHANNEL_ID = "slib_default";
	private final static String NOTIFICATION_DEFAULT_CHANNEL_NAME = "UserNotification";

	private static native void nativeOnClickMessage(UserNotificationMessage message);

	public static void initialize(Activity activity) {
		try {
			SlibActivity.addIntentListener(new SlibActivity.IntentListener() {
				@Override
				public void onIntent(Intent intent) {
					if (intent.hasExtra(NOTIFICATION_INTENT_EXTRA_ID)) {
						UserNotificationMessage message = new UserNotificationMessage();
						message.id = intent.getIntExtra(NOTIFICATION_INTENT_EXTRA_ID, 0);
						message.title = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_TITLE);
						message.content = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_CONTENT);
						message.data = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_DATA);
						message.action = intent.getStringExtra(NOTIFICATION_INTENT_EXTRA_ACTION);
						message.actualDeliveryTime = intent.getLongExtra(NOTIFICATION_INTENT_EXTRA_TIME, 0);
						nativeOnClickMessage(message);
					}
				}
			});
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
				NotificationManager notificationManager = (NotificationManager) (activity.getSystemService(Context.NOTIFICATION_SERVICE));
				if (notificationManager != null) {
					NotificationChannel channel = new NotificationChannel(NOTIFICATION_DEFAULT_CHANNEL_ID, NOTIFICATION_DEFAULT_CHANNEL_NAME, NotificationManager.IMPORTANCE_DEFAULT);
					notificationManager.createNotificationChannel(channel);
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void add(Activity activity, UserNotificationMessage message) {
		try {
			NotificationManager mgr = (NotificationManager) (activity.getSystemService(Context.NOTIFICATION_SERVICE));
			if (mgr == null) {
				return;
			}

			String channelId = message.channelId;
			if (channelId == null || channelId.isEmpty()) {
				channelId = NOTIFICATION_DEFAULT_CHANNEL_ID;
			}

			NotificationCompat.Builder builder = new NotificationCompat.Builder(activity, channelId);

			builder.setDefaults(android.app.Notification.DEFAULT_SOUND | android.app.Notification.DEFAULT_VIBRATE | android.app.Notification.DEFAULT_LIGHTS);
			builder.setAutoCancel(true);

			if (message.title != null && message.title.length() > 0) {
				builder.setContentTitle(message.title);
			}
			if (message.content != null && message.content.length() > 0) {
				builder.setContentText(message.content);
			}
			if (message.subText != null && message.subText.length() > 0) {
				builder.setSubText(message.subText);
			}
			if (message.contentInfo != null && message.contentInfo.length() > 0) {
				builder.setContentInfo(message.contentInfo);
			}
			if (message.ticker != null && message.ticker.length() > 0) {
				builder.setTicker(message.ticker);
			}
			if (message.groupKey != null && message.groupKey.length() > 0) {
				builder.setGroup(message.groupKey);
				builder.setGroupSummary(message.flagGroupSummary);
			}
			if (message.sortKey != null && message.sortKey.length() > 0) {
				builder.setSortKey(message.sortKey);
			}
			if (message.category != null && message.category.length() > 0) {
				builder.setCategory(message.category);
			}
			if (message.iconName != null && message.iconName.length() > 0) {
				try {
					builder.setSmallIcon(Integer.parseInt(message.iconName));
				} catch (Exception e) {
					Logger.exception(e);
				}
			} else {
				builder.setSmallIcon(activity.getApplication().getApplicationInfo().icon);
			}
			if (message.color != 0) {
				builder.setColor(message.color);
			}
			builder.setOngoing(message.flagOngoing);
			builder.setOnlyAlertOnce(message.flagOnlyAlertOnce);
			builder.setPriority(message.priority);

			builder.setContentIntent(createIntent(activity, message, "", System.currentTimeMillis()));

			mgr.notify(message.id, builder.build());

		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void cancel(Activity activity, int id) {
		try {
			NotificationManager mgr = (NotificationManager) (activity.getSystemService(Context.NOTIFICATION_SERVICE));
			if (mgr != null) {
				mgr.cancel(id);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void cancelAll(Activity activity) {
		try {
			NotificationManager mgr = (NotificationManager) (activity.getSystemService(Context.NOTIFICATION_SERVICE));
			if (mgr != null) {
				mgr.cancelAll();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static PendingIntent createIntent(Activity activity, UserNotificationMessage message, String action, long time) {
		Intent intent = new Intent(activity, activity.getClass());
		intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_ID, message.id);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_TITLE, message.title);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_CONTENT, message.content);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_DATA, message.data);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_ACTION, action);
		intent.putExtra(NOTIFICATION_INTENT_EXTRA_TIME, time);
		return PendingIntent.getActivity(activity, SlibActivity.REQUEST_INTENT_LOCAL_NOTIFICATION, intent, PendingIntent.FLAG_ONE_SHOT);
	}

}
