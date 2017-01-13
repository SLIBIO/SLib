package slib.platform.android.ui.view;

import android.content.Context;
import android.graphics.Rect;
import android.view.View;
import android.widget.FrameLayout;
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

	InnerHorizontalView horz;
	
	public static View _create(Context context, boolean flagBothScroll, boolean flagVertical) {
		try {
			if (flagBothScroll) {
				return new UiScrollView(context);
			} else {
				if (flagVertical) {
					return new VerticalView(context);
				} else {
					return new HorizontalView(context);
				}
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
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			sv.scrollTo(0, y);
			sv.horz.scrollTo(x, 0);
		} else {
			view.scrollTo(x, y);
		}
	}
	
	public static int _getScrollX(View view) {
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			return sv.horz.getScrollX();
		} else {
			return view.getScrollX();
		}
	}

	public static int _getScrollY(View view) {
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			return sv.getScrollY();
		} else {
			return view.getScrollY();
		}
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
		horz = new InnerHorizontalView(context);
		FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.WRAP_CONTENT);
		horz.setLayoutParams(lp);
		super.addView(horz);
	}
	
	public void addView(View view) {
		horz.removeAllViews();
		horz.addView(view);
	}

	protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
	    super.onScrollChanged( l, t, oldl, oldt );
	    onEventScroll(this, horz.getScrollX(), t);
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
		
		public void addView(View view) {
			removeAllViews();
			super.addView(view);
		}

		protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
		    super.onScrollChanged( l, t, oldl, oldt );
		    onEventScroll(this, l, t);
		}
	}

	static class VerticalView extends ScrollView implements IView {

		private long mInstance = 0;
		public long getInstance() { return mInstance; }
		public void setInstance(long instance) { this.mInstance = instance; }
		private int mLeft, mTop, mRight, mBottom;
		public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
		public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }
		
		public VerticalView(Context context) {
			super(context);
		}
		
		public void addView(View view) {
			removeAllViews();
			super.addView(view);
		}

		protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
		    super.onScrollChanged( l, t, oldl, oldt );
		    onEventScroll(this, l, t);
		}
	}

	class InnerHorizontalView extends HorizontalScrollView {
		
		public InnerHorizontalView(Context context) {
			super(context);
		}
		
		protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
		    super.onScrollChanged( l, t, oldl, oldt );
		    onEventScroll(UiScrollView.this, l, UiScrollView.this.getScrollY());
		}
	}
	
}
