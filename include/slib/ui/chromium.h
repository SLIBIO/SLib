/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_UI_CHROMIUM
#define CHECKHEADER_SLIB_UI_CHROMIUM

#include "definition.h"

#include "web_view.h"

namespace slib
{
	
	class SLIB_EXPORT ChromiumView : public WebView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ChromiumView();
		
		~ChromiumView();

	protected:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		Ptr<IWebViewInstance> getWebViewInstance() override;

	};

	class SLIB_EXPORT ChromiumSettings
	{
	public:
		String userAgent;

	public:
		ChromiumSettings();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ChromiumSettings)

	};
	
	class HttpCookie;
	
	class SLIB_EXPORT Chromium
	{
	public:
		// call at first line in `main()`
#ifdef SLIB_PLATFORM_IS_WIN32
		static void startup(void* hInstance);
		static void startup(void* hInstance, const ChromiumSettings& settings);
#else
		static void startup(int argc, const void* argv);
		static void startup(int argc, const void* argv, const ChromiumSettings& settings);
#endif		
		static void shutdown();

		static void clearCache();
		
		static void getAllCookies(const Function<void(const List<HttpCookie>& cookies)>& callback);
		static void getAllCookies(const String& url, const Function<void(const List<HttpCookie>& cookies)>& callback);
		static void setCookie(const String& url, const HttpCookie& cookie, const Function<void(sl_bool flagSuccess)>& callback);
		static void deleteCookies(const String& url, const String& name, const Function<void(sl_uint32 countDeleted)>& callback);
		static void clearCookies();
		
	};

}

#endif
