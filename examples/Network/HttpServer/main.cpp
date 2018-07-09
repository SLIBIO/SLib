/*
 *  Copyright (c) 2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <slib.h>

using namespace slib;

#define PORT 8080

int main(int argc, const char * argv[])
{
	HttpServiceParam param;
	param.port = PORT;
	param.onRequest = [](HttpService* service, HttpServiceContext* context) {
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
	Ref<HttpService> service = HttpService::create(param);
	if (service.isNull()) {
		return -1;
	}
	Console::println("Server is running on port: %d", PORT);
	for(;;) {
		Console::println("\nPress x to exit!!!");
		if (Console::readChar() == 'x') {
			break;
		}
	}
	return 0;
}
