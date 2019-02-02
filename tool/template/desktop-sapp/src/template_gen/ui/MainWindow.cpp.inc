

		SLIB_DEFINE_WINDOW_LAYOUT(MainWindow)

		void MainWindow::initialize()
		{
			this->setTitle("Hello");
			this->setWidth(400);
			this->setHeight(300);
			this->setCenterScreenOnCreate(sl_true);

			_linear1 = new slib::LinearView;
			_linear1->setWidthWrapping(slib::UIUpdateMode::Init);
			_linear1->setHeightWrapping(slib::UIUpdateMode::Init);
			_linear1->setCenterHorizontal(slib::UIUpdateMode::Init);
			_linear1->setCenterVertical(slib::UIUpdateMode::Init);
			m_contentView->addChild(_linear1, slib::UIUpdateMode::Init);

			_image1 = new slib::ImageView;
			_image1->setWidth(150, slib::UIUpdateMode::Init);
			_image1->setAlignParentLeft(slib::UIUpdateMode::Init);
			_image1->setSource(drawable::slib_icon::get(), slib::UIUpdateMode::Init);
			_image1->setAutoAspectRatio(sl_true);
			_image1->setAspectRatioMode(slib::AspectRatioMode::AdjustHeight, slib::UIUpdateMode::Init);
			_linear1->addChild(_image1, slib::UIUpdateMode::Init);

			_label1 = new slib::LabelView;
			_label1->setWidthWrapping(slib::UIUpdateMode::Init);
			_label1->setHeightWrapping(slib::UIUpdateMode::Init);
			_label1->setCenterHorizontal(slib::UIUpdateMode::Init);
			_label1->setFont(slib::Font::create(_label1->getFontFamily(), 20.0f, sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			_label1->setText("Hello World", slib::UIUpdateMode::Init);
			_label1->setTextColor(slib::Color(0, 0, 0, 255), slib::UIUpdateMode::Init);
			_linear1->addChild(_label1, slib::UIUpdateMode::Init);

		}

		void MainWindow::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

