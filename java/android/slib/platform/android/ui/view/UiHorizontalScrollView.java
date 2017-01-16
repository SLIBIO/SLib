package slib.platform.android.ui.view;

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

	public UiHorizontalScrollView(Context context) {
		super(context);
	}

	public void setPaging(boolean flagPaging) {
		mPaging = flagPaging;
	}

	@Override
	public void addView(View view) {
		removeAllViews();
		super.addView(view);
	}

	@Override
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {
		super.onScrollChanged( l, t, oldl, oldt );
		UiScrollView.onEventScroll(this, l, t);
	}

	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
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
		int width = getWidth();
		int sx = getScrollX();
		int page = sx / width;
		int align = page * width;
		if (sx + velocity > align + width / 2) {
			align += width;
		}
		smoothScrollTo(align, getScrollY());
	}

}
