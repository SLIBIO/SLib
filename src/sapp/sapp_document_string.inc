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

namespace slib
{
	
	sl_bool SAppDocument::_parseStringResources(const String& localNamespace, const Ref<XmlElement>& element, const Locale& localeDefault, const String16& source)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		String strLocale = element->getAttribute("locale");
		Locale locale = Locale::Unknown;
		if (strLocale.isNotEmpty()) {
			locale = Locale(strLocale);
			if (locale.isInvalid()) {
				_logError(element, _g_sdev_sapp_error_resource_string_locale_invalid.arg(strLocale));
				return sl_false;
			}
		}
		if (locale == Locale::Unknown) {
			locale = localeDefault;
		}

		ListLocker< Ref<XmlElement> > children(element->getChildElements());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<XmlElement>& child = children[i];
			if (child.isNotNull()) {
				if (child->getName() == "string") {
					if (!_parseStringResource(localNamespace, child, locale, source)) {
						return sl_false;
					}
				} else {
					_logError(child, _g_sdev_sapp_error_invalid_tag.arg(child->getName()));
					return sl_false;
				}
			}
		}
		return sl_true;
	}

	sl_bool SAppDocument::_parseStringResource(const String& localNamespace, const Ref<XmlElement>& element, const Locale& localeDefault, const String16& source)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		String strLocale = element->getAttribute("locale");
		Locale locale = Locale::Unknown;
		if (strLocale.isNotEmpty()) {
			locale = Locale(strLocale);
			if (locale.isInvalid()) {
				_logError(element, _g_sdev_sapp_error_resource_string_locale_invalid.arg(strLocale));
				return sl_false;
			}
		}
		if (locale == Locale::Unknown) {
			locale = localeDefault;
		}
		
		String name = element->getAttribute("name");
		if (name.isEmpty()) {
			_logError(element, _g_sdev_sapp_error_resource_string_name_is_empty);
			return sl_false;
		}
		if (!(SAppUtil::checkName(name.getData(), name.getLength()))) {
			_logError(element, _g_sdev_sapp_error_resource_string_name_invalid.arg(name));
			return sl_false;
		}
		
		name = getNameInLocalNamespace(localNamespace, name);
		String value = source.substring(element->getStartContentPositionInSource(), element->getEndContentPositionInSource());
		
		Ref<SAppStringResource> res = m_strings.getValue(name, Ref<SAppStringResource>::null());
		if (res.isNull()) {
			res = new SAppStringResource;
			if (res.isNull()) {
				_logError(element, _g_sdev_sapp_error_out_of_memory);
				return sl_false;
			}
			res->name = name;
			if (!(m_strings.put(name, res))) {
				_logError(element, _g_sdev_sapp_error_out_of_memory);
				return sl_false;
			}
		}
		
		if (locale == Locale::Unknown) {
			if (res->defaultValue.isNotNull()) {
				_logError(element, _g_sdev_sapp_error_resource_string_redefine_default);
				return sl_false;
			}
			res->defaultValue = value;
		} else {
			if (res->values.find(locale)) {
				_logError(element, _g_sdev_sapp_error_resource_string_redefine_locale.arg(locale.toString()));
				return sl_false;
			}
			if (!(res->values.put(locale, value))) {
				_logError(element, _g_sdev_sapp_error_out_of_memory);
				return sl_false;
			}
		}
		
		return sl_true;
	}

	sl_bool SAppDocument::_generateStringsCpp(const String& targetPath)
	{
		_log(_g_sdev_sapp_log_generate_cpp_strings_begin);
		
		StringBuffer sbHeader, sbCpp, sbMap;
		
		sbHeader.add(String::format(
									"#pragma once%n%n"
									"#include <slib/core/resource.h>%n%n"
									"namespace %s%n"
									"{%n\tnamespace string%n\t{%n%n"
									, m_conf.generate_cpp_namespace));
		sbCpp.add(String::format(
								 "#include \"strings.h\"%n%n"
								 "namespace %s%n"
								 "{%n\tnamespace string%n\t{%n%n"
								 , m_conf.generate_cpp_namespace));
		
		sbMap.add("\t\tSLIB_DEFINE_STRING_RESOURCE_MAP_BEGIN\r\n");
		
		for (auto& pair : m_strings) {
			
			if (pair.value.isNotNull()) {
				
				sbHeader.add(String::format("\t\tSLIB_DECLARE_STRING_RESOURCE(%s)%n", pair.key));
				
				String defValue = pair.value->defaultValue;
				
				if (pair.value->values.isEmpty()) {
					sbCpp.add(String::format("\t\tSLIB_DEFINE_STRING_RESOURCE_SIMPLE(%s, \"%s\")%n%n", pair.key, ParseUtil::applyBackslashEscapes(defValue, sl_true, sl_false, sl_true)));
				} else {
					
					sbCpp.add(String::format("\t\tSLIB_DEFINE_STRING_RESOURCE_BEGIN(%s, \"%s\")%n", pair.key, ParseUtil::applyBackslashEscapes(defValue, sl_true, sl_false, sl_true)));
					
					{
						for (auto& pairValues : pair.value->values) {
							if (pairValues.key.getCountry() != Country::Unknown && pairValues.key.getScript() != LanguageScript::Unknown) {
								sbCpp.add(String::format("\t\t\tSLIB_DEFINE_STRING_RESOURCE_VALUE(%s, \"%s\")%n", pairValues.key.toString(), ParseUtil::applyBackslashEscapes(pairValues.value, sl_true, sl_false, sl_true)));
							}
						}
					}
					{
						for (auto& pairValues : pair.value->values) {
							if (pairValues.key.getCountry() != Country::Unknown && pairValues.key.getScript() == LanguageScript::Unknown) {
								sbCpp.add(String::format("\t\t\tSLIB_DEFINE_STRING_RESOURCE_VALUE(%s, \"%s\")%n", pairValues.key.toString(), ParseUtil::applyBackslashEscapes(pairValues.value, sl_true, sl_false, sl_true)));
							}
						}
					}
					{
						for (auto& pairValues : pair.value->values) {
							if (pairValues.key.getCountry() == Country::Unknown && pairValues.key.getScript() != LanguageScript::Unknown) {
								sbCpp.add(String::format("\t\t\tSLIB_DEFINE_STRING_RESOURCE_VALUE(%s, \"%s\")%n", pairValues.key.toString(), ParseUtil::applyBackslashEscapes(pairValues.value, sl_true, sl_false, sl_true)));
							}
						}
					}
					{
						for (auto& pairValues : pair.value->values) {
							if (pairValues.key.getCountry() == Country::Unknown && pairValues.key.getScript() == LanguageScript::Unknown) {
								sbCpp.add(String::format("\t\t\tSLIB_DEFINE_STRING_RESOURCE_VALUE(%s, \"%s\")%n", pairValues.key.toString(), ParseUtil::applyBackslashEscapes(pairValues.value, sl_true, sl_false, sl_true)));
							}
						}
					}
					
					static sl_char8 strEnd[] = "\t\tSLIB_DEFINE_STRING_RESOURCE_END\r\n\r\n";
					sbCpp.addStatic(strEnd, sizeof(strEnd)-1);
				}
				
				sbMap.add(String::format("\t\t\tSLIB_DEFINE_RESOURCE_MAP_ITEM(%s)%n", pair.key));
				
			}
		}
		
		sbMap.add("\t\tSLIB_DEFINE_STRING_RESOURCE_MAP_END\r\n");
		
		sbHeader.add("\r\n\t\tSLIB_DECLARE_STRING_RESOURCE_MAP\r\n\r\n\t}\r\n}\r\n");
		
		sbCpp.link(sbMap);
		sbCpp.add("\r\n\t}\r\n}\r\n");
		
		String pathHeader = targetPath + "/strings.h";
		String contentHeader = sbHeader.merge();
		if (File::readAllTextUTF8(pathHeader) != contentHeader) {
			if (!(File::writeAllTextUTF8(pathHeader, contentHeader))) {
				_logError(_g_sdev_sapp_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}
		}
		
		String pathCpp = targetPath + "/strings.cpp";
		String contentCpp = sbCpp.merge();
		if (File::readAllTextUTF8(pathCpp) != contentCpp) {
			if (!(File::writeAllTextUTF8(pathCpp, contentCpp))) {
				_logError(_g_sdev_sapp_error_file_write_failed.arg(pathCpp));
				return sl_false;
			}
		}
		
		return sl_true;
	}

	sl_bool SAppDocument::_getStringAccessString(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element, String& result)
	{
		if (!(value.flagDefined)) {
			result = "slib::String::null()";
			return sl_true;
		}
		if (value.flagReferResource) {
			String name;
			if (_checkStringName(localNamespace, value.valueOrName, element, &name)) {
				result = String::format("string::%s::get()", name);
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			if (value.valueOrName.isNull()) {
				result = "slib::String::null()";
			} else {
				result = String::format("\"%s\"", ParseUtil::applyBackslashEscapes(value.valueOrName, sl_true, sl_false, sl_true));
			}
			return sl_true;
		}
	}

	sl_bool SAppDocument::_getStringValue(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element, String& result)
	{
		Locale locale = getCurrentSimulatorLocale();
		if (!(value.flagDefined)) {
			result = String::null();
			return sl_true;
		}
		if (value.flagReferResource) {
			Ref<SAppStringResource> resource;
			if (_checkStringName(localNamespace, value.valueOrName, element, sl_null, &resource)) {
				{
					for (auto& item : resource->values) {
						if (item.key == locale) {
							result = item.value;
							return sl_true;
						}
					}
				}
				{
					for (auto& item : resource->values) {
						if (item.key == Locale(locale.getLanguage(), locale.getCountry())) {
							result = item.value;
							return sl_true;
						}
					}
				}
				{
					for (auto& item : resource->values) {
						if (item.key == Locale(locale.getLanguage(), locale.getScript(), Country::Unknown)) {
							result = item.value;
							return sl_true;
						}
					}
				}
				{
					for (auto& item : resource->values) {
						if (item.key == Locale(locale.getLanguage())) {
							result = item.value;
							return sl_true;
						}
					}
				}
				result = resource->defaultValue;
				return sl_true;
			} else {
				return sl_false;
			}
		} else {
			result = value.valueOrName;
			return sl_true;
		}
	}

	sl_bool SAppDocument::_checkStringValue(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element)
	{
		if (!(value.flagDefined)) {
			return sl_true;
		}
		if (value.flagReferResource) {
			return _checkStringName(localNamespace, value.valueOrName, element);
		} else {
			return sl_true;
		}
	}
	
	sl_bool SAppDocument::_checkStringName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName, Ref<SAppStringResource>* outResource)
	{
		if (getItemFromMap(m_strings, localNamespace, name, outName, outResource)) {
			return sl_true;
		} else {
			_logError(element, _g_sdev_sapp_error_string_not_found.arg(name));
			return sl_false;
		}
	}

}
