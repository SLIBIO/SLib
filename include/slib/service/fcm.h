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

#ifndef CHECKHEADER_SLIB_SERVICE_FCM
#define CHECKHEADER_SLIB_SERVICE_FCM

#include "definition.h"

#include "push_notification.h"

namespace slib
{
	
	class FCM_ServiceSendResult
	{
	public:
		String message_id;
		String registration_id;
		String error;
		
	public:
		FCM_ServiceSendResult();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(FCM_ServiceSendResult)
		
		SLIB_DECLARE_JSON
		
	};

	class UrlRequest;

	class FCM_ServiceSendResponse
	{
	public:
		sl_bool flagSuccess;
		UrlRequest* request;
		
		String multicast_id;
		sl_uint32 success;
		sl_uint32 failure;
		List<FCM_ServiceSendResult> results;
		
	public:
		FCM_ServiceSendResponse();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(FCM_ServiceSendResponse)
		
		SLIB_DECLARE_JSON
		
	};

	class FCM_SendParam
	{
	public:
		String legacyServerKey;

		List<String> receiverDeviceTokens;
		String receiverDeviceToken;
		PushNotificationMessage message;
		
		Json customMessage;
		
		Function<void(FCM_ServiceSendResponse&)> callback;
		
	public:
		FCM_SendParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(FCM_SendParam)
		
	};

	class FCM_Service
	{
	public:
		static void sendNotification(const FCM_SendParam& param);
		
	};
	
}

#endif
