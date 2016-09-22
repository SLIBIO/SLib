package slib.platform.android.ui.view;

import java.util.HashMap;

import slib.platform.android.Logger;
import slib.platform.android.ui.Graphics;
import slib.platform.android.ui.UiThread;
import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.Rect;
import android.os.Build;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.Button;
import android.widget.FrameLayout;

public class UiView {
	
	private UiView() {
	}

	public static Context getContext(View view)
	{
		return view.getContext();
	}

	static HashMap<View, Long> mInstances = new HashMap<View, Long>();

	public static long getInstance(View view) {
		Long instance = mInstances.get(view);
		if (instance != null) {
			return (long)instance;
		} else {
			return 0;
		}
	}
	
	public static void setInstance(View view, long instance) {
		mInstances.put(view, instance);
	}
	
	public static void freeView(View view) {
		mInstances.remove(view);
	}

	public static View createGeneric(Context context) {
		try {
			UiGenericView view = new UiGenericView(context);
			return view;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static View createGroup(Context context) {
		try {
			UiGroupView view = new UiGroupView(context);
			return view;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void setFocus(View view) {
		view.requestFocus();
	}

	public static void invalidate(View view) {
		if (UiThread.isUiThread()) {
			view.invalidate();
		} else {
			view.postInvalidate();			
		}
	}
	
	public static void invalidateRect(View view, int left, int top, int right, int bottom) {
		if (UiThread.isUiThread()) {
			view.invalidate(left, top, right, bottom);			
		} else {
			view.postInvalidate(left, top, right, bottom);			
		}
	}
	
	public static Rect getFrame(View view)
	{
		Rect ret = new Rect();
		ret.left = view.getLeft();
		ret.top = view.getTop();
		ret.right = view.getRight();
		ret.bottom = view.getBottom();
		return ret;
	}
	
	public static boolean setFrame(View view, int left, int top, int right, int bottom) {
		
		if (right < left) {
			right = left;
		}
		if (bottom < top) {
			bottom = top;
		}
		
		try {
			ViewGroup.LayoutParams _params = view.getLayoutParams();
			FrameLayout.LayoutParams params;
			if (_params != null && _params instanceof FrameLayout.LayoutParams) {
				params = (FrameLayout.LayoutParams)_params;
			} else {
				params = new FrameLayout.LayoutParams(right - left, bottom - top);
			}
			if (view instanceof Button) {
				top -= 5;
				bottom += 5;
			}
			params.leftMargin = left;
			params.topMargin = top;
			params.width = right - left;
			params.height = bottom - top;
			view.setLayoutParams(params);
			view.setMinimumWidth(params.width);
			view.setMinimumHeight(params.height);
			return true;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static boolean isVisible(View view) {
		return view.getVisibility() == View.VISIBLE;
	}
	
	public static void setVisible(View view, boolean flag) {
		if (flag) {
			view.setVisibility(View.VISIBLE);
		} else {
			view.setVisibility(View.INVISIBLE);
		}
	}
	
	public static boolean isEnabled(View view) {
		return view.isEnabled();
	}
	
	public static void setEnabled(View view, boolean flag) {
		view.setEnabled(flag);
	}
	
	public static Point convertCoordinateFromScreenToView(View view, int x, int y) {
		Point ret = new Point();
		int[] location = new int[2];
		view.getLocationOnScreen(location);
		ret.x = x - location[0];
		ret.y = y - location[1];
		return ret;
	}
	
	public static Point convertCoordinateFromViewToScreen(View view, int x, int y) {
		Point ret = new Point();
		int[] location = new int[2];
		view.getLocationOnScreen(location);
		ret.x = location[0] + x;
		ret.y = location[1] + y;
		return ret;
	}
	
	public static void addChild(View _group, View view) {
		try {
			if (_group instanceof ViewGroup) {
				ViewGroup group = (ViewGroup)_group;
				ViewParent parent = view.getParent();
				if (parent == group) {
					return;
				}
				if (parent != null) {
					if (parent instanceof ViewGroup) {
						((ViewGroup)parent).removeView(view);
					} else {
						return;
					}
				}
				group.addView(view);
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}

	public static void removeChild(View _group, View view) {
		try {
			if (_group instanceof ViewGroup) {
				ViewGroup group = (ViewGroup)_group;
				ViewParent parent = view.getParent();
				if (parent == group) {
					group.removeView(view);
				}
			}
		} catch (Throwable e) {
			Logger.exception(e);
		}
	}
	
	// events
	private static native void nativeOnDraw(long instance, Graphics graphics);
	public static void onEventDraw(View view, Canvas canvas) {
		long instance = getInstance(view);
		if (instance != 0) {
			Graphics graphics = new Graphics(canvas);
			nativeOnDraw(instance, graphics);
		}
	}
	
	private static native boolean nativeOnKeyEvent(long instance, boolean flagDown, int vkey
			, boolean flagControl, boolean flagShift, boolean flagAlt, boolean flagWin);
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	public static boolean onEventKey(View view, boolean flagDown, int keycode, KeyEvent event) {
		long instance = getInstance(view);
		if (instance != 0) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				return nativeOnKeyEvent(
						instance
						, flagDown, keycode
						, event.isCtrlPressed()
						, event.isShiftPressed()
						, event.isAltPressed()
						, event.isMetaPressed()
						);
			} else {
				return nativeOnKeyEvent(
						instance
						, flagDown, keycode
						, false
						, event.isShiftPressed()
						, event.isAltPressed()
						, false
						);
			}
		}
		return true;
	}
	
	private static native boolean nativeOnTouchEvent(long instance, int action, UiTouchPoint[] pts);
	public static boolean onEventTouch(View view, MotionEvent event) {
		long instance = getInstance(view);
		if (instance != 0) {
			int action = 0;
			int _action = event.getActionMasked();
			switch (_action) {
			case MotionEvent.ACTION_DOWN:
				action = 0x0301;
				break;
			case MotionEvent.ACTION_MOVE:
			case MotionEvent.ACTION_POINTER_DOWN:
			case MotionEvent.ACTION_POINTER_UP:
				action = 0x0302;
				break;
			case MotionEvent.ACTION_UP:
				action = 0x0303;
				break;
			case MotionEvent.ACTION_CANCEL:
				action = 0x0304;
				break;
			default:
				return false;
			}
			int n = event.getPointerCount();
			if (n > 0) {
				int actionIndex = event.getActionIndex();
				UiTouchPoint[] pts = new UiTouchPoint[n];
				for (int i = 0; i < n; i++) {
					UiTouchPoint pt = new UiTouchPoint();
					pt.x = event.getX(i);
					pt.y = event.getY(i);
					pt.pressure = event.getPressure(i);
					if (_action == MotionEvent.ACTION_DOWN) {
						pt.phase = 1;
					} else if (_action == MotionEvent.ACTION_UP) {
						pt.phase = 2;
					} else if (_action == MotionEvent.ACTION_CANCEL) {
						pt.phase = 3;
					} else {
						if (actionIndex == i) {
							if (_action == MotionEvent.ACTION_POINTER_DOWN) {
								pt.phase = 1;
							} else if (_action == MotionEvent.ACTION_POINTER_UP) {
								pt.phase = 2;
							} else {
								pt.phase = 0;
							}
						} else {
							pt.phase = 0;
						}
					}
					pts[i] = pt;
				}
				return nativeOnTouchEvent(instance, action, pts);
			}
		}
		return false;
	}
	
	private static native void nativeOnClick(long instance);
	public static void onEventClick(View view) {
		long instance = getInstance(view);
		if (instance != 0) {
			nativeOnClick(instance);
		}
	}

	private static native boolean nativeHitTestTouchEvent(long instance, int x, int y);
	public static boolean onHitTestTouchEvent(View view, int x, int y) {
		long instance = getInstance(view);
		if (instance != 0) {
			return nativeHitTestTouchEvent(instance, x, y);
		}
		return false;
	}
	
}
