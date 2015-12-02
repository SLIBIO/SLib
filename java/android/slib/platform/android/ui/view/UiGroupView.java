package slib.platform.android.ui.view;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.FrameLayout;

public class UiGroupView extends FrameLayout {
	
	public UiGroupView(Context context) {
		super(context);
		setWillNotDraw(false);
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
