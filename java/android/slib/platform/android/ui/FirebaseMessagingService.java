package slib.platform.android.ui;

import android.app.NotificationManager;
import android.content.Context;
import android.media.RingtoneManager;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v4.app.NotificationCompat;

import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

public class FirebaseMessagingService extends com.google.firebase.messaging.FirebaseMessagingService {
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        String messageBody = remoteMessage.getNotification().getBody();
        Map<String, String> map = remoteMessage.getData();
        Vector<String> data = new Vector<String>();

        if (data == null) {
            int size = data.size();
            String[] values = map.values().toArray(new String[] {});
            String[] keys = map.keySet().toArray(new String[] {});

            for (int i = 0; i < map.size(); i++) {
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
