package slib.platform.android.ui.view;

import android.content.Context;
import android.graphics.Rect;
import android.view.View;
import android.widget.HorizontalScrollView;
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
				return new HorizontalView(context);
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

	private static native void nativeOnScroll(long instance, int x, int y);
	public static void onEventScroll(IView view, int x, int y) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnScroll(instance, x, y);
		}
	}

	public UiScrollView(Context context) {
		super(context);
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

	static class HorizontalView extends HorizontalScrollView implements IView {

		private long mInstance = 0;
		public long getInstance() { return mInstance; }
		public void setInstance(long instance) { this.mInstance = instance; }
		private int mLeft, mTop, mRight, mBottom;
		public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
		public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

		public HorizontalView(Context context) {
			super(context);
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
			setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
		}
	}

}
