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
	getMainWindow()->setResizable(sl_true);
	
	m_webView = new WebView;
	m_webView->setWidthFilling();
	m_webView->setHeightFilling();
	m_webView->loadURL("http://mozilla.org/MPL/2.0/");

	Ref<LinearView> linear = new LinearView;
	linear->setBackgroundColor(Color::LightGray);
	linear->setWidthFilling();
	linear->setHeightFilling();
	linear->addChild(m_webView);
	
	Ref<LinearView> linear2 = new HorizontalLinearView;
	linear2->setWidthWrapping();
	linear2->setHeightWrapping();
	linear2->setCenterHorizontal();
	linear->addChild(linear2);

	Ref<Button> button1 = new Button;
	button1->setWidth(130);
	button1->setHeight(40);
	button1->setText("Start Test");
	button1->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleWebViewApp, onClickTest1, this));
	linear2->addChild(button1);
	
	Ref<Button> button2 = new Button;
	button2->setMarginLeft(30);
	button2->setWidth(130);
	button2->setHeight(40);
	button2->setText("Call Javascript");
	button2->setOnClick(SLIB_FUNCTION_WEAKREF(ExampleWebViewApp, onClickTest2, this));
	linear2->addChild(button2);
	
	addViewToContent(linear);
}

void ExampleWebViewApp::onClickTest1(View* button)
{
	String html = SLIB_STRINGIFY
	(
		<html>
			<body>
	 			Test for Javascript Interface
	 			<BR>
	 			<div id='result' style='color:red'></div>
	 			<BR>
				<a href='javascript:call_cpp()'>Call C++ Function</a>
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
					slib_send('MyMessage1', window.call_cpp_count);
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
