#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/core/preference.h"

#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/platform_apple.h"

#import <Foundation/Foundation.h>

namespace slib
{

	void Preference::setValue(const String& key, const Json& value)
	{
		if (key.isEmpty()) {
			return;
		}
		NSString* _key = Apple::getNSStringFromString(key);
		String jsonString = value.toJsonString();
		if (jsonString.getLength() <= 0) {
			return;
		}
		NSString* _value = Apple::getNSStringFromString(jsonString);
		NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
		[defaults setValue:_value forKey:_key];
		[defaults synchronize];
	}

	Json Preference::getValue(const String& key)
	{
		if (key.isEmpty()) {
			return sl_null;
		}
		
		NSString* _key = Apple::getNSStringFromString(key);
		NSString* _value = [[NSUserDefaults standardUserDefaults] stringForKey:_key];
		
		if (_value == nil) {
			return sl_null;
		}
		
		return Json::parseJson(Apple::getStringFromNSString(_value));
	}

}

#endif
