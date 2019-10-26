#include "MainWindow.h"

void MainWindow::onCreate()
{
	btnFacebookAPI->setOnClick([this](View*) {
		txtAuthUrl->setText("https://www.facebook.com/dialog/oauth");
		txtAccessTokenUrl->setText("https://graph.facebook.com/oauth/access_token");
		txtRedirectUri->setText("https://github.com/SLIBIO/SLib");
		txtClientId->setText("895581487493562");
		txtScopes->setText("public_profile,email");
		txtRequestURL->setText("https://graph.facebook.com/me?fields=id,name");
	});
	
	btnExampleServer->setOnClick([this](View*) {
		txtAuthUrl->setText("http://localhost:8080/login");
		txtAccessTokenUrl->setText("http://localhost:8080/access_token");
		txtRedirectUri->setText("redirect://");
		txtClientId->setText("test");
		txtScopes->setText("public,email");
		txtRequestURL->setText("http://localhost:8080/me");
	});
	
	btnAuth->setOnClick([this](View*) {
		OAuthParam param;
		param.authorizeUrl = txtAuthUrl->getText();
		param.accessTokenUrl = txtAccessTokenUrl->getText();
		param.clientId = txtClientId->getText();
		param.redirectUri = txtRedirectUri->getText();
		param.defaultScopes.addAll(txtScopes->getText().split(","));
		Ref<OAuth2> oauth = new OAuth2(param);
		auto ref = ToRef(this);
		OAuthLoginParam loginParam;
		loginParam.dialogOptions.width = 800;
		loginParam.dialogOptions.height = 600;
		loginParam.dialogOptions.title = "Authentication";
		loginParam.onComplete = [ref, this](OAuthLoginResult& result) {
			txtAccessToken->setText(result.accessToken.token);
			txtResponse->setText(result.response.toJsonString());
		};
		oauth->login(loginParam);
	});
	
	btnSendRequest->setOnClick([this](View*) {
		OAuthParam param;
		param.accessToken = txtAccessToken->getText();
		Ref<OAuth2> oauth = new OAuth2(param);

		UrlRequestParam rp;
		rp.url = txtRequestURL->getText();
		auto ref = ToRef(this);
		rp.onComplete = [ref, this](UrlRequest* request) {
			txtResponse->setText(String::format("%d %s\r\n%s", (int)(request->getResponseStatus()), HttpStatuses::toString(request->getResponseStatus()), request->getResponseContentAsString()));
		};
		oauth->authorizeRequest(rp);
		UrlRequest::send(rp);
	});
}
