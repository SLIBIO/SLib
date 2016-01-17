#ifndef CHECKHEADER_SLIB_CORE_SETTING
#define CHECKHEADER_SLIB_CORE_SETTING

#include "definition.h"

#include "map.h"
#include "variant.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT IniSetting
{
private:
	HashMap<String, String> m_mapValues;

public:
	void initialize();
	
	sl_bool parseFromUtf8TextFile(const String& filePath);
	
	sl_bool parseFromText(const String& text);
	
	Variant getValue(const String& name);

};

SLIB_NAMESPACE_END

#endif
