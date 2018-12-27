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

#include "sapp.h"

#include "slib/ui.h"


namespace slib
{

	Ref<Image> SAppDrawableResourceImageItem::loadImage()
	{
		ObjectLocker lock(this);
		if (image.isNull()) {
			Ref<Image> image = Image::loadFromFile(filePath);
			if (image.isNotNull()) {
				image->setCustomDrawable(Image::loadAnimationFromFile(filePath));
			}
			this->image = image;
		}
		return image;
	}

	SAppDrawableResourceImageAttributes::SAppDrawableResourceImageAttributes()
	{
		defaultImages = List< Ref<SAppDrawableResourceImageItem> >::create();
	}

	SAppDrawableResource::SAppDrawableResource()
	{
		type = typeUnknown;
	}


	SAppLayoutViewAttributes::SAppLayoutViewAttributes()
	{
		finalFontBold = sl_false;
		finalFontItalic = sl_false;
		finalFontUnderline = sl_false;
	}

	sl_bool SAppLayoutViewAttributes::isNotRequiredNative(sl_bool flagCheckBackgroundColor)
	{
		if (background.flagDefined) {
			if (flagCheckBackgroundColor) {
				return sl_true;
			} else {
				if (!(background.flagColor)) {
					return sl_true;
				}
			}
		}
		if (pressedBackground.flagDefined) {
			return sl_true;
		}
		if (hoverBackground.flagDefined) {
			return sl_true;
		}
		if (borderWidth.flagDefined) {
			return sl_true;
		}
		if (borderColor.flagDefined) {
			return sl_true;
		}
		if (borderStyle.flagDefined) {
			return sl_true;
		}
		return sl_false;
	}


	sl_bool SAppLayoutButtonAttributes::isNotRequiredNative()
	{
		if (textColor.flagDefined) {
			return sl_true;
		}
		if (icon.flagDefined) {
			return sl_true;
		}
		if (iconWidth.flagDefined) {
			return sl_true;
		}
		if (iconHeight.flagDefined) {
			return sl_true;
		}
		if (gravity.flagDefined) {
			return sl_true;
		}
		if (iconAlign.flagDefined) {
			return sl_true;
		}
		if (textAlign.flagDefined) {
			return sl_true;
		}
		if (textBeforeIcon.flagDefined) {
			return sl_true;
		}
		if (orientation.flagDefined) {
			return sl_true;
		}
		if (iconMarginLeft.flagDefined) {
			return sl_true;
		}
		if (iconMarginTop.flagDefined) {
			return sl_true;
		}
		if (iconMarginRight.flagDefined) {
			return sl_true;
		}
		if (iconMarginBottom.flagDefined) {
			return sl_true;
		}
		if (textMarginLeft.flagDefined) {
			return sl_true;
		}
		if (textMarginTop.flagDefined) {
			return sl_true;
		}
		if (textMarginRight.flagDefined) {
			return sl_true;
		}
		if (textMarginBottom.flagDefined) {
			return sl_true;
		}
		if (defaultColorFilter.flagDefined) {
			return sl_true;
		}
		if (colorOverlay.flagDefined) {
			return sl_true;
		}
		for (sl_uint32 i = 0; i < SLIB_SAPP_LAYOUT_BUTTON_CATEGORY_MAX; i++) {
			SAppLayoutButtonCategory& category = categories[i];
			for (sl_uint32 k = 0; k < (sl_uint32)(ButtonState::Count); k++) {
				if (category.textColor[k].flagDefined) {
					return sl_true;
				}
				if (category.icon[k].flagDefined) {
					return sl_true;
				}
				if (category.background[k].flagDefined) {
					return sl_true;
				}
				if (category.borderWidth[k].flagDefined || category.borderColor[k].flagDefined || category.borderStyle[k].flagDefined) {
					return sl_true;
				}
				if (category.colorOverlay[k].flagDefined) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}


	sl_bool SAppLayoutLabelAttributes::isNotRequiredNative()
	{
		return sl_false;
	}


	sl_bool SAppLayoutEditAttributes::isNotRequiredNative()
	{
		return sl_false;
	}


	sl_bool SAppLayoutSelectAttributes::isNotRequiredNative()
	{
		if (leftIcon.flagDefined) {
			return sl_true;
		}
		if (rightIcon.flagDefined) {
			return sl_true;
		}
		if (iconWidth.flagDefined) {
			return sl_true;
		}
		if (iconHeight.flagDefined) {
			return sl_true;
		}
		return sl_false;
	}


	sl_bool SAppLayoutTabAttributes::isNotRequiredNative()
	{
		if (orientation.flagDefined) {
			return sl_true;
		}
		if (tabWidth.flagDefined) {
			return sl_true;
		}
		if (tabWidth.flagDefined) {
			return sl_true;
		}
		if (barBackground.flagDefined) {
			return sl_true;
		}
		if (contentBackground.flagDefined) {
			return sl_true;
		}
		if (tabBackground.flagDefined) {
			return sl_true;
		}
		if (selectedTabBackground.flagDefined) {
			return sl_true;
		}
		if (hoverTabBackground.flagDefined) {
			return sl_true;
		}
		if (labelColor.flagDefined) {
			return sl_true;
		}
		if (selectedLabelColor.flagDefined) {
			return sl_true;
		}
		if (hoverLabelColor.flagDefined) {
			return sl_true;
		}
		if (labelAlign.flagDefined) {
			return sl_true;
		}
		if (labelMarginLeft.flagDefined) {
			return sl_true;
		}
		if (labelMarginTop.flagDefined) {
			return sl_true;
		}
		if (labelMarginRight.flagDefined) {
			return sl_true;
		}
		if (labelMarginBottom.flagDefined) {
			return sl_true;
		}
		return sl_false;
	}

	sl_bool SAppLayoutPickerAttributes::isNotRequiredNative()
	{
		if (textColor.flagDefined) {
			return sl_true;
		}
		return sl_false;
	}


	SAppLayoutResourceItem::SAppLayoutResourceItem()
	{
		itemType = SAppLayoutItemType::Unknown;
		flagGeneratedName = sl_false;
	}

	String SAppLayoutResourceItem::getXmlAttribute(const String& name)
	{
		String value = element->getAttribute(name);
		if (value.isNotNull()) {
			return value;
		}
		ListLocker< Ref<SAppLayoutStyle> > _styles(styles);
		for (sl_size i = 0; i < _styles.count; i++) {
			Ref<SAppLayoutStyle> style = _styles[i];
			if (style.isNotNull()) {
				value = style->element->getAttribute(name);
				if (value.isNotNull()) {
					return value;
				}
			}
		}
		return String::null();
	}

	SAppLayoutResource::SAppLayoutResource()
	{
		layoutType = SAppLayoutType::View;
		itemType = SAppLayoutItemType::ViewGroup;
		nAutoIncreaseNameView = 0;
		nAutoIncreaseNameViewGroup = 0;
		nAutoIncreaseNameImport = 0;
		nAutoIncreaseNameButton = 0;
		nAutoIncreaseNameLabel = 0;
		nAutoIncreaseNameCheck = 0;
		nAutoIncreaseNameRadio = 0;
		nAutoIncreaseNameEdit = 0;
		nAutoIncreaseNamePassword = 0;
		nAutoIncreaseNameTextArea = 0;
		nAutoIncreaseNameImage = 0;
		nAutoIncreaseNameSelect = 0;
		nAutoIncreaseNameScroll = 0;
		nAutoIncreaseNameLinear = 0;
		nAutoIncreaseNameList = 0;
		nAutoIncreaseNameListReport = 0;
		nAutoIncreaseNameRender = 0;
		nAutoIncreaseNameTab = 0;
		nAutoIncreaseNameTree = 0;
		nAutoIncreaseNameWeb = 0;
		nAutoIncreaseNameSplit = 0;
		nAutoIncreaseNameProgress = 0;
		nAutoIncreaseNameSlider = 0;
		nAutoIncreaseNamePicker = 0;
		nAutoIncreaseNamePager = 0;
		nAutoIncreaseNameVideo = 0;
	}

	String SAppLayoutResource::getAutoIncreasingName(SAppLayoutItemType type)
	{
		String prefix;
		sl_uint32* pN;
		switch (type) {
			case SAppLayoutItemType::Unknown:
				return String::null();
			case SAppLayoutItemType::View:
				prefix = "view";
				pN = &nAutoIncreaseNameView;
				break;
			case SAppLayoutItemType::ViewGroup:
				prefix = "group";
				pN = &nAutoIncreaseNameViewGroup;
				break;
			case SAppLayoutItemType::Import:
				prefix = "import";
				pN = &nAutoIncreaseNameImport;
				break;
			case SAppLayoutItemType::Button:
				prefix = "button";
				pN = &nAutoIncreaseNameButton;
				break;
			case SAppLayoutItemType::Label:
				prefix = "label";
				pN = &nAutoIncreaseNameLabel;
				break;
			case SAppLayoutItemType::Check:
				prefix = "check";
				pN = &nAutoIncreaseNameCheck;
				break;
			case SAppLayoutItemType::Radio:
				prefix = "radio";
				pN = &nAutoIncreaseNameRadio;
				break;
			case SAppLayoutItemType::Edit:
				prefix = "edit";
				pN = &nAutoIncreaseNameEdit;
				break;
			case SAppLayoutItemType::Password:
				prefix = "password";
				pN = &nAutoIncreaseNamePassword;
				break;
			case SAppLayoutItemType::TextArea:
				prefix = "textArea";
				pN = &nAutoIncreaseNameTextArea;
				break;
			case SAppLayoutItemType::Image:
				prefix = "image";
				pN = &nAutoIncreaseNameImage;
				break;
			case SAppLayoutItemType::Select:
				prefix = "select";
				pN = &nAutoIncreaseNameSelect;
				break;
			case SAppLayoutItemType::Scroll:
				prefix = "scroll";
				pN = &nAutoIncreaseNameScroll;
				break;
			case SAppLayoutItemType::Linear:
				prefix = "linear";
				pN = &nAutoIncreaseNameLinear;
				break;
			case SAppLayoutItemType::List:
				prefix = "list";
				pN = &nAutoIncreaseNameList;
				break;
			case SAppLayoutItemType::ListReport:
				prefix = "listReport";
				pN = &nAutoIncreaseNameListReport;
				break;
			case SAppLayoutItemType::Render:
				prefix = "render";
				pN = &nAutoIncreaseNameRender;
				break;
			case SAppLayoutItemType::Tab:
				prefix = "tab";
				pN = &nAutoIncreaseNameTab;
				break;
			case SAppLayoutItemType::Tree:
				prefix = "tree";
				pN = &nAutoIncreaseNameTree;
				break;
			case SAppLayoutItemType::Web:
				prefix = "web";
				pN = &nAutoIncreaseNameWeb;
				break;
			case SAppLayoutItemType::Split:
				prefix = "split";
				pN = &nAutoIncreaseNameSplit;
				break;
			case SAppLayoutItemType::Progress:
				prefix = "progress";
				pN = &nAutoIncreaseNameProgress;
				break;
			case SAppLayoutItemType::Slider:
				prefix = "slider";
				pN = &nAutoIncreaseNameSlider;
				break;
			case SAppLayoutItemType::Picker:
				prefix = "picker";
				pN = &nAutoIncreaseNamePicker;
				break;
			case SAppLayoutItemType::Pager:
				prefix = "pager";
				pN = &nAutoIncreaseNamePager;
				break;
			case SAppLayoutItemType::Video:
				prefix = "video";
				pN = &nAutoIncreaseNameVideo;
				break;
			case SAppLayoutItemType::Camera:
				prefix = "camera";
				pN = &nAutoIncreaseNameCamera;
				break;
			case SAppLayoutItemType::QRCodeScanner:
				prefix = "qrCodeScanner";
				pN = &nAutoIncreaseNameQRCodeScanner;
				break;
			default:
				return String::null();
		}
		while (1) {
			(*pN)++;
			String name = String::format("_%s%d", prefix, *pN);
			if (!(itemsByName.find(name))) {
				return name;
			}
		}
		return String::null();
	}

	SAppLayoutItemType SAppLayoutResource::getTypeFromName(const String& _strType)
	{
		String strType = _strType.toLower();
		SAppLayoutItemType type = SAppLayoutItemType::Unknown;
		if (strType == "view") {
			type = SAppLayoutItemType::View;
		} else if (strType == "group") {
			type = SAppLayoutItemType::ViewGroup;
		} else if (strType == "import") {
			type = SAppLayoutItemType::Import;
		} else if (strType == "button") {
			type = SAppLayoutItemType::Button;
		} else if (strType == "label") {
			type = SAppLayoutItemType::Label;
		} else if (strType == "check") {
			type = SAppLayoutItemType::Check;
		} else if (strType == "radio") {
			type = SAppLayoutItemType::Radio;
		} else if (strType == "edit") {
			type = SAppLayoutItemType::Edit;
		} else if (strType == "password") {
			type = SAppLayoutItemType::Password;
		} else if (strType == "textarea" || strType == "text-area") {
			type = SAppLayoutItemType::TextArea;
		} else if (strType == "image") {
			type = SAppLayoutItemType::Image;
		} else if (strType == "select") {
			type = SAppLayoutItemType::Select;
		} else if (strType == "scroll" || strType == "hscroll") {
			type = SAppLayoutItemType::Scroll;
		} else if (strType == "linear" || strType == "hlinear") {
			type = SAppLayoutItemType::Linear;
		} else if (strType == "list") {
			type = SAppLayoutItemType::List;
		} else if (strType == "listreport" || strType == "list-report") {
			type = SAppLayoutItemType::ListReport;
		} else if (strType == "render") {
			type = SAppLayoutItemType::Render;
		} else if (strType == "tab") {
			type = SAppLayoutItemType::Tab;
		} else if (strType == "tree") {
			type = SAppLayoutItemType::Tree;
		} else if (strType == "web") {
			type = SAppLayoutItemType::Web;
		} else if (strType == "split") {
			type = SAppLayoutItemType::Split;
		} else if (strType == "progress") {
			type = SAppLayoutItemType::Progress;
		} else if (strType == "slider") {
			type = SAppLayoutItemType::Slider;
		} else if (strType == "picker") {
			type = SAppLayoutItemType::Picker;
		} else if (strType == "pager") {
			type = SAppLayoutItemType::Pager;
		} else if (strType == "video") {
			type = SAppLayoutItemType::Video;
		} else if (strType == "camera") {
			type = SAppLayoutItemType::Camera;
		} else if (strType == "qrcodescanner" || strType == "qr-code-scanner") {
			type = SAppLayoutItemType::QRCodeScanner;
		}
		return type;
	}
	
	
	SAppLayoutSimulationParams::SAppLayoutSimulationParams()
	{
		screenWidth = 0;
		screenHeight = 0;
		viewportWidth = 0;
		viewportHeight = 0;
		sp = 1;
	}


	Ref<Referable> SAppLayoutSimulator::getReferable()
	{
		return m_refer;
	}

	Ref<View> SAppLayoutSimulator::getViewByName(const String& name)
	{
		return m_views.getValue(name, Ref<View>::null());
	}

	void SAppLayoutSimulator::registerViewByName(const String& name, const Ref<View>& view)
	{
		m_views.put(name, view);
	}

	Ref<RadioGroup> SAppLayoutSimulator::getRadioGroup(const String& name)
	{
		return m_radioGroups.getValue(name, Ref<RadioGroup>::null());
	}

	Ref<SAppDocument> SAppLayoutSimulator::getDocument()
	{
		return m_document;
	}

	Ref<SAppLayoutResource> SAppLayoutSimulator::getLayoutResource()
	{
		return m_layoutResource;
	}

	Ref<SAppLayoutSimulationWindow> SAppLayoutSimulator::getSimulationWindow()
	{
		return m_simulationWindow;
	}

	Ref<View> SAppLayoutSimulator::getSimulationContentView()
	{
		return m_simulationContentView;
	}

	SAppLayoutSimulationWindow::SAppLayoutSimulationWindow()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		m_simulationWindow = this;
		m_refer = this;
	}

	void SAppLayoutSimulationWindow::open(SAppDocument* doc, SAppLayoutResource* layout)
	{
		m_document = doc;
		m_layoutResource = layout;
		{
			ListElements<String> radioGroups(layout->radioGroups.getAllKeys());
			for (sl_size i = 0; i < radioGroups.count; i++) {
				Ref<RadioGroup> group = new RadioGroup;
				if (group.isNotNull()) {
					m_radioGroups.put(radioGroups[i], group);
				}
			}
		}
		Ref<View> viewContent;
		if (layout->layoutType == SAppLayoutType::Window) {
			m_simulationContentView = getContentView();
		} else {
			setCenterScreenOnCreate(sl_true);
			setResizable(sl_true);
			viewContent = new ViewGroup;
			m_simulationContentView = viewContent;
		}
		viewContent = doc->_simulateLayoutCreateOrLayoutView(this, layout, sl_null, sl_null, sl_false);
		setInitialized();
		if (viewContent.isNotNull()) {
			if (layout->layoutType != SAppLayoutType::Window) {
				if (viewContent->getBackgroundColor().isZero()) {
					viewContent->setBackgroundColor(Color::White, UIUpdateMode::Init);
				}
				setBackgroundColor(Color::Black);
				addView(viewContent);
			}
			doc->_simulateLayoutCreateOrLayoutView(this, layout, sl_null, sl_null, sl_true);
			create();
			doc->_registerLayoutSimulationWindow(this);
		}
	}

	void SAppLayoutSimulationWindow::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		WindowLayoutResource::dispatchResize(width, height);
		layoutViews(width, height);
	}
	
	void SAppLayoutSimulationWindow::layoutViews(sl_ui_len width, sl_ui_len height)
	{
		Ref<SAppDocument> doc = m_document;
		Ref<SAppLayoutResource> layout = m_layoutResource;
		if (doc.isNotNull() && layout.isNotNull()) {
			doc->_simulateLayoutCreateOrLayoutView(this, layout.get(), sl_null, sl_null, sl_true);
		}
	}

	void SAppLayoutSimulationWindow::onClose(UIEvent* ev)
	{
		Ref<SAppDocument> doc = m_document;
		if (doc.isNotNull()) {
			doc->_removeLayoutSimulationWindow(this);
		}
	}


	SAppLayoutImportView::SAppLayoutImportView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		m_refer = this;
	}

	void SAppLayoutImportView::init(SAppLayoutSimulator* simulator, SAppLayoutResource* layout)
	{
		Ref<SAppDocument> document = simulator->getDocument();
		m_document = document;
		m_simulationWindow = simulator->getSimulationWindow();
		m_layoutResource = layout;
		{
			ListElements<String> radioGroups(layout->radioGroups.getAllKeys());
			for (sl_size i = 0; i < radioGroups.count; i++) {
				Ref<RadioGroup> group = new RadioGroup;
				if (group.isNotNull()) {
					m_radioGroups.put(radioGroups[i], group);
				}
			}
		}
		m_simulationContentView = this;
		Ref<View> viewContent = document->_simulateLayoutCreateOrLayoutView(this, layout, sl_null, sl_null, sl_false);
		setInitialized();
		if (viewContent.isNotNull()) {
			document->_simulateLayoutCreateOrLayoutView(this, layout, sl_null, sl_null, sl_true);
		}
	}

	void SAppLayoutImportView::layoutViews(sl_ui_len width, sl_ui_len height)
	{
		Ref<SAppDocument> doc = m_document;
		Ref<SAppLayoutResource> layout = m_layoutResource;
		if (doc.isNotNull() && layout.isNotNull()) {
			doc->_simulateLayoutCreateOrLayoutView(this, layout.get(), sl_null, sl_null, sl_true);
		}
	}

}
