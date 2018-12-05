/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

package slib.platform.android.ui.view;

import io.slib.R;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Build;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.Util;

public class UiSelectView extends Spinner implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }

	public void setUIFrame(int left, int top, int right, int bottom) {
		mLeft = left; mTop = top; mRight = right; mBottom = bottom;
		setPadding(2, 0, (bottom - top) + 2, 0);
	}

	public static UiSelectView _create(Context context)
	{
		try {
			UiSelectView view = new UiSelectView(context);
			return view;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _applyList(final View view, final String[] items) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_applyList(view, items);
				}
			});
			return;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			MyAdapter adapter = sv.new MyAdapter(items);
			sv.setAdapter(adapter);
		}
	}
	
	public static int _getSelectedIndex(View view) {
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			int n = sv.getSelectedItemPosition();
			if (n < 0) {
				n = 0;
			}
			return n;
		}
		return 0;
	}
	
	public static void _select(final View view, final int n) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_select(view, n);
				}
			});
			return;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			sv.setSelection(n);
		}
	}

	public static boolean _setAlignment(final View view, final int align) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setAlignment(view, align);
				}
			});
			return true;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			sv.textAlignment = Util.getAndroidAlignment(align);
			View child = sv.getChildAt(0);
			if (child instanceof TextView) {
				((TextView)child).setGravity(sv.textAlignment);
			}
			return true;
		}
		return false;
	}

	public static boolean _setTextColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setTextColor(view, color);
				}
			});
			return true;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			sv.textColor = color;
			View child = sv.getChildAt(0);
			if (child instanceof TextView) {
				((TextView)child).setTextColor(color);
			}
			return true;
		}
		return false;
	}

	public static boolean _setBorder(final View view, final boolean flag) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setBorder(view, flag);
				}
			});
			return true;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			sv.border = flag;
			sv.invalidate();
			return true;
		}
		return false;
	}

	public static boolean _setBackgroundColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setBackgroundColor(view, color);
				}
			});
			return true;
		}
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			sv.backgroundColor = color;
			sv.invalidate();
			return true;
		}
		return false;
	}

	public static boolean _setFont(View view, UiFont font) {
		if (view instanceof UiSelectView) {
			UiSelectView sv = (UiSelectView)view;
			if (font != null) {
				sv.font = font;
				return true;				
			}
		}
		return false;
	}

	private static native void nativeOnSelect(long instance, int n);
	public static void onEventSelect(IView view, int n) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnSelect(instance, n);
		}
	}

	int textAlignment = Gravity.CENTER;
	int textColor = Color.BLACK;
	boolean border = true;
	int backgroundColor = 0;
	UiFont font;

	public UiSelectView(Context context) {
		
		super(context);
		this.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
				if (position < 0) {
					position = 0;
				}
				onEventSelect(UiSelectView.this, position);
			}
			
			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});
		setBackgroundColor(0);
	}
	
	class MyAdapter extends ArrayAdapter<String> {
		
		MyAdapter(String[] items) {
			super(UiSelectView.this.getContext(), R.layout.slib_spinner_default_item, items);
			setDropDownViewResource(R.layout.slib_spinner_default_dropdown_item);
		}
		
		public View getView(int position, View convertView, ViewGroup parent) {
			View ret = super.getView(position, convertView, parent);
			if (font != null) {
				if (ret != null) {
					if (ret instanceof TextView) {
						TextView tv = (TextView)ret;
						tv.setGravity(textAlignment);
						tv.setTextColor(textColor);
						tv.setTypeface(font.getTypeface());
						tv.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
					}
				}
			}
			return ret;
		}
		
		public View getDropDownView(int position, View convertView, ViewGroup parent) {
			View ret = super.getDropDownView(position, convertView, parent);
			if (font != null) {
				if (ret != null) {
					if (ret instanceof TextView) {
						TextView tv = (TextView)ret;
						tv.setTypeface(font.getTypeface());
					}
				}				
			}
			return ret;
		}
	}

	Paint paintBorder;
	Paint paintFill;
	Paint paintDropdown;

	@Override
	protected void onDraw(Canvas canvas) {
		if (paintBorder == null) {
			paintBorder = new Paint();
			paintBorder.setAntiAlias(true);
			paintBorder.setColor(0xFFB0B0B0);
			paintBorder.setStrokeWidth(1);
			paintBorder.setStyle(Paint.Style.STROKE);
		}
		if (paintFill == null) {
			paintFill = new Paint();
			paintFill.setStyle(Paint.Style.FILL);
		}
		if (paintDropdown == null) {
			paintDropdown = new Paint();
			paintDropdown.setAntiAlias(true);
			paintDropdown.setColor(0xFF707070);
			paintDropdown.setStyle(Paint.Style.STROKE);
			paintDropdown.setStrokeCap(Paint.Cap.ROUND);
		}
		int w = getWidth();
		int h = getHeight();
		paintFill.setColor(backgroundColor);
		if (border) {
			if (Build.VERSION.SDK_INT >= 21) {
				int min = w > h ? h : w;
				int radius = min / 6;
				if (backgroundColor != 0) {
					canvas.drawRoundRect(0, 0, w, h, radius, radius, paintFill);
				}
				canvas.drawRoundRect(0, 0, w, h, radius, radius, paintBorder);
			} else {
				if (backgroundColor != 0) {
					canvas.drawRect(0, 0, w, h, paintFill);
				}
				canvas.drawRect(0, 0, w, h, paintBorder);
			}
		} else {
			if (backgroundColor != 0) {
				canvas.drawRect(0, 0, w, h, paintFill);
			}
		}

		paintDropdown.setStrokeWidth(h / 10);
		int h2 = h / 2;
		int x1 = w - h2 / 2;
		int x2 = x1 - h2 / 2;
		int x3 = x2 - h2 / 2;
		int y1 = h2 - h / 7;
		int y2 = h2 + h / 7;
		canvas.drawLine(x1, y1, x2, y2, paintDropdown);
		canvas.drawLine(x2, y2, x3, y1, paintDropdown);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

}
