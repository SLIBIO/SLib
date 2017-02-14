#ifndef CHECKHEADER_SLIB_CORE_SETTING
#define CHECKHEADER_SLIB_CORE_SETTING

#include "definition.h"

#include "map.h"
#include "variant.h"

namespace slib
{
	
	class SLIB_EXPORT IniSetting : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		IniSetting();

		~IniSetting();

	public:
		void initialize();

		sl_bool parseFromUtf8TextFile(const String& filePath);

		sl_bool parseFromText(const String& text);

		Variant getValue(const String& name);

	private:
		HashMap<String, String> m_mapValues;

	};
	
	class SLIB_EXPORT SettingUtil
	{
	public:
		static sl_bool parseUint32Range(const String& str, sl_uint32* from = sl_null, sl_uint32* to = sl_null);
	
	};

}

#endif
