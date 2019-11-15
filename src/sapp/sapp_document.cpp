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

#include "sapp_document.h"

#include "sapp_util.h"

#include "slib/core/file.h"
#include "slib/core/log.h"
#include "slib/core/resource.h"
#include "slib/core/string_buffer.h"
#include "slib/core/scoped.h"
#include "slib/ui.h"

#define TAG "SApp"

namespace slib
{
	
	namespace priv
	{
		namespace sapp
		{
			
			SLIB_STATIC_STRING(g_str_log_appconf_begin, "Opening app configuration file: %s")
			SLIB_STATIC_STRING(g_str_log_appconf_end, "Finished app configuration")
			SLIB_STATIC_STRING(g_str_log_open_drawables_begin, "Opening drwable resources: %s")
			SLIB_STATIC_STRING(g_str_log_open_raws_begin, "Opening raw resources: %s")
			SLIB_STATIC_STRING(g_str_log_open_resource_begin, "Opening resource file: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_begin, "Generating C++ files: %s")
			SLIB_STATIC_STRING(g_str_error_generate_cpp_target_path_is_empty, "App configuration: <generate-cpp>/<target-path> is empty")
			SLIB_STATIC_STRING(g_str_error_generate_cpp_target_path_invalid, "App configuration: Failed to create target path for generating cpp: %s")
			SLIB_STATIC_STRING(g_str_error_generate_cpp_namespace_is_empty, "App configuration: <generate-cpp>/<namespace> is empty")
			SLIB_STATIC_STRING(g_str_error_generate_cpp_namespace_invalid, "App configuration: Invalid <generate-cpp>/<namespace>: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_resources_h_begin, "Generating: resources.h")
			
			SLIB_STATIC_STRING(g_str_error_configuration_value_empty, "App configuration: <%s> tag is empty")
			SLIB_STATIC_STRING(g_str_error_configuration_value_invalid, "App configuration: value of <%s> tag is invalid: %s")
			
			SLIB_STATIC_STRING(g_str_error_out_of_memory, "Out of memory")
			SLIB_STATIC_STRING(g_str_error_directory_not_found, "Directory not found: %s")
			SLIB_STATIC_STRING(g_str_error_directory_create_failed, "Creating directory is failed: %s")
			SLIB_STATIC_STRING(g_str_error_file_not_found, "File not found: %s")
			SLIB_STATIC_STRING(g_str_error_file_write_failed, "Failed to write to file: %s")
			SLIB_STATIC_STRING(g_str_error_invalid_root_tag, "<sapp> tag is not declared for root element")
			SLIB_STATIC_STRING(g_str_error_configuration_tag_not_found, "<configuration> tag is not found in root element")
			SLIB_STATIC_STRING(g_str_error_invalid_tag, "Invalid tag: %s")
			SLIB_STATIC_STRING(g_str_error_string_not_found, "String resource is not defined: %s")
			SLIB_STATIC_STRING(g_str_error_color_not_found, "Color resource is not defined: %s")
			SLIB_STATIC_STRING(g_str_error_drawable_not_found, "Drawable resource is not defined: %s")
			SLIB_STATIC_STRING(g_str_error_drawable_not_image, "Drawable resource is not image: %s")
			SLIB_STATIC_STRING(g_str_error_load_image_failed, "Failed to load image drawable resource: %s")
			SLIB_STATIC_STRING(g_str_error_menu_not_found, "Menu resource is not defined: %s")
			SLIB_STATIC_STRING(g_str_error_layout_style_not_found, "style is not found: %s")
			SLIB_STATIC_STRING(g_str_error_layout_include_not_found, "include is not found: %s")
			SLIB_STATIC_STRING(g_str_error_layout_not_found, "layout is not found: %s")
			SLIB_STATIC_STRING(g_str_error_layout_is_not_view, "layout is not a view: %s")
			
			SLIB_STATIC_STRING(g_str_error_resource_raw_name_duplicated, "Raw Resource: Generated name %s is duplicated: %s")
			SLIB_STATIC_STRING(g_str_error_resource_raw_size_big, "Raw Resource: Size is larger than 16MB: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_raws_begin, "Generating: raws, raws.cpp")
			
			SLIB_STATIC_STRING(g_str_error_resource_drawable_locale_invalid, "Drawable Resource: Invalid locale is found in the drawable directory name: %s")
			SLIB_STATIC_STRING(g_str_error_resource_drawable_filename_invalid, "Drawable Resource: Invalid image filename: %s")
			SLIB_STATIC_STRING(g_str_error_resource_drawable_not_defined_default, "Drawable Resource: default resource is not defined for this name: %s")
			SLIB_STATIC_STRING(g_str_error_resource_drawable_type_duplicated, "Drawable Resource: Invalid type is already defined on the name: %s")
			SLIB_STATIC_STRING(g_str_error_resource_drawable_load_image_failed, "Drawable Resource: Loading image file failed: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepieces_name_is_empty, "Nine-Pieces Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_ninepieces_name_invalid, "Nine-Pieces Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepieces_name_redefined, "Nine-Pieces Resource: name is redefined: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepieces_attribute_invalid, "Nine-Pieces Resource: Invalid %s attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepatch_name_is_empty, "Nine-Patch Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_ninepatch_name_invalid, "Nine-Patch Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepatch_name_redefined, "Nine-Patch Resource: name is redefined: %s")
			SLIB_STATIC_STRING(g_str_error_resource_ninepatch_attribute_invalid, "Nine-Patch Resource: Invalid %s attribute value: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_drawables_begin, "Generating: drawables.h, drawables.cpp")
			
			SLIB_STATIC_STRING(g_str_error_resource_string_locale_invalid, "String Resource: Invalid locale attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_string_name_is_empty, "String Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_string_name_invalid, "String Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_string_redefine_default, "String Resource: default value is redefined")
			SLIB_STATIC_STRING(g_str_error_resource_string_redefine_locale, "String Resource: value is redefined for %s locale")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_strings_begin, "Generating: strings.h, strings.cpp")
			
			SLIB_STATIC_STRING(g_str_error_resource_color_name_is_empty, "Color Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_color_name_invalid, "Color Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_color_name_redefined, "Color Resource: name is redefined: %s")
			SLIB_STATIC_STRING(g_str_error_resource_color_value_invalid, "Color Resource: Invalid color value: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_colors_begin, "Generating: colors.h, colors.cpp")
			
			SLIB_STATIC_STRING(g_str_error_resource_menu_name_is_empty, "Menu Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_menu_name_is_root, "Menu Resource: 'root' is not allowed for item name")
			SLIB_STATIC_STRING(g_str_error_resource_menu_name_invalid, "Menu Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_name_redefined, "Menu Resource: name is redefined: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_title_refer_invalid, "Menu Resource: title should be valid string value or string resource: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_icon_invalid, "Menu Resource: Invalid icon image resource: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_platform_invalid, "Menu Resource: Invalid platform attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_shortcutKey_invalid, "Menu Resource: Invalid shortcutKey attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_macShortcutKey_invalid, "Menu Resource: Invalid macShortcutKey attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_children_tag_invalid, "Menu Resource: Invalid tag: %s")
			SLIB_STATIC_STRING(g_str_error_resource_menu_item_name_redefined, "Menu Resource: item name is redefined: %s")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_menus_begin, "Generating: menus.h, menus.cpp")
			
			SLIB_STATIC_STRING(g_str_error_resource_layout_type_invalid, "Layout Resource: Invalid layout type: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_name_is_empty, "Layout Resource: name attribute is empty")
			SLIB_STATIC_STRING(g_str_error_resource_layout_name_invalid, "Layout Resource: Invalid name attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_name_redefined, "Layout Resource: name is redefined: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_name_array_item_class_different, "Layout Resource: different class is used for array item: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_value_invalid, "Layout Resource: Invalid value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_attribute_invalid, "Layout Resource: Invalid %s attribute value: %s")
			SLIB_STATIC_STRING(g_str_error_resource_layout_scrollview_must_contain_one_child, "Layout Resource: ScrollView can contain only one child")
			SLIB_STATIC_STRING(g_str_error_resource_layout_item_must_contain_one_child, "Layout Resource: item can contain only one child")
			SLIB_STATIC_STRING(g_str_error_resource_layout_refreshview_must_contain_one_child, "Layout Resource: RefreshView can contain only one child")
			SLIB_STATIC_STRING(g_str_log_generate_cpp_layouts_begin, "Generating: layouts.h, layouts.cpp")

		}
	}
	
	using namespace priv::sapp;

#define RAW_MAX_SIZE 0x1000000 // 16MB

	SAppConfiguration::SAppConfiguration()
	{
		simulator_locale = Locale::Unknown;
	}
	
	SAppConfiguration::SAppConfiguration(SAppConfiguration&& other) = default;
	
	SAppConfiguration::~SAppConfiguration()
	{
	}
		
	SAppConfiguration& SAppConfiguration::operator=(SAppConfiguration&& other) = default;
	
	SAppSimulateLayoutParam::SAppSimulateLayoutParam()
	{
		pageSize.x = 450;
		pageSize.y = 800;
	}
	
	
	SAppDocument::SAppDocument()
	{
		m_flagOpened = sl_false;
	}

	SAppDocument::~SAppDocument()
	{
	}

	sl_bool SAppDocument::open(const String& filePath)
	{
		ObjectLocker lock(this);
		
		SAppConfiguration conf;
		if (!_parseConfiguration(filePath, conf)) {
			return sl_false;
		}
		
		close();
		
		m_pathConf = filePath;
		m_pathApp = conf.app_path;

		m_conf = Move(conf);
		
		m_flagOpened = sl_true;
		
		return sl_true;
	}

	void SAppDocument::close()
	{
		ObjectLocker lock(this);
		if (m_flagOpened) {
			_freeResources();
			m_flagOpened = sl_false;
		}
	}

	sl_bool SAppDocument::isOpened()
	{
		return m_flagOpened;
	}
	
	void SAppDocument::clearAllResources()
	{
		m_drawables.removeAll();
		m_strings.removeAll();
		m_colors.removeAll();
		m_menus.removeAll();
		m_raws.removeAll();
		m_layouts.removeAll();
		m_layoutStyles.removeAll();
		m_layoutIncludes.removeAll();
	}

	sl_bool SAppDocument::openResources()
	{
		ObjectLocker lock(this);
		
		if (!m_flagOpened) {
			return sl_false;
		}
		
		_freeResources();
		
		if (!(_openRawResources())) {
			return sl_false;
		}
		if (!(_openImageResources())) {
			return sl_false;
		}
		if (!(_openGlobalResources())) {
			return sl_false;
		}
		if (!(_openUiResources())) {
			return sl_false;
		}

		return sl_true;
	}
	
	sl_bool SAppDocument::openUiResource(const String& filePath)
	{
		ObjectLocker lock(this);
		
		if (!m_flagOpened) {
			return sl_false;
		}
		
		_freeResources();
		
		if (!(_openRawResources())) {
			return sl_false;
		}
		if (!(_openImageResources())) {
			return sl_false;
		}
		if (!(_openGlobalResources())) {
			return sl_false;
		}
		if (!(_openUiResource(filePath))) {
			return sl_false;
		}
		
		return sl_true;
	}
	
	sl_bool SAppDocument::_openImageResources()
	{
		List<String> fileList = File::getFiles(m_pathApp);
		fileList.sort();
		for (auto& fileName : fileList) {
			if (fileName.isNotNull()) {
				if (fileName == "image") {
					if (!(_registerImageResources("image", m_pathApp + "/image", Locale::Unknown))) {
						return sl_false;
					}
				} else if (fileName.startsWith("image-")) {
					String strLocale = fileName.substring(6);
					Locale locale = Locale::Unknown;
					if (strLocale.isNotEmpty()) {
						locale = Locale(strLocale);
					}
					if (locale == Locale::Unknown || locale.isInvalid()) {
						_logError(g_str_error_resource_drawable_locale_invalid.arg(fileName));
						return sl_false;
					}
					if (!(_registerImageResources(fileName, m_pathApp + "/" + fileName, locale))) {
						return sl_false;
					}
				}
			}
		}
		return sl_true;
	}
	
	sl_bool SAppDocument::_openRawResources()
	{
		m_raws.removeAll();
		String path = m_pathApp + "/raw";
		if (File::exists(path)) {
			if (!(_registerRawResources(path))) {
				return sl_false;
			}
		}
		return sl_true;
	}
	
	sl_bool SAppDocument::_openGlobalResources()
	{
		String pathDir = m_pathApp + "/global";
		for (auto& fileName : File::getFiles(pathDir)) {
			String path = pathDir + "/" + fileName;
			if (File::exists(path)) {
				if (!(File::isDirectory(path))) {
					if (File::getFileExtension(fileName) == "xml") {
						if (!(_parseResourcesXml(sl_null, path))) {
							return sl_false;
						}
					}
				}
			}
		}
		return sl_true;
	}
	
	sl_bool SAppDocument::_openUiResources()
	{
		String pathDir = m_pathApp + "/ui";
		for (auto& fileName : File::getFiles(pathDir)) {
			String path = pathDir + "/" + fileName;
			if (File::getFileExtension(fileName) == "xml" || File::getFileExtension(fileName) == "uiml") {
				if (!(_openUiResource(path))) {
					return sl_false;
				}
			}
		}
		return sl_true;
	}
	
	sl_bool SAppDocument::_openUiResource(const String& path)
	{
		if (File::exists(path)) {
			if (!(File::isDirectory(path))) {
				String fileName = File::getFileName(path);
				String localNamespace = File::getFileNameOnly(fileName);
				if (localNamespace.isNotEmpty()) {
					if (SAppUtil::checkName(localNamespace.getData())) {
						if (_parseResourcesXml(localNamespace, path)) {
							return sl_true;
						}
					}
				}
			}
		}
		return sl_false;
	}

	sl_bool SAppDocument::generateCpp()
	{
		ObjectLocker lock(this);

		if (!m_flagOpened) {
			return sl_false;
		}
		
		String path = m_conf.generate_cpp_target_path;
		if (path.isEmpty()) {
			_logError(g_str_error_generate_cpp_target_path_is_empty);
			return sl_false;
		}
		if (!(File::isDirectory(path))) {
			File::createDirectories(path);
			if (!(File::isDirectory(path))) {
				_logError(g_str_error_generate_cpp_target_path_invalid.arg(path));
				return sl_false;
			}
		}
		
		if (m_conf.generate_cpp_namespace.isEmpty()) {
			_logError(g_str_error_generate_cpp_namespace_is_empty);
			return sl_false;
		}
		if (!(SAppUtil::checkName(m_conf.generate_cpp_namespace.getData(), m_conf.generate_cpp_namespace.getLength()))) {
			_logError(g_str_error_generate_cpp_namespace_invalid.arg(m_conf.generate_cpp_namespace));
			return sl_false;
		}
		
		_log(g_str_log_generate_cpp_begin.arg(path));
		
		if (!_generateResourcesH(path)) {
			return sl_false;
		}
		if (!_generateStringsCpp(path)) {
			return sl_false;
		}
		if (!_generateRawCpp(path)) {
			return sl_false;
		}
		if (!_generateDrawablesCpp(path)) {
			return sl_false;
		}
		if (!_generateColorsCpp(path)) {
			return sl_false;
		}
		if (!_generateMenusCpp(path)) {
			return sl_false;
		}
		if (!_generateLayoutsCpp(path)) {
			return sl_false;
		}

		return sl_true;
	}

	List< Ref<SAppLayoutResource> > SAppDocument::getLayouts()
	{
		ObjectLocker lock(this);
		
		if (!m_flagOpened) {
			return List< Ref<SAppLayoutResource> >::null();
		}
		
		return m_layouts.getAllValues();
		
	}

	void SAppDocument::simulateLayoutInWindow(const String& layoutName, const SAppSimulateLayoutParam& param)
	{
		ObjectLocker lock(this);
		
		if (!m_flagOpened) {
			return;
		}

		Ref<SAppLayoutResource> layout = m_layouts.getValue(layoutName, Ref<SAppLayoutResource>::null());
		if (layout.isNotNull()) {
			_simulateLayoutInWindow(layout.get(), param);
		}
		
	}
	
	Locale SAppDocument::getCurrentSimulatorLocale()
	{
		Locale locale = m_conf.simulator_locale;
		if (locale == Locale::Unknown) {
			locale = Locale::getCurrent();
		}
		return locale;
	}

	/***************************************************
	 					Utilities
	***************************************************/
	
	String SAppDocument::getShortcutKeyDefinitionString(const KeycodeAndModifiers& km, sl_bool flagMac)
	{
		if (km.getKeycode() == Keycode::Unknown) {
			return "slib::Keycode::Unknown";
		}
		String ret;
		if (km.isControlKey()) {
			ret = "slib::Modifiers::Control";
		}
		if (flagMac) {
			if (km.isOptionKey()) {
				if (ret.isNotEmpty()) {
					ret += " | ";
				}
				ret += "slib::Modifiers::Option";
			}
		} else {
			if (km.isAltKey()) {
				if (ret.isNotEmpty()) {
					ret += " | ";
				}
				ret += "slib::Modifiers::Alt";
			}
		}
		if (km.isShiftKey()) {
			if (ret.isNotEmpty()) {
				ret += " | ";
			}
			ret += "slib::Modifiers::Shift";
		}
		if (flagMac) {
			if (km.isCommandKey()) {
				if (ret.isNotEmpty()) {
					ret += " | ";
				}
				ret += "slib::Modifiers::Command";
			}
		} else {
			if (km.isWindowsKey()) {
				if (ret.isNotEmpty()) {
					ret += " | ";
				}
				ret += "slib::Modifiers::Windows";
			}
		}
		if (ret.isNotEmpty()) {
			ret += " | ";
		}
		ret += "slib::Keycode::";
		ret += UIEvent::getKeyName(km.getKeycode(), sl_false);
		return ret;
	}
	
	String SAppDocument::getNameInLocalNamespace(const String& localNamespace, const String& name)
	{
		if (localNamespace.isNotEmpty()) {
			return localNamespace + "_" + name;
		} else {
			return name;
		}
	}

	/***************************************************
						Log
	***************************************************/

	void SAppDocument::_log(const String& text)
	{
		Log(TAG, text);
	}

	void SAppDocument::_logError(const String& text)
	{
		LogError(TAG, text);
	}

	void SAppDocument::_logError(const String& filePath, sl_size line, sl_size col, const String& text)
	{
		LogError(TAG, "%s(%d:%d)%n%s", filePath, line, col, text);
	}

	void SAppDocument::_logError(const Ref<XmlElement>& element, const String& text)
	{
		if (element.isNotNull()) {
			LogError(TAG, "%s(%d:%d)%n%s", element->getSourceFilePath(), element->getLineNumberInSource(), element->getColumnNumberInSource(), text);
		} else {
			LogError(TAG, text);
		}
	}


	/***************************************************
					Resources Entry
	***************************************************/

	sl_bool SAppDocument::_parseConfiguration(const String& filePath, SAppConfiguration& conf)
	{
		_log(g_str_log_appconf_begin.arg(filePath));
		
		if (!(File::exists(filePath))) {
			_logError(g_str_error_file_not_found.arg(filePath));
			return sl_false;
		}
		
		XmlParseParam param;
		param.flagLogError = sl_false;
		param.setCreatingOnlyElementsAndTexts();
		Ref<XmlDocument> xml = Xml::parseXmlFromTextFile(filePath, param);
		if (param.flagError) {
			_logError(filePath, param.errorLine, param.errorColumn, param.errorMessage);
			return sl_false;
		}
		
		if (xml.isNull()) {
			return sl_false;
		}
		Ref<XmlElement> root = xml->getRoot();
		if (root.isNull()) {
			return sl_false;
		}
		if (root->getName() != "sapp") {
			_logError(root, g_str_error_invalid_root_tag);
			return sl_false;
		}
		
		root = root->getFirstChildElement("configuration");
		if (root.isNull()) {
			_logError(root, g_str_error_configuration_tag_not_found);
			return sl_false;
		}
		
		// app-path
		{
			Ref<XmlElement> el_app_path = root->getFirstChildElement("app-path");
			if (el_app_path.isNotNull()) {
				String strPath = el_app_path->getText();
				if (strPath.startsWith('.')) {
					strPath = File::getParentDirectoryPath(filePath) + "/" + strPath;
				}
				if (File::isDirectory(strPath)) {
					conf.app_path = strPath;
				} else {
					_logError(el_app_path, g_str_error_directory_not_found.arg(strPath));
					return sl_false;
				}
			} else {
				conf.app_path = File::getParentDirectoryPath(filePath);
			}
		}
		
		// generate-cpp
		Ref<XmlElement> el_generate_cpp = root->getFirstChildElement("generate-cpp");
		if (el_generate_cpp.isNotNull()) {
			Ref<XmlElement> el_target_path = el_generate_cpp->getFirstChildElement("target-path");
			if (el_target_path.isNotNull()) {
				String strPath = el_target_path->getText();
				if (strPath.isEmpty()) {
					_logError(el_target_path, g_str_error_configuration_value_empty.arg("target-path"));
					return sl_false;
				}
				if (strPath.startsWith('.')) {
					strPath = File::getParentDirectoryPath(filePath) + "/" + strPath;
				}
				conf.generate_cpp_target_path = strPath;
			}
			Ref<XmlElement> el_namespace = el_generate_cpp->getFirstChildElement("namespace");
			if (el_namespace.isNotNull()) {
				conf.generate_cpp_namespace = el_namespace->getText();
				if (!(SAppUtil::checkName(conf.generate_cpp_namespace.getData(), conf.generate_cpp_namespace.getLength()))) {
					_logError(el_namespace, g_str_error_configuration_value_invalid.arg("namespace", conf.generate_cpp_namespace));
					return sl_false;
				}
			}
			Ref<XmlElement> el_layout = el_generate_cpp->getFirstChildElement("layout");
			if (el_layout.isNotNull()) {
				ListLocker< Ref<XmlElement> > children(el_layout->getChildElements());
				for (sl_size i = 0; i < children.count; i++) {
					Ref<XmlElement> child = children[i];
					if (child.isNotNull()) {
						if (child->getName() == "include-header") {
							String str = child->getText().trim();
							if (str.isNotEmpty()) {
								conf.generate_cpp_layout_include_headers.add(str);
							}
						} else if (child->getName() == "include-header-in-cpp") {
							String str = child->getText().trim();
							if (str.isNotEmpty()) {
								conf.generate_cpp_layout_include_headers_in_cpp.add(str);
							}
						}
					}
				}
			}
		}
		
		// simulator
		Ref<XmlElement> el_simulator = root->getFirstChildElement("simulator");
		if (el_simulator.isNotNull()) {
			Ref<XmlElement> el_locale = el_simulator->getFirstChildElement("locale");
			if (el_locale.isNotNull()) {
				String strLocale = el_locale->getText();
				if (strLocale.isNotEmpty()) {
					Locale locale;
					if (locale.parse(strLocale)) {
						conf.simulator_locale = locale;
					} else {
						_logError(el_locale, g_str_error_configuration_value_invalid.arg("locale", strLocale));
						return sl_false;
					}
				}
			}
		}
		
		_log(g_str_log_appconf_end);
		
		return sl_true;
	}

	void SAppDocument::_freeResources()
	{
		clearAllResources();
		
		{
			ListLocker< Ref<SAppLayoutSimulationWindow> > windows(m_layoutSimulationWindows);
			for (sl_size i = 0; i < windows.count; i++) {
				Ref<SAppLayoutSimulationWindow> window = windows[i];
				if (window->isOpened()) {
					window->close();
				}
			}
		}
		m_layoutSimulationWindows.removeAll();
	}

	sl_bool SAppDocument::_parseResourcesXml(const String& localNamespace, const String& filePath)
	{
		_log(g_str_log_open_resource_begin.arg(filePath));
		
		XmlParseParam param;
		param.flagLogError = sl_false;
		param.flagSupportCpp11String = sl_true;
		param.setCreatingOnlyElementsAndTexts();
		String16 textXML = File::readAllText16(filePath);
		Ref<XmlDocument> xml = Xml::parseXml16(textXML, param);
		if (param.flagError) {
			_logError(filePath, param.errorLine, param.errorColumn, param.errorMessage);
			return sl_false;
		}
		if (xml.isNull()) {
			return sl_false;
		}
		Ref<XmlElement> root = xml->getRoot();
		if (root.isNull()) {
			return sl_false;
		}
		if (root->getName() != "sapp") {
			_logError(root, g_str_error_invalid_root_tag);
			return sl_false;
		}
		
		String fileName = File::getFileNameOnly(filePath);
		Locale locale = Locale::Unknown;
		if (fileName.startsWith("strings_")) {
			locale = Locale(fileName.substring(8));
			if (locale.isInvalid()) {
				locale = Locale::Unknown;
			}
		}

		ListLocker< Ref<XmlElement> > children(root->getChildElements());
		sl_size i;
		for (i = 0; i < children.count; i++) {
			Ref<XmlElement>& child = children[i];
			if (child.isNotNull()) {
				String type = child->getName().toLower();
				if (type == "strings" || type == "string") {
					if (type == "strings") {
						if (!_parseStringResources(localNamespace, child, locale, textXML)) {
							return sl_false;
						}
					} else {
						if (!_parseStringResource(localNamespace, child, locale, textXML)) {
							return sl_false;
						}
					}
				} else if (type == "color") {
					if (!_parseColorResource(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "nine-pieces") {
					if (!_parseNinePiecesDrawableResource(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "nine-patch") {
					if (!_parseNinePatchDrawableResource(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "menu" || type == "popupmenu" || type == "popup-menu") {
					if (!_parseMenuResource(localNamespace, child, type != "menu")) {
						return sl_false;
					}
				} else if (type == "layout-include" || type == "include") {
					if (!_parseLayoutInclude(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "layout-style" || type == "style") {
					if (!_parseLayoutStyle(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "unit") {
					if (!_parseLayoutUnit(localNamespace, child)) {
						return sl_false;
					}
				} else if (type == "layout") {
				} else {
					_logError(child, g_str_error_invalid_tag.arg(child->getName()));
					return sl_false;
				}
			}
		}
		for (i = 0; i < children.count; i++) {
			Ref<XmlElement>& child = children[i];
			if (child.isNotNull()) {
				if (child->getName() == "layout") {
					if (!_parseLayoutResource(filePath, localNamespace, child, textXML)) {
						return sl_false;
					}
				}
			}
		}
		return sl_true;
	}

	sl_bool SAppDocument::_generateResourcesH(const String& targetPath)
	{
		_log(g_str_log_generate_cpp_resources_h_begin);
		
		String content =
			"#pragma once\r\n\r\n"
			"#include \"raws.h\"\r\n"
			"#include \"strings.h\"\r\n"
			"#include \"colors.h\"\r\n"
			"#include \"drawables.h\"\r\n"
			"#include \"menus.h\"\r\n"
			"#include \"layouts.h\"\r\n";
		
		String pathHeader = targetPath + "/resources.h";
		if (File::readAllTextUTF8(pathHeader) != content) {
			if (!(File::writeAllTextUTF8(pathHeader, content))) {
				_logError(g_str_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}
		}		
		return sl_true;
	}
	
	template <class MAP, class ITEM>
	sl_bool SAppDocument::getItemFromMap(const MAP& map, const String& localNamespace, const String& name, String* outName, ITEM* _out)
	{
		if (localNamespace.isNotEmpty()) {
			String localName = getNameInLocalNamespace(localNamespace, name);
			if (map.get(localName, _out)) {
				if (outName) {
					*outName = localName;
				}
				return sl_true;
			}
		}
		if (map.get(name, _out)) {
			if (outName) {
				*outName = name;
			}
			return sl_true;
		}
		return sl_false;
	}
	
}

#include "sapp_document_raw.inc"
#include "sapp_document_string.inc"
#include "sapp_document_color.inc"
#include "sapp_document_drawable.inc"
#include "sapp_document_menu.inc"
#include "sapp_document_layout.inc"
