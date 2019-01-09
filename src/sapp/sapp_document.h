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

#ifndef CHECKHEADER_SLIB_SDEV_SAPP_DOCUMENT
#define CHECKHEADER_SLIB_SDEV_SAPP_DOCUMENT

#include "sapp_values.h"
#include "sapp_resources.h"

#include "slib/core/map.h"
#include "slib/core/locale.h"

namespace slib
{

	class SAppConfiguration
	{
	public:
		String app_path;
		
		String generate_cpp_target_path;
		String generate_cpp_namespace;
		CList<String> generate_cpp_layout_include_headers;
		CList<String> generate_cpp_layout_include_headers_in_cpp;
		
		Locale simulator_locale;
		
	public:
		SAppConfiguration();
		
		SAppConfiguration(SAppConfiguration&& other);

		~SAppConfiguration();
				
		SAppConfiguration& operator=(SAppConfiguration&& other);
		
	};
	
	class SAppSimulateLayoutParam
	{
	public:
		UISize windowSize;
		Function<void(Window*, UIEvent*)> onCloseWindow;

	public:
		SAppSimulateLayoutParam();
	};
	
	class SAppDocument : public Object
	{
	public:
		SAppDocument();
		~SAppDocument();

	public:
		sl_bool open(const String& filePath);
		
		void close();
		
		sl_bool isOpened();
	
		
		void clearAllResources();
		
		sl_bool openResources();
		
		sl_bool openUiResource(const String& filePath);
		
		
		sl_bool generateCpp();
		
		List< Ref<SAppLayoutResource> > getLayouts();
		
		void simulateLayoutInWindow(const String& layoutName, const SAppSimulateLayoutParam& param);
		
		Locale getCurrentSimulatorLocale();
		
		
		// Utilities
		static String getShortcutKeyDefinitionString(const KeycodeAndModifiers& km, sl_bool flagMac);
		
		static String getNameInLocalNamespace(const String& localNamespace, const String& name);
		
	protected:
		sl_bool _openImageResources();
		sl_bool _openRawResources();
		sl_bool _openGlobalResources();
		sl_bool _openUiResources();
		sl_bool _openUiResource(const String& path);

		// Log
		void _log(const String& text);
		void _logError(const String& text);
		void _logError(const String& filePath, sl_size line, sl_size col, const String& text);
		void _logError(const Ref<XmlElement>& element, const String& text);

		// Resources Entry
		sl_bool _parseConfiguration(const String& filePath, SAppConfiguration& conf);
		void _freeResources();
		sl_bool _parseResourcesXml(const String& localNamespace, const String& filePath);
		sl_bool _generateResourcesH(const String& targetPath);
		
		// Raw Resources
		sl_bool _registerRawResources(const String& fileDirPath);
		sl_bool _registerRawResource(const String& resourceName, const String& filePath, String& outName);
		sl_bool _generateRawCpp(const String& targetPath);
		sl_bool _generateRawDataFile(const String& targetPath, const String& sourcePath, const String& resourceName);
		
		// String Resources
		sl_bool _parseStringResources(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _parseStringResource(const String& localNamespace, const Ref<XmlElement>& element, Locale locale);
		sl_bool _generateStringsCpp(const String& targetPath);
		sl_bool _getStringAccessString(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _getStringValue(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _checkStringValue(const String& localNamespace, const SAppStringValue& value, const Ref<XmlElement>& element);
		sl_bool _checkStringName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName = sl_null, Ref<SAppStringResource>* outResource = sl_null);
		
		// Color Resources
		sl_bool _parseColorResource(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _generateColorsCpp(const String& targetPath);
		sl_bool _getColorAccessString(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _getColorValue(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element, Color& result);
		sl_bool _checkColorValue(const String& localNamespace, const SAppColorValue& value, const Ref<XmlElement>& element);
		sl_bool _checkColorName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName = sl_null, Ref<SAppColorResource>* outResource = sl_null);

		// Drawable Resources
		sl_bool _generateDrawablesCpp(const String& targetPath);
		sl_bool _getDrawableAccessString(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _getDrawableValue(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element, Ref<Drawable>& result);
		sl_bool _checkDrawableValue(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element);
		sl_bool _checkDrawableName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName = sl_null, Ref<SAppDrawableResource>* outResource = sl_null);
		sl_bool _getImageAccessString(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _getImageValue(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element, Ref<Image>& result);
		sl_bool _checkImageValue(const String& localNamespace, const SAppDrawableValue& value, const Ref<XmlElement>& element);
		sl_bool _registerImageResources(const String& resourcePath, const String& fileDirPath, const Locale& locale);
		sl_bool _generateDrawablesCpp_Image(SAppDrawableResource* res, StringBuffer& sbHeader, StringBuffer& sbCpp, StringBuffer& sbMap);
		Ref<Drawable> _getDrawableValue_Image(SAppDrawableResource* res);
		sl_bool _parseNinePiecesDrawableResource(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _generateDrawablesCpp_NinePieces(SAppDrawableResource* res, StringBuffer& sbHeader, StringBuffer& sbCpp, StringBuffer& sbMap);
		Ref<Drawable> _getDrawableValue_NinePieces(SAppDrawableResource* res);
		sl_bool _parseNinePatchDrawableResource(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _generateDrawablesCpp_NinePatch(SAppDrawableResource* res, StringBuffer& sbHeader, StringBuffer& sbCpp, StringBuffer& sbMap);
		Ref<Drawable> _getDrawableValue_NinePatch(SAppDrawableResource* res);
		
		// Menu Resources
		sl_bool _parseMenuResource(const String& localNamespace, const Ref<XmlElement>& element);
		Ref<SAppMenuResourceItem> _parseMenuResourceItem(const String& localNamespace, const Ref<XmlElement>& element, SAppMenuResource* menu, int platforms);
		sl_bool _generateMenusCpp(const String& targetPath);
		sl_bool _generateMenusCpp_Item(SAppMenuResource* resource, const String& parentName, int parentPlatforms, SAppMenuResourceItem* item, StringBuffer& sbHeader, StringBuffer& sbCpp, int tabLevel);
		sl_bool _getMenuAccessString(const String& localNamespace, const SAppMenuValue& value, const Ref<XmlElement>& element, String& result);
		sl_bool _getMenuValue(const String& localNamespace, const SAppMenuValue& value, const Ref<XmlElement>& element, Ref<Menu>& result);
		sl_bool _getMenuValue_Item(SAppMenuResource* resource, const Ref<Menu>& parent, SAppMenuResourceItem* item);
		sl_bool _checkMenuValue(const String& localNamespace, const SAppMenuValue& value, const Ref<XmlElement>& element);
		sl_bool _checkMenuName(const String& localNamespace, const String& name, const Ref<XmlElement>& element, String* outName = sl_null, Ref<SAppMenuResource>* outResource = sl_null);
		
		// Layout Resources
		sl_bool _parseLayoutStyle(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _parseLayoutInclude(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _parseLayoutResource(const String& localNamespace, const Ref<XmlElement>& element);
		sl_bool _parseLayoutResourceItem(SAppLayoutResource* layout, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent);
		Ref<SAppLayoutResourceItem> _parseLayoutResourceItemChild(SAppLayoutResource* layout, SAppLayoutResourceItem* parentItem, const Ref<XmlElement>& element);
		sl_bool _generateLayoutsCpp(const String& targetPath);
		sl_bool _generateLayoutsCpp_Layout(const String& targetPath, SAppLayoutResource* layout);
		sl_bool _generateLayoutsCpp_Item(SAppLayoutResource* layout, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent, StringBuffer& sbDeclare, StringBuffer& sbDefineInit, StringBuffer& sbDefineLayout, const String& addStatement);
		void _simulateLayoutInWindow(SAppLayoutResource* layout, const SAppSimulateLayoutParam& param);
		void _registerLayoutSimulationWindow(const Ref<SAppLayoutSimulationWindow>& window);
		void _removeLayoutSimulationWindow(const Ref<SAppLayoutSimulationWindow>& window);
		Ref<View> _simulateLayoutCreateOrLayoutView(SAppLayoutSimulator* simulator, SAppLayoutResourceItem* item, SAppLayoutResourceItem* parent, View* parentView, sl_bool flagOnLayout);
		sl_ui_pos _getDimensionIntValue(const SAppDimensionValue& value);
		sl_real _getDimensionFloatValue(const SAppDimensionFloatValue& value);
		List< Ref<XmlElement> > _getLayoutItemChildElements(SAppLayoutResourceItem* item, const String& localNamespace, const String& tagName);
		sl_bool _addXmlChildElements(List< Ref<XmlElement> >& list, const Ref<XmlElement>& parent, const String& localNamespace, const String& tagName);

		struct LayoutControlProcessParams
		{
			int op;
			SAppLayoutResource* resource;
			SAppLayoutResourceItem* resourceItem;
			SAppLayoutResourceItem* parentResourceItem;
			String name;
			String addStatement;
			StringBuffer* sbDeclare;
			StringBuffer* sbDefineInit;
			StringBuffer* sbDefineLayout;
			SAppLayoutSimulator* simulator;
			SAppLayoutSimulationWindow* window;
			Ref<View> view;
			View* parentView;
			sl_bool flagOnLayout;
		};
		sl_bool _processLayoutResourceControl(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Window(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Page(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_View(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_ViewGroup(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Import(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Button(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Label(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Check(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Radio(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Edit(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Password(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_TextArea(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Image(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Select(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Scroll(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Linear(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_List(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_ListReport(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Render(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Tab(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Tree(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Split(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Web(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Progress(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Slider(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Picker(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Pager(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Video(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Camera(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_QRCodeScanner(LayoutControlProcessParams* params);
		sl_bool _processLayoutResourceControl_Drawer(LayoutControlProcessParams* params);

	private:
		sl_bool m_flagOpened;
		String m_pathConf;
		String m_pathApp;
		
		SAppConfiguration m_conf;
		
		CMap< String, Ref<SAppRawResource> > m_raws;
		CMap< String, Ref<SAppDrawableResource> > m_drawables;
		CMap< String, Ref<SAppStringResource> > m_strings;
		CMap< String, Ref<SAppColorResource> > m_colors;
		CMap< String, Ref<SAppMenuResource> > m_menus;
		CHashMap< String, Ref<SAppLayoutStyle> > m_layoutStyles;
		CHashMap< String, Ref<SAppLayoutInclude> > m_layoutIncludes;
		CMap< String, Ref<SAppLayoutResource> > m_layouts;
		
		CList< Ref<SAppLayoutSimulationWindow> > m_layoutSimulationWindows;
		SAppLayoutSimulationParams m_layoutSimulationParams;
		
		friend class SAppLayoutSimulationWindow;
		friend class SAppLayoutImportView;
		
		template <class MAP, class ITEM>
		static sl_bool getItemFromMap(const MAP& map, const String& localNamespace, const String& name, String* outName, ITEM* _out);
		
	};
	
	
}

#endif
