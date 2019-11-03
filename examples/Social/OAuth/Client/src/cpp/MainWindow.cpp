#include "MainWindow.h"

void MainWindow::onCreate()
{

	radioGrantType->setOnSelect([this](RadioGroup*, RadioButton* button) {
		String value = button->getValue();
		if (value == "password") {
			txtUsername->setEnabled(sl_true);
			txtPassword->setEnabled(sl_true);
		} else {
			txtUsername->setEnabled(sl_false);
			txtPassword->setEnabled(sl_false);
		}
	});
	
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
		txtClientSecret->setText("test-secret");
		txtScopes->setText("public,email");
		txtRequestURL->setText("http://localhost:8080/me");
	});
	
	btnAuth->setOnClick([this](View*) {
		String grantType = radioGrantType->getSelectedValue();
		
		OAuthParam param;
		param.authorizeUrl = txtAuthUrl->getText();
		param.accessTokenUrl = txtAccessTokenUrl->getText();
		param.clientId = txtClientId->getText();
		if (grantType != "implicit") {
			param.clientSecret = txtClientSecret->getText();
		}
		param.redirectUri = txtRedirectUri->getText();
		List<String> scopes = txtScopes->getText().split(",");
		Ref<OAuth2> oauth = new OAuth2(param);
		
		if (grantType == "implicit" || grantType == "code") {
			OAuthLoginParam loginParam;
			loginParam.authorization.responseType = grantType == "code" ? OAuthResponseType::Code : OAuthResponseType::Token;
			loginParam.authorization.codeVerifier = txtVerifier->getText();
			loginParam.authorization.scopes = scopes;
			loginParam.dialogOptions.width = 800;
			loginParam.dialogOptions.height = 600;
			loginParam.dialogOptions.title = "Authentication";
			auto ref = ToRef(this);
			loginParam.onComplete = [ref, this, grantType](OAuthLoginResult& result) {
				if (grantType == "code") {
					txtAuthCode->setText(result.code);
					txtAccessToken->setText(String::null());
					txtRefreshToken->setText(String::null());
				} else {
					txtAccessToken->setText(result.accessToken.token);
					txtRefreshToken->setText(result.accessToken.refreshToken);
				}
				txtResponse->setText(result.response.toJsonString());
			};
			oauth->login(loginParam);
		} else if (grantType == "client") {
			auto ref = ToRef(this);
			oauth->requestAccessTokenFromClientCredentials(scopes, [ref, this](OAuthAccessTokenResult& result) {
				txtAccessToken->setText(result.accessToken.token);
				txtRefreshToken->setText(result.accessToken.refreshToken);
				txtResponse->setText(result.response.toJsonString());
			});
		} else if (grantType == "password") {
			String username = txtUsername->getText();
			if (username.isEmpty()) {
				UI::alert("Input the username");
				return;
			}
			String password = txtPassword->getText();
			if (password.isEmpty()) {
				UI::alert("Input the password");
				return;
			}
			auto ref = ToRef(this);
			oauth->requestAccessTokenFromUserPassword(username, password, scopes, [ref, this](OAuthAccessTokenResult& result) {
				txtAccessToken->setText(result.accessToken.token);
				txtRefreshToken->setText(result.accessToken.refreshToken);
				txtResponse->setText(result.response.toJsonString());
			});
		}
	});
	
	btnGetToken->setOnClick([this](View*) {
		OAuthParam param;
		param.accessTokenUrl = txtAccessTokenUrl->getText();
		param.clientId = txtClientId->getText();
		param.clientSecret = txtClientSecret->getText();
		String code = txtAuthCode->getText();
		if (code.isEmpty()) {
			UI::alert("Input or authorize the authorization code");
			return;
		}
		String redirectUri = txtRedirectUri->getText();
		String codeVerifier = txtVerifier->getText();
		List<String> scopes = txtScopes->getText().split(",");
		Ref<OAuth2> oauth = new OAuth2(param);
		auto ref = ToRef(this);
		oauth->requestAccessTokenFromCode(code, redirectUri, codeVerifier, scopes, [ref, this](OAuthAccessTokenResult& result) {
			txtAccessToken->setText(result.accessToken.token);
			txtRefreshToken->setText(result.accessToken.refreshToken);
			txtResponse->setText(result.response.toJsonString());
		});
	});
	
	btnRefresh->setOnClick([this](View*) {
		OAuthParam param;
		param.accessTokenUrl = txtAccessTokenUrl->getText();
		param.clientId = txtClientId->getText();
		param.clientSecret = txtClientSecret->getText();
		String refreshToken = txtRefreshToken->getText();
		if (refreshToken.isEmpty()) {
			UI::alert("Input or get the refresh token");
			return;
		}
		List<String> scopes = txtScopes->getText().split(",");
		Ref<OAuth2> oauth = new OAuth2(param);
		auto ref = ToRef(this);
		oauth->refreshAccessToken(refreshToken, scopes, [ref, this](OAuthAccessTokenResult& result) {
			txtAccessToken->setText(result.accessToken.token);
			txtResponse->setText(result.response.toJsonString());
		});
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
