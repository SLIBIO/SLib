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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "slib/core/locale.h"

#include "slib/core/platform_apple.h"

#if defined(SLIB_PLATFORM_IS_MACOS)
@interface _priv_Slib_ChangeLocaleNotificationObserver : NSObject
-(void)onChangeLocale:(NSNotification*)notification;
@end

@implementation _priv_Slib_ChangeLocaleNotificationObserver
-(void)onChangeLocale:(NSNotification*)notification
{
	slib::Locale::dispatchChangeCurrentLocale();
}
@end
#endif

namespace slib
{

	Locale Locale::_getCurrent()
	{
		NSArray* arr = [NSLocale preferredLanguages];
		if (arr != nil && arr.count > 0) {
			NSString* s = arr[0];
			Locale locale;
			if (locale.parse(s.UTF8String)) {
				return locale;
			}
		}
		NSLocale* locale = [NSLocale currentLocale];
		NSString* languageCode = [locale languageCode];
		NSString* scriptCode = [locale scriptCode];
		NSString* countryCode = [locale countryCode];
		Language lang = getLanguageFromCode([languageCode UTF8String]);
		LanguageScript script = getScriptFromCode([scriptCode UTF8String]);
		Country country = getCountryFromCode([countryCode UTF8String]);
		return Locale(lang, script, country);
	}

#if defined(SLIB_PLATFORM_IS_MACOS)
	void Locale::_setupOnChangeCurrentLocale()
	{
		static _priv_Slib_ChangeLocaleNotificationObserver* observer = [[_priv_Slib_ChangeLocaleNotificationObserver alloc] init];
		[[NSNotificationCenter defaultCenter] addObserver:observer selector:@selector(onChangeLocale:) name:NSCurrentLocaleDidChangeNotification object:nil];
	}
#endif
	
}

#endif
