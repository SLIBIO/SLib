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

	sl_bool SAppDocument::_parseColorResource(const String& localNamespace, const Ref<XmlElement>& element)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		String name = element->getAttribute("name");
		if (name.isEmpty()) {
			_logError(element, _g_sdev_sapp_error_resource_color_name_is_empty);
			return sl_false;
		}
		if (!(SAppUtil::checkName(name.getData(), name.getLength()))) {
			_logError(element, _g_sdev_sapp_error_resource_color_name_invalid.arg(name));
			return sl_false;
		}
		
		name = getNameInLocalNamespace(localNamespace, name);
		
		if (m_colors.find(name)) {
			_logError(element, _g_sdev_sapp_error_resource_color_name_redefined.arg(name));
			return sl_false;
		}
		
		String valueText = element->getText();
		Color value;
		if (!(Color::parse(valueText, &value))) {
			_logError(element, _g_sdev_sapp_error_resource_color_value_invalid.arg(valueText));
			return sl_false;
		}
		
		Ref<SAppColorResource> res = new SAppColorResource;
		if (res.isNull()) {
			_logError(element, _g_sdev_sapp_error_out_of_memory);
			return sl_false;
		}
		res->name = name;
		res->value = value;
		if (!(m_colors.put(name, res))) {
			_logError(element, _g_sdev_sapp_error_out_of_memory);
			return sl_false;
		}
		
		return sl_true;
	}

	sl_bool SAppDocument::_generateColorsCpp(const String& targetPath)
	{
		_log(_g_sdev_sapp_log_generate_cpp_colors_begin);
		
		StringBuffer sbHeader, sbCpp, sbMap;
		
		sbHeader.add(String::format(
									"#pragma once%n%n"
									"#include <slib/graphics/resource.h>%n%n"
									"namespace %s%n"
									"{%n\tnamespace color%n\t{%n%n"
									, m_conf.generate_cpp_namespace));
		sbCpp.add(String::format(
								 "#include \"colors.h\"%n%n"
								 "namespace %s%n"
								 "{%n\tnamespace color%n\t{%n%n"
								 , m_conf.generate_cpp_namespace));
		
		sbMap.add("\t\tSLIB_DEFINE_COLOR_RESOURCE_MAP_BEGIN\r\n");
		
		for (auto& pair : m_colors) {
			
			if (pair.value.isNotNull()) {
				
				sbHeader.add(String::format("\t\tSLIB_DECLARE_COLOR_RESOURCE(%s)%n", pair.key));

				Color& color = pair.value->value;
				sbCpp.add(String::format("\t\tSLIB_DEFINE_COLOR_RESOURCE(%s, %d, %d, %d, %d)%n", pair.key, color.r, color.g, color.b, color.a));
				
				sbMap.add(String::format("\t\t\tSLIB_DEFINE_COLOR_RESOURCE_MAP_ITEM(%s)%n", pair.key));
				
			}
		}
		
		sbMap.add("\t\tSLIB_DEFINE_COLOR_RESOURCE_MAP_END\r\n");
		
		sbHeader.add("\r\n\t\tSLIB_DECLARE_COLOR_RESOURCE_MAP\r\n\r\n\t}\r\n}\r\n");
		
		sbCpp.link(sbMap);
		sbCpp.add("\r\n\t}\r\n}\r\n");
		
		String pathHeader = targetPath + "/colors.h";
		String contentHeader = sbHeader.merge();
		if (File::readAllTextUTF8(pathHeader) != contentHeader) {
			if (!(File::writeAllTextUTF8(pathHeader, contentHeader))) {
				_logError(_g_sdev_sapp_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}
		}
		
		String pathCpp = targetPath + "/colors.cpp";
		String contentCpp = sbCpp.merge();
		if (File::readAllTextUTF8(pathCpp) != contentCpp) {
			if (!(File::writeAllTextUTF8(pathCpp, contentCpp))) {
				_logError(_g_sdev_sapp_error_file_write_failed.arg(pathCpp));
				return sl_false;
			}
		}
		
		return sl_true;
	}
	
	sl_bool SAppDocument::_getColorAccessString(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element, String& result)
	{
		if (!(value.flagDefined)) {
			result = "slib::Color::zero()";
			return sl_true;
		}
		if (value.resourceName.isNull()) {
			result = String::format("slib::Color(%d, %d, %d, %d)", value.color.r, value.color.g, value.color.b, value.color.a);
			return sl_true;
		} else {
			String name;
			if (_checkColorName(localNamespace, value.resourceName, element, &name)) {
				result = String::format("color::%s::get()", name);
				return sl_true;
			} else {
				return sl_false;
			}
		}
	}

	sl_bool SAppDocument::_getColorValue(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element, Color& result)
	{
		if (!(value.flagDefined)) {
			result = Color::zero();
			return sl_true;
		}
		if (value.resourceName.isNull()) {
			result = value.color;
			return sl_true;
		} else {
			Ref<SAppColorResource> resource;
			if (_checkColorName(localNamespace, value.resourceName, element, sl_null, &resource)) {
				result = resource->value;
				return sl_true;
			} else {
				return sl_false;
			}
		}
	}

	sl_bool SAppDocument::_checkColorValue(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element)
	{
		if (!(value.flagDefined)) {
			return sl_true;
		}
		if (value.resourceName.isNull()) {
			return sl_true;
		} else {
			return _checkColorName(localNamespace, value.resourceName, element);
		}
	}
	
	sl_bool SAppDocument::_checkColorName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName, Ref<SAppColorResource>* outResource)
	{
		if (getItemFromMap(m_colors, localNamespace, name, outName, outResource)) {
			return sl_true;
		} else {
			_logError(element, _g_sdev_sapp_error_color_not_found.arg(name));
			return sl_false;
		}
	}

}
