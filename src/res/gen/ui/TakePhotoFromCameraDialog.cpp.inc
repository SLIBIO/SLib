namespace slib
{
	namespace ui
	{
		SLIB_DEFINE_WINDOW_LAYOUT(TakePhotoFromCameraDialog)

		void TakePhotoFromCameraDialog::initialize()
		{
			this->setTitle(string::take_photo::get());
			this->setClientWidth(500);
			this->setClientHeight(500);
			this->setCenterScreenOnCreate(sl_true);
			m_contentView->setBackground(slib::Drawable::createColorDrawable(slib::Color(0, 0, 0, 255)), slib::UIUpdateMode::Init);

		}

		void TakePhotoFromCameraDialog::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
