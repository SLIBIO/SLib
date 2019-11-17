/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/ui/notification.h"

#include "slib/core/json.h"

#include "slib/ui/platform.h"

namespace slib
{
	
	namespace priv
	{
		namespace ui_notification_android
		{
			
			SLIB_JNI_BEGIN_CLASS(JUserNotificationMessage, "slib/platform/android/ui/notification/UserNotificationMessage")
				SLIB_JNI_NEW(init, "()V");
				
				SLIB_JNI_INT_FIELD(id)
				SLIB_JNI_STRING_FIELD(title)
				SLIB_JNI_STRING_FIELD(content)
				SLIB_JNI_STRING_FIELD(data)

				SLIB_JNI_STRING_FIELD(subText)
				SLIB_JNI_STRING_FIELD(contentInfo)
				SLIB_JNI_STRING_FIELD(ticker)
				SLIB_JNI_STRING_FIELD(channelId)
				SLIB_JNI_STRING_FIELD(category)
				SLIB_JNI_STRING_FIELD(groupKey)
				SLIB_JNI_BOOLEAN_FIELD(flagGroupSummary)
				SLIB_JNI_STRING_FIELD(sortKey)

				SLIB_JNI_STRING_FIELD(iconName)
				SLIB_JNI_INT_FIELD(color)

				SLIB_JNI_BOOLEAN_FIELD(flagOngoing)
				SLIB_JNI_BOOLEAN_FIELD(flagOnlyAlertOnce)
				SLIB_JNI_INT_FIELD(priority)

				SLIB_JNI_LONG_FIELD(deliveryTime)
				SLIB_JNI_DOUBLE_FIELD(deliveryInterval)
				SLIB_JNI_BOOLEAN_FIELD(flagRepeat)

				SLIB_JNI_STRING_FIELD(action)
				SLIB_JNI_STRING_FIELD(response)
				SLIB_JNI_LONG_FIELD(actualDeliveryTime)
			SLIB_JNI_END_CLASS

			void OnClickMessage(JNIEnv* env, jobject _this, jobject jmsg);

			SLIB_JNI_BEGIN_CLASS(JUserNotification, "slib/platform/android/ui/notification/UserNotification")
				SLIB_JNI_STATIC_METHOD(initialize, "initialize", "(Landroid/app/Activity;)V");
				SLIB_JNI_STATIC_METHOD(add, "add", "(Landroid/app/Activity;Lslib/platform/android/ui/notification/UserNotificationMessage;)V");
				SLIB_JNI_STATIC_METHOD(cancel, "cancel", "(Landroid/app/Activity;I)V");
				SLIB_JNI_STATIC_METHOD(cancelAll, "cancelAll", "(Landroid/app/Activity;)V");
				SLIB_JNI_NATIVE(onClickMessage, "nativeOnClickMessage", "(Lslib/platform/android/ui/notification/UserNotificationMessage;)V", OnClickMessage);
			SLIB_JNI_END_CLASS
			
			class UserNotificationImpl : public UserNotification
			{
			public:
				sl_uint32 m_id;

			public:
				static Ref<UserNotificationImpl> create(const UserNotificationMessage& message)
				{
					jobject jactivity = Android::getCurrentActivity();
					if (!jactivity) {
						return sl_null;
					}

					JniLocal<jobject> jmsg = JUserNotificationMessage::init.newObject(sl_null);
					if (jmsg.isNotNull()) {

						Ref<UserNotificationImpl> ret = new UserNotificationImpl;
						if (ret.isNotNull()) {

							sl_uint32 id = message.id;
							if (message.identifier.isNotEmpty()) {
								message.identifier.parseUint32(10, &id);
							}							
							JUserNotificationMessage::id.set(jmsg.get(), (jint)id);

							if (message.title.isNotEmpty()) {
								JUserNotificationMessage::title.set(jmsg.get(), message.title);
							}
							if (message.content.isNotEmpty()) {
								JUserNotificationMessage::content.set(jmsg.get(), message.content);
							}
							if (message.data.isNotUndefined()) {
								JUserNotificationMessage::data.set(jmsg.get(), message.data.toJsonString());
							}

							if (message.subText.isNotEmpty()) {
								JUserNotificationMessage::subText.set(jmsg.get(), message.subText);
							}
							if (message.contentInfo.isNotEmpty()) {
								JUserNotificationMessage::contentInfo.set(jmsg.get(), message.contentInfo);
							}
							if (message.ticker.isNotEmpty()) {
								JUserNotificationMessage::ticker.set(jmsg.get(), message.ticker);
							}
							if (message.channelId.isNotEmpty()) {
								JUserNotificationMessage::channelId.set(jmsg.get(), message.channelId);
							}
							if (message.category.isNotEmpty()) {
								JUserNotificationMessage::category.set(jmsg.get(), message.category);
							}
							if (message.groupKey.isNotEmpty()) {
								JUserNotificationMessage::groupKey.set(jmsg.get(), message.groupKey);
								JUserNotificationMessage::flagGroupSummary.set(jmsg.get(), message.flagGroupSummary);
							}
							if (message.sortKey.isNotEmpty()) {
								JUserNotificationMessage::sortKey.set(jmsg.get(), message.sortKey);
							}

							if (message.iconName.isNotEmpty()) {
								JUserNotificationMessage::iconName.set(jmsg.get(), message.iconName);
							}
							if (message.color.isNotZero()) {
								JUserNotificationMessage::color.set(jmsg.get(), message.color.getARGB());
							}

							JUserNotificationMessage::flagOngoing.set(jmsg.get(), message.flagOngoing);
							JUserNotificationMessage::flagOnlyAlertOnce.set(jmsg.get(), message.flagOnlyAlertOnce);
							JUserNotificationMessage::priority.set(jmsg.get(), (jint)(message.priority));

							if (message.deliveryTime.isNotZero()) {
								JUserNotificationMessage::deliveryTime.set(jmsg.get(), (jlong)(message.deliveryTime.getMillisecondsCount()));
							}
							if (message.deliveryInterval > 0.5) {
								JUserNotificationMessage::deliveryInterval.set(jmsg.get(), (jdouble)(message.deliveryInterval));
								JUserNotificationMessage::flagRepeat.set(jmsg.get(), message.flagRepeat);
							}

							JUserNotification::add.call(sl_null, jactivity, jmsg.get());

							ret->m_id = id;

							return ret;
						}						
					}
					return sl_null;
				}

			public:
				void cancelPending() override
				{					
				}

				void removeFromDeliveredList() override
				{
					removeDeliveredNotification(m_id);
				}
				
			};

			void OnClickMessage(JNIEnv* env, jobject _this, jobject jmsg)
			{
				UserNotificationMessage message;
				message.id = (sl_uint32)(JUserNotificationMessage::id.get(jmsg));
				message.identifier = String::fromUint32(message.id);
				message.title = JUserNotificationMessage::title.get(jmsg);
				message.content = JUserNotificationMessage::content.get(jmsg);
				message.data = Json::parseJson(JUserNotificationMessage::data.get(jmsg));
				message.action = JUserNotificationMessage::action.get(jmsg);
				message.response = JUserNotificationMessage::response.get(jmsg);
				message.actualDeliveryTime = Time::withMilliseconds((sl_int64)(JUserNotificationMessage::actualDeliveryTime.get(jmsg)));
				UserNotification::dispatchClickMessage(message);
			}

		}
	}

	using namespace priv::ui_notification_android;

	void UserNotification::startInternal()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JUserNotification::initialize.call(sl_null, jactivity);
		}
	}

	Ref<UserNotification> UserNotification::add(const UserNotificationMessage& message)
	{
		return Ref<UserNotification>::from(UserNotificationImpl::create(message));
	}

	void UserNotification::removePendingNotification(const String& identifier)
	{
	}
	
	void UserNotification::removePendingNotification(sl_uint32 id)
	{
	}
	
	void UserNotification::removeAllPendingNotifications()
	{
	}

	void UserNotification::removeDeliveredNotification(const String& identifier)
	{
		sl_uint32 id;
		if (identifier.parseUint32(10, &id)) {
			removeDeliveredNotification(id);
		}
	}
	
	void UserNotification::removeDeliveredNotification(sl_uint32 id)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JUserNotification::cancel.call(sl_null, jactivity, (jint)id);
		}
	}
	
	void UserNotification::removeAllDeliveredNotifications()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JUserNotification::cancelAll.call(sl_null, jactivity);
		}
	}

}

#endif
