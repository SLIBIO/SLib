package slib.platform.android.ui.view;

public class UiTouchPoint {
	float x;
	float y;
	float pressure;
	int phase; // 0-Move, 1-Begin, 2-End, 3-Cancel
}
