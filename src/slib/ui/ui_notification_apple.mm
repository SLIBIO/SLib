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

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "slib/ui/notification.h"

#include "slib/core/json.h"
#include "slib/core/safe_static.h"

#include "slib/ui/platform.h"

#include "../resources.h"

#define SUPPORT_USER_NOTIFICATIONS_FRAMEWORK

#ifdef SLIB_PLATFORM_IS_IOS
#	ifndef SUPPORT_USER_NOTIFICATIONS_FRAMEWORK
#		define SUPPORT_USER_NOTIFICATIONS_FRAMEWORK
#	endif
#endif

#ifdef SUPPORT_USER_NOTIFICATIONS_FRAMEWORK
#	import <UserNotifications/UserNotifications.h>
#endif

#if defined(SLIB_PLATFORM_IS_MACOS)
#	if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
#		define CHECK_UN_API @available(macOS 10.14, *)
#		define DEFINE_UN_API API_AVAILABLE(macos(10.14))
#	else
#		define CHECK_UN_API 0
#		define DEFINE_UN_API
#	endif
#else
#	define CHECK_UN_API 1
#	define DEFINE_UN_API
#endif

#if defined(SLIB_PLATFORM_IS_MACOS)
@interface SLIBNSUserNotificationCenterDelegate : NSObject<NSUserNotificationCenterDelegate>
{
}
@end
#endif

#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
DEFINE_UN_API
@interface SLIBUNUserNotificationCenterDelegate : NSObject<UNUserNotificationCenterDelegate>
{
}
@end
#endif

namespace slib
{
	
	namespace priv
	{
		namespace ui_notification_apple
		{
		
			class StaticContext
			{
			public:
#if defined(SLIB_PLATFORM_IS_MACOS)
				NSUserNotificationCenter* centerNS;
				SLIBNSUserNotificationCenterDelegate* delegateNS;
#endif
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
				DEFINE_UN_API UNUserNotificationCenter* centerUN;
				DEFINE_UN_API SLIBUNUserNotificationCenterDelegate* delegateUN;
#endif

			public:
				StaticContext()
				{
					if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
						centerUN = [UNUserNotificationCenter currentNotificationCenter];
						delegateUN = [SLIBUNUserNotificationCenterDelegate new];
						centerUN.delegate = delegateUN;
						NSMutableSet* categories = [NSMutableSet new];
						{
							UNNotificationAction* actionOpen = [UNNotificationAction actionWithIdentifier:@"open" title:Apple::getNSStringFromString(::slib::string::open::get()) options:0];
							UNNotificationCategory* category = [UNNotificationCategory categoryWithIdentifier:@"default" actions:@[actionOpen] intentIdentifiers:@[] options:UNNotificationCategoryOptionNone];
							[categories addObject:category];
						}
						{
							UNTextInputNotificationAction* actionReply = [UNTextInputNotificationAction actionWithIdentifier:@"reply" title:Apple::getNSStringFromString(::slib::string::reply::get()) options:0];
							UNNotificationCategory* category = [UNNotificationCategory categoryWithIdentifier:@"reply" actions:@[actionReply] intentIdentifiers:@[] options:UNNotificationCategoryOptionNone];
							[categories addObject:category];
						}
						[centerUN setNotificationCategories:categories];
#endif
					} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
						centerNS = [NSUserNotificationCenter defaultUserNotificationCenter];
						delegateNS = [SLIBNSUserNotificationCenterDelegate new];
						centerNS.delegate = delegateNS;
#endif
					}
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)
			
			class UserNotificationImpl : public UserNotification
			{
			public:
#if defined(SLIB_PLATFORM_IS_MACOS)
				NSUserNotification* m_object;
#endif
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
				NSString* m_identifier;
#endif

			public:
				static Ref<UserNotificationImpl> create(const UserNotificationMessage& message)
				{
#if defined(SLIB_PLATFORM_IS_MACOS)
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
					if (CHECK_UN_API) {
						return createUN(message);
					}
#endif
					return createNS(message);
#else
					return createUN(message);
#endif
				}
				
				static String getIdentifier(const UserNotificationMessage& message)
				{
					if (message.identifier.isNotNull()) {
						return message.identifier;
					}
					return String::fromUint32(message.id);
				}

#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
				DEFINE_UN_API
				static Ref<UserNotificationImpl> createUN(const UserNotificationMessage& message)
				{
					StaticContext* context = GetStaticContext();
					Ref<UserNotificationImpl> ret = new UserNotificationImpl;
					if (ret.isNotNull()) {
						UNMutableNotificationContent* content = [UNMutableNotificationContent new];
						if (message.title.isNotEmpty()) {
							content.title = Apple::getNSStringFromString(message.title);
						}
						if (message.content.isNotEmpty()) {
							content.body = Apple::getNSStringFromString(message.content);
						}
						if (message.data.isNotUndefined()) {
							content.userInfo = @{@"data": Apple::getNSStringFromString(message.data.toJsonString())};
						}
						if (message.subTitle.isNotEmpty()) {
							content.subtitle = Apple::getNSStringFromString(message.subTitle);
						}
						if (message.categoryIdentifier.isNotEmpty()) {
							content.categoryIdentifier = Apple::getNSStringFromString(message.categoryIdentifier);
						} else {
							if (message.flagActionButton) {
								content.categoryIdentifier = @"default";
							} else if (message.flagReplyButton) {
								content.categoryIdentifier = @"reply";
							}
						}
						if (message.threadIdentifier.isNotEmpty()) {
							content.threadIdentifier = Apple::getNSStringFromString(message.threadIdentifier);
						}
						if (message.targetContentIdentifier.isNotEmpty()) {
#ifdef SLIB_UI_IS_IOS
							if (@available(iOS 13.0, *)) {
								content.targetContentIdentifier = Apple::getNSStringFromString(message.targetContentIdentifier);
							}
#else
							content.targetContentIdentifier = Apple::getNSStringFromString(message.targetContentIdentifier);
#endif
						}
						if (message.badge.isNotNull()) {
							content.badge = @((NSInteger)(message.badge.get()));
						}
						if (message.attachments.isNotEmpty()) {
							NSMutableArray* arr = [NSMutableArray new];
							ListElements<UserNotificationAttachment> list(message.attachments);
							for (sl_size i = 0; i < list.count; i++) {
								NSString* _id = Apple::getNSStringFromString(list[i].identifier);
								NSURL* url = [NSURL URLWithString:Apple::getNSStringFromString(list[i].url)];
								UNNotificationAttachment* attach = [UNNotificationAttachment attachmentWithIdentifier:_id URL:url options:nil error:nil];
								if (attach != nil) {
									[arr addObject:attach];
								}
							}
							content.attachments = arr;
						}
#ifdef SLIB_UI_IS_IOS
						if (message.launchImageName.isNotEmpty()) {
							content.launchImageName = Apple::getNSStringFromString(message.launchImageName);
						}
#endif
						if (message.flagSound) {
							if (message.soundName.isNotEmpty()) {
								content.sound = [UNNotificationSound soundNamed:(Apple::getNSStringFromString(message.soundName))];
							} else {
								content.sound = [UNNotificationSound defaultSound];
							}
						} else {
							content.sound = nil;
						}
					
						UNNotificationTrigger* trigger = nil;
						if (message.deliveryTime.isNotZero()) {
							NSDateComponents* comps = [NSDateComponents new];
							comps.year = message.deliveryTime.getYear();
							comps.month = message.deliveryTime.getMonth();
							comps.day = message.deliveryTime.getDay();
							comps.hour = message.deliveryTime.getHour();
							comps.minute = message.deliveryTime.getMinute();
							comps.second = message.deliveryTime.getSecond();
							trigger = [UNCalendarNotificationTrigger triggerWithDateMatchingComponents:comps repeats:NO];
						} else if (message.deliveryInterval > 0.1) {
							double interval = message.deliveryInterval;
							if (message.flagRepeat) {
								if (interval < 60) {
									interval = 60;
								}
							}
							trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:interval repeats:message.flagRepeat];
						}
						
						NSString* _id = Apple::getNSStringFromString(getIdentifier(message), @"");
						UNNotificationRequest* notification = [UNNotificationRequest requestWithIdentifier:_id content:content trigger:trigger];
						[context->centerUN addNotificationRequest:notification withCompletionHandler:nil];
					
						ret->m_identifier = _id;
						return ret;
					}
					return sl_null;
				}
#endif
		
#if defined(SLIB_PLATFORM_IS_MACOS)
				static Ref<UserNotificationImpl> createNS(const UserNotificationMessage& message)
				{
					StaticContext* context = GetStaticContext();
					Ref<UserNotificationImpl> ret = new UserNotificationImpl;
					if (ret.isNotNull()) {
						NSUserNotification* notification = [NSUserNotification new];
								
						notification.identifier = Apple::getNSStringFromString(getIdentifier(message), @"");
						if (message.title.isNotEmpty()) {
							notification.title = Apple::getNSStringFromString(message.title);
						}
						if (message.content.isNotEmpty()) {
							notification.informativeText = Apple::getNSStringFromString(message.content);
						}
						if (message.data.isNotUndefined()) {
							notification.userInfo = @{@"data": Apple::getNSStringFromString(message.data.toJsonString())};
						}
				
						if (message.subTitle.isNotEmpty()) {
							notification.subtitle = Apple::getNSStringFromString(message.subTitle);
						}
						notification.contentImage = GraphicsPlatform::getNSImage(message.contentImage);
						if (message.flagSound) {
							if (message.soundName.isNotEmpty()) {
								notification.soundName = Apple::getNSStringFromString(message.soundName);
							} else {
								notification.soundName = NSUserNotificationDefaultSoundName;
							}
						} else {
							notification.soundName = nil;
						}

						notification.hasActionButton = message.flagActionButton;
						notification.hasReplyButton = message.flagReplyButton;
						if (message.actionButtonTitle.isNotEmpty()) {
							notification.actionButtonTitle = Apple::getNSStringFromString(message.actionButtonTitle);
						}
						if (message.closeButtonTitle.isNotEmpty()) {
							notification.otherButtonTitle = Apple::getNSStringFromString(message.closeButtonTitle);
						}
						if (message.additionalActions.isNotEmpty()) {
							NSMutableArray* arr = [NSMutableArray new];
							for (auto& pair : message.additionalActions) {
								NSString* _id = Apple::getNSStringFromString(pair.key);
								NSString* title = Apple::getNSStringFromString(pair.value);
								NSUserNotificationAction* action = [NSUserNotificationAction actionWithIdentifier:_id title:title];
								[arr addObject:action];
							}
							notification.additionalActions = arr;
							@try {
								[notification setValue:@(YES) forKey:@"_alwaysShowAlternateActionMenu"];
							} @catch(NSException*) {
							}
						}
						if (message.responsePlaceholder.isNotEmpty()) {
							notification.responsePlaceholder = Apple::getNSStringFromString(message.responsePlaceholder);
						}
				
						if (message.deliveryTime.isNotZero()) {
							notification.deliveryDate = Apple::getNSDateFromTime(message.deliveryTime);
						}
						if (message.deliveryInterval > 0.1) {
							if (message.flagRepeat) {
								NSDateComponents* comps = [NSDateComponents new];
								NSInteger interval = (NSInteger)(message.deliveryInterval);
								if (interval < 60) {
									interval = 60;
								}
								comps.second = interval;
								notification.deliveryRepeatInterval = comps;
							} else {
								notification.deliveryDate = Apple::getNSDateFromTime(Time::now() + Time::withSecondsf(message.deliveryInterval));
							}
						}
				
						ret->m_object = notification;
						[context->centerNS scheduleNotification:notification];
				
						return ret;
					}
					return sl_null;
				}
#endif
	
			public:
				void cancelPending() override
				{
					StaticContext* context = GetStaticContext();
					if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
						if (m_identifier != nil) {
							[context->centerUN removePendingNotificationRequestsWithIdentifiers:@[m_identifier]];
						}
#endif
					} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
						if (m_object != nil) {
							[context->centerNS removeScheduledNotification:m_object];
						}
#endif
					}
				}
		
				void removeFromDeliveredList() override
				{
					StaticContext* context = GetStaticContext();
					if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
						if (m_identifier != nil) {
							[context->centerUN removeDeliveredNotificationsWithIdentifiers:@[m_identifier]];
						}
#endif
					} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
						if (m_object != nil) {
							[context->centerNS removeDeliveredNotification:m_object];
						}
#endif
					}
				}
				
#if defined(SLIB_PLATFORM_IS_MACOS)
				static sl_bool onMessage(NSUserNotification* notification, sl_bool flagPresent)
				{
					StaticContext* context = GetStaticContext();
					UserNotificationMessage message;
					message.identifier = Apple::getStringFromNSString(notification.identifier);
					message.id = message.identifier.parseUint32();
					message.title = Apple::getStringFromNSString(notification.title);
					message.content = Apple::getStringFromNSString(notification.informativeText);
					message.data = Json::parseJson(Apple::getStringFromNSString(notification.userInfo[@"data"]));
					message.action = Apple::getStringFromNSString(notification.additionalActivationAction.identifier);
					message.response = Apple::getStringFromNSString(notification.response.string);
					message.actualDeliveryTime = Apple::getTimeFromNSDate(notification.actualDeliveryDate);
					if (flagPresent) {
						message.flagRemove = sl_false;
						dispatchPresentMessage(message);
						return !(message.flagRemove);
					} else {
						message.flagRemove = sl_true;
						dispatchClickMessage(message);
						if (message.flagRemove) {
							[context->centerNS removeDeliveredNotification:notification];
						}
						return sl_true;
					}
				}
#endif
				
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
				DEFINE_UN_API
				static sl_bool onMessage(UNNotification* notification, NSString* action, NSString* input, sl_bool flagPresent)
				{
					UNNotificationRequest* request = notification.request;
					UNNotificationContent* content = request.content;
					StaticContext* context = GetStaticContext();
					UserNotificationMessage message;
					message.identifier = Apple::getStringFromNSString(request.identifier);
					message.id = message.identifier.parseUint32();
					message.title = Apple::getStringFromNSString(content.title);
					message.content = Apple::getStringFromNSString(content.body);
					message.data = Json::parseJson(Apple::getStringFromNSString(content.userInfo[@"data"]));
					message.action = Apple::getStringFromNSString(action);
					message.response = Apple::getStringFromNSString(input);
					message.actualDeliveryTime = Apple::getTimeFromNSDate(notification.date);
					if (flagPresent) {
						message.flagRemove = sl_false;
						dispatchPresentMessage(message);
						return !(message.flagRemove);
					} else {
						message.flagRemove = sl_true;
						dispatchClickMessage(message);
						if (message.flagRemove) {
							[context->centerUN removeDeliveredNotificationsWithIdentifiers:@[request.identifier]];
						}
						return sl_true;
					}
				}

				DEFINE_UN_API
				static void onClickMessage(UNNotificationResponse* response)
				{
					NSString* input = nil;
					if ([response isKindOfClass:[UNTextInputNotificationResponse class]]) {
						input = ((UNTextInputNotificationResponse*)response).userText;
					}
					onMessage(response.notification, response.actionIdentifier, input, sl_false);
				}
	
				DEFINE_UN_API
				static sl_bool onPresentMessage(UNNotification* notification)
				{
					return onMessage(notification, nil, nil, sl_true);
				}
#endif
			};

		}
	}

	using namespace priv::ui_notification_apple;

	void UserNotification::startInternal()
	{
		GetStaticContext();
		if (CHECK_UN_API) {
			return;
		}
#if defined(SLIB_PLATFORM_IS_MACOS)
		UIPlatform::registerDidFinishLaunchingCallback([](NSNotification* notification) {
			NSDictionary* userInfo = notification.userInfo;
			if (userInfo != nil) {
				id obj = [userInfo objectForKey:NSApplicationLaunchUserNotificationKey];
				if (obj != nil) {
					if ([obj isKindOfClass:[NSUserNotification class]]) {
						UserNotificationImpl::onMessage((NSUserNotification*)obj, sl_false);
					}
				}
			}
		});
#endif
	}

	Ref<UserNotification> UserNotification::add(const UserNotificationMessage& message)
	{
		return Ref<UserNotification>::from(UserNotificationImpl::create(message));
	}

	void UserNotification::requestAuthorization(const UserNotificationAuthorizationOptions& options, const Function<void(sl_bool flagGranted)>& _callback)
	{
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
		if (CHECK_UN_API) {
			StaticContext* context = GetStaticContext();
			Function<void(sl_bool flagGranted)> callback = _callback;
			[context->centerUN requestAuthorizationWithOptions:options.value completionHandler:^(BOOL granted, NSError* error) {
				callback(granted);
			}];
			return;
		}
#endif
		_callback(sl_true);
	}

	void UserNotification::removePendingNotification(const String& identifier)
	{
		StaticContext* context = GetStaticContext();
		NSString* _id = Apple::getNSStringFromString(identifier, @"");
		if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
			[context->centerUN removePendingNotificationRequestsWithIdentifiers:@[_id]];
#endif
		} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
			for (NSUserNotification* notification : [context->centerNS scheduledNotifications]) {
				NSString* v = notification.identifier;
				if (v == nil) {
					v = @"";
				}
				if ([v isEqualToString:_id]) {
					[context->centerNS removeScheduledNotification:notification];
					return;
				}
			}
#endif
		}
	}
	
	void UserNotification::removePendingNotification(sl_uint32 _id)
	{
		removePendingNotification(String::fromUint32(_id));
	}

	void UserNotification::removeAllPendingNotifications()
	{
		StaticContext* context = GetStaticContext();
		if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
			[context->centerUN removeAllPendingNotificationRequests];
#endif
		} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
			NSArray* arr = [[context->centerNS scheduledNotifications] copy];
			for (NSUserNotification* notification : arr) {
				[context->centerNS removeScheduledNotification:notification];
			}
#endif
		}
	}

	void UserNotification::removeDeliveredNotification(const String& identifier)
	{
		StaticContext* context = GetStaticContext();
		NSString* _id = Apple::getNSStringFromString(identifier, @"");
		if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
			[context->centerUN removeDeliveredNotificationsWithIdentifiers:@[_id]];
#endif
		} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
			for (NSUserNotification* notification : [context->centerNS deliveredNotifications]) {
				NSString* v = notification.identifier;
				if (v == nil) {
					v = @"";
				}
				if ([v isEqualToString:_id]) {
					[context->centerNS removeDeliveredNotification:notification];
					return;
				}
			}
#endif
		}
	}
	
	void UserNotification::removeDeliveredNotification(sl_uint32 _id)
	{
		removeDeliveredNotification(String::fromUint32(_id));
	}

	void UserNotification::removeAllDeliveredNotifications()
	{
		StaticContext* context = GetStaticContext();
		if (CHECK_UN_API) {
#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
			[context->centerUN removeAllDeliveredNotifications];
#endif
		} else {
#if defined(SLIB_PLATFORM_IS_MACOS)
			[context->centerNS removeAllDeliveredNotifications];
#endif
		}
	}

}

using namespace slib;
using namespace slib::priv::ui_notification_apple;

#if defined(SLIB_PLATFORM_IS_MACOS)

@implementation SLIBNSUserNotificationCenterDelegate

- (void)userNotificationCenter:(NSUserNotificationCenter*)center didActivateNotification:(NSUserNotification*)notification
{
	UserNotificationImpl::onMessage(notification, sl_false);
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
	return UserNotificationImpl::onMessage(notification, sl_true);
}

@end

#endif

#if defined(SUPPORT_USER_NOTIFICATIONS_FRAMEWORK)
@implementation SLIBUNUserNotificationCenterDelegate

- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)(void))completionHandler
{
	UserNotificationImpl::onClickMessage(response);
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler
{
	if (UserNotificationImpl::onPresentMessage(notification)) {
		completionHandler(0xFFFF);
	} else {
		completionHandler(0);
	}
}
@end
#endif

#endif
