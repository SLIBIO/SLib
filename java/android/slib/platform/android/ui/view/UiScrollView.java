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
import slib.platform.android.ui.window.UiWindow;

public class UiScrollView extends ScrollView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) {
		mLeft = left; mTop = top; mRight = right; mBottom = bottom;
		if (mContent instanceof UiScrollContentView) {
			((UiScrollContentView)mContent).setupTiles(this);
		}
	}

	UiGestureDetector gestureDetector;

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
		try {
			view.setBackgroundColor(color);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public static void _scrollTo(final View view, final int x, final int y, final boolean flagAnimate) {
		try {
			if (view instanceof UiScrollView) {
				if (flagAnimate) {
					((UiScrollView) view).smoothScrollTo(x, y);
				} else {
					((UiScrollView) view)._scrollTo(x, y);
				}
			} else if (view instanceof UiHorizontalScrollView) {
				if (flagAnimate) {
					((UiHorizontalScrollView) view).smoothScrollTo(x, y);
				} else {
					((UiHorizontalScrollView) view)._scrollTo(x, y);
				}
			} else {
				view.scrollTo(x, y);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	public static int _getScrollX(View view) {
		return view.getScrollX();
	}

	public static int _getScrollY(View view) {
		return view.getScrollY();
	}

	public static void _setPaging(View view, boolean flagPaging, int pageWidth, int pageHeight) {
		try {
			if (view instanceof UiScrollView) {
				((UiScrollView) view).setPaging(flagPaging, pageWidth, pageHeight);
			} else if (view instanceof UiHorizontalScrollView) {
				((UiHorizontalScrollView) view).setPaging(flagPaging, pageWidth, pageHeight);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void _setLockScroll(View view, boolean flagLock) {
		try {
			if (view instanceof UiScrollView) {
				((UiScrollView) view).setLockScroll(flagLock);
			} else if (view instanceof UiHorizontalScrollView) {
				((UiHorizontalScrollView) view).setLockScroll(flagLock);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void _setScrollBarsVisible(View view, boolean flagHorz, boolean flagVert) {
		try {
			view.setHorizontalScrollBarEnabled(flagHorz);
			view.setVerticalScrollBarEnabled(flagVert);
		} catch (Exception e) {
			Logger.exception(e);
		}
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
	View mContent;

	public UiScrollView(Context context) {
		super(context);
	}

	public void setPaging(boolean flagPaging, int pageWidth, int pageHeight) {
		mPaging = flagPaging;
		mPageHeight = pageHeight;
	}

	@Override
	public void addView(View view) {
		mContent = view;
		removeAllViews();
		super.addView(view);
		if (view instanceof UiScrollContentView) {
			((UiScrollContentView)view).setupTiles(this);
		}
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

	@Override
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {	    
	    super.onScrollChanged( l, t, oldl, oldt );
	    if (flagInitedContent) {
		    onEventScroll(this, l, t);
	    }
	    if (mContent instanceof UiScrollContentView) {
		    ((UiScrollContentView)mContent).setupTiles(this);
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
			if (child instanceof IView) {
				IView view = (IView)child;
				Rect frame = view.getUIFrame();
				child.layout(frame.left, frame.top, frame.right, frame.bottom);
			}
			if (child.getHeight() > 0) {
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

	boolean flagLock = false;

	public void setLockScroll(boolean flag) {
		flagLock = flag;
	}

	@SuppressLint("ClickableViewAccessibility")
	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		if (checkLock(ev)) {
			return false;
		}
		// UiWindow.dismissKeyboard(this, ev);
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

	@Override
	public boolean onInterceptTouchEvent(MotionEvent ev) {
		if (checkLock(ev)) {
			return false;
		}
		return super.onInterceptTouchEvent(ev);
	}

	@Override
	public boolean dispatchTouchEvent(MotionEvent ev) {
		if (gestureDetector != null) {
			gestureDetector.onTouchEvent(ev);
		}
		UiView.onEventTouch(this, ev, true, true);
		return super.dispatchTouchEvent(ev);
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

	boolean checkLock(MotionEvent ev) {
		int action = ev.getAction();
		if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
			flagLock = false;
		}
		return flagLock;
	}

}
