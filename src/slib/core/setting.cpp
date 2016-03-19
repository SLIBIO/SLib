#include "../../../inc/slib/core/setting.h"
#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN

void IniSetting::initialize()
{
	m_mapValues.removeAll();
}

sl_bool IniSetting::parseFromUtf8TextFile(const String& filePath)
{
	if (File::exists(filePath)) {
		String text = File::readUtf8Text(filePath);
		return parseFromText(text);
	} else {
		return sl_false;
	}
}

sl_bool IniSetting::parseFromText(const String& text)
{
	sl_uint32 len = text.getLength();
	const sl_char8* buf = text.getData();
	sl_int32 indexAssign = -1;
	sl_int32 indexComment = -1;
	sl_int32 indexStart = 0;
	ObjectLocker lock(&m_mapValues);
	for (sl_uint32 i = 0; i <= len; i++) {
		sl_char16 ch = buf[i];
		if (ch == '=') {
			if (indexAssign < 0 && indexComment < 0) {
				indexAssign = i;
			}
		} else if (ch == '#') {
			if (indexComment < 0) {
				indexComment = i;
			}
		} else if (ch == '\r' || ch == '\n' || ch == 0) {
			if (indexAssign > 0) {
				String key = String(buf + indexStart, indexAssign - indexStart).trim();
				String value;
				if (indexComment < 0) {
					value = String(buf + indexAssign + 1, i - indexAssign - 1).trim();
				} else {
					value = String(buf + indexAssign + 1, indexComment - indexAssign - 1).trim();
				}
				m_mapValues.put_NoLock(key, value);
			}
			indexAssign = -1;
			indexComment = -1;
			indexStart = i + 1;
		}
	}
	return sl_true;
}

Variant IniSetting::getValue(const String& name)
{
	String ret = m_mapValues.getValue(name, String::null());
	if (ret.isEmpty()) {
		return Variant::null();
	} else {
		return Variant::fromString(ret);
	}
}


sl_bool SettingUtil::parseUint32Range(const String& str, sl_uint32* _from, sl_uint32* _to)
{
	sl_uint32 from;
	sl_uint32 to;

	sl_int32 index = str.indexOf('-');
	if (index > 0) {
		if (str.substring(0, index).parseUint32(10, &from)) {
			if (str.substring(index + 1).parseUint32(10, &to)) {
				if (to >= from) {
					if (_from) {
						*_from = from;
					}
					if (_to) {
						*_to = to;
					}
					return sl_true;
				}
			}
		}
	} else {
		if (str.substring(0, index).parseUint32(10, &from)) {
			if (_from) {
				*_from = from;
			}
			if (_to) {
				*_to = from;
			}
			return sl_true;
		}
	}
	return sl_false;
}

SLIB_NAMESPACE_END
