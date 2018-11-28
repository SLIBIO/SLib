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

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Rect;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ScrollView;
import slib.platform.android.Logger;
import slib.platform.android.ui.UiThread;

public class UiScrollView extends ScrollView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public static View _create(Context context, boolean flagVertical) {
		try {
			if (flagVertical) {
				return new UiScrollView(context);
			} else {
				return new UiHorizontalScrollView(context);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void _setBackgroundColor(final View view, final int color) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setBackgroundColor(view, color);
				}
			});
		}
		view.setBackgroundColor(color);
	}
	
	public static void _scrollTo(final View view, final int x, final int y) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_scrollTo(view, x, y);
				}
			});
		}
		view.scrollTo(x, y);
	}
	
	public static int _getScrollX(View view) {
		return view.getScrollX();
	}

	public static int _getScrollY(View view) {
		return view.getScrollY();
	}

	public static void _setPaging(View view, boolean flagPaging, int pageWidth, int pageHeight) {
		if (view instanceof UiScrollView) {
			((UiScrollView)view).setPaging(flagPaging, pageWidth, pageHeight);
		} else if (view instanceof UiHorizontalScrollView) {
			((UiHorizontalScrollView)view).setPaging(flagPaging, pageWidth, pageHeight);
		}
	}

	public static void _setScrollBarsVisible(View view, boolean flagHorz, boolean flagVert) {
		view.setHorizontalScrollBarEnabled(flagHorz);
		view.setVerticalScrollBarEnabled(flagVert);
	}

	private static native void nativeOnScroll(long instance, int x, int y);
	public static void onEventScroll(IView view, int x, int y) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnScroll(instance, x, y);
		}
	}


	boolean mPaging = false;
	int mPageHeight = 0;

	public UiScrollView(Context context) {
		super(context);
	}

	public void setPaging(boolean flagPaging, int pageWidth, int pageHeight) {
		mPaging = flagPaging;
		mPageHeight = pageHeight;
	}

	@Override
	public void addView(View view) {
		removeAllViews();
		super.addView(view);
	}

	@Override
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
	    super.onScrollChanged( l, t, oldl, oldt );
		onEventScroll(this, l, t);
	}

	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(resolveSizeAndState(mRight-mLeft, widthMeasureSpec, 0), resolveSizeAndState(mBottom-mTop, heightMeasureSpec, 0));
	}

	boolean flagFling = false;

	public void fling(int velocity) {
		if (mPaging) {
			scrollToPage(velocity);
			flagFling = true;
		} else {
			super.fling(velocity);
		}
	}

	@SuppressLint("ClickableViewAccessibility")
	public boolean onTouchEvent(MotionEvent ev) {
		flagFling = false;
		boolean flag = super.onTouchEvent(ev);
		if (mPaging) {
			int action = ev.getAction();
			if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
				if (!flagFling) {
					scrollToPage(0);
				}
			}
		}
		return flag;
	}

	void scrollToPage(int velocity) {
		int height = mPageHeight;
		if (height <= 0) {
			height = getHeight();
		}
		int sy = getScrollY();
		int page = sy / height;
		int align = page * height;
		if (sy + velocity > align + height / 2) {
			align += height;
		}
		smoothScrollTo(getScrollX(), align);
	}

}
