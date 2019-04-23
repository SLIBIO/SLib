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
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PushNotificationMessage)
	
	PushNotificationMessage::PushNotificationMessage()
	{
		badge = -1;
		flagClicked = sl_false;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicString, _g_slib_ui_notification_token);

	String PushNotification::getDeviceToken()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_token)) {
			return sl_null;
		}
		return _g_slib_ui_notification_token;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList<Function<void(String)>>, _g_slib_ui_notification_listTokenRefreshCallback);

	void PushNotification::addTokenRefreshCallback(const Function<void(String)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_tokenRefreshCallback)) {
			return;
		}
		_g_slib_ui_notification_listTokenRefreshCallback.addIfNotExist(callback);
	}
	
	void PushNotification::removeTokenRefreshCallback(const Function<void(String)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_tokenRefreshCallback)) {
			return;
		}
		_g_slib_ui_notification_listTokenRefreshCallback.remove(callback);
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicList<Function<void(PushNotificationMessage&)>>, _g_slib_ui_notification_listNotificationReceivedCallback);

	void PushNotification::addNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_listNotificationReceivedCallback)) {
			return;
		}
		_g_slib_ui_notification_listNotificationReceivedCallback.addIfNotExist(callback);
	}
	
	void PushNotification::removeNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_listNotificationReceivedCallback)) {
			return;
		}
		_g_slib_ui_notification_listNotificationReceivedCallback.remove(callback);
	}

	SLIB_STATIC_ZERO_INITIALIZED(SpinLock, _g_slib_ui_notification_lock);
	
	void PushNotification::start()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_lock)) {
			return;
		}
		{
			SpinLocker lock(&_g_slib_ui_notification_lock);
			static sl_bool flagInitialized = sl_false;
			if (flagInitialized) {
				return;
			}
			flagInitialized = sl_true;
		}
		_doInit();
	}
	
	void PushNotification::_onRefreshToken(const String& token)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_token)) {
			return;
		}
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_listTokenRefreshCallback)) {
			return;
		}
		_g_slib_ui_notification_token = token;
		for (auto& callback : _g_slib_ui_notification_listTokenRefreshCallback) {
			if (callback.isNotNull()) {
				callback(token);
			}
		}
	}
	
	void PushNotification::_onNotificationReceived(PushNotificationMessage& message)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_slib_ui_notification_listNotificationReceivedCallback)) {
			return;
		}
		for (auto& callback : _g_slib_ui_notification_listNotificationReceivedCallback) {
			if (callback.isNotNull()) {
				callback(message);
			}
		}
	}
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void PushNotification::_doInit()
	{
	}
#endif
	
}

