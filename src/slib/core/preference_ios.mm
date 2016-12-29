#include "../../../inc/slib/core/preference.h"
#include "../../../inc/slib/core/platform_apple.h"
#include "../../../inc/slib/core/json.h"

#import <Foundation/Foundation.h>

SLIB_NAMESPACE_BEGIN

void Preference::setValue(const String &key, const slib::Variant &value)
{
	if (key.getLength() <= 0) {
		return;
	}
	NSString* _key = Apple::getNSStringFromString(key);
	String jsonString = value.toJsonString();
	if (jsonString.getLength() <= 0) {
		return;
	}
	NSString* _value = Apple::getNSStringFromString(jsonString);
	[[NSUserDefaults standardUserDefaults] setValue:_value forKey:_key];
}

Variant Preference::getValue(const String &key)
{
	if (key.getLength() <= 0) {
		return sl_null;
	}
	
	NSString* _key = Apple::getNSStringFromString(key);
	NSString* _value = [[NSUserDefaults standardUserDefaults] stringForKey:_key];
	
	if (_value == nil) {
		return sl_null;
	}
	
	Variant value = Json::parseJson(Apple::getStringFromNSString(_value));
	return value;
}

SLIB_NAMESPACE_END
