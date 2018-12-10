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

SLIB_DEFINE_APPLICATION(ExampleWebViewApp, MobileApp)

ExampleWebViewApp::ExampleWebViewApp()
{
}

void ExampleWebViewApp::onStart()
{
#ifdef SLIB_PLATFORM_IS_DESKTOP
	sl_real fontSize = 20;
#else
	sl_real fontSize = UI::getScreenWidth() / 25;
#endif
	
	getMainWindow()->setResizable(sl_true);
	
	m_webView = new WebView;
	m_webView->setWidthFilling();
	m_webView->setHeightFilling();

	m_webView->loadURL("https://en.wikipedia.org");

	Ref<LinearView> linear = new LinearView;
	linear->setBackgroundColor(Color::White);
	linear->setWidthFilling();
	linear->setHeightFilling();
	linear->addChild(m_webView);
	
	Ref<LinearView> linear2 = new HorizontalLinearView;
	linear2->setPadding((sl_ui_pos)fontSize/2);
	linear2->setWidthWrapping();
	linear2->setHeightWrapping();
	linear2->setCenterHorizontal();
	linear->addChild(linear2);

	Ref<Button> button1 = new Button;
	button1->setWidthWrapping();
	button1->setHeightWrapping();
	button1->setAlignParentTop();
	button1->setFontSize(fontSize);
	button1->setBackgroundColor(Color::LightGray);
	button1->setText("Start Test");
	button1->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleWebViewApp, onClickTest1, this));
	linear2->addChild(button1);
	
	Ref<Button> button2 = new Button;
	button2->setMarginLeft((sl_ui_pos)fontSize);
	button2->setWidthWrapping();
	button2->setHeightWrapping();
	button2->setAlignParentTop();
	button2->setFontSize(fontSize);
	button2->setBackgroundColor(Color::LightGray);
	button2->setText("Call Script");
	button2->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleWebViewApp, onClickTest2, this));
	linear2->addChild(button2);
	
	Ref<Button> button3 = new Button;
	button3->setMarginLeft((sl_ui_pos)fontSize);
	button3->setWidthWrapping();
	button3->setHeightWrapping();
	button3->setAlignParentTop();
	button3->setFontSize(fontSize);
	button3->setBackgroundColor(Color::LightGray);
	button3->setText("Local Server");
	button3->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleWebViewApp, onClickTest3, this));
	linear2->addChild(button3);
	
	addViewToContent(linear);
	
}

void ExampleWebViewApp::onClickTest1(View* button)
{
	UI::grantCameraPermission();
	UI::grantWriteExternalStoragePermission();

	String html = SLIB_STRINGIFY
	(
		<html>
			<body>
	 			Test for Javascript Interface
	 			<BR>
	 			<div id='result' style='color:red'></div>
	 			<BR>
				<a href='javascript:call_cpp()'>Call C++ Function</a>
	 			<BR>
	 			<input type="file" accept="image/*">
	 		</body>
	 		<script>
	 			function test_func(p1, p2)
	 			{
					document.getElementById('result').innerHTML = 'Call from C++: Paramters=' + p1 + ', ' + p2;
				}
	 
	 			window.call_cpp_count = 0;
	 			function call_cpp()
	 			{
					window.call_cpp_count++;
					slib.send('MyMessage1', window.call_cpp_count);
				}
	 		</script>
		</html>
	);
	m_webView->loadHTML(html, "http://mytest.org");
	m_webView->setOnMessageFromJavaScript([](WebView*, String message, String param) {
		UI::showAlert(String::format("Message from JavaScript: %s, %s", message, param));
	});
}

void ExampleWebViewApp::onClickTest2(View* button)
{
	static int n = 0;
	n++;
	m_webView->runJavaScript(String::format("test_func(%d, '%s')", n, Time::now()));
}

void ExampleWebViewApp::onClickTest3(View *button)
{
	if (m_http.isNull()) {
		HttpServiceParam param;
		param.flagUseAsset = sl_true;
		param.prefixAsset = "web/";
		for (int port = 50000; port < 50100; port++) {
			param.port = port;
			m_http = HttpService::create(param);
			if (m_http.isNotNull()) {
				break;
			}
		}
	}
	m_webView->loadURL(String::format("http://127.0.0.1:%d/index.html", m_http->getParam().port));
}
