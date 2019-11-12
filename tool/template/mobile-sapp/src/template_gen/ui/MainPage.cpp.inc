namespace sapp
{
	namespace ui
	{
		SLIB_DEFINE_PAGE_LAYOUT(MainPage)

		void MainPage::initialize()
		{
			this->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			this->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			this->setAlignParentLeft(slib::UIUpdateMode::Init);
			this->setAlignParentTop(slib::UIUpdateMode::Init);
			this->setBackground(slib::Drawable::createColorDrawable(slib::Color(255, 255, 255, 255)), slib::UIUpdateMode::Init);

			_label1 = new slib::LabelView;
			_label1->setWidthWrapping(slib::UIUpdateMode::Init);
			_label1->setHeightWrapping(slib::UIUpdateMode::Init);
			_label1->setCenterHorizontal(slib::UIUpdateMode::Init);
			_label1->setCenterVertical(slib::UIUpdateMode::Init);
			_label1->setFont(slib::Font::create(slib::UI::getDefaultFontFamily(), slib::UIResource::millimeterToPixel(5.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			_label1->setText("Hello World", slib::UIUpdateMode::Init);
			_label1->setTextColor(slib::Color(0, 0, 0, 255), slib::UIUpdateMode::Init);
			m_contentView->addChild(_label1, slib::UIUpdateMode::Init);

		}

		void MainPage::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
