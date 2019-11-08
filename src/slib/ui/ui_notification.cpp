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
	
	void PushNotificationClient::addTokenRefreshCallback(const Function<void(String)>& callback)
	{
		m_callbackTokenRefresh.add(callback);
	}
	
	void PushNotificationClient::removeTokenRefreshCallback(const Function<void(String)>& callback)
	{
		m_callbackTokenRefresh.remove(callback);
	}
	
	void PushNotificationClient::addNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_callbackNotificationReceived.add(callback);
	}
	
	void PushNotificationClient::removeNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		m_callbackNotificationReceived.remove(callback);
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
	
	void PushNotificationClient::dispatchTokenRefresh(const String& token)
	{
		m_deviceToken = token;
		m_callbackTokenRefresh(token);
	}
	
	void PushNotificationClient::dispatchNotificationReceived(PushNotificationMessage& message)
	{
		m_callbackNotificationReceived(message);
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

