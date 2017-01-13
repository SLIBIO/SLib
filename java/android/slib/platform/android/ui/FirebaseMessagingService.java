package slib.platform.android.ui;

import com.google.firebase.messaging.RemoteMessage;

import java.util.Map;
import java.util.Vector;

public class FirebaseMessagingService extends com.google.firebase.messaging.FirebaseMessagingService {
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        Map<String, String> map = remoteMessage.getData();
        Vector<String> data = new Vector<String>();

        if (map != null) {
            int size = map.size();
            String[] values = map.values().toArray(new String[] {});
            String[] keys = map.keySet().toArray(new String[] {});

            for (int i = 0; i < size; i++) {
                String key = keys[i];
                String value = values[i];

                if (key == null && value == null) {
                    break;
                }

                data.add(key);
                data.add(value);
            }
        }

        String title = remoteMessage.getNotification().getTitle();
        String body = remoteMessage.getNotification().getBody();

        nativeOnMessageReceived(title, body, data.toArray(new String[] {}));
    }

    private static native void nativeOnMessageReceived(String title, String body, String[] data);
}
