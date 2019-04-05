namespace slib
{
	namespace ui
	{
		SLIB_DEFINE_PAGE_LAYOUT(MobileAlertDialog)

		void MobileAlertDialog::initialize()
		{
			this->setWidthWeight(0.8f, slib::UIUpdateMode::Init);
			this->setHeightWrapping(slib::UIUpdateMode::Init);
			this->setCenterHorizontal(slib::UIUpdateMode::Init);
			this->setCenterVertical(slib::UIUpdateMode::Init);
			this->setMaximumWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(500.0f)), slib::UIUpdateMode::Init);
			this->setClipping(sl_true, slib::UIUpdateMode::Init);
			this->setBoundRadius(slib::UIResource::dpToPixel(12.0f), slib::UIUpdateMode::Init);
			this->setCreatingInstance(sl_false);

			_linear1 = new slib::LinearView;
			_linear1->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			_linear1->setHeightWrapping(slib::UIUpdateMode::Init);
			_linear1->setAlignParentLeft(slib::UIUpdateMode::Init);
			_linear1->setAlignParentTop(slib::UIUpdateMode::Init);
			m_contentView->addChild(_linear1, slib::UIUpdateMode::Init);

			txtTitle = new slib::LabelView;
			txtTitle->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			txtTitle->setHeightWrapping(slib::UIUpdateMode::Init);
			txtTitle->setAlignParentLeft(slib::UIUpdateMode::Init);
			txtTitle->setMarginTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			txtTitle->setFont(slib::Font::create(txtTitle->getFontFamily(), slib::UIResource::dpToPixel(25.0f), sl_true, sl_false, sl_false), slib::UIUpdateMode::Init);
			txtTitle->setText("Title", slib::UIUpdateMode::Init);
			txtTitle->setGravity(slib::Alignment::MiddleCenter, slib::UIUpdateMode::Init);
			txtTitle->setMultiLine(slib::MultiLineMode::WordWrap, slib::UIUpdateMode::Init);
			_linear1->addChild(txtTitle, slib::UIUpdateMode::Init);

			_scroll1 = new slib::ScrollView;
			_scroll1->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			_scroll1->setHeightWrapping(slib::UIUpdateMode::Init);
			_scroll1->setAlignParentLeft(slib::UIUpdateMode::Init);
			_scroll1->setMaximumHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(160.0f)), slib::UIUpdateMode::Init);
			_linear1->addChild(_scroll1, slib::UIUpdateMode::Init);

			txtContent = new slib::LabelView;
			txtContent->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			txtContent->setHeightWrapping(slib::UIUpdateMode::Init);
			txtContent->setMinimumHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(80.0f)), slib::UIUpdateMode::Init);
			txtContent->setPaddingLeft(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			txtContent->setPaddingTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			txtContent->setPaddingRight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			txtContent->setPaddingBottom(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(10.0f)), slib::UIUpdateMode::Init);
			txtContent->setFont(slib::Font::create(txtContent->getFontFamily(), slib::UIResource::dpToPixel(20.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			txtContent->setText("Content", slib::UIUpdateMode::Init);
			txtContent->setGravity(slib::Alignment::MiddleCenter, slib::UIUpdateMode::Init);
			txtContent->setMultiLine(slib::MultiLineMode::WordWrap, slib::UIUpdateMode::Init);
			_scroll1->setContentView(txtContent, slib::UIUpdateMode::Init);

			_view1 = new slib::View;
			_view1->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			_view1->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view1->setAlignParentLeft(slib::UIUpdateMode::Init);
			_view1->setMarginTop(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view1->setBackground(slib::Drawable::createColorDrawable(slib::Color(176, 176, 176, 255)), slib::UIUpdateMode::Init);
			_linear1->addChild(_view1, slib::UIUpdateMode::Init);

			_linear2 = new slib::LinearView;
			_linear2->setOrientation(slib::LayoutOrientation::Horizontal, slib::UIUpdateMode::Init);
			_linear2->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			_linear2->setHeightWrapping(slib::UIUpdateMode::Init);
			_linear2->setAlignParentLeft(slib::UIUpdateMode::Init);
			_linear1->addChild(_linear2, slib::UIUpdateMode::Init);

			btnOK = new slib::Button;
			btnOK->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			btnOK->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnOK->setAlignParentTop(slib::UIUpdateMode::Init);
			btnOK->setPaddingLeft(0, slib::UIUpdateMode::Init);
			btnOK->setPaddingTop(0, slib::UIUpdateMode::Init);
			btnOK->setPaddingRight(0, slib::UIUpdateMode::Init);
			btnOK->setPaddingBottom(0, slib::UIUpdateMode::Init);
			btnOK->setFont(slib::Font::create(btnOK->getFontFamily(), slib::UIResource::dpToPixel(25.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			btnOK->setText(string::ok::get(), slib::UIUpdateMode::Init);
			_linear2->addChild(btnOK, slib::UIUpdateMode::Init);

			_view2 = new slib::View;
			_view2->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view2->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			_view2->setAlignParentRight(slib::UIUpdateMode::Init);
			_view2->setAlignParentBottom(slib::UIUpdateMode::Init);
			_view2->setBackground(slib::Drawable::createColorDrawable(slib::Color(176, 176, 176, 255)), slib::UIUpdateMode::Init);
			btnOK->addChild(_view2, slib::UIUpdateMode::Init);

			btnYes = new slib::Button;
			btnYes->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			btnYes->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnYes->setAlignParentTop(slib::UIUpdateMode::Init);
			btnYes->setPaddingLeft(0, slib::UIUpdateMode::Init);
			btnYes->setPaddingTop(0, slib::UIUpdateMode::Init);
			btnYes->setPaddingRight(0, slib::UIUpdateMode::Init);
			btnYes->setPaddingBottom(0, slib::UIUpdateMode::Init);
			btnYes->setFont(slib::Font::create(btnYes->getFontFamily(), slib::UIResource::dpToPixel(25.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			btnYes->setText(string::yes::get(), slib::UIUpdateMode::Init);
			_linear2->addChild(btnYes, slib::UIUpdateMode::Init);

			_view3 = new slib::View;
			_view3->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view3->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			_view3->setAlignParentRight(slib::UIUpdateMode::Init);
			_view3->setAlignParentBottom(slib::UIUpdateMode::Init);
			_view3->setBackground(slib::Drawable::createColorDrawable(slib::Color(176, 176, 176, 255)), slib::UIUpdateMode::Init);
			btnYes->addChild(_view3, slib::UIUpdateMode::Init);

			btnNo = new slib::Button;
			btnNo->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			btnNo->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnNo->setAlignParentTop(slib::UIUpdateMode::Init);
			btnNo->setPaddingLeft(0, slib::UIUpdateMode::Init);
			btnNo->setPaddingTop(0, slib::UIUpdateMode::Init);
			btnNo->setPaddingRight(0, slib::UIUpdateMode::Init);
			btnNo->setPaddingBottom(0, slib::UIUpdateMode::Init);
			btnNo->setFont(slib::Font::create(btnNo->getFontFamily(), slib::UIResource::dpToPixel(25.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			btnNo->setText(string::no::get(), slib::UIUpdateMode::Init);
			_linear2->addChild(btnNo, slib::UIUpdateMode::Init);

			_view4 = new slib::View;
			_view4->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view4->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			_view4->setAlignParentRight(slib::UIUpdateMode::Init);
			_view4->setAlignParentBottom(slib::UIUpdateMode::Init);
			_view4->setBackground(slib::Drawable::createColorDrawable(slib::Color(176, 176, 176, 255)), slib::UIUpdateMode::Init);
			btnNo->addChild(_view4, slib::UIUpdateMode::Init);

			btnCancel = new slib::Button;
			btnCancel->setWidthFilling(1.0f, slib::UIUpdateMode::Init);
			btnCancel->setHeight(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(40.0f)), slib::UIUpdateMode::Init);
			btnCancel->setAlignParentTop(slib::UIUpdateMode::Init);
			btnCancel->setPaddingLeft(0, slib::UIUpdateMode::Init);
			btnCancel->setPaddingTop(0, slib::UIUpdateMode::Init);
			btnCancel->setPaddingRight(0, slib::UIUpdateMode::Init);
			btnCancel->setPaddingBottom(0, slib::UIUpdateMode::Init);
			btnCancel->setFont(slib::Font::create(btnCancel->getFontFamily(), slib::UIResource::dpToPixel(25.0f), sl_false, sl_false, sl_false), slib::UIUpdateMode::Init);
			btnCancel->setText(string::cancel::get(), slib::UIUpdateMode::Init);
			_linear2->addChild(btnCancel, slib::UIUpdateMode::Init);

			_view5 = new slib::View;
			_view5->setWidth(slib::UIResource::toUiPos(slib::UIResource::dpToPixel(1.0f)), slib::UIUpdateMode::Init);
			_view5->setHeightFilling(1.0f, slib::UIUpdateMode::Init);
			_view5->setAlignParentRight(slib::UIUpdateMode::Init);
			_view5->setAlignParentBottom(slib::UIUpdateMode::Init);
			_view5->setBackground(slib::Drawable::createColorDrawable(slib::Color(176, 176, 176, 255)), slib::UIUpdateMode::Init);
			btnCancel->addChild(_view5, slib::UIUpdateMode::Init);

		}

		void MobileAlertDialog::layoutViews(sl_ui_len CONTENT_WIDTH, sl_ui_len CONTENT_HEIGHT)
		{
		}

	}
}
