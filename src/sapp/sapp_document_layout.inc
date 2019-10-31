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

	enum LAYOUT_OP
	{
		OP_PARSE,
		OP_GENERATE_CPP,
		OP_SIMULATE
	};

	sl_bool SAppDocument::_parseLayoutStyle(const String& localNamespace, const Ref<XmlElement>& element)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		Ref<SAppLayoutStyle> style = new SAppLayoutStyle;
		if (style.isNull()) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		style->element = element;

		String name = element->getAttribute("name").trim();
		if (name.isEmpty()) {
			_logError(element, g_str_error_resource_layout_name_is_empty);
			return sl_false;
		}
		
		name = getNameInLocalNamespace(localNamespace, name);
		
		if (m_layoutStyles.find(name)) {
			_logError(element, g_str_error_resource_layout_name_redefined.arg(name));
			return sl_false;
		}
		style->name = name;
		
		if (!(m_layoutStyles.put(name, style))) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		String strInherit = element->getAttribute("inherit").trim();
		if (strInherit.isNotEmpty()) {
			ListLocker<String> arr(strInherit.split(","));
			for (sl_size i = 0; i < arr.count; i++) {
				String s = arr[i].trim();
				Ref<SAppLayoutStyle> inheritStyle;
				getItemFromMap(m_layoutStyles, localNamespace, s, sl_null, &inheritStyle);
				if (inheritStyle.isNotNull()) {
					style->inherit.add(inheritStyle);
				} else {
					_logError(element, g_str_error_layout_style_not_found.arg(s));
					return sl_false;
				}
			}
		}

		return sl_true;
	}

	sl_bool SAppDocument::_parseLayoutInclude(const String& localNamespace, const Ref<XmlElement>& element)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		Ref<SAppLayoutInclude> include = new SAppLayoutInclude;
		if (include.isNull()) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		include->element = element;
		
		String name = element->getAttribute("name").trim();
		if (name.isEmpty()) {
			_logError(element, g_str_error_resource_layout_name_is_empty);
			return sl_false;
		}
		
		name = getNameInLocalNamespace(localNamespace, name);
		
		if (m_layoutIncludes.find(name)) {
			_logError(element, g_str_error_resource_layout_name_redefined.arg(name));
			return sl_false;
		}
		include->name = name;
		
		if (!(m_layoutIncludes.put(name, include))) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		return sl_true;
	}

	sl_bool SAppDocument::_parseLayoutUnit(const String& localNamespace, const Ref<XmlElement>& element)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		String name = element->getAttribute("name").trim();
		if (name.isEmpty()) {
			_logError(element, g_str_error_resource_layout_name_is_empty);
			return sl_false;
		}
		
		name = getNameInLocalNamespace(localNamespace, name);
		
		if (m_layoutUnits.find(name)) {
			_logError(element, g_str_error_resource_layout_name_redefined.arg(name));
			return sl_false;
		}
		
		String strValue = element->getText();
		
		SAppDimensionValue value;
		if (!(value.parse(strValue, this))) {
			_logError(element, g_str_error_resource_layout_value_invalid.arg(strValue));
			return sl_false;
		}

		if (!(m_layoutUnits.put(name, value))) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		return sl_true;
	}
	
	sl_bool SAppDocument::_parseLayoutResource(const String& filePath, const String& localNamespace, const Ref<XmlElement>& element, const String16& source)
	{
		if (element.isNull()) {
			return sl_false;
		}
		
		Ref<SAppLayoutResource> layout = new SAppLayoutResource;
		if (layout.isNull()) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		layout->filePath = filePath;
		layout->element = element;

		SAppLayoutType type;
		String strType = element->getAttribute("type");
		if (strType.isEmpty() || strType == "view") {
			type = SAppLayoutType::View;
		} else if (strType == "window") {
			type = SAppLayoutType::Window;
		} else if (strType == "page") {
			type = SAppLayoutType::Page;
		} else {
			_logError(element, g_str_error_resource_layout_type_invalid.arg(strType));
			return sl_false;
		}
		layout->layoutType = type;
		
		if (m_layouts.find(localNamespace)) {
			_logError(element, g_str_error_resource_layout_name_redefined.arg(localNamespace));
			return sl_false;
		}
		layout->name = localNamespace;
		
		layout->baseClassName = layout->getXmlAttribute("base");

		if (!(_parseLayoutResourceItem(layout.get(), layout.get(), sl_null, source))) {
			return sl_false;
		}
		
		if (!(m_layouts.put(localNamespace, layout))) {
			_logError(element, g_str_error_out_of_memory);
			return sl_false;
		}
		
		return sl_true;
	}
	
	void SAppDocument::_openLayoutResource(SAppLayoutResource* layout, const String& fileName)
	{
		Ref<SAppLayoutResource> layoutImport;
		if (!(m_layouts.get(fileName, &layoutImport))) {
			do {
				String path = File::getParentDirectoryPath(layout->filePath) + "/" + fileName;
				if (File::isFile(path + ".xml")) {
					path += ".xml";
				} else if (File::isFile(path + ".uiml")) {
					path += ".uiml";
				} else {
					break;
				}
				_openUiResource(path);
			} while (0);
			m_layouts.emplace(fileName, sl_null);
		}
	}

	sl_bool SAppDocument::_parseLayoutResourceItem(SAppLayoutResource* layout, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent, const String16& source)
	{
		Ref<XmlElement> element = item->element;
		if (element.isNull()) {
			return sl_false;
		}
		
		String strStyles = element->getAttribute("styles").trim();
		if (strStyles.isNotEmpty()) {
			ListLocker<String> arr(strStyles.split(","));
			for (sl_size i = 0; i < arr.count; i++) {
				String s = arr[i].trim();
				Ref<SAppLayoutStyle> style;
				getItemFromMap(m_layoutStyles, layout->name, s, sl_null, &style);
				if (style.isNotNull()) {
					item->styles.add(style);
				} else {
					_logError(element, g_str_error_layout_style_not_found.arg(s));
					return sl_false;
				}
			}
		}
		strStyles = element->getAttribute("style").trim();
		if (strStyles.isNotEmpty()) {
			ListLocker<String> arr(strStyles.split(","));
			for (sl_size i = 0; i < arr.count; i++) {
				String s = arr[i].trim();
				Ref<SAppLayoutStyle> style;
				getItemFromMap(m_layoutStyles, layout->name, s, sl_null, &style);
				if (style.isNotNull()) {
					item->styles.add(style);
				} else {
					_logError(element, g_str_error_layout_style_not_found.arg(s));
					return sl_false;
				}
			}
		}
		
		if (parent) {
			String name = element->getAttribute("name");
			String arrayName;
			sl_int32 arrayIndex = -1;
			if (name.isNotEmpty()) {
				if (!(SAppUtil::checkNameOrArrayMember(name, &arrayName, &arrayIndex))) {
					_logError(element, g_str_error_resource_layout_name_invalid.arg(name));
					return sl_false;
				}
				if (layout->itemsByName.find(name)) {
					_logError(element, g_str_error_resource_layout_name_redefined.arg(name));
					return sl_false;
				}
				if (layout->radioGroups.find(name)) {
					_logError(element, g_str_error_resource_layout_name_redefined.arg(name));
					return sl_false;
				}
			} else {
				name = layout->getAutoIncreasingName(item->itemType);
				item->flagGeneratedName = sl_true;
			}
			item->name = name;
			item->arrayName = arrayName;
			item->arrayIndex = arrayIndex;
		}
		
		LayoutControlProcessParams pp;
		pp.op = OP_PARSE;
		pp.source = source;
		pp.resource = layout;
		pp.resourceItem = item;
		pp.parentResourceItem = parent;
		if (!(_processLayoutResourceControl(&pp))) {
			return sl_false;
		}
		
		String customClassName = item->getXmlAttribute("class").trim();
		if (customClassName.isNotNull()) {
			item->className = customClassName;
		}
		
		if (customClassName.isNotEmpty()) {
			if (!(layout->customClasses.put(customClassName, sl_true))) {
				_logError(element, g_str_error_out_of_memory);
				return sl_false;
			}
		}
		
		if (!parent) {
			String strSP = layout->getXmlAttribute("sp");
			if (!(layout->sp.parse(strSP, this))) {
				_logError(element, g_str_error_resource_layout_attribute_invalid.arg("sp", strSP));
			}
			if (!(layout->sp.checkSP())) {
				_logError(element, g_str_error_resource_layout_attribute_invalid.arg("sp", strSP));
				return sl_false;
			}
		}
		
		if (parent) {
			if (!(layout->itemsByName.put(item->name, item))) {
				_logError(element, g_str_error_out_of_memory);
				return sl_false;
			}
		}
		if (item->arrayIndex >= 0) {
			sl_uint32 n = item->arrayIndex + 1;
			SAppLayoutResource::ItemArrayDesc desc;
			if (layout->itemArrays.get(item->arrayName, &desc)) {
				if (desc.className != item->className) {
					_logError(element, g_str_error_resource_layout_name_array_item_class_different.arg(item->name));
				}
				if (desc.itemsCount < n) {
					desc.itemsCount = n;
					layout->itemArrays.put(item->arrayName, desc);
				}
			} else {
				desc.className = item->className;
				desc.itemsCount = n;
				layout->itemArrays.put(item->arrayName, desc);
			}
		}
		
		return sl_true;
	}

	Ref<SAppLayoutResourceItem> SAppDocument::_parseLayoutResourceItemChild(SAppLayoutResource* layout, SAppLayoutResourceItem* parentItem, const Ref<XmlElement>& element, const String16& source)
	{
		String strType = element->getName();
		SAppLayoutItemType type = SAppLayoutResource::getTypeFromName(strType);
		if (type == SAppLayoutItemType::Unknown) {
			_logError(element, g_str_error_resource_layout_type_invalid.arg(strType));
			return Ref<SAppLayoutResourceItem>::null();
		}
		
		Ref<SAppLayoutResourceItem> childItem = new SAppLayoutResourceItem;
		if (childItem.isNull()) {
			_logError(element, g_str_error_out_of_memory);
			return Ref<SAppLayoutResourceItem>::null();
		}
		
		childItem->itemType = type;
		childItem->element = element;
		
		if (!(_parseLayoutResourceItem(layout, childItem.get(), parentItem, source))) {
			return Ref<SAppLayoutResourceItem>::null();
		}
		
		return childItem;
	}

	sl_bool SAppDocument::_generateLayoutsCpp(const String& targetPath)
	{
		_log(g_str_log_generate_cpp_layouts_begin);
		
		if (!(File::isDirectory(targetPath + "/ui"))) {
			File::createDirectory(targetPath + "/ui");
			if (!(File::isDirectory(targetPath + "/ui"))) {
				_log(g_str_error_directory_create_failed.arg(targetPath + "/ui"));
				return sl_false;
			}
		}

		StringBuffer sbHeader, sbHeaderBase, sbCpp;
		sbHeaderBase.add("#pragma once\r\n\r\n#include <slib/ui/resource.h>\r\n\r\n");

		{
			ListLocker<String> includes(m_conf.generate_cpp_layout_include_headers);
			for (sl_size i = 0; i < includes.count; i++) {
				if (includes[i].isNotEmpty()) {
					sbHeaderBase.add(String::format("#include \"%s\"%n", includes[i]));
				}
			}
		}

		sbCpp.add(String::format(
								 "#include \"layouts.h\"%n%n"
								 "#include \"strings.h\"%n"
								 "#include \"colors.h\"%n"
								 "#include \"drawables.h\"%n"
								 "#include \"menus.h\"%n%n"
								 "#include <slib/ui.h>%n%n"
								 , m_conf.generate_cpp_namespace));
		
		{
			ListLocker<String> includes(m_conf.generate_cpp_layout_include_headers_in_cpp);
			for (sl_size i = 0; i < includes.count; i++) {
				if (includes[i].isNotEmpty()) {
					sbCpp.add(String::format("#include \"%s\"%n", includes[i]));
				}
			}
		}
		
		sbHeaderBase.add(String::format("%n" "namespace %s%n" "{%n\tnamespace ui%n\t{%n", m_conf.generate_cpp_namespace));
		{
			for (auto& pair : m_layouts) {
				if (pair.value.isNotNull()) {
					Ref<SAppLayoutResource> layout = pair.value;
					sbHeaderBase.add(String::format("\t\tclass %s;%n", pair.key));
				}
			}
		}
		sbHeaderBase.add("\t}\r\n}\r\n");

		{
			String headerPrefix = sbHeader.merge();
			for (auto& pair : m_layouts) {
				if (pair.value.isNotNull()) {
					sbHeader.add(String::format("#include \"ui/%s.h\"%n", pair.key));
					sbCpp.add(String::format("#include \"ui/%s.cpp.inc\"%n", pair.key));
					if (!(_generateLayoutsCpp_Layout(targetPath, pair.value.get()))) {
						return sl_false;
					}
				}
			}
		}
		
		String pathHeaderBase = targetPath + "/layouts_base.h";
		String contentHeaderBase = sbHeaderBase.merge();
		if (File::readAllTextUTF8(pathHeaderBase) != contentHeaderBase) {
			if (!(File::writeAllTextUTF8(pathHeaderBase, contentHeaderBase))) {
				_logError(g_str_error_file_write_failed.arg(pathHeaderBase));
				return sl_false;
			}
		}
		
		String pathHeader = targetPath + "/layouts.h";
		String contentHeader = sbHeader.merge();
		if (File::readAllTextUTF8(pathHeader) != contentHeader) {
			if (!(File::writeAllTextUTF8(pathHeader, contentHeader))) {
				_logError(g_str_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}
		}
		
		String pathCpp = targetPath + "/layouts.cpp";
		String contentCpp = sbCpp.merge();
		if (File::readAllTextUTF8(pathCpp) != contentCpp) {
			if (!(File::writeAllTextUTF8(pathCpp, contentCpp))) {
				_logError(g_str_error_file_write_failed.arg(pathCpp));
				return sl_false;
			}
		}
		
		return sl_true;
	}
	
	sl_bool SAppDocument::_generateLayoutsCpp_Layout(const String& targetPath, SAppLayoutResource* layout)
	{
		
		String name = layout->name;
		
		StringBuffer sbHeader, sbCpp;
		
		sbHeader.add("#pragma once\r\n\r\n#include \"../layouts_base.h\"\r\n\r\n");

		String namespacePrefix = String::format("namespace %s%n" "{%n\tnamespace ui%n\t{%n" , m_conf.generate_cpp_namespace);
		sbHeader.add(namespacePrefix);
		sbCpp.add(namespacePrefix);
		
		if (layout->baseClassName.isNotEmpty()) {
			sbHeader.add(String::format("\t\tSLIB_DECLARE_UILAYOUT_BEGIN(%s, %s)%n", name, layout->baseClassName));
			sbCpp.add(String::format("\t\tSLIB_DEFINE_UILAYOUT(%s, %s)%n%n", name, layout->baseClassName));
		} else {
			if (layout->layoutType == SAppLayoutType::Window) {
				sbHeader.add(String::format("\t\tSLIB_DECLARE_WINDOW_LAYOUT_BEGIN(%s)%n", name));
				sbCpp.add(String::format("\t\tSLIB_DEFINE_WINDOW_LAYOUT(%s)%n%n", name));
			} else if (layout->layoutType == SAppLayoutType::Page) {
				sbHeader.add(String::format("\t\tSLIB_DECLARE_PAGE_LAYOUT_BEGIN(%s)%n", name));
				sbCpp.add(String::format("\t\tSLIB_DEFINE_PAGE_LAYOUT(%s)%n%n", name));
			} else if (layout->layoutType == SAppLayoutType::View) {
				sbHeader.add(String::format("\t\tSLIB_DECLARE_VIEW_LAYOUT_BEGIN(%s)%n", name));
				sbCpp.add(String::format("\t\tSLIB_DEFINE_VIEW_LAYOUT(%s)%n%n", name));
			} else {
				return sl_false;
			}
		}
		
		sbCpp.add(String::format("\t\tvoid %s::initialize()%n\t\t{%n", name));
		
		{
			ListElements<String> radioGroups(layout->radioGroups.getAllKeys());
			for (sl_size i = 0; i < radioGroups.count; i++) {
				sbHeader.add(String::format("\t\t\tslib::Ref<slib::RadioGroup> %s;%n", radioGroups[i]));
				sbCpp.add(String::format("\t\t\t%s = new slib::RadioGroup;%n", radioGroups[i]));
			}
			if (radioGroups.count > 0) {
				sbHeader.add("\r\n");
				sbCpp.add("\r\n");
			}
		}
		{
			ObjectLocker lock(&(layout->itemArrays));
			for (auto& item : layout->itemArrays) {
				sbHeader.add(String::format("\t\t\tslib::Ref<%s> %s[%d];%n", item.value.className, item.key, item.value.itemsCount));
			}
			if (layout->itemArrays.isNotEmpty()) {
				sbHeader.add("\r\n");
			}
		}
		
		StringBuffer sbLayout;
		
		if (layout->sp.flagDefined) {
			if (layout->sp.isNeededOnLayoutFunction()) {
				sbLayout.add(String::format("%n\t\t\tsetScaledPixel(%s);%n", layout->sp.getAccessString()));
			} else {
				sbCpp.add(String::format("%n\t\t\tsetScaledPixel(%s);%n%n", layout->sp.getAccessString()));
			}
		}
		
		if (!(_generateLayoutsCpp_Item(layout, layout, sl_null, sbHeader, sbCpp, sbLayout, String::null()))) {
			return sl_false;
		}
		
		sbCpp.add(String::format("\t\t}%n%n\t\tvoid %s::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)%n\t\t{%n", name));
		sbCpp.link(sbLayout);
		static sl_char8 strEndCpp[] = "\t\t}\r\n\r\n";
		sbCpp.addStatic(strEndCpp, sizeof(strEndCpp)-1);
		
		if (layout->baseClassName.isNotEmpty()) {
			static sl_char8 strEndHeader[] = "\t\tSLIB_DECLARE_UILAYOUT_END\r\n\r\n";
			sbHeader.addStatic(strEndHeader, sizeof(strEndHeader)-1);
		} else {
			if (layout->layoutType == SAppLayoutType::Window) {
				static sl_char8 strEndHeader[] = "\t\tSLIB_DECLARE_WINDOW_LAYOUT_END\r\n\r\n";
				sbHeader.addStatic(strEndHeader, sizeof(strEndHeader)-1);
			} else if (layout->layoutType == SAppLayoutType::Page) {
				static sl_char8 strEndHeader[] = "\t\tSLIB_DECLARE_PAGE_LAYOUT_END\r\n\r\n";
				sbHeader.addStatic(strEndHeader, sizeof(strEndHeader)-1);
			} else if (layout->layoutType == SAppLayoutType::View) {
				static sl_char8 strEndHeader[] = "\t\tSLIB_DECLARE_VIEW_LAYOUT_END\r\n\r\n";
				sbHeader.addStatic(strEndHeader, sizeof(strEndHeader)-1);
			}
		}
		
		sbHeader.add("\t}\r\n}\r\n");
		sbCpp.add("\t}\r\n}\r\n");

		String pathHeader = targetPath + "/ui/" + name + ".h";
		String contentHeader = sbHeader.merge();
		if (File::readAllTextUTF8(pathHeader) != contentHeader) {
			if (!(File::writeAllTextUTF8(pathHeader, contentHeader))) {
				_logError(g_str_error_file_write_failed.arg(pathHeader));
				return sl_false;
			}
		}
		
		String pathCpp = targetPath + "/ui/" + name + ".cpp.inc";
		String contentCpp = sbCpp.merge();
		if (File::readAllTextUTF8(pathCpp) != contentCpp) {
			if (!(File::writeAllTextUTF8(pathCpp, contentCpp))) {
				_logError(g_str_error_file_write_failed.arg(pathCpp));
				return sl_false;
			}
		}

		return sl_true;
		
	}

	sl_bool SAppDocument::_generateLayoutsCpp_Item(SAppLayoutResource* layout, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent, StringBuffer& sbDeclare, StringBuffer& sbDefineInit, StringBuffer& sbDefineLayout, const String& addStatement)
	{
		String name;
		if (parent) {
			name = item->name;
			if (item->arrayIndex < 0) {
				sbDeclare.add(String::format("\t\t\tslib::Ref<%s> %s;%n", item->className, name));
			}
			sbDefineInit.add(String::format("\t\t\t%2$s = new %1$s;%n", item->className, name));
		} else {
			name = "this";
		}
		
		LayoutControlProcessParams pp;
		pp.op = OP_GENERATE_CPP;
		pp.resource = layout;
		pp.resourceItem = item;
		pp.parentResourceItem = parent;
		pp.addStatement = addStatement;
		pp.name = name;
		pp.sbDeclare = &sbDeclare;
		pp.sbDefineInit = &sbDefineInit;
		pp.sbDefineLayout = &sbDefineLayout;
		
		if (!(_processLayoutResourceControl(&pp))) {
			return sl_false;
		}
		
		return sl_true;
	}

	void SAppDocument::_simulateLayoutInWindow(SAppLayoutResource* layout, const SAppSimulateLayoutParam& param)
	{
		Ref<SAppLayoutSimulationWindow> window = new SAppLayoutSimulationWindow;
		if (window.isNotNull()) {
			if (layout->layoutType == SAppLayoutType::Window) {
				window->setOnClose(param.onCloseWindow);
			} else {
				window->setClientSize(param.pageSize);
				window->setOnClose(param.onClosePage);
			}
			window->open(this, layout);
		}
	}

	void SAppDocument::_registerLayoutSimulationWindow(const Ref<SAppLayoutSimulationWindow>& window)
	{
		m_layoutSimulationWindows.add(window);
	}

	void SAppDocument::_removeLayoutSimulationWindow(const Ref<SAppLayoutSimulationWindow>& window)
	{
		m_layoutSimulationWindows.remove(window);
	}

	Ref<View> SAppDocument::_simulateLayoutCreateOrLayoutView(SAppLayoutSimulator* simulator, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent, View* parentView, sl_bool flagOnLayout)
	{
		Ref<SAppLayoutSimulationWindow> window = simulator->getSimulationWindow();
		if (window.isNull()) {
			return Ref<View>::null();
		}
		Ref<SAppLayoutResource> layout = simulator->getLayoutResource();
		if (layout.isNull()) {
			return Ref<View>::null();
		}
		
		Ref<View> view;
		if (parent) {
			if (flagOnLayout) {
				view = simulator->getViewByName(item->name);
				if (view.isNull()) {
					return Ref<View>::null();
				}
			}
		} else {
			view = simulator->getSimulationContentView();
			if (view.isNull()) {
				return Ref<View>::null();
			}
			if (layout->layoutType == SAppLayoutType::Window) {
				UISize size = UI::getScreenSize();
				m_layoutSimulationParams.screenWidth = size.x;
				m_layoutSimulationParams.screenHeight = size.y;
				size = window->getClientSize();
				m_layoutSimulationParams.viewportWidth = size.x;
				m_layoutSimulationParams.viewportHeight = size.y;
			} else {
				UISize size = window->getClientSize();
				m_layoutSimulationParams.screenWidth = size.x;
				m_layoutSimulationParams.screenHeight = size.y;
				m_layoutSimulationParams.viewportWidth = view->getWidth();
				m_layoutSimulationParams.viewportHeight = view->getHeight();
			}
			if (layout->sp.flagDefined) {
				m_layoutSimulationParams.sp = _getDimensionFloatValue(layout->sp);
			} else {
				m_layoutSimulationParams.sp = 1;
			}
		}
		
		LayoutControlProcessParams pp;
		pp.op = OP_SIMULATE;
		pp.resource = layout.get();
		pp.resourceItem = item;
		pp.parentResourceItem = parent;
		pp.simulator = simulator;
		pp.window = window.get();
		pp.view = view;
		pp.parentView = parentView;
		pp.flagOnLayout = flagOnLayout;
		if (!(_processLayoutResourceControl(&pp))) {
			return Ref<View>::null();
		}
		
		if (parent) {
			if (!flagOnLayout) {
				view = pp.view;
				if (view.isNull()) {
					return Ref<View>::null();
				}
				simulator->registerViewByName(item->name, view);
			}
		}

		return view;
		
	}

	sl_ui_pos SAppDocument::_getDimensionIntValue(const SAppDimensionValue& value)
	{
		if (!(value.flagDefined)) {
			return 0;
		}
		switch (value.unit) {
			case SAppDimensionValue::PX:
				return UIResource::toUiPos(value.amount);
			case SAppDimensionValue::SW:
			case SAppDimensionValue::SAFE_W:
				return UIResource::toUiPos(value.amount * m_layoutSimulationParams.screenWidth);
			case SAppDimensionValue::SH:
			case SAppDimensionValue::SAFE_H:
				return UIResource::toUiPos(value.amount * m_layoutSimulationParams.screenHeight);
			case SAppDimensionValue::SMIN:
				return UIResource::toUiPos(value.amount * SLIB_MIN(m_layoutSimulationParams.screenWidth, m_layoutSimulationParams.screenHeight));
			case SAppDimensionValue::SMAX:
				return UIResource::toUiPos(value.amount * SLIB_MAX(m_layoutSimulationParams.screenWidth, m_layoutSimulationParams.screenHeight));
			case SAppDimensionValue::VW:
				return UIResource::toUiPos(value.amount * m_layoutSimulationParams.viewportWidth);
			case SAppDimensionValue::VH:
				return UIResource::toUiPos(value.amount * m_layoutSimulationParams.viewportHeight);
			case SAppDimensionValue::VMIN:
				return UIResource::toUiPos(value.amount * SLIB_MIN(m_layoutSimulationParams.viewportWidth, m_layoutSimulationParams.viewportHeight));
			case SAppDimensionValue::VMAX:
				return UIResource::toUiPos(value.amount * SLIB_MAX(m_layoutSimulationParams.viewportWidth, m_layoutSimulationParams.viewportHeight));
			case SAppDimensionValue::SP:
				return UIResource::toUiPos(value.amount * m_layoutSimulationParams.sp);
			case SAppDimensionValue::DP:
				return UIResource::toUiPos(slib::UIResource::dpToPixel(value.amount));
			case SAppDimensionValue::PT:
				return UIResource::toUiPos(slib::UIResource::pointToPixel(value.amount));
			case SAppDimensionValue::M:
				return UIResource::toUiPos(slib::UIResource::meterToPixel(value.amount));
			case SAppDimensionValue::CM:
				return UIResource::toUiPos(slib::UIResource::centimeterToPixel(value.amount));
			case SAppDimensionValue::MM:
				return UIResource::toUiPos(slib::UIResource::millimeterToPixel(value.amount));
			case SAppDimensionValue::INCH:
				return UIResource::toUiPos(slib::UIResource::inchToPixel(value.amount));
		}
		return 0;
	}

	sl_real SAppDocument::_getDimensionFloatValue(const SAppDimensionFloatValue& value)
	{
		if (!(value.flagDefined)) {
			return 0;
		}
		switch (value.unit) {
			case SAppDimensionValue::PX:
				return value.amount;
			case SAppDimensionValue::SW:
				return value.amount * (sl_real)(m_layoutSimulationParams.screenWidth);
			case SAppDimensionValue::SH:
				return value.amount * (sl_real)(m_layoutSimulationParams.screenHeight);
			case SAppDimensionValue::SMIN:
				return value.amount * (sl_real)(SLIB_MIN(m_layoutSimulationParams.screenWidth, m_layoutSimulationParams.screenHeight));
			case SAppDimensionValue::SMAX:
				return value.amount * (sl_real)(SLIB_MAX(m_layoutSimulationParams.screenWidth, m_layoutSimulationParams.screenHeight));
			case SAppDimensionValue::VW:
				return value.amount * (sl_real)(m_layoutSimulationParams.viewportWidth);
			case SAppDimensionValue::VH:
				return value.amount * (sl_real)(m_layoutSimulationParams.viewportHeight);
			case SAppDimensionValue::VMIN:
				return value.amount * (sl_real)(SLIB_MIN(m_layoutSimulationParams.viewportWidth, m_layoutSimulationParams.viewportHeight));
			case SAppDimensionValue::VMAX:
				return value.amount * (sl_real)(SLIB_MAX(m_layoutSimulationParams.viewportWidth, m_layoutSimulationParams.viewportHeight));
			case SAppDimensionValue::SP:
				return value.amount * m_layoutSimulationParams.sp;
			case SAppDimensionValue::DP:
				return slib::UIResource::dpToPixel(value.amount);
			case SAppDimensionValue::PT:
				return slib::UIResource::pointToPixel(value.amount);
			case SAppDimensionValue::M:
				return slib::UIResource::meterToPixel(value.amount);
			case SAppDimensionValue::CM:
				return slib::UIResource::centimeterToPixel(value.amount);
			case SAppDimensionValue::MM:
				return slib::UIResource::millimeterToPixel(value.amount);
			case SAppDimensionValue::INCH:
				return slib::UIResource::inchToPixel(value.amount);
		}
		return 0;
	}
	
	List< Ref<XmlElement> > SAppDocument::_getLayoutItemChildElements(SAppLayoutResourceItem* item, const String& localNamespace, const String& tagName)
	{
		List< Ref<XmlElement> > ret;
		if (!_addXmlChildElements(ret, item->element, localNamespace, tagName)) {
			return sl_null;
		}
		{
			ListLocker< Ref<SAppLayoutStyle> > _styles(item->styles);
			for (sl_size i = 0; i < _styles.count; i++) {
				Ref<SAppLayoutStyle> style = _styles[i];
				if (style.isNotNull()) {
					if (!_addLayoutStyleChildElements(ret, style.get(), localNamespace, tagName)) {
						return sl_null;
					}
				}
			}
		}
		return ret;
	}

	sl_bool SAppDocument::_addLayoutStyleChildElements(List< Ref<XmlElement> >& list, SAppLayoutStyle* style, const String& localNamespace, const String& tagName)
	{
		{
			ListLocker< Ref<SAppLayoutStyle> > _styles(style->inherit);
			for (sl_size i = 0; i < _styles.count; i++) {
				Ref<SAppLayoutStyle> other = _styles[i];
				if (other.isNotNull()) {
					if (!_addLayoutStyleChildElements(list, other.get(), localNamespace, tagName)) {
						return sl_false;
					}
				}
			}
		}
		return _addXmlChildElements(list, style->element, localNamespace, tagName);
	}

	sl_bool SAppDocument::_addXmlChildElements(List< Ref<XmlElement> >& list, const Ref<XmlElement>& parent, const String& localNamespace, const String& tagName)
	{
		{
			ListLocker< Ref<XmlElement> > children(parent->getChildElements());
			for (sl_size i = 0; i < children.count; i++) {
				Ref<XmlElement>& child = children[i];
				if (child.isNotNull()) {
					String name = child->getName();
					if (name == "include") {
						String src = child->getAttribute("src");
						if (src.isEmpty()) {
							_logError(child, g_str_error_resource_layout_attribute_invalid.arg("src", name));
							return sl_false;
						}
						Ref<SAppLayoutInclude> include;
						getItemFromMap(m_layoutIncludes, localNamespace, src, sl_null, &include);
						if (include.isNotNull()) {
							if (!_addXmlChildElements(list, include->element, localNamespace, tagName)) {
								return sl_false;
							}
						} else {
							_logError(child, g_str_error_layout_include_not_found.arg(name));
							return sl_false;
						}
					} else if (tagName.isEmpty() || name == tagName) {
						list.add(child);
					}
				}
			}
		}
		return sl_true;
	}

#define PROCESS_CONTROL_SWITCH(NAME) \
	case SAppLayoutItemType::NAME: \
		if (!(_processLayoutResourceControl_##NAME(params))) { \
			return sl_false; \
		} \
		break;

	sl_bool SAppDocument::_processLayoutResourceControl(LayoutControlProcessParams* params)
	{
		m_currentLocalNamespace = params->resource->name;
		SAppLayoutResourceItem* resourceItem = params->resourceItem;
		int op = params->op;
		switch (resourceItem->itemType)
		{
			case SAppLayoutItemType::ViewGroup:
				{
					if (params->parentResourceItem) {
						if (!(_processLayoutResourceControl_ViewGroup(params))) {
							return sl_false;
						}
					} else {
						if (params->resource->layoutType == SAppLayoutType::Window) {
							if (!(_processLayoutResourceControl_Window(params))) {
								return sl_false;
							}
						} else if (params->resource->layoutType == SAppLayoutType::Page) {
							if (!(_processLayoutResourceControl_Page(params))) {
								return sl_false;
							}
						} else {
							if (!(_processLayoutResourceControl_ViewGroup(params))) {
								return sl_false;
							}
						}
					}
				}
				break;
			PROCESS_CONTROL_SWITCH(View)
			PROCESS_CONTROL_SWITCH(Import)
			PROCESS_CONTROL_SWITCH(Button)
			PROCESS_CONTROL_SWITCH(Label)
			PROCESS_CONTROL_SWITCH(Line)
			PROCESS_CONTROL_SWITCH(Check)
			PROCESS_CONTROL_SWITCH(Radio)
			PROCESS_CONTROL_SWITCH(Edit)
			PROCESS_CONTROL_SWITCH(Password)
			PROCESS_CONTROL_SWITCH(TextArea)
			PROCESS_CONTROL_SWITCH(Image)
			PROCESS_CONTROL_SWITCH(Select)
			PROCESS_CONTROL_SWITCH(Scroll)
			PROCESS_CONTROL_SWITCH(Linear)
			PROCESS_CONTROL_SWITCH(List)
			PROCESS_CONTROL_SWITCH(Collection)
			PROCESS_CONTROL_SWITCH(Grid)
			PROCESS_CONTROL_SWITCH(ListReport)
			PROCESS_CONTROL_SWITCH(Render)
			PROCESS_CONTROL_SWITCH(Tab)
			PROCESS_CONTROL_SWITCH(Tree)
			PROCESS_CONTROL_SWITCH(Split)
			PROCESS_CONTROL_SWITCH(Web)
			PROCESS_CONTROL_SWITCH(Progress)
			PROCESS_CONTROL_SWITCH(Slider)
			PROCESS_CONTROL_SWITCH(Switch)
			PROCESS_CONTROL_SWITCH(Picker)
			PROCESS_CONTROL_SWITCH(DatePicker)
			PROCESS_CONTROL_SWITCH(Pager)
			PROCESS_CONTROL_SWITCH(Navigation)
			PROCESS_CONTROL_SWITCH(Video)
			PROCESS_CONTROL_SWITCH(Camera)
			PROCESS_CONTROL_SWITCH(QRCodeScanner)
			PROCESS_CONTROL_SWITCH(Drawer)
			PROCESS_CONTROL_SWITCH(Chat)
			PROCESS_CONTROL_SWITCH(Refresh)
			default:
				return sl_false;
		}
		
		switch (resourceItem->itemType) {
			case SAppLayoutItemType::Scroll:
				break;
			default:
				if (op == OP_PARSE) {
					do {
						if (params->resourceItem->flagNoChildren) {
							break;
						}
						ListLocker< Ref<XmlElement> > children(_getLayoutItemChildElements(resourceItem, params->resource->name, String::null()));
						for (sl_size i = 0; i < children.count; i++) {
							const Ref<XmlElement>& child = children[i];
							if (child.isNotNull()) {
								String tagName = child->getName();
								if (tagName == "item" || tagName == "column" || tagName == "row") {
									continue;
								}
								Ref<SAppLayoutResourceItem> childItem = _parseLayoutResourceItemChild(params->resource, resourceItem, child, params->source);
								if (childItem.isNull()) {
									return sl_false;
								}
								if (resourceItem->itemType == SAppLayoutItemType::Linear) {
									if (!(resourceItem->attrsLinear->orientation.flagDefined) || resourceItem->attrsLinear->orientation.value == LayoutOrientation::Vertical) {
										childItem->attrsView->topMode = PositionMode::Free;
										childItem->attrsView->bottomMode = PositionMode::Free;
									} else {
										childItem->attrsView->leftMode = PositionMode::Free;
										childItem->attrsView->rightMode = PositionMode::Free;
									}
								} else if (resourceItem->itemType == SAppLayoutItemType::Refresh) {
									childItem->attrsView->width.flagDefined = sl_true;
									childItem->attrsView->width.amount = 1;
									childItem->attrsView->width.unit = SAppDimensionValue::FILL;
									childItem->attrsView->height.flagDefined = sl_true;
									childItem->attrsView->height.amount = 1;
									childItem->attrsView->height.unit = SAppDimensionValue::FILL;
								}
								if (!(resourceItem->children.add(childItem))) {
									_logError(resourceItem->element, g_str_error_out_of_memory);
									return sl_false;
								}
							}
						}
					} while (0);
					return sl_true;
				} else if (op == OP_GENERATE_CPP) {
					String name;
					if (params->parentResourceItem) {
						name = params->name;
					} else {
						static sl_char8 strEnd[] = "\r\n";
						params->sbDefineInit->addStatic(strEnd, sizeof(strEnd)-1);
						name = "m_contentView";
					}
					ListLocker< Ref<SAppLayoutResourceItem> > children(resourceItem->children);
					for (sl_size i = 0; i < children.count; i++) {
						Ref<SAppLayoutResourceItem>& child = children[i];
						if (child.isNotNull()) {
							String addStatement = String::format("\t\t\t%s->addChild(%s, slib::UIUpdateMode::Init);%n%n", name, child->name);
							if (!(_generateLayoutsCpp_Item(params->resource, child.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addStatement) )) {
								return sl_false;
							}
						}
					}
				} else if (op == OP_SIMULATE) {
					if (!(params->parentResourceItem) && params->resource->layoutType != SAppLayoutType::Window) {
						m_layoutSimulationParams.viewportWidth = params->view->getWidth();
						m_layoutSimulationParams.viewportHeight = params->view->getHeight();
					}
					ListLocker< Ref<SAppLayoutResourceItem> > children(resourceItem->children);
					for (sl_size i = 0; i < children.count; i++) {
						Ref<SAppLayoutResourceItem>& child = children[i];
						if (child.isNotNull()) {
							Ref<View> childView = _simulateLayoutCreateOrLayoutView(params->simulator, child.get(), resourceItem, params->view.get(), params->flagOnLayout);
							if (childView.isNotNull()) {
								if (!(params->flagOnLayout)) {
									params->view->addChild(childView, UIUpdateMode::Init);
								}
							} else {
								return sl_false;
							}
						}
					}
				}
				break;
		}

		return sl_true;
		
	}

#define BEGIN_PROCESS_LAYOUT_CONTROL(NAME, VIEWTYPE) \
	sl_bool SAppDocument::_processLayoutResourceControl_##NAME(LayoutControlProcessParams* params) \
	{ \
		String strTab = "\t\t\t"; \
		SAppLayoutResource* resource = params->resource; \
		SLIB_UNUSED(resource) \
		SAppLayoutResourceItem* resourceItem = params->resourceItem; \
		Ref<XmlElement> element = resourceItem->element; \
		int op = params->op; \
		String name = params->name; \
		sl_bool flagOnLayout = params->flagOnLayout; \
		Ref<SAppLayout##NAME##Attributes> attr; \
		if (op == OP_PARSE) { \
			attr = new SAppLayout##NAME##Attributes; \
			if (attr.isNull()) { \
				_logError(element, g_str_error_out_of_memory); \
				return sl_false; \
			} \
			resourceItem->attrs##NAME = attr; \
			if (resourceItem->className.isEmpty()) { \
				resourceItem->className = "slib::" #VIEWTYPE; \
			} \
		} else { \
			attr = resourceItem->attrs##NAME; \
			if (op == OP_SIMULATE) { \
				if (!flagOnLayout && params->view.isNull()) { \
					params->view = new VIEWTYPE; \
				} \
			} \
		} \
		VIEWTYPE* view = (VIEWTYPE*)(params->view.get()); \
		SLIB_UNUSED(view)


#define END_PROCESS_LAYOUT_CONTROL \
		return sl_true; \
	}



#define LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(XML, NAME) \
	_logError(XML, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str_##NAME));

#define LAYOUT_CONTROL_PARSE_XML_ATTR(XML, ATTR, NAME) \
	String str_##NAME = XML->getAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME))) { \
		LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(XML, NAME) \
		return sl_false; \
	}

#define LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(XML, ATTR, NAME) \
	String str_##NAME = XML->getAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME, this))) { \
		LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(XML, NAME) \
		return sl_false; \
	}

#define LAYOUT_CONTROL_PARSE_XML_DRAWABLE_ATTR(XML, ATTR, NAME) \
	String str_##NAME = XML->getAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME, this))) { \
		LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(XML, NAME) \
		return sl_false; \
	}

#define LOG_ERROR_LAYOUT_CONTROL_ATTR(NAME) \
	_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str_##NAME));

#define LAYOUT_CONTROL_PARSE_ATTR(ATTR, NAME) \
	String str_##NAME = resourceItem->getXmlAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME))) { \
		LOG_ERROR_LAYOUT_CONTROL_ATTR(NAME) \
		return sl_false; \
	}
	
#define LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(ATTR, NAME) \
	String str_##NAME = resourceItem->getXmlAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME, this))) { \
		LOG_ERROR_LAYOUT_CONTROL_ATTR(NAME) \
		return sl_false; \
	}

#define LAYOUT_CONTROL_PARSE_DRAWABLE_ATTR(ATTR, NAME) \
	String str_##NAME = resourceItem->getXmlAttribute(#NAME); \
	if (!(ATTR NAME.parse(str_##NAME, this))) { \
		LOG_ERROR_LAYOUT_CONTROL_ATTR(NAME) \
		return sl_false; \
	}
	
#define LAYOUT_CONTROL_GENERIC_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, attr->NAME.getAccessString())); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				view->SETFUNC(attr->NAME.value); \
			} \
		} \
	}

#define LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->NAME.getAccessString())); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				view->SETFUNC(attr->NAME.value, UIUpdateMode::Init); \
			} \
		} \
	}


#define LAYOUT_CONTROL_NOPARAM_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined && attr->NAME.value) { \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "();%n", strTab, name)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined && attr->NAME.value) { \
				view->SETFUNC(); \
			} \
		} \
	}
	
#define _LAYOUT_CONTROL_DIMENSION_ATTR(NAME, SETFUNC, CHECKFUNC, TYPE) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str, this))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
		if (!(attr->NAME.CHECKFUNC())) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			if (!(SAppDimensionValue::isRelativeUnit(attr->NAME.unit))) { \
				if (attr->NAME.isNeededOnLayoutFunction()) { \
					params->sbDefineLayout->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, attr->NAME.getAccessString())); \
				} else { \
					params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, attr->NAME.getAccessString())); \
				} \
			} \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				if (!(SAppDimensionValue::isRelativeUnit(attr->NAME.unit))) { \
					view->SETFUNC(_getDimension##TYPE##Value(attr->NAME)); \
				} \
			} \
		} \
	}

#define _LAYOUT_CONTROL_DIMENSION_ATTR_NOREDRAW(NAME, SETFUNC, CHECKFUNC, TYPE) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str, this))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
		if (!(attr->NAME.CHECKFUNC())) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			if (!(SAppDimensionValue::isRelativeUnit(attr->NAME.unit))) { \
				if (attr->NAME.isNeededOnLayoutFunction()) { \
					params->sbDefineLayout->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, attr->NAME.getAccessString())); \
				} else { \
					params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->NAME.getAccessString())); \
				} \
			} \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				if (!(SAppDimensionValue::isRelativeUnit(attr->NAME.unit))) { \
					view->SETFUNC(_getDimension##TYPE##Value(attr->NAME)); \
				} \
			} \
		} \
	}

#define LAYOUT_CONTROL_INT_DIMENSION_ATTR(NAME, SETFUNC, CHECKFUNC) _LAYOUT_CONTROL_DIMENSION_ATTR(NAME, SETFUNC, CHECKFUNC, Int)
#define LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(NAME, SETFUNC, CHECKFUNC) _LAYOUT_CONTROL_DIMENSION_ATTR_NOREDRAW(NAME, SETFUNC, CHECKFUNC, Int)
#define LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR(NAME, SETFUNC, CHECKFUNC) _LAYOUT_CONTROL_DIMENSION_ATTR(NAME, SETFUNC, CHECKFUNC, Float)
#define LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(NAME, SETFUNC, CHECKFUNC) _LAYOUT_CONTROL_DIMENSION_ATTR_NOREDRAW(NAME, SETFUNC, CHECKFUNC, Float)
	
	
#define LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str, this))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
		if (params->parentResourceItem == sl_null) { \
			if (!(attr->NAME.checkForRootViewMargin())) { \
				_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
				return sl_false; \
			} \
		} else { \
			if (!(attr->NAME.checkMargin())) { \
				_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
				return sl_false; \
			} \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			if (attr->NAME.isNeededOnLayoutFunction()) { \
				if (attr->NAME.unit == SAppDimensionValue::WEIGHT) { \
					params->sbDefineLayout->add(String::format("%s%s->" #SETFUNC "Weight(%ff);%n", strTab, name, attr->NAME.amount)); \
				} else { \
					params->sbDefineLayout->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, attr->NAME.getAccessString())); \
				} \
			} else { \
				if (attr->NAME.unit == SAppDimensionValue::WEIGHT) { \
					params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "Weight(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->NAME.amount)); \
				} else { \
					params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->NAME.getAccessString())); \
				} \
			} \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				if (attr->NAME.unit == SAppDimensionValue::WEIGHT) { \
					view->SETFUNC##Weight(attr->NAME.amount); \
				} else { \
					view->SETFUNC(_getDimensionIntValue(attr->NAME)); \
				} \
			} \
		} \
	}

#define LAYOUT_CONTROL_MENU_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			String _accessString; \
			if (!(_getMenuAccessString(resource->name, attr->NAME, element, _accessString))) { \
				return sl_false; \
			} \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, _accessString)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				Ref<Menu> _menuValue; \
				if (!(_getMenuValue(resource->name, attr->NAME, element, _menuValue))) { \
					return sl_false; \
				} \
				view->SETFUNC(_menuValue); \
			} \
		} \
	}


#define LAYOUT_CONTROL_STRING_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			String _accessString; \
			if (!(_getStringAccessString(resource->name, attr->NAME, element, _accessString))) { \
				return sl_false; \
			} \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s);%n", strTab, name, _accessString)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				String _stringValue; \
				if (!(_getStringValue(resource->name, attr->NAME, element, _stringValue))) { \
					return sl_false; \
				} \
				view->SETFUNC(_stringValue); \
			} \
		} \
	}


#define LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			String _accessString; \
			if (!(_getStringAccessString(resource->name, attr->NAME, element, _accessString))) { \
				return sl_false; \
			} \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, _accessString)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				String _stringValue; \
				if (!(_getStringValue(resource->name, attr->NAME, element, _stringValue))) { \
					return sl_false; \
				} \
				view->SETFUNC(_stringValue); \
			} \
		} \
	}


#define LAYOUT_CONTROL_DRAWABLE_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str, this))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			String _accessString; \
			if (!(_getDrawableAccessString(resource->name, attr->NAME, element, _accessString))) { \
				return sl_false; \
			} \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, _accessString)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				Ref<Drawable> _drawableValue; \
				if (!(_getDrawableValue(resource->name, attr->NAME, element, _drawableValue))) { \
					return sl_false; \
				} \
				view->SETFUNC(_drawableValue); \
			} \
		} \
	}


#define LAYOUT_CONTROL_COLOR_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str = resourceItem->getXmlAttribute(#NAME); \
		if (!(attr->NAME.parse(str))) { \
			_logError(element, g_str_error_resource_layout_attribute_invalid.arg(#NAME, str)); \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.flagDefined) { \
			String _accessString; \
			if (!(_getColorAccessString(resource->name, attr->NAME, element, _accessString))) { \
				return sl_false; \
			} \
			params->sbDefineInit->add(String::format("%s%s->" #SETFUNC "(%s, slib::UIUpdateMode::Init);%n", strTab, name, _accessString)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.flagDefined) { \
				Color _colorValue; \
				if (!(_getColorValue(resource->name, attr->NAME, element, _colorValue))) { \
					return sl_false; \
				} \
				view->SETFUNC(_colorValue); \
			} \
		} \
	}

#define LAYOUT_CONTROL_FONT_ATTR(NAME, SETFUNC) \
	if (op == OP_PARSE) { \
		String str_family = resourceItem->getXmlAttribute(#NAME "Family"); \
		if (!(attr->NAME.family.parse(str_family))) { \
			LOG_ERROR_LAYOUT_CONTROL_ATTR(family) \
			return sl_false; \
		} \
		String str_size = resourceItem->getXmlAttribute(#NAME "Size"); \
		if (!(attr->NAME.size.parse(str_size, this))) { \
			LOG_ERROR_LAYOUT_CONTROL_ATTR(size) \
			return sl_false; \
		} \
		if (params->parentResourceItem == sl_null) { \
			if (!(attr->NAME.size.checkForRootViewPosition())) { \
				LOG_ERROR_LAYOUT_CONTROL_ATTR(size) \
				return sl_false; \
			} \
		} else { \
			if (!(attr->NAME.size.checkPosition())) { \
				LOG_ERROR_LAYOUT_CONTROL_ATTR(size) \
				return sl_false; \
			} \
		} \
		String str_bold = resourceItem->getXmlAttribute(#NAME "Bold"); \
		if (!(attr->NAME.bold.parse(str_bold))) { \
			LOG_ERROR_LAYOUT_CONTROL_ATTR(bold) \
			return sl_false; \
		} \
		String str_italic = resourceItem->getXmlAttribute(#NAME "Italic"); \
		if (!(attr->NAME.italic.parse(str_italic))) { \
			LOG_ERROR_LAYOUT_CONTROL_ATTR(italic) \
			return sl_false; \
		} \
		String str_underline = resourceItem->getXmlAttribute(#NAME "Underline"); \
		if (!(attr->NAME.underline.parse(str_underline))) { \
			LOG_ERROR_LAYOUT_CONTROL_ATTR(underline) \
			return sl_false; \
		} \
	} else if (op == OP_GENERATE_CPP) { \
		if (attr->NAME.isDefined()) { \
			String size; \
			if (attr->NAME.size.flagDefined) { \
				size = attr->NAME.size.getAccessString(); \
			} else { \
				size = "slib::UI::getDefaultFontSize()"; \
			} \
			StringBuffer* sb; \
			String strUpdateMode; \
			if (attr->NAME.size.isNeededOnLayoutFunction()) { \
				sb = params->sbDefineLayout; \
				strUpdateMode = "slib::UIUpdateMode::UpdateLayout"; \
			} else { \
				sb = params->sbDefineInit; \
				strUpdateMode = "slib::UIUpdateMode::Init"; \
			} \
			if (attr->NAME.family.flagDefined) { \
				String strFamily; \
				if (_getStringAccessString(resource->name, attr->NAME.family, element, strFamily)) { \
					sb->add(String::format("%s%s->" #SETFUNC "(slib::Font::create(%s, ", strTab, name, strFamily)); \
				} else { \
					return sl_false; \
				} \
			} else { \
				sb->add(String::format("%s%s->" #SETFUNC "(slib::Font::create(slib::UI::getDefaultFontFamily(), ", strTab, name, name)); \
			} \
			sb->add(String::format("%s, %s, %s, %s), %s);%n", size, attr->NAME.bold.value?"sl_true":"sl_false", attr->NAME.italic.value?"sl_true":"sl_false", attr->NAME.underline.value?"sl_true":"sl_false", strUpdateMode)); \
		} \
	} else if (op == OP_SIMULATE) { \
		if (flagOnLayout) { \
			if (attr->NAME.isDefined()) { \
				sl_real size; \
				if (attr->NAME.size.flagDefined) { \
					size = _getDimensionFloatValue(attr->NAME.size); \
				} else { \
					size = UI::getDefaultFontSize(); \
				} \
				if (attr->NAME.family.flagDefined) { \
					String family; \
					if (_getStringValue(resource->name, attr->NAME.family, element, family)) { \
						view->SETFUNC(Font::create(family, size, attr->NAME.bold.value, attr->NAME.italic.value, attr->NAME.underline.value, sl_false)); \
					} else { \
						return sl_false; \
					} \
				} else { \
					view->SETFUNC(Font::create(UI::getDefaultFontFamily(), size, attr->NAME.bold.value, attr->NAME.italic.value,  attr->NAME.underline.value, sl_false)); \
				} \
			} \
		} \
	}
	

#define LAYOUT_CONTROL_PROCESS_SUPER(BASE) \
	String tempAddStatement = params->addStatement; \
	params->addStatement = String::null(); \
	if (!(_processLayoutResourceControl_##BASE(params))) { \
		return sl_false; \
	} \
	params->addStatement = tempAddStatement;

#define LAYOUT_CONTROL_ADD_STATEMENT \
	if (op == OP_GENERATE_CPP) { \
		params->sbDefineInit->add(params->addStatement); \
	}

#define LAYOUT_CONTROL_SET_NATIVE_WIDGET \
	if (op == OP_GENERATE_CPP) { \
		if (resourceItem->attrsView->isNotRequiredNative(sl_false) || attr->isNotRequiredNative()) { \
			if (!(resourceItem->attrsView->nativeWidget.flagDefined)) { \
				params->sbDefineInit->add(String::format("%s%s->setCreatingNativeWidget(sl_false);%n", strTab, name)); \
			} \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (resourceItem->attrsView->isNotRequiredNative(sl_false) || attr->isNotRequiredNative()) { \
				if (!(resourceItem->attrsView->nativeWidget.flagDefined)) { \
					view->setCreatingNativeWidget(sl_false); \
				} \
			} \
		} \
	}
	
#define LAYOUT_CONTROL_SET_NATIVE_WIDGET_CHECK_BACKGROUND_COLOR \
	if (op == OP_GENERATE_CPP) { \
		if (resourceItem->attrsView->isNotRequiredNative(sl_true) || attr->isNotRequiredNative()) { \
			if (!(resourceItem->attrsView->nativeWidget.flagDefined)) { \
				params->sbDefineInit->add(String::format("%s%s->setCreatingNativeWidget(sl_false);%n", strTab, name)); \
			} \
		} \
	} else if (op == OP_SIMULATE) { \
		if (!flagOnLayout) { \
			if (resourceItem->attrsView->isNotRequiredNative(sl_true) || attr->isNotRequiredNative()) { \
				if (!(resourceItem->attrsView->nativeWidget.flagDefined)) { \
					view->setCreatingNativeWidget(sl_false); \
				} \
			} \
		} \
	}

	BEGIN_PROCESS_LAYOUT_CONTROL(Window, View)
	{
		Window* view = params->window;
		
		LAYOUT_CONTROL_MENU_ATTR(menu, setMenu)
		LAYOUT_CONTROL_STRING_ATTR(title, setTitle)
		if (op == OP_PARSE) {
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(left, setLeft, checkForWindow)
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(top, setTop, checkForWindow)
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(width, setClientWidth, checkForWindowSize)
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(height, setClientHeight, checkForWindowSize)
		} else if (op == OP_GENERATE_CPP) {
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(left, setLeft, checkForWindow)
			LAYOUT_CONTROL_INT_DIMENSION_ATTR(top, setTop, checkForWindow)
			if (attr->width.flagDefined) {
				if (attr->width.unit == SAppDimensionValue::WRAP) {
					params->sbDefineInit->add(String::format("%s%s->setWidthWrapping();%n", strTab, name));
				} else {
					LAYOUT_CONTROL_INT_DIMENSION_ATTR(width, setClientWidth, checkForWindowSize)
				}
			}
			if (attr->height.flagDefined) {
				if (attr->height.unit == SAppDimensionValue::WRAP) {
					params->sbDefineInit->add(String::format("%s%s->setHeightWrapping();%n", strTab, name));
				} else {
					LAYOUT_CONTROL_INT_DIMENSION_ATTR(height, setClientHeight, checkForWindowSize)
				}
			}
		} else if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				flagOnLayout = sl_true;
				LAYOUT_CONTROL_INT_DIMENSION_ATTR(left, setLeft, checkForWindow)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR(top, setTop, checkForWindow)
				if (attr->width.flagDefined) {
					if (attr->width.unit == SAppDimensionValue::WRAP) {
						view->setWidthWrapping();
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR(width, setClientWidth, checkForWindowSize)
					}
				}
				if (attr->height.flagDefined) {
					if (attr->height.unit == SAppDimensionValue::WRAP) {
						view->setHeightWrapping();
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR(height, setClientHeight, checkForWindowSize)
					}
				}
				flagOnLayout = sl_false;
			}
		}
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(minWidth, setMinimumWidth, checkForWindow)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(maxWidth, setMaximumWidth, checkForWindow)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(minHeight, setMinimumHeight, checkForWindow)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(maxHeight, setMaximumHeight, checkForWindow)
		LAYOUT_CONTROL_GENERIC_ATTR(aspectRatio, setAspectRatio)
		LAYOUT_CONTROL_GENERIC_ATTR(minAspectRatio, setMinimumAspectRatio)
		LAYOUT_CONTROL_GENERIC_ATTR(maxAspectRatio, setMaximumAspectRatio)
		
		LAYOUT_CONTROL_GENERIC_ATTR(minimized, setMinimized)
		LAYOUT_CONTROL_GENERIC_ATTR(maximized, setMaximized)
		LAYOUT_CONTROL_GENERIC_ATTR(visible, setVisible)
		LAYOUT_CONTROL_GENERIC_ATTR(alwaysOnTop, setAlwaysOnTop)
		LAYOUT_CONTROL_GENERIC_ATTR(closeButton, setCloseButtonEnabled)
		LAYOUT_CONTROL_GENERIC_ATTR(minimizeButton, setMinimizeButtonEnabled)
		LAYOUT_CONTROL_GENERIC_ATTR(maximizeButton, setMaximizeButtonEnabled)
		LAYOUT_CONTROL_GENERIC_ATTR(resizable, setResizable)
		LAYOUT_CONTROL_GENERIC_ATTR(alpha, setAlpha)
		LAYOUT_CONTROL_GENERIC_ATTR(transparent, setTransparent)
		
		LAYOUT_CONTROL_GENERIC_ATTR(modal, setModal)
		LAYOUT_CONTROL_GENERIC_ATTR(dialog, setDialog)
		LAYOUT_CONTROL_GENERIC_ATTR(borderless, setBorderless)
		LAYOUT_CONTROL_GENERIC_ATTR(titleBar, setTitleBarVisible)
		LAYOUT_CONTROL_GENERIC_ATTR(fullScreen, setFullScreenOnCreate)
		LAYOUT_CONTROL_GENERIC_ATTR(centerScreen, setCenterScreenOnCreate)
		
		params->name = "m_contentView";
		if (!(_processLayoutResourceControl_View(params))) {
			params->resourceItem->attrsView->resetLayout();
			return sl_false;
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL

	#define SAppLayoutPageAttributes SAppLayoutViewAttributes
	#define attrsPage attrsView
	BEGIN_PROCESS_LAYOUT_CONTROL(Page, ViewPage)
	{
		if (!(_processLayoutResourceControl_View(params))) {
			return sl_false;
		}
		
		if (op == OP_PARSE) {
			if (!(resourceItem->attrsView->width.flagDefined)) {
				resourceItem->attrsView->width.flagDefined = sl_true;
				resourceItem->attrsView->width.amount = 1;
				resourceItem->attrsView->width.unit = SAppDimensionValue::FILL;
			}
			if (!(resourceItem->attrsView->height.flagDefined)) {
				resourceItem->attrsView->height.flagDefined = sl_true;
				resourceItem->attrsView->height.amount = 1;
				resourceItem->attrsView->height.unit = SAppDimensionValue::FILL;
			}
		}

	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(View, View)
	{
		sl_bool flagView = params->parentResourceItem != sl_null || params->resource->layoutType != SAppLayoutType::Window;
		sl_bool flagRoot = params->parentResourceItem == sl_null;

		if (flagView) {
			LAYOUT_CONTROL_STRING_ATTR(id, setId)
			if (op == OP_PARSE) {
				if (flagRoot) {
					LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(width, setWidth, checkForRootViewSize)
					LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(height, setHeight, checkForRootViewSize)
				} else {
					LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(width, setWidth, checkSize)
					LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(height, setHeight, checkSize)
				}
			} else if (op == OP_GENERATE_CPP) {
				if (attr->width.flagDefined) {
					if (attr->width.unit == SAppDimensionValue::FILL) {
						params->sbDefineInit->add(String::format("%s%s->setWidthFilling(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->width.amount));
					} else if (attr->width.unit == SAppDimensionValue::WRAP) {
						params->sbDefineInit->add(String::format("%s%s->setWidthWrapping(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->width.unit == SAppDimensionValue::WEIGHT) {
						params->sbDefineInit->add(String::format("%s%s->setWidthWeight(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->width.amount));
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(width, setWidth, checkSize)
					}
				}
				if (attr->height.flagDefined) {
					if (attr->height.unit == SAppDimensionValue::FILL) {
						params->sbDefineInit->add(String::format("%s%s->setHeightFilling(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->height.amount));
					} else if (attr->height.unit == SAppDimensionValue::WRAP) {
						params->sbDefineInit->add(String::format("%s%s->setHeightWrapping(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->height.unit == SAppDimensionValue::WEIGHT) {
						params->sbDefineInit->add(String::format("%s%s->setHeightWeight(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->height.amount));
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(height, setHeight, checkSize)
					}
				}
			} else if (op == OP_SIMULATE) {
				if (attr->width.flagDefined) {
					if (attr->width.unit == SAppDimensionValue::FILL) {
						if (!flagOnLayout) {
							view->setWidthFilling(attr->width.amount, UIUpdateMode::Init);
						}
					} else if (attr->width.unit == SAppDimensionValue::WRAP) {
						if (!flagOnLayout) {
							view->setWidthWrapping(UIUpdateMode::Init);
						}
					} else if (attr->width.unit == SAppDimensionValue::WEIGHT) {
						if (!flagOnLayout) {
							view->setWidthWeight(attr->width.amount, UIUpdateMode::Init);
						}
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(width, setWidth, checkSize)
					}
				}
				if (attr->height.flagDefined) {
					if (attr->height.unit == SAppDimensionValue::FILL) {
						if (!flagOnLayout) {
							view->setHeightFilling(attr->height.amount, UIUpdateMode::Init);
						}
					} else if (attr->height.unit == SAppDimensionValue::WRAP) {
						if (!flagOnLayout) {
							view->setHeightWrapping(UIUpdateMode::Init);
						}
					} else if (attr->height.unit == SAppDimensionValue::WEIGHT) {
						if (!flagOnLayout) {
							view->setHeightWeight(attr->height.amount, UIUpdateMode::Init);
						}
					} else {
						LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(height, setHeight, checkSize)
					}
				}
			}
			
			if (flagRoot) {
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(left, setLeft, checkForRootViewPosition)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(top, setTop, checkForRootViewPosition)
			} else {
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(left, setLeft, checkPosition)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(top, setTop, checkPosition)
			}

			if (op == OP_PARSE) {
				
				attr->leftMode = PositionMode::Free;
				attr->topMode = PositionMode::Free;
				attr->rightMode = PositionMode::Free;
				attr->bottomMode = PositionMode::Free;
				
				SAppAlignLayoutValue alignLeft;
				LAYOUT_CONTROL_PARSE_ATTR(, alignLeft)
				if (alignLeft.flagDefined) {
					if (alignLeft.flagAlignParent) {
						attr->leftMode = PositionMode::ParentEdge;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(alignLeft.referingView)) {
							attr->leftMode = PositionMode::OtherStart;
							attr->leftReferingView = alignLeft.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(alignLeft)
							return sl_false;
						}
					}
				}
				SAppNameValue toRightOf;
				LAYOUT_CONTROL_PARSE_ATTR(, toRightOf)
				if (toRightOf.flagDefined) {
					if (!flagRoot && params->resource->itemsByName.find(toRightOf.value)) {
						attr->leftMode = PositionMode::OtherEnd;
						attr->leftReferingView = toRightOf.value;
					} else {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(toRightOf)
						return sl_false;
					}
				}
				SAppAlignLayoutValue alignTop;
				LAYOUT_CONTROL_PARSE_ATTR(, alignTop)
				if (alignTop.flagDefined) {
					if (alignTop.flagAlignParent) {
						attr->topMode = PositionMode::ParentEdge;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(alignTop.referingView)) {
							attr->topMode = PositionMode::OtherStart;
							attr->topReferingView = alignTop.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(alignTop)
							return sl_false;
						}
					}
				}
				SAppNameValue below;
				LAYOUT_CONTROL_PARSE_ATTR(, below)
				if (below.flagDefined) {
					if (!flagRoot && params->resource->itemsByName.find(below.value)) {
						attr->topMode = PositionMode::OtherEnd;
						attr->topReferingView = below.value;
					} else {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(below)
						return sl_false;
					}
				}
				SAppAlignLayoutValue alignRight;
				LAYOUT_CONTROL_PARSE_ATTR(, alignRight)
				if (alignRight.flagDefined) {
					if (alignRight.flagAlignParent) {
						attr->rightMode = PositionMode::ParentEdge;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(alignRight.referingView)) {
							attr->rightMode = PositionMode::OtherEnd;
							attr->rightReferingView = alignRight.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(alignRight)
							return sl_false;
						}
					}
				}
				SAppNameValue toLeftOf;
				LAYOUT_CONTROL_PARSE_ATTR(, toLeftOf)
				if (toLeftOf.flagDefined) {
					if (!flagRoot && params->resource->itemsByName.find(toLeftOf.value)) {
						attr->rightMode = PositionMode::OtherStart;
						attr->rightReferingView = toLeftOf.value;
					} else {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(toLeftOf)
						return sl_false;
					}
				}
				SAppAlignLayoutValue alignBottom;
				LAYOUT_CONTROL_PARSE_ATTR(, alignBottom)
				if (alignBottom.flagDefined) {
					if (alignBottom.flagAlignParent) {
						attr->bottomMode = PositionMode::ParentEdge;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(alignBottom.referingView)) {
							attr->bottomMode = PositionMode::OtherEnd;
							attr->bottomReferingView = alignBottom.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(alignBottom)
							return sl_false;
						}
					}
				}
				SAppNameValue above;
				LAYOUT_CONTROL_PARSE_ATTR(, above)
				if (above.flagDefined) {
					if (!flagRoot && params->resource->itemsByName.find(above.value)) {
						attr->bottomMode = PositionMode::OtherStart;
						attr->bottomReferingView = above.value;
					} else {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(above)
						return sl_false;
					}
				}
				SAppAlignLayoutValue centerHorizontal;
				LAYOUT_CONTROL_PARSE_ATTR(, centerHorizontal)
				if (centerHorizontal.flagDefined) {
					if (centerHorizontal.flagAlignParent) {
						attr->leftMode = PositionMode::CenterInParent;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(centerHorizontal.referingView)) {
							attr->leftMode = PositionMode::CenterInOther;
							attr->leftReferingView = centerHorizontal.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(centerHorizontal)
							return sl_false;
						}
					}
				}
				SAppAlignLayoutValue centerVertical;
				LAYOUT_CONTROL_PARSE_ATTR(, centerVertical)
				if (centerVertical.flagDefined) {
					if (centerVertical.flagAlignParent) {
						attr->topMode = PositionMode::CenterInParent;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(centerVertical.referingView)) {
							attr->topMode = PositionMode::CenterInOther;
							attr->topReferingView = centerVertical.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(centerVertical)
							return sl_false;
						}
					}
				}
				SAppAlignLayoutValue alignCenter;
				LAYOUT_CONTROL_PARSE_ATTR(, alignCenter)
				if (alignCenter.flagDefined) {
					if (alignCenter.flagAlignParent) {
						attr->leftMode = PositionMode::CenterInParent;
						attr->topMode = PositionMode::CenterInParent;
					} else {
						if (!flagRoot && params->resource->itemsByName.find(alignCenter.referingView)) {
							attr->leftMode = PositionMode::CenterInOther;
							attr->topMode = PositionMode::CenterInOther;
							attr->leftReferingView = alignCenter.referingView;
							attr->topReferingView = alignCenter.referingView;
						} else {
							LOG_ERROR_LAYOUT_CONTROL_ATTR(alignCenter)
							return sl_false;
						}
					}
				}

				if (resourceItem->itemType != SAppLayoutItemType::Import) {
					if (!(attr->left.flagDefined)) {
						if (attr->leftMode == PositionMode::Free && attr->rightMode == PositionMode::Free) {
							attr->leftMode = PositionMode::ParentEdge;
						}
					}
					if (!(attr->top.flagDefined)) {
						if (attr->topMode == PositionMode::Free && attr->bottomMode == PositionMode::Free) {
							attr->topMode = PositionMode::ParentEdge;
						}
					}
				}

			} else if (op == OP_GENERATE_CPP) {
				if (attr->leftMode == PositionMode::CenterInParent) {
					params->sbDefineInit->add(String::format("%s%s->setCenterHorizontal(slib::UIUpdateMode::Init);%n", strTab, name));
				} else if (attr->leftMode == PositionMode::CenterInOther) {
					params->sbDefineInit->add(String::format("%s%s->setAlignCenterHorizontal(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->leftReferingView));
				} else {
					if (attr->leftMode == PositionMode::ParentEdge) {
						params->sbDefineInit->add(String::format("%s%s->setAlignParentLeft(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->leftMode == PositionMode::OtherStart) {
						params->sbDefineInit->add(String::format("%s%s->setAlignLeft(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->leftReferingView));
					} else if (attr->leftMode == PositionMode::OtherEnd) {
						params->sbDefineInit->add(String::format("%s%s->setRightOf(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->leftReferingView));
					}
					if (attr->rightMode == PositionMode::ParentEdge) {
						params->sbDefineInit->add(String::format("%s%s->setAlignParentRight(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->rightMode == PositionMode::OtherStart) {
						params->sbDefineInit->add(String::format("%s%s->setLeftOf(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->rightReferingView));
					} else if (attr->rightMode == PositionMode::OtherEnd) {
						params->sbDefineInit->add(String::format("%s%s->setAlignRight(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->rightReferingView));
					}
				}
				
				if (attr->topMode == PositionMode::CenterInParent) {
					params->sbDefineInit->add(String::format("%s%s->setCenterVertical(slib::UIUpdateMode::Init);%n", strTab, name));
				} else if (attr->topMode == PositionMode::CenterInOther) {
					params->sbDefineInit->add(String::format("%s%s->setAlignCenterVertical(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->topReferingView));
				} else {
					if (attr->topMode == PositionMode::ParentEdge) {
						params->sbDefineInit->add(String::format("%s%s->setAlignParentTop(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->topMode == PositionMode::OtherStart) {
						params->sbDefineInit->add(String::format("%s%s->setAlignTop(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->topReferingView));
					} else if (attr->topMode == PositionMode::OtherEnd) {
						params->sbDefineInit->add(String::format("%s%s->setBelow(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->topReferingView));
					}
					if (attr->bottomMode == PositionMode::ParentEdge) {
						params->sbDefineInit->add(String::format("%s%s->setAlignParentBottom(slib::UIUpdateMode::Init);%n", strTab, name));
					} else if (attr->bottomMode == PositionMode::OtherStart) {
						params->sbDefineInit->add(String::format("%s%s->setAbove(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->bottomReferingView));
					} else if (attr->bottomMode == PositionMode::OtherEnd) {
						params->sbDefineInit->add(String::format("%s%s->setAlignBottom(%s, slib::UIUpdateMode::Init);%n", strTab, name, attr->bottomReferingView));
					}
				}
			} else if (op == OP_SIMULATE) {
				if (attr->leftMode == PositionMode::CenterInParent) {
					if (!flagOnLayout) {
						view->setCenterHorizontal(UIUpdateMode::Init);
					}
				} else if (attr->leftMode == PositionMode::CenterInOther) {
					if (!flagOnLayout) {
						view->setAlignCenterHorizontal(params->simulator->getViewByName(attr->leftReferingView), UIUpdateMode::Init);
					}
				} else {
					if (attr->leftMode == PositionMode::ParentEdge) {
						if (!flagOnLayout) {
							view->setAlignParentLeft(UIUpdateMode::Init);
						}
					} else if (attr->leftMode == PositionMode::OtherStart) {
						if (!flagOnLayout) {
							view->setAlignLeft(params->simulator->getViewByName(attr->leftReferingView), UIUpdateMode::Init);
						}
					} else if (attr->leftMode == PositionMode::OtherEnd) {
						if (!flagOnLayout) {
							view->setRightOf(params->simulator->getViewByName(attr->leftReferingView), UIUpdateMode::Init);
						}
					}
					if (attr->rightMode == PositionMode::ParentEdge) {
						if (!flagOnLayout) {
							view->setAlignParentRight(UIUpdateMode::Init);
						}
					} else if (attr->rightMode == PositionMode::OtherStart) {
						if (!flagOnLayout) {
							view->setLeftOf(params->simulator->getViewByName(attr->rightReferingView), UIUpdateMode::Init);
						}
					} else if (attr->rightMode == PositionMode::OtherEnd) {
						if (!flagOnLayout) {
							view->setAlignRight(params->simulator->getViewByName(attr->rightReferingView), UIUpdateMode::Init);
						}
					}
				}
				
				if (attr->topMode == PositionMode::CenterInParent) {
					if (!flagOnLayout) {
						view->setCenterVertical(UIUpdateMode::Init);
					}
				} else if (attr->topMode == PositionMode::CenterInOther) {
					if (!flagOnLayout) {
						view->setAlignCenterVertical(params->simulator->getViewByName(attr->topReferingView), UIUpdateMode::Init);
					}
				} else {
					if (attr->topMode == PositionMode::ParentEdge) {
						if (!flagOnLayout) {
							view->setAlignParentTop(UIUpdateMode::Init);
						}
					} else if (attr->topMode == PositionMode::OtherStart) {
						if (!flagOnLayout) {
							view->setAlignTop(params->simulator->getViewByName(attr->topReferingView), UIUpdateMode::Init);
						}
					} else if (attr->topMode == PositionMode::OtherEnd) {
						if (!flagOnLayout) {
							view->setBelow(params->simulator->getViewByName(attr->topReferingView), UIUpdateMode::Init);
						}
					}
					if (attr->bottomMode == PositionMode::ParentEdge) {
						if (!flagOnLayout) {
							view->setAlignParentBottom(UIUpdateMode::Init);
						}
					} else if (attr->bottomMode == PositionMode::OtherStart) {
						if (!flagOnLayout) {
							view->setAbove(params->simulator->getViewByName(attr->bottomReferingView), UIUpdateMode::Init);
						}
					} else if (attr->bottomMode == PositionMode::OtherEnd) {
						if (!flagOnLayout) {
							view->setAlignBottom(params->simulator->getViewByName(attr->bottomReferingView), UIUpdateMode::Init);
						}
					}
				}
			}
			
			if (flagRoot) {
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(minWidth, setMinimumWidth, checkForRootViewScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(maxWidth, setMaximumWidth, checkForRootViewScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(minHeight, setMinimumHeight, checkForRootViewScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(maxHeight, setMaximumHeight, checkForRootViewScalarSize)
			} else {
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(minWidth, setMinimumWidth, checkScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(maxWidth, setMaximumWidth, checkScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(minHeight, setMinimumHeight, checkScalarSize)
				LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(maxHeight, setMaximumHeight, checkScalarSize)
			}
			
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(aspectRatio, setAspectRatio)
			
			if (op == OP_PARSE) {
				if (!flagRoot || resource->layoutType == SAppLayoutType::View) {
					if (!(attr->width.flagDefined) && attr->leftMode != PositionMode::Free && attr->rightMode != PositionMode::Free) {
						attr->width.flagDefined = sl_true;
						attr->width.amount = 1;
						attr->width.unit = SAppDimensionValue::FILL;
					}
					if (!(attr->height.flagDefined) && attr->topMode != PositionMode::Free && attr->bottomMode != PositionMode::Free) {
						attr->height.flagDefined = sl_true;
						attr->height.amount = 1;
						attr->height.unit = SAppDimensionValue::FILL;
					}
					if (resourceItem->itemType != SAppLayoutItemType::Import && resourceItem->itemType != SAppLayoutItemType::Drawer && resourceItem->itemType != SAppLayoutItemType::Image) {
						if (attr->aspectRatio.flagDefined) {
							if (!(attr->width.flagDefined) && !(attr->height.flagDefined)) {
								attr->width.flagDefined = sl_true;
								attr->width.amount = 1;
								attr->width.unit = SAppDimensionValue::WRAP;
							}
						} else {
							if (!(attr->width.flagDefined)) {
								attr->width.flagDefined = sl_true;
								attr->width.amount = 1;
								attr->width.unit = SAppDimensionValue::WRAP;
							}
							if (!(attr->height.flagDefined)) {
								attr->height.flagDefined = sl_true;
								attr->height.amount = 1;
								attr->height.unit = SAppDimensionValue::WRAP;
							}
						}
					}
				}
			}
			if (op == OP_GENERATE_CPP) {
				if (attr->aspectRatio.flagDefined) {
					if (attr->width.flagDefined) {
						if (!(attr->height.flagDefined)) {
							params->sbDefineInit->add(String::format("%s%s->setAspectRatioMode(slib::AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);%n", strTab, name));
						}
					} else {
						if (attr->height.flagDefined) {
							params->sbDefineInit->add(String::format("%s%s->setAspectRatioMode(slib::AspectRatioMode::AdjustWidth, slib::UIUpdateMode::Init);%n", strTab, name));
						}
					}
				}
			} else if (op == OP_SIMULATE) {
				if (attr->aspectRatio.flagDefined) {
					if (!flagOnLayout) {
						if (attr->width.flagDefined) {
							if (!(attr->height.flagDefined)) {
								view->setAspectRatioMode(AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);
							}
						} else {
							if (attr->height.flagDefined) {
								view->setAspectRatioMode(AspectRatioMode::AdjustWidth, slib::UIUpdateMode::Init);
							}
						}
					}
				}
			}

			LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(marginLeft, setMarginLeft)
			LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(marginTop, setMarginTop)
			LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(marginRight, setMarginRight)
			LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(marginBottom, setMarginBottom)
			if (op == OP_PARSE) {
				SAppDimensionValue margin;
				LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, margin)
				if (flagRoot) {
					if (!(margin.checkForRootViewMargin())) {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(margin)
						return sl_false;
					}
				} else {
					if (!(margin.checkMargin())) {
						LOG_ERROR_LAYOUT_CONTROL_ATTR(margin)
						return sl_false;
					}
				}
				if (margin.flagDefined) {
					if (!(attr->marginLeft.flagDefined)) {
						attr->marginLeft = margin;
					}
					if (!(attr->marginTop.flagDefined)) {
						attr->marginTop = margin;
					}
					if (!(attr->marginRight.flagDefined)) {
						attr->marginRight = margin;
					}
					if (!(attr->marginBottom.flagDefined)) {
						attr->marginBottom = margin;
					}
				}
			}
		}
		
		LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(paddingLeft, setPaddingLeft)
		LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(paddingTop, setPaddingTop)
		LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(paddingRight, setPaddingRight)
		LAYOUT_CONTROL_MARGIN_ATTR_NOREDRAW(paddingBottom, setPaddingBottom)
		if (op == OP_PARSE) {
			SAppDimensionValue padding;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, padding)
			if (flagRoot) {
				if (!(padding.checkForRootViewMargin())) {
					LOG_ERROR_LAYOUT_CONTROL_ATTR(padding)
					return sl_false;
				}
			} else {
				if (!(padding.checkMargin())) {
					LOG_ERROR_LAYOUT_CONTROL_ATTR(padding)
					return sl_false;
				}
			}
			if (padding.flagDefined) {
				if (!(attr->paddingLeft.flagDefined)) {
					attr->paddingLeft = padding;
				}
				if (!(attr->paddingTop.flagDefined)) {
					attr->paddingTop = padding;
				}
				if (!(attr->paddingRight.flagDefined)) {
					attr->paddingRight = padding;
				}
				if (!(attr->paddingBottom.flagDefined)) {
					attr->paddingBottom = padding;
				}
			}
		}
		
		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(visibility, setVisibility)
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(visible, setVisible)
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(enabled, setEnabled)
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(clipping, setClipping)
		}
		LAYOUT_CONTROL_DRAWABLE_ATTR(background, setBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedBackground, setPressedBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverBackground, setHoverBackground)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(backgroundScale, setBackgroundScaleMode)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(backgroundAlign, setBackgroundAlignment)

		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(border, setBorder)
			if (op == OP_PARSE) {
				if (flagRoot) {
					LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(borderWidth, setBorderWidth, checkForRootViewScalarSize)
				} else {
					LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(borderWidth, setBorderWidth, checkScalarSize)
				}
				LAYOUT_CONTROL_COLOR_ATTR(borderColor, setBorderColor)
				LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(borderStyle, setBorderStyle)
				if (!(attr->borderWidth.flagDefined)) {
					if (attr->borderColor.flagDefined || attr->borderStyle.flagDefined) {
						attr->borderWidth.flagDefined = sl_true;
						attr->borderWidth.amount = 1;
						attr->borderWidth.unit = SAppDimensionValue::PX;
					}
				}
			} else if (op == OP_GENERATE_CPP){
				if (attr->borderWidth.flagDefined) {
					if (Math::isAlmostZero(attr->borderWidth.amount)) {
						params->sbDefineInit->add(String::format("%s%s->setBorder(slib::Ref<slib::Pen>::null(), slib::UIUpdateMode::Init);%n", strTab, name));
					} else {
						if (attr->borderColor.flagDefined && attr->borderStyle.flagDefined) {
							String strBorderColor;
							if (_getColorAccessString(resource->name, attr->borderColor, element, strBorderColor)) {
								if (attr->borderWidth.isNeededOnLayoutFunction()) {
									params->sbDefineLayout->add(String::format("%s%s->setBorder(slib::Pen::create(%s, %s, %s));%n", strTab, name, attr->borderStyle.getAccessString(), attr->borderWidth.getAccessString(), strBorderColor));
								} else {
									params->sbDefineInit->add(String::format("%s%s->setBorder(slib::Pen::create(%s, %s, %s), slib::UIUpdateMode::Init);%n", strTab, name, attr->borderStyle.getAccessString(), attr->borderWidth.getAccessString(), strBorderColor));
								}
							} else {
								return sl_false;
							}
						} else {
							LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(borderWidth, setBorderWidth, checkScalarSize)
							LAYOUT_CONTROL_COLOR_ATTR(borderColor, setBorderColor)
							LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(borderStyle, setBorderStyle)
						}
					}
				} else {
					LAYOUT_CONTROL_COLOR_ATTR(borderColor, setBorderColor)
					LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(borderStyle, setBorderStyle)
				}
			} else if (op == OP_SIMULATE) {
				if (attr->borderWidth.flagDefined) {
					if (Math::isAlmostZero(attr->borderWidth.amount)) {
						if (!flagOnLayout) {
							view->setBorder(Ref<Pen>::null(), UIUpdateMode::Init);
						}
					} else {
						if (attr->borderColor.flagDefined && attr->borderStyle.flagDefined) {
							if (flagOnLayout) {
								Color borderColor;
								if (_getColorValue(resource->name, attr->borderColor, element, borderColor)) {
									view->setBorder(Pen::create(attr->borderStyle.value, _getDimensionFloatValue(attr->borderWidth), borderColor), UIUpdateMode::None);
								} else {
									return sl_false;
								}
							}
						} else {
							LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(borderWidth, setBorderWidth, checkScalarSize)
							LAYOUT_CONTROL_COLOR_ATTR(borderColor, setBorderColor)
							LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(borderStyle, setBorderStyle)
						}
					}
				} else {
					LAYOUT_CONTROL_COLOR_ATTR(borderColor, setBorderColor)
					LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(borderStyle, setBorderStyle)
				}
			}
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(drawing, setDrawing)

		if (flagView) {
			if (flagRoot) {
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadius, setBoundRadius, checkForRootViewScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadiusX, setBoundRadiusX, checkForRootViewScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadiusY, setBoundRadiusY, checkForRootViewScalarSize)
			} else {
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadius, setBoundRadius, checkScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadiusX, setBoundRadiusX, checkScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(boundRadiusY, setBoundRadiusY, checkScalarSize)
			}
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(boundShape, setBoundShape)
			if (flagRoot) {
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadius, setContentRadius, checkForRootViewScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadiusX, setContentRadiusX, checkForRootViewScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadiusY, setContentRadiusY, checkForRootViewScalarSize)
			} else {
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadius, setContentRadius, checkScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadiusX, setContentRadiusX, checkScalarSize)
				LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(contentRadiusY, setContentRadiusY, checkScalarSize)
			}
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(contentShape, setContentShape)
		}

		LAYOUT_CONTROL_FONT_ATTR(font, setFont)
		if (op == OP_PARSE) {
			attr->finalFont = attr->font;
			if (!flagRoot) {
				attr->finalFont.inheritFrom(params->parentResourceItem->attrsView->finalFont);
			}
			if (attr->font.isDefined()) {
				attr->font = attr->finalFont;
			}
		}
		
		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(alpha, setAlpha)
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(opaque, setOpaque)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(layer, setLayer)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(shadowOpacity, setShadowOpacity)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(shadowRadius, setShadowRadius, checkScalarSize)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(shadowOffsetX, setShadowOffsetX, checkScalarSize)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(shadowOffsetY, setShadowOffsetY, checkScalarSize)
		LAYOUT_CONTROL_COLOR_ATTR(shadowColor, setShadowColor)

		if (op == OP_PARSE) {
			LAYOUT_CONTROL_PARSE_ATTR(attr->, scrolling)
			if (!(attr->scrolling.flagDefined)) {
				if (element->getName() == "hscroll") {
					attr->scrolling.flagDefined = sl_true;
					attr->scrolling.horizontal = sl_true;
					attr->scrolling.vertical = sl_false;
				} else if (element->getName() == "vscroll") {
					attr->scrolling.flagDefined = sl_true;
					attr->scrolling.horizontal = sl_false;
					attr->scrolling.vertical = sl_true;
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			if (attr->scrolling.flagDefined) {
				params->sbDefineInit->add(String::format("%s%s->setHorizontalScrolling(%s);%n", strTab, name, (attr->scrolling.horizontal?"sl_true":"sl_false")));
				params->sbDefineInit->add(String::format("%s%s->setVerticalScrolling(%s);%n", strTab, name, (attr->scrolling.vertical?"sl_true":"sl_false")));
			}
		} else if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				if (attr->scrolling.flagDefined) {
					view->setHorizontalScrolling(attr->scrolling.horizontal?sl_true:sl_false);
					view->setVerticalScrolling(attr->scrolling.vertical?sl_true:sl_false);
				}
			}
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR(paging, setPaging)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(pageWidth, setPageWidth, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(pageHeight, setPageHeight, checkScalarSize)
		
		if (op == OP_PARSE) {
			LAYOUT_CONTROL_PARSE_ATTR(attr->, scrollBars)
		} else if (op == OP_GENERATE_CPP) {
			if (attr->scrollBars.flagDefined) {
				params->sbDefineInit->add(String::format("%s%s->setScrollBarsVisible(%s, %s, slib::UIUpdateMode::Init);%n", strTab, name, attr->scrollBars.horizontalScrollBar ? "sl_true" : "sl_false", attr->scrollBars.verticalScrollBar ? "sl_true" : "sl_false"));
			}
		} else if (op == OP_SIMULATE) {
			if (attr->scrollBars.flagDefined) {
				view->setScrollBarsVisible(attr->scrollBars.horizontalScrollBar, attr->scrollBars.verticalScrollBar, UIUpdateMode::None);
			}
		}
		LAYOUT_CONTROL_GENERIC_ATTR(scrollingByMouse, setContentScrollingByMouse)
		LAYOUT_CONTROL_GENERIC_ATTR(scrollingByTouch, setContentScrollingByTouch)
		LAYOUT_CONTROL_GENERIC_ATTR(scrollingByMouseWheel, setContentScrollingByMouseWheel)
		LAYOUT_CONTROL_GENERIC_ATTR(scrollingByKeyboard, setContentScrollingByKeyboard)
		
		LAYOUT_CONTROL_GENERIC_ATTR(hitTest, setHitTestable)
		LAYOUT_CONTROL_GENERIC_ATTR(touchMultipleChildren, setTouchMultipleChildren)
		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR(tabStop, setTabStopEnabled)
		}
		if (!flagRoot) {
			if (op == OP_PARSE) {
				attr->nextTabStop = resourceItem->getXmlAttribute("nextTabStop");
				attr->previousTabStop = resourceItem->getXmlAttribute("previousTabStop");
			} else if (op == OP_GENERATE_CPP) {
				if (params->resource->itemsByName.find(attr->nextTabStop)) {
					params->sbDefineInit->add(String::format("%s%s->setNextTabStop(%s);%n", strTab, name, attr->nextTabStop));
				}
				if (params->resource->itemsByName.find(attr->previousTabStop)) {
					params->sbDefineInit->add(String::format("%s%s->setPreviousTabStop(%s);%n", strTab, name, attr->previousTabStop));
				}
			} else if (op == OP_SIMULATE) {
				if (!flagOnLayout) {
					view->setNextTabStop(params->simulator->getViewByName(attr->nextTabStop));
					view->setPreviousTabStop(params->simulator->getViewByName(attr->previousTabStop));
				}
			}
		}
		
		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR(instance, setCreatingInstance)
			LAYOUT_CONTROL_GENERIC_ATTR(nativeWidget, setCreatingNativeWidget)
			LAYOUT_CONTROL_GENERIC_ATTR(nativeLayer, setCreatingNativeLayer)
			LAYOUT_CONTROL_GENERIC_ATTR(largeContent, setCreatingLargeContent)
			LAYOUT_CONTROL_GENERIC_ATTR(emptyContent, setCreatingEmptyContent)
		}
		LAYOUT_CONTROL_GENERIC_ATTR(childInstances, setCreatingChildInstances)
		
		if (flagView) {
			LAYOUT_CONTROL_GENERIC_ATTR(okCancelEnabled, setOkCancelEnabled)
			LAYOUT_CONTROL_NOPARAM_ATTR(ok, setOkOnClick)
			LAYOUT_CONTROL_NOPARAM_ATTR(cancel, setCancelOnClick)
			LAYOUT_CONTROL_GENERIC_ATTR(keepKeyboard, setKeepKeyboard)
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


#define SAppLayoutViewGroupAttributes SAppLayoutViewAttributes
#define attrsViewGroup attrsView
	BEGIN_PROCESS_LAYOUT_CONTROL(ViewGroup, ViewGroup)
	{
		if (!(_processLayoutResourceControl_View(params))) {
			return sl_false;
		}
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Import, SAppLayoutImportView)
	{
		if (op == OP_PARSE) {
			attr->layout = resourceItem->getXmlAttribute("layout");
			if (attr->layout.isEmpty()) {
				_logError(element, g_str_error_resource_layout_attribute_invalid.arg("layout", attr->layout));
				return sl_false;
			}
			resourceItem->className = attr->layout;
		} else {
			_openLayoutResource(resource, attr->layout);
		}
		if (op == OP_GENERATE_CPP) {
			Ref<SAppLayoutResource> layoutImport;
			m_layouts.get(attr->layout, &layoutImport);
			if (layoutImport.isNull()) {
				_logError(element, g_str_error_layout_not_found.arg(attr->layout));
				return sl_false;
			}
			if (layoutImport->layoutType != SAppLayoutType::View && layoutImport->layoutType != SAppLayoutType::Page) {
				_logError(element, g_str_error_layout_is_not_view.arg(attr->layout));
				return sl_false;
			}
		} else if (op == OP_SIMULATE) {
			Ref<SAppLayoutResource> layoutImport;
			m_layouts.get(attr->layout, &layoutImport);
			if (layoutImport.isNull()) {
				_logError(element, g_str_error_layout_not_found.arg(attr->layout));
				return sl_false;
			}
			if (layoutImport->layoutType != SAppLayoutType::View && layoutImport->layoutType != SAppLayoutType::Page) {
				_logError(element, g_str_error_layout_is_not_view.arg(attr->layout));
				return sl_false;
			}
			if (!flagOnLayout) {
				Ref<SAppLayoutImportView> _view = new SAppLayoutImportView;
				if (_view.isNotNull()) {
					_view->initialize(params->simulator, layoutImport.get());
				} else {
					return sl_false;
				}
				params->view = _view;
				view = _view.get();
			} else {
				if (!view) {
					return sl_false;
				}
			}
		}
		
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_SIMULATE) {
			if (flagOnLayout) {
				view->layoutViews(view->getWidth(), view->getHeight());
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Button, Button)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(text, setText)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(multiLine, setMultiLine)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(defaultButton, setDefaultButton)
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		LAYOUT_CONTROL_DRAWABLE_ATTR(icon, setIcon)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconWidth, setIconWidth, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconHeight, setIconHeight, checkScalarSize)
		if (op == OP_PARSE) {
			SAppDimensionValue iconSize;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, iconSize)
			if (!(iconSize.checkScalarSize())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(iconSize)
				return sl_false;
			}
			if (iconSize.flagDefined) {
				if (!(attr->iconWidth.flagDefined)) {
					attr->iconWidth = iconSize;
				}
				if (!(attr->iconHeight.flagDefined)) {
					attr->iconHeight = iconSize;
				}
			}
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(iconAlign, setIconAlignment)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(textAlign, setTextAlignment)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(textBeforeIcon, setTextBeforeIcon)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setLayoutOrientation)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconMarginLeft, setIconMarginLeft, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconMarginTop, setIconMarginTop, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconMarginRight, setIconMarginRight, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconMarginBottom, setIconMarginBottom, checkPosition)
		if (op == OP_PARSE) {
			SAppDimensionValue iconMargin;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, iconMargin)
			if (!(iconMargin.checkPosition())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(iconMargin)
				return sl_false;
			}
			if (iconMargin.flagDefined) {
				if (!(attr->iconMarginLeft.flagDefined)) {
					attr->iconMarginLeft = iconMargin;
				}
				if (!(attr->iconMarginTop.flagDefined)) {
					attr->iconMarginTop = iconMargin;
				}
				if (!(attr->iconMarginRight.flagDefined)) {
					attr->iconMarginRight = iconMargin;
				}
				if (!(attr->iconMarginBottom.flagDefined)) {
					attr->iconMarginBottom = iconMargin;
				}
			}
		}
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(textMarginLeft, setTextMarginLeft, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(textMarginTop, setTextMarginTop, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(textMarginRight, setTextMarginRight, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(textMarginBottom, setTextMarginBottom, checkPosition)
		if (op == OP_PARSE) {
			SAppDimensionValue textMargin;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, textMargin)
			if (!(textMargin.checkPosition())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(textMargin)
				return sl_false;
			}
			if (textMargin.flagDefined) {
				if (!(attr->textMarginLeft.flagDefined)) {
					attr->textMarginLeft = textMargin;
				}
				if (!(attr->textMarginTop.flagDefined)) {
					attr->textMarginTop = textMargin;
				}
				if (!(attr->textMarginRight.flagDefined)) {
					attr->textMarginRight = textMargin;
				}
				if (!(attr->textMarginBottom.flagDefined)) {
					attr->textMarginBottom = textMargin;
				}
			}
		}
		
		LAYOUT_CONTROL_COLOR_ATTR(colorOverlay, setColorOverlay)

		String strStates[] = {"Normal", "Hover", "Pressed", "Disabled"};
		ButtonState states[] = {ButtonState::Normal, ButtonState::Hover, ButtonState::Pressed, ButtonState::Disabled};
		for (sl_uint32 i = 0; i < SLIB_SAPP_LAYOUT_BUTTON_CATEGORY_MAX; i++) {
			SAppLayoutButtonCategory& category = attr->categories[i];
			String suffixAll = String::format("%d", i);
			for (sl_uint32 k = 0; k < (sl_uint32)(ButtonState::Count); k++) {
				if (op == OP_PARSE) {
					String suffix;
					if (i > 0) {
						suffix = String::format("%s%d", strStates[k], i);
					} else {
						suffix = strStates[k];
					}
#define BUTTON_STATE_GET_VALUE(NAME, ...) \
					String value_##NAME##_name = #NAME + suffix; \
					String value_##NAME = resourceItem->getXmlAttribute(value_##NAME##_name); \
					if (value_##NAME.isEmpty()) { \
						value_##NAME##_name = #NAME + suffixAll; \
						value_##NAME = resourceItem->getXmlAttribute(value_##NAME##_name); \
					} \
					if (!(category.NAME[k].parse(value_##NAME, ##__VA_ARGS__))) { \
						_logError(element, g_str_error_resource_layout_attribute_invalid.arg(value_##NAME##_name, value_##NAME)); \
						return sl_false; \
					}

					BUTTON_STATE_GET_VALUE(textColor)
					BUTTON_STATE_GET_VALUE(icon, this)
					BUTTON_STATE_GET_VALUE(background, this)
					BUTTON_STATE_GET_VALUE(borderStyle)
					BUTTON_STATE_GET_VALUE(borderWidth, this)
					BUTTON_STATE_GET_VALUE(borderColor)
					BUTTON_STATE_GET_VALUE(colorOverlay)
					
					if (!(category.borderWidth[k].flagDefined)) {
						if (category.borderColor[k].flagDefined || category.borderStyle[k].flagDefined) {
							category.borderWidth[k].flagDefined = sl_true;
							category.borderWidth[k].amount = 1;
							category.borderWidth[k].unit = SAppDimensionValue::PX;
						}
					}

				} else if (op == OP_GENERATE_CPP) {
					if (category.textColor[k].flagDefined) {
						String strTextColor;
						if (_getColorAccessString(resource->name, category.textColor[k], element, strTextColor)) {
							params->sbDefineInit->add(String::format("%s%s->setTextColor(%s, slib::ButtonState::%s, %d, slib::UIUpdateMode::Init);%n", strTab, name, strTextColor, strStates[k], i));
						} else {
							return sl_false;
						}
					}
					if (category.icon[k].flagDefined) {
						String strIcon;
						if (_getDrawableAccessString(resource->name, category.icon[k], element, strIcon)) {
							params->sbDefineInit->add(String::format("%s%s->setIcon(%s, slib::ButtonState::%s, %d, slib::UIUpdateMode::Init);%n", strTab, name, strIcon, strStates[k], i));
						} else {
							return sl_false;
						}
					}
					if (category.background[k].flagDefined) {
						String strBackground;
						if (!(_getDrawableAccessString(resource->name, category.background[k], element, strBackground))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setBackground(%s, slib::ButtonState::%s, %d, slib::UIUpdateMode::Init);%n", strTab, name, strBackground, strStates[k], i));
					}
					if (category.borderWidth[k].flagDefined || category.borderColor[k].flagDefined || category.borderStyle[k].flagDefined) {
						if (category.borderWidth[k].flagDefined && Math::isAlmostZero(category.borderWidth[k].amount)) {
							params->sbDefineInit->add(String::format("%s%s->setBorder(slib::Ref<slib::Pen>::null(), slib::ButtonState::%s, %d, slib::UIUpdateMode::Init);%n", strTab, name, strStates[k], i));
						} else {
							StringBuffer* sb;
							String strUpdateMode;
							if (category.borderWidth[k].isNeededOnLayoutFunction()) {
								sb = params->sbDefineLayout;
								strUpdateMode = "slib::UIUpdateMode::None";
							} else {
								sb = params->sbDefineInit;
								strUpdateMode = "slib::UIUpdateMode::Init";
							}
							String _borderWidth, _borderColor, _borderStyle;
							if (category.borderWidth[k].flagDefined) {
								_borderWidth = category.borderWidth[k].getAccessString();
							} else {
								_borderWidth = "1";
							}
							if (category.borderColor[k].flagDefined) {
								if (!(_getColorAccessString(resource->name, category.borderColor[k], element, _borderColor))) {
									return sl_false;
								}
							} else {
								_borderColor = "slib::Color::Black";
							}
							if (category.borderStyle[k].flagDefined) {
								_borderStyle = category.borderStyle[k].getAccessString();
							} else {
								_borderStyle = "slib::PenStyle::Solid";
							}
							sb->add(String::format("%s%s->setBorder(slib::Pen::create(%s, %s, %s), slib::ButtonState::%s, %d, %s);%n", strTab, name, _borderStyle, _borderWidth, _borderColor, strStates[k], i, strUpdateMode));
						}
					}
					if (category.colorOverlay[k].flagDefined) {
						String strColorOverlay;
						if (_getColorAccessString(resource->name, category.colorOverlay[k], element, strColorOverlay)) {
							params->sbDefineInit->add(String::format("%s%s->setColorOverlay(%s, slib::ButtonState::%s, %d, slib::UIUpdateMode::Init);%n", strTab, name, strColorOverlay, strStates[k], i));
						} else {
							return sl_false;
						}
					}
				} else if (op == OP_SIMULATE) {
					if (category.textColor[k].flagDefined) {
						Color _textColor;
						if (!(_getColorValue(resource->name, category.textColor[k], element, _textColor))) {
							return sl_false;
						}
						if (!flagOnLayout) {
							view->setTextColor(_textColor, states[k], i, UIUpdateMode::Init);
						}
					}
					if (category.background[k].flagDefined) {
						Ref<Drawable> _background;
						if (!(_getDrawableValue(resource->name, category.background[k], element, _background))) {
							return sl_false;
						}
						if (flagOnLayout) {
							view->setBackground(_background, states[k], i, UIUpdateMode::Redraw);
						}
					}
					if (category.icon[k].flagDefined) {
						Ref<Drawable> _icon;
						if (!(_getDrawableValue(resource->name, category.icon[k], element, _icon))) {
							return sl_false;
						}
						if (flagOnLayout) {
							view->setIcon(_icon, states[k], i, UIUpdateMode::Redraw);
						}
					}
					if (category.borderWidth[k].flagDefined || category.borderColor[k].flagDefined || category.borderStyle[k].flagDefined) {
						if (category.borderWidth[k].flagDefined && Math::isAlmostZero(category.borderWidth[k].amount)) {
							if (!flagOnLayout) {
								view->setBorder(Ref<Pen>::null(), states[k], i, UIUpdateMode::Init);
							}
						} else {
							if (flagOnLayout) {
								sl_real _borderWidth;
								Color _borderColor;
								PenStyle _borderStyle;
								if (category.borderWidth[k].flagDefined) {
									_borderWidth = _getDimensionFloatValue(category.borderWidth[k]);
								} else {
									_borderWidth = 1;
								}
								if (category.borderColor[k].flagDefined) {
									if (!(_getColorValue(resource->name, category.borderColor[k], element, _borderColor))) {
										return sl_false;
									}
								} else {
									_borderColor = Color::Black;
								}
								if (category.borderStyle[k].flagDefined) {
									_borderStyle = category.borderStyle[k].value;
								} else {
									_borderStyle = PenStyle::Solid;
								}
								view->setBorder(Pen::create(_borderStyle, _borderWidth, _borderColor), states[k], i, UIUpdateMode::None);
							}
						}
					}
					if (category.colorOverlay[k].flagDefined) {
						Color _colorOverlay;
						if (!(_getColorValue(resource->name, category.colorOverlay[k], element, _colorOverlay))) {
							return sl_false;
						}
						if (!flagOnLayout) {
							view->setColorOverlay(_colorOverlay, states[k], i, UIUpdateMode::Init);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(defaultColorFilter, setUsingDefaultColorFilter)

		LAYOUT_CONTROL_SET_NATIVE_WIDGET_CHECK_BACKGROUND_COLOR
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Label, LabelView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(text, setText)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(hyperText, setHyperText)
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(multiLine, setMultiLine)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(ellipsize, setEllipsize)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(lines, setLinesCount)

		if (op == OP_PARSE) {
			if (!(attr->text.flagDefined) && !(attr->hyperText.flagDefined)) {
				resourceItem->flagNoChildren = sl_true;
				String value = params->source.substring(element->getStartContentPositionInSource(), element->getEndContentPositionInSource());
				if (value.isNotEmpty()) {
					attr->hyperText.flagDefined = sl_true;
					attr->hyperText.flagReferResource = sl_false;
					attr->hyperText.valueOrName = value;
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT

	}
	END_PROCESS_LAYOUT_CONTROL

	
	BEGIN_PROCESS_LAYOUT_CONTROL(Line, LineView)
	{
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setOrientation)
		if (!(attr->orientation.flagDefined)) {
			if (op == OP_PARSE) {
				if (element->getName() == "hline") {
					attr->orientation.flagDefined = sl_true;
					attr->orientation.value = LayoutOrientation::Horizontal;
				} else if (element->getName() == "vline") {
					attr->orientation.flagDefined = sl_true;
					attr->orientation.value = LayoutOrientation::Vertical;
				}
			}
		}
		
		LAYOUT_CONTROL_COLOR_ATTR(lineColor, setLineColor)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(thickness, setLineThickness, checkScalarSize)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(lineStyle, setLineStyle)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	
	BEGIN_PROCESS_LAYOUT_CONTROL(Check, CheckBox)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(Button)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(checked, setChecked)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Radio, RadioButton)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(Check)
		
		LAYOUT_CONTROL_STRING_ATTR(value, setValue)

		if (op == OP_PARSE) {
			attr->group = resourceItem->getXmlAttribute("group");
			if (attr->group.isNotEmpty()) {
				if (!(SAppUtil::checkName(attr->group.getData(), attr->group.getLength()))) {
					_logError(element, g_str_error_resource_layout_name_invalid.arg(attr->group));
					return sl_false;
				}
				if (params->resource->itemsByName.find(attr->group)) {
					_logError(element, g_str_error_resource_layout_name_redefined.arg(attr->group));
					return sl_false;
				}
				params->resource->radioGroups.put(attr->group, sl_true);
			}
		} else if (op == OP_GENERATE_CPP) {
			if (attr->group.isNotEmpty()) {
				params->sbDefineInit->add(String::format("%s%s->add(%s);%n", strTab, attr->group, name));
			}
		} else if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				if (attr->group.isNotEmpty()) {
					Ref<RadioGroup> group = params->simulator->getRadioGroup(attr->group);
					if (group.isNotNull()) {
						group->add(view);
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Edit, EditView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(text, setText)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(hintText, setHintText)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(hintGravity, setHintGravity)
		LAYOUT_CONTROL_COLOR_ATTR(hintTextColor, setHintTextColor)
		LAYOUT_CONTROL_FONT_ATTR(hintFont, setHintFont)
		if (op == OP_PARSE) {
			if (attr->hintFont.isDefined()) {
				attr->hintFont.inheritFrom(resourceItem->attrsView->finalFont);
			}
		}
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(readOnly, setReadOnly)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(password, setPassword)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(multiLine, setMultiLine)
		LAYOUT_CONTROL_GENERIC_ATTR(returnKey, setReturnKeyType)
		LAYOUT_CONTROL_GENERIC_ATTR(keyboard, setKeyboardType)
		LAYOUT_CONTROL_GENERIC_ATTR(autoCap, setAutoCapitalizationType)
		LAYOUT_CONTROL_NOPARAM_ATTR(focusNextOnReturnKey, setFocusNextOnReturnKey)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


#define SAppLayoutPasswordAttributes SAppLayoutEditAttributes
#define attrsPassword attrsEdit
	BEGIN_PROCESS_LAYOUT_CONTROL(Password, PasswordView)
	{
		if (!(_processLayoutResourceControl_Edit(params))) {
			return sl_false;
		}
	}
	END_PROCESS_LAYOUT_CONTROL


#define SAppLayoutTextAreaAttributes SAppLayoutEditAttributes
#define attrsTextArea attrsEdit
	BEGIN_PROCESS_LAYOUT_CONTROL(TextArea, TextArea)
	{
		if (!(_processLayoutResourceControl_Edit(params))) {
			return sl_false;
		}
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Image, ImageView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(scale, setScaleMode)
		LAYOUT_CONTROL_GENERIC_ATTR(minAspectRatio, setMinimumAutoAspectRatio)
		LAYOUT_CONTROL_GENERIC_ATTR(maxAspectRatio, setMaximumAutoAspectRatio)

		
		if (op == OP_PARSE) {
			if (!(resourceItem->attrsView->width.flagDefined) && !(resourceItem->attrsView->height.flagDefined)) {
				if (resourceItem->attrsView->aspectRatio.flagDefined) {
					resourceItem->attrsView->width.flagDefined = sl_true;
					resourceItem->attrsView->width.amount = 1;
					resourceItem->attrsView->width.unit = SAppDimensionValue::WRAP;
				} else {
					resourceItem->attrsView->width.flagDefined = sl_true;
					resourceItem->attrsView->width.amount = 1;
					resourceItem->attrsView->width.unit = SAppDimensionValue::WRAP;
					resourceItem->attrsView->height.flagDefined = sl_true;
					resourceItem->attrsView->height.amount = 1;
					resourceItem->attrsView->height.unit = SAppDimensionValue::WRAP;
				}
			}
		}
		if (!(resourceItem->attrsView->aspectRatio.flagDefined)) {
			if (op == OP_GENERATE_CPP) {
				if (resourceItem->attrsView->width.flagDefined) {
					if (!(resourceItem->attrsView->height.flagDefined)) {
						params->sbDefineInit->add(String::format("%s%s->setAutoAspectRatio(sl_true);%n", strTab, name));
						params->sbDefineInit->add(String::format("%s%s->setAspectRatioMode(slib::AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);%n", strTab, name));
					}
				} else {
					if (resourceItem->attrsView->height.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setAutoAspectRatio(sl_true);%n", strTab, name));
						params->sbDefineInit->add(String::format("%s%s->setAspectRatioMode(slib::AspectRatioMode::AdjustWidth, slib::UIUpdateMode::Init);%n", strTab, name));
					}
				}
			} else if (op == OP_SIMULATE) {
				if (!flagOnLayout) {
					if (resourceItem->attrsView->width.flagDefined) {
						if (!(resourceItem->attrsView->height.flagDefined)) {
							view->setAutoAspectRatio(sl_true);
							view->setAspectRatioMode(AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);
						}
					} else {
						if (resourceItem->attrsView->height.flagDefined) {
							view->setAutoAspectRatio(sl_true);
							view->setAspectRatioMode(AspectRatioMode::AdjustWidth, slib::UIUpdateMode::Init);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_DRAWABLE_ATTR(src, setSource)
		LAYOUT_CONTROL_STRING_ATTR(url, loadUrl)

		LAYOUT_CONTROL_ADD_STATEMENT
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Select, SelectView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_DRAWABLE_ATTR(leftIcon, setLeftIcon)
		LAYOUT_CONTROL_DRAWABLE_ATTR(rightIcon, setRightIcon)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconWidth, setIconWidth, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconHeight, setIconHeight, checkScalarSize)
		if (op == OP_PARSE) {
			SAppDimensionValue iconSize;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, iconSize)
			if (!(iconSize.checkScalarSize())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(iconSize)
				return sl_false;
			}
			if (iconSize.flagDefined) {
				if (!(attr->iconWidth.flagDefined)) {
					attr->iconWidth = iconSize;
				}
				if (!(attr->iconHeight.flagDefined)) {
					attr->iconHeight = iconSize;
				}
			}
		}
		
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)

		LAYOUT_CONTROL_SET_NATIVE_WIDGET
		
		if (op == OP_PARSE) {
			ListElements< Ref<XmlElement> > itemXmls(_getLayoutItemChildElements(resourceItem, resource->name, "item"));
			for (sl_size i = 0; i < itemXmls.count; i++) {
				Ref<XmlElement>& itemXml = itemXmls[i];
				if (itemXml.isNotNull()) {
					SAppLayoutSelectItem subItem;
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., title)
					if (!(subItem.title.flagDefined)) {
						String text = itemXml->getText();
						if (text.isNotEmpty()) {
							if (!(subItem.title.parse(text))) {
								LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(itemXml, title);
								return sl_false;
							}
						}
					}
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., value)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., selected)
					subItem.element = itemXml;
					if (!(attr->items.add(subItem))) {
						_logError(itemXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutSelectItem> selectItems(attr->items);
			if (selectItems.count > 0) {
				sl_size indexSelected = 0;
				sl_bool flagSelected = sl_false;
				params->sbDefineInit->add(String::format("%s%s->setItemsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, selectItems.count));
				for (sl_size i = 0; i < selectItems.count; i++) {
					SAppLayoutSelectItem& selectItem = selectItems[i];
					String strSelectedItemTitle;
					if (!(_getStringAccessString(resource->name, selectItem.title, selectItem.element, strSelectedItemTitle))) {
						return sl_false;
					}
					if (selectItem.title.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setItemTitle(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strSelectedItemTitle));
					}
					String strSelectedItemValue;
					if (!(_getStringAccessString(resource->name, selectItem.value, selectItem.element, strSelectedItemValue))) {
						return sl_false;
					}
					if (selectItem.value.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setItemValue(%d, %s);%n", strTab, name, i, strSelectedItemValue));
					}
					if (selectItem.selected.flagDefined && selectItem.selected.value) {
						flagSelected = sl_true;
						indexSelected = i;
					}
				}
				if (flagSelected) {
					params->sbDefineInit->add(String::format("%s%s->selectIndex(%d, slib::UIUpdateMode::Init);%n", strTab, name, indexSelected));
				}
			}
		} else if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				ListLocker<SAppLayoutSelectItem> selectItems(attr->items);
				if (selectItems.count > 0) {
					sl_uint32 indexSelected = 0;
					sl_bool flagSelected = sl_false;
					sl_uint32 n = (sl_uint32)(selectItems.count);
					view->setItemsCount(n, UIUpdateMode::Init);
					for (sl_uint32 i = 0; i < n; i++) {
						SAppLayoutSelectItem& selectItem = selectItems[i];
						String selectedItemTitle;
						if (!(_getStringValue(resource->name, selectItem.title, selectItem.element, selectedItemTitle))) {
							return sl_false;
						}
						if (selectItem.title.flagDefined) {
							view->setItemTitle(i, selectedItemTitle, UIUpdateMode::Init);
						}
						String selectedItemValue;
						if (!(_getStringValue(resource->name, selectItem.value, selectItem.element, selectedItemValue))) {
							return sl_false;
						}
						if (selectItem.value.flagDefined) {
							view->setItemValue(i, selectedItemValue);
						}
						if (selectItem.selected.flagDefined && selectItem.selected.value) {
							flagSelected = sl_true;
							indexSelected = i;
						}
					}
					if (flagSelected) {
						if (!flagOnLayout) {
							view->selectIndex(indexSelected, UIUpdateMode::Init);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Scroll, ScrollView)
	{

		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_PARSE) {
			sl_size nChildren = element->getChildElementsCount();
			if (nChildren > 0) {
				if (nChildren != 1) {
					_logError(element, g_str_error_resource_layout_scrollview_must_contain_one_child);
					return sl_false;
				}
				Ref<XmlElement> xmlChild = element->getFirstChildElement();
				if (xmlChild.isNotNull()) {
					Ref<SAppLayoutResourceItem> contentItem = _parseLayoutResourceItemChild(params->resource, resourceItem, xmlChild, params->source);
					if (contentItem.isNull()) {
						return sl_false;
					}
					if (!(resourceItem->attrsView->scrolling.flagDefined) || resourceItem->attrsView->scrolling.vertical) {
						contentItem->attrsView->topMode = PositionMode::Free;
						contentItem->attrsView->bottomMode = PositionMode::Free;
					}
					if (!(resourceItem->attrsView->scrolling.flagDefined) || resourceItem->attrsView->scrolling.horizontal) {
						contentItem->attrsView->leftMode = PositionMode::Free;
						contentItem->attrsView->rightMode = PositionMode::Free;
					}
					attr->content = contentItem;
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			if (attr->content.isNotNull()) {
				String addChildStatement = String::format("%s%s->setContentView(%s, slib::UIUpdateMode::Init);%n%n", strTab, name, attr->content->name);
				if (!(_generateLayoutsCpp_Item(params->resource, attr->content.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addChildStatement))) {
					return sl_false;
				}
			}
		} else if (op == OP_SIMULATE) {
			if (attr->content.isNotNull()) {
				Ref<View> contentView = _simulateLayoutCreateOrLayoutView(params->simulator, attr->content.get(), resourceItem, view, flagOnLayout);
				if (contentView.isNotNull()) {
					if (!flagOnLayout) {
						view->setContentView(contentView, UIUpdateMode::Init);
					}
				} else {
					return sl_false;
				}
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Linear, LinearView)
	{
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setOrientation)
		if (!(attr->orientation.flagDefined)) {
			if (op == OP_PARSE) {
				if (element->getName() == "hlinear") {
					attr->orientation.flagDefined = sl_true;
					attr->orientation.value = LayoutOrientation::Horizontal;
				} else if (element->getName() == "vlinear") {
					attr->orientation.flagDefined = sl_true;
					attr->orientation.value = LayoutOrientation::Vertical;
				}
			}
		}
		
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	class _SAppDocument_SimuationListViewAdapter : public ViewAdapter
	{
	public:
		WeakRef<Referable> refer;
		SAppLayoutSimulator* simulator;
		Ref<SAppLayoutResource> layout;
		
	public:
		sl_uint64 getItemsCount() override
		{
			return 100;
		}
		
		Ref<View> getView(sl_uint64 index, View* original, View* parent) override
		{
			if (original) {
				return original;
			}
			Ref<Referable> _refer = refer;
			if (_refer.isNull()) {
				return Ref<View>::null();
			}
			Ref<SAppLayoutImportView> view = new SAppLayoutImportView;
			if (view.isNotNull()) {
				view->initialize(simulator, layout.get());
			}
			return view;
		}
		
	};

	BEGIN_PROCESS_LAYOUT_CONTROL(List, ListView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			attr->itemLayout = resourceItem->getXmlAttribute("item");
		} else if (op == OP_SIMULATE) {
			_openLayoutResource(resource, attr->itemLayout);
			
			if (attr->itemLayout.isNotEmpty() && !(view->getProperty("setAdapter").getBoolean())) {
				
				Ref<SAppLayoutResource> layoutItem;
				m_layouts.get(attr->itemLayout, &layoutItem);
				if (layoutItem.isNull()) {
					_logError(element, g_str_error_layout_not_found.arg(attr->itemLayout));
					return sl_false;
				}
				if (layoutItem->layoutType != SAppLayoutType::View) {
					_logError(element, g_str_error_layout_is_not_view.arg(attr->itemLayout));
					return sl_false;
				}
				
				Ref<_SAppDocument_SimuationListViewAdapter> adapter = new _SAppDocument_SimuationListViewAdapter;
				adapter->refer = params->simulator->getReferable();
				adapter->simulator = params->simulator;
				adapter->layout = layoutItem;
				
				view->setAdapter(adapter);
				view->setProperty("setAdapter", sl_true);
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT

	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(Collection, CollectionView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			attr->itemLayout = resourceItem->getXmlAttribute("item");
		} else if (op == OP_SIMULATE) {
			_openLayoutResource(resource, attr->itemLayout);
			
			if (attr->itemLayout.isNotEmpty() && !(view->getProperty("setAdapter").getBoolean())) {
				
				Ref<SAppLayoutResource> layoutItem;
				m_layouts.get(attr->itemLayout, &layoutItem);
				if (layoutItem.isNull()) {
					_logError(element, g_str_error_layout_not_found.arg(attr->itemLayout));
					return sl_false;
				}
				if (layoutItem->layoutType != SAppLayoutType::View) {
					_logError(element, g_str_error_layout_is_not_view.arg(attr->itemLayout));
					return sl_false;
				}
				
				Ref<_SAppDocument_SimuationListViewAdapter> adapter = new _SAppDocument_SimuationListViewAdapter;
				adapter->refer = params->simulator->getReferable();
				adapter->simulator = params->simulator;
				adapter->layout = layoutItem;
				
				view->setAdapter(adapter);
				view->setProperty("setAdapter", sl_true);
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(Grid, GridView)
	{
		
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			{
				ListLocker< Ref<XmlElement> > columnXmls(_getLayoutItemChildElements(resourceItem, resource->name, "column"));
				for (sl_size i = 0; i < columnXmls.count; i++) {
					Ref<XmlElement>& columnXml = columnXmls[i];
					if (columnXml.isNotNull()) {
						SAppLayoutGridColumn column;
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(columnXml, column., width)
						if (!(column.width.checkSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(columnXml, width)
							return sl_false;
						}
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(columnXml, column., minWidth)
						if (!(column.minWidth.checkScalarSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(columnXml, minWidth)
							return sl_false;
						}
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(columnXml, column., maxWidth)
						if (!(column.maxWidth.checkScalarSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(columnXml, maxWidth)
							return sl_false;
						}
						if (!(attr->columns.add_NoLock(column))) {
							_logError(columnXml, g_str_error_out_of_memory);
							return sl_false;
						}
					}
				}
			}
			{
				CHashMap< Pair<sl_uint32, sl_uint32>, sl_bool > cellAllocs;
				ListLocker< Ref<XmlElement> > rowXmls(_getLayoutItemChildElements(resourceItem, resource->name, "row"));
				sl_uint32 nRows = (sl_uint32)(rowXmls.count);
				for (sl_uint32 i = 0; i < nRows; i++) {
					Ref<XmlElement>& rowXml = rowXmls[i];
					if (rowXml.isNotNull()) {
						SAppLayoutGridRow row;
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(rowXml, row., height)
						if (!(row.height.checkSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(rowXml, height)
							return sl_false;
						}
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(rowXml, row., minHeight)
						if (!(row.minHeight.checkScalarSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(rowXml, minHeight)
							return sl_false;
						}
						LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(rowXml, row., maxHeight)
						if (!(row.maxHeight.checkScalarSize())) {
							LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(rowXml, maxHeight)
							return sl_false;
						}
						
						sl_uint32 iCell = 0;
						ListLocker< Ref<XmlElement> > childXmls(rowXml->getChildElements());
						for (sl_size k = 0; k < childXmls.count; k++) {
							Ref<XmlElement>& xmlView = childXmls[k];
							if (xmlView.isNotNull()) {
								Ref<SAppLayoutResourceItem> subItemView = _parseLayoutResourceItemChild(params->resource, resourceItem, xmlView, params->source);
								if (subItemView.isNull()) {
									return sl_false;
								}
								SAppLayoutGridCell cell;
								cell.view = subItemView;
								LAYOUT_CONTROL_PARSE_XML_ATTR(xmlView, cell., rowspan)
								LAYOUT_CONTROL_PARSE_XML_ATTR(xmlView, cell., colspan)
								if (!(cell.rowspan.flagDefined) || cell.rowspan.value < 1) {
									cell.rowspan.value = 1;
								}
								if (!(cell.colspan.flagDefined) || cell.colspan.value < 1) {
									cell.colspan.value = 1;
								}
								while (cellAllocs.find_NoLock(Pair<sl_uint32, sl_uint32>(i, iCell))) {
									iCell++;
								}
								if (iCell + cell.colspan.value > attr->columns.getCount()) {
									attr->columns.setCount_NoLock(iCell+cell.colspan.value);
								}
								for (sl_uint32 t1 = 0; t1 < cell.rowspan.value; t1++) {
									for (sl_uint32 t2 = 0; t2 < cell.colspan.value; t2++) {
										cellAllocs.put_NoLock(Pair<sl_uint32, sl_uint32>(i + t1, iCell + t2), sl_true);
									}
								}
								row.cells.setCount_NoLock(iCell+1);
								row.cells.setAt_NoLock(iCell, cell);
							}
						}
						
						if (!(attr->rows.add_NoLock(row))) {
							_logError(rowXml, g_str_error_out_of_memory);
							return sl_false;
						}
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListElements<SAppLayoutGridColumn> cols(attr->columns);
			ListElements<SAppLayoutGridRow> rows(attr->rows);
			sl_uint32 iRow, iCol;
			sl_uint32 nCols = (sl_uint32)(cols.count);
			sl_uint32 nRows = (sl_uint32)(rows.count);
			params->sbDefineInit->add(String::format("%s%s->setColumnsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, nCols));
			params->sbDefineInit->add(String::format("%s%s->setRowsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, nRows));
			for (iCol = 0; iCol < nCols; iCol++) {
				SAppLayoutGridColumn& col = cols[iCol];
				if (col.width.flagDefined) {
					if (col.width.unit == SAppDimensionValue::WRAP) {
						params->sbDefineInit->add(String::format("%s%s->setColumnWidthWrapping(%d, slib::UIUpdateMode::Init);%n", strTab, name, iCol));
					} else if (col.width.unit == SAppDimensionValue::FILL) {
						params->sbDefineInit->add(String::format("%s%s->setColumnWidthFilling(%d, %ff, slib::UIUpdateMode::Init);%n", strTab, name, iCol, col.width.amount));
					} else if (col.width.unit == SAppDimensionValue::WEIGHT) {
						params->sbDefineInit->add(String::format("%s%s->setColumnWidthWeight(%d, %ff, slib::UIUpdateMode::Init);%n", strTab, name, iCol, col.width.amount));
					} else {
						if (col.width.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setColumnWidth(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iCol, col.width.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setColumnWidth(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iCol, col.width.getAccessString()));
						}
					}
					if (col.minWidth.flagDefined) {
						if (col.minWidth.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setColumnMinimumWidth(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iCol, col.minWidth.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setColumnMinimumWidth(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iCol, col.minWidth.getAccessString()));
						}
					}
					if (col.maxWidth.flagDefined) {
						if (col.maxWidth.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setColumnMaximumWidth(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iCol, col.maxWidth.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setColumnMaximumWidth(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iCol, col.maxWidth.getAccessString()));
						}
					}
				}
			}
			for (iRow = 0; iRow < nRows; iRow++) {
				SAppLayoutGridRow& row = rows[iRow];
				if (row.height.flagDefined) {
					if (row.height.unit == SAppDimensionValue::WRAP) {
						params->sbDefineInit->add(String::format("%s%s->setRowHeightWrapping(%d, slib::UIUpdateMode::Init);%n", strTab, name, iRow));
					} else if (row.height.unit == SAppDimensionValue::FILL) {
						params->sbDefineInit->add(String::format("%s%s->setRowHeightFilling(%d, %ff, slib::UIUpdateMode::Init);%n", strTab, name, iRow, row.height.amount));
					} else if (row.height.unit == SAppDimensionValue::WEIGHT) {
						params->sbDefineInit->add(String::format("%s%s->setRowHeightWeight(%d, %ff, slib::UIUpdateMode::Init);%n", strTab, name, iRow, row.height.amount));
					} else {
						if (row.height.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setRowHeight(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iRow, row.height.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setRowHeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iRow, row.height.getAccessString()));
						}
					}
					if (row.minHeight.flagDefined) {
						if (row.minHeight.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setRowMinimumHeight(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iRow, row.minHeight.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setRowMinimumHeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iRow, row.minHeight.getAccessString()));
						}
					}
					if (row.maxHeight.flagDefined) {
						if (row.maxHeight.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setRowMaximumHeight(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, iRow, row.maxHeight.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setRowMaximumHeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, iRow, row.maxHeight.getAccessString()));
						}
					}
				}
			}
		} else if (op == OP_SIMULATE) {
			ListElements<SAppLayoutGridColumn> cols(attr->columns);
			ListElements<SAppLayoutGridRow> rows(attr->rows);
			sl_uint32 iRow, iCol;
			sl_uint32 nCols = (sl_uint32)(cols.count);
			sl_uint32 nRows = (sl_uint32)(rows.count);
			if (!flagOnLayout) {
				view->setColumnsCount(nCols, UIUpdateMode::Init);
				view->setRowsCount(nRows, UIUpdateMode::Init);
			}
			for (iCol = 0; iCol < nCols; iCol++) {
				SAppLayoutGridColumn& col = cols[iCol];
				if (col.width.flagDefined) {
					if (col.width.unit == SAppDimensionValue::WRAP) {
						if (!flagOnLayout) {
							view->setColumnWidthWrapping(iCol, UIUpdateMode::Init);
						}
					} else if (col.width.unit == SAppDimensionValue::FILL) {
						if (!flagOnLayout) {
							view->setColumnWidthFilling(iCol, col.width.amount, UIUpdateMode::Init);
						}
					} else if (col.width.unit == SAppDimensionValue::WEIGHT) {
						if (!flagOnLayout) {
							view->setColumnWidthWeight(iCol, col.width.amount, UIUpdateMode::Init);
						}
					} else {
						if (flagOnLayout) {
							view->setColumnWidth(iCol, _getDimensionIntValue(col.width), UIUpdateMode::None);
						}
					}
				}
				if (col.minWidth.flagDefined) {
					if (flagOnLayout) {
						view->setColumnMinimumWidth(iCol, _getDimensionIntValue(col.minWidth), UIUpdateMode::None);
					}
				}
				if (col.maxWidth.flagDefined) {
					if (flagOnLayout) {
						view->setColumnMaximumWidth(iCol, _getDimensionIntValue(col.maxWidth), UIUpdateMode::None);
					}
				}
			}
			for (iRow = 0; iRow < nRows; iRow++) {
				SAppLayoutGridRow& row = rows[iRow];
				if (row.height.flagDefined) {
					if (row.height.unit == SAppDimensionValue::WRAP) {
						if (!flagOnLayout) {
							view->setRowHeightWrapping(iRow, UIUpdateMode::Init);
						}
					} else if (row.height.unit == SAppDimensionValue::FILL) {
						if (!flagOnLayout) {
							view->setRowHeightFilling(iRow, row.height.amount, UIUpdateMode::Init);
						}
					} else if (row.height.unit == SAppDimensionValue::WEIGHT) {
						if (!flagOnLayout) {
							view->setRowHeightWeight(iRow, row.height.amount, UIUpdateMode::Init);
						}
					} else {
						if (flagOnLayout) {
							view->setRowHeight(iRow, _getDimensionIntValue(row.height), UIUpdateMode::None);
						}
					}
				}
				if (row.minHeight.flagDefined) {
					if (flagOnLayout) {
						view->setRowMinimumHeight(iRow, _getDimensionIntValue(row.minHeight), UIUpdateMode::None);
					}
				}
				if (row.maxHeight.flagDefined) {
					if (flagOnLayout) {
						view->setRowMaximumHeight(iRow, _getDimensionIntValue(row.maxHeight), UIUpdateMode::None);
					}
				}
			}
			for (iRow = 0; iRow < nRows; iRow++) {
				SAppLayoutGridRow& row = rows[iRow];
				ListElements<SAppLayoutGridCell> cells(row.cells);
				sl_uint32 nCells = Math::min((sl_uint32)(cells.count), nCols);
				for (iCol = 0; iCol < nCells; iCol++) {
					SAppLayoutGridCell& cell = cells[iCol];
					if (cell.view.isNotNull()) {
						Ref<View> contentView = _simulateLayoutCreateOrLayoutView(params->simulator, cell.view.get(), resourceItem, view, flagOnLayout);
						if (contentView.isNotNull()) {
							if (!flagOnLayout) {
								view->setCell(iRow, iCol, contentView, cell.rowspan.value, cell.colspan.value, UIUpdateMode::Init);
							}
						} else {
							return sl_false;
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_GENERATE_CPP) {
			ListElements<SAppLayoutGridColumn> cols(attr->columns);
			ListElements<SAppLayoutGridRow> rows(attr->rows);
			sl_uint32 iRow, iCol;
			sl_uint32 nCols = (sl_uint32)(cols.count);
			sl_uint32 nRows = (sl_uint32)(rows.count);
			for (iRow = 0; iRow < nRows; iRow++) {
				SAppLayoutGridRow& row = rows[iRow];
				ListElements<SAppLayoutGridCell> cells(row.cells);
				sl_uint32 nCells = Math::min((sl_uint32)(cells.count), nCols);
				for (iCol = 0; iCol < nCells; iCol++) {
					SAppLayoutGridCell& cell = cells[iCol];
					if (cell.view.isNotNull()) {
						String addChildStatement;
						if (cell.colspan.value <= 1 && cell.rowspan.value <= 1) {
							addChildStatement = String::format("%s%s->setCell(%d, %d, %s, slib::UIUpdateMode::Init);%n%n", strTab, name, iRow, iCol, cell.view->name);
						} else {
							addChildStatement = String::format("%s%s->setCell(%d, %d, %s, %d, %d, slib::UIUpdateMode::Init);%n%n", strTab, name, iRow, iCol, cell.view->name, cell.rowspan.value, cell.colspan.value);
						}
						if (!(_generateLayoutsCpp_Item(params->resource, cell.view.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addChildStatement))) {
							return sl_false;
						}
					}
				}
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(ListReport, ListReportView)
	{
		
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			ListLocker< Ref<XmlElement> > columnXmls(_getLayoutItemChildElements(resourceItem, resource->name, "column"));
			for (sl_size i = 0; i < columnXmls.count; i++) {
				Ref<XmlElement>& columnXml = columnXmls[i];
				if (columnXml.isNotNull()) {
					SAppLayoutListReportColumn column;
					LAYOUT_CONTROL_PARSE_XML_ATTR(columnXml, column., title)
					LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(columnXml, column., width)
					if (!(column.width.checkScalarSize())) {
						LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(columnXml, width)
						return sl_false;
					}
					LAYOUT_CONTROL_PARSE_XML_ATTR(columnXml, column., align)
					LAYOUT_CONTROL_PARSE_XML_ATTR(columnXml, column., headerAlign)
					
					if (!(attr->columns.add(column))) {
						_logError(columnXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutListReportColumn> columns(attr->columns);
			if (columns.count > 0) {
				params->sbDefineInit->add(String::format("%s%s->setColumnsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, columns.count));
				for (sl_size i = 0; i < columns.count; i++) {
					SAppLayoutListReportColumn& column = columns[i];
					String strColumnTitle;
					if (!(_getStringAccessString(resource->name, column.title, element, strColumnTitle))) {
						return sl_false;
					}
					if (column.title.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setHeaderText(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strColumnTitle));
					}
					if (column.width.flagDefined) {
						if (column.width.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setColumnWidth(%d, %s, slib::UIUpdateMode::None);%n", strTab, name, i, column.width.getAccessString()));
						} else {
							params->sbDefineInit->add(String::format("%s%s->setColumnWidth(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, column.width.getAccessString()));
						}
					}
					if (column.align.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setColumnAlignment(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, column.align.getAccessString()));
					}
					if (column.headerAlign.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setHeaderAlignment(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, column.headerAlign.getAccessString()));
					}
				}
			}
		} else if (op == OP_SIMULATE) {
			ListLocker<SAppLayoutListReportColumn> columns(attr->columns);
			if (columns.count > 0) {
				sl_uint32 n = (sl_uint32)(columns.count);
				if (!flagOnLayout) {
					view->setColumnsCount(n, UIUpdateMode::Init);
				}
				for (sl_uint32 i = 0; i < n; i++) {
					SAppLayoutListReportColumn& column = columns[i];
					if (!flagOnLayout) {
						String columnTitle;
						if (!(_getStringValue(resource->name, column.title, element, columnTitle))) {
							return sl_false;
						}
						if (column.title.flagDefined) {
							view->setHeaderText(i, columnTitle, UIUpdateMode::Init);
						}
						if (column.align.flagDefined) {
							view->setColumnAlignment(i, column.align.value, UIUpdateMode::Init);
						}
						if (column.headerAlign.flagDefined) {
							view->setHeaderAlignment(i, column.headerAlign.value, UIUpdateMode::Init);
						}
					}
					if (column.width.flagDefined) {
						if (flagOnLayout) {
							view->setColumnWidth(i, _getDimensionIntValue(column.width), UIUpdateMode::None);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Render, RenderView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR(redraw, setRedrawMode)

		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Tab, TabView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setOrientation)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(tabWidth, setTabWidth, checkScalarSize)
		LAYOUT_CONTROL_FLOAT_DIMENSION_ATTR_NOREDRAW(tabHeight, setTabHeight, checkScalarSize)
		LAYOUT_CONTROL_DRAWABLE_ATTR(barBackground, setBarBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(contentBackground, setContentBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(tabBackground, setTabBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(selectedTabBackground, setSelectedTabBackground)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverTabBackground, setHoverTabBackground)
		LAYOUT_CONTROL_COLOR_ATTR(labelColor, setLabelColor)
		LAYOUT_CONTROL_COLOR_ATTR(selectedLabelColor, setSelectedLabelColor)
		LAYOUT_CONTROL_COLOR_ATTR(hoverLabelColor, setHoverLabelColor)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(tabAlign, setTabAlignment)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(tabPaddingLeft, setTabPaddingLeft, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(tabPaddingTop, setTabPaddingTop, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(tabPaddingRight, setTabPaddingRight, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(tabPaddingBottom, setTabPaddingBottom, checkPosition)
		if (op == OP_PARSE) {
			SAppDimensionValue tabPadding;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, tabPadding)
			if (!(tabPadding.checkPosition())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(tabPadding)
				return sl_false;
			}
			if (tabPadding.flagDefined) {
				if (!(attr->tabPaddingLeft.flagDefined)) {
					attr->tabPaddingLeft = tabPadding;
				}
				if (!(attr->tabPaddingTop.flagDefined)) {
					attr->tabPaddingTop = tabPadding;
				}
				if (!(attr->tabPaddingRight.flagDefined)) {
					attr->tabPaddingRight = tabPadding;
				}
				if (!(attr->tabPaddingBottom.flagDefined)) {
					attr->tabPaddingBottom = tabPadding;
				}
			}
		}
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(tabSpaceSize, setTabSpaceSize, checkPosition)

		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconWidth, setIconWidth, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(iconHeight, setIconHeight, checkScalarSize)
		if (op == OP_PARSE) {
			SAppDimensionValue iconSize;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, iconSize)
			if (!(iconSize.checkScalarSize())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(iconSize)
				return sl_false;
			}
			if (iconSize.flagDefined) {
				if (!(attr->iconWidth.flagDefined)) {
					attr->iconWidth = iconSize;
				}
				if (!(attr->iconHeight.flagDefined)) {
					attr->iconHeight = iconSize;
				}
			}
		}

		if (op == OP_PARSE) {
			ListLocker< Ref<XmlElement> > itemXmls(_getLayoutItemChildElements(resourceItem, resource->name, "item"));
			for (sl_size i = 0; i < itemXmls.count; i++) {
				Ref<XmlElement>& itemXml = itemXmls[i];
				if (itemXml.isNotNull()) {
					SAppLayoutTabItem subItem;
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., label)
					LAYOUT_CONTROL_PARSE_XML_DRAWABLE_ATTR(itemXml, subItem., icon)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., selected)
					
					sl_size nViews = itemXml->getChildElementsCount();
					if (nViews > 0) {
						if (nViews != 1) {
							_logError(itemXml, g_str_error_resource_layout_item_must_contain_one_child);
							return sl_false;
						}
						Ref<XmlElement> xmlView= itemXml->getFirstChildElement();
						if (xmlView.isNotNull()) {
							Ref<SAppLayoutResourceItem> subItemView = _parseLayoutResourceItemChild(params->resource, resourceItem, xmlView, params->source);
							if (subItemView.isNull()) {
								return sl_false;
							}
							subItemView->attrsView->resetLayout();
							subItem.view = subItemView;
						}
					}
					
					subItem.element = itemXml;
					
					if (!(attr->items.add(subItem))) {
						_logError(itemXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutTabItem> subItems(attr->items);
			if (subItems.count > 0) {
				sl_size indexSelected = 0;
				sl_bool flagSelected = sl_false;
				
				params->sbDefineInit->add(String::format("%s%s->setTabsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, subItems.count));
				
				for (sl_size i = 0; i < subItems.count; i++) {
					SAppLayoutTabItem& subItem = subItems[i];
					if (subItem.label.flagDefined) {
						String strItemLabel;
						if (!(_getStringAccessString(resource->name, subItem.label, subItem.element, strItemLabel))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setTabLabel(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strItemLabel));
					}
					if (subItem.icon.flagDefined) {
						String strItemIcon;
						if (!(_getDrawableAccessString(resource->name, subItem.icon, element, strItemIcon))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setTabIcon(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strItemIcon));
					}
					if (subItem.selected.flagDefined && subItem.selected.value) {
						flagSelected = sl_true;
						indexSelected = i;
					}
				}
				
				if (flagSelected) {
					params->sbDefineInit->add(String::format("%s%s->selectTab(%d, slib::UIUpdateMode::Init);%n", strTab, name, indexSelected));
				}
			}
		} else if (op == OP_SIMULATE) {
			ListLocker<SAppLayoutTabItem> subItems(attr->items);
			if (subItems.count > 0) {
				
				sl_uint32 indexSelected = 0;
				sl_bool flagSelected = sl_false;
				
				sl_uint32 nSubItems = (sl_uint32)(subItems.count);
				if (!flagOnLayout) {
					view->setTabsCount(nSubItems, UIUpdateMode::Init);
				}
				
				for (sl_uint32 i = 0; i < nSubItems; i++) {
					
					SAppLayoutTabItem& subItem = subItems[i];
					if (subItem.label.flagDefined) {
						if (!flagOnLayout) {
							String itemLabel;
							if (!(_getStringValue(resource->name, subItem.label, subItem.element, itemLabel))) {
								return sl_false;
							}
							view->setTabLabel(i, itemLabel, UIUpdateMode::Init);
						}
					}
					if (subItem.icon.flagDefined) {
						if (!flagOnLayout) {
							Ref<Drawable> itemIcon;
							if (!(_getDrawableValue(resource->name, subItem.icon, subItem.element, itemIcon))) {
								return sl_false;
							}
							view->setTabIcon(i, itemIcon, UIUpdateMode::Init);
						}
					}
					if (subItem.selected.flagDefined && subItem.selected.value) {
						flagSelected = sl_true;
						indexSelected = i;
					}
					
					if (subItem.view.isNotNull()) {
						Ref<View> contentView = _simulateLayoutCreateOrLayoutView(params->simulator, subItem.view.get(), resourceItem, view, flagOnLayout);
						if (contentView.isNotNull()) {
							if (!flagOnLayout) {
								view->setTabContentView(i, contentView, UIUpdateMode::Init);
							}
						} else {
							return sl_false;
						}
					}
					
					if (flagSelected) {
						if (!flagOnLayout) {
							view->selectTab(indexSelected, UIUpdateMode::Init);
						}
					}
					
				}
				
			}

		}
		
		LAYOUT_CONTROL_SET_NATIVE_WIDGET

		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutTabItem> subItems(attr->items);
			for (sl_size i = 0; i < subItems.count; i++) {
				SAppLayoutTabItem& subItem = subItems[i];
				if (subItem.view.isNotNull()) {
					String addChildStatement = String::format("%s%s->setTabContentView(%d, %s, slib::UIUpdateMode::Init);%n%n", strTab, name, i, subItem.view->name);
					if (!(_generateLayoutsCpp_Item(params->resource, subItem.view.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addChildStatement))) {
						return sl_false;
					}
				}
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Tree, TreeView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_DRAWABLE_ATTR(itemIcon, setItemIcon)
		LAYOUT_CONTROL_DRAWABLE_ATTR(opendIcon, setOpenedItemIcon)
		LAYOUT_CONTROL_DRAWABLE_ATTR(closedIcon, setClosedItemIcon)
		LAYOUT_CONTROL_DRAWABLE_ATTR(collapsedIcon, setCollapsedIcon)
		LAYOUT_CONTROL_DRAWABLE_ATTR(expandedIcon, setExpandedIcon)
		LAYOUT_CONTROL_COLOR_ATTR(selectedBackgroundColor, setSelectedItemBackgroundColor)
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setItemTextColor)
		LAYOUT_CONTROL_COLOR_ATTR(hoverTextColor, setHoverItemTextColor)
		LAYOUT_CONTROL_COLOR_ATTR(selectedTextColor, setSelectedItemTextColor)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(itemHeight, setItemHeight, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(itemPadding, setItemPadding, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(itemIndent, setItemIndent, checkPosition)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(textIndent, setTextIndent, checkPosition)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Split, SplitView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setOrientation)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(dividerWidth, setDividerWidth, checkScalarSize)
		LAYOUT_CONTROL_DRAWABLE_ATTR(dividerBackground, setDividerBackground)
		LAYOUT_CONTROL_COLOR_ATTR(dividerColor, setDividerColor)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(cursorMargin, setCursorMargin, checkScalarSize)
		
		sl_bool flagRelayoutOnInit = sl_false;
		sl_bool flagRelayoutOnLayout = sl_false;
		
		if (op == OP_PARSE) {
			ListLocker< Ref<XmlElement> > itemXmls(_getLayoutItemChildElements(resourceItem, resource->name, "item"));
			for (sl_size i = 0; i < itemXmls.count; i++) {
				Ref<XmlElement>& itemXml = itemXmls[i];
				if (itemXml.isNotNull()) {
					SAppLayoutSplitItem subItem;
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., weight)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., minWeight)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., maxWeight)
					LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(itemXml, subItem., minSize)
					if (!(subItem.minSize.checkScalarSize())) {
						LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(itemXml, minSize)
						return sl_false;
					}
					LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(itemXml, subItem., maxSize)
					if (!(subItem.maxSize.checkScalarSize())) {
						LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(itemXml, maxSize)
						return sl_false;
					}
					LAYOUT_CONTROL_PARSE_XML_DIMENSION_ATTR(itemXml, subItem., dividerWidth)
					if (!(subItem.dividerWidth.checkScalarSize())) {
						LOG_ERROR_LAYOUT_CONTROL_XML_ATTR(itemXml, dividerWidth)
						return sl_false;
					}
					LAYOUT_CONTROL_PARSE_XML_DRAWABLE_ATTR(itemXml, subItem., dividerBackground)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., dividerColor)
					
					sl_size nViews = itemXml->getChildElementsCount();
					if (nViews > 0) {
						if (nViews != 1) {
							_logError(itemXml, g_str_error_resource_layout_item_must_contain_one_child);
							return sl_false;
						}
						Ref<XmlElement> xmlView= itemXml->getFirstChildElement();
						if (xmlView.isNotNull()) {
							Ref<SAppLayoutResourceItem> subItemView = _parseLayoutResourceItemChild(params->resource, resourceItem, xmlView, params->source);
							if (subItemView.isNull()) {
								return sl_false;
							}
							subItem.view = subItemView;
						}
					}
					
					subItem.element = itemXml;
					
					if (!(attr->items.add(subItem))) {
						_logError(itemXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutSplitItem> subItems(attr->items);
			if (subItems.count > 0) {
				if (subItems.count > 2) {
					params->sbDefineInit->add(String::format("%s%s->setItemsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, subItems.count));
				}
				
				for (sl_size i = 0; i < subItems.count; i++) {
					SAppLayoutSplitItem& subItem = subItems[i];
					if (subItem.weight.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setItemWeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.weight.getAccessString()));
						flagRelayoutOnInit = sl_true;
					}
					if (subItem.minWeight.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setItemMinimumWeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.minWeight.getAccessString()));
						flagRelayoutOnInit = sl_true;
					}
					if (subItem.maxWeight.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->setItemMaximumWeight(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.maxWeight.getAccessString()));
						flagRelayoutOnInit = sl_true;
					}
					if (subItem.minSize.flagDefined) {
						if (subItem.minSize.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setItemMinimumSize(%d, %s);%n", strTab, name, i, subItem.minSize.getAccessString()));
							flagRelayoutOnLayout = sl_true;
						} else {
							params->sbDefineInit->add(String::format("%s%s->setItemMinimumSize(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.minSize.getAccessString()));
							flagRelayoutOnInit = sl_true;
						}
					}
					if (subItem.maxSize.flagDefined) {
						if (subItem.maxSize.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setItemMaximumSize(%d, %s);%n", strTab, name, i, subItem.maxSize.getAccessString()));
							flagRelayoutOnLayout = sl_true;
						} else {
							params->sbDefineInit->add(String::format("%s%s->setItemMaximumSize(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.maxSize.getAccessString()));
							flagRelayoutOnInit = sl_true;
						}
					}
					if (subItem.dividerWidth.flagDefined) {
						if (subItem.dividerWidth.isNeededOnLayoutFunction()) {
							params->sbDefineLayout->add(String::format("%s%s->setItemDividerWidth(%d, %s);%n", strTab, name, i, subItem.dividerWidth.getAccessString()));
							flagRelayoutOnLayout = sl_true;
						} else {
							params->sbDefineInit->add(String::format("%s%s->setItemDividerWidth(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, subItem.dividerWidth.getAccessString()));
							flagRelayoutOnInit = sl_true;
						}
					}
					if (subItem.dividerBackground.flagDefined) {
						String strDividerBackground;
						if (!(_getDrawableAccessString(resource->name, subItem.dividerBackground, subItem.element, strDividerBackground))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setItemDividerBackground(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strDividerBackground));
						flagRelayoutOnInit = sl_true;
					}
					if (subItem.dividerColor.flagDefined) {
						String strDividerColor;
						if (!(_getColorAccessString(resource->name, subItem.dividerBackground, subItem.element, strDividerColor))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setItemDividerColor(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strDividerColor));
						flagRelayoutOnInit = sl_true;
					}
				}
			}
		} else if (op == OP_SIMULATE) {
			
			ListLocker<SAppLayoutSplitItem> subItems(attr->items);
			if (subItems.count > 0) {
				if (!flagOnLayout) {
					if (subItems.count > 2) {
						view->setItemsCount(subItems.count, UIUpdateMode::Init);
					}
				}
				for (sl_size i = 0; i < subItems.count; i++) {
					SAppLayoutSplitItem& subItem = subItems[i];
					if (subItem.weight.flagDefined) {
						if (!flagOnLayout) {
							view->setItemWeight(i, subItem.weight.value, UIUpdateMode::Init);
						}
					}
					if (subItem.minWeight.flagDefined) {
						if (!flagOnLayout) {
							view->setItemMinimumWeight(i, subItem.minWeight.value, UIUpdateMode::Init);
						}
					}
					if (subItem.maxWeight.flagDefined) {
						if (!flagOnLayout) {
							view->setItemMaximumWeight(i, subItem.maxWeight.value, UIUpdateMode::Init);
						}
					}
					if (subItem.minSize.flagDefined) {
						if (flagOnLayout) {
							view->setItemMinimumSize(i, _getDimensionIntValue(subItem.minSize));
						}
					}
					if (subItem.maxSize.flagDefined) {
						if (flagOnLayout) {
							view->setItemMaximumSize(i, _getDimensionIntValue(subItem.maxSize));
						}
					}
					if (subItem.dividerWidth.flagDefined) {
						if (flagOnLayout) {
							view->setItemDividerWidth(i, _getDimensionIntValue(subItem.dividerWidth));
						}
					}
					if (subItem.dividerBackground.flagDefined) {
						Ref<Drawable> dividerBackground;
						if (!(_getDrawableValue(resource->name, subItem.dividerBackground, subItem.element, dividerBackground))) {
							return sl_false;
						}
						if (flagOnLayout) {
							view->setItemDividerBackground(i, dividerBackground, UIUpdateMode::None);
						}
					}
					if (subItem.dividerColor.flagDefined) {
						Color dividerColor;
						if (!(_getColorValue(resource->name, subItem.dividerColor, subItem.element, dividerColor))) {
							return sl_false;
						}
						if (!flagOnLayout) {
							view->setItemDividerColor(i, dividerColor, UIUpdateMode::Init);
						}
					}
					if (subItem.view.isNotNull()) {
						Ref<View> contentView = _simulateLayoutCreateOrLayoutView(params->simulator, subItem.view.get(), resourceItem, view, flagOnLayout);
						if (contentView.isNotNull()) {
							if (!flagOnLayout) {
								view->setItemView(i, contentView);
							}
						} else {
							return sl_false;
						}
					}
				}
				
				view->relayout();
				
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutSplitItem> subItems(attr->items);
			for (sl_size i = 0; i < subItems.count; i++) {
				SAppLayoutSplitItem& subItem = subItems[i];
				if (subItem.view.isNotNull()) {
					String addChildStatement = String::format("%s%s->setItemView(%d, %s, slib::UIUpdateMode::Init);%n%n", strTab, name, i, subItem.view->name);
					if (!(_generateLayoutsCpp_Item(params->resource, subItem.view.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addChildStatement))) {
						return sl_false;
					}
				}
			}
			if (flagRelayoutOnInit) {
				params->sbDefineInit->add(String::format("%s%s->relayout(slib::UIUpdateMode::None);%n%n", strTab, name));
			}
			if (flagRelayoutOnLayout) {
				params->sbDefineLayout->add(String::format("%s%s->relayout();%n%n", strTab, name));
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Web, WebView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			LAYOUT_CONTROL_PARSE_ATTR(attr->, url)
			LAYOUT_CONTROL_PARSE_ATTR(attr->, html)
		} else {
			if (op == OP_GENERATE_CPP) {
				String strUrl;
				if (!(_getStringAccessString(resource->name, attr->url, element, strUrl))) {
					return sl_false;
				}
				String strHtml;
				if (!(_getStringAccessString(resource->name, attr->html, element, strHtml))) {
					return sl_false;
				}
				if (attr->html.flagDefined) {
					if (attr->url.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->loadHTML(%s, %s);%n%n", strTab, name, strHtml, strUrl));
					} else {
						params->sbDefineInit->add(String::format("%s%s->loadHTML(%s, slib::String::null());%n%n", strTab, name, strHtml));
					}
				} else {
					if (attr->url.flagDefined) {
						params->sbDefineInit->add(String::format("%s%s->loadURL(%s);%n%n", strTab, name, strUrl));
					}
				}
			} else if (op == OP_SIMULATE) {
				if (!flagOnLayout) {
					String _url;
					if (!(_getStringValue(resource->name, attr->url, element, _url))) {
						return sl_false;
					}
					String _html;
					if (!(_getStringValue(resource->name, attr->html, element, _html))) {
						return sl_false;
					}
					if (attr->html.flagDefined) {
						if (attr->url.flagDefined) {
							view->loadHTML(_html, _url);
						} else {
							view->loadHTML(_html, String::null());
						}
					} else {
						if (attr->url.flagDefined) {
							view->loadURL(_url);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Progress, ProgressBar)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(orientation, setOrientation)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(min, setMinimumValue)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(max, setMaximumValue)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(value, setValue)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(value2, setSecondaryValue)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(dual, setDualValues)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(discrete, setDiscrete)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(step, setStep)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(reversed, setReversed)
		LAYOUT_CONTROL_DRAWABLE_ATTR(track, setTrackDrawable)
		LAYOUT_CONTROL_DRAWABLE_ATTR(progress, setProgressDrawable)
		LAYOUT_CONTROL_DRAWABLE_ATTR(progress2, setSecondaryProgressDrawable)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL


	BEGIN_PROCESS_LAYOUT_CONTROL(Slider, Slider)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(Progress)
		
		LAYOUT_CONTROL_DRAWABLE_ATTR(thumb, setThumbDrawable)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedThumb, setPressedThumbDrawable)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverThumb, setHoverThumbDrawable);

		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(thumbWidth, setThumbWidth, checkScalarSize);
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(thumbHeight, setThumbHeight, checkScalarSize);
		if (op == OP_PARSE) {
			SAppDimensionValue thumbSize;
			LAYOUT_CONTROL_PARSE_DIMENSION_ATTR(, thumbSize)
			if (!(thumbSize.checkScalarSize())) {
				LOG_ERROR_LAYOUT_CONTROL_ATTR(thumbSize)
				return sl_false;
			}
			if (thumbSize.flagDefined) {
				if (!(attr->thumbWidth.flagDefined)) {
					attr->thumbWidth = thumbSize;
				}
				if (!(attr->thumbHeight.flagDefined)) {
					attr->thumbHeight = thumbSize;
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	
	BEGIN_PROCESS_LAYOUT_CONTROL(Switch, SwitchView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		if (op == OP_PARSE) {
			String value = resourceItem->getXmlAttribute("value").trim().toLower();
			if (value == "left" || value == "off") {
				attr->value.flagDefined = sl_true;
				attr->value.value = sl_false;
			} else if (value == "right" || value == "on") {
				attr->value.flagDefined = sl_true;
				attr->value.value = sl_true;
			} else {
				LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(value, setValue)
			}
		} else {
			LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(value, setValue)
		}
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(textInButton, setTextInButton)

		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(text, setText)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(textOff, setTextOff)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(textOn, setTextOn)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(leftText, setLeftText)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(rightText, setRightText)
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		LAYOUT_CONTROL_COLOR_ATTR(textColorOff, setTextColorOff)
		LAYOUT_CONTROL_COLOR_ATTR(textColorOn, setTextColorOn)

		LAYOUT_CONTROL_DRAWABLE_ATTR(thumb, setThumb)
		LAYOUT_CONTROL_DRAWABLE_ATTR(thumbOff, setThumbOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(thumbOn, setThumbOn)
		LAYOUT_CONTROL_DRAWABLE_ATTR(track, setTrack)
		LAYOUT_CONTROL_DRAWABLE_ATTR(trackOff, setTrackOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(trackOn, setTrackOn)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedThumb, setPressedThumb)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedThumbOff, setPressedThumbOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedThumbOn, setPressedThumbOn)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedTrack, setPressedTrackOn)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedTrackOff, setPressedTrack)
		LAYOUT_CONTROL_DRAWABLE_ATTR(pressedTrackOn, setPressedTrackOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverThumb, setHoverThumb)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverThumbOff, setHoverThumbOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverThumbOn, setHoverThumbOn)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverTrack, setHoverTrack)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverTrackOff, setHoverTrackOff)
		LAYOUT_CONTROL_DRAWABLE_ATTR(hoverTrackOn, setHoverTrackOn)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	
	BEGIN_PROCESS_LAYOUT_CONTROL(Picker, PickerView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_COLOR_ATTR(textColor, setTextColor)
		
		LAYOUT_CONTROL_SET_NATIVE_WIDGET
		
		if (op == OP_PARSE) {
			ListElements< Ref<XmlElement> > itemXmls(_getLayoutItemChildElements(resourceItem, resource->name, "item"));
			for (sl_size i = 0; i < itemXmls.count; i++) {
				Ref<XmlElement>& itemXml = itemXmls[i];
				if (itemXml.isNotNull()) {
					SAppLayoutSelectItem subItem;
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., title)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., value)
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., selected)
					subItem.element = itemXml;
					if (!(attr->items.add(subItem))) {
						_logError(itemXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutSelectItem> selectItems(attr->items);
			if (selectItems.count > 0) {
				sl_size indexSelected = 0;
				sl_bool flagSelected = sl_false;
				params->sbDefineInit->add(String::format("%s%s->setItemsCount(%d, slib::UIUpdateMode::Init);%n", strTab, name, selectItems.count));
				for (sl_size i = 0; i < selectItems.count; i++) {
					SAppLayoutSelectItem& selectItem = selectItems[i];
					if (selectItem.title.flagDefined) {
						String strSelectItemTitle;
						if (!(_getStringAccessString(resource->name, selectItem.title, selectItem.element, strSelectItemTitle))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setItemTitle(%d, %s, slib::UIUpdateMode::Init);%n", strTab, name, i, strSelectItemTitle));
					}
					if (selectItem.value.flagDefined) {
						String strSelectItemValue;
						if (!(_getStringAccessString(resource->name, selectItem.value, selectItem.element, strSelectItemValue))) {
							return sl_false;
						}
						params->sbDefineInit->add(String::format("%s%s->setItemValue(%d, %s);%n", strTab, name, i, strSelectItemValue));
					}
					if (selectItem.selected.flagDefined && selectItem.selected.value) {
						flagSelected = sl_true;
						indexSelected = i;
					}
				}
				if (flagSelected) {
					params->sbDefineInit->add(String::format("%s%s->selectIndex(%d, slib::UIUpdateMode::Init);%n", strTab, name, indexSelected));
				}
			}
		} else if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				ListLocker<SAppLayoutSelectItem> selectItems(attr->items);
				if (selectItems.count > 0) {
					sl_uint32 indexSelected = 0;
					sl_bool flagSelected = sl_false;
					sl_uint32 n = (sl_uint32)(selectItems.count);
					view->setItemsCount(n, UIUpdateMode::Init);
					for (sl_uint32 i = 0; i < n; i++) {
						SAppLayoutSelectItem& selectItem = selectItems[i];
						if (selectItem.title.flagDefined) {
							String selectItemTitle;
							if (!(_getStringValue(resource->name, selectItem.title, selectItem.element, selectItemTitle))) {
								return sl_false;
							}
							view->setItemTitle(i, selectItemTitle, UIUpdateMode::Init);
						}
						if (selectItem.value.flagDefined) {
							String selectItemValue;
							if (!(_getStringValue(resource->name, selectItem.value, selectItem.element, selectItemValue))) {
								return sl_false;
							}
							view->setItemValue(i, selectItemValue);
						}
						if (selectItem.selected.flagDefined && selectItem.selected.value) {
							flagSelected = sl_true;
							indexSelected = i;
						}
					}
					if (flagSelected) {
						if (!flagOnLayout) {
							view->selectIndex(indexSelected, UIUpdateMode::Init);
						}
					}
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(DatePicker, DatePicker)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(date, setDate)

		LAYOUT_CONTROL_ADD_STATEMENT

	}
	END_PROCESS_LAYOUT_CONTROL
	
	BEGIN_PROCESS_LAYOUT_CONTROL(Pager, ViewPager)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR(loop, setLoop)
		
		if (op == OP_PARSE) {
			ListLocker< Ref<XmlElement> > itemXmls(_getLayoutItemChildElements(resourceItem, resource->name, "item"));
			for (sl_size i = 0; i < itemXmls.count; i++) {
				Ref<XmlElement>& itemXml = itemXmls[i];
				if (itemXml.isNotNull()) {
					SAppLayoutPagerItem subItem;
					LAYOUT_CONTROL_PARSE_XML_ATTR(itemXml, subItem., selected)
					
					sl_size nViews = itemXml->getChildElementsCount();
					if (nViews > 0) {
						if (nViews != 1) {
							_logError(itemXml, g_str_error_resource_layout_item_must_contain_one_child);
							return sl_false;
						}
						Ref<XmlElement> xmlView= itemXml->getFirstChildElement();
						if (xmlView.isNotNull()) {
							Ref<SAppLayoutResourceItem> subItemView = _parseLayoutResourceItemChild(params->resource, resourceItem, xmlView, params->source);
							if (subItemView.isNull()) {
								return sl_false;
							}
							subItemView->attrsView->resetLayout();
							subItem.view = subItemView;
						}
					}
					
					subItem.element = itemXml;
					
					if (!(attr->items.add(subItem))) {
						_logError(itemXml, g_str_error_out_of_memory);
						return sl_false;
					}
				}
			}
		} else if (op == OP_GENERATE_CPP) {
			/* see below */
		} else if (op == OP_SIMULATE) {
			ListLocker<SAppLayoutPagerItem> subItems(attr->items);
			if (subItems.count > 0) {
				
				sl_uint32 indexSelected = 0;
				
				sl_uint32 nSubItems = (sl_uint32)(subItems.count);
				
				for (sl_uint32 i = 0; i < nSubItems; i++) {
					
					SAppLayoutPagerItem& subItem = subItems[i];
					
					if (subItem.selected.flagDefined && subItem.selected.value) {
						indexSelected = i;
					}
					
					if (subItem.view.isNotNull()) {
						Ref<View> contentView = _simulateLayoutCreateOrLayoutView(params->simulator, subItem.view.get(), resourceItem, view, flagOnLayout);
						if (contentView.isNotNull()) {
							if (!flagOnLayout) {
								view->addPage(contentView, UIUpdateMode::Init);
							}
						} else {
							return sl_false;
						}
					}
					
				}
				
				if (!flagOnLayout) {
					view->selectPage(indexSelected);
				}
				
			}
			
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_GENERATE_CPP) {
			ListLocker<SAppLayoutPagerItem> subItems(attr->items);
			if (subItems.count > 0) {
				sl_size indexSelected = 0;
				for (sl_size i = 0; i < subItems.count; i++) {
					SAppLayoutPagerItem& subItem = subItems[i];
					if (subItem.view.isNotNull()) {
						String addChildStatement = String::format("%s%s->addPage(%s, slib::UIUpdateMode::Init);%n%n", strTab, name, subItem.view->name);
						if (!(_generateLayoutsCpp_Item(params->resource, subItem.view.get(), resourceItem, *(params->sbDeclare), *(params->sbDefineInit), *(params->sbDefineLayout), addChildStatement))) {
							return sl_false;
						}
					}
					if (subItem.selected.flagDefined && subItem.selected.value) {
						indexSelected = i;
					}
				}
				params->sbDefineInit->add(String::format("%s%s->selectPage(%d);%n", strTab, name, indexSelected));
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(Navigation, ViewPageNavigationController)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR(swipe, setSwipeNavigation)
		
		LAYOUT_CONTROL_ADD_STATEMENT

	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(Video, VideoView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_GENERIC_ATTR(repeat, setRepeat)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(rotation, setRotation)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(flip, setFlip)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(scale, setScaleMode)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(gravity, setGravity)
		LAYOUT_CONTROL_GENERIC_ATTR_NOREDRAW(controls, setControlsVisible)

		if (op == OP_PARSE || op == OP_GENERATE_CPP) {
			LAYOUT_CONTROL_STRING_ATTR(src, setSource)
		} else {
			if (!flagOnLayout) {
				if (attr->src.flagDefined) {
					String value;
					if (!(_getStringValue(resource->name, attr->src, element, value))) {
						return sl_false;
					}
					if (value.startsWith("asset://")) {
						value = m_pathApp + "/asset/" + value.substring(8);
					}
					view->setSource(value);
				}
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
	}
	END_PROCESS_LAYOUT_CONTROL

	BEGIN_PROCESS_LAYOUT_CONTROL(Camera, CameraView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(Video)
		
		LAYOUT_CONTROL_STRING_ATTR(device, setDeviceId)
		LAYOUT_CONTROL_GENERIC_ATTR(autoStart, setAutoStart)
		LAYOUT_CONTROL_GENERIC_ATTR(touchFocus, setTouchFocusEnabled)

		LAYOUT_CONTROL_ADD_STATEMENT
	}
	END_PROCESS_LAYOUT_CONTROL
	
#define SAppLayoutQRCodeScannerAttributes SAppLayoutCameraAttributes
#define attrsQRCodeScanner attrsCamera
	BEGIN_PROCESS_LAYOUT_CONTROL(QRCodeScanner, QRCodeScanner)
	{
		if (!(_processLayoutResourceControl_Camera(params))) {
			return sl_false;
		}
	}
	END_PROCESS_LAYOUT_CONTROL
	
	BEGIN_PROCESS_LAYOUT_CONTROL(Drawer, Drawer)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(ViewGroup)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(drawerSize, setDrawerSize, checkScalarSize)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR(dragEdgeSize, setDragEdgeSize, checkScalarSize)
		LAYOUT_CONTROL_GENERIC_ATTR(gravity, setGravity)
		
		LAYOUT_CONTROL_ADD_STATEMENT
	}
	END_PROCESS_LAYOUT_CONTROL
	
	BEGIN_PROCESS_LAYOUT_CONTROL(Chat, ChatView)
	{
		LAYOUT_CONTROL_PROCESS_SUPER(View)
		
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(chatWidth, setChatWidth, checkScalarSizeOrWeight)
		LAYOUT_CONTROL_INT_DIMENSION_ATTR_NOREDRAW(userIconSize, setUserIconSize, checkScalarSizeOrWeight)
		if (op == OP_GENERATE_CPP) {
			if (attr->chatWidth.flagDefined && attr->chatWidth.unit == SAppDimensionValue::WEIGHT) {
				params->sbDefineInit->add(String::format("%s%s->setChatWidthWeight(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->chatWidth.amount));
			}
			if (attr->userIconSize.flagDefined && attr->userIconSize.unit == SAppDimensionValue::WEIGHT) {
				params->sbDefineInit->add(String::format("%s%s->setUserIconSizeWeight(%ff, slib::UIUpdateMode::Init);%n", strTab, name, attr->userIconSize.amount));
			}
		} else if (op == OP_SIMULATE) {
			if (flagOnLayout) {
				if (attr->chatWidth.flagDefined && attr->chatWidth.unit == SAppDimensionValue::WEIGHT) {
					view->setChatWidthWeight(attr->chatWidth.amount);
				}
				if (attr->userIconSize.flagDefined && attr->userIconSize.unit == SAppDimensionValue::WEIGHT) {
					view->setUserIconSizeWeight(attr->userIconSize.amount);
				}
			}
		}
		
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(dateFormat, setDateFormat)
		LAYOUT_CONTROL_STRING_ATTR_NOREDRAW(timeFormat, setTimeFormat)
		LAYOUT_CONTROL_COLOR_ATTR(receivedChatBackColor, setReceivedChatBackColor)
		LAYOUT_CONTROL_COLOR_ATTR(receivedChatTextColor, setReceivedChatTextColor)
		LAYOUT_CONTROL_COLOR_ATTR(sentChatBackColor, setSentChatBackColor)
		LAYOUT_CONTROL_COLOR_ATTR(sentChatTextColor, setSentChatTextColor)
		LAYOUT_CONTROL_COLOR_ATTR(dateTextColor, setDateTextColor)

		if (op == OP_SIMULATE) {
			if (!flagOnLayout) {
				List<ChatViewItem> items;
				Ref<Drawable> iconCustomer = Drawable::createColorDrawable(Color::Red)->clipEllipse();
				Ref<Drawable> iconMe = Drawable::createColorDrawable(Color::Blue)->clipEllipse();
				for (sl_size i = 0; i < 1000; i++) {
					ChatViewItem item;
					if (i % 2) {
						item.userName = "Customer";
						item.userIcon = iconCustomer;
						item.flagMe = sl_false;
						item.message = String::format("Chat Message %d", i);
					} else {
						item.userIcon = iconMe;
						item.flagMe = sl_true;
						item.message = String::format("Welcome to this message %d. This is the test message for multi-line wrapping.", i);
					}
					item.time = Time::now() - Time::withMinutes((1000 - i) * 30);
					items.add_NoLock(item);
				}
				view->setItems(items);
			}
		}
		
		LAYOUT_CONTROL_ADD_STATEMENT
	}
	END_PROCESS_LAYOUT_CONTROL
	
	BEGIN_PROCESS_LAYOUT_CONTROL(Refresh, RefreshView)
	{
		
		LAYOUT_CONTROL_PROCESS_SUPER(ViewGroup)
		
		LAYOUT_CONTROL_ADD_STATEMENT
		
		if (op == OP_PARSE) {
			sl_size nChildren = element->getChildElementsCount();
			if (nChildren > 0) {
				if (nChildren != 1) {
					_logError(element, g_str_error_resource_layout_refreshview_must_contain_one_child);
					return sl_false;
				}
			}
		}
		
	}
	END_PROCESS_LAYOUT_CONTROL

}
