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

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import android.webkit.JavascriptInterface;
import android.webkit.JsPromptResult;
import android.webkit.JsResult;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.EditText;

import slib.platform.android.Logger;
import slib.platform.android.SlibActivity;
import slib.platform.android.ui.helper.WebViewFileChooser;

public class UiWebView extends WebView implements IView {

	private long mInstance = 0;
	public long getInstance() { return mInstance; }
	public void setInstance(long instance) { this.mInstance = instance; }
	private int mLeft, mTop, mRight, mBottom;
	public Rect getUIFrame() { return new Rect(mLeft, mTop, mRight, mBottom); }
	public void setUIFrame(int left, int top, int right, int bottom) { mLeft = left; mTop = top; mRight = right; mBottom = bottom; }
	private boolean mStopPropagation = false;
	public boolean isStopPropagation() { return mStopPropagation; }
	public void setStopPropagation(boolean flag) { mStopPropagation = flag; }
	public boolean dispatchSuperTouchEvent(MotionEvent ev) { return super.dispatchTouchEvent(ev); }

	public static UiWebView _create(Context context) {
		try {
			initialize();
			return new UiWebView(context);
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _load(final View view, final String url) {
		if (url == null || url.length() == 0) {
			return;
		}
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.loadUrl(url);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}
	
	public static void _loadHTML(final View view, final String html, final String url) {
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				if (url == null || url.length() == 0) {
					wv.loadData(url, "text/html", "utf-8");
				} else {
					wv.loadDataWithBaseURL(url, html, "text/html", "utf-8", null);
				}
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static String _getURL(View view) {
		try {
			if (view instanceof WebView) {
				WebView wv = (WebView)view;
				return wv.getUrl();
			}	
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static String _getTitle(View view) {
		try {
			if (view instanceof WebView) {
				WebView wv = (WebView)view;
				return wv.getTitle();
			}	
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}
	
	public static void _goBack(final View view) {
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.goBack();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _goForward(final View view) {
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.goForward();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _reload(final View view) {
		if (view instanceof WebView) {
			final WebView wv = (WebView)view;
			try {
				wv.reload();
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _runJavaScript(final View view, final String script) {
		if (script != null && script.length() > 0) {
			if (view instanceof WebView) {
				final WebView wv = (WebView)view;
				try {
					wv.loadUrl("javascript:" + script);
				} catch (Exception e) {
					Logger.exception(e);
				}
			}
		}
	}

	public static void _setCustomUserAgent(final View view, final String userAgent) {
		if (view instanceof WebView) {
			final WebView wv = (WebView) view;
			try {
				wv.getSettings().setUserAgentString(userAgent);
			} catch (Exception e) {
				Logger.exception(e);
			}
		}
	}

	public static void _clearCache(Activity activity) {
		try {
			WebView wv = new WebView(activity);
			wv.clearCache(true);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static void _clearCookie(Activity activity) {
		try {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
				CookieManager manager = CookieManager.getInstance();
				if (manager != null) {
					manager.removeAllCookies(null);
					manager.removeSessionCookies(null);
					manager.flush();
				}
			} else {
				CookieSyncManager sync = CookieSyncManager.createInstance(activity);
				if (sync != null) {
					sync.startSync();
					CookieManager manager = CookieManager.getInstance();
					if (manager != null) {
						manager.removeAllCookie();
						manager.removeSessionCookie();
						sync.stopSync();
						sync.sync();
					}
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static native void nativeOnStartLoad(long instance, String url);
	public static void onStartLoad(IView view, String url) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnStartLoad(instance, url);
		}
	}
	
	private static native void nativeOnFinishLoad(long instance, String url);
	public static void onFinishLoad(IView view, String url) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnFinishLoad(instance, url);
		}
	}

	private static native void nativeOnErrorLoad(long instance, String url, String error);
	public static void onErrorLoad(IView view, String url, String error) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnErrorLoad(instance, url, error);
		}
	}

	private static native void nativeOnMessage(long instance, String msg, String param);
	public static void onMessage(IView view, String msg, String param) {
		long instance = view.getInstance();
		if (instance != 0) {
			nativeOnMessage(instance, msg, param);
		}
	}
	
	public UiWebView(Context context) {
		super(context);
		
		WebSettings webSettings = this.getSettings();
		webSettings.setJavaScriptEnabled(true);

		webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);
		webSettings.setBuiltInZoomControls(false); // Hide zoom control
		webSettings.setDisplayZoomControls(false);
		webSettings.setSupportZoom(false); // No scaling
		webSettings.setAllowFileAccess(true);
		if (Build.VERSION.SDK_INT >= 16) {
			webSettings.setAllowFileAccessFromFileURLs(true);
			webSettings.setAllowUniversalAccessFromFileURLs(true);
		}
		webSettings.setAllowContentAccess(true);
		webSettings.setDomStorageEnabled(true);
		webSettings.setDatabaseEnabled(true);
		String cacheDirPath = context.getFilesDir().getAbsolutePath()+ "/webcache";
		webSettings.setDatabasePath(cacheDirPath);
		webSettings.setAppCachePath(cacheDirPath);
		webSettings.setAppCacheEnabled(true);
		
		setWebViewClient(new MyWebViewClient());
		setWebChromeClient(new MyWebChromeClient());
		
		requestFocusFromTouch();
		
		addJavascriptInterface(new MyJavaScriptObject(), "slib");
	}

	@Override
	protected void onFocusChanged(boolean focused, int direction, Rect previouslyFocusedRect) {
		super.onFocusChanged(focused, direction, previouslyFocusedRect);
		if (focused) {
			UiView.onEventSetFocus(this);
		}
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(UiView.resolveMeasure(mRight-mLeft, widthMeasureSpec), UiView.resolveMeasure(mBottom-mTop, heightMeasureSpec));
	}

	class MyWebViewClient extends WebViewClient {
		
		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			return false;
		}
		
		public void onPageStarted(WebView view, String url, Bitmap favicon) {
			onStartLoad(UiWebView.this, url);
		}
		
		public void onPageFinished(WebView view, String url) {
			onFinishLoad(UiWebView.this, url);
		}
		
		public void onReceivedError (WebView view, int errorCode, String description, String failingUrl) {
			onErrorLoad(UiWebView.this, failingUrl, description);
		}
		
	}
	
	class MyWebChromeClient extends WebChromeClient {

		@Override
		public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("").setMessage(message).setPositiveButton(android.R.string.ok, null);
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
						return true;
					}
				});
				builder.setCancelable(false);
				AlertDialog dialog = builder.create();
				dialog.show();
				result.confirm();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}

		@Override
		public boolean onJsConfirm(WebView view, String url, String message, final JsResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("")
						.setMessage(message)
						.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,int which) {
								result.confirm();
							}
						})
						.setNeutralButton(android.R.string.no, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.cancel();
							}
						});
				builder.setOnCancelListener(new DialogInterface.OnCancelListener() {
					@Override
					public void onCancel(DialogInterface dialog) {
						result.cancel();
					}
				});
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					@Override
					public boolean onKey(DialogInterface dialog, int keyCode,KeyEvent event) {
						return true;
					}
				});
				AlertDialog dialog = builder.create();
				dialog.show();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}

		@Override
		public boolean onJsPrompt(WebView view, String url, String message, String defaultValue, final JsPromptResult result) {
			try {
				final AlertDialog.Builder builder = new AlertDialog.Builder(view.getContext());
				builder.setTitle("").setMessage(message);
				final EditText et = new EditText(view.getContext());
				et.setSingleLine();
				et.setText(defaultValue);
				builder.setView(et)
						.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.confirm(et.getText().toString());
							}

						})
						.setNeutralButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								result.cancel();
							}
						});
				builder.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,KeyEvent event) {
						return true;
					}
				});
				AlertDialog dialog = builder.create();
				dialog.show();
				return true;
			} catch (Exception e) {
				Logger.exception(e);
				return false;
			}
		}

		public boolean onShowFileChooser(WebView webView, ValueCallback<Uri[]> filePathCallback, FileChooserParams fileChooserParams) {
			fileChooser = new WebViewFileChooser(webView, SlibActivity.REQUEST_ACTIVITY_WEBVIEW_CHOOSE_FILE);
			fileChooser.chooseFile(filePathCallback, fileChooserParams);
			return true;
		}

		public void openFileChooser(ValueCallback<Uri> uploadMsg) {
			fileChooser = new WebViewFileChooser(UiWebView.this, SlibActivity.REQUEST_ACTIVITY_WEBVIEW_CHOOSE_FILE);
			fileChooser.chooseFile(uploadMsg);
		}

		public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType) {
			fileChooser = new WebViewFileChooser(UiWebView.this, SlibActivity.REQUEST_ACTIVITY_WEBVIEW_CHOOSE_FILE);
			fileChooser.chooseFile(uploadMsg, acceptType);
		}

		public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType, String capture) {
			fileChooser = new WebViewFileChooser(UiWebView.this, SlibActivity.REQUEST_ACTIVITY_WEBVIEW_CHOOSE_FILE);
			fileChooser.chooseFile(uploadMsg, acceptType, capture);
		}

	}
	
	class MyJavaScriptObject {
		
		@JavascriptInterface
		public void send(String msg, String param) {
			onMessage(UiWebView.this, msg, param);
		}
	}

	private static WebViewFileChooser fileChooser;

	private static boolean mFlagInitialized = false;

	private static synchronized void initialize() {
		if (mFlagInitialized) {
			return;
		}
		mFlagInitialized = true;
		SlibActivity.addActivityResultListener(SlibActivity.REQUEST_ACTIVITY_WEBVIEW_CHOOSE_FILE, new SlibActivity.ActivityResultListener() {
			@Override
			public void onActivityResult(Activity activity, int requestCode, int resultCode, Intent data) {
				if (fileChooser != null) {
					fileChooser.processActivityResult(resultCode, data);
					fileChooser = null;
				}
			}
		});
	}

}
