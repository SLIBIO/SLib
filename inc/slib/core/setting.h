#ifndef CHECKHEADER_SLIB_CORE_SETTING
#define CHECKHEADER_SLIB_CORE_SETTING

#include "definition.h"
#include "map.h"
#include "variant.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT IniSetting
{
private:
	Map<String, String> m_mapValues;

public:
	sl_bool parseFromUtf8TextFile(const String& filePath);
	sl_bool parseFromText(const String& text);

	void initialize()
	{
		m_mapValues.clear();
	}
	Variant getValue(const String& name);

};

SLIB_NAMESPACE_END

#endif