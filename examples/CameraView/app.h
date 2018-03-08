/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <slib/ui.h>

using namespace slib;

class ExampleCameraViewApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleCameraViewApp)
public:
	ExampleCameraViewApp();
	
	void onStart() override;
	
private:
	Ref<CameraView> m_cameraView;
};
