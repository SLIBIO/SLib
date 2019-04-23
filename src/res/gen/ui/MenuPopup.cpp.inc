namespace slib
{
	namespace ui
	{
		SLIB_DEFINE_VIEW_LAYOUT(MenuPopup)

		void MenuPopup::initialize()
		{
			this->setWidthWrapping(slib::UIUpdateMode::Init);
			this->setHeightWrapping(slib::UIUpdateMode::Init);
			this->setLeft(0, slib::UIUpdateMode::Init);
			this->setTop(0, slib::UIUpdateMode::Init);
			this->setPaddingLeft(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			this->setPaddingTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			this->setPaddingRight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			this->setPaddingBottom(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			this->setBackground(slib::NinePatchDrawable::create(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(16.0f)), slib::UIResource::toUiPos(slib::UIResource::dpToPixel(16.0f)), slib::UIResource::toUiPos(slib::UIResource::dpToPixel(16.0f)), slib::UIResource::toUiPos(slib::UIResource::dpToPixel(16.0f)), drawable::round_shadow_back::get(), 32.0f, 32.0f, 32.0f, 32.0f), slib::UIUpdateMode::Init);

			container = new slib::LinearView;
			container->setWidthWrapping(slib::UIUpdateMode::Init);
			container->setHeightWrapping(slib::UIUpdateMode::Init);
			container->setAlignParentLeft(slib::UIUpdateMode::Init);
			container->setAlignParentTop(slib::UIUpdateMode::Init);
			m_contentView->addChild(container, slib::UIUpdateMode::Init);

		}

		void MenuPopup::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
