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

int main(int argc, const char * argv[])
{
	StunServerParam param;
	param.flagLogging = sl_true;
	Ref<StunServer> service = StunServer::create(param);
	if (service.isNull()) {
		return -1;
	}
	Console::println("STUN service is running on port: %d", param.port);
	for(;;) {
		Console::println("\nPress x to exit!!!");
		if (Console::readChar() == 'x') {
			break;
		}
	}
	return 0;
}
