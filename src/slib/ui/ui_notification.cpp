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

#include "slib/ui/notification.h"

#include "slib/core/json.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace ui_notification
		{
		
			class StaticContext
			{
			public:
				Mutex lock;
				
				sl_bool flagStarted;
				
				AtomicFunction<void(UserNotificationMessage&)> onClickMessage;
				AtomicFunction<void(UserNotificationMessage&)> onPresentMessage;
				
				List<UserNotificationMessage> messagesClicked;
				List<UserNotificationMessage> messagesPresented;

			public:
				StaticContext()
				{
					flagStarted = sl_false;
				}
				
			public:
				void dispatchClickMessage(UserNotificationMessage& message)
				{
					MutexLocker locker(&lock);
					if (flagStarted) {
						locker.unlock();
						onClickMessage(message);
					} else {
						messagesClicked.add_NoLock(message);
						message.flagRemove = sl_false;
					}
				}
				
				void dispatchPresentMessage(UserNotificationMessage& message)
				{
					MutexLocker locker(&lock);
					if (flagStarted) {
						locker.unlock();
						onPresentMessage(message);
					} else {
						messagesPresented.add_NoLock(message);
						message.flagRemove = sl_false;
					}
				}
				
				void dispatchQueuedMessages()
				{
					{
						ListElements<UserNotificationMessage> messages(messagesPresented);
						for (sl_size i = 0; i < messages.count; i++) {
							messages[i].flagRemove = sl_false;
							onPresentMessage(messages[i]);
							if (messages[i].flagRemove) {
								UserNotification::removeDeliveredNotification(messages[i].identifier);
							}
						}
					}
					{
						ListElements<UserNotificationMessage> messages(messagesClicked);
						for (sl_size i = 0; i < messages.count; i++) {
							messages[i].flagRemove = sl_false;
							onClickMessage(messages[i]);
							if (messages[i].flagRemove) {
								UserNotification::removeDeliveredNotification(messages[i].identifier);
							}
						}
					}
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
		
		}
	}
	
	using namespace priv::ui_notification;

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(UserNotificationAttachment)

	UserNotificationAttachment::UserNotificationAttachment()
	{
	}

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(UserNotificationMessage)

	UserNotificationMessage::UserNotificationMessage()
	{
		flagGroupSummary = sl_false;
		
		flagSound = sl_true;
		color.setZero();

		flagActionButton = sl_false;
		flagReplyButton = sl_false;
		flagOngoing = sl_false;
		flagOnlyAlertOnce = sl_false;
		priority = 0;

		deliveryTime.setZero();
		deliveryInterval = 0;
		flagRepeat = sl_false;

		actualDeliveryTime.setZero();

		flagRemove = sl_true;
	}

	SLIB_DEFINE_OBJECT(UserNotification, Object)

	UserNotification::UserNotification()
	{
	}

	UserNotification::~UserNotification()
	{		
	}

#if defined(SLIB_PLATFORM_IS_APPLE) || defined(SLIB_PLATFORM_IS_ANDROID)
	void UserNotification::start()
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			MutexLocker locker(&(context->lock));
			if (context->flagStarted) {
				return;
			}
			context->flagStarted = sl_true;
			locker.unlock();
			startInternal();
			context->dispatchQueuedMessages();
		}
	}
#else
	void UserNotification::start()
	{
	}

	void UserNotification::startInternal()
	{
	}

	Ref<UserNotification> UserNotification::add(const UserNotificationMessage& param)
	{
		return sl_null;
	}

	void UserNotification::removePendingNotification(const String& identifier)
	{
	}
	
	void UserNotification::removePendingNotification(sl_uint32 _id)
	{
	}

	void UserNotification::removeAllPendingNotifications()
	{
	}
	
	void UserNotification::removeDeliveredNotification(const String& identifier)
	{
	}
	
	void UserNotification::removeDeliveredNotification(sl_uint32 _id)
	{
	}

	void UserNotification::removeAllDeliveredNotifications()
	{
	}
#endif

#if !defined(SLIB_PLATFORM_IS_APPLE)
	void UserNotification::requestAuthorization(const UserNotificationAuthorizationOptions& options, const Function<void(sl_bool flagGranted)>& callback)
	{
		callback(sl_true);
	}
#endif

	void UserNotification::addOnClickMessage(const Function<void(UserNotificationMessage&)>& callback)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->onClickMessage.add(callback);
		}
	}

	void UserNotification::removeOnClickMessage(const Function<void(UserNotificationMessage&)>& callback)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->onClickMessage.remove(callback);
		}
	}

	void UserNotification::dispatchClickMessage(UserNotificationMessage& message)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->dispatchClickMessage(message);
		}
	}

	void UserNotification::addOnPresentMessage(const Function<void(UserNotificationMessage&)>& callback)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->onPresentMessage.add(callback);
		}
	}

	void UserNotification::removeOnPresentMessage(const Function<void(UserNotificationMessage&)>& callback)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->onPresentMessage.remove(callback);
		}
	}

	void UserNotification::dispatchPresentMessage(UserNotificationMessage& message)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			context->dispatchPresentMessage(message);
		}
	}


	SLIB_DEFINE_OBJECT(PushNotificationClient, Object)
	
	PushNotificationClient::PushNotificationClient()
	{
		m_flagStarted = sl_false;
	}
	
	PushNotificationClient::~PushNotificationClient()
	{
	}
	
	String PushNotificationClient::getDeviceToken()
	{
		return m_deviceToken;
	}
	
	void PushNotificationClient::addOnRefreshToken(const Function<void(String)>& callback)
	{
		m_onRefreshToken.add(callback);
	}
	
	void PushNotificationClient::removeOnRefreshToken(const Function<void(String)>& callback)
	{
		m_onRefreshToken.remove(callback);
	}
	
	void PushNotificationClient::addOnReceiveMessage(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_onReceiveMessage.add(callback);
	}
	
	void PushNotificationClient::removeOnReceiveMessage(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_onReceiveMessage.remove(callback);
	}
	
	void PushNotificationClient::start()
	{
		{
			ObjectLocker lock(this);
			if (m_flagStarted) {
				return;
			}
			m_flagStarted = sl_true;
		}
		onStart();
	}
	
	void PushNotificationClient::dispatchRefreshToken(const String& token)
	{
		m_deviceToken = token;
		m_onRefreshToken(token);
	}
	
	void PushNotificationClient::dispatchReceiveMessage(PushNotificationMessage& message)
	{
		m_onReceiveMessage(message);
	}
	
	void PushNotificationClient::onStart()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(APNs, PushNotificationClient)
	
	APNs::APNs()
	{
	}
	
	APNs::~APNs()
	{
	}
	
#if !defined(SLIB_UI_IS_IOS)
	Ref<APNs> APNs::getInstance()
	{
		return sl_null;
	}

	void APNs::onStart()
	{
	}
#endif
	
	
	SLIB_DEFINE_OBJECT(FCM, PushNotificationClient)
	
	FCM::FCM()
	{
	}
	
	FCM::~FCM()
	{
	}
	
#if !defined(SLIB_UI_IS_ANDROID)
	Ref<FCM> FCM::getInstance()
	{
		return sl_null;
	}

	void FCM::onStart()
	{
	}
#endif
	
}

