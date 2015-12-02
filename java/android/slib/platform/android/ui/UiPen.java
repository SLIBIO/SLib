package slib.platform.android.ui;

import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.PathEffect;
import android.graphics.Paint.Cap;
import android.graphics.Paint.Join;

public class UiPen {

	public int style;
	public static final int STYLE_SOLID = 0;
	public static final int STYLE_DOT = 1;
	public static final int STYLE_DASH = 2;
	public static final int STYLE_DASHDOT = 3;
	public static final int STYLE_DASHDOTDOT = 4;
	
	public int cap;
	public static final int CAP_FLAT = 0;
	public static final int CAP_ROUND = 1;
	public static final int CAP_SQUARE = 2;
	
	public int join;
	public static final int JOIN_MITER = 0;
	public static final int JOIN_ROUND = 1;
	public static final int JOIN_BEVEL = 2;
	
	public float width;
	public int color;
	public float miterLimit;
	
	public PathEffect pathEffect;
	
	public void setStyle(int style)
	{
		this.style = style;
		pathEffect = null;
		switch (style) {
		case STYLE_SOLID:
			break;
		case STYLE_DOT:
			pathEffect = new DashPathEffect(new float[] {width, width*2}, 0);
			break;
		case STYLE_DASH:
			pathEffect = new DashPathEffect(new float[] {width * 3, width * 3}, 0);
			break;
		case STYLE_DASHDOT:
			pathEffect = new DashPathEffect(new float[] {width * 3, width * 3, width * 1, width * 3}, 0);
			break;
		case STYLE_DASHDOTDOT:
			pathEffect = new DashPathEffect(new float[] {width * 3, width * 3, width * 1, width * 2, width * 1, width * 3}, 0);
			break;
		}
	}
	
	public void applyPaint(Paint paint) {
		
		paint.setPathEffect(pathEffect);
		
		switch (cap) {
		case UiPen.CAP_FLAT:
			paint.setStrokeCap(Cap.BUTT);
			break;
		case UiPen.CAP_ROUND:
			paint.setStrokeCap(Cap.ROUND);
			break;
		case UiPen.CAP_SQUARE:
			paint.setStrokeCap(Cap.SQUARE);
			break;
		}
		
		switch (join) {
		case UiPen.JOIN_MITER:
			paint.setStrokeJoin(Join.MITER);
			paint.setStrokeMiter(miterLimit);
			break;
		case UiPen.JOIN_ROUND:
			paint.setStrokeJoin(Join.ROUND);
			break;
		case UiPen.JOIN_BEVEL:
			paint.setStrokeJoin(Join.BEVEL);
			break;
		}
		
		paint.setStrokeWidth(width);
		paint.setColor(color);
		paint.setStyle(Style.STROKE);
	
	}
}
