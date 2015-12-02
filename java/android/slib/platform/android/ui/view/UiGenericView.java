package slib.platform.android.ui.view;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

public class UiGenericView extends View {
	
	public UiGenericView(Context context) {
		super(context);
	}

	public void onDraw(Canvas canvas) {
		UiView.onEventDraw(this, canvas);
	}
	
	public boolean onKeyDown(int keycode, KeyEvent event) {
		if (UiView.onEventKey(this, true, keycode, event)) {
			return super.onKeyDown(keycode, event);
		}
		return false;
	}
	
	public boolean onKeyUp(int keycode, KeyEvent event) {
		if (UiView.onEventKey(this, false, keycode, event)) {
			return super.onKeyUp(keycode, event);
		}
		return false;
	}
	
	@SuppressLint("ClickableViewAccessibility")
	public boolean onTouchEvent(MotionEvent event) {		
		if (UiView.onEventTouch(this, event)) {
			return true;
		}
		return false;
	}
	
}
