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
