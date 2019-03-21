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

package slib.platform.android.ui.window;

import slib.platform.android.Android;
import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.ui.UiThread;
import slib.platform.android.ui.Util;
import slib.platform.android.ui.view.IView;
import slib.platform.android.ui.view.UiGestureDetector;
import slib.platform.android.ui.view.UiScrollView;
import slib.platform.android.ui.view.UiView;
import slib.platform.android.ui.view.UiWebView;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.Rect;
import android.os.Build;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.ViewTreeObserver;
import android.widget.EditText;
import android.widget.FrameLayout;

public class UiWindow extends FrameLayout implements IView, ViewTreeObserver.OnGlobalLayoutListener {

	private Activity activity;
	public long instance;
	private boolean flagClosed = false;
	private int backgroundColor;
	private boolean flagFullScreen;
	private int mLeft, mTop, mRight, mBottom;
	private int mWidth, mHeight;

	private long mViewInstance = 0;
	public long getInstance() { return mViewInstance; }
	public void setInstance(long instance) { this.mViewInstance = instance; }
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	public UiGestureDetector gestureDetector;

	private UiWindow(Context context) {
		super(context);
		instance = 0;
		backgroundColor = 0;
		if (context instanceof SlibActivity) {
			((SlibActivity) context).onCreateWindow(this);
		}
		setFocusableInTouchMode(true);
	}

	static UiWindow create(Activity activity
			, boolean flagFullScreen, boolean flagCenterScreen
			, int x, int y, int width, int height) {
		try {
			final UiWindow ret = new UiWindow(activity);
			ret.flagFullScreen = flagFullScreen;
			FrameLayout.LayoutParams params;
			if (flagFullScreen) {
				Logger.info("FullScreen Window Created");
				params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT);
			} else {
				ret.mLeft = x;
				ret.mTop = y;
				ret.mRight = x + width;
				ret.mBottom = y + height;
				Logger.info("Window Created (" + x + ", " + y + ") - Size(" + width + "," + height + ")");
				params = new FrameLayout.LayoutParams(width, height);
				if (flagCenterScreen) {
					params.gravity = Gravity.CENTER;
				} else {
					params.leftMargin = x;
					params.topMargin = y;
				}
			}
			activity.addContentView(ret, params);
			ret.activity = activity;
			return ret;

		} catch (Exception e) {
			Logger.exception(e);
			return null;
		}
	}

	public View getContentView() {
		return this;
	}

	public void close() {
		try {
			if (flagClosed) {
				return;
			}
			flagClosed = true;
			instance = 0;
			if (activity instanceof SlibActivity) {
				if (((SlibActivity) activity).onCloseWindow(this)) {
					return;
				}
			}
			ViewParent parent = getParent();
			if (parent instanceof ViewGroup) {
				((ViewGroup) parent).removeView(this);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	int getWindowBackgroundColor() {
		return backgroundColor;
	}

	public void setWindowBackgroundColor(int color) {
		backgroundColor = color;
		setBackgroundColor(color);
	}

	public Rect getFrame() {
		Rect ret = new Rect();
		if (flagFullScreen) {
			int[] off = new int[2];
			getLocationOnScreen(off);
			ret.left = off[0];
			ret.top = off[1];
			ret.right = ret.left + getWidth();
			ret.bottom = ret.top + getHeight();
		} else {
			ret.left = mLeft;
			ret.top = mTop;
			ret.right = mRight;
			ret.bottom = mBottom;
		}
		return ret;
	}

	public void setFrame(int left, int top, int right, int bottom) {
		if (flagFullScreen) {
			return;
		}
		mLeft = left;
		mTop = top;
		mRight = right;
		mBottom = bottom;
		try {
			final FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(right - left, bottom - top);
			params.leftMargin = left;
			params.topMargin = top;
			if (UiThread.isUiThread()) {
				setLayoutParams(params);
			} else {
				activity.runOnUiThread(new Runnable() {
					public void run() {
						setLayoutParams(params);
					}
				});
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public Point getSize() {
		Point ret = new Point();
		ret.x = mWidth;
		ret.y = mHeight;
		return ret;
	}

	public void setSize(int width, int height) {
		if (flagFullScreen) {
			return;
		}
		mWidth = width;
		mHeight = height;
		mRight = mLeft + width;
		mBottom = mTop + height;
		try {
			final FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) (getLayoutParams());
			if (params != null) {
				params.width = width;
				params.height = height;
			}
			if (UiThread.isUiThread()) {
				setLayoutParams(params);
			} else {
				activity.runOnUiThread(new Runnable() {
					public void run() {
						setLayoutParams(params);
					}
				});
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public boolean isVisible() {
		return getVisibility() != View.VISIBLE;
	}

	public void setVisible(boolean flag) {
		setVisibility(flag ? View.VISIBLE : View.INVISIBLE);
	}

	public boolean isAlwaysOnTop() {
		return false;
	}

	public void setAlwaysOnTop(boolean flag) {
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	float _getAlpha() {
		return getAlpha();
	}

	public float getWindowAlpha() {
		return _getAlpha();
	}

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	void _setAlpha(float alpha) {
		setAlpha(alpha);
	}

	public void setWindowAlpha(float alpha) {
		if (alpha < 0) {
			alpha = 0;
		}
		if (alpha > 1) {
			alpha = 1;
		}
		_setAlpha(alpha);
	}

	public void focus() {
		requestFocus();
	}

	public Point convertCoordinateFromScreenToWindow(int x, int y) {
		Point ret = new Point();
		int[] location = new int[2];
		getLocationOnScreen(location);
		ret.x = x - location[0];
		ret.y = y - location[1];
		return ret;
	}

	public Point convertCoordinateFromWindowToScreen(int x, int y) {
		Point ret = new Point();
		int[] location = new int[2];
		getLocationOnScreen(location);
		ret.x = location[0] + x;
		ret.y = location[1] + y;
		return ret;
	}

	private static native void nativeOnResize(long instance, int w, int h);

	public void onSizeChanged(int w, int h, int oldw, int oldh) {
		super.onSizeChanged(w, h, oldw, oldh);
		Logger.info("Window Resized - Size(" + w + "," + h + ")");
		mWidth = w;
		mHeight = h;
		if (instance != 0) {
			nativeOnResize(instance, w, h);
		}
	}

	private static native boolean nativeOnClose(long instance);

	public void onClose() {
		if (instance != 0) {
			if (nativeOnClose(instance)) {
				close();
			}
		}
	}

	static int resolveFullSize(int measureSpec) {
		final int specMode = MeasureSpec.getMode(measureSpec);
		final int specSize = MeasureSpec.getSize(measureSpec);
		final int result;
		switch (specMode) {
			case MeasureSpec.AT_MOST:
				result = specSize;
				break;
			case MeasureSpec.EXACTLY:
				result = specSize;
				break;
			case MeasureSpec.UNSPECIFIED:
			default:
				result = 0;
		}
		return result;
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		measureChildren(widthMeasureSpec, heightMeasureSpec);
		if (flagFullScreen) {
			setMeasuredDimension(resolveFullSize(widthMeasureSpec), resolveFullSize(heightMeasureSpec));
		} else {
			setMeasuredDimension(UiView.resolveMeasure(mRight - mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom - mTop, heightMeasureSpec));
		}
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		int count = getChildCount();
		for (int i = 0; i < count; i++) {
			View child = getChildAt(i);
			if (child.getVisibility() != GONE) {
				if (child instanceof IView) {
					IView view = (IView)child;
					Rect frame = view.getUIFrame();
					child.layout(frame.left, frame.top, frame.right, frame.bottom);
				}
			}
		}
	}

	boolean flagAttachedGlobalLayoutListener = false;
	boolean flagAttachedNativeWindow = false;

	@Override
	protected void onAttachedToWindow() {
		super.onAttachedToWindow();
		Logger.info("UiWindow is attached to native window");
		flagAttachedNativeWindow = true;
		if (!flagAttachedGlobalLayoutListener) {
			getViewTreeObserver().addOnGlobalLayoutListener(this);
			flagAttachedGlobalLayoutListener = true;
		}
	}

	@Override
	protected void onDetachedFromWindow() {
		super.onDetachedFromWindow();
		Logger.info("UiWindow is detached from native window");
		flagAttachedNativeWindow = false;
	}

	@Override
	public void onGlobalLayout() {
		try {
			checkLayoutChanges();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	static int savedRootViewBottom = 0;

	public void checkLayoutChanges() {
		if (!flagAttachedNativeWindow) {
			return;
		}
		Rect rc = new Rect();
		View rootView = getRootView();
		rootView.getWindowVisibleDisplayFrame(rc);
		if (savedRootViewBottom == 0) {
			savedRootViewBottom = rc.bottom;
			return;
		}
		if (savedRootViewBottom == rc.bottom) {
			return;
		}
		savedRootViewBottom = rc.bottom;
		int heightKeyboard = Util.getDisplaySize(activity).y - rc.bottom;
		int heightKeyboardMin = (int)(60 * Util.getDisplayDensity(activity));
		if (heightKeyboard > heightKeyboardMin) {
			processKeyboardAppeared(activity, heightKeyboard);
		} else {
			processKeyboardDisappeared();
		}
	}

	static IView keyboardScrollView = null;
	static Rect originalFrame_keyboardScrollView = null;

	static boolean isFocusableView(View view) {
		return (view instanceof EditText || view instanceof UiWebView);
	}

	static void processKeyboardAppeared(Activity activity, int keyboardHeight) {
		Logger.info("Keyboard is appeared, height=" + keyboardHeight);
		View focusedView = activity.getCurrentFocus();
		if (isFocusableView(focusedView)) {
			IView scroll = null;
			if (focusedView instanceof UiWebView) {
				scroll = (UiWebView)focusedView;
			} else {
				ViewParent _parent = focusedView.getParent();
				View parent = null;
				if (_parent instanceof View) {
					parent = (View)_parent;
				}
				while (parent != null) {
					if (parent instanceof UiScrollView) {
						scroll = (UiScrollView)parent;
						break;
					}
					_parent = parent.getParent();
					if (_parent instanceof View) {
						parent = (View)_parent;
					} else {
						parent = null;
					}
				}
			}
			if (keyboardScrollView != scroll) {
				restoreKeyboardScrollView();
			}

			int heightScreen = Util.getDisplaySize(activity).y;
			if (scroll != null) {
				Rect scrollFrame = scroll.getUIFrame();
				if (keyboardScrollView == scroll) {
					scrollFrame = originalFrame_keyboardScrollView;
				}
				int yScroll = UiView.convertCoordinateFromViewToScreen((View)scroll, 0, scrollFrame.height()).y;
				if (yScroll > heightScreen - keyboardHeight) {
					originalFrame_keyboardScrollView = new Rect(scrollFrame);
					scrollFrame.bottom -= yScroll - (heightScreen - keyboardHeight);
					UiView.setFrame((View)scroll, scrollFrame.left, scrollFrame.top, scrollFrame.right, scrollFrame.bottom);
					keyboardScrollView = scroll;
				}
			}

		} else {
			restoreKeyboardScrollView();
		}
	}

	static void processKeyboardDisappeared() {
		Logger.info("Keyboard is disappeared");
		restoreKeyboardScrollView();
	}

	static void restoreKeyboardScrollView() {
		if (keyboardScrollView != null) {
			Rect frame = originalFrame_keyboardScrollView;
			UiView.setFrame((View)keyboardScrollView, frame.left, frame.top, frame.right, frame.bottom);
			keyboardScrollView = null;
		}
	}

	int locationOfFoucsView[] = new int[2];
	int locationOfWindow[] = new int[2];

	@Override
	public boolean dispatchTouchEvent(MotionEvent ev) {
		super.dispatchTouchEvent(ev);
		if (ev.getAction() == MotionEvent.ACTION_UP) {
			View view = activity.getCurrentFocus();
			if (view != null && isFocusableView(view) && view.isShown()) {
				view.getLocationOnScreen(locationOfFoucsView);
				getLocationOnScreen(locationOfWindow);
				float x = ev.getX() + locationOfWindow[0];
				float y = ev.getY() + locationOfWindow[1];
				if (x < locationOfFoucsView[0] || x > locationOfFoucsView[0] + view.getWidth() || y < locationOfFoucsView[1] || y > locationOfFoucsView[1] + view.getHeight()) {
					Android.dismissKeyboard(activity);
				}
			} else {
				Android.dismissKeyboard(activity);
			}
		}
		if (gestureDetector != null) {
			gestureDetector.onTouchEvent(ev);
			super.dispatchTouchEvent(ev);
			return true;
		} else {
			return super.dispatchTouchEvent(ev);
		}
	}

	@Override
	public void onDraw(Canvas canvas) {
		UiView.onEventDraw(this, canvas);
	}

	@Override
	public boolean onKeyDown(int keycode, KeyEvent event) {
		if (!(UiView.onEventKey(this, true, keycode, event))) {
			return super.onKeyDown(keycode, event);
		}
		return true;
	}

	@Override
	public boolean onKeyUp(int keycode, KeyEvent event) {
		if (!(UiView.onEventKey(this, false, keycode, event))) {
			return super.onKeyUp(keycode, event);
		}
		return true;
	}

	@Override
	public boolean onInterceptTouchEvent(MotionEvent event) {
		if (UiView.onHitTestTouchEvent(this, (int)(event.getX()), (int)(event.getY()))) {
			return true;
		}
		return false;
	};

	@SuppressLint("ClickableViewAccessibility")
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (!(UiView.onEventTouch(this, event))) {
			super.onTouchEvent(event);
		}
		return true;
	}

}