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
	// Find configuration file
	Json conf = Json::parseJsonFromTextFile(Application::getApplicationDirectory() + "/slib_http.conf");
	if (conf.isNull()) {
		conf = Json::parseJsonFromTextFile(Application::getApplicationDirectory() + "/../slib_http.conf");
	}
	if (conf.isNull()) {
		conf = Json::parseJsonFromTextFile(Application::getApplicationDirectory() + "/../../slib_http.conf");
	}
	if (conf.isNull()) {
		conf = Json::parseJsonFromTextFile("/etc/slib_http.conf");
	}

	// Set configuration
	HttpServiceParam param;
	param.flagUseWebRoot = sl_true;
	param.port = (sl_uint16)(conf["port"].getUint32(8080));
	param.flagLogDebug = conf["debug"].getBoolean(sl_false);
	param.webRootPath = conf["root"].getString();

	param.onRequest = [](HttpService*, HttpServiceContext* context) {
		if (context->getPath() != "/") {
			return sl_false;
		}
		Console::println("Method: %s, Path: %s", context->getMethodText(), context->getPath());
		Console::println("Headers:");
		for (auto& pair : context->getRequestHeaders()) {
			Console::println("%s: %s", pair.key, pair.value);
		}
		Console::println("Body:");
		Console::println("%s", String::fromMemory(context->getRequestBody()));
		Console::println("");
		context->write("Welcome");
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
