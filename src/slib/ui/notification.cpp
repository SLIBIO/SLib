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

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(PushNotificationMessage)
	
	PushNotificationMessage::PushNotificationMessage()
	{
		badge = -1;
		flagClicked = sl_false;
		flagBackground = sl_false;
	}
	
	
	SLIB_DEFINE_OBJECT(PushNotificationService, Object)
	
	PushNotificationService::PushNotificationService()
	{
		m_flagStarted = sl_false;
		m_listTokenRefreshCallbacks.create();
		m_listNotificationReceivedCallbacks.create();
	}
	
	PushNotificationService::~PushNotificationService()
	{
	}
	
	String PushNotificationService::getDeviceToken()
	{
		return m_deviceToken;
	}
	
	void PushNotificationService::addTokenRefreshCallback(const Function<void(String)>& callback)
	{
		m_listTokenRefreshCallbacks.add(callback);
	}
	
	void PushNotificationService::removeTokenRefreshCallback(const Function<void(String)>& callback)
	{
		m_listTokenRefreshCallbacks.remove(callback);
	}
	
	void PushNotificationService::addNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_listNotificationReceivedCallbacks.add(callback);
	}
	
	void PushNotificationService::removeNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_listNotificationReceivedCallbacks.remove(callback);
	}
	
	void PushNotificationService::start()
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
	
	void PushNotificationService::dispatchTokenRefresh(const String& token)
	{
		m_deviceToken = token;
		for (auto& callback : m_listTokenRefreshCallbacks) {
			if (callback.isNotNull()) {
				callback(token);
			}
		}
	}
	
	void PushNotificationService::dispatchNotificationReceived(PushNotificationMessage& message)
	{
		for (auto& callback : m_listNotificationReceivedCallbacks) {
			if (callback.isNotNull()) {
				callback(message);
			}
		}
	}
	
	void PushNotificationService::onStart()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(APNs, PushNotificationService)
	
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
	
	
	SLIB_DEFINE_OBJECT(FCM, PushNotificationService)
	
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

