package slib.platform.android.ui.view;

import android.content.Context;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.ScrollView;
import slib.platform.android.Logger;

public class UiScrollView extends ScrollView {
	
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

	public static void _setBackgroundColor(View view, int color) {
		view.setBackgroundColor(color);
	}
	
	public static void _scrollTo(View view, int x, int y) {
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
	public static void onEventScroll(View view, int x, int y) {
		long instance = UiView.getInstance(view);
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
	
	static class HorizontalView extends HorizontalScrollView {
		
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

	static class VerticalView extends ScrollView {
		
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
