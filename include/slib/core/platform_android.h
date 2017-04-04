/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_ANDROID
#define CHECKHEADER_SLIB_CORE_PLATFORM_ANDROID

#include "definition.h"

#ifdef SLIB_PLATFORM_IS_ANDROID

#include "java.h"

namespace slib
{
	
	class SLIB_EXPORT Android
	{
	public:
		static void initialize(JavaVM* jvm);


		static jobject getCurrentActivity();

		static void setCurrentActivity(jobject activity);
	

		static void finishActivity();

		static void finishActivity(jobject activity);
	

		static jobject openAssetFile(const String& path);

		static Memory readAllBytesFromAsset(const String& path);
	
	
		static void dismissKeyboard();
	
	};

}

#endif

#endif
