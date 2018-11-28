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

#include "slib/core/asset.h"

#include "slib/core/file.h"
#include "slib/core/system.h"
#include "slib/core/app.h"

#include "slib/core/platform_apple.h"
#include "slib/core/platform_android.h"
#include "slib/core/platform_tizen.h"


#if defined(SLIB_PLATFORM_IS_ANDROID)

namespace slib
{
	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_false;
	}

	String Assets::getFilePath(const String& path)
	{
		return sl_null;
	}

	Memory Assets::readAllBytes(const String& path)
	{
		return Android::readAllBytesFromAsset(path);
	}

}

#else

namespace slib
{

	sl_bool Assets::isBasedOnFileSystem()
	{
		return sl_true;
	}

#	if defined(SLIB_PLATFORM_IS_APPLE)

	String Assets::getFilePath(const String& path)
	{
		return Apple::getAssetFilePath(path);
	}

#	elif defined(SLIB_PLATFORM_IS_TIZEN)

	String Assets::getFilePath(const String& path)
	{
		return Tizen::getAssetFilePath(path);
	}

#	else

	String Assets::getFilePath(const String& path)
	{
		String s = File::makeSafeFilePath(path);
		if (s.isNotEmpty()) {
			return Application::getApplicationDirectory() + "/" + s;
		}
		return sl_null;
	}

#	endif

	Memory Assets::readAllBytes(const String& path)
	{
		Memory ret;
		String s = Assets::getFilePath(path);
		if (s.isNotEmpty()) {
			ret = File::readAllBytes(s);
		}
		return ret;
	}
}

#endif
