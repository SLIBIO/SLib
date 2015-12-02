package slib.platform.android.ui;

import android.graphics.Paint;
import android.graphics.Paint.Style;

public class UiBrush {

	public int style;
	public static final int STYLE_SOLID = 0;
	
	public int color;
	
	public void applyPaint(Paint paint) {
		paint.setColor(color);
		paint.setStyle(Style.FILL);
	}
}
