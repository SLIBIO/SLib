package slib.platform.android.wechat;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import slib.platform.android.Logger;

public class WeChat {

	private static boolean mFlagInitialized = false;
	private static IWXAPI mApi;
	private static IWXAPIEventHandler mListener;

	public static void initialize(final Activity activity, final String appId) {

		if (mFlagInitialized) {
			return;
		}

		if (activity == null) {
			return;
		}

		try {

			mApi = WXAPIFactory.createWXAPI(activity, appId, true);

			mApi.registerApp(appId);

			mListener = new IWXAPIEventHandler() {
				@Override
				public void onReq(BaseReq baseReq) {
				}

				@Override
				public void onResp(BaseResp baseResp) {
					onResponse(baseResp);
				}
			};

			activity.registerReceiver(new BroadcastReceiver() {
				@Override
				public void onReceive(Context context, Intent intent) {
					mApi.registerApp(appId);
				}
			}, new IntentFilter(ConstantsAPI.ACTION_REFRESH_WXAPP));

			mFlagInitialized = true;

		} catch (Exception e) {
			Logger.exception(e);
		}

	}

	public static void login() {
		try {
			SendAuth.Req req = new SendAuth.Req();
			req.scope = "snsapi_userinfo";
			req.state = String.valueOf(System.currentTimeMillis());
			mApi.sendReq(req);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void handleResponseIntent(Intent intent) {
		try {
			mApi.handleIntent(intent, mListener);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static native void nativeOnLoginResult(boolean flagSuccess, boolean flagCancel, String code, String errStr);

	private static void onResponse(BaseResp resp) {
		if (resp instanceof SendAuth.Resp) {
			SendAuth.Resp response = (SendAuth.Resp)resp;
			boolean flagSuccess = false;
			boolean flagCancel = false;
			switch(response.errCode){
				case BaseResp.ErrCode.ERR_OK:
					flagSuccess = true;
					break;
				case BaseResp.ErrCode.ERR_USER_CANCEL:
					flagCancel = true;
				default:
					break;
			}
			nativeOnLoginResult(flagSuccess, flagCancel, response.code, response.errStr);
		}
	}

}
