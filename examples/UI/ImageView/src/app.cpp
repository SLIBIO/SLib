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

SLIB_DEFINE_APPLICATION(ExampleImageViewApp, MobileApp)

ExampleImageViewApp::ExampleImageViewApp()
{
}

void ExampleImageViewApp::onStart()
{
	getMainWindow()->setResizable(sl_true);
	getMainWindow()->setTitle("Example - ImageView");
	Ref<ImageView> imageView = new ImageView;
	imageView->setWidthFilling();
	imageView->setHeightFilling();
	imageView->setSource(Image::loadFromAsset("images/slib.png"));
	addViewToContent(imageView);
}
