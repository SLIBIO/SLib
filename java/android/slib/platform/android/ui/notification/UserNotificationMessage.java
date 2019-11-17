package slib.platform.android.ui.notification;

import androidx.core.app.NotificationCompat;

public class UserNotificationMessage {

	public int id = 0;
	public String title = null;
	public String content = null;
	public String data = null;

	public String subText = null;
	public String contentInfo = null;
	public String ticker = null;
	public String channelId = null;
	public String category = null;
	public String groupKey = null;
	public boolean flagGroupSummary = false;
	public String sortKey = null;

	public String iconName = null;
	public int color = 0;

	public boolean flagOngoing = false;
	public boolean flagOnlyAlertOnce = false;
	public int priority = NotificationCompat.PRIORITY_DEFAULT;

	public long deliveryTime = 0;
	public double deliveryInterval = 0;
	public boolean flagRepeat = false;

	public String action = null;
	public String response = null;
	public long actualDeliveryTime = 0;

}
