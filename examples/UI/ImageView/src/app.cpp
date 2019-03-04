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

#include "app.h"

SLIB_DEFINE_APPLICATION(ExampleImageViewApp, MobileApp)

ExampleImageViewApp::ExampleImageViewApp()
{
}

void ExampleImageViewApp::onStart()
{
	getMainWindow()->setResizable(sl_true);
	getMainWindow()->setTitle("Example - ImageView");
	
	sl_real fontSize;
#if defined(SLIB_PLATFORM_IS_DESKTOP)
	fontSize = UIResource::dpToPixel(25);
#else
	fontSize = UIResource::dpToPixel(18);
	getContentView()->setBackgroundColor(Color::White);
#endif
	
	Ref<LinearView> linear1 = new LinearView;
	linear1->setWidthFilling();
	linear1->setHeightFilling();
	
	Ref<ImageView> image = new ImageView;
	image->setWidthFilling();
	image->setHeightFilling();
	image->setScaleMode(ScaleMode::Contain);
	image->setSource(Image::loadFromAsset("images/slib.png"));
	linear1->addChild(image);
	
	Ref<HorizontalLinearView> linear2 = new HorizontalLinearView;
	linear2->setWidthWrapping();
	linear2->setHeightWrapping();
	linear2->setCenterHorizontal();
	linear2->setMarginTop((sl_ui_len)fontSize / 2);
	{
		Ref<Button> button = new Button;
		button->setText("Rotate90");
		button->setWidthWrapping();
		button->setHeightWrapping();
		button->setFontSize(fontSize);
		button->setOnClick([image](View*) {
			image->setSource(image->getSource()->rotate(RotationMode::Rotate90));
		});
		linear2->addChild(button);
	}
	{
		Ref<Button> button = new Button;
		button->setText("Rotate180");
		button->setWidthWrapping();
		button->setHeightWrapping();
		button->setFontSize(fontSize);
		button->setOnClick([image](View*) {
			image->setSource(image->getSource()->rotate(RotationMode::Rotate180));
		});
		button->setMarginLeft((sl_ui_len)fontSize);
		linear2->addChild(button);
	}
	{
		Ref<Button> button = new Button;
		button->setText("Rotate270");
		button->setWidthWrapping();
		button->setHeightWrapping();
		button->setFontSize(fontSize);
		button->setOnClick([image](View*) {
			image->setSource(image->getSource()->rotate(RotationMode::Rotate270));
		});
		button->setMarginLeft((sl_ui_len)fontSize);
		linear2->addChild(button);
	}
	linear1->addChild(linear2);

	Ref<HorizontalLinearView> linear3 = new HorizontalLinearView;
	linear3->setWidthWrapping();
	linear3->setHeightWrapping();
	linear3->setCenterHorizontal();
	linear3->setMargin((sl_ui_len)fontSize / 2);
	{
		Ref<Button> button = new Button;
		button->setText("Flip Horizontal");
		button->setWidthWrapping();
		button->setHeightWrapping();
		button->setFontSize(fontSize);
		button->setOnClick([image](View*) {
			image->setSource(image->getSource()->flip(FlipMode::Horizontal));
		});
		linear3->addChild(button);
	}
	{
		Ref<Button> button = new Button;
		button->setText("Flip Vertical");
		button->setWidthWrapping();
		button->setHeightWrapping();
		button->setFontSize(fontSize);
		button->setOnClick([image](View*) {
			image->setSource(image->getSource()->flip(FlipMode::Vertical));
		});
		button->setMarginLeft((sl_ui_len)fontSize);
		linear3->addChild(button);
	}
	linear1->addChild(linear3);
	
	addViewToContent(linear1);
}
