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

import android.content.Context;
import android.graphics.Rect;
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout;
import android.view.View;

import slib.platform.android.Logger;
import slib.platform.android.ui.UiThread;

public class UiRefreshView extends SwipeRefreshLayout implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public static View _create(Context context) {
		try {
			return new UiRefreshView(context);
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void _setRefreshing(final View view, final boolean flag) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					_setRefreshing(view, flag);
				}
			});
			return;
		}
		if (view instanceof SwipeRefreshLayout) {
			SwipeRefreshLayout rv = (SwipeRefreshLayout)view;
			rv.setRefreshing(flag);
		}
	}

	private static native void nativeOnRefresh(long instance);

	public static void onEventRefresh(IView view) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnRefresh(instance);
		}
	}

	public UiRefreshView(Context context) {
		super(context);
		this.setOnRefreshListener(new OnRefreshListener()  {
			@Override
			public void onRefresh() {
				onEventRefresh(UiRefreshView.this);
			}
		});
	}

	@Override
	public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		super.onLayout(changed, l, t, r, b);
	}

}
