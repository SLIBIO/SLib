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

#include "slib/service/fcm.h"

#include "slib/network/url_request.h"
#include "slib/network/http_common.h"
#include "slib/core/log.h"

namespace slib
{

	namespace priv
	{
		namespace fcm_service
		{
		
			SLIB_STATIC_STRING(g_str_to, "to")
			SLIB_STATIC_STRING(g_str_registration_ids, "registration_ids")
			SLIB_STATIC_STRING(g_str_data, "data")
			SLIB_STATIC_STRING(g_str_notification, "notification")
			SLIB_STATIC_STRING(g_str_title, "title")
			SLIB_STATIC_STRING(g_str_body, "body")
			SLIB_STATIC_STRING(g_str_sound, "sound")
			SLIB_STATIC_STRING(g_str_badge, "badge")

		}
	}

	using namespace priv::fcm_service;


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(FCM_ServiceSendResult)

	SLIB_DEFINE_JSON_MEMBERS(FCM_ServiceSendResult, message_id, registration_id, error)

	FCM_ServiceSendResult::FCM_ServiceSendResult()
	{
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(FCM_ServiceSendResponse)

	SLIB_DEFINE_JSON_MEMBERS(FCM_ServiceSendResponse, multicast_id, success, failure, results)

	FCM_ServiceSendResponse::FCM_ServiceSendResponse()
	{
		flagSuccess = sl_false;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(FCM_SendParam)
	
	FCM_SendParam::FCM_SendParam()
	{
	}
	

	void FCM_Service::sendNotification(const FCM_SendParam& param)
	{
		HttpHeaderMap headers;
		headers.put_NoLock(HttpHeader::Authorization, String::format("key=%s", param.legacyServerKey));
		headers.put_NoLock(HttpHeader::ContentType, ContentType::Json);
		Json body;
		if (param.receiverDeviceTokens.isNotEmpty()) {
			if (param.receiverDeviceTokens.getCount() == 1) {
				body.putItem(g_str_to, param.receiverDeviceTokens.getValueAt(0));
			} else {
				body.putItem(g_str_registration_ids, param.receiverDeviceTokens);
			}
		} else if (param.receiverDeviceToken.isNotEmpty()) {
			body.putItem(g_str_to, param.receiverDeviceToken);
		}
		body.putItem(g_str_data, param.message.data);
		Json notification;
		notification.putItem(g_str_title, param.message.title);
		notification.putItem(g_str_body, param.message.content);
		notification.putItem(g_str_badge, param.message.badge);
		notification.putItem(g_str_sound, param.message.sound);
		body.putItem(g_str_notification, notification);
		if (param.customMessage.isJsonMap()) {
			for (auto& item : param.customMessage.getJsonMap()) {
				body.putItem(item.key, item.value);
			}
		}
		
		Function<void(FCM_ServiceSendResponse&)> callback = param.callback;
		SLIB_STATIC_STRING(url, "https://fcm.googleapis.com/fcm/send")
		UrlRequest::post(url, headers, body, [callback](UrlRequest* request) {
			if (callback.isNull()) {
				return;
			}
			FCM_ServiceSendResponse response;
			response.flagSuccess = sl_true;
			response.request = request;
			if (request->getResponseStatus() != HttpStatus::OK) {
				response.flagSuccess = sl_false;
			}
			Json json = request->getResponseContentAsJson();
			FromJson(json, response);
			if (response.failure) {
				response.flagSuccess = sl_false;
			}
			callback(response);
		});
	}
	
}
