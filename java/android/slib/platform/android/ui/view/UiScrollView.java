package slib.platform.android.ui.view;

import android.content.Context;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.ScrollView;
import slib.platform.android.Logger;

public class UiScrollView extends ScrollView {
	
	HorizontalScrollView horz;
	
	public static UiScrollView _create(Context context) {
		try {
			UiScrollView ret = new UiScrollView(context);
			return ret;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void _setBackgroundColor(View view, int color) {
		view.setBackgroundColor(color);
	}
	
	public static void _scrollTo(View view, float x, float y) {
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			sv.scrollTo(0, (int)y);
			sv.horz.scrollTo((int)x, 0);
		} else {
			view.scrollTo(0, (int)y);
		}
	}
	
	public static float _getScrollX(View view) {
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			return sv.horz.getScrollX();
		} else {
			return view.getScrollX();
		}
	}

	public static float _getScrollY(View view) {
		if (view instanceof UiScrollView) {
			UiScrollView sv = (UiScrollView)view;
			return sv.getScrollY();
		} else {
			return view.getScrollY();
		}
	}
	
	public UiScrollView(Context context) {
		super(context);
		horz = new HorizontalScrollView(context);
		FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.WRAP_CONTENT);
		horz.setLayoutParams(lp);
		super.addView(horz);
	}
	
	public void addView(View view) {
		horz.removeAllViews();
		horz.addView(view);
	}
	
}
