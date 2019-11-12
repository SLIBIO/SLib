namespace sapp
{
	namespace ui
	{
		SLIB_DEFINE_PAGE_LAYOUT(LaunchScreen)

		void LaunchScreen::initialize()
		{
			this->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			this->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			this->setAlignParentLeft(slib::UIUpdateMode::Init);
			this->setAlignParentTop(slib::UIUpdateMode::Init);
			this->setBackground(slib::Drawable::createColorDrawable(slib::Color(255, 255, 255, 255)), slib::UIUpdateMode::Init);

			_image1 = new slib::ImageView;
			_image1->setWidthWeight(0.8f, slib::UIUpdateMode::Init);
			_image1->setCenterHorizontal(slib::UIUpdateMode::Init);
			_image1->setCenterVertical(slib::UIUpdateMode::Init);
			_image1->setAutoAspectRatio(sl_true);
			_image1->setAspectRatioMode(slib::AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);
			_image1->setSource(drawable::slib_icon::get(), slib::UIUpdateMode::Init);
			m_contentView->addChild(_image1, slib::UIUpdateMode::Init);

		}

		void LaunchScreen::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
