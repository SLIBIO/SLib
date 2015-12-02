package slib.platform.android.ui;

import java.util.Vector;

import slib.platform.android.Logger;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.RectF;

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
	
	public void draw(Graphics graphics, float dx1, float dy1, float dx2, float dy2
			, int sx1, int sy1, int sx2, int sy2) {
		try {
			RectF rd = new RectF(dx1, dy1, dx2, dy2);
			Rect rs = new Rect(sx1, sy1, sx2, sy2);
			graphics.canvas.drawBitmap(bitmap, rs, rd, null);
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	public static void drawPixels(Graphics graphics, float dx1, float dy1, float dx2, float dy2
			, int[] pixels, int stride, int sw, int sh) {
		try {
			if (sw == 0 || sh == 0) {
				return;
			}
			graphics.canvas.save();
			graphics.canvas.scale((dx2 - dx1) / sw, (dy2 - dy1) / sh, dx1, dy1);
			graphics.canvas.drawBitmap(pixels, 0, stride, dx1, dy1, sw, sh, true, null);
			graphics.canvas.restore();
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	static int[] arrayBufferForUi;
	static Vector<int[]> arrayBufferForNonUi = new Vector<int[]>();
	public static int[] getArrayBuffer() {
		try {
			if (Util.isUiThread()) {
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
