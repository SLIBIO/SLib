package slib.platform.android.wechat;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class WeChatResponseActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		WeChat.handleResponseIntent(getIntent());
		finish();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		WeChat.handleResponseIntent(data);
		finish();
	}

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
		WeChat.handleResponseIntent(intent);
		finish();
	}

}
