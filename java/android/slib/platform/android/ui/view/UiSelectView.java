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
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.Util;

public class UiSelectView extends Spinner implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }
	private boolean mStopPropagation = false;
	public boolean isStopPropagation() { return mStopPropagation; }
	public void setStopPropagation(boolean flag) { mStopPropagation = flag; }
	public boolean dispatchSuperTouchEvent(MotionEvent ev) { return super.dispatchTouchEvent(ev); }

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
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				MyAdapter adapter = sv.new MyAdapter(items);
				sv.setAdapter(adapter);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public static int _getSelectedIndex(View view) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				int n = sv.getSelectedItemPosition();
				if (n < 0) {
					n = 0;
				}
				return n;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return 0;
	}
	
	public static void _select(final View view, final int n) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				sv.setSelection(n);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static boolean _setAlignment(final View view, final int align) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				sv.textAlignment = Util.getAndroidAlignment(align);
				View child = sv.getChildAt(0);
				if (child instanceof TextView) {
					((TextView) child).setGravity(sv.textAlignment);
				}
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static boolean _setTextColor(final View view, final int color) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				sv.textColor = color;
				View child = sv.getChildAt(0);
				if (child instanceof TextView) {
					((TextView) child).setTextColor(color);
				}
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static boolean _setBorder(final View view, final boolean flag) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				sv.border = flag;
				sv.applyBackground();
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static boolean _setBackgroundColor(final View view, final int color) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				sv.backgroundColor = color;
				sv.applyBackground();
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static boolean _setFont(View view, UiFont font) {
		try {
			if (view instanceof UiSelectView) {
				UiSelectView sv = (UiSelectView) view;
				if (font != null) {
					sv.font = font;
					return true;
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
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

	int textAlignment = Gravity.LEFT;
	int textColor = Color.BLACK;
	boolean border = true;
	int backgroundColor = 0;
	UiFont font;

	public UiSelectView(Context context) {
		
		super(context);
		this.setOnItemSelectedListener(new Spinner.OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
				invalidate();
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
		setPadding(2, 0,  2, 0);
	}
	
	class MyAdapter extends ArrayAdapter<String> {
		
		MyAdapter(String[] items) {
			super(UiSelectView.this.getContext(), android.R.layout.simple_spinner_item, items);
			setDropDownViewResource(R.layout.slib_spinner_default_dropdown_item);
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			if (convertView != null) {
				return convertView;
			}
			return new View(parent.getContext());
		}

		@Override
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

	Paint paintDropdown;
	Paint paintText;
	Rect boundsText = new Rect();

	@Override
	protected void onDraw(Canvas canvas) {

		if (paintDropdown == null) {
			paintDropdown = new Paint();
			paintDropdown.setAntiAlias(true);
			paintDropdown.setColor(0xFF707070);
			paintDropdown.setStyle(Paint.Style.STROKE);
			paintDropdown.setStrokeCap(Paint.Cap.ROUND);
		}
		if (paintText == null) {
			paintText = new Paint();
			paintText.setAntiAlias(true);
			paintDropdown.setStyle(Paint.Style.STROKE);
			paintDropdown.setStrokeCap(Paint.Cap.ROUND);
		}

		int w = getWidth();
		int h = getHeight();

		paintDropdown.setStrokeWidth(h / 10);
		int h2 = h / 2;
		int x1 = w - h2 / 2;
		int x2 = x1 - h2 / 2;
		int x3 = x2 - h2 / 2;
		int y1 = h2 - h / 7;
		int y2 = h2 + h / 7;
		canvas.drawLine(x1, y1, x2, y2, paintDropdown);
		canvas.drawLine(x2, y2, x3, y1, paintDropdown);

		String text = (String)(getSelectedItem());
		if (text != null && text.length() > 0) {
			paintText.setColor(textColor);
			paintText.setTypeface(font.getTypeface());
			paintText.setTextSize(font.getSize());
			paintText.getTextBounds(text, 0, text.length(), boundsText);
			float y = (h + boundsText.height()) / 2;
			int align = textAlignment & Gravity.HORIZONTAL_GRAVITY_MASK;
			if (align == Gravity.LEFT) {
				canvas.drawText(text, border ? h / 10 : 2, y, paintText);
			} else {
				float tw = boundsText.width();
				if (align == Gravity.RIGHT) {
					canvas.drawText(text, w - h - tw, y, paintText);
				} else {
					canvas.drawText(text, (w - tw) / 2, y, paintText);
				}
			}
		}

	}

	@Override
	protected void onFocusChanged(boolean focused, int direction, Rect previouslyFocusedRect) {
		super.onFocusChanged(focused, direction, previouslyFocusedRect);
		if (focused) {
			UiView.onEventSetFocus(this);
		}
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	void applyBackground() {
		UiEditView.applyBackground(this, border, backgroundColor);
	}

}
