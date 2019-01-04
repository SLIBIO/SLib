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

#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	// Set configuration
	HttpServiceParam param;
	do {
		if (param.parseJsonFile(Application::getApplicationDirectory() + "/../slib_http.conf")) {
			break;
		}
		if (param.parseJsonFile(Application::getApplicationDirectory() + "/../../slib_http.conf")) {
			break;
		}
		param.parseJsonFile("/etc/slib_http.conf");
	} while (0);
	
	param.onRequest = [](HttpService*, HttpServiceContext* context) {
		if (context->getPath() != "/") {
			return sl_false;
		}
		Console::println("Method: %s, Path: %s", context->getMethodText(), context->getPath());
		Console::println("Headers:");
		for (auto& pair : context->getRequestHeaders()) {
			Console::println("%s: %s", pair.key, pair.value);
		}
		Console::println("Cookies: %s", Json(context->getRequestCookies()));
		Console::println("Body:");
		Console::println("%s", String::fromMemory(context->getRequestBody()));
		Console::println("");
		
		HttpCookie cookie;
		cookie.name = "Cookie1";
		cookie.value = Time::now().toHttpDate();
		context->addResponseCookie(cookie);
		cookie.expires = Time::now().addSeconds(10);
		cookie.name = "Cookie2";
		cookie.value = "This Cookie will be expired after 10 seconds";
		context->addResponseCookie(cookie);

		Json data = {
			JsonItem("remote", context->getRemoteAddress().toString()),
			JsonItem("http", Json({
				JsonItem("method", context->getMethodText()),
				JsonItem("path", context->getPath()),
				JsonItem("list", Json({
					"item0", "item1"
				}))
			})),
			JsonItem("cookie1", context->getRequestCookie("Cookie1")),
			JsonItem("cookie2", context->getRequestCookie("Cookie2"))
		};
		context->write(Ginger::render(SLIB_STRINGIFY(
			Welcome ${remote} <BR>
			Method: ${http.method} <BR>
			Path: ${http.path} <BR>
			Cookie1: ${cookie1} <BR>
			Cookie2: ${cookie2} <BR>
			$for x in http.list {{ ${x} }}
		), data));
		return sl_true;
	};
	
	param.onPostRequest = [](HttpService*, HttpServiceContext* context, sl_bool flagProcessed) {
		if (!flagProcessed) {
			context->setResponseCode(HttpStatus::NotFound);
			context->write("Not found the specified file!");
		}
	};
	
	Ref<HttpService> service = HttpService::create(param);
	if (service.isNull()) {
		return -1;
	}
	
	Console::println("Server is running on port: %d, Webroot: %s", param.port, param.webRootPath);
	
	for(;;) {
		Console::println("\nPress x to exit!!!");
		if (Console::readChar() == 'x') {
			break;
		}
	}
	return 0;
}
