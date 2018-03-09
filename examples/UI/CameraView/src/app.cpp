/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "app.h"

SLIB_DEFINE_APPLICATION(ExampleCameraViewApp, MobileApp)

ExampleCameraViewApp::ExampleCameraViewApp()
{
}

void ExampleCameraViewApp::onStart()
{
	m_cameraView = new CameraView;
	m_cameraView->setWidthFilling();
	m_cameraView->setHeightFilling();
	CameraParam param;
	m_cameraView->start(param);
	addViewToContent(m_cameraView);
}
