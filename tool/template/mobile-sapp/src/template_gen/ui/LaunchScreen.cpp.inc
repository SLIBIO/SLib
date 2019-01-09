

		SLIB_DEFINE_PAGE_LAYOUT(LaunchScreen)

		void LaunchScreen::initialize()
		{
			this->setWidth(slib::UIResource::getScreenWidth(), slib::UIUpdateMode::Init);
			this->setHeight(slib::UIResource::getScreenHeight(), slib::UIUpdateMode::Init);
			this->setAlignParentLeft(slib::UIUpdateMode::Init);
			this->setAlignParentTop(slib::UIUpdateMode::Init);
			this->setBackground(slib::Drawable::createColorDrawable(slib::Color(255, 255, 255, 255)), slib::UIUpdateMode::Init);

			_image1 = new slib::ImageView;
			_image1->setWidthWeight(0.8f, slib::UIUpdateMode::Init);
			_image1->setCenterHorizontal(slib::UIUpdateMode::Init);
			_image1->setCenterVertical(slib::UIUpdateMode::Init);
			_image1->setSource(drawable::slib_icon::get(), slib::UIUpdateMode::Init);
			_image1->setAspectRatioMode(slib::AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);
			m_contentView->addChild(_image1, slib::UIUpdateMode::Init);

		}

		void LaunchScreen::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

