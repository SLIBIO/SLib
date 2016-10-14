package slib.platform.android.ui;

import java.util.Vector;

import slib.platform.android.Logger;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Shader;

public class UiBitmap {
	
	public Bitmap bitmap;
	
	private UiBitmap() {}

	public static UiBitmap create(int width, int height) {
		try {
			Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
			if (bitmap != null) {
				UiBitmap ret = new UiBitmap();
				ret.bitmap = bitmap;
				return ret;
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static UiBitmap load(byte[] data) {
		try {
			Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);
			if (bitmap != null) {				
				if (bitmap.getWidth() > 0 && bitmap.getHeight() > 0) {
					UiBitmap ret = new UiBitmap();
					ret.bitmap = bitmap;
					return ret;						
				}
				bitmap.recycle();
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public int getWidth() {
		try {
			return bitmap.getWidth();
		} catch (Throwable e) {
			Logger.exception(e);
			return 0;
		}
	}
	
	public int getHeight() {
		try {
			return bitmap.getHeight();
		} catch (Throwable e) {
			Logger.exception(e);
			return 0;
		}
	}
	
	public void recycle() {
		try {
			bitmap.recycle();			
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public void read(int x, int y, int width, int height, int[] out, int stride) {
		try {
			bitmap.getPixels(out, 0, stride, x, y, width, height);			
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public void write(int x, int y, int width, int height, int[] in, int stride) {
		try {
			bitmap.setPixels(in, 0, stride, x, y, width, height);			
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public Graphics getCanvas() {
		try {
			Canvas canvas = new Canvas(bitmap);
			return new Graphics(canvas);
		} catch (Throwable e) {
			Logger.exception(e);
			return null;
		}
	}
	
	void draw(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int sx1, int sy1, int sx2, int sy2, 
			float _alpha, float blur, int tileMode,
			float[] cm) 
	{
		try {
			int alpha = (int)(_alpha * 255);
			if (alpha <= 0) {
				return;
			}			
			RectF rd = new RectF(dx1, dy1, dx2, dy2);
			Rect rs = new Rect(sx1, sy1, sx2, sy2);
			if (alpha < 255 || blur > 0.5f || tileMode != 0) {
				Paint paint = new Paint();
				if (alpha < 255) {
					paint.setAlpha(alpha);				
				}
				if (cm != null) {
					ColorMatrixColorFilter cf = new ColorMatrixColorFilter(cm);
					paint.setColorFilter(cf);
				}
				if (tileMode != 0) {
					BitmapShader bs = new BitmapShader(bitmap, Shader.TileMode.REPEAT, Shader.TileMode.REPEAT);
					paint.setShader(bs);
					graphics.canvas.drawRect(rd, paint);
				} else {
					graphics.canvas.drawBitmap(bitmap, rs, rd, paint);
				}
			} else {
				graphics.canvas.drawBitmap(bitmap, rs, rd, null);
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public void draw(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int sx1, int sy1, int sx2, int sy2,
			float alpha, float blur, int tileMode)
	{
		draw(graphics, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, alpha, blur, tileMode, null);
	}

	public void draw(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int sx1, int sy1, int sx2, int sy2,
			float alpha, float blur, int tileMode,
			float crx, float cry, float crz, float crw,
			float cgx, float cgy, float cgz, float cgw,
			float cbx, float cby, float cbz, float cbw,
			float cax, float cay, float caz, float caw,
			float ccx, float ccy, float ccz, float ccw)
	{
		float[] f = new float[] {
			crx, cry, crz, crw, ccx * 255,
			cgx, cgy, cgz, cgw, ccy * 255,
			cbx, cby, cbz, cbw, ccz * 255,
			cax, cay, caz, caw, ccw * 255,
		};
		draw(graphics, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, alpha, blur, tileMode, f);
	}
	
	static void drawPixels(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int[] pixels, int stride, int sw, int sh,
			float _alpha, float blur, int tileMode,
			float[] cm) 
	{
		try {
			if (sw == 0 || sh == 0) {
				return;
			}
			int alpha = (int)(_alpha * 255);
			if (alpha <= 0) {
				return;
			}			
			graphics.canvas.save();
			graphics.canvas.scale((dx2 - dx1) / sw, (dy2 - dy1) / sh, dx1, dy1);
			if (alpha < 255 || cm != null) {
				Paint paint = new Paint();
				if (alpha < 255) {
					paint.setAlpha(alpha);			
				}
				if (cm != null) {
					ColorMatrixColorFilter cf = new ColorMatrixColorFilter(cm);
					paint.setColorFilter(cf);
				}
				graphics.canvas.drawBitmap(pixels, 0, stride, dx1, dy1, sw, sh, true, paint);
			} else {
				graphics.canvas.drawBitmap(pixels, 0, stride, dx1, dy1, sw, sh, true, null);
			}
			graphics.canvas.restore();
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public static void drawPixels(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int[] pixels, int stride, int sw, int sh,
			float alpha, float blur, int tileMode) 
	{
		drawPixels(graphics, dx1, dy1, dx2, dy2, pixels, stride, sw, sh, alpha, blur, tileMode, null);
	}
	
	public static void drawPixels(Graphics graphics,
			float dx1, float dy1, float dx2, float dy2,
			int[] pixels, int stride, int sw, int sh,
			float alpha, float blur, int tileMode,
			float crx, float cry, float crz, float crw,
			float cgx, float cgy, float cgz, float cgw,
			float cbx, float cby, float cbz, float cbw,
			float cax, float cay, float caz, float caw,
			float ccx, float ccy, float ccz, float ccw)
	{
		float[] f = new float[] {
			crx, cry, crz, crw, ccx * 255,
			cgx, cgy, cgz, cgw, ccy * 255,
			cbx, cby, cbz, cbw, ccz * 255,
			cax, cay, caz, caw, ccw * 255,
		};
		drawPixels(graphics, dx1, dy1, dx2, dy2, pixels, stride, sw, sh, alpha, blur, tileMode, f);
	}
	
	static int[] arrayBufferForUi;
	static Vector<int[]> arrayBufferForNonUi = new Vector<int[]>();
	public static int[] getArrayBuffer() {
		try {
			if (UiThread.isUiThread()) {
				if (arrayBufferForUi == null) {
					arrayBufferForUi = new int[1024 * 1024];
				}
				return arrayBufferForUi;
			} else {
				int[] ret = null;
				try {
					ret = arrayBufferForNonUi.remove(0);
				} catch (Exception ex) {}
				if (ret == null) {
					ret = new int[1024 * 1024];
				}
				return ret;
			}
		} catch (Throwable e) {
			Logger.exception(e);
			return null;
		}
	}
	
	public static void returnArrayBuffer(int[] buffer) {
		if (buffer != arrayBufferForUi) {
			if (arrayBufferForNonUi.size() < 5) {
				arrayBufferForNonUi.add(buffer);
			}
		}
	}
}
