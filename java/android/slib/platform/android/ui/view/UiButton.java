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

import slib.platform.android.Logger;
import slib.platform.android.ui.UiFont;
import slib.platform.android.ui.UiThread;

import android.content.Context;
import android.graphics.Rect;
import android.util.TypedValue;
import android.view.View;
import android.widget.Button;

public class UiButton extends Button implements IView, Button.OnClickListener {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public static UiButton _create(Context context)
	{
		try {
			UiButton button = new UiButton(context);
			return button;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static String _getText(View view) {
		try {
			if (view instanceof Button) {
				String text = ((Button)view).getText().toString();
				if (text == null) {
					text = "";
				}
				return text;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return "";
	}
	
	public static boolean _setText(final View view, String text) {
		if (!(UiThread.isUiThread())) {
			final String t = text;
			view.post(new Runnable() {
				public void run() {
					_setText(view, t);
				}
			});
			return true;
		}
		if (view instanceof Button) {
			if (text == null) {
				text = "";
			}
			((Button)view).setText(text);
			return true;
		}
		return false;
	}

	public static boolean _setFont(final View view, UiFont font) {
		if (!(UiThread.isUiThread())) {
			final UiFont f = font;
			view.post(new Runnable() {
				public void run() {
					_setFont(view, f);
				}
			});
			return true;
		}
		if (view instanceof Button) {
			if (font != null) {
				Button v = (Button)view;
				v.setTypeface(font.getTypeface());
				v.setTextSize(TypedValue.COMPLEX_UNIT_PX, font.getSize());
				return true;				
			}
		}
		return false;
	}
	
	
	public UiButton(Context context) {
		super(context);
		setOnClickListener(this);
		this.setPadding(0, -5, 0, 0);
	}

	@Override
	public void onClick(View v) {
		UiView.onEventClick(this);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

}
