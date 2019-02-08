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

#include "../core/string.h"
#include "../core/json.h"
#include "../core/function.h"

namespace slib
{

	class SLIB_EXPORT PushNotificationMessage
	{
	public:
		String title;
		String content;

		Json data; // custom message
		
		// Sending Params
		sl_int32 badge;
		String sound;
		
		// Received Params
		sl_bool flagClicked;
		
	public:
		PushNotificationMessage();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(PushNotificationMessage)

	};
	
	class SLIB_EXPORT PushNotification
	{
	public:
		static String getDeviceToken();
		
		static void addTokenRefreshCallback(const Function<void(String)>& callback);
		
		static void removeTokenRefreshCallback(const Function<void(String)>& callback);

		static void addNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback);
		
		static void removeNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback);

	private:
		static void _init();
		
		static void _doInit();
		
	public:
		static void _onRefreshToken(const String& token);
		
		static void _onNotificationReceived(PushNotificationMessage& message);
		
	};

}

#endif
