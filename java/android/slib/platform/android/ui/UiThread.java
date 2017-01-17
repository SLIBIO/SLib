package slib.platform.android.ui;

import java.lang.reflect.Method;

import android.os.Binder;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.MessageQueue;
import slib.platform.android.Logger;

public class UiThread {

	static boolean flagInitStatics = false;
	static Handler handlerUi = null;
	static Method method_MessageQueue_next;
	static Method method_Message_recycleUnchecked;
	static Runnable callbackQuitLoop = new Runnable() {
		public void run() {			
		}
	};

	synchronized static boolean initStatics() {
		
		if (flagInitStatics) {
			return true;
		}
		try {
			handlerUi = new Handler(Looper.getMainLooper());
			
			method_MessageQueue_next = MessageQueue.class.getDeclaredMethod("next");
			method_MessageQueue_next.setAccessible(true);

			try {
				method_Message_recycleUnchecked = Message.class.getDeclaredMethod("recycleUnchecked");
			} catch (Exception e) {
				method_Message_recycleUnchecked = Message.class.getDeclaredMethod("recycle");				
			}
			method_Message_recycleUnchecked.setAccessible(true);

			flagInitStatics = true;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return flagInitStatics;
		
	}

	public static boolean isUiThread() {
		return Looper.myLooper() == Looper.getMainLooper();
	}
	
	private static native void nativeDispatchCallback();
	private static Runnable callbackDispatch = new Runnable() {
		public void run() {
			nativeDispatchCallback();
		}
	};

	public static void dispatch() {
		if (!(initStatics())) {
			return;
		}
		handlerUi.post(callbackDispatch);
	}

	private static native void nativeDispatchDelayedCallback(long ptr);

	public static void dispatchDelayed(final long ptr, int delayMillis) {
		if (!(initStatics())) {
			return;
		}
		handlerUi.postDelayed(new Runnable() {
			public void run() {
				nativeDispatchDelayedCallback(ptr);
			}
		}, delayMillis);
	}

	public static void runLoop() {

		if (!(initStatics())) {
			return;
		}

		try {
			Looper looper = Looper.getMainLooper();
			if (Looper.myLooper() != looper) {
				return;
			}
			MessageQueue queue = Looper.myQueue();

			Binder.clearCallingIdentity();
			final long ident = Binder.clearCallingIdentity();

			for (;;) {
				Message msg = (Message) (method_MessageQueue_next.invoke(queue));
				if (msg == null) {
					return;
				}
				if (msg.getCallback() == callbackQuitLoop) {
					break;
				}
				try {
					msg.getTarget().dispatchMessage(msg);
				} catch (Exception e) {
				}				
				final long newIdent = Binder.clearCallingIdentity();
				if (ident != newIdent) {
					Logger.info("Thread identity changed from 0x" + Long.toHexString(ident) + " to 0x"
									+ Long.toHexString(newIdent) + " while dispatching to "
									+ msg.getTarget().getClass().getName() + " " + msg.getCallback() + " what=" + msg.what);
				}
				try {
					method_Message_recycleUnchecked.invoke(msg);
				} catch (Exception e) {
					Logger.exception(e);
				}
			}

		} catch (Exception e) {
			Logger.exception(e);
		}

	}

	public static void quitLoop() {		
		if (!(initStatics())) {
			return;
		}
		handlerUi.post(callbackQuitLoop);
	}
	
	public static void post(Runnable runnable) {
		if (!(initStatics())) {
			return;
		}
		handlerUi.post(runnable);
	}
	
}
