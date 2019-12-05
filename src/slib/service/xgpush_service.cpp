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

#include "slib/service/xgpush.h"

#include "slib/network/url_request.h"
#include "slib/network/http_common.h"
#include "slib/crypto/base64.h"
#include "slib/core/log.h"

namespace slib
{

	namespace priv
	{
		namespace xgpush
		{
		
			SLIB_STATIC_STRING(g_str_title, "title")
			SLIB_STATIC_STRING(g_str_content, "content")
			SLIB_STATIC_STRING(g_str_body, "body")
			SLIB_STATIC_STRING(g_str_alert, "alert")
			SLIB_STATIC_STRING(g_str_aps, "aps")
			SLIB_STATIC_STRING(g_str_ios, "ios")
			SLIB_STATIC_STRING(g_str_badge_type, "badge_type")
			SLIB_STATIC_STRING(g_str_sound, "sound")
			SLIB_STATIC_STRING(g_str_custom, "custom")
			SLIB_STATIC_STRING(g_str_android, "android")
			SLIB_STATIC_STRING(g_str_ring, "ring")
			SLIB_STATIC_STRING(g_str_ring_raw, "ring_raw")
			SLIB_STATIC_STRING(g_str_custom_content, "custom_content")
			SLIB_STATIC_STRING(g_str_audience_type, "audience_type")
			SLIB_STATIC_STRING(g_str_token_list, "token_list")
			SLIB_STATIC_STRING(g_str_platform, "platform")
			SLIB_STATIC_STRING(g_str_environment, "environment")
			SLIB_STATIC_STRING(g_str_message_type, "message_type")
			SLIB_STATIC_STRING(g_str_message, "message")
			SLIB_STATIC_STRING(g_str_notify, "notify")
			SLIB_STATIC_STRING(g_str_token, "token")
			SLIB_STATIC_STRING(g_str_ret_code, "ret_code")
			SLIB_STATIC_STRING(g_str_err_msg, "err_msg")

		}
	}

	using namespace priv::xgpush;

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(XgPushSendParam)
	
	XgPushSendParam::XgPushSendParam()
	{
		platform = XgPushPlatform::All;
		environment = XgPushEnvironment::Production;
	}
	
	void XgPushService::sendNotification(const XgPushSendParam& param)
	{
		HttpHeaderMap headers;
		headers.put_NoLock(HttpHeader::Authorization, String::format("Basic %s", Base64::encode(String::format("%s:%s", param.appId, param.secretKey))));
		headers.put_NoLock(HttpHeader::ContentType, ContentType::Json);
		
		Json message;
		message.putItem(g_str_title, param.message.title);
		message.putItem(g_str_content, param.message.content);
		if (param.platform == XgPushPlatform::All || param.platform == XgPushPlatform::iOS) {
			Json alert;
			alert.putItem(g_str_title, param.message.title);
			alert.putItem(g_str_body, param.message.content);
			Json aps;
			aps.putItem(g_str_alert, alert);
			if (param.message.badge >= 0) {
				aps.putItem(g_str_badge_type, param.message.badge);
			}
			if (param.message.sound.isNotEmpty()) {
				aps.putItem(g_str_sound, param.message.sound);
			}
			Json ios;
			ios.putItem(g_str_aps, aps);
			if (param.message.data.isNotNull()) {
				ios.putItem(g_str_custom, param.message.data);
			}
			message.putItem(g_str_ios, ios);
		}
		if (param.platform == XgPushPlatform::All || param.platform == XgPushPlatform::Android) {
			Json android;
			android.putItem(g_str_ring, 1);
			if (param.message.sound.isNotEmpty()) {
				android.putItem(g_str_ring_raw, param.message.sound);
			}
			if (param.message.data.isNotNull()) {
				android.putItem(g_str_custom_content, param.message.data);
			}
			message.putItem(g_str_android, android);
		}
		if (param.customMessage.isJsonMap()) {
			for (auto& item : param.customMessage.getJsonMap()) {
				message.putItem(item.key, item.value);
			}
		}
		Json body;
		body.putItem(g_str_audience_type, param.receiverDeviceTokens.getCount() > 1 ? g_str_token_list : g_str_token);
		body.putItem(g_str_token_list, param.receiverDeviceTokens);
		body.putItem(g_str_platform, getPlatformString(param.platform));
		body.putItem(g_str_environment, getEnvironmentString(param.environment));
		body.putItem(g_str_message_type, g_str_notify);
		body.putItem(g_str_message, message);

		auto callback = param.callback;
		SLIB_STATIC_STRING(url, "https://openapi.xg.qq.com/v3/push/app")
		UrlRequest::post(url, headers, body, [callback](UrlRequest* request) {
			String response = request->getResponseContentAsString();
			if (request->isError()) {
				LogError("XgPush", "[Status] %d %s - Error: %s %s", (int)(request->getResponseStatus()), request->getResponseMessage(), request->getLastErrorMessage(), response);
				if (response.isEmpty()) {
					response = request->getLastErrorMessage();
				}
				callback(sl_false, response);
			} else {
				Json json = request->getResponseContentAsJson();
				if (json.isNull()) {
					callback(sl_false, request->getResponseContentAsString());
				} else {
					sl_uint32 ret_code = json[g_str_ret_code].getUint32();
					if (ret_code != 0) {
						callback(sl_false, json[g_str_err_msg].getString());
					} else {
						callback(sl_true, sl_null);
					}
				}
			}
		});
	}
	
	String XgPushService::getPlatformString(XgPushPlatform platform)
	{
		switch (platform) {
			case XgPushPlatform::All:
				SLIB_RETURN_STRING("all")
			case XgPushPlatform::Android:
				SLIB_RETURN_STRING("android")
			case XgPushPlatform::iOS:
				SLIB_RETURN_STRING("ios")
		}
		return sl_null;
	}
	
	String XgPushService::getEnvironmentString(XgPushEnvironment environment)
	{
		switch (environment) {
			case XgPushEnvironment::Production:
				SLIB_RETURN_STRING("product")
			case XgPushEnvironment::Development:
				SLIB_RETURN_STRING("dev")
		}
		return sl_null;
	}
	
}
