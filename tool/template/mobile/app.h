/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <slib.h>

using namespace slib;

class SLIB_TEMPLATE_APP_NAMEApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(SLIB_TEMPLATE_APP_NAMEApp)
public:
	SLIB_TEMPLATE_APP_NAMEApp();
	
protected:
	void onStart() override;
	
};
