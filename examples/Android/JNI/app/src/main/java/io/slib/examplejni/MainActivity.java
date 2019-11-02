package io.slib.examplejni;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

	// Used to load the 'native-lib' library on application startup.
	static {
		System.loadLibrary("native-lib");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// Example of a call to a native method
		TextView tv = (TextView) findViewById(R.id.sample_text);
		tv.setText(callCpp("Hello!", 5));
	}

	/**
	 * A native method that is implemented by the 'native-lib' native library,
	 * which is packaged with this application.
	 */
	public native String callCpp(String message, int nSeconds);

	public String callJava(final String message, final int n) {
		final TextView tv = (TextView) findViewById(R.id.sample_text);
		tv.post(new Runnable() {
			@Override
			public void run() {
				tv.setText(message + ", 2nd Param: " + n);
			}
		});
		return "OK";
	}

}
