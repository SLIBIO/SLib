/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_SINGLETON
#define CHECKHEADER_SLIB_CORE_SINGLETON

#include "ref.h"
#include "safe_static.h"

#define SLIB_DECLARE_SINGLETON(TYPE) \
public: \
	static slib::Ref<TYPE> getInstance();

#define SLIB_DEFINE_SINGLETON(TYPE, ...) \
	slib::Ref<TYPE> TYPE::getInstance() \
	{ \
		SLIB_TRY_CONVERT_TYPE(TYPE*, Referable*) \
		SLIB_SAFE_STATIC(slib::Ref<TYPE>, instance, new TYPE(__VA_ARGS__)); \
		if (SLIB_SAFE_STATIC_CHECK_FREED(instance)) { \
			return sl_null; \
		} \
		return instance; \
	}

#define SLIB_INIT_SINGLETON(TYPE, ...) \
	slib::Ref<TYPE> TYPE::getInstance() \
	{ \
		SLIB_TRY_CONVERT_TYPE(TYPE*, Referable*) \
		SLIB_SAFE_STATIC(slib::Ref<TYPE>, instance, Init<TYPE>(__VA_ARGS__)); \
		if (SLIB_SAFE_STATIC_CHECK_FREED(instance)) { \
			return sl_null; \
		} \
		return instance; \
	}

#endif
