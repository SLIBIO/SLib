package io.slib.examplecameraview;

import android.os.Bundle;

import slib.platform.android.SlibActivity;

public class MainActivity extends SlibActivity {

	// Used to load the 'native-lib' library on application startup.
	static {
		System.loadLibrary("native-lib");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

}
