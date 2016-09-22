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
		UiView.onEventKey(this, true, keycode, event);
		return true;
	}
	
	public boolean onKeyUp(int keycode, KeyEvent event) {
		UiView.onEventKey(this, false, keycode, event);
		return true;
	}
	
	@SuppressLint("ClickableViewAccessibility")
	public boolean onTouchEvent(MotionEvent event) {		
		UiView.onEventTouch(this, event);
		return true;
	}
	
}
