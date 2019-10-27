#include <slib.h>

using namespace slib;

OAuthServer oauth;

struct MyUser
{
	String userId;
	String clientId;
	List<String> scopes;
	
	SLIB_JSON_MEMBERS(userId, clientId, scopes)
};

Memory oauth_secret = String("oauth example").toMemory();

void IssueToken(OAuthAccessToken& ret, OAuthClientEntity* client, const OAuthUserEntity& entity, const List<String>& scopes) {
	MyUser user;
	user.userId = entity.userId;
	user.clientId = client->clientId;
	user.scopes = scopes;
	ret.expirationTime = Time::now() + Time::withHours(12);
	Jwt jwt;
	jwt.payload = user.toJson();
	jwt.setExpirationTime(ret.expirationTime);
	ret.token = jwt.encode(oauth_secret);
}

sl_bool ValidateToken(HttpServerContext* context, MyUser& user)
{
	String token = OAuthServer::getAccessToken(context);
	if (token.isNotEmpty()) {
		Jwt jwt;
		if (jwt.decode(oauth_secret, token)) {
			user.fromJson(jwt.payload);
			if (user.userId.isNotEmpty()) {
				return sl_true;
			}
		}
	}
	return sl_false;
}

int main(int argc, const char * argv[])
{
	oauth.setOnIssueAccessToken(&IssueToken);
	
	HttpServerParam param;
	param.port = 8080;
	
	param.router.GET("/", [](HttpServerContext* context) {
		return "Welcome to OAuth 2.0 Test Server";
	});
	param.router.GET("/login", [](HttpServerContext* context) {
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
							</form>
						</center>
					</body>
				</html>
			), context->getParameters()
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
			OAuthUserEntity user;
			user.userId = loginId;
			oauth.completeAuthorizationRequest(context, request, user);
		}
		return String::null();
	});
	param.router.GET("/me", [](HttpServerContext* context) {
		MyUser user;
		if (ValidateToken(context, user)) {
			return user.toJson();
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
