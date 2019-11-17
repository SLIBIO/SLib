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

#ifndef CHECKHEADER_SLIB_UI_NOTIFICATION
#define CHECKHEADER_SLIB_UI_NOTIFICATION

#include "definition.h"

#include "../core/function.h"
#include "../service/push_notification.h"
#include "../graphics/image.h"

namespace slib
{

	class UserNotification;
	class Json;

	class SLIB_EXPORT UserNotificationAttachment
	{
	public:
		String identifier;
		String url;
		
	public:
		UserNotificationAttachment();
		
		UserNotificationAttachment(const String& _id, const String& _url) : identifier(_id), url(_url) {}
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(UserNotificationAttachment)

	};

	class SLIB_EXPORT UserNotificationMessage
	{
	public:
		sl_uint32 id;
		String identifier; // iOS, macOS
		String title;
		String content;
		Json data;

		// Sending
		String subTitle; // iOS, macOS
		String subText; // Android
		String contentInfo; // Android
		String ticker; // Android
		String categoryIdentifier; // iOS, macOS
		String threadIdentifier; // iOS, macOS
		String targetContentIdentifier; // iOS, macOS
		String channelId; // Android
		String category; // Android (must be one of "alarm", "call", "email", "err", "event", "msg", "navigation", "progress", "promo", "recommendation", "reminder", "social", "service", "status", "sys", "transport")
		String groupKey; // Android
		sl_bool flagGroupSummary; // Android
		String sortKey; // Android

		Nullable<sl_uint32> badge; // iOS, macOS
		Ref<Drawable> contentImage; // macOS(old SDK)
		List<UserNotificationAttachment> attachments; // iOS, macOS
		String iconName; // Android(Resource int, small icon)
		String launchImageName; // iOS
		sl_bool flagSound; // macOS, iOS
		String soundName; // macOS, iOS
		Color color; // Android
		
		sl_bool flagOngoing; // Android
		sl_bool flagOnlyAlertOnce; // Android
		sl_int32 priority; // Android(from -2 to 2)

		sl_bool flagActionButton;
		String actionButtonTitle; // macOS(old SDK)
		String closeButtonTitle; // macOS(old SDK)
		HashMap<String, String> additionalActions; // macOS(old SDK), [action, title] pairs
		sl_bool flagReplyButton;
		String responsePlaceholder; // macOS(old SDK)
		
		// Trigger
		Time deliveryTime;
		// In Seconds
		double deliveryInterval;
		sl_bool flagRepeat;
		
		// Receiving
		String action;
		String response;
		Time actualDeliveryTime; // iOS, macOS
		
		// Remove after the action is processed
		sl_bool flagRemove;
		
	public:
		UserNotificationMessage();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(UserNotificationMessage)
		
	};

	class UserNotificationAuthorizationOptions
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(UserNotificationAuthorizationOptions, value)
		
		// same as UNAuthorizationOptions
		enum {
			Badge = 1,
			Sound = (1<<1),
			Alert = (1<<2),
			CarPlay = (1<<3),
			CriticalAlert = (1<<4),
			ProvidesAppNotificationSettings = (1<<5),
			Provisional = (1<<6),
			Announcement = (1<<7)
		};
	};

	class SLIB_EXPORT UserNotification : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		UserNotification();
		
		~UserNotification();
		
	public:
		static void start();
		
		static Ref<UserNotification> add(const UserNotificationMessage& message);
		
		static void removePendingNotification(const String& identifier);
		
		static void removePendingNotification(sl_uint32 _id);
		
		static void removeAllPendingNotifications();
		
		static void removeDeliveredNotification(const String& identifier);
		
		static void removeDeliveredNotification(sl_uint32 _id);
		
		static void removeAllDeliveredNotifications();
		
		static void requestAuthorization(const UserNotificationAuthorizationOptions& options, const Function<void(sl_bool flagGranted)>& callback);

		static void addOnClickMessage(const Function<void(UserNotificationMessage&)>& callback);
		
		static void removeOnClickMessage(const Function<void(UserNotificationMessage&)>& callback);
		
		static void dispatchClickMessage(UserNotificationMessage& message);
		
		static void addOnPresentMessage(const Function<void(UserNotificationMessage&)>& callback);
		
		static void removeOnPresentMessage(const Function<void(UserNotificationMessage&)>& callback);
		
		static void dispatchPresentMessage(UserNotificationMessage& message);

	public:
		virtual void cancelPending() = 0;
		
		virtual void removeFromDeliveredList() = 0;
		
	private:
		static void startInternal();
		
	};

	class SLIB_EXPORT PushNotificationClient : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		PushNotificationClient();
		
		~PushNotificationClient();
		
	public:
		String getDeviceToken();
		
		void addOnRefreshToken(const Function<void(String)>& callback);
		
		void removeOnRefreshToken(const Function<void(String)>& callback);

		void addOnReceiveMessage(const Function<void(PushNotificationMessage&)>& callback);
		
		void removeOnReceiveMessage(const Function<void(PushNotificationMessage&)>& callback);
		
		void start();
		
	public:
		void dispatchRefreshToken(const String& deviceToken);
		
		void dispatchReceiveMessage(PushNotificationMessage& message);
		
	protected:
		virtual void onStart();
		
	protected:
		sl_bool m_flagStarted;
		AtomicString m_deviceToken;
		AtomicFunction<void(String)> m_onRefreshToken;
		AtomicFunction<void(PushNotificationMessage&)> m_onReceiveMessage;

	};
	
	// Apple Push Notification service
	class SLIB_EXPORT APNs : public PushNotificationClient
	{
		SLIB_DECLARE_OBJECT
		
	public:
		APNs();
		
		~APNs();
		
	protected:
		void onStart() override;
		
	public:
		static Ref<APNs> getInstance();
		
	};

	// Firebase Cloud Messaging
	class SLIB_EXPORT FCM : public PushNotificationClient
	{
		SLIB_DECLARE_OBJECT
		
	public:
		FCM();
		
		~FCM();
		
	public:
		void onStart() override;
		
	public:		
		static Ref<FCM> getInstance();
		
	};
	
}

#endif
