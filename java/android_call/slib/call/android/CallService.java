package slib.call.android;

import android.annotation.TargetApi;
import android.os.Build;
import android.telecom.Call;
import android.telecom.InCallService;

@TargetApi(Build.VERSION_CODES.M)
public class CallService extends InCallService {

	@Override
	public void onCallAdded(Call call) {
		super.onCallAdded(call);

	}

	@Override
	public void onCallRemoved(Call call) {
		super.onCallRemoved(call);
	}

}