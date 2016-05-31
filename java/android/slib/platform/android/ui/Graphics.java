package slib.platform.android.ui;

import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.Rect;

public class Graphics {

	public Canvas canvas;
	public boolean flagAntiAlias;
	
	public Graphics(Canvas canvas) {
		this.canvas = canvas;
		this.flagAntiAlias = true;
	}
	
	public int getWidth() {
		return canvas.getWidth();
	}
	
	public int getHeight() {
		return canvas.getHeight();
	}
	
	public void save() {
		canvas.save();
	}
	
	public void restore() {
		canvas.restore();
	}
	
	public Rect getClipBounds()
	{
		return canvas.getClipBounds();
	}
	
	public void clipToRectangle(float left, float top, float right, float bottom) {
		canvas.clipRect(left, top, right, bottom);
	}
	
	public void clipToPath(UiPath path) {
		if (path == null) {
			return;
		}
		canvas.clipPath(path.path);
	}

	public void concatMatrix(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22) {
		Matrix mat = new Matrix();
		float values[] = new float[] {m00, m10, m20, m01, m11, m21, m02, m12, m22};
		mat.setValues(values);
		canvas.concat(mat);
	}
	
	public void drawText(String text, float x, float y, UiFont font, int color) {
		if (font == null || text == null) {
			return;
		}
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		font.applyPaint(paint);
		paint.setColor(color);
		canvas.drawText(text, x, y - paint.ascent(), paint);
	}
	
	public void drawLine(float x1, float y1, float x2, float y2, UiPen pen) {
		if (pen == null) {
			return;
		}
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		pen.applyPaint(paint);
		canvas.drawLine(x1, y1, x2, y2, paint);
	}
	
	public void drawLines(float[] points, UiPen pen) {
		if (points == null) {
			return;
		}
		int n = points.length / 2;
		if (n > 1) {
			Path path = new Path();
			path.moveTo(points[0], points[1]);			
			for (int i = 1; i < n; i++) {
				path.lineTo(points[i*2], points[i*2+1]);			
			}
			Paint paint = new Paint();
			paint.setAntiAlias(flagAntiAlias);
			pen.applyPaint(paint);
			canvas.drawPath(path, paint);			
		}
	}
	
	public void drawArc(float x1, float y1, float x2, float y2, float startDegrees, float sweepDegrees, UiPen pen) {
		if (pen == null) {
			return;
		}
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		pen.applyPaint(paint);
		canvas.drawArc(
				new RectF(x1, y1, x2, y2)
				, startDegrees
				, sweepDegrees
				, false, paint);
	}
	
	public void drawRectangle(float x1, float y1, float x2, float y2, UiPen pen, UiBrush brush) {
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		if (brush != null) {
			brush.applyPaint(paint);
			canvas.drawRect(x1, y1, x2, y2, paint);
		}
		if (pen != null) {
			pen.applyPaint(paint);
			canvas.drawRect(x1, y1, x2, y2, paint);
		}
	}

	public void drawRoundRectangle(float x1, float y1, float x2, float y2, float rx, float ry, UiPen pen, UiBrush brush) {
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		if (brush != null) {
			brush.applyPaint(paint);
			canvas.drawRoundRect(new RectF(x1, y1, x2, y2), rx, ry, paint);
		}
		if (pen != null) {
			pen.applyPaint(paint);
			canvas.drawRoundRect(new RectF(x1, y1, x2, y2), rx, ry, paint);
		}
	}

	public void drawEllipse(float x1, float y1, float x2, float y2, UiPen pen, UiBrush brush) {
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		if (brush != null) {
			brush.applyPaint(paint);
			canvas.drawOval(new RectF(x1, y1, x2, y2), paint);
		}
		if (pen != null) {
			pen.applyPaint(paint);
			canvas.drawOval(new RectF(x1, y1, x2, y2), paint);
		}
	}
	
	public void drawPolygon(float[] points, UiPen pen, UiBrush brush, int fillMode) {
		if (points == null) {
			return;
		}
		int n = points.length / 2;
		if (n > 1) {
			Path path = new Path();
			path.moveTo(points[0], points[1]);			
			for (int i = 1; i < n; i++) {
				path.lineTo(points[i*2], points[i*2+1]);			
			}
			path.close();
			path.setFillType(UiPath.getFillType(fillMode));
			Paint paint = new Paint();
			paint.setAntiAlias(flagAntiAlias);
			if (brush != null) {
				brush.applyPaint(paint);
				canvas.drawPath(path, paint);
			}
			if (pen != null) {
				pen.applyPaint(paint);
				canvas.drawPath(path, paint);
			}			
		}
	}
	
	public void drawPath(UiPath path, UiPen pen, UiBrush brush) {
		if (path == null) {
			return;
		}
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		if (brush != null) {
			brush.applyPaint(paint);
			canvas.drawPath(path.path, paint);
		}
		if (pen != null) {
			pen.applyPaint(paint);
			canvas.drawPath(path.path, paint);
		}
	}
	
	public void drawPie(float x1, float y1, float x2, float y2, float startDegrees, float sweepDegrees, UiPen pen, UiBrush brush) {
		Paint paint = new Paint();
		paint.setAntiAlias(flagAntiAlias);
		if (brush != null) {
			brush.applyPaint(paint);
			canvas.drawArc(
					new RectF(x1, y1, x2, y2)
					, startDegrees
					, sweepDegrees
					, true, paint);
		}
		if (pen != null) {
			pen.applyPaint(paint);
			canvas.drawArc(
					new RectF(x1, y1, x2, y2)
					, startDegrees
					, sweepDegrees
					, true, paint);
		}
	}
	
}
