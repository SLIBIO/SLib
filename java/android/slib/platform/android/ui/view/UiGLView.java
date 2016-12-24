package slib.platform.android.ui.view;

import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import slib.platform.android.Logger;
import slib.platform.android.ui.UiThread;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

public class UiGLView extends GLSurfaceView implements IView, GLSurfaceView.Renderer {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }

	UiGestureDetector gestureDetector;

	static Object sync = new Object();
	static Vector<UiGLView> glViewList = new Vector<UiGLView>();
	
	public static UiGLView _create(Context context) {
		try {
			UiGLView ret = new UiGLView(context);
			return ret;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static boolean _setRenderMode(final View view, final int mode) {
		if (view instanceof GLSurfaceView) {
			if (mode == 0) {
				((GLSurfaceView)view).setRenderMode(UiGLView.RENDERMODE_CONTINUOUSLY);				
			} else {
				((GLSurfaceView)view).setRenderMode(UiGLView.RENDERMODE_WHEN_DIRTY);				
			}
			return true;
		}
		return false;
	}

	public static void _requestRender(final View view) {
		if (view instanceof UiGLView) {			
			((UiGLView)view).requestRender();				
		}
	}
	

	private static native void nativeOnCreate(long instance);
	public static void onEventCreate(IView view) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnCreate(instance);
		}
	}
	
	private static native void nativeOnFrame(long instance, int width, int height);
	public static void onEventFrame(IView view, int width, int height) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnFrame(instance, width, height);
		}
	}
	
	public UiGLView(Context context) {
		super(context);
		
		setEGLContextClientVersion(2);
		setEGLConfigChooser(new MultisampleConfigChooser());
		
		setRenderer(this);
		setRenderMode(RENDERMODE_WHEN_DIRTY);
	}
	
	public boolean onKeyDown(int keycode, KeyEvent event) {
		UiView.onEventKey(this, true, keycode, event);
		return true;
	}
	
	public boolean onKeyUp(int keycode, KeyEvent event) {
		UiView.onEventKey(this, false, keycode, event);
		return true;
	}
	
	@SuppressLint("ClickableViewAccessibility")
	public boolean onTouchEvent(MotionEvent event) {		
		UiView.onEventTouch(this, event);
		return true;
	}

	public boolean dispatchTouchEvent(MotionEvent event) {
		if (gestureDetector != null) {
			gestureDetector.onTouchEvent(event);
			super.dispatchTouchEvent(event);
			return true;
		} else {
			return super.dispatchTouchEvent(event);
		}
	}
	
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		onEventCreate(this);
		synchronized (sync) {
			if (!(glViewList.contains(this))) {
				glViewList.add(this);			
			}
		}
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		widthViewport = width;
		heightViewport = height;
	}

	int widthViewport = 0;
	int heightViewport = 0;
	@Override
	public void onDrawFrame(GL10 gl) {
		if (UiThread.isUiThread()) {
			Logger.info("GL UI");			
		} else {
			Logger.info("GL Render");
		}
		onEventFrame(this, widthViewport, heightViewport);
	}
	
	
	public static void removeView(final View view) {
		if (!(UiThread.isUiThread())) {
			view.post(new Runnable() {
				public void run() {
					removeView(view);
				}
			});
			return;
		}
		if (view instanceof UiGLView) {
			synchronized (sync) {
				glViewList.remove((UiGLView)view);
			}
		}
	}
	
	public static void onPauseViews() {
		synchronized (sync) {
			try {
				for (UiGLView view : glViewList) {
					try {
						view.onPause();
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void onResumeViews() {
		synchronized (sync) {
			try {
				for (UiGLView view : glViewList) {
					try {
						view.onResume();
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}
	
}