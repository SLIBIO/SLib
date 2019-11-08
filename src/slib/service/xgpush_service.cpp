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

#define SLIB_SUPPORT_STD_TYPES

#include "slib/service/xgpush.h"

#include "slib/network/url_request.h"
#include "slib/network/http_common.h"
#include "slib/crypto/base64.h"
#include "slib/core/log.h"

namespace slib
{
	
	XgPushSendParam::XgPushSendParam()
	{
		platform = XgPushPlatform::All;
		environment = XgPushEnvironment::Production;
	}
	
	void XgPushService::sendNotification(const XgPushSendParam& param)
	{
		HttpHeaderMap headers = {
			{"Authorization", String::format("Basic %s", Base64::encode(String::format("%s:%s", param.appId, param.secretKey)))},
			{"Content-Type", "application/json"}
		};
		Json message = Json({
			JsonItem("title", param.message.title),
			JsonItem("content", param.message.content)
		});
		if (param.platform == XgPushPlatform::All || param.platform == XgPushPlatform::iOS) {
			Json ios = Json({
				JsonItem("aps", Json({
					JsonItem("alert", Json({
						JsonItem("title", param.message.title),
						JsonItem("body", param.message.content),
					})),
				}))
			});
			if (param.message.badge >= 0) {
				ios["aps"].putItem("badge_type", param.message.badge);
			}
			if (param.message.sound.isNotEmpty()) {
				ios["aps"].putItem("sound", param.message.sound);
			}
			if (param.message.data.isNotNull()) {
				ios.putItem("custom", param.message.data);
			}
			message.putItem("ios", ios);
		}
		if (param.platform == XgPushPlatform::All || param.platform == XgPushPlatform::Android) {
			Json android = Json({
				JsonItem("ring", 1),
			});
			if (param.message.sound.isNotEmpty()) {
				android.putItem("ring_raw", param.message.sound);
			}
			if (param.message.data.isNotNull()) {
				android.putItem("custom_content", param.message.data);
			}
			message.putItem("android", android);
		}
		if (param.customMessage.isJsonMap()) {
			for (auto& item : param.customMessage.getJsonMap()) {
				message.putItem(item.key, item.value);
			}
		}
		Json body = Json({
			JsonItem("audience_type", param.receiverDeviceTokens.getCount() > 1 ? "token_list" : "token"),
			JsonItem("token_list", param.receiverDeviceTokens),
			JsonItem("platform", getPlatformString(param.platform)),
			JsonItem("environment", getEnvironmentString(param.environment)),
			JsonItem("message_type", "notify"),
			JsonItem("message", message)
		});

		auto callback = param.callback;
		UrlRequest::post("https://openapi.xg.qq.com/v3/push/app", headers, body, [callback](UrlRequest* request) {
			String response = request->getResponseContentAsString();
			if (request->isError()) {
				LogError("XgPush", "[Status] %d %s - Error: %s %s", (int)(request->getResponseStatus()), request->getResponseMessage(), request->getLastErrorMessage(), response);
				if (response.isEmpty()) {
					response = request->getLastErrorMessage();
				}
				callback(sl_false, response);
			} else {
				Log("XgPush", "[Status] %d %s - %s", (int)(request->getResponseStatus()), request->getResponseMessage(), response);
				Json json = request->getResponseContentAsJson();
				if (json.isNull()) {
					callback(sl_false, request->getResponseContentAsString());
				} else {
					sl_uint32 ret_code = json["ret_code"].getUint32();
					if (ret_code != 0) {
						callback(sl_false, json["err_msg"].getString());
					} else {
						callback(sl_true, sl_null);
					}
				}
			}
		});
	}
	
	String XgPushService::getPlatformString(XgPushPlatform platform)
	{
		String ret;
		switch (platform) {
			case XgPushPlatform::All:
				ret = "all";
				break;
			case XgPushPlatform::Android:
				ret = "android";
				break;
			case XgPushPlatform::iOS:
				ret = "ios";
				break;
		}
		return ret;
	}
	
	String XgPushService::getEnvironmentString(XgPushEnvironment environment)
	{
		String ret;
		switch (environment) {
			case XgPushEnvironment::Production:
				ret = "product";
				break;
			case XgPushEnvironment::Development:
				ret = "dev";
				break;
		}
		return ret;
	}
	
}
