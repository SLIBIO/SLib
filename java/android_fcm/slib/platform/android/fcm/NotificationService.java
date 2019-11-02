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

package slib.platform.android.fcm;

import androidx.annotation.NonNull;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.InstanceIdResult;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Map;

public class NotificationService extends FirebaseMessagingService {

	public interface NotificationListener {

		void onToken(String token);

		void onMessageReceived(String title, String content, Map<String, String> data);

		void onException(Exception e);

	}

	@Override
	public void onCreate() {
		super.onCreate();
	}

	public static void initialize(final NotificationListener listener) {
		mListener = listener;
		if (listener == null) {
			return;
		}
		try {
			FirebaseInstanceId.getInstance().getInstanceId().addOnCompleteListener(new OnCompleteListener<InstanceIdResult>() {
				@Override
				public void onComplete(@NonNull Task<InstanceIdResult> task) {
					try {
						if (!(task.isSuccessful())) {
							listener.onException(task.getException());
						}
						InstanceIdResult result = task.getResult();
						if (result != null) {
							String token = result.getToken();
							listener.onToken(token);
						}
					} catch (Exception e) {
						listener.onException(e);
					}
				}
			});
		} catch (Exception e) {
			listener.onException(e);
		}
	}

	@Override
	public void onNewToken(String token) {
		NotificationListener listener = mListener;
		if (listener == null) {
			return;
		}
		if (token != null) {
			listener.onToken(token);
		}
	}

	@Override
	public void onMessageReceived(RemoteMessage remoteMessage) {
		NotificationListener listener = mListener;
		if (listener == null) {
			return;
		}
		try {
			String title = null;
			String content = null;
			RemoteMessage.Notification notification = remoteMessage.getNotification();
			if (notification != null) {
				title = notification.getTitle();
				content = notification.getBody();
			}
			Map<String, String> data = remoteMessage.getData();
			listener.onMessageReceived(title, content, data);
		} catch (Exception e) {
			listener.onException(e);
		}
    }

	private static NotificationListener mListener;

}
