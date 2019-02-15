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
import android.widget.HorizontalScrollView;

public class UiHorizontalScrollView extends HorizontalScrollView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	boolean mPaging = false;
	int mPageWidth = 0;

	public UiHorizontalScrollView(Context context) {
		super(context);
	}

	boolean flagInitedContent = false;
	int initScrollX = 0;
	int initScrollY = 0;

	public void _scrollTo(int x, int y) {
		if (flagInitedContent) {
			scrollTo(x, y);
		} else {
			initScrollX = x;
			initScrollY = y;
		}
	}

	public void setPaging(boolean flagPaging, int pageWidth, int pageHeight) {
		mPaging = flagPaging;
		mPageWidth = pageWidth;
	}

	@Override
	public void addView(View view) {
		removeAllViews();
		super.addView(view);
	}

	@Override
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {
		super.onScrollChanged( l, t, oldl, oldt );
		if (flagInitedContent) {
			UiScrollView.onEventScroll(this, l, t);
		}
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		super.onLayout(changed, l, t, r, b);
		if (getChildCount() > 0) {
			View child = getChildAt(0);
			if (child.getWidth() > 0) {
				if (!flagInitedContent) {
					scrollTo(initScrollX, initScrollY);
					flagInitedContent = true;
				}
			}
		}
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
	@Override
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
		int width = mPageWidth;
		if (width <= 0) {
			width = getWidth();
		}
		int sx = getScrollX();
		int page = sx / width;
		int align = page * width;
		if (sx + velocity > align + width / 2) {
			align += width;
		}
		smoothScrollTo(align, getScrollY());
	}

}
