#include <slib.h>

using namespace slib;

class MyAuth : public OAuthServerWithJwtAndOpenSSL
{
public:
	MyAuth()
	{
		// setMasterKey("1234", 4); // HS256
		
		setAlgorithm(JwtAlgorithm::ES256);
		setPrivateKey(R"(-----BEGIN PRIVATE KEY-----
MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgevZzL1gdAFr88hb2
OF/2NxApJCzGCEDdfSp6VQO30hyhRANCAAQRWz+jn65BtOMvdyHKcvjBeBSDZH2r
1RTwjmYSi9R/zpBnuQ4EiMnCqfMPWiZqB4QdbAd0E7oH50VpuZ1P087G
-----END PRIVATE KEY-----)");
		setPublicKey(R"(-----BEGIN PUBLIC KEY-----
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEEVs/o5+uQbTjL3chynL4wXgUg2R9
q9UU8I5mEovUf86QZ7kOBIjJwqnzD1omageEHWwHdBO6B+dFabmdT9POxg==
-----END PUBLIC KEY-----)");
	}
	
public:
	sl_bool onClientCredentialsGrant(OAuthTokenPayload& payload) override
	{
		payload.user.putItem("type", "credential");
		return sl_true;
	}

	sl_bool onPasswordGrant(const String& username, const String& password, OAuthTokenPayload& payload) override
	{
		payload.user.putItem("type", "password");
		payload.user.putItem("username", username);
		return sl_true;
	}
	
} oauth;

int main(int argc, const char * argv[])
{	
	HttpServerParam param;
	param.port = 8080;
	
	param.router.GET("/", [](HttpServerContext* context) {
		return "Welcome to OAuth 2.0 Test Server";
	});
	param.router.POST("/access_token", [](HttpServerContext* context) {
		oauth.respondToAccessTokenRequest(context);
		return sl_true;
	});
	param.router.GET("/login", [](HttpServerContext* context) {
		auto params = context->getParameters();
		params.put("code_challenge", params.getValue("code_challenge", ""));
		params.put("code_challenge_method", params.getValue("code_challenge_method", ""));
		return Ginger::render(
			SLIB_STRINGIFY(
				<html>
					<body>
						<center>
							<form action='/login' method='post'>
								<table>
									<tr>
										<td>Login ID</td>
										<td><input type='text' name='login_id' style='width:200px'/></td>
									</tr>
									<tr>
										<td>Password</td>
										<td><input type='password' name='password' style='width:200px'/></td>
									</tr>
									<tr>
										<td align='center' colspan='2'>
											<input type='submit' value='&nbsp;Login&nbsp;' />
											&nbsp;&nbsp;
											<input type='button' value='&nbsp;Cancel&nbsp;' onclick="javascript:document.location='${redirect_uri}?error=access_denied&state=${state}'" />
										</td>
									</tr>
								</table>
								<input type='hidden' name='response_type' value='${response_type}' />
								<input type='hidden' name='client_id' value='${client_id}' />
								<input type='hidden' name='redirect_uri' value='${redirect_uri}' />
								<input type='hidden' name='scope' value='${scope}' />
								<input type='hidden' name='state' value='${state}' />
								<input type='hidden' name='code_challenge' value='${code_challenge}' />
								<input type='hidden' name='code_challenge_method' value='${code_challenge_method}' />
							</form>
						</center>
					</body>
				</html>
			), params
		);
	});
	param.router.POST("/login", [](HttpServerContext* context) {
		OAuthServerAuthorizationRequest request;
		if (oauth.validateAuthorizationRequest(context, request)) {
			String loginId = context->getParameter("login_id");
			String password = context->getParameter("password");
			if (loginId.isEmpty() || password.isEmpty()) {
				return String::format(SLIB_STRINGIFY(<html><body><a href='/login?%s'>Invalid Login. Go Back.</a></body></html>), HttpRequest::buildQuery(context->getParameters()));
			}
			Json user;
			user.putItem("type", "login");
			user.putItem("loginId", loginId);
			oauth.completeAuthorizationRequest(context, request, user);
		}
		return String::null();
	});
	param.router.GET("/me", [](HttpServerContext* context) {
		OAuthTokenPayload data;
		if (oauth.validateAccessToken(context, data)) {
			Json ret;
			ret.putItem("user", data.user);
			ret.putItem("exp", data.accessTokenExpirationTime);
			ret.putItem("scope", data.scopes);
			return ret;
		}
		return Json("Invalid Authorization");
	});
	
	Ref<HttpServer> server = HttpServer::create(param);
	if (server.isNull()) {
		return -1;
	}
	
	Console::println("Server is running on port: %d", param.port);
	
	for(;;) {
		Console::println("\nPress x to exit!!!");
		if (Console::readChar() == 'x') {
			break;
		}
	}
	
	return 0;
}
