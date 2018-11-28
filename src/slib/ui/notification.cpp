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

	PushNotificationMessage::PushNotificationMessage()
	{
	}

	PushNotificationMessage::~PushNotificationMessage()
	{
	}

	SLIB_SAFE_STATIC_GETTER(String, _getDeviceToken)
	SLIB_SAFE_STATIC_GETTER(Function<void (String)>, _getTokenRefreshCallback)
	SLIB_SAFE_STATIC_GETTER(Function<void(PushNotificationMessage&)>, _getNotificationReceivedCallback)
	
	String PushNotification::getDeviceToken()
	{
		String* token = _getDeviceToken();
		if (token) {
			return *token;
		}
		return String::null();
	}
	
	void PushNotification::setTokenRefreshCallback(const Function<void(String)>& callback)
	{
		Function<void (String)>* _refreshCallback = _getTokenRefreshCallback();
		if (_refreshCallback) {
			*_refreshCallback = callback;
		}
		
		_initToken();
	}
	
	void PushNotification::setNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		Function<void(PushNotificationMessage&)>* _receivedCallback = _getNotificationReceivedCallback();
		if (_receivedCallback) {
			*_receivedCallback = callback;
		}
	}
	
	Function<void(String)> PushNotification::getTokenRefreshCallback()
	{
		Function<void (String)>* callback = _getTokenRefreshCallback();
		if (callback) {
			return *callback;
		}
		return sl_null;
	}
	
	Function<void(PushNotificationMessage&)> PushNotification::getNotificationReceivedCallback()
	{
		Function<void(PushNotificationMessage&)>* callback = _getNotificationReceivedCallback();
		if (callback) {
			return *callback;
		}
		return sl_null;
	}

}
