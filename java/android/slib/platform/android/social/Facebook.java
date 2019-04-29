package slib.platform.android.social;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;

import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareHashtag;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;

public class Facebook {

	static CallbackManager callbackManager;
	static LoginManager loginManager;

	public static void initialize() {
		try {
			callbackManager = CallbackManager.Factory.create();
			SlibActivity.addActivityResultListener(new SlibActivity.ActivityResultListener() {
				@Override
				public void onActivityResult(Activity activity, int requestCode, int resultCode, Intent data) {
					callbackManager.onActivityResult(requestCode, resultCode, data);
				}
			});

			loginManager = LoginManager.getInstance();
			loginManager.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
				@Override
				public void onSuccess(LoginResult loginResult) {
					nativeOnLoginResult(getToken(loginResult.getAccessToken()), false);
				}

				@Override
				public void onCancel() {
					nativeOnLoginResult(null, true);
				}

				@Override
				public void onError(FacebookException e) {
					Logger.exception(e);
					nativeOnLoginResult(null, false);
				}
			});
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static FacebookToken getToken(AccessToken accessToken) {
		try {
			if (accessToken != null) {
				FacebookToken token = new FacebookToken();
				token.token = accessToken.getToken();
				StringBuilder sb = new StringBuilder();
				for (String permission : accessToken.getPermissions()) {
					if (sb.length() > 0) {
						sb.append(",");
					}
					sb.append(permission);
				}
				token.scopes = sb.toString();
				token.expirationTime = accessToken.getExpires().getTime();
				token.refreshTime = accessToken.getLastRefresh().getTime();
				return token;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static FacebookToken getCurrentToken() {
		try {
			AccessToken accessToken = AccessToken.getCurrentAccessToken();
			if (accessToken != null) {
				return getToken(accessToken);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void login(Activity activity, boolean flagPublish, String scopes) {
		try {
			List<String> permissions;
			if (scopes == null || scopes.isEmpty()) {
				permissions = new ArrayList<String>();
				permissions.add("public_profile");
				permissions.add("email");
			} else {
				permissions = new ArrayList<String>();
				Collections.addAll(permissions, scopes.split(","));
			}
			if (flagPublish) {
				loginManager.logInWithPublishPermissions(activity, permissions);
			} else {
				loginManager.logInWithReadPermissions(activity, permissions);
			}
		} catch (Exception e) {
			Logger.exception(e);
			nativeOnLoginResult(null, false);
		}
	}

	private static native void nativeOnLoginResult(FacebookToken token, boolean flagCancel);

	public static void share(Activity activity, String url, String quote, String hashtag) {
		try {
			ShareLinkContent.Builder contentBuilder = new ShareLinkContent.Builder();
			contentBuilder.setContentUrl(Uri.parse(url));
			contentBuilder.setQuote(quote);
			contentBuilder.setShareHashtag(new ShareHashtag.Builder().setHashtag(hashtag).build());
			ShareLinkContent content = contentBuilder.build();

			ShareDialog dialog = new ShareDialog(activity);
			dialog.registerCallback(callbackManager, new FacebookCallback<Sharer.Result>() {
				@Override
				public void onSuccess(Sharer.Result result) {
					nativeOnShareResult(true, false);
				}

				@Override
				public void onCancel() {
					nativeOnShareResult(false, true);
				}

				@Override
				public void onError(FacebookException e) {
					Logger.exception(e);
					nativeOnShareResult(false, false);
				}
			});
			dialog.show(content);
		} catch (Exception e) {
			Logger.exception(e);
			nativeOnShareResult(false, false);
		}
	}

	private static native void nativeOnShareResult(boolean flagSuccess, boolean flagCancel);

	public static void clearAccessToken() {
		try {
			loginManager.logOut();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

}
