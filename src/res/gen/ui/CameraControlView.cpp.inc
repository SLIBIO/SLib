namespace slib
{
	namespace ui
	{
		SLIB_DEFINE_VIEW_LAYOUT(CameraControlView)

		void CameraControlView::initialize()
		{
			this->setWidthWeight(1.0f, slib::UIUpdateMode::Init);
			this->setHeightWeight(1.0f, slib::UIUpdateMode::Init);
			this->setAlignParentLeft(slib::UIUpdateMode::Init);
			this->setAlignParentTop(slib::UIUpdateMode::Init);

			btnShutter = new slib::Button;
			btnShutter->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(100.0f)), slib::UIUpdateMode::Init);
			btnShutter->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(100.0f)), slib::UIUpdateMode::Init);
			btnShutter->setCenterHorizontal(slib::UIUpdateMode::Init);
			btnShutter->setAlignParentBottom(slib::UIUpdateMode::Init);
			btnShutter->setMarginBottom(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnShutter->setClipping(sl_true, slib::UIUpdateMode::Init);
			btnShutter->setBackground(slib::Drawable::createColorDrawable(slib::Color(224, 224, 224, 255)), slib::UIUpdateMode::Init);
			btnShutter->setBoundShape(slib::BoundShape::Ellipse, slib::UIUpdateMode::Init);
			btnShutter->setCreatingNativeWidget(sl_false);
			m_contentView->addChild(btnShutter, slib::UIUpdateMode::Init);

			btnSwitch = new slib::Button;
			btnSwitch->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnSwitch->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnSwitch->setAlignParentRight(slib::UIUpdateMode::Init);
			btnSwitch->setAlignParentTop(slib::UIUpdateMode::Init);
			btnSwitch->setMarginTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(30.0f)), slib::UIUpdateMode::Init);
			btnSwitch->setMarginRight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(30.0f)), slib::UIUpdateMode::Init);
			btnSwitch->setIcon(drawable::camera_view_control_switch::get(), slib::UIUpdateMode::Init);
			btnSwitch->setCreatingNativeWidget(sl_false);
			m_contentView->addChild(btnSwitch, slib::UIUpdateMode::Init);

			btnFlash = new slib::Button;
			btnFlash->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnFlash->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnFlash->setAlignParentLeft(slib::UIUpdateMode::Init);
			btnFlash->setAlignParentTop(slib::UIUpdateMode::Init);
			btnFlash->setMarginLeft(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(30.0f)), slib::UIUpdateMode::Init);
			btnFlash->setMarginTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(30.0f)), slib::UIUpdateMode::Init);
			btnFlash->setIcon(drawable::camera_view_control_flash_auto::get(), slib::UIUpdateMode::Init);
			btnFlash->setCreatingNativeWidget(sl_false);
			m_contentView->addChild(btnFlash, slib::UIUpdateMode::Init);

		}

		void CameraControlView::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
